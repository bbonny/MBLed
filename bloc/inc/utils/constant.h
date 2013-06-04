/*
 * constant.h
 *
 * Constants used by the MB Led
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

#ifndef _constant_h_
#define _constant_h_

// Size of the command packet
#define MAX_COMMAND_SIZE 8

// Size of the data packet
#define MAX_DATA_SIZE 40







#ifdef MBLED
#define NB_BLOCKS 20
#else
#define NB_BLOCKS 20
#endif

//For the modules.
#define IN 0
#define I 1
#define J 2
#define LEADER 3

#define PING_TIME 80
#define PING_ACK_TIME 40
#define PING_OUT_TIME 600
#define RESET_TIME 150
#define ELECTION_TIME 400
#define TEMP_WAIT 100
#define IRDA_WAIT 15
#define RESEND_TIME 30
#define TURN_TIME 300

#define NB_NO_PING 8
#define NB_SYNCHRO 6
#define NB_RESEND_MESS 10
#define NB_POSITION 30
#define NB_CHECK 15
 
//For broadcast!
#define EVERYBODY 0x0E // 15.
//Values for packet_transmit (cf send.c)
#define SPREAD   0x08 //means 8


#define TTL 15

#define IMAGE_SIZE 64

#endif
