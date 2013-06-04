/*
 * algo.c
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


#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include "irda.h" 
#include "send.h"
#include "packet.h"
#include "election.h"
#include "position.h"
#include "algo.h"
#include "debug.h"
#include "synchro.h"
#include "ping.h"
#include "network.h"
#include "turn.h"
#include "graph.h"

static int8_t myID,leaderID,direction, membersInNetwork;

static Interfaces interfaces;
static Interfaces savedInterfaces;


#ifndef MBLED
static const uint8_t uarts[4] = {1,4,2,3};
static const uint32_t table_ID[20] = {0x43193038,
                                      0x43193046,
                                      0x43193131,
                                      0x43193432,
                                      0x43193433,
                                      0x43193434,
                                      0x43193437,
                                      0x43193764,
                                      0x43193841,
                                      0x43202550,
                                      0x43203148,
                                      0x43233356,
                                      0x43234646,
                                      0x43234962,
                                      0x43252542,
                                      0x43252544,
                                      0x43252640,
                                      0x43256454,
                                      0x43193839,
                                      0x43193436};
#define NB_TABLE 20
#else
static const uint8_t uarts[4] = {2,5,4,3};
#define NB_TABLE 18
static const uint32_t table_ID[NB_TABLE] = {0x43233251,
                                            0x43243839,
					    0x43108711,
					    0x43087113,
					    0x43123451,
					    0x43243949,
					    0x43088812,
					    0x43233230,
					    0x43087813,
					    0x43243549,
					    0x43108111,
					    0x43146128,
					    0x43243733,
					    0x43086316,
					    0x43243857,
					    0x43086115,
					    0x43088412,
					    0x43233250};
#endif


//GET/SET for my ID


int8_t which_is_myID() {
  uint32_t myID = ID_ADDR;
  for (int i=0;i< NB_TABLE;i++)
    if (table_ID[i] == myID)
      return ((int8_t) (i+1));
  return 0;
}


int8_t getMyID(){
  return myID;
}


//Functions for the leaderID

void setLeaderID(int8_t newLeader){
  if(newLeader >= -1 && newLeader < NB_BLOCKS){
    leaderID = newLeader;

  }
}

int8_t getLeaderID(){
  return leaderID;
}

int8_t isLeader(){
  if(myID == leaderID)
    return 1;
  return 0;

}

int8_t isLeaderOfANetwork(){
  if(getMembersInNetwork() > 1 && isLeader())
    return 1;
  return 0;
}




//Functions for the direction

void setDirection(int8_t newDirection){
  newDirection = newDirection % 4;
  if(newDirection < 0) newDirection = newDirection + 4;
  direction = newDirection;
  shift_direction_table(direction);
}

//We add a rotation to the current direction.
void addDirection(int8_t rotation){
  setDirection( (direction + rotation) % 4 );
}

int8_t getDirection(){
  return direction;
}


//Functions for the members in network.

int8_t getMembersInNetwork(){
  return membersInNetwork;
}

void setMembersInNetwork(int8_t newNumber){
  if(newNumber >= 0)
    membersInNetwork = newNumber;
}

void addMemberInNetwork(){
  membersInNetwork++;
}

void delMemberInNetwork(){
  membersInNetwork--;
}


//Functinos for the interfaces

//Checking the interfaces.
int interface_ok(int i){
  if(i >= 0 && i <= 3)
    return 1;

  return 0;
}


Interfaces getInterfaces(){
  return interfaces;
}

Interfaces getSavedInterfaces(){
  return savedInterfaces;
}

//Changing something on one of the interfaces

void setInterface(int8_t interface, int8_t newNeighbor, int8_t hisInterface ){
  if(interface_ok(interface)){
    if(newNeighbor != -1){
      interfaces.ready[interface] = 1;
    }
     else{
      interfaces.ready[interface] = 0;
    }
    
    interfaces.hisInterface[interface] = hisInterface;
    interfaces.ID[interface] = newNeighbor;
  }   
}

int8_t interface_neighbor(int interface){
  if(interface_ok(interface))
    return interfaces.ID[interface];
  return -1;
}


int8_t interface_where_is_neighbor(int neighbor, int myInterfaces){
  Interfaces testedInterfaces;

  if(myInterfaces == 1)
    testedInterfaces = getInterfaces();
  else
    testedInterfaces = getSavedInterfaces();


  for(int i=0; i < 4; i++){
    if(testedInterfaces.ID[i] == neighbor)
      return i;
  }
  return -1;
}

int8_t interface_hisInterface(int interface){
  if(interface_ok(interface))
    return interfaces.hisInterface[interface];
  return -1;
}

int8_t interface_is_busy(int interface)
{
  if(interface_ok(interface))
    return interfaces.ready[interface];
  return 0;
}

int8_t interfaces_nb_neighbors(){
  int nb=0;
  for(int i=0; i <4 ; i++){
    if(interface_is_busy(i))
      nb++;
  }  
  return nb;
}


//Returns which interfaces are busy : it's a table of 5 max.
// NB_BUSY 1ST_BUSY 2ND_BUSY ...
void interfaces_which_are_busy(int8_t* interfaces_busy){
  int nb=0;
  for(int i=0; i <4 ; i++){
    if(interface_is_busy(i)){
      interfaces_busy[nb + 1] = i;
      nb++;
    }
  } 
  interfaces_busy[0] = nb;
  
}


//Remembrance of the interfaces.
void interfaces_copy(){
  for(int i=0; i < 4; i++){
    savedInterfaces.ready[i] = interfaces.ready[i];
    savedInterfaces.ID[i] = interfaces.ID[i];
    savedInterfaces.hisInterface[i] = interfaces.hisInterface[i];
  }
}



//Returns 1 if the module was there before.
//Returns 0 if not.
int ID_was_in_interfaces(int hisID){
  for(int i=0; i <4; i++){
    if(savedInterfaces.ID[i] == hisID)
      return 1;
  }  
  return 0;
}

int ID_in_interfaces(int hisID){
 for(int i=0; i <4; i++){
    if(interfaces.ID[i] == hisID)
      return 1;
  }  
  return 0;
}

//This function returns the nb of 90° rotation
//Returns -1 if there is no rotation.
int8_t interfaces_turn(Interfaces before, Interfaces after){

  //I represents the turn (i=0 -> No turn / i=1 -> Quarter. etc.)
  for(int i=0; i <4; i++){
    int interfacesMatch = 1;
    //J represents the interfaces
    for(int j=0; j <4; j++){
      
      //One interface isn't matching.
      if(before.ID[(i+j)%4] != after.ID[j]){
	interfacesMatch = 0;
      }
    }

    //All the interfaces are the same.
    if(interfacesMatch == 1){
      return i;
    }
  }
  return -1;

}

void interfaces_show(Interfaces interfaces){

  writeHexDebug(interfaces.ID[0]);
  writeDebugInit(" ");
  writeHexDebug(interfaces.ID[1]);
  writeDebugInit(" ");
  writeHexDebug(interfaces.ID[2]);
  writeDebugInit(" ");
  writeHexDebug(interfaces.ID[3]);
  writeDebugInit("\r\n");

}


void interfaces_show_ready(Interfaces interfaces){

  writeHexDebug(interfaces.ready[0]);
  writeDebugInit(" ");
  writeHexDebug(interfaces.ready[1]);
  writeDebugInit(" ");
  writeHexDebug(interfaces.ready[2]);
  writeDebugInit(" ");
  writeHexDebug(interfaces.ready[3]);
  writeDebugInit("\r\n");

}




uint8_t uart2interface(uint8_t uart){  
  for(int i=0; i<4; i++){
    if(uart == uarts[i])
      return i;
  }
  writeDebugInit("---------------------------\r\n");
  writeDebugInit("UART Error \r\n");
  writeHexDebug(uart);
  writeDebugInit("--------------------------\r\n");
  return -1;
}

uint8_t interface2uart(uint8_t interface){
  if((interface >= 0) & (interface < 4)){
    return uarts[interface];
  }
    writeDebugInit("---------------------------\r\n");
    writeDebugInit("Interface Error \r\n");
    writeHexDebug(interface);
    writeDebugInit("--------------------------\r\n");
    return -1;
}




//Initialization of the algorithm.
void algo_init(){
  setPacketID(0);
  setDirection(0);
  myID = which_is_myID();

  setLeaderID(myID);

  //Init of the election task (states null, queue)
  election_init(); 
  //Nobody in the network.
  network_init();

  //Resetting the interfaces.
  for(int i=0; i < 4; i ++){
    setInterface(i, -1, -1);
  }
  //The "saved" interfaces are resetted.
  interfaces_copy();

  turn_init();
  ping_init(); 
 
}



void algo_task_create(){

  xTaskCreate(leader_task, (signed portCHAR *) "leader", 
	      4*configMINIMAL_STACK_SIZE, NULL,  
	      tskIDLE_PRIORITY + 1, NULL);

  xTaskCreate(network_reset_task, (signed portCHAR *) "reset", 
	      configMINIMAL_STACK_SIZE, NULL,  
	      tskIDLE_PRIORITY + 1, NULL); 

  xTaskCreate(pong_task, (signed portCHAR *) "PONG", 
	      configMINIMAL_STACK_SIZE, NULL,  
	      configMAX_PRIORITIES - 1, NULL); 

  xTaskCreate(election_timer, (signed portCHAR *) "Election", 
	      configMINIMAL_STACK_SIZE, NULL, 
	      tskIDLE_PRIORITY + 1, NULL);

  xTaskCreate(turn_task, (signed portCHAR *) "Turn", 
	      configMINIMAL_STACK_SIZE, NULL, 
	      tskIDLE_PRIORITY + 1, NULL);

}
