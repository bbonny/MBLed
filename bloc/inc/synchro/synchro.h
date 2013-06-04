/*
 * synchro.h
 *
 * This file starts the timer and allows the leader to transmit the current time to everyone.
 * Blocks who receive a TIME packet use the information and spread the TIME.
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

#ifndef _SYNCHRO_H_
#define _SYNCHRO_H_

/*
 * synchro.h
 * Return the local clock
 *
 */
#include "packet.h"

static inline uint32_t current_time()
{
  extern volatile uint32_t time_counter;
  return time_counter;
}

uint8_t getSynchroID();
void addSynchroID();

void adjust_current_time(uint32_t new_time);
void time_init();
void synchro_update(Packet *packet);
void time_restart();
#endif // _SYNCHRO_H_
