/*
 * election.h
 *
 *  This module handle the election of a leader in the network
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

#ifndef H_LEADER
#define H_LEADER

#define STATE_NULL          0x10
#define STATE_START         0x11
#define STATE_CANDIDATE     0x12
#define STATE_REJECTED      0x13
#define STATE_ELECTED       0x14


void setStillInTheElection(int i);
uint8_t getStillInTheElection();

void states_init();
void states_elected(int module);
void states_print(int i);
void set_state(int module, int new_state);
int get_state(int i);

void election_init();
void election_timer();
void election_main(Packet * packet);
void election_start();
void election_candidature(Packet * packet);
void election_end();

void election_send_network(int module, int membersInHisNetwork, int interface);



#endif
