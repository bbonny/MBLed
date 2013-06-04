/*
 * snake_sound.c
 *
 *  This file adds sound to the snake game. 
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

#include "FreeRTOS.h"
#include "buzzer.h"
#include "stm32f10x.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "debug.h"


static xSemaphoreHandle snake_sound_Semaphore;
static uint8_t sound_id; 

void snake_sound_task(){
  for(;;){
    xSemaphoreTake(snake_sound_Semaphore, portMAX_DELAY);
    switch(sound_id){
    case 0:
      buzzer_put_freq(SI5);
      vTaskDelay(80);
      buzzer_put_freq(LA5);
      vTaskDelay(80);
      buzzer_put_freq(SOL5);
      vTaskDelay(80);
      buzzer_put_freq(FA5);
      vTaskDelay(80);
      buzzer_put_freq(MI5);
      vTaskDelay(80);
      buzzer_put_freq(RE5);
      vTaskDelay(80);
      buzzer_put_freq(DO5);
      vTaskDelay(80);
      buzzer_put_freq(0);
      break;
    case 1:
      buzzer_put_freq(SOL5);
      vTaskDelay(80);
      buzzer_put_freq(FA5);
      vTaskDelay(80);
      buzzer_put_freq(SOL5);
      vTaskDelay(80);
      buzzer_put_freq(FA5);
      vTaskDelay(80);
      buzzer_put_freq(0);
      break;

    }
   
  }
}


void snake_init_sound(){
  vSemaphoreCreateBinary(snake_sound_Semaphore);
  xSemaphoreTake(snake_sound_Semaphore, portMAX_DELAY);
  xTaskCreate( snake_sound_task, ( signed portCHAR * ) "snake sound", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
}


void snake_play_game_over(){
  sound_id = 0;
  xSemaphoreGive(snake_sound_Semaphore);    
} 

void snake_play_apple(){
  sound_id = 1;
  xSemaphoreGive(snake_sound_Semaphore);    
} 

