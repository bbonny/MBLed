/*
 * debug.h
 *
 * Functions for the debug
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

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "packet.h"
/*
 * debug.h
 *
 * This file is used for the management of the USART1 as a serial debug port.
 * It is also used by the master block which is commanded through a serial port.
 * The functions above configures this port correctly and offers the possibility
 * of writing characters, numbers and strings.
 * There is a task which read the message received on USART1 and which
 * interprets these message as command if possible.
 *
 */


void initDebug ();
void writeDebug (const char *);
void writeDebugSend ();
void writeDebugInit (const char *);
void writeCharDebug (char);
void writeHexDebug(unsigned char);
void writeHex16Debug(uint16_t);
void writeHexStrDebug(const char *prefix,
           const char *str, unsigned int length);
void writePacketDebug (Packet* packet);
void receiveDebug();
void writeHex32Debug(uint32_t c);
void writeHex64Debug(uint64_t c);


#define PING_SEND 0
#define PING_RECEIVE 0
#define PONG_SEND 0
#define PONG_RECEIVE 0

#define ACK_SHOW 0
#define TIME_SHOW 0

#define RECEIVE_TALK 0
#define SEND_TALK 0

#ifdef GLIP
#define IRDA_MIN 1
#define IRDA_MAX 4
#endif
#ifdef GLIP_DEBUG
#define IRDA_MIN 2
#define IRDA_MAX 4
#endif
#ifdef MBLED
#define IRDA_MIN 2
#define IRDA_MAX 5
#endif


#endif
