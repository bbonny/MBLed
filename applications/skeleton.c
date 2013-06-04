/*
 * skeleton.c
 *
 * This is a skeleton for an MB Led application
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

// All includes for an application
#include "application.h"
#include "skeleton.h"

// state of the game
static skeleton_state state;

// contains the image to display
static pixel_t current [64];

/* Init or reset the game parameters*/ 
void skeleton_reset(){
  lib_image_reset(current);
  set_image_pointer(current);
}
	    


/*
 * Init semaphores, queues, buzzer etc.
 *
 */
void skeleton_init(){ 
}
	    

/*
 * Send command to an other block
 *
 */ 
void skeleton_send_command(uint8_t direction, int8_t type, int8_t data_1, int8_t data_2){
  
  int8_t data[3];
  data[0] = type;
  data[1] = data_1;
  data[2] = data_2;
  send_new_packet(COMMAND, &data[0], direction, size_of_data, ttl);
}


/*
 * Receive command from other blocks
 */
void skeleton_receive_command(Packet * packet){
}

void skeleton_turn_right(){
}
void skeleton_turn_left(){
}
void skeleton_turn_180(){
}

/*
 * This function is called every (state.speed x 10-2) s 
 * It makes the application evolves
 */
void skeleton_step(){
  // modify the state
  
  // modify the displayed image
  
  return;  
}


/*
 * skeleton_command_task()
 * receive the command messages from queue
 *
 */
void skeleton_command_task(){
  for(;;){
    Packet p; 
    xQueueReceive(commandQueue, &p, portMAX_DELAY);   // wait for a new message
    skeleton_receive_command(&p);
  }
}



/*
 * skeleton_turn_task()
 * receive the messages from queue
 *
 */
void skeleton_turn_task(){
  for(;;){
    uint8_t rotation; 
    xQueueReceive(turnQueue, &(rotation), portMAX_DELAY);   // wait for a rotation of the block
    switch ((rotation+2)%4){
    case 1:
      skeleton_turn_right();
      break;
    case 2:
      skeleton_turn_180();
      break;
    case 3:
      skeleton_turn_left();
      break;
    default:
      break;
    }    
  }
}

/*
 * skeleton_task
 * Main task
 *
 */   
void skeleton_task(void *pvParameters){
  uint8_t my_id = (uint8_t) pvParameters;

  // Reset/init the application
  skeleton_reset();
  // Look after the turn messages
  xTaskCreate(skeleton_turn_task, ( signed portCHAR * ) "turn detection", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
  // Look after commands from other blocks
  xTaskCreate(skeleton_command_task, ( signed portCHAR * ) "command detection", configMINIMAL_STACK_SIZE, NULL, 1, NULL );

  for(;;){
    if(current_demo == my_id)
      skeleton_step();
  }
}

		
