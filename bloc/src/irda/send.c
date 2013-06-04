/*
 * send.c
 *
 *  This module is the link between the algorithm and the IrDA. With theses functi\
ons, you are able to send data via IrDA.  
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


// FreeRTOS and STM32 features
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

// MB Led features
#include "constant.h"
#include "debug.h"
#include "checksum.h"
#include "send.h"
#include "algo.h"
#include "synchro.h"



void send_new_command(int8_t mode,  uint8_t * data, uint8_t interface, uint8_t size_of_data, uint8_t ttl){


  Packet new_packet;
  packet_init(&new_packet);
  int8_t myID = getMyID();

  packet_build(&new_packet, 0,myID, interface
	       ,ttl,mode
	       ,size_of_data, data);

  send_to(&new_packet, interface);

}

//Classics packets
void send_new_packet(uint8_t mode,  uint8_t * data, uint8_t interface){


  Packet new_packet;
  packet_init(&new_packet);
  int8_t myID = getMyID();
  
  uint8_t ttl, size_of_data;


  switch(mode){
    //PING packet is : (void)
  case PING:
       packet_build(&new_packet,0,myID, EVERYBODY
    		 ,1,PING
    		 ,0, new_packet.data);

       new_packet.interface = interface;
       packet_transmit(&new_packet, interface2uart(interface));
    return;  

    //PONG packet is : MEMBERS_IN_MY_NETWORK / MY_INTERFACE
  case PONG:
    packet_build(&new_packet, 0,myID, interface
		 ,1,PONG
		 ,2, data)   ;  
    
    packet_transmit(&new_packet, interface2uart(interface));
    return;

    //ACK packet is : PACKET_FROM / PACKET_ID
  case ACK:
    packet_build(&new_packet,0,myID, interface
		 ,1,ACK
		 ,2,data);
    packet_transmit(&new_packet, interface2uart(interface));
    return;

    //START packet is :(void)
  case START:
    ttl = 1;
    size_of_data = 0;
    break;

    //CANDIDATE packet is: LEADER_ID / MEMBERS_IN_MY_NETWORK
  case CANDIDATE:
    ttl = TTL;
    size_of_data = 2;
    break;
    //REJECTED packet is: LEADER_ID / MEMBERS_IN_MY_NETWORK
  case REJECTED:
    ttl = TTL;
    size_of_data = 2;
    break;  
    //ELECETED packet is: LEADER_ID / MEMBERS_IN_MY_NETWORK
 case ELECTED:
    ttl = TTL;
    size_of_data = 2;
    break;

    //NETWORK packet is : SOME_BLOCK_ID BLOCK_X BLOCK_Y BLOCK_LEADER_ID
  case NETWORK:
    ttl = TTL;
    size_of_data = 4;
   break;

   //This command is different from the others.
   //POSITION packet is : DIRECTION_ID/LEADER_ID/MY_DIRECTION/YOUR_I/YOUR_J/MY_INTERFACE.
  case POSITION:
    ttl = data[6];
    size_of_data = 6;
    break;

    //TURN packet is : ROTATION
  case TURN:
    ttl = TTL;
    size_of_data = 1;
    break;
    //TIME packet is : TIME( on 32 bits) / SYNCHRO_ID / MEMBERS_IN_MY_NETWORK
  case TIME:
    ttl = TTL;
    size_of_data = 6;
    break;

    //MISSING packet is : MISSING_BLOCK_ID
  case MISSING:
    ttl = TTL;
    size_of_data = 1;
    break;

  default:
    ttl = TTL;
    size_of_data = 0;
    break;
  }

  packet_build(&new_packet, 0,myID, interface
	       ,ttl,mode
	       ,size_of_data, data);

   send_to(&new_packet, interface);
}



void send_time(int8_t interface){


  if(isLeader()){
    addSynchroID();
  }
  Packet new_packet;
  packet_init(&new_packet);

  uint8_t data[6];
  
  *(uint32_t *)&data[0] = current_time();
  data[4] = getSynchroID();
  data[5] = getMembersInNetwork();

  packet_build(&new_packet, 0,getLeaderID(), interface
	       ,TTL,TIME
	       ,6, data);

  send_to(&new_packet, interface);

}



void send_broadcast(Packet * packet){
   packet->to = EVERYBODY;
  
 
  for(int interface=0; interface < 4; interface++){

    if(interface_is_busy(interface)){
      packet->interface = interface;
      packet_transmit(packet,interface2uart(interface));
    }
  }

 
}

void send_spread(Packet * packet, uint8_t badInterface){
 

  if(packet->from != getMyID()){
    packet->ttl = packet->ttl - 1;
    calculateCRC16Packet (packet, 0);
  }

 for(int interface=0; interface<4; interface++){
   if(interface_is_busy(interface) == 1){
      if(interface != badInterface){
	packet->interface = interface;
	packet_transmit(packet,interface2uart(interface));
      }
    }
  }


}


void send_to(Packet *  packet, uint8_t interface){

  if(interface == EVERYBODY){
    send_broadcast(packet);
  }

  else if((interface >= SPREAD) && (interface < SPREAD + 4)){
    send_spread(packet, (interface - SPREAD));
  }

  else{

    if(interface_is_busy(interface))  
      packet_transmit(packet, interface2uart(interface));
  }
  vTaskDelay(IRDA_WAIT);
}




