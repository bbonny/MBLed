/*
 * synchro.c
 *
 * This file starts the timer and allows the leader to transmit the current time to everyone.
 * Blocks who receive a TIME packet use the information and spread the TIME.
 *
 * Copyright 2011 - Benjamin Bonny    <benjamin.bonny@gmail.com>,
 *                  C�dric Le Ninivin <cedriclen@gmail.com>,
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

#include <stdlib.h>
#include "synchro.h"
#include "send.h"
#include "debug.h"
#include "algo.h"
#include "ping.h"

/*
 *
 * Maximum value beyond which the clock must be changed directly and no
 * progressively.
 * 100 * 100�s = 10ms = 1/4 of frame at 25Hz
 *
 */
#define TIME_THRESHOLD 100

//The leader send a synchroID to the others so that they know it's a good TIME packet.
static uint8_t synchroID;

// local clock
volatile uint32_t time_counter = 0;


static uint8_t timerSynchroID;
static uint8_t timerLeaderID;




uint8_t getSynchroID(){
  return synchroID;
}

void addSynchroID(){
  synchroID++;
}

/*
 *
 * Change the local clock depending on the THRESHOLD value
 *
 */
void adjust_current_time(uint32_t new_time)
{
  int64_t now = current_time();
  int64_t newt = new_time;
  if (abs(newt - now) > TIME_THRESHOLD)
    time_counter = new_time;
  else
    time_counter += (newt - now) / 2;
}

void synchro_update(Packet * packet){
  uint8_t hisSynchroID = packet->data[4];
  uint8_t membersInHisNetwork = packet->data[5];

  if(packet->from == getLeaderID() ){

    //First time the new leader sends us a time packet.
    if(timerLeaderID != packet->from){
      timerLeaderID = packet->from;
      timerSynchroID = 0;
    }
    
    if(hisSynchroID > timerSynchroID){
      timerSynchroID = hisSynchroID;

      adjust_current_time(*(uint32_t *)&packet->data[0]);

      uint32_t now = current_time();

      packet->data[3] = (now >> 24) &(0x00FF);
      packet->data[2] = (now >> 16) &(0x00FF);
      packet->data[1] = (now >> 8)  &(0x00FF);
      packet->data[0] = now &(0x00FF);

      send_spread(packet,packet->interface);
    }
  }
  else{
    ping_decision(packet->from, membersInHisNetwork, packet->interface);
  }
}

/*
 *
 * Timer initialization
 *
 */
void time_init()
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;   // clock
  TIM5->PSC = 0;
  TIM5->ARR = 7200;

  // IRQ
  NVIC->IP[50] = 16;
  NVIC->ISER[1] |= 1 << 18;

  TIM5->DIER = 1 << 0;

  TIM5->CR1 = 1 << 0;       // Start the timer

  time_restart();
}

void time_restart(){
  timerSynchroID = 0;
  timerLeaderID = 0;
  synchroID = 0;
}

/*
 *
 * We used an external variable to store the counter because the stm32's timers
 * are on 16bits and we need 32bits.
 *
 */
void TIM5_IRQHandler () {
  TIM5->SR &= ~1;
  time_counter++;
}
