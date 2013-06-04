/*
 * packet.c
 *
 * This module creates packet and sends it via an UART
 *
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
#include <stm32f10x.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

// MB Led features
#include "packet.h"
#include "irda.h"
#include "checksum.h"
#include "debug.h"
#include "send.h"
#include "synchro.h"
#include "algo.h"




static uint8_t packet_ID;



extern xQueueHandle xQueueSendIrDA[4],xQueueSendIrDABuffer[4], xQueueSendQuick[4];
uint16_t irda_out;
extern uint16_t irda_in;
Packet output_buffer[4];


void setPacketID(uint8_t ID){
  packet_ID = ID;
}

uint8_t getPacketID(){
  return packet_ID;
}

uint8_t updatePacketID(){
  packet_ID++;
  return packet_ID;
}

/*****************************************************************************
                            Function to initialize packet
*****************************************************************************/


void packet_init (Packet * packet) {

  packet->idPacket     = 0;
  packet->priority     = 0;
  packet->from         = 0;
  packet->to           = 0;
  packet->ttl          = 0;
  packet->mode         = 0;
  packet->checksum     = 0;
  packet->size_of_data = 0;
  packet->interface         = 0;
  for (int i =0;i< MAX_DATA_SIZE; i++)
    packet->data[i]=0;
}


/****************************************************************************
 *
 * This function fills a packet with :
 *     - its new id : idPackets
 *     - the transmitter : from
 *     - the ttl : ttl
 *     - the mode : see the constant.h file to view availabled modes
 *     - the data.
 * Next, it calculates the checksum of the packet and put it in it.
 *
 ***************************************************************************/

void packet_build (Packet * packet, uint16_t idPacket, uint8_t from, uint8_t interface,
		   uint8_t ttl, uint8_t mode, uint8_t size_of_data,
		   const uint8_t * data)
{
  if(idPacket == 0){
    packet->idPacket = packet_ID;
    packet_ID++;
  }
  else{
    packet->idPacket = idPacket;
  }
  packet->from      = from;
  packet->ttl       = ttl;
  packet->mode      = mode;
  if((interface >= 0) & (interface < 4)){
    packet->to        =  interface_neighbor(interface);
  }
  else 
    packet->to = interface;
  packet->interface      = interface;
  if (mode | 0x7F){
    packet->size_of_data = size_of_data;
    memcpy(packet->data,data, size_of_data);
  }
  calculateCRC16Packet (packet, 0);
  
}

/*****************************************************************************
          Send a packet through the specified interface     
*****************************************************************************/
void packet_transmit (Packet * packet, int uart) {
  
  if(uart == -1 || uart == 0xFF){
    writePacketDebug(packet);
  }


  if(packet->ttl > 0){
    
    if(packet->mode == PING
       || packet->mode == PONG
       || packet->mode == ACK
       || packet->mode == TIME){ 
      xQueueSend(xQueueSendQuick[uart-IRDA_MIN] , packet, 0) ;
    }
    else{
      xQueueSend(xQueueSendIrDA[uart-IRDA_MIN] , packet , 0 );  
    }
  }
}



//xSemaphoreTake (xSemaphoreTransmit, portMAX_DELAY);
void packet_send_IrDA (void *pvParameters){
  portBASE_TYPE xStatus;
  irda_out = 0;
  Packet packet;
  //uint8_t counter;
  unsigned int uart = (unsigned int) pvParameters;
  // A packet comes out...
  packet_init(&packet);
  for (;;){

    xStatus= xQueueReceive(xQueueSendQuick[uart-IRDA_MIN], &packet, 0); 
    if(xStatus == pdFALSE)
      xStatus =xQueueReceive(xQueueSendIrDABuffer[uart-IRDA_MIN], &packet, 1/portTICK_RATE_MS); 
    
    if(xStatus == pdPASS){
     packet_show(&packet, 1);
      xSemaphoreTake(xSemaphoreIrDA[uart-IRDA_MIN],portMAX_DELAY);
      irda_write_data ("MBL", uart, 3); 
      irda_write_char (packet.from, uart);
      irda_write_char (packet.idPacket, uart);
      irda_write_char (((char )(packet.priority << 3)|packet.to), uart);
      irda_write_char ((packet.ttl), uart);
      irda_write_char ((packet.mode), uart);
      if ( (     (((packet.mode) >> 7)& 0x01))== 1){
	irda_write_char(packet.size_of_data, uart);
	irda_write_data((const char *) packet.data, uart, packet.size_of_data);
      }
      irda_write_short(packet.checksum, uart);
      xSemaphoreGive (xSemaphoreIrDA[uart-IRDA_MIN]);
      packet_init(&packet);
      vTaskDelay(5);
      irda_out++;
    }

  }
}



/*****************************************************************************
                       Display function for a packet     
*****************************************************************************/
#ifndef GLIP

void packet_show(Packet * packet, int send){

 
  if(packet->mode == PING){
    if(packet->from == getMyID()){
      if(!PING_SEND)
	return;
      }
    else{
      if(!PING_RECEIVE)
	return;
    }
 
  }
  else if(packet->mode == PONG){


    if(packet->from == getMyID()){
      if(!PONG_SEND)
	return;
    }
    else{
      if(!PONG_RECEIVE)
	return;
    }
		 
  }


 else{ 
  if((!ACK_SHOW) & (packet->mode == ACK))
    return;
  if((!TIME_SHOW) & (packet-> mode == TIME))
   return;
  if(packet->from != getMyID()){
    if(!RECEIVE_TALK)
      return;
  }
  else{
    if(!SEND_TALK)
      return;
      }  
}  
 
  if(send)
    writeDebugInit("S :");
  else
    writeDebugInit("R :");
   

  char mode[15];
  int size;
  switch(packet->mode){
  case(PING):
    memcpy(mode,"PING",4);
    size = 4;
    break;
  case(PONG):
    memcpy(mode,"PONG",4);
    size =4;
    break;
  case(START):
    memcpy(mode,"START",5);
    size = 5;
    break;
  case(CANDIDATE):
    memcpy(mode,"CANDIDATE",9);
    size = 9;
    break;
  case(REJECTED):
    memcpy(mode,"REJECTED",8);
    size = 8;
    break;
  case(ELECTED):
    memcpy(mode,"ELECTED",7);
    size = 7;
    break;
  case(NETWORK):
    memcpy(mode,"NETWORK",7);
    size = 7;
    break;
  case(POSITION):
    memcpy(mode,"POSITION",8);
    size = 8;
    break;
  case(TURN):
    memcpy(mode,"TURN",4);
    size=4;
    break;
 case(ACK):
    memcpy(mode,"ACK",3);
    size=3;
    break;
  case(CMD_DATA):
    memcpy(mode, "DATA", 4);
    size=4;
    break;
  case(TIME):
    memcpy(mode, "TIME", 4);
    size = 4;
    break;
  case MISSING:
    memcpy(mode, "MISSING",7);
    size=7;
    break;
  case COMMAND:
    memcpy(mode, "COMMAND", 7);
    size=7;
    break;
  default:
    memcpy(mode, "UNKNOWN",7);
    size = 7;
    break;
    }
  mode[size] = '\0';

  writeHexDebug(packet->from);
  writeDebugInit(" -> ");
  writeHexDebug(packet->to);
  writeDebugInit(" - I ");
  writeHexDebug(packet->interface);
  writeDebugInit(mode);
  for(int i=0; i < packet->size_of_data; i++)
    writeHexDebug(packet->data[i]);
  writeDebugInit(" [ID ");
  writeHex16Debug(packet->idPacket);
  writeDebugInit(" / TTL ");
  writeHexDebug(packet->ttl);
  writeDebugInit("] \r\n");
 
}

#else
//No debug for GLiP
void packet_show(Packet * packet, int send){}
#endif
