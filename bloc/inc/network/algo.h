/*
 * algo.h
 *
 * This module initializes the algorithm and handles all things about myID, leaderID, direction, membersInNetwork and the interfaces
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

 
#ifndef H_INIT
#define H_INIT

#include "packet.h"

typedef struct{
  int8_t ready[4];
  int8_t ID[4];
  int8_t hisInterface[4];
} Interfaces;


int8_t getMyID();
int8_t getLeaderID();
void setLeaderID(int8_t newLeader);
int8_t isLeader();
int8_t isLeaderOfANetwork();

int8_t getDirection();
void setDirection(int8_t newDirection) ;
void addDirection(int8_t rotation);


int8_t getMembersInNetwork();
void setMembersInNetwork(int8_t newNumber);
void addMemberInNetwork();
void delMemberInNetwork();


int interface_ok(int i);
Interfaces getInterfaces();
Interfaces getSavedInterfaces();
void setInterface(int8_t interface, int8_t newNeighbor, int8_t hisInterface );

int8_t interface_neighbor(int interface);
int8_t interface_where_is_neighbor(int neighbor, int testedInterfaces);
int8_t interface_is_busy(int interface);
int8_t interface_hisInterface(int interface);
void interfaces_copy();
int8_t interfaces_nb_neighbors();
void interfaces_which_are_busy(int8_t* interfaces_busy);
int ID_was_in_interfaces(int hisID);
int ID_in_interfaces(int hisID);
int8_t interfaces_turn(Interfaces savedInterface, Interfaces interfaces);
void interfaces_show(Interfaces interfaces);
void interfaces_show_ready(Interfaces interfaces);



void algo_init();

uint8_t uart2interface(uint8_t uart);
uint8_t interface2uart(uint8_t interface);


int8_t which_is_myID();
void algo_task_create();

#define NORTH getDirection()
#define EAST (getDirection() + 1) % 4
#define SOUTH (getDirection() + 2) % 4
#define WEST (getDirection() + 3) % 4


#endif
 
 
