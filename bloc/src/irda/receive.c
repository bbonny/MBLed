/*
 * receive.c
 *
 * This module handles the reception of an IrDA packet.
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
#include "irda.h"
#include "constant.h"
#include "debug.h"
#include "checksum.h"
#include "receive.h"
#include "send.h"
#include "turn.h"
#include "synchro.h"
#include "election.h"
#include "network.h"
#include "position.h"
#include "ping.h"
#include "algo.h"
#include "snake.h"

#include "manager.h"
#include "command.h"


extern xQueueHandle xQueuePacket;
static Packet packet ;


/*
 *
 * This function gives the behaviour of the block when it receives a new
 * packet in the queue.
 *
 */

void irda_receive () {

 
  for ( ;; ) {

  
    xQueueReceive (xQueuePacket, &packet, portMAX_DELAY);


    switch(packet.mode)
      {

	//Election packet!
      case(START):
	election_main(&packet);
	break;
         
      case(CANDIDATE):
	election_main(&packet);
	break;
	
      case(REJECTED):
	election_main(&packet);
	break;
        
      case(ELECTED):
	election_main(&packet);
	break;  
      case(NETWORK):
	network_receive(&packet);
	break;
      case(POSITION):
	position_receive(&packet);
	break;
      case(TURN):
	turn_receive(&packet);
	break;
      case(TIME):
	synchro_update(&packet);
	break;
      case(MISSING):
	network_missing_receive(&packet);
	break;
      case(MANAGER):
	manager_receive_command(&packet);
	break;
      case(COMMAND):
	command_receive(&packet);
	break;

	//      case(MANAGER_NORTH):
	//position_remote_north(&packet);
	//	break;
      default:
	break;
      }
  }
}


