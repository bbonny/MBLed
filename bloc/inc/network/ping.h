/*
 * ping.h
 *
 * This module is used for the PING/PONG operation.
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

#ifndef H_PING
#define H_PING

uint8_t getSynchroID();
void addSynchroID();

void pong_answer(Packet * mess);
void ping_task(void *parameters);
void ping_init();
void setPingOut();

void ping_restart();
void ping_acq_task(void*parameters);
void leader_task(void *parameters);
void pong_task(void *parameters);

void ping_new_neighbor(uint8_t neighbor, uint8_t membersInHisNetwork, uint8_t interface, uint8_t hisInterface);
void ping_neighbor_turn(uint8_t neighbor, uint8_t membersInHisNetwork, uint8_t interface, uint8_t hisInterface);

void ping_decision(uint8_t neighbor, uint8_t membersInHisNetwork, uint8_t interface);
void ping_no_answer(uint8_t* nbNoAnswer, uint8_t interface);



#endif
