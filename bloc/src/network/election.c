/*
 * election.c
 *
 * This module handles the election of a leader in the network
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
#include <queue.h>

#include "constant.h"
#include "packet.h"
#include "receive.h"
#include "election.h"
#include "network.h"
#include "position.h"
#include "send.h"
#include "debug.h"
#include "algo.h"

xQueueHandle xElectionQueue;

//Remembering the election-state of the blocks
static uint8_t states[NB_BLOCKS];

//When starting election, stillInTheElection = 1
//When receiving REJECTED MYID, stillInTheElection turns to 0.
//If it stays at 1, the module is the leader!
static uint8_t stillInTheElection;

//For the debug.
static uint8_t electionID;


void election_init(){
  //Queue for the election : when a character is added to this queue, the election starts. 
  xElectionQueue = xQueueCreate(1, sizeof(uint8_t));
  states_init();
}


void setStillInTheElection(int i){
  stillInTheElection = i;
}

uint8_t getStillInTheElection(){
  return stillInTheElection;
}

//Functions for the table of states.

void states_init(){
  for(int i=0; i<NB_BLOCKS;i++){
    states[i] = STATE_NULL;
  }
}

void set_state(int block, int new_state){
  if(block_ok(block))
    states[block] = new_state;
}

int get_state(int block){
  if(block_ok(block))
    return states[block];
  else
    return STATE_NULL;
}

void states_print(int block){
  if(block_ok(block)){
    switch(states[block]){
    case STATE_NULL:
      writeDebugInit("State : Unknown");
      break;
    case STATE_CANDIDATE:
      writeDebugInit("State : Candidate");
      break;
    case STATE_REJECTED:
      writeDebugInit("State : Rejected");
      break;
    case STATE_ELECTED:
      writeDebugInit("State : Elected");
    break;
    default:
      writeDebugInit("State : ?");
      break;
    }
  }
}


void states_elected(int block){
  if(block_ok(block)){

    for(int i=0; i<NB_BLOCKS;i++){
      if(states[i] > STATE_START){
	states[i] = STATE_REJECTED;
      }
    }
    states[block] = STATE_ELECTED;
  }

}




//The task of the election : it's a queue receive plus a timer.

void election_timer(void* parameters){
  uint8_t mess;
  electionID = 0;

  while(1){    
    xQueueReceive(xElectionQueue, &mess, portMAX_DELAY);
    
    //We fill the queue so that no message can be added
    xQueueSend(xElectionQueue, &mess,0);
    electionID++;
   
    vTaskDelay(TEMP_WAIT);
   
    


    //Setting winning election to 1 before the election
    setStillInTheElection(1);

    if(get_state(getMyID()) == STATE_NULL){
      set_state(getMyID(), STATE_START);
      }
    //Sending this to others.
    election_start();
    
    

    vTaskDelay(ELECTION_TIME); // Timer start!
      
    //If winning election is still at 1, we have won!
    if(getStillInTheElection() == 1){
      election_end();
    }
    
    setStillInTheElection(0);
    xQueueReceive(xElectionQueue, &mess, portMAX_DELAY);
  
   }
}



/*
  End of the election : the winner sends his informations to the others.
*/
void election_end(){
  
  uint8_t data[2];
  int8_t myID = getMyID();
  int8_t leaderID = getLeaderID();
  
  if(leaderID == -1)
    return;

  //Sending the result of the election.
  data[0] = leaderID;
  data[1] = getMembersInNetwork();
  send_new_packet(ELECTED, data, EVERYBODY);
 
  vTaskDelay(IRDA_WAIT);
  states_elected(leaderID);
 


  //Setting my position to (0,0)
  if(myID == leaderID)
    set_block_position(myID, 0,0);


  //Sending who is in the network.
  for(int interface=0;interface<4; interface++){
    network_send(interface);
  }
  send_time(EVERYBODY);


  //Sending the position to everyone.
  BROADCAST_POSITION;

}


//The main function of the election.
//Each time a block receive an election packet,  it analyses if the candidature is ok or not then it spreads the info to his neighbor.
void election_main(Packet* packet){
  
  uint8_t mode = packet->mode;  
  int8_t block, membersInHisNetwork;
  int8_t leaderID = getLeaderID();
  uint8_t stateOfBlock;

  if(mode > START){
    block = packet->data[0];
    membersInHisNetwork = packet->data[1];
  }
  else{
    block = 0;
    membersInHisNetwork = 0;
  }

  //Sometimes, a wrong packet is sent so we stop it.
  if(block == -1){
    return;
  }
  //Current state of block.
  stateOfBlock = get_state(block);
  
  switch(mode){
  case(START) : //Someone wants to start the election.
    election_start();
    break;

  case(CANDIDATE): 
    if(stateOfBlock < STATE_CANDIDATE){
      election_candidature(packet);
    }
    break;

  case(REJECTED):
    if(stateOfBlock < STATE_REJECTED || block == leaderID){
     
      //The block's leader is rejected...
      if(block == leaderID){

	setRightPosition(0);
	//Stopping the timer and setting no leader.
	setStillInTheElection(0);
	setLeaderID(-1);
      }
      send_spread(packet, packet->interface);
      stateOfBlock = STATE_REJECTED;

    }   
    break;

  case(ELECTED): //Someone has been elected.

    if(stateOfBlock < STATE_ELECTED){
      
      setLeaderID(block);
      setDirectionID(0); //directionID serves to have a good Position.
      send_spread(packet, packet->interface);
      states_elected(block);
    }
    else states_print(block);

    break;
  }

}

//Analysing a candidature.

void election_candidature(Packet* packet){

  uint8_t hisLeader = packet->data[0];
  uint8_t membersInHisNetwork = packet->data[1];
  int8_t membersInMyNetwork = getMembersInNetwork();
  int8_t leaderID = getLeaderID();

  set_state(hisLeader, STATE_CANDIDATE);

  //The current block is in a network so maybe a decision can be taken without an election.
  if(membersInMyNetwork > 1){ 

    //The current block's network is the most powerful, it wins.
    if(membersInMyNetwork > membersInHisNetwork){ 
      election_send_network(hisLeader, membersInHisNetwork, packet->interface);
    }
    //Same number of members but the ID of the block's leader is smaller : we win.
    else if((membersInMyNetwork == membersInHisNetwork) && (leaderID < hisLeader) && (leaderID >= 0)){
      election_send_network(hisLeader, membersInHisNetwork, packet->interface);
    }
  }
  //The block is alone.
  else{
    //The other one is also alone.
    if(membersInHisNetwork == 1) {
      //The block's ID is smaller : he wins.
      if((hisLeader > leaderID) && (leaderID >= 0)){
	send_new_packet(REJECTED, packet->data, EVERYBODY);
	set_state(hisLeader, STATE_REJECTED);
      }
      else{
	send_spread(packet, packet->interface);
      }
    } 
  }  
}


//Spreading to everyone the start of the election.
void election_start(){
  
  int8_t to;
  uint8_t data[2];
  int8_t membersInNetwork = getMembersInNetwork();
  int8_t leaderID = getLeaderID();

  if(get_state(getMyID()) == STATE_START){ //The block don't know anything about election.
    for(int interface=0; interface<4; interface++){ 
      to = interface_neighbor(interface);
      if(to != -1){ //There is a neighbor.
	if(get_state(to) == STATE_NULL){ 	  //"to" isn't aware of the election (or the block isn't aware of his status)
	  send_new_packet(START, (uint8_t *) NULL, interface);
	}
      }
    }  

    if(leaderID != -1){

      //The block is candidate.
      data[0] = leaderID;
      data[1] = membersInNetwork;
      
      send_new_packet(CANDIDATE, data, EVERYBODY);
      set_state(getMyID(), STATE_CANDIDATE);
    }
    else
      setStillInTheElection(0);
  }

  else if(membersInNetwork > 1){ //The block is already in a network!
  
    for(int interface=0; interface < 4; interface++){ 
  
      to = interface_neighbor(interface);
      if(to != -1){ //There is a new neighbor.
	if(!block_in(to)){ //This neighbor is unknown to the block : START + CANDIDATE.
	  send_new_packet(START, (uint8_t *) NULL, interface);

	  if(leaderID != -1){

	    data[0] = leaderID;
	    data[1] = membersInNetwork;
	    
	    send_new_packet(CANDIDATE,data,interface);
	  }
	  else{
	    setStillInTheElection(0);
	  }
 	}
      }
    }
  }  
}

void election_send_network(int block, int membersInHisNetwork, int interface){

 
  if(getLeaderID() == -1)
    return;


  uint8_t data[2];

   // Timer stop.
  setStillInTheElection(0);

  //Sometimes, we want to send this to a block in our network (when blocks are moving quickly)
  if(block != getLeaderID()){

    // Send REJECTED BLOCK to interface.
    data[0] = block;
    data[1] = membersInHisNetwork;
    
    send_new_packet(REJECTED,data,interface);
  }

  // Send ELECTED LEADER_ID to interface
  data[0] = getLeaderID();
  data[1] = getMembersInNetwork();

  send_new_packet(ELECTED, data, interface);

  // Send list of positions.
  network_send(interface);
  send_time(interface);


  //Waiting for the other to know the network!
  vTaskDelay(TEMP_WAIT / 2);
  // Send his new position.
  position_send(interface, TTL);

  set_state(block, STATE_REJECTED);
}


