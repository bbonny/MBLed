/*
 * application.h
 *
 * General include for the applications
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

// STM32 features
#include <stm32f10x.h>

// FreeRTOS features
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>

// Graphic features
#ifdef MBLED
#include "image.h"
#else
#include "image_glip.h"
#endif
#include "lib_image.h"

#include "number.h"
#include "graphic_barre.h"
#include "various_graphic.h"

// network features
#include "constant.h"
#include "algo.h"
#include "network.h"
#include "position.h"
#include "send.h"
#include "synchro.h"
#include "turn.h"

// Only for the UART debug
#include "debug.h"

extern xQueueHandle turnQueue; // queue used to notice the current game/animation/task
extern xQueueHandle commandQueue; // queue used to notice the current game/animation/task
extern uint8_t current_demo; // Used to check if it's always the turn of the task
// algorithm variable
extern int8_t  minI, minJ, maxI, maxJ;


// Includes for the applications
#include "snake.h"
#include "all_demos.h"
#include "demo.h"



