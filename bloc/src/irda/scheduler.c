/*
 * scheduler.c
 *
 * ( descripts the module here ) 
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
#include <semphr.h>

// MB Led features
#include "irda.h"
#include "constant.h"
#include "debug.h"
#include "checksum.h"
#include "receive.h"
#include "send.h"
#include "scheduler.h"
#include "algo.h"


extern xQueueHandle xQueueSendIrDA[4],xQueueSendIrDABuffer[4];
extern xQueueHandle xQueueAck[4];


/****************************************************************************/
/*          Task managing acknoledgmenet                                    */
/*          Takes packet in queue and wait an acknoldgement                 */
/*          If an ack does not come after NB_RESEND_MESS cycles             */
/*          The packet is drop                                              */
/****************************************************************************/

void irda_ack (void * pvParameters){
  portBASE_TYPE xStatus;
  unsigned int uartn = (unsigned int) pvParameters;
  Packet output_packet;
  ID_PACKET ack_packet;
  uint8_t numberOfTry = 0 ;

  ack_packet.from=0;
  ack_packet.idPacket =0;
  packet_init(&output_packet);      
 
  xQueueReceive(xQueueSendIrDA[uartn-IRDA_MIN], &output_packet, portMAX_DELAY);
  for (;;){
    numberOfTry++;
    xQueueSend(xQueueSendIrDABuffer[uartn-IRDA_MIN], &output_packet, 0);
   

    xStatus= xQueueReceive(xQueueAck[uartn-IRDA_MIN]
			   , &ack_packet,RESEND_TIME + (getMyID() % 20 ));
     
    if( pdPASS == xStatus)
      {

	//Success of the ACK.
	if ((ack_packet.from == output_packet.from)
            && (ack_packet.idPacket == output_packet.idPacket))
	  {
	    numberOfTry = 0;
	    
	    packet_init(&output_packet);      
	    xQueueReceive(xQueueSendIrDA[uartn-IRDA_MIN]
			  , &output_packet, portMAX_DELAY);
	  }
      }  
    if(numberOfTry == NB_RESEND_MESS) { 
      packet_init(&output_packet);      
      xQueueReceive(xQueueSendIrDA[uartn-IRDA_MIN], &output_packet, portMAX_DELAY);
      numberOfTry = 0;
    }
  }
}
