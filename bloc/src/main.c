/*
 * main.c
 *
 * Creates the tasks and launch the scheduler
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

// FreeRTOS and STM32 features
#include <stm32f10x.h>
#include <stm32f10x_flash.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>

// MBLed features
#include "hardware_init.h"
#include "debug.h"
#include "irda.h"
#include "packet.h"
#include "send.h"
#include "receive.h"
#include "scheduler.h"
#include "constant.h"
#include "ping.h"
#include "algo.h"
#include "network.h"
#include "election.h"
#include "synchro.h"
#include "turn.h"
#include "manager.h"
#include "buzzer.h"
#include "command.h"
#include "application.h"
#include "watchdog.h"


#include "anim.h"

#ifndef MBLED
#include "image_glip.h"
#else
#include "image.h"
#include "music.h"
#endif

#include "snake.h"

int main () { 
/* General initialization */
  hardware_init(); // STM32 initialization
  time_init();     // Time init for the inter-block synchronization
#ifndef GLIP 
  initDebug ();    // Debug initialization for the MB Led block
#endif  
  irda_init ();    // IrDAs initialization
  image_init();    // LED driver init
  algo_init(0);     // Inititalization of the shared algorithm
  command_init();  // Initialization for the inter-application communication 
#ifdef MBLED
  buzzer_init();   // Only the MB Led blocks have a buzzer
#endif


  /* Application initialization */ 
  snake_init(); 
  

  xTaskCreate(switching_task, ( signed portCHAR * ) "switching task"
  	      , configMINIMAL_STACK_SIZE, NULL, 1, NULL );
  
  
  
  // Create tasks
  for(unsigned int i=IRDA_MIN;i<=IRDA_MAX;i++)
    {
   
      xTaskCreate(irda_read,(signed portCHAR *)"irda receive",
      		  configMINIMAL_STACK_SIZE, (void*)i,
      		  configMAX_PRIORITIES - 1, NULL);

      xTaskCreate(irda_ack,(signed portCHAR *)"irda ack",
      		  configMINIMAL_STACK_SIZE, (void*)i,
      		  tskIDLE_PRIORITY+2, NULL);
      
        xTaskCreate(packet_send_IrDA,(signed portCHAR *)"irda send",
      		  configMINIMAL_STACK_SIZE, (void*)i,
      		   configMAX_PRIORITIES - 1, NULL);
      
    xTaskCreate(ping_acq_task,(signed portCHAR *)"ping ack",
  		  2*configMINIMAL_STACK_SIZE, (void*)i,
  		  tskIDLE_PRIORITY + 3, NULL);  
      xTaskCreate(ping_task, (signed portCHAR *) "PING", 
  		    configMINIMAL_STACK_SIZE,(void*)i, 
  		    tskIDLE_PRIORITY + 5, NULL); 
    }
  
  
   
  
  algo_task_create();
  
  
  xTaskCreate(irda_receive,(signed portCHAR *)"analyse",
  	      3*configMINIMAL_STACK_SIZE, NULL,
  	      configMAX_PRIORITIES - 1,NULL);


  
  
  xTaskCreate(music_task, (signed portCHAR *) "Music"
	      , configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
  
  // Start scheduler and tasks
  vTaskStartScheduler();
  for(;;);

  return 0;
}
  
 
void vApplicationStackOverflowHook( void )
{
	/* This will get called if an overflow is detected in the stack of a task.
	Inspect pxCurrentTCB to see which was the offending task. */
	for( ;; );
}

