/*
 * snake.c
 *
 * Snake game application 
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

#include "application.h"
#include "snake.h"
#include "snake_sound.h"

// state of the game
snake_state state;

// contains the image to display
static pixel_t current [64];

// used to erase the snake queue
static int8_t cpt_step, erase_on, cpt_erase;
// used to detect an illegal move
static int8_t enter_interface, from_block;
// used to keep or remove the apple on the block
static int8_t cpt_apple;

/*
 * set_snake_here()
 *
 * Indicates that the block is on the block 
 *
 */
void set_snake_here(){
  state.isSnakeHere = 1;
}

/* Init or reset the game parameters*/ 
void snake_reset(){
  state.isSnakeHere = 0;  
  erase_on = 0;
  cpt_step = 0; 
  cpt_erase = 0;
  state.i = 3;
  state.j = 3;
  state.size = 1;
  from_block = 0;
#ifdef MBLED
  snake_play_game_over();
#endif
  cpt_apple = 0;
  state.current_apple_i = -1;
  state.current_apple_j = -1;  
  state.speed = SNAKE_BASIC_SPEED;    // Speed of the snake 
  state.hasTurned = SNAKE_TURN_NONE;  // has the block turned between two steps? 
  state.color = 0xff0;                // head is yellow
  lib_image_reset(current);           // Init current with 0
  set_image_pointer(current);         // set current as the image to be displayed
  state.direction = SNAKE_DIR_NS;
}
	    


/*
 * Init sound
 *
 */
void snake_init(){ 
#ifdef MBLED
  snake_init_sound();  
#endif
}
	    

/*
 * Send the snake to an other block
 *
 */ 
void snake_send_snake(uint8_t direction, uint8_t myInt){
  uint8_t data[5];  
  data[0] = SNAKE_COMMAND_SNAKE;
  data[1] = myInt;  
  data[2] = direction;  
  data[3] = state.size;
  data[4] = state.speed;
  for(int i=0; i<5; i++){
    send_new_command(COMMAND, &data[0], direction, 5, 1);
    vTaskDelay(20);
  }
}

/*
 * Receive the snake from an other block
 */
void snake_receive_snake(Packet * packet){
  lib_image_reset(current);
  cpt_step   = 0;
  erase_on   = 0;
  cpt_erase  = 0;
  from_block = 1;
  lib_image_set_pixel(current,0x00f, state.current_apple_i, state.current_apple_j);     
  
  state.size = packet->data[3];
  state.speed = packet->data[4];  
  enter_interface = packet->interface;
  
  switch(packet->interface){
  case SNAKE_EAST:
    state.j = 0;
    state.direction = SNAKE_DIR_EW;     
    switch (packet->data[2]){
    case SNAKE_SOUTH:
      state.i = 7-packet->data[1];
      break;
    case SNAKE_WEST:
      state.i = packet->data[1];
      break;
    case SNAKE_NORTH:
      state.i = packet->data[1];
      break;
    case SNAKE_EAST:
      state.i = 7-packet->data[1];
      break;
    }
    break;
  case SNAKE_SOUTH:
    state.i = 7;
    state.direction = SNAKE_DIR_SN; 
    switch (packet->data[2]){
    case SNAKE_SOUTH:
      state.j = 7-packet->data[1];
      break;
    case SNAKE_WEST:
      state.j = 7-packet->data[1];
      break;
    case SNAKE_NORTH:
      state.j = packet->data[1];
      break;
    case SNAKE_EAST:
      state.j = 7-packet->data[1];
      break;
    }
    break;
  case SNAKE_WEST:
    state.j = 7;
    state.direction = SNAKE_DIR_WE; 
    switch (packet->data[2]){
    case SNAKE_SOUTH:
      state.i = packet->data[1];
      break;
    case SNAKE_WEST:
      state.i = 7-packet->data[1];
      break;
    case SNAKE_NORTH:
      state.i = 7-packet->data[1];
      break;
    case SNAKE_EAST:
      state.i = packet->data[1];
      break;
    } 
    break;
  case SNAKE_NORTH:
    state.i = 0;
    state.direction = SNAKE_DIR_NS; 
    switch (packet->data[2]){
    case SNAKE_SOUTH:
      state.j = packet->data[1];
      break;
    case SNAKE_WEST:
      state.j = 7-packet->data[1];
      break;
    case SNAKE_NORTH:
      state.j = 7-packet->data[1];
      break;
    case SNAKE_EAST:
      state.j = packet->data[1];
      break;
    } 
    break;
  }
  lib_image_set_pixel(current, 0x0f0, state.i, state.j);
  state.isSnakeHere = 1; 
}


/*
 * Change the direction when a turning is detected
 */
void snake_turn_right(){
  switch(state.direction){
  case SNAKE_DIR_NS:
    state.direction = SNAKE_DIR_EW;
    break;
  case SNAKE_DIR_SN:
    state.direction = SNAKE_DIR_WE;
    break;
  case SNAKE_DIR_EW:
    state.direction = SNAKE_DIR_SN;
    break;
  case SNAKE_DIR_WE:
    state.direction = SNAKE_DIR_NS;
    break;
  }
}

/*
 * Change the direction when a turning is detected
 */
void snake_turn_left(){
  switch(state.direction){
  case SNAKE_DIR_NS:
    state.direction = SNAKE_DIR_WE;
    break;
  case SNAKE_DIR_SN:
    state.direction = SNAKE_DIR_EW;
    break;
  case SNAKE_DIR_EW:
    state.direction = SNAKE_DIR_NS;
    break;
  case SNAKE_DIR_WE:
    state.direction = SNAKE_DIR_SN;
    break;
  }
}

/*
 * This function is called every (state.speed x 10-2) s 
 * It makes the snake move:
 * -  eat apples
 * -  erase the queue
 * -  send the snake to an other block
 */
void snake_move_snake(){
  cpt_apple++;
  
  // Build apples
  if(cpt_apple%11 == 10){
    //erase the previous apple
    lib_image_set_pixel(current,0,state.current_apple_i, state.current_apple_j);     
    // create a new apple
    state.current_apple_i = current_time()%6 + 1;
    state.current_apple_j = (current_time()*3)%6 + 1;
    lib_image_set_pixel(current,0x00f, state.current_apple_i, state.current_apple_j);     
  }
  
  /*erasing the queue*/
  state.old_i_position[cpt_step] = state.i; // save i and j position
  state.old_j_position[cpt_step] = state.j;
  cpt_step = (cpt_step+1) % 64;
  
  
  lib_image_set_pixel(current,0xf60, state.i, state.j);  // display the body
  

  if(erase_on){  // if erase on then errase the queue LED
    lib_image_set_pixel(current,0, state.old_i_position[cpt_erase], state.old_j_position[cpt_erase]);     
    cpt_erase ++;
  }
  else 
    if(!interface_is_busy(enter_interface) && (from_block==1)){ //You lose
      snake_reset();
    }
  
  if(cpt_step==state.size){
    cpt_erase = 0;
    erase_on = 1;
  }

   

  if(state.isSnakeHere){
    switch (state.direction){
      /*******************************************************************************/
    case SNAKE_DIR_NS:    // If the snake is going to the south   
      if((state.i+1==state.current_apple_i) && (state.j==state.current_apple_j)){ // if there is an apple on the way then size ++
	 state.size = state.size + 1;
	 state.current_apple_j = -1;
	 state.current_apple_i = -1;
	 snake_play_apple();
	 cpt_erase--;  
      }
	 
      if(state.i<7)// If the snake is always on the block after the move
	state.i = state.i + 1;
      else {
	if(interface_is_busy(SNAKE_SOUTH)){   // If there is a south block
	  state.isSnakeHere = 0;
	  snake_send_snake(SNAKE_SOUTH, state.j);  // Send him speed/i/j
	}
	else{ //return to initial position
	  snake_reset();
	  state.isSnakeHere = 1;
	}
      }
      break;
      
      /*******************************************************************************/
    case SNAKE_DIR_SN:    // If the snake is going to the north   
      if((state.i-1==state.current_apple_i) && (state.j==state.current_apple_j)){ // if there is an apple on the way then size ++
	state.size = state.size + 1;
	state.current_apple_j = -1;
	state.current_apple_i = -1;
	snake_play_apple();
	cpt_erase--;
      }
	 
      if(state.i>0)// If the snake is always on the block after the move
	state.i = state.i - 1;
      else {
	if(interface_is_busy(SNAKE_NORTH)){   // If there is a north block
	  state.isSnakeHere = 0;
	  snake_send_snake(SNAKE_NORTH, state.j); 	  // Send him speed/i/j	  
	}
	else{ //return to initial position
	  snake_reset();
	  state.isSnakeHere = 1;
	}
      }
      break;
      
      /*******************************************************************************/
    case SNAKE_DIR_EW:    // If the snake is going to the west   
      if((state.i==state.current_apple_i) && (state.j + 1 ==state.current_apple_j)){ // if there is an apple on the way then size ++
	state.size = state.size + 1;
	state.current_apple_j = -1;
	state.current_apple_i = -1;
	cpt_erase--;
	snake_play_apple();
      }
	 
      if(state.j<7)// If the snake is always on the block after the move
	state.j = state.j + 1;
      else {
	if(interface_is_busy(SNAKE_WEST)){   // If there is a west block
	  state.isSnakeHere = 0;
	  snake_send_snake(SNAKE_WEST, state.i); // Send him speed/i/j
	}
	else{  //return to initial position
	  snake_reset();	  
	  state.isSnakeHere = 1;
	}
      }
      break;
      
      /*******************************************************************************/
    case SNAKE_DIR_WE:    // If the snake is going to the east   
      if((state.i==state.current_apple_i) && (state.j-1==state.current_apple_j)){ // if there is an apple on the way then size ++
	state.size = state.size + 1;
	state.current_apple_j = -1;
	state.current_apple_i = -1;
	cpt_erase--;
	snake_play_apple();
      }
      if(state.j>0)// If the snake is always on the block after the move
	state.j = state.j - 1;
      else{ 
	if(interface_is_busy(SNAKE_EAST)){   // If there is a east block
	  state.isSnakeHere = 0;
	  snake_send_snake(SNAKE_EAST, state.i);  // Send him speed/i/j
	}
	else{ //return to initial position
	  snake_reset();	
	  state.isSnakeHere = 1;
	}
      }
      break;
    }
  }
  
  if(state.isSnakeHere) // If the snake is here
    lib_image_set_pixel(current,state.color, state.i, state.j);  // display its current position
  
  return;  
}



/*
 * T_snake_command()
 * receive the command messages from queue
 *
 */
void T_snake_command(void *pvParameters){
  uint8_t my_id = (unsigned int) pvParameters;

  for(;;){
    if(current_demo == my_id){
      Packet p; 
      xQueueReceive(commandQueue, &p, portMAX_DELAY);   // wait for a new message
      if((p.data[0] == SNAKE_COMMAND_SNAKE) && state.isSnakeHere!=1  )
	snake_receive_snake(&p);
      if(p.data[0] == SNAKE_COMMAND_TURN) {
	int8_t rotation = p.data[1]; 
	xQueueSend(turnQueue, ( void * )&rotation, 0); // put the rotation into the queue
      }
    }
    else
      vTaskDelay(100);
  }
}


/*
 * T_snake_turn()
 * receive the turn messages from queue
 *
 */
void T_snake_turn(void *pvParameters){
  uint8_t my_id = (unsigned int) pvParameters;

  for(;;){
    uint8_t rotation;  
    if(current_demo == my_id){
      xQueueReceive(turnQueue, &(rotation), portMAX_DELAY);   // wait for a rotation of the block
      switch ((rotation+2)%4){
      case 1:
	snake_turn_right();
	break;
      case 3:
	snake_turn_left();
	break;
      default:
	break;
      }
    }
    else
      vTaskDelay(100);
  }
}

/*
 * T_snake
 * Main task
 *
 */   
void T_snake(void *pvParameters){
  unsigned int my_id = (unsigned int) pvParameters;
  uint8_t in=0; // Am I the current task ?
  snake_reset();
  xTaskCreate(T_snake_turn, ( signed portCHAR * ) "turn detection", configMINIMAL_STACK_SIZE, (void*)my_id, 1, NULL );
  xTaskCreate(T_snake_command, ( signed portCHAR * ) "command detection", configMINIMAL_STACK_SIZE, (void*)my_id, 1, NULL );
 
  for(;;){
    if(current_demo == my_id){
      if(in == 0){
	snake_reset();
	in = 1;
      }
      // set current as the image to be displayed
      set_image_pointer(current);
      snake_move_snake();
      vTaskDelay(state.speed*10);
    }
    else{
      in=0;
      vTaskDelay(100);
    }
  }
}

		
