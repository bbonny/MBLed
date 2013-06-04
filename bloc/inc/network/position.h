/*
 * position.h
 *
 * This module creates a network table and manipulates it. Also, it sends and receives POSITION messages.
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


#ifndef H_POSITION
#define H_POSITION

typedef struct Coo{
  int8_t i;
  int8_t j;
}Coo;

int8_t block_ok(int block);
int8_t block_in(int block);
int8_t block_leader(int block);
void set_blocks_out();
void set_block_in(int block);
void set_block_out(int block);
void set_block_leader(int block, int leader);
void set_block_position(int block, int i, int j);
int8_t* get_block(int8_t id);
int8_t* get_block_me();

int8_t getDirectionTable(int i );
void shift_direction_table(uint8_t direction);


void setDirectionID(int i);
void addDirectionID();


void setRightPosition(int i);
int getRightPosition();

void network_init();
void position_send(uint8_t interface, uint8_t ttl);
void position_receive(Packet* mess);
void position_spread(uint8_t forbiddenInterface, uint8_t ttl);
void position_calculate(Coo* coo, uint8_t interface);



int is_my_position(int8_t i, int8_t j);
uint8_t distance_from_leader();
int find_same_position (int i, int j);
void who_is_in_this_position (int8_t* whoIsHere,int i, int j);

void position_remote_north(Packet * packet);

#endif
