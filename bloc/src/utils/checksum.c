/*
 * checksum.c
 *
 * This module calculates the checksum of a packet. The checksum used is a CRC16
 * MODBUS checksum.
 * We use the polynom 0xA001 and the initialization value is 0xFFFF.
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
 * This file was created by GLiP (www.glip.fr) and was modified by MB Led.
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



#include <stm32f10x.h>
#include <stdio.h>
#include <stdint.h>

// MB Led features
#include "checksum.h"
#include "constant.h"


static void calculateCRC16_Modbus (unsigned char val, uint16_t *crc16) {
  *crc16 ^= val;

  for (unsigned int i=0; i<8; i++) {
    unsigned int carry = (*crc16) & 0x0001;
    *crc16 >>= 1;
    if (carry)
      *crc16 ^= 0xA001;
  }
}

// Macro for adding one or two bytes to the checksum
#define CRC16(x)  calculateCRC16_Modbus ((x), &crc16)
#define CRC16_2(x) do {calculateCRC16_Modbus ((x)&0xff, &crc16); calculateCRC16_Modbus (((x)>>8)&0xff, &crc16); } while(0)


/*
 *
 * This function calculate the checksum of a packet.
 *
 * If check is set, it adds the current checksum and checks if the returned value is 0
 * else it puts the computed checksum in the checksum field of the packet.
 *
 */
int calculateCRC16Packet (Packet * packet, int check) {

  // Initialization of the crc
  uint16_t crc16 = 0xFFFF;

  CRC16(packet->from);
  CRC16(packet->idPacket);
  // CRC16(packet->priority);
  // CRC16(packet->to);
  CRC16(packet->ttl);
  CRC16(packet->mode);
  if ( (     (((packet->mode) >> 7)& 0x01))== 1){
    CRC16(packet->size_of_data);
    for (unsigned int i=0; i<(packet->size_of_data); i++)
      CRC16(packet->data[i]);
  }
  if (check)
    CRC16_2(packet->checksum);

  if (check)
    return crc16;
  else {
    packet->checksum = crc16;
    return -1;
  }
}



uint32_t calculateCRC32Data (uint32_t *data, uint8_t size){
  CRC->CR |= 0x01;                      // Reset CRC
  for (uint8_t i =0; i< size; i++){
    CRC->DR = data[i];
  }
  return CRC->DR;                       // Read CRC
}

uint32_t calculateCRC32Flash (uint32_t* start_adress, uint32_t * end_adress){
  CRC->CR |= 0x01;                      // Reset CRC
  while (start_adress<end_adress){
    CRC->DR = *start_adress;
    start_adress++;
  }
  return CRC->DR;                       // Read CRC
}
