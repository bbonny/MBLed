/*
 * packet.h
 *
 * This module creates packet and sends it via an UART
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

#ifndef _PACKET_H_
#define _PACKET_H_


#include "constant.h"
/*
 * packet.h
 *
 * This module contains all the necessary for the packet management.
 * There are functions for :
 *     - creation
 *     - initialization
 *     - transmission
 * Be careful to see the constant for the MAX_SIZE of the commands and
 * data.
 *
 */

// MB Led features
#include <stdint.h>

// Definition of the packet structure
typedef struct Packet {

  // ID of the sender
  uint8_t from;

  // ID of the packet : Number of the packet
  uint8_t idPacket;

  // Priority of the packet : 0 the lowest, 7 the highest
  uint8_t priority;

  // ID of destination : 0 to neighboor, 3 to the leader, 15 broadcast
  uint8_t to;

  // Time To Live
  uint8_t ttl;

  // Command (first bit account for the presence of Data)
  uint8_t mode;

  // Size of Data
  uint8_t size_of_data;

  // Data of the packet
  uint8_t data[MAX_DATA_SIZE];

  uint8_t interface;

  // Checksum CRC16
  uint16_t checksum;

 
} Packet;


void setPacketID(uint8_t ID);
uint8_t getPacketID();
uint8_t updatePacketID();

void packet_init (Packet*);

void packet_build (Packet * packet, uint16_t idPacket, uint8_t from,
		   uint8_t interface, uint8_t ttl, uint8_t mode, uint8_t size_of_data,
		   const uint8_t * data);

void packet_transmit (Packet *, int);
void packet_send_IrDA (void *pvParameters);
void packet_show(Packet *packet, int send);

#endif
