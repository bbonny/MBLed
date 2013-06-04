/*
 * manager
 *
 *
 * This file contains the main tasks which will
 * modify the current state and decide what to do 
 * when the block receive a command message. 
 * like:
 *  - change the current use (animation, ,image, game, waiting)
 *
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

#define NB_DEMOS 13
#include "application.h"

#include "manager.h"
#include "demo.h"
#include "snake.h"
#include "anim.h"
#include "debug.h"
#include "watchdog.h"
#include "music.h"

pixel_t current[64];

const pixel_t wait_sprite[64] = {LETTER_W(0xFFF)};
const pixel_t snake_sprite[64] = {LETTER_S(0xFFF)};
const pixel_t debug_sprite[64] = {LETTER_D(0xFFF)};
const pixel_t demo0_sprite[64] = {LETTER_0(0xFFF)};
const pixel_t demo1_sprite[64] = {LETTER_1(0xFFF)};
const pixel_t demo2_sprite[64] = {LETTER_2(0xFFF)};
const pixel_t demo3_sprite[64] = {LETTER_3(0xFFF)};
const pixel_t demo4_sprite[64] = {LETTER_4(0xFFF)};
const pixel_t demo5_sprite[64] = {LETTER_5(0xFFF)};
const pixel_t demo6_sprite[64] = {LETTER_6(0xFFF)};
const pixel_t demo7_sprite[64] = {LETTER_7(0xFFF)};
const pixel_t demo8_sprite[64] = {LETTER_8(0xFFF)};
const pixel_t demo9_sprite[64] = {LETTER_9(0xFFF)};
const pixel_t demo10_sprite[64] = {LETTER_10(0xFFF)};
const pixel_t demo11_sprite[64] = {LETTER_11(0xFFF)};
const pixel_t demo12_sprite[64] = {LETTER_12(0xFFF)};



extern xSemaphoreHandle turn180Semaphore;
static uint8_t counter, old_counter;
static uint8_t isTaskLauncher;
static uint8_t rotation; 


uint8_t current_demo;
uint8_t new_demo;


void create_demo_tasks();

void watchdog_task(){
  watchdog_init();

  for(;;){
    watchdog_reload();
    vTaskDelay(300);
  }
}

/*
 * manager_turn_task()
 * receive the messages from queue
 *
 */
void manager_turn_task(void *pvParameters){
  uint8_t my_id = (unsigned int) pvParameters;

  for(;;){
    if(current_demo == my_id){   
      xQueueReceive(turnQueue, &(rotation), portMAX_DELAY);   // wait for a rotation of the block
      switch (rotation){
      case 1:
	counter = (counter + 1)% NB_DEMOS;
	break;
      case 3:
	if(counter == 0)
	  counter = NB_DEMOS - 1;
      else
	counter = counter - 1;
	break;
      }    
    }
    else
      vTaskDelay(100);
  }
}

/*
 * switching_task()
 *
 * - At the beginning ask for the current application
 * - Create all the application tasks
 * - Switch between remote and application after a 180 rotation
 *
 *
 */
void switching_task(){
  current_demo = MANAGER_DEMO12; // T_ALEA
  isTaskLauncher = 1;

  // After reset, the first thing to do is to ask to his neigbor what is the current task
  uint8_t data[1];
  data[0] = MANAGER_COMMAND_ASK_TASK;
  for(int j=0; j<2; j++){
    vTaskDelay(100);
    send_new_command(MANAGER, &data[0], EVERYBODY, 1, 1);
  }
  
  create_demo_tasks();
  
  for(;;){
    // wait for a 180 rotation
    xSemaphoreTake(turn180Semaphore, portMAX_DELAY);
    if(isTaskLauncher==1){
      isTaskLauncher = 0;
      current_demo = MANAGER_TASKS;  
    }
    else{
     isTaskLauncher = 1;  
     // kill the turn task and the manager
     current_demo = new_demo;
     //For snake.
     if(current_demo == MANAGER_DEMO10){
       vTaskDelay(500);
       set_snake_here();
     } 
    }
  }
}


/*
 * manager_send_command(uint8_t command, uint8_t time)
 *
 * Send a command to other blocks indicating which application to play
 *
 */
void manager_send_command(uint8_t command, uint8_t time){
  uint8_t data[2];
  new_demo = command;
  data[0] = MANAGER_COMMAND_GIVE_TASK;
  data[1] = command;
  for(int j=0; j<time; j++){
    send_new_command(MANAGER, &data[0], EVERYBODY, 2, TTL);
    vTaskDelay(100);
  }
}



/*
 * manager_task(void *pvParameters)
 * 
 *
 * Display the current_demo number depending of the counter and send a message to other blocks
 * telling them the current_demo to play  
 *
 */
void manager_task(void *pvParameters){
  uint8_t my_id = (unsigned int) pvParameters;
  new_demo = MANAGER_DEMO1;
  counter = 0;
  old_counter = 0; 
  
  for(;;){
    if(current_demo == my_id){   
  
      BROADCAST_POSITION;
	
      lib_image_set_image(current, (image_t) wait_sprite); 
      set_image_pointer(current);

      switch(counter){
      case 0: 
	lib_image_set_image(current, (image_t) demo0_sprite); 
	manager_send_command(MANAGER_DEMO0, 5);
	break;
      case 1: 
	lib_image_set_image(current, (image_t) demo1_sprite);
	manager_send_command(MANAGER_DEMO1, 5);
	break;
      case 2: 
	lib_image_set_image(current, (image_t) demo2_sprite);
	manager_send_command(MANAGER_DEMO2, 5);
	break;
      case 3: 
	lib_image_set_image(current, (image_t) demo3_sprite);
	manager_send_command(MANAGER_DEMO3, 5);
	break;
      case 4: 
	lib_image_set_image(current, (image_t) demo4_sprite);
	manager_send_command(MANAGER_DEMO4, 5);
	break;
      case 5: 
	lib_image_set_image(current, (image_t) demo5_sprite);
	manager_send_command(MANAGER_DEMO5, 5);
	break;
      case 6: 
	lib_image_set_image(current, (image_t) demo6_sprite);
	manager_send_command(MANAGER_DEMO6, 5);
	break;
      case 7: 
	lib_image_set_image(current, (image_t) demo7_sprite);
	manager_send_command(MANAGER_DEMO7, 5);
	break;
      case 8: 
	lib_image_set_image(current, (image_t) demo8_sprite);
	manager_send_command(MANAGER_DEMO8, 5);
	break;
      case 9: 
	lib_image_set_image(current, (image_t) demo9_sprite);
	manager_send_command(MANAGER_DEMO9, 5);
	break;
      case 10: 
	lib_image_set_image(current, (image_t) demo10_sprite);
	manager_send_command(MANAGER_DEMO10, 5);
	break;
      case 11: 
	lib_image_set_image(current, (image_t) demo11_sprite);
	manager_send_command(MANAGER_DEMO11, 5);
	break;
     case 12: 
	lib_image_set_image(current, (image_t) demo12_sprite);
	manager_send_command(MANAGER_DEMO12, 5);
	break;

      default:
	lib_image_set_image(current, (image_t) demo0_sprite);
	break;
      }      
    }
    vTaskDelay(100);    
  }
}

/*
 * create_demo_tasks()
 *
 * creates the demo tasks and the manager task
 *
 */
void create_demo_tasks(){

  xTaskCreate(  watchdog_task,    ( signed portCHAR * ) "switch", configMINIMAL_STACK_SIZE, NULL, 1,  NULL);  
  // 6 firsts demo tasks
  xTaskCreate(  demo_task,      ( signed portCHAR * ) "switch demos", configMINIMAL_STACK_SIZE, NULL, 1,  NULL);
  xTaskCreate(  anim_task,   ( signed portCHAR * ) "anim", configMINIMAL_STACK_SIZE, NULL, 1,  NULL);
  xTaskCreate(  music_task,     ( signed portCHAR * ) "switch", configMINIMAL_STACK_SIZE, (void*)MANAGER_DEMO9 , 5,  NULL);
  xTaskCreate(  T_snake,     ( signed portCHAR * ) "switch", configMINIMAL_STACK_SIZE, (void*)MANAGER_DEMO10, 1,  NULL);
  xTaskCreate(  T_alea,      ( signed portCHAR * ) "switch", configMINIMAL_STACK_SIZE, (void*)MANAGER_DEMO12, 1,  NULL);

  // manager tasks which look after the rotation
  xTaskCreate( manager_turn_task, ( signed portCHAR * ) "turn", configMINIMAL_STACK_SIZE, (void*)MANAGER_TASKS, 1,  NULL);
  // create the manager task
  xTaskCreate( manager_task, ( signed portCHAR * ) "switch", configMINIMAL_STACK_SIZE, (void*)MANAGER_TASKS, 1,  NULL);
  
}


/*
 *   manager_receive_command(Packet *packet)
 *   when receive a manager command then change the current demo
 */
void manager_receive_command(Packet *packet){
  if((uint8_t)packet->data[0] == MANAGER_COMMAND_GIVE_TASK){
    if((uint8_t)packet->data[1] != current_demo){
      current_demo = (uint8_t)packet->data[1];
      send_spread(packet, packet->interface);
    }
  }
  if((uint8_t)packet->data[0] == MANAGER_COMMAND_ASK_TASK){    
    uint8_t data2[2];
    data2[0] = MANAGER_COMMAND_GIVE_TASK;
    data2[1] = current_demo;
    send_new_command(MANAGER, &data2[0], packet->interface, 2, 1);
  }
}
