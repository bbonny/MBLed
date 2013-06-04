/*
 * ping.c
 *
 * This module is used for the PING/PONG operation.
 *
 * Copyright 2011 - Benjamin Bonny    <benjamin.bonny@gmail.com>,
 *                  Cédric Le Ninivin <cedriclen@gmail.com>,
 *                  Guillaume Normand <guillaume.normand.gn@gmail.com>          
 *
 * All rights reserved.
 * MB Led
 * Telecom ParisTech - ELECINF344/ELECINF381
 *
 * This file is part of MB Led Project.
 *
 * MB Led Project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MB Led Project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MB Led Project.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 */


// FreeRTOS features
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>


#include "packet.h"
#include "ping.h"
#include "send.h"
#include "debug.h"
#include "synchro.h"
#include "algo.h"
#include "network.h"
#include "irda.h"
#include "position.h"
#include "graph.h"
#include "election.h"
#include "turn.h"


static Packet xPacketPingAck[4];
static xSemaphoreHandle xPingSemaphore[4], xPingAckSemaphore[4];

extern xQueueHandle xPongQueue;
extern xQueueHandle xElectionQueue;
xQueueHandle xPingQueue[4];

//If the block isn't awareOfMissingBlock, he will copy the interfaces when someone leaves.
extern uint8_t awareOfMissingBlock;

//If sendPing = 0, the block won't send PING and PONG during PING_OUT_TIME.
static uint8_t sendPing;


//Initialisation of the semaphore and of the queue.
void ping_init(){
  for(int interface=0; interface <4; interface++)
    {
      vSemaphoreCreateBinary(xPingSemaphore[interface]);
      vSemaphoreCreateBinary(xPingAckSemaphore[interface]);
      xSemaphoreTake(xPingAckSemaphore[interface], portMAX_DELAY);     
      xPingQueue[interface] = xQueueCreate(4, sizeof(Packet));
    }
  ping_restart();
}
 

void ping_restart(){
   sendPing = 1;
}



//No ping/pong for approximatively PING_OUT_TIME ms.
void setPingOut(){
  sendPing = 0;
  vTaskDelay(PING_OUT_TIME + (getMyID() % 20) );
  sendPing = 1;
}

//Send PING to an interface. 
//Gives the ping ack semaphore of this very interface (in order to check the acknowledgement of the ping)
//Waits for the ping semaphore of this very interface (given by the ping ack at the end of his task)

void ping_task(void* pvParameters){

  unsigned int uart = (unsigned int) pvParameters;
  uint8_t interface = uart2interface(uart);

  while(1){
    vTaskDelay(PING_TIME + (getMyID() % 20)  );
  
    xSemaphoreTake(xPingSemaphore[interface], portMAX_DELAY);
    if(sendPing == 1)
      SEND_PING(interface);
    xSemaphoreGive(xPingAckSemaphore[interface]);    
    
   }
}

//Send PONG in order to answer a PING message.
void pong_task(void *parameters){
  Packet packet;
  uint8_t data[2];
  while(1){
    xQueueReceive(xPongQueue, &packet, portMAX_DELAY);

    data[0] = getMembersInNetwork();
    data[1] = packet.interface;
    
    if(sendPing == 1)
      send_new_packet(PONG,data, packet.interface);
  }
} 


void leader_task(void *parameters){
  uint8_t send_synchro = 0;

  while(1){
    vTaskDelay(PING_TIME);
    
    send_synchro++;
    //Every NB_CHECK, the block checks his graph and then send his position to everyone.
    //And his neihbors.
    if((send_synchro % NB_CHECK) == 0){
      graph_check_neighbor();
      network_send_my_position(EVERYBODY);
      network_send_neighbors();
    }
    
    //The rest is only for the leader.
    if(!isLeaderOfANetwork()){
      continue;
    }
    
    //The leader sends the TIME every NB_SYNCHRO pings to synchronize everybody!    
    if((send_synchro%NB_SYNCHRO) == 0){
      send_time(EVERYBODY);
    }

    //Every NB_POSITION messages, the leader sends the direction again.
    if((send_synchro % NB_POSITION) == 0){
      BROADCAST_POSITION;
    }
  }
}


//After sending a PING message, the block checks if there is an answer or not. If there isn't any answer, maybe the neighbor has gone (if there was a neighbor).
//If there is an answer, maybe it's a new neighbor or maybe it's someone who turned.
void ping_acq_task(void* pvParameters){


  unsigned int uart = (unsigned int) pvParameters;
  uint8_t interface = uart2interface(uart);
  uint8_t nbNoAnswer = 0;
  uint8_t membersInHisNetwork, hisInterface;
  uint8_t neighbor;

  while(1){ 

    //The PING task gives the PingAckSemaphore after sending a PING.
    xSemaphoreTake(xPingAckSemaphore[interface], portMAX_DELAY);
    //There is an answer to my PING in less than PING_ACK_TIME
    if(xQueueReceive(xPingQueue[interface], &xPacketPingAck[interface], PING_ACK_TIME) == pdTRUE){

      nbNoAnswer = 0;
      //Reading the PONG packet.
      neighbor = xPacketPingAck[interface].from;
      membersInHisNetwork = xPacketPingAck[interface].data[0];
      hisInterface = (int8_t) xPacketPingAck[interface].data[1];

      //This message isn't acked so we have to check the correctness of the value.
      if(!interface_ok(hisInterface))
	continue;

      //Nobody was on this UART : it's a new neighbor!
      if(!interface_is_busy(interface)){
	ping_new_neighbor(neighbor, membersInHisNetwork, interface, hisInterface);
      }
      //Not the good interface : maybe the neighbor has turned.
      else if(hisInterface != interface_hisInterface(interface) ){
	turn_function(neighbor, membersInHisNetwork, interface, hisInterface);
      }

    }
    //There isn't any answer.
    else{
      ping_no_answer(&nbNoAnswer, interface); 
    }

    xSemaphoreGive(xPingSemaphore[interface]);
      
  }

}

/*
Two cases : 
* A module is coming back.
* A new module is coming.
*/

void ping_new_neighbor(uint8_t block, uint8_t membersInHisNetwork, uint8_t interface, uint8_t hisInterface){

  //The table wasn't in the interfaces.
  if(!ID_in_interfaces(block) && !ID_was_in_interfaces(block)){

    //He is in the interface and we check if we need an election.
    setInterface(interface, block, hisInterface);
    ping_decision(block, membersInHisNetwork, interface);
  }
  //He was in the interfaces : maybe he turned.
  else{
    turn_function(block, membersInHisNetwork, interface, hisInterface);
  }
}

//Ping_decision compares the nb of members in our network and in the neighbor's network.
//If we are larger than them, we send the networK.
//Else if it is equal, we have to start an election... 
void ping_decision(uint8_t neighbor, uint8_t membersInHisNetwork, uint8_t interface){
  
  int8_t membersInMyNetwork = getMembersInNetwork();
  
  if(membersInHisNetwork < membersInMyNetwork){
    election_send_network(neighbor, membersInHisNetwork, interface);
  }

  else if(membersInHisNetwork == membersInMyNetwork){

    for(int i=0; i<4; i ++){
      if(!block_in(i)){
	//Starting an election.
	uint8_t a = 0;
	xQueueSend(xElectionQueue, &a,0);
	break;
      }
    }
  }
}


//There is no answer from the PING.
void ping_no_answer(uint8_t* nbNoAnswer, uint8_t interface){
  
  int8_t neighbor = interface_neighbor(interface);

  //There is no neighbor, so it's logical.
  if(neighbor == -1 )
    return;

  //There was someone few seconds ago, so nbNoAnswer increases.
  (*nbNoAnswer)++;

   //When we have no news for a long time, the module is gone.
  if((*nbNoAnswer) == NB_NO_PING){

    //the first interface to have no answer will copy the interfaces.
    if(!awareOfMissingBlock)
      interfaces_copy();

    setInterface(interface, -1, -1);
    //Throwing away the IrDA queues for this UART.
    irda_reset_queues(interface2uart(interface));
    
    //Maybe he has just turned so we check...
    if(!ID_in_interfaces(neighbor)) {
      network_reset(neighbor, interface);
    }

    (*nbNoAnswer) = 0;
  }
   
}



