/*
 * irda.c
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


// FreeRTOS and STM32 features
#include <stm32f10x.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>


// MB Led 
#include "debug.h"
#include "packet.h"
#include "checksum.h"
#include "irda.h"
#include "send.h"
#include "algo.h"
#include "scheduler.h"
#include "synchro.h"


#define NB_MESS_QUEUE 15
#define NB_MAIN_QUEUE 40
//!\\ For debug
#include "synchro.h"

// Declaration of the reception, transmission and treatment queues,
// One per IRDA
static xQueueHandle xQueueRead [4];
static xQueueHandle xQueueWrite [4];

  // Packets used for the reception
static  Packet usartPacket [4];

// Queues for sending packet
xQueueHandle xQueueSendIrDA [4],xQueueSendIrDABuffer [4], xQueueSendQuick[4];
xQueueHandle xPongQueue;
extern xQueueHandle xPingQueue[4];
xQueueHandle xQueueAck[4];


// Id of last packet
static ID_PACKET input_buffer[4];

// Semaphore for sending:
xSemaphoreHandle xSemaphoreIrDA[4];

xQueueHandle xQueuePacket;

uint16_t irda_in;

/*****************************************************************************
                            Initialisations Functions                               
*****************************************************************************/


void irda_config(int n) {


  static const int interrupt [5] = {37,38,39,52,53};
  static const int shift [5] = {1<<5,1<<6,1<<7,1<<20,1<<21};

  USART_TypeDef* UART;

  //Configure U(S)ART n
  switch(n) {
 case 1:
   UART = USART1;
   // Configuration of ports PA9, PA10
   GPIOA->CRH = (GPIOA->CRH & ~(0x000000F0)) | 0x000000B0;
   GPIOA->CRH = (GPIOA->CRH & ~(0x00000F00)) | 0x00000800;
   // Forcing the pull-up
   GPIOA->BSRR = (1 << 10);
   break;

  case 2:
    UART = USART2;
    // Configuration of ports PA2/TX, PA3/RX
    GPIOA->CRL = (GPIOA->CRL & ~(0x00000F00)) | 0x00000B00;
    GPIOA->CRL = (GPIOA->CRL & ~(0x0000F000)) | 0x00008000;
    // Forcing the pull-up
     GPIOA->BSRR = (1 << (3));

#ifdef MBLED
    // SD port in PA1 and switch on transceiver in FIR mode
    GPIOA->CRL |=  GPIO_CRL_MODE1 ;   //Max output speed 50MHz
    GPIOA->CRL &= ~GPIO_CRL_CNF1 ;    // GPO Pull-Push
    GPIOA->BSRR = 1<<(1+16);          //SD =0
#endif   

    break;

  case 3:
    UART = USART3;
    // Configuration of ports PB10/TX, PB11/RX
    GPIOB->CRH = (GPIOB->CRH & ~(0x00000F00)) | 0x00000B00;
    GPIOB->CRH = (GPIOB->CRH & ~(0x0000F000)) | 0x00008000;
    // Forcing the pull-up
     GPIOB->BSRR = (1 << (11));

#ifdef MBLED
    // SD port in PB1 and switch on transceiver in FIR mode
    GPIOB->CRL |=  GPIO_CRL_MODE1 ;   //Max output speed 50MHz
    GPIOB->CRL &= ~GPIO_CRL_CNF1 ;    // GPO Pull-Push
    GPIOB->BSRR = (1<<(16+1));
#endif

    break;

  case 4:
    UART = UART4;
    // Configuration of ports PC10/TX, PC11/RX
    GPIOC->CRH = (GPIOC->CRH & ~(0x00000F00)) | 0x00000B00;
    GPIOC->CRH = (GPIOC->CRH & ~(0x0000F000)) | 0x00008000;
    // Forcing the pull-up
    GPIOC->BSRR = (1 << 11);

#ifdef MBLED
    // SD port in PA8 and switch on transceiver in FIR mode
    GPIOA->CRH |=  GPIO_CRH_MODE8 ;   //Max output speed 50MHz
    GPIOA->CRH &= ~GPIO_CRH_CNF8 ;    //GPIO Pull-Push
     GPIOA->BSRR = (1<<(8+16));
 #endif

   break;


  case 5:
    UART = UART5;
    // Configuration of ports PC12, PD2
    GPIOC->CRH = (GPIOA->CRH & ~(0x000F0000)) | 0x000B0000;
    GPIOD->CRL = (GPIOA->CRH & ~(0x00000F00)) | 0x00000800;
    // Forcing the pull-up
    GPIOA->BSRR = (1 << 10);

#ifdef MBLED
    // SD port in PB5 and switch on transceiver in FIR mode
    GPIOB->CRL |=  GPIO_CRL_MODE5 ;   //Max output speed 50MHz
    GPIOB->CRL &= ~GPIO_CRL_CNF5 ;    // GPO Pull-Push
     GPIOB->BSRR = (1<<(5+16));
 #endif

    break;
  default:
    UART = 0;
  }
  // Setting up CR2
  UART->CR2 &= ~(1 << 11) // CLKEN 0
    & ~(3 << 12) // STOP 1 bit
    & ~(1 << 14); // LINEN 0

  // Setting up CR3
  UART->CR3 &= ~(1 << 5) // SCEN 0
    & ~(1 << 3); // HDSEL 0

  // Normal IrDA mode -> PSC = 00000001
  UART->GTPR = (UART->GTPR & 0xFFFFFF00) | 0x00000001;


  //Only for GLIPs
  if(n==1){
    UART->BRR = 0x0271;
  }
  else{
    // Baud rate 115.2Kbps
    UART->BRR = 0x0138;
  }
  // Setting the IrDA
  UART->CR3 |= (1 << 1);

  // Mode 8N1
  UART->CR1 = (0 << 12) // 8 bits wordsts
    | (0 << 10) // No parity
    | (1 << 13) // Activate USART
    | (1 << 5) // IRQ on reception
    | (0 << 7); // No IRQ on writing

  // Activating interruptions for reception
  NVIC->IP[interrupt[n-1]] = 254;
  NVIC->ISER[1] |= shift[n-1];

  // Activating interruptions for writing
  UART->CR1 |= (1 << 3);

  // Activating of the reception
  UART->CR1 |= (1 << 2);

  // Initialization of queues
  xQueueRead[n-IRDA_MIN]  = xQueueCreate (128, sizeof(char));
  xQueueWrite[n-IRDA_MIN] = xQueueCreate (128, sizeof(char));
  xQueueSendIrDA[n-IRDA_MIN]= xQueueCreate (NB_MESS_QUEUE, sizeof(Packet));
  xQueueSendIrDABuffer[n-IRDA_MIN]= xQueueCreate (1, sizeof(Packet));
  xQueueSendQuick[n-IRDA_MIN] = xQueueCreate (NB_MESS_QUEUE, sizeof(Packet));
  xQueueAck[n-IRDA_MIN] = xQueueCreate(NB_MESS_QUEUE, sizeof(ID_PACKET));
}



/*
 *
 * This function initializes the clocks of the U(S)ARTs
 * and the treatment queue.
 * Then it calls the previous function to configure each U(S)ART.
 *
 */


void irda_init () {

  // Clocks initialization
  RCC->APB2ENR |= (1 << 2) // IOPAEN
    | (1 << 3) // IOPBEN
    | (1 << 4) // IOPCEN
    | (1 << 5) // IOPDEN
    | (1 << 0) // AFIOEN
    | (1 << 14); //USART1EN

  RCC->APB1ENR |= (1 << 17) // USART2EN
    | (1 << 18) // USART3EN
    | (1 << 19) // UART4EN
    | (1 << 20); // UART5EN
  
  // Initialisation in FIR mode for shorter distance communication
      
  // Treatment queue initialization
  xQueuePacket = xQueueCreate (NB_MAIN_QUEUE, sizeof(Packet));
  xPongQueue = xQueueCreate(5, sizeof(Packet));

  irda_input_buffer_init();

   
  // Configure U(S)ARTs from MIN to MAX
  for (unsigned int i = IRDA_MIN; i <= IRDA_MAX; i++){
    irda_config (i);
    // Transmission mutex initialization
    xSemaphoreIrDA[i-IRDA_MIN] = xSemaphoreCreateMutex();
  }
   
  if (IRDA_MAX == 5){
    irda_config (4);
    // Transmission mutex initialization
    xSemaphoreIrDA[4-IRDA_MIN] = xSemaphoreCreateMutex();
  }
 

}


/*****************************************************************************
         Function to reset queues in case a neighbour leave     
*****************************************************************************/
void irda_reset_queues(unsigned int n){

  if(n == -1 || n == 0xFF)
    {
      writeDebugInit("Reset queue failed");
    }
  Packet packet;
  ID_PACKET id_packet;

  for(int i=0; i <NB_MESS_QUEUE; i++){
    xQueueReceive(xQueueSendIrDA[n-IRDA_MIN], &packet, 0);
    xQueueReceive(xQueueSendQuick[n - IRDA_MIN], &packet, 0);
    xQueueReceive(xQueueAck[n - IRDA_MIN], &id_packet, 0);
  }
  xQueueReceive(xQueueSendIrDABuffer[n - IRDA_MIN], &packet, 0);
}


/*****************************************************************************
                          Handler functions                                        
*****************************************************************************/
#ifdef GLIP
void USART1_IRQHandler () {
  portBASE_TYPE resched_needed = pdFALSE;

  char c;

  if (USART1->SR & (1 << 5)) {
    // Put the character in the reception queue
    c = USART1->DR;
    xQueueSendFromISR ( xQueueRead [1 - IRDA_MIN], &c, &resched_needed );
  }
  else if (USART1->SR & (1 << 7)) {

    // Write a character in the register DR
    if(xQueueReceiveFromISR( xQueueWrite [1 - IRDA_MIN], &c, &resched_needed ) == pdTRUE) {
      USART1->DR = c;
    }
    else
      // Stopping writing interruptions
      USART1->CR1 &= ~(1 << 7);
  }

  portEND_SWITCHING_ISR (resched_needed);
}
#endif


// Handler for USART2 interruptions
void USART2_IRQHandler () {
  portBASE_TYPE resched_needed = pdFALSE;

  char c;

  if (USART2->SR & (1 << 5)) {
    // Put the character in the reception queue
    c = USART2->DR;
    xQueueSendFromISR ( xQueueRead [2 - IRDA_MIN], &c, &resched_needed );
  }
  else if (USART2->SR & (1 << 7)) {
    // Write a character in the register DR
    if(xQueueReceiveFromISR( xQueueWrite [2 - IRDA_MIN], &c, &resched_needed ) == pdTRUE) {
      USART2->DR = c;
    }
    else
      // Stopping writing interruptions
      USART2->CR1 &= ~(1 << 7);
  }

  portEND_SWITCHING_ISR (resched_needed);
}

// Handler for USART3 interruptions
void USART3_IRQHandler () {
  portBASE_TYPE resched_needed = pdFALSE;

  char c;

  if (USART3->SR & (1 << 5)) {
    // Put the character in the reception queue
    c = USART3->DR;
    xQueueSendFromISR ( xQueueRead[3 - IRDA_MIN], &c, &resched_needed );
  }
  else if (USART3->SR & (1 << 7)) {
    // Write a character in the register DR
    if(xQueueReceiveFromISR( xQueueWrite[3 - IRDA_MIN], &c, &resched_needed ) == pdTRUE) {
      USART3->DR = c;
    }
    else
      // Stopping writing interruptions
      USART3->CR1 &= ~(1 << 7);
  }

  portEND_SWITCHING_ISR (resched_needed);
}

// Handler for UART4 interruptions
void UART4_IRQHandler () {
  portBASE_TYPE resched_needed = pdFALSE;

  char c;

  if (UART4->SR & (1 << 5)) {
    // Put the character in the reception queue
    c = UART4->DR;
    xQueueSendFromISR ( xQueueRead[4 - IRDA_MIN], &c, &resched_needed );
  }
  else if (UART4->SR & (1 << 7)) {
    // Write a character in the register DR
    if(xQueueReceiveFromISR( xQueueWrite[4 - IRDA_MIN], &c, &resched_needed ) == pdTRUE) {
      UART4->DR = c;
    }
    else
      // Stopping writing interruptions
      UART4->CR1 &= ~(1 << 7);
  }

  portEND_SWITCHING_ISR (resched_needed);
}



#ifdef MBLED
// Handler for UART5 interruptions
void UART5_IRQHandler () {
  portBASE_TYPE resched_needed = pdFALSE;

  char c;

  if (UART5->SR & (1 << 5)) {
    // Put the character in the reception queue
    c = UART5->DR;
    xQueueSendFromISR ( xQueueRead[5 - IRDA_MIN], &c, &resched_needed );
  }
  else if (UART5->SR & (1 << 7)) {
    // Write a character in the register DR
    if(xQueueReceiveFromISR( xQueueWrite[5 - IRDA_MIN], &c, &resched_needed ) == pdTRUE) {
      UART5->DR = c;
    }
    else
      // Stopping writing interruptions
      UART5->CR1 &= ~(1 << 7);
  }

  portEND_SWITCHING_ISR (resched_needed);
}
#endif





/*****************************************************************************
                      Function handling arriving characters                           
*****************************************************************************/

void irda_read (void *pvParameters){


  portBASE_TYPE xStatus;
  unsigned int uartn = (unsigned int) pvParameters;
  char c;
  STATE state = STATE_IDLE;
  char begin[3];
  
  irda_in = 0;

  int index=0;
  
  for( ;; ){
    //Wait character    
    xStatus=xQueueReceive( xQueueRead [uartn-IRDA_MIN], &c, 10);
    if (xStatus == pdPASS){

      begin[index] =c;
      index++;
      index = index % 3;
    }
    else{
      xSemaphoreGive(xSemaphoreIrDA[uartn-IRDA_MIN]);
    }
    // Evaluate new state
    irda_new_state (&usartPacket[uartn-IRDA_MIN], &state, index, begin, c, uartn);

  }  
}


/*****************************************************************************
      Function constructing packets thanks to a state machine
*****************************************************************************/


void irda_new_state (Packet* packet, STATE * state , int index, char begin[], char c, unsigned int uartn)
{
  static uint8_t size_of_data ,index_data;
  uint8_t send_ack_info[2];
  ID_PACKET input_ack;
  int check = -1;
  if (*state != STATE_GET_DATA
      &&begin[index] == 'M'
      && begin[(index+1)%3] == 'B'
      && begin[(index+2)%3] == 'L'){
    //welcoming new packet
    xSemaphoreTake(xSemaphoreIrDA[uartn-IRDA_MIN],0);

    packet_init(packet);
    *state = STATE_FROM;
    size_of_data =0;
    index_data =0;
  }
  else{
    switch(*state)
      {
      case STATE_FROM:
	packet->from = c;
	(*state)++;
	break;
      case STATE_ID:
	packet->idPacket =c;
	(*state) ++;
	break;
      case STATE_PRIORITY_TO:
	packet->priority=((c & 0xF0)>> 3);
	packet->to = (c & 0x0F);
	(*state)++;
	break;
      case STATE_TTL:
	packet->ttl =c;
	(*state)++;
	break;
      case STATE_COMMAND:
	packet->mode = c;
	if( (     (((packet->mode) >> 7)& 0x01))== 1) {
	  (*state)++; 
	}
	else{
	  *state = STATE_CHECKSUM1;
	}
	break;
      case STATE_DATA_SIZE:
	packet->size_of_data = c;
	size_of_data = c;
	if (size_of_data > MAX_DATA_SIZE){
	  (*state)= STATE_CHECKSUM2;
	}
	(index_data) = 0;
	irda_data_init(packet->data ,c) ;
	(*state)++;
	break;
      case STATE_GET_DATA:
	packet->data[index_data] =c;
	(index_data)++;
	if (index_data>=size_of_data){
	  index_data=0;
	  (*state)++;
	}
	break;
      case STATE_CHECKSUM1:
	packet->checksum = (c << 8);
	(*state)++;
	break;
      case STATE_CHECKSUM2:
	packet->checksum |= c;
	// Checking packet's integrity
	check = calculateCRC16Packet (packet, 1);

	if (check == 0) {
          // If the packet is ok, we put it in the treatment queue
	  packet->interface= uart2interface(uartn);
	  packet_show(packet, 0);
	  irda_in ++;
	  
	  // this part switch importants packet to their specified queue
	  if(packet->mode == PING){ 
	    xQueueSend(xPongQueue , packet, 0) ;
	  }
	  else if(packet->mode == PONG){
	    xQueueSend(xPingQueue[packet->interface] , packet,0 );
	  }
	  else if(packet->mode == ACK){
	    input_ack.from = packet->data[0];
	    input_ack.idPacket = packet->data[1];
	    xQueueSend(xQueueAck[uartn-IRDA_MIN],&input_ack, 0 );
	  }
	  else if(packet->mode == TIME){
	    xQueueSend(xQueuePacket, packet,0);
	  }
	  else {
	    // other packets need ack
	      send_ack_info[0] = packet->from;
	      send_ack_info[1] = packet->idPacket;
	      send_new_packet(ACK,
			      send_ack_info, 
			      packet->interface);
	    if (!((packet->from == input_buffer[uartn-IRDA_MIN].from)
		  && (packet -> idPacket == input_buffer[uartn-IRDA_MIN].idPacket))){
	      xQueueSend (xQueuePacket, packet,0);
	      input_buffer[uartn-IRDA_MIN].from = packet->from;
	      input_buffer[uartn-IRDA_MIN].idPacket =packet->idPacket;
	    }
	  }
	}
	else{
	}
	xSemaphoreGive(xSemaphoreIrDA[uartn-IRDA_MIN]);
	(*state)= STATE_IDLE;
	break;
      default:
	*state = STATE_IDLE;
      } 
  }
}



/*****************************************************************************
                      IrDA sending functions                                   
*****************************************************************************/

void irda_write_char (char c, unsigned int nUSART){
  // Put the character in the queue
  xQueueSend ( xQueueWrite [nUSART-IRDA_MIN], &c, portMAX_DELAY );
  // Activating interruptions for writing
  switch (nUSART) {
 case 1:
    USART1->CR1 |= (1 << 7);
    break;
  case 2:
    USART2->CR1 |= (1 << 7);
    break;
  case 3:
    USART3->CR1 |= (1 << 7);
    break;
  case 4:
    UART4->CR1 |= (1 << 7);
    break;
  case 5:
    UART5->CR1 |= (1 << 7);
    break;
  }
}

void irda_write_short (unsigned short s, unsigned int nsUART) {
  irda_write_char(s >> 8, nsUART);
  irda_write_char(s & 0xff, nsUART);
}

void irda_write_data (const char *s, unsigned int nUSART, unsigned int size) {
  for (unsigned int i = 0;i < size; i++)
    irda_write_char(s[i], nUSART);
}



/*****************************************************************************
                            helpfull functions     
*****************************************************************************/

void irda_data_init (uint8_t* t, short c){
  uint8_t temp[c];
  t = temp;
}


 void irda_input_buffer_init (){
   for (uint8_t i=0 ; i <4; i++){
     input_buffer[i].from = 0 ;
     input_buffer[i].idPacket = 0 ;
   }
 }
