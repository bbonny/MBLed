/*
 * priorityqueue.h
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

#ifndef H_PRIORITY
#define H_PRIORITY

#define SIZE_OF_PQ   ((uint16_t )1024)


// Heap structure
typedef struct
{
  Packet* priority_queue;
  short N;
}heap;


void pq_init (heap* pq);

// Insert packet in priority queue
void pq_insert_packet (Packet* p_in, heap* pq);

// Get first element
Packet* pq_get_packet (heap* pq);

// Extract head of the heap, and reconstruct it.
void pq_extract_head (heap* pq);

// Mantain max-heap property
void pq_max_heapify (uint16_t i, heap* pq);

// Exchanging two element in a Packet array
void pq_exchange (int i, int j, Packet t[]);



#endif
