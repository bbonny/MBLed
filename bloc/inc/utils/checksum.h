/*
 * checksum.h
 *
 * This module calculates the checksum of a packet. The checksum used is a CRC16
 * MODBUS checksum.
 * We use the polynom 0xA001 and the initialization value is 0xFFFF.
 *
 * Copyright 2011 - Benjamin Bonny    <benjamin.bonny@gmail.com>,
 *                  Cédric Le Ninivin <cedriclen@gmail.com>,
 *                  Guillaume Normand <guillaume.normand.gn@gmail.com>          
 *
 * All rights reserved.
 * MB Led
 * Telecom ParisTech - ELECINF344/ELECINF381
 *
 * This file is part of MB Led Project 
 * This file was created by GLiP (www.glip.fr) and was modified by MB Led..
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

#ifndef _CHECKSUM_H_
#define _CHECKSUM_H_

/*
 * checksum.h
 *
 * This module calculates the checksum of a packet. The checksum used is a CRC16
 * MODBUS checksum.
 * We use the polynom 0xA001 and the initialization value is 0xFFFF.
 *
 *

 *
 */

#include "packet.h"

int calculateCRC16Packet (Packet *, int);
uint32_t calculateCRC32Data (uint32_t *data, uint8_t size);
uint32_t calculateCRC32Flash (uint32_t* start_adress, uint32_t * end_adress);

#endif
