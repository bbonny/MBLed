/*
 * graph.c
 *
 * This module creates and handles a graph according to the network table and deletes wrong informations.
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

#include "constant.h"
#include "algo.h"
#include "network.h"
#include "position.h"
#include "debug.h"
#include "graph.h"
#include "ping.h"
#include "election.h"




typedef struct node{
  int8_t in;
  int8_t i,j;
  int8_t id;
  int8_t isLeader;
  struct node *node_north;
  struct node *node_east;
  struct node *node_south;
  struct node *node_west;
} node;


typedef node* graph;

node nodes[NB_BLOCKS]; // table storing the nodes of the graph
int8_t nodes_here[NB_BLOCKS]; // Used to store the present nodes
node* list[NB_BLOCKS];
int8_t indice;
graph my_graph;

int8_t modules_temp[NB_BLOCKS][4];

extern int8_t minI, maxI, minJ, maxJ;


// Privates function
/**********************************************************/
void copy_module ();
void  add_member ( int8_t id);
void add_neighbor ( int8_t* block, uint8_t id);
/**********************************************************/



void  add_member (int8_t id){
  list[indice] = &nodes[id];
  indice++;
}

// Function check for neigbors already included into the graph and add links between nodes
void add_neighbor ( int8_t* block, uint8_t id){
  int8_t k= 0;
  while (k < indice){
    if ((block[I] == list[k]->i) && (block[J] == (list[k]->j +1) )){
      nodes[id].node_south = &nodes[list[k]->id];
      nodes[list[k]->id] .node_north = &nodes[id];
    }
    else if ((block[I] == list[k]->i) && (block[J] == (list[k]->j - 1) )){
      nodes[id].node_north = &nodes[list[k]->id];
      nodes[list[k]->id] .node_south = &nodes[id];
    }
    else if ((block[I] == (list[k]->i +1) ) && (block[J] == list[k]->j  )){
      nodes[id].node_east = &nodes[list[k]->id];
      nodes[list[k]->id] .node_west = &nodes[id];
    }
    else if ((block[I] == (list[k]->i -1)) && (block[J] == list[k]->j )){
      nodes[id].node_west = &nodes[list[k]->id];
      nodes[list[k]->id] .node_east = &nodes[id];
    }
    k++;
  }
}

// Build graph by taking block one by one
void build_graph(){
  int8_t* block;
  int8_t b;
  indice = 0;
  for (b = 0; b< NB_BLOCKS; b++){
    block = get_block(b);
    if (block[IN] == 1){
      nodes[b].in = 1;
      nodes[b].id = b;
      nodes[b].i = block[I];
      nodes[b].j = block[J];
      nodes[b].node_north = NULL;
      nodes[b].node_south = NULL;
      nodes[b].node_east = NULL;
      nodes[b].node_west = NULL;
      if (LEADER == b)
	nodes[b].isLeader = 1;
      else
	nodes[b].isLeader = 0;
      add_neighbor( block,b);
      add_member(b);
    }
    else
      nodes[b].in = 0;
  }
  my_graph = &nodes[getMyID()];
}

// Function counting member into network
int8_t count_members (){
  static uint8_t list[NB_BLOCKS];
  uint8_t j = 0, k= 1;
  for (uint8_t i = 0; i < NB_BLOCKS ; i++){
    list[i]=0xff;
    nodes_here[i]=0;
  }
  list[0] = my_graph->id;
  nodes_here[my_graph->id]=1;

  while (list[j] != 0xff){

    if (nodes[list[j]].node_north != NULL){
      if(nodes_here[nodes[list[j]].node_north->id] == 0){
	list[k] = nodes[list[j]].node_north->id;
	k++;
	nodes_here[nodes[list[j]].node_north->id] = 1;
      }
    }
    if (nodes[list[j]].node_south != NULL){
      if( nodes_here[nodes[list[j]].node_south->id] ==0){
	list[k] = nodes[list[j]].node_south->id;
	k++;
	nodes_here[nodes[list[j]].node_south->id] = 1;
      }
    }

    if (nodes[list[j]].node_east != NULL){
      if( nodes_here[nodes[list[j]].node_east->id] == 0){
	list[k] = nodes[list[j]].node_east->id;
	k++;
	nodes_here[nodes[list[j]].node_east->id] = 1;
      }
    }
    if (nodes[list[j]].node_west != NULL){
      if( nodes_here[nodes[list[j]].node_west->id] ==0){
	list[k] = nodes[list[j]].node_west->id;
	k++;
	nodes_here[nodes[list[j]].node_west->id] = 1;
      }
    }
    j++;
  }
  return j ;
}

// Check if the leader still in the network
int8_t is_there_a_leader  (){
  static uint8_t list[NB_BLOCKS];
  uint8_t j = 0, k= 1;
  uint8_t leader = getLeaderID();

  if(isLeader())
    return 1;
  
  for (uint8_t i = 0; i < NB_BLOCKS ; i++){
    list[i]=0xff;
    nodes_here[i]=0;
  }
  
  list[0] = my_graph->id;
  nodes_here[my_graph->id]=1;

  if (isLeader())
    return 1;

  while (list[j] != 0xff){

    if (nodes[list[j]].node_north != NULL){
      if(nodes_here[nodes[list[j]].node_north->id] == 0){
	if (leader == nodes[list[j]].node_north->id )
	  return 1;
	list[k] = nodes[list[j]].node_north->id;
	k++;
	nodes_here[nodes[list[j]].node_north->id] = 1;
      }
    }
    if (nodes[list[j]].node_south != NULL){
      if( nodes_here[nodes[list[j]].node_south->id] ==0){
	if (leader == nodes[list[j]].node_south->id )
	  return 1;
	list[k] = nodes[list[j]].node_south->id;
	k++;
	nodes_here[nodes[list[j]].node_south->id] = 1;
      }
    }

    if (nodes[list[j]].node_east != NULL){
      if( nodes_here[nodes[list[j]].node_east->id] == 0){
	if (leader == nodes[list[j]].node_east->id )
	  return 1;
	list[k] = nodes[list[j]].node_east->id;
	k++;
	nodes_here[nodes[list[j]].node_east->id] = 1;
      }
    }
    if (nodes[list[j]].node_west != NULL){
      if( nodes_here[nodes[list[j]].node_west->id] ==0){
	if (leader == nodes[list[j]].node_west->id )
	  return 1;
	list[k] = nodes[list[j]].node_west->id;
	k++;
	nodes_here[nodes[list[j]].node_west->id] = 1;
      }
    }
    j++;
  }
  return 0 ;
}


void graph_check_neighbor(){
  int8_t* me = get_block_me();
  int8_t neighbor_table[4] = {-1,-1,-1,-1}; 
  uint8_t counter = 0;
  static Interfaces interfaces;
  uint8_t nb_diff = 0;
  int8_t* block;
  interfaces = getInterfaces();

  for(int8_t i=0; i<NB_BLOCKS; i++){
    block = get_block(i);

    if(block[IN]==1){
      // add the north block
      if( (block[I] == me[I]-1) && (block[J] == me[J]) ){
	neighbor_table[NORTH]= i;
	counter++;
      }
      // add the south block
      if((block[I] == me[I]+1) && (block[J] == me[J])){
	neighbor_table[SOUTH]= i;
	counter++;
      }
      // add the east block
      if((block[I] == me[I]) && (block[J] == me[J]+1)){
	neighbor_table[EAST]= i;
	counter++;
      }
      // add the west block
      if((block[I] == me[I]) && (block[J] == me[J]-1)){
	neighbor_table[WEST]= i;
	counter++;
      }
   
    }
  }

  for(int i=0; i<=3; i++){

    if(getRightPosition() == 1){
      if(interfaces.ID[i] != neighbor_table[i]){
	//Not coherent.
	nb_diff++;
	if(neighbor_table[i] >= 0)
	  set_block_out(neighbor_table[i]);
      }
    }
  }

  //There are people but I don't notice them or i'm alone and i'm not leader.
  if(getMembersInNetwork() == 1)
    {
      if(interfaces_nb_neighbors() != 0)
	nb_diff++;

      if(!isLeader()){
	setLeaderID(getMyID());
	set_block_leader(getMyID(),getMyID());
	nb_diff++;
      }
    }
  

  if(nb_diff>= 1){
    if(getStillInTheElection() != 1){
      setPingOut();
      build_graph();
      build_modules();
      
      if(getLeaderID() != -1)
	states_elected(getLeaderID());
    }
  }
}

// this function update the block table 
void build_modules(){
  static uint8_t list[NB_BLOCKS];
  uint8_t j = 0, k= 1;

  // Init modules_temp
  for(int8_t i=0; i<NB_BLOCKS; i++){
    modules_temp[i][I] = 0;
    modules_temp[i][J] = 0;
    modules_temp[i][IN] = 0;
    modules_temp[i][LEADER] = 0;
  }
  
  for(int8_t i=0; i<NB_BLOCKS; i++){
    nodes_here[i] = 0;
    list[i] = 0xff;
  }
  
  list[0] = my_graph->id;
  while (list[j] != 0xff){
    /* add the node to the module table */
    modules_temp[list[j]][IN] = 1;
    modules_temp[list[j]][I] = nodes[list[j]].i;
    modules_temp[list[j]][J] = nodes[list[j]].j;
    modules_temp[list[j]][LEADER] = getLeaderID();
    
    if (nodes[list[j]].node_north != NULL){
      if(nodes_here[nodes[list[j]].node_north->id] == 0){
	list[k] = nodes[list[j]].node_north->id;
	k++;
	nodes_here[nodes[list[j]].node_north->id] = 1;
      }
    }
    if (nodes[list[j]].node_south != NULL){
      if( nodes_here[nodes[list[j]].node_south->id] ==0){
	list[k] = nodes[list[j]].node_south->id;
	k++;
	nodes_here[nodes[list[j]].node_south->id] = 1;
      }
    }

    if (nodes[list[j]].node_east != NULL){
      if( nodes_here[nodes[list[j]].node_east->id] == 0){
	list[k] = nodes[list[j]].node_east->id;
	k++;
	nodes_here[nodes[list[j]].node_east->id] = 1;
      }
    }
    if (nodes[list[j]].node_west != NULL){
      if( nodes_here[nodes[list[j]].node_west->id] ==0){
	list[k] = nodes[list[j]].node_west->id;
	k++;
	nodes_here[nodes[list[j]].node_west->id] = 1;
      }
    }
    j++;
  }
  copy_module();
} 


void copy_module (){
  // copy modules_temp into modules

  for(int i=0; i<NB_BLOCKS; i++){
    set_block_position(i, modules_temp[i][I], modules_temp[i][J]);
    if(modules_temp[i][IN]){
      set_block_in(i);
    }
    else{
      set_block_out(i);
    }
    set_block_leader(i, modules_temp[i][LEADER] );
  }

}

