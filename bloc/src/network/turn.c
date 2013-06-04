/*
 * turn.c
 *
 * This module detects rotation of blocks after a PING/PONG and sends the information.
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


#include <stm32f10x.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include "algo.h"
#include "packet.h"
#include "turn.h"
#include "send.h"
#include "election.h"
#include "debug.h"
#include "ping.h"


#define NOT_AWARE 0
#define AWARE    1
#define AWARE_AND_CHECK 2
 
static xSemaphoreHandle turnSemaphore;
xSemaphoreHandle turn180Semaphore;

xQueueHandle turnQueue; // queue used to notice the current game/animation/task
static int8_t awareOfTurn; //During TURN_TIME, the block is aware of turn so he can't turn again.
static Interfaces saveOfMyInterfaces;


//Initialization of the turn semaphores and queues.
void turn_init(){
  vSemaphoreCreateBinary(turnSemaphore);
  vSemaphoreCreateBinary(turn180Semaphore);
  xSemaphoreTake(turn180Semaphore,portMAX_DELAY);
  turnQueue = xQueueCreate(1,sizeof(int8_t));
  awareOfTurn = NOT_AWARE;
}


//When the block turns, it waits TURN_TIME before being able to notice another turn.
void turn_task(void* parameters){
  for(;;){
    xSemaphoreTake(turnSemaphore,portMAX_DELAY);
    vTaskDelay(TURN_TIME);
    awareOfTurn = NOT_AWARE;
    saveOfMyInterfaces = getInterfaces();
  }
}


//Sending the TURN message to the network.
void turn_send(int8_t rotation){
  uint8_t data;
  xQueueSend(turnQueue, ( void * )&rotation, 0); // put the rotation into the queue
  data = (uint8_t) rotation;
  send_new_packet(TURN, &data, EVERYBODY); // tell to the neighbors that the block turned 
  addDirection(( 4 - rotation));
}


//Receiving a TURN message.
void turn_receive(Packet * packet){
  uint8_t rotation;
  rotation = packet->data[0];   
  xQueueSend(turnQueue, ( void * )&rotation, 0); // put the rotation into the queue  
}



//Function to notice the turn of the block.
void turn_function(uint8_t neighbor, uint8_t membersInHisNetwork, uint8_t interface, uint8_t hisInterface){

    int8_t rotation ;
    int oldInterface;
    int hasLeft;
    
    //The first interface to notify the change of interfaces will save the interfaces.
    if(awareOfTurn == NOT_AWARE){
      awareOfTurn = AWARE;
      xSemaphoreGive(turnSemaphore);
      
      if(ID_in_interfaces(neighbor))
	saveOfMyInterfaces = getInterfaces();
      else if(ID_was_in_interfaces(neighbor)){
	saveOfMyInterfaces = getSavedInterfaces();
      }
      //Shouldn't be here.
      else
	return;
    }  
    
    //Two options:
    //He was in our network few times ago. (really few)
    if(ID_in_interfaces(neighbor)){
      oldInterface = interface_where_is_neighbor(neighbor,1);
      hasLeft = 0;
    }
    //He quitted and then came again.
    else{
      oldInterface = interface_where_is_neighbor(neighbor, 0);
      hasLeft = 1;
    }
    
    //Changing the old interface
    if(oldInterface != -1){
      setInterface(oldInterface, -1,-1);
    }

    //Setting the new parameters to this interface.
    setInterface(interface, neighbor, hisInterface);  

    //Again, the first interface to notify will calculate the rotation.
    if(awareOfTurn == AWARE){
      awareOfTurn = AWARE_AND_CHECK;
      //Calculate differences between the interfaces and the saved ones.
      rotation = interfaces_turn(saveOfMyInterfaces, getInterfaces() );
    
      //The block has left so we decide if we need an election.
      if(hasLeft){
	ping_decision(neighbor, membersInHisNetwork, interface);
      }

      if(rotation > 0){
	// if it's 180 degre rotation then give the semaphore wich wil be used by the switcher in manager.c
	if((rotation!=1) && (rotation!=3) )
	  xSemaphoreGive(turn180Semaphore);
	else
	  turn_send(rotation);
      }
      else if(rotation == 0){ }
      else{
	//Maybe another interface will change in few times
	//So, we wait and check again.

	vTaskDelay(3*PING_ACK_TIME);
	rotation = interfaces_turn(saveOfMyInterfaces, getInterfaces());
	if(rotation > 0){
	   if((rotation!=1) && (rotation!=3) )
	     xSemaphoreGive(turn180Semaphore);
	   else
	     turn_send(rotation);
      	}

      }
    }
}



