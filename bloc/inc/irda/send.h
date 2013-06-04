/*
 * send.h
 *
 * Define packets' type and ways to send them
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

#ifndef H_SEND
#define H_SEND
#include "packet.h"



#define PING          0x00
#define START         0x11
#define NEEDINFO      0x30

#define ACK           0x80
#define PONG          0x81
#define TIME          0x82

#define CANDIDATE     0x92
#define REJECTED      0x93
#define ELECTED       0x94
#define IAMLEADER     0x95

#define POSITION      0xA0
#define MISSING       0xA1
#define TURN          0xA2

#define NETWORK       0xB0

#define COMMAND       0xC0
#define MANAGER       0xC1
//#define MANAGER_NORTH  0xC2



#define CMD_DATA      0xFF


#define ID_ADDR (*(volatile unsigned long *) 0x1FFFF7F0)



#define SEND_PING(x)      send_new_packet(PING, (uint8_t *) NULL, x)
#define BROADCAST_POSITION     position_spread(-1, TTL)

void send_new_command(int8_t mode,  uint8_t * data, uint8_t interface, uint8_t size_of_data, uint8_t ttl); 

void send_new_packet(uint8_t mode, uint8_t * data,  uint8_t interfaces);
void send_broadcast(Packet * packet);
void send_spread(Packet * packet, uint8_t uart);
void send_to(Packet * packet, uint8_t to);
void send_time(int8_t interface);

#endif
