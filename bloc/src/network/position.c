/*
 * position.c
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


#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>
#include <stdlib.h>

#include "constant.h"
#include "packet.h"
#include "position.h"
#include "network.h"
#include "send.h"
#include "debug.h"
#include "algo.h"
#include "election.h"

static uint8_t directionID; //ID for the direction.. The leader is the only one to update this.
static int8_t network_table[NB_BLOCKS][4];
static int8_t direction_table[4];
static uint8_t right_position; //If we're not in the right position, there is no check of the interfaces.
static int8_t whoIsHere[3];

//Init of the network table.
void network_init(){
  uint8_t myID = getMyID();
  //None is in the network table and we put them to (0,0)
  set_blocks_out();
  for(int i=0; i < NB_BLOCKS; i++)
    set_block_position(i, 0,0);
  
  set_block_in(myID);
  set_block_leader(myID, myID);
  setDirectionID(0);
  setRightPosition(1);
  network_semaphore_create();
}


/***********    Functions for the network table.  ********************/
 
//Checking if the block can be the network table.
int8_t block_ok(int block){
  if( (block >= 0) && (block < NB_BLOCKS)){
    return 1;
  }

  return 0;
}


//Checking if the block is in the network.
int8_t block_in(int block){
  if(block_ok(block))
    return network_table[block][IN];
  return -1;
}

//Getting the leader of the block.
int8_t block_leader(int block){
  if(block_ok(block))
    return network_table[block][LEADER];
  return -1;
}

//All the info for one block.
int8_t* get_block(int8_t id){
  if(block_ok(id))
    return network_table[id];
  return get_block_me();
}

//Info about the block.
int8_t* get_block_me(){
  return network_table[getMyID()];
}

//All blocks are out of the network!
void set_blocks_out(){
  
  for(int i=0; i<NB_BLOCKS; i++){
    network_table[i][LEADER] = -1;
    network_table[i][IN] = 0;
  }
  setMembersInNetwork(0);
}


//Add one block to the network.
void set_block_in(int block){
  if(block_ok(block)){
    if(!network_table[block][IN]){
      network_table[block][IN] = 1;
      addMemberInNetwork();
    }
  }  
}

//Delete one block of the network.
void set_block_out(int block){
  if(block_ok(block)){
    if(network_table[block][IN]){
      network_table[block][IN] = 0;
      network_table[block][LEADER] = -1; //No leader
      set_state(block, STATE_NULL); //No state
      delMemberInNetwork();      
    }
  }  
}

void set_block_leader(int block, int leader){
  if(block_ok(block)){
    network_table[block][LEADER] = leader;
  }
}

void set_block_position(int block, int i, int j){
  if(block_ok(block)){
    network_table[block][I] = i;
    network_table[block][J] = j;
  }
}


//Functions for the right position

void setRightPosition(int i){
  right_position = i;
}

int getRightPosition(){
  return right_position;
}


//Functions of the direction ID.

void setDirectionID(int i){
   directionID = i;
}

void addDirectionID(){
  directionID++;
}


//Functions for the direction table

int8_t getDirectionTable(int interface ){
  if(interface_ok(interface))
    return direction_table[interface];
  return -1;
}

void shift_direction_table(uint8_t direction){
  for(int i=0; i<4; i++){
    direction_table[i] = (4 + i - direction) %4;
    if(direction_table[i] < 0) direction_table[i] = direction_table[i] + 4;
  }
}



//Sending a POSITION packet.
 //POSITION packet is : DIRECTION_ID/LEADER_ID/MY_DIRECTION/YOUR_I/YOUR_J/MY_INTERFACE.
void position_send(uint8_t interface, uint8_t ttl){
  uint8_t data[7];
  int8_t* me = get_block_me();

  Coo coo = { .i = me[I], .j = me[J]};
  //Calculate the position of the neighbor.
 position_calculate(&coo,getDirectionTable(interface));

  if(getLeaderID() == -1)
    return;

  //Broadcasting the result!
  data[0] = directionID;
  data[1] = getLeaderID();
  data[2] = getDirection();
  data[3] = coo.i ;
  data[4] = coo.j;
  data[5] = interface;
  
  //This isn't send in the data, it's only the ttl
  data[6] = ttl;

  send_new_packet(POSITION, data, interface);
}



void position_spread(uint8_t forbiddenInterface, uint8_t ttl){

  //The leader changes the directionID.
  if(isLeader()){
    directionID++;
  }

  //Send position to every interface except for forbiddenInterface.
  for(int interface=0; interface<4;interface++){
    if((interface != forbiddenInterface) && (interface_is_busy(interface) )){
      position_send(interface, ttl);
    }
  }
}

//Received a POSITION packet.
//POSITION PACKET IS : DIRECTION_ID/LEADER_ID/MY_DIRECTION/YOUR_I/YOUR_J/MY_INTERFACE.
void position_receive(Packet *  packet){
  
  uint8_t from, his_direction, i, j,hisInterface;
  int8_t diffDirection;
  uint8_t hisDirectionID;
  int8_t* me = get_block_me();

  hisDirectionID = packet->data[0];
  from = packet->data[1];
  his_direction = packet->data[2];
  i = packet->data[3];
  j = packet->data[4];
  hisInterface = packet->data[5];

  
  //Not coming from the leader
  if( (int8_t) from != getLeaderID())
    return;

  //The message doesn't come from one of my neighbor.
  if(!ID_in_interfaces(packet->from)){
    return;
  }

  //Wrong interface.
  if((hisInterface != interface_hisInterface(packet->interface)) ){
     return;
  }

  //Someone is sending the block a wrong direction.
  if(isLeaderOfANetwork() && (i!= me[I] ) && (j!= me[J] )){
    BROADCAST_POSITION;
    return;
  }
  

  //If it's a new packet for the block, it will read it.
  if(directionID < hisDirectionID){
    directionID = hisDirectionID;
    who_is_in_this_position(whoIsHere,i,j);

    //OK; it's the block's position.
    if(whoIsHere[0] == 1 && is_my_position(i,j) ){
       return;
    }
    else if(whoIsHere[0] >= 1){
      return;
    }

    setRightPosition(1);
    diffDirection = ((his_direction - getDirection() ) - (hisInterface - packet->interface + 2 ) ) % 4;
    
    if(diffDirection != 0){
      int8_t new_direction = (getDirection() + diffDirection) % 4;
      if(new_direction < 0) new_direction = new_direction + 4;
      setDirection(new_direction);
    }
    
    uint8_t myID = getMyID();
    set_block_position(myID, i, j);
    set_block_leader(myID, from);

    //Sending POSITION packet and waiting a little bit.
    position_spread(packet->interface, packet->ttl - 1);
    vTaskDelay(IRDA_WAIT);

    //Sending the network.
    for(int interface=0; interface <4; interface++){
      if(interface != packet->interface)
	 network_send(interface);
      else{
	network_send_my_position(packet->interface);
      }   
    }

  }

}


//Calculate the next position according to the interfaceDirection
void position_calculate(Coo* coo, uint8_t interfaceDirection){
 
  if(interfaceDirection == 0){
    coo->i = coo->i - 1;
  }
  else if(interfaceDirection == 1){
    coo->j = coo->j + 1;
  }
  else if(interfaceDirection == 2){
    coo->i = coo->i + 1; 
  }
  else{
    coo->j = coo->j - 1;
  }
}



//If (i,j) is the block's position, this function returns 1.
int is_my_position(int8_t i, int8_t j){
  int8_t* me = get_block_me();

  if(me[I] == i && me[J] == j)
    return 1;
  
  return 0;
}

//Checking if (i,j) is used.
//Returns a 3 cases table : NB_HERE (0,1 or 2) ID1 ID2. 
void who_is_in_this_position (int8_t* whoIsHere,int i, int j){

  int nb=0;
  int8_t leaderID = getLeaderID();
  int8_t* block;

  for(int k=0; k < NB_BLOCKS; k++){
    block = get_block(k);

    if((block[I] == (int8_t) i) && (block[J] == (int8_t) j))
      {
        if((block[IN] == 1) && (block[LEADER] == leaderID)){
          whoIsHere[nb + 1] = (int8_t) k;
	  nb++;
        }
      }
    //If there is more than 2 people in (i,j), we send only the two first.
    if(nb == 2)
      break;
  }
  whoIsHere[0] = nb;
}



