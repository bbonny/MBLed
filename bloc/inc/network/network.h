/*
 * network.h
 *
 * This module handles the network : it send and add people to the global network. If someone is missing, it will use 
 * the graph to delete all the members.
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

#ifndef H_NETWORK
#define H_NETWORK

#include "packet.h"

void network_send(int interface);
void network_receive();
void network_show();
void network_show_task(void* parameters);


void network_max_values();
void network_send_my_position(int interface);
void network_send_neighbors();
void network_cut_branch(int lost_neighbor);


void network_changeLeader(int leaderID);
void network_missing_receive(Packet * packet);
void network_reset(uint8_t neighbor, int badInterface);
void network_reset_task(void* parameters);

void network_semaphore_create();
#endif
