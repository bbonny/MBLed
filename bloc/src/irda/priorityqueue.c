/*
 * priorityqueue.c
 *
 * ( descripts the module here ) 
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


#include "packet.h"
#include "priorityqueue.h"

void pq_init (heap* pq){
  Packet temp_pq[(SIZE_OF_PQ)];
  pq->priority_queue = temp_pq;
  pq->N= 0;
}

// Insert packet in priority queue
void pq_insert_packet (Packet* p_in, heap* pq){
  uint16_t k= 0;
  pq->N++;
  while ((k!= 0) && (pq->priority_queue[k].priority < pq->priority_queue[((k+1)/2)-1].priority)){
    pq_exchange (k, (k+1/2)-1, pq->priority_queue);
    k=(k/2 )%SIZE_OF_PQ;
  }
}

// Get first element
Packet* pq_get_packet (heap* pq){
  return &pq->priority_queue[0];
}


void pq_extract_head (heap* pq){
  pq->N--;
  pq_exchange(0,pq->N,pq->priority_queue);
  pq_max_heapify(0,pq);
}


void pq_max_heapify (uint16_t i, heap* pq){
  uint16_t l= 2*(i+1)-1, r = 2*(i+1);
  uint16_t largest;
  if ((l < pq->N) && (pq->priority_queue[l].priority > pq->priority_queue[i].priority ))
    largest = l;
  else
    largest = i;
  if ((r < pq->N) && (pq->priority_queue[r].priority > pq->priority_queue[largest].priority ))
    largest = r;
  if (largest != i){
    pq_exchange(i, largest, pq->priority_queue);
    pq_max_heapify(largest,pq);
  }
}


// Exchanging two element in a Packet array
void pq_exchange (int i, int j, Packet priority_queue[])
{
  Packet temp = priority_queue[i];
  priority_queue[i]=priority_queue[j];
  priority_queue[j]=temp;
}

