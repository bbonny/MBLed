/*
 * irda.h
 *
 * This module initializes the IrDA and sends packets too.
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

#ifndef H_IRDA
#define H_IRDA

#include <semphr.h>

#include "packet.h"
#include "constant.h"


extern xSemaphoreHandle xSemaphoreIrDA[4];


/*****************************************************************************
                  Declaration of the differents states   
*****************************************************************************/
typedef enum STATE{
  STATE_IDLE        = -1,
  STATE_FROM        =  0,
  STATE_ID          =  1,
  STATE_PRIORITY_TO =  2,
  STATE_TTL         =  3,
  STATE_COMMAND     =  4,
  STATE_DATA_SIZE   =  5,
  STATE_GET_DATA    =  6,
  STATE_CHECKSUM1   =  7,
  STATE_CHECKSUM2   =  8
} STATE;

/*****************************************************************************
                            Initializations Functions                               
*****************************************************************************/

void irda_config(int n) ;
void irda_init () ;

/*****************************************************************************
         Function to reset queues in case a neighbour leave     
*****************************************************************************/
void irda_reset_queues(unsigned int n);

/*****************************************************************************
                      Function handling arriving characters                           
*****************************************************************************/


void irda_read (void *pvParameters);


/*****************************************************************************
      Function constructing packets thanks to a state machine
*****************************************************************************/

void irda_new_state (Packet* packet, STATE* state , int index, char begin[], char c,unsigned int uart);


/*****************************************************************************
                       IrDA sending functions
*****************************************************************************/

void irda_write_char (char c, unsigned int nUSART);
void irda_write_short (unsigned short s, unsigned int nsUART);
void irda_write_data (const char *s, unsigned int nUSART, unsigned int size);


/*****************************************************************************
                            helpfull functions and structures    
*****************************************************************************/

void irda_data_init (uint8_t* t, short c);


// pair of number allowing to idenfity a packet
typedef struct ID_PACKET{
  
  // ID of the sender
  uint8_t from;
  
  // ID of packet
  uint8_t idPacket;

} ID_PACKET;


void irda_input_buffer_init ();
#endif
