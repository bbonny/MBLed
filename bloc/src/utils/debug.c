/*
 * debug.c
 *
 * Debug functions. 
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

// FreeRTOS and STM32 features
#include <stm32f10x.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

// MB Led features
#include "packet.h"
#include "debug.h"
#include "send.h"
#include "util.h"
#include "algo.h"

// Definition of the read queue
static xQueueHandle xQueueReadDebug;
static xQueueHandle xQueueSendDebug;
extern uint8_t myID;
/*
 *
 * This function configures the USART1 to use it as a serial
 * debug port.
 *
 */
void initDebug () {

  RCC->APB2ENR |= (1 << 2); // Port A
  RCC->APB2ENR |= (1 << 14); // USART1

  // Clocks initialization
  RCC->APB2ENR |= (1 << 14) | (1 << 2) | (1 << 0);

  // Port configuration
  GPIOA->CRH = (GPIOA->CRH & ~(0x000000F0)) | 0x000000B0;
  GPIOA->CRH = (GPIOA->CRH & ~(0x00000F00)) | 0x00000800;

  // Forcing the pull-up
  GPIOA->BSRR = 1<<10;

  // Setting up CR2
  USART1->CR2 &= ~(3 << 12); // STOP 1 bit

  // Baud rate 115.2Kbps
  USART1->BRR = 0x0271;

  // 8N1 mode
  USART1->CR1 = (0 << 12) // 8 bits words
    | (0 << 10) // No parity
    | (1 << 13) // Activating USART1
    | (1 << 5) // Reading IRQ
    | (0 << 7); // No writing IRQ

  // Setting reading interruption
  NVIC->IP[37] = 254;
  NVIC->ISER[1] |= (1 << 5);

  // Writing activation
  USART1->CR1 |= (1 << 3);

  // Reading activation
  USART1->CR1 |= (1 << 2);

  // Queue initialization
  xQueueReadDebug = xQueueCreate(50,sizeof(char));
  xQueueSendDebug = xQueueCreate(50,50*sizeof(char));
}

/*
 *
 * If the block is the master block, we configure the IRQHandler
 * for the serial port else it will be configured for IrDA.
 *
 */

#ifndef GLIP
void USART1_IRQHandler() {
  portBASE_TYPE resched_needed = pdFALSE;

  char c;
  if(USART1->SR & (1<<5)) {
    c = USART1->DR;
    xQueueSendFromISR(xQueueReadDebug,&c,&resched_needed);
  }
  portEND_SWITCHING_ISR(resched_needed);
}

/*
 *
 * This function tries to find a command in the characters
 * which are received on the USART1.
 *
 */

//extern uint16_t irda_in, irda_out;

void receiveDebug() {
 

#ifdef MBLED
  writeDebugInit("Welcome on MB LED Beta\r\n");
#endif

#ifdef GLIP_DEBUG
  writeDebugInit("Welcome on GLiP 2.0 Beta\r\n");
#endif

  static char c;
  static unsigned int allow = 0;
  static unsigned int data_in =0 , data_index = 0;
  Packet packet;
  uint8_t temp[MAX_DATA_SIZE];
  uint32_t buffer[128];
    for(;;) {
    //writeDebugInit("En attente d'ordre\n\r");
    xQueueReceive(xQueueReadDebug,&c,portMAX_DELAY);
    // 'C' : send a message                  

    if(c == 'P' && !allow){
      SEND_PING(1);
      SEND_PING(2);
      SEND_PING(3);
      SEND_PING(4);
      buffer[0]=1;
    }
    else if(c == 'S' && !allow) {
      allow = 1;
      writeCharDebug(c);
      data_in = 1;
      data_index = 0;	
      packet_init(&packet);
      writeDebugInit("\n\rCe qui sera tape sera envoye\r\n");
    }
    // Number of the animation                                                                        
    else if (((c != '\r') && (c != '\n')) && data_in && allow){
      writeCharDebug(c);
      temp[data_index] =c;
      data_index++;
    }
  
    // The number is confirmed, we change the animation                                               
    
    else if (((c == '\r') || (c == '\n')) && allow ) {
      allow = 0;
  
      //   packet_build(&packet, 1, 1, 0, 10, 0xff, data_index,temp);
      writeDebugInit ("On transmet le paquet\r\n");
      packet_transmit(&packet, 2);
      packet_transmit(&packet, 3);
      writeDebugInit ("Paquet transmit\r\n");
      data_in =0;
    }
    else if (((c == '\r') || (c == '\n'))) {
      writeDebugInit ("_______________________________________________________________\r\n");
      writeDebugInit ("'S' -> Send message\r\n");
      writeDebugInit ("'P' -> Ping\r\n");
      writeDebugInit ("_______________________________________________________________\r\n");
    }
   
    // 'R' : reset block                                                       
    else if(c == 'R') {
      SCB->AIRCR = 0x05FA0004;
      for(;;);
    }
  }
}

/*****************************************************************************
                             function Send debug     
*****************************************************************************/
#endif


#ifndef GLIP
void writePacketDebug (Packet *packet){
  writeDebugInit("\r\nfrom: ");
  writeHexDebug(packet->from);
  writeDebugInit("\r\n");
  writeDebugInit("id: ");
  writeHexDebug(packet->idPacket);
  writeDebugInit("\r\n");
  writeDebugInit("prio/to: ");
  writeHexDebug(packet->priority);
  writeHexDebug(packet->to);
  writeDebugInit("\r\n");
  writeDebugInit("ttl: ");
  writeHexDebug(packet->ttl);
  writeDebugInit("\r\n");
  writeDebugInit("mode: ");
  writeHexDebug(packet->mode);
  writeDebugInit("\r\n");
  if (packet->mode | 0x7F){
    writeDebugInit("data size: ");
    writeHexDebug(packet->size_of_data);
    writeDebugInit("\r\n");
    writeDebugInit("Data: ");
    writeDebugInit((const char *)packet->data);
    writeDebugInit("\r\n");    
  }
  writeDebugInit("CRC: ");
  writeHexDebug(packet->checksum);
  writeDebugInit("\r\n");
  writeDebugInit("End of packet\r\n"); 
}

/*
 *
 * This function write the character c on the serial port
 * We don't use interrupts for writing as far as it is not
 * used that much...
 *
 */

void writeCharDebug (char c) {
  while (!(USART1->SR & (1 << 7)));

  USART1->DR = c;
}

/*
 *
 * This function writes the string s on the serial port
 *
 */
void writeDebugSend () {
  char* s;
  for (;;){
    xQueueReceive(xQueueReadDebug,&s,portMAX_DELAY);
    // Write each character
    for (;*s;s++)
      writeCharDebug (*s);
  }
}


void writeDebugInit (const char *s) {
  // Write each character
  for (;*s;s++)
    writeCharDebug (*s);
}

/*
 *
 * This function writes the nibble c on the serial port
 *
 */
static void writeNibbleDebug(unsigned char c)
{
  writeCharDebug(c >= 10 ? 'A' - 10 + c : '0' + c);
}

/*
 *
 * This function writes the number c in hexa on the serial port
 *
 */
void writeHexDebug(unsigned char c)
{
  writeNibbleDebug(c >> 4);
  writeNibbleDebug(c & 0xf);
}

void writeHex16Debug (uint16_t c)
{
  writeHexDebug ((c >> 8) & 0x00ff);
  writeHexDebug (c & 0x00ff);
}

void writeHex32Debug (uint32_t c)
{ 
  writeHex16Debug ((c >> 16) & 0x0000ffff);
  writeHex16Debug (c & 0x0000ffff);
}

void writeHex64Debug (uint64_t c)
{ 
  writeHex32Debug ((c >> 32) & 0x00000000ffffffff);
  writeHex32Debug (c & 0x00000000ffffffff);
}

#else
//No debug for GLiP!

void writePacketDebug (Packet *packet){}
void writeCharDebug (char c) {} 
void writeDebugSend () {} 
void writeDebugInit (const char *s) {}
void writeHexDebug(unsigned char c){}
void writeHex16Debug (uint16_t c){}
void writeHex32Debug (uint32_t c){ }
void writeHex64Debug (uint64_t c){}
#endif
