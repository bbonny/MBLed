/*
 * snake.c
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


#ifndef _SNAKE_H
#define _SNAKE_H

#include <FreeRTOS.h>
#include <task.h>
#include <stm32f10x.h>

#include "packet.h"

#define SNAKE_DIR_NONE 0
#define SNAKE_DIR_NS   1
#define SNAKE_DIR_SN   2
#define SNAKE_DIR_EW   3
#define SNAKE_DIR_WE   4

#define SNAKE_TURN_NONE  0 
#define SNAKE_TURN_LEFT  1
#define SNAKE_TURN_RIGHT 2
#define SNAKE_TURN_180   3

#define SNAKE_NORTH 0 
#define SNAKE_EAST  3
#define SNAKE_SOUTH 2
#define SNAKE_WEST  1
 
#define SNAKE_COMMAND_SNAKE 0
#define SNAKE_COMMAND_TURN  1


// define the vitesse in 1/10 s
#define SNAKE_BASIC_SPEED 50

typedef struct snake_state {
  // 1 if the snake is on the block
  uint8_t isSnakeHere;
  
  // position of the snake if it is here
  uint8_t i;
  uint8_t j;

  // Direction of the snake
  uint8_t direction;
  
  // Speed of the snake
  uint16_t speed;

  // has the block turned between two steps?
  uint8_t hasTurned;

  // old i position 
  uint8_t old_i_position[64];

  // old j position  
  uint8_t old_j_position[64];

  // has the block turned between two steps?
  uint16_t color;

  // size of the snake
  uint16_t size ;
  
  // isGameOver ?
  uint8_t isGameOver;

  int8_t current_apple_i;
  int8_t current_apple_j;

} snake_state;



void T_snake();

/*
 * Receive the snake from an other block
 */
void snake_receive_snake(Packet * packet);
void snake_init();
void set_snake_here();

#endif
