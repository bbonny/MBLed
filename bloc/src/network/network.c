/*
 * network.c
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


#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include "constant.h"
#include "packet.h"
#include "network.h"
#include "debug.h"
#include "send.h"
#include "election.h"
#include "position.h"
#include "algo.h"
#include "graph.h"


#include "irda.h"


extern xQueueHandle xElectionQueue;
static xSemaphoreHandle resetSemaphore, resetSemaphoreAck;

int awareOfMissingBlock;
int8_t minI, minJ, maxI, maxJ;
static int8_t whoIsHere[3];


//Creation of the semaphores for the reset.
void network_semaphore_create(){
   vSemaphoreCreateBinary(resetSemaphore);
   vSemaphoreCreateBinary(resetSemaphoreAck);
   xSemaphoreTake (resetSemaphoreAck,0);
}

//Debug function : shows the network at the very moment.
void network_show(){

  Interfaces myInterfaces = getInterfaces();
  Interfaces savedInterfaces = getSavedInterfaces();
  int8_t* block;

  writeDebugInit("****NETWORK******* \n\r");
  writeDebugInit("Right position?");
  writeHexDebug(getRightPosition());
  writeDebugInit("\r\nMy ID: ");
  writeHexDebug(getMyID());
  writeDebugInit(" |  Leader : ");
  writeHexDebug(getLeaderID());
  writeDebugInit(" | Network : "); 
  writeHexDebug(getMembersInNetwork());
  writeDebugInit("\n\r");
  
  for(int i=0; i< NB_BLOCKS; i++){
    block = get_block(i);
    if(block[IN]) {
      writeHexDebug(i);
      writeDebugInit(" : (");
      writeHexDebug(block[I]);
      writeDebugInit(",");
      writeHexDebug(block[J]);
      writeDebugInit(") / Leader:");
      writeHexDebug(block[LEADER]);
      writeDebugInit(" / ");
      states_print(i);
      writeDebugInit("\n\r");
    }

  }

  vTaskDelay(IRDA_WAIT);

  writeDebugInit("direction : ");
  writeHexDebug(getDirection());
  writeDebugInit("\r\n");
  writeDebugInit("Direction : ");
  writeHexDebug(getDirectionTable(0));
  writeDebugInit(" ");
  writeHexDebug(getDirectionTable(1));
  writeDebugInit(" ");
  writeHexDebug(getDirectionTable(2));
  writeDebugInit(" ");
  writeHexDebug(getDirectionTable(3));

  writeDebugInit("Neighbors : ");
  interfaces_show(myInterfaces);
  writeDebugInit("Saved nei : ");
  interfaces_show(savedInterfaces);

  network_max_values();
  writeDebugInit("I : ");
  writeHexDebug(minI);
  writeDebugInit(" / ");
  writeHexDebug(maxI);
  writeDebugInit(" - J : ");
  writeHexDebug(minJ);
  writeDebugInit(" / ");
  writeHexDebug(maxJ);
  writeDebugInit("\r\n");

  writeDebugInit("********************* \n\r");
}

void network_show_task(void* parameters){
  
  while(1){
    vTaskDelay(500);
    network_show();
    vTaskDelay(2000);
  }
}


//Sending to the neighbor all the module knows about the network.
//Information is NETWORK ID X Y LEADER_ID
void network_send(int interface){

  uint8_t data[4];
  int8_t to = interface_neighbor(interface);
  int8_t* block;

  int8_t leaderID = getLeaderID();
  if(leaderID == -1)
    return;

  //There is someone in front of this interface.
  if(to != -1){
    for(int i=0; i < NB_BLOCKS; i++){
      block = get_block(i);
      
      if((block[IN] == 1) && (block[LEADER] == leaderID)){ //Modules is in the good network, so we send it.
	data[0] = i;
	data[1] = block[I];
	data[2] = block[J];
	data[3] = leaderID;
	send_new_packet(NETWORK, data, interface);
      }
    }
  }
}

//Sending all the block's interfaces to other interfaces.
void network_send_neighbors(){
  int8_t* block;
  int8_t neighbor;
  int8_t leaderID = getLeaderID();
  uint8_t data[4];

  for(int interface=0; interface<4; interface++){
    //There is someone in the interface
    if(interface_is_busy(interface)){
      neighbor = interface_neighbor(interface);
      block = get_block(neighbor);

      //Good leader so it's a good postion.
      if(block[LEADER] == leaderID){
	data[0] = neighbor;
        data[1] = block[I];
        data[2] = block[J];
        data[3] = leaderID;

	//Spreading the packet.
	send_new_packet(NETWORK, data, SPREAD + interface);
	
      }
    }
  }
}


//Sending the block position to an interface.
void network_send_my_position(int interface){

  int8_t* me = get_block_me();
  int8_t leaderID = getLeaderID();

  if(leaderID == -1)
    return;

  uint8_t data[4];
  data[0] = getMyID();
  data[1] = me[I];
  data[2] = me[J];
  data[3] = leaderID;

  send_new_packet(NETWORK, data, interface);
}


//Received a NETWORK packet
void network_receive(Packet * packet){

  uint8_t blockID, i , j, lead;
  int8_t* block;
  int nbChanges = 0;


  //Informations of the packet.
  blockID = packet->data[0];
  i = packet->data[1];
  j = packet->data[2];
  lead = packet->data[3];

  //Nb isn't a good blockID.
  if(!block_ok(blockID)){
    return;
  }
   block = get_block(blockID);
  
   who_is_in_this_position(whoIsHere,i,j);

  //Someone is already at this position...
  if(whoIsHere[0] == 1){
    int8_t block_here = whoIsHere[1];

    if(block_here == getMyID()){
      position_send(packet->interface, 1);
      return;
    }
    else if(block_here == blockID){
      return;      
    }
  }
  //2 blocks at one place... Strange and should not happen!
  else if(whoIsHere[0] >= 2){
      return;
    }


  //To change the block position, others have to send POSITION packet.
  if((lead == getLeaderID()) && (blockID != getMyID() )){

    //The module wasn't in the network before.
    if(!block_in(blockID)){
      set_block_in(blockID);
      nbChanges ++;
    }
    if(block[I] != i || block[J] != j){
      set_block_position(blockID, i,j );
      nbChanges++;
    }
    set_block_leader(blockID, lead);

    //Spreading the packet if there is a change.
    if(nbChanges > 0){
      send_spread(packet, packet->interface);
    }

  }
}


//Finding the max values of the network.
void network_max_values(){
  uint8_t leaderID = getLeaderID();
  int8_t* block = get_block_me();
  

  //At first, the module define the minI,minJ, maxI, maxJ.
  minI = block[I];
  minJ = block[J];
  maxI = block[I];
  maxJ = block[J];
  

  for(int i=0; i < NB_BLOCKS; i++){
    block = get_block(i);
    if((block[IN] == 1) && (block[LEADER] == leaderID )){
      if(block[I] > maxI)
	maxI = block[I];
      if(block[J] > maxJ)
	maxJ = block[J];
      if(block[I] < minI)
	minI = block[I];
      if(block[J] < minJ)
	minJ = block[J];
    }
  }
}

//Receive a MISSING packet
//Spreading the news.
void network_missing_receive(Packet * packet){
  uint8_t lost_block = packet->data[0];

  //In reality, the lost_block is in front of me. I don't spread the news.
  if(ID_in_interfaces(lost_block))
    return;
  
  //Cutting the branch of the lost block
  if(block_in(lost_block)){
    xSemaphoreTake(resetSemaphoreAck, 0);
    xSemaphoreGive(resetSemaphore);
    
    network_cut_branch(lost_block);  
    send_spread(packet, packet->interface);
  }
 
}



// The first to notice the missing of one block, sends it to everyone.
void network_reset(uint8_t neighbor, int badInterface){
 
  //When we find someone missing, we change awareOfMissingBlock to 1 so that we won't spread the packet uselessly.
  if(awareOfMissingBlock == 0){
    awareOfMissingBlock = 1;
    //Giving the semaphore to the network_reset_task.
    xSemaphoreTake(resetSemaphoreAck, 0);
    xSemaphoreGive(resetSemaphore );     
    
    //Cutting branch and giving news to others.
    network_cut_branch(neighbor);
    send_new_packet(MISSING,(uint8_t *) &neighbor, SPREAD + badInterface);
  }
}


void network_reset_task(void* parameters){
   int8_t myID;

   while(1){
     
     xSemaphoreTake(resetSemaphore, portMAX_DELAY);
     vTaskDelay(RESET_TIME);

     myID = getMyID();
     //The block is alone, so it resets his network table and his state table.
     if(interfaces_nb_neighbors() == 0){
       states_init();
       set_blocks_out();
       set_block_in(myID);
       set_block_position(myID, 0,0);
       set_block_leader(myID, myID);
       setLeaderID(myID);
     }
     awareOfMissingBlock = 0;
     xSemaphoreGive(resetSemaphoreAck);
   }
   
}



void network_cut_branch(int lost_block){

  int8_t myID = getMyID();

  //Setting the lost_block out and building the graph of connectivity. Members in network will change.
  set_block_out(lost_block);
  build_graph();
  build_modules();
  setMembersInNetwork(count_members());
  
  //The block is alone so it is the leader.
  if(getMembersInNetwork() == 1){
    set_state(myID, STATE_NULL);
    setLeaderID(myID);
  }
  else{

    //The leader has been lost, another one is elected
    if(!is_there_a_leader()){
      int8_t minID = myID;
      for(int i = minID-1; i>=0; i--)
	if(block_in(i))
	  minID = i;

      network_changeLeader(minID);
    }
  }

  
}


//Every block in the network is REJECTED and the leader is ELECTED.
void network_changeLeader(int leaderID){

  int8_t* block;

  for(int i=0; i < NB_BLOCKS; i++){
    block = get_block(i);
    if(block[IN] == 1){ //Modules is in the good network.
      set_block_leader(i, leaderID);
      set_state(i, STATE_REJECTED);
    }
  }

  setLeaderID(leaderID);
  set_state(leaderID, STATE_ELECTED); 
  setRightPosition(1);
}


