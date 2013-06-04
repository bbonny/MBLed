/*
 * anim.c
 *
 * Animation application - display animations depending of the current_demo variable
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
#include <stm32f10x.h>


#include "animationDescriptor.h"
#include "catalogue.h"


#include "application.h"
#include "manager.h"


 
static pixel_t current_frame1 [64]; // contains the image to display
static pixel_t current_frame2 [64];

const AnimationDescriptor * currentAnimation; // Pointer on the current animation
extern uint8_t current_demo; // Used to check if it's always the turn of the task

//Calculate which anim we have to display.
uint8_t which_anim(){
  network_max_values();
  
  int8_t size_i = maxI - minI + 1;
  int8_t size_j = maxJ - minJ + 1;

  if(size_i >= 4 || size_j >= 4)
    return 2;
  else if(size_i == 3 || size_j == 3)
    return 1;
  return 0;
}


/*
 * anim_task()
 *
 * Display the animation depending of the current animation and of the current_time
 *
 */
void anim_task(){

  pixel_t *current_frame = current_frame1;
  uint8_t switcher = 2;
  uint8_t anim = 0;
  static int8_t pos = 0;
  static int8_t* me; 


  for(;;){
    if(current_demo == MANAGER_DEMO6 || current_demo == MANAGER_DEMO7 || current_demo == MANAGER_DEMO8 || current_demo == MANAGER_DEMO9 || current_demo == MANAGER_DEMO11){
	switch(current_demo){
	  //PIKA     
	case MANAGER_DEMO6:
	  currentAnimation = animationList[3];
	  break;
	  //MICKA BIRTHDAY
	case MANAGER_DEMO7:
	  currentAnimation = animationList[4];
	  break;
	  //MEGAMAN
	case MANAGER_DEMO8: 
	  anim = which_anim();
	  currentAnimation = animationList[anim];
	  break;
	  //TETRIS
	case MANAGER_DEMO9:
	  currentAnimation = animationList[5];
	  break;	  

	  //Psychedelic 4x4
	case MANAGER_DEMO11: 
	  currentAnimation = animationList[6];
	 break;  
	}

      network_max_values();
      me = get_block_me();         

     // Find my position into the rectangular shape
      for(int i=0; i < currentAnimation->nb_colonnes; i++){
	for(int j=0; j < currentAnimation->nb_lignes  ; j++){	
	  if(me[I] == minI + i && me[J] == minJ + j){
	    pos =  currentAnimation->nb_lignes* i + j;
	  }
	}
      }

    
      memcpy (current_frame,
	      &currentAnimation->data[(pos)*64*(currentAnimation->nb_images)+(((current_time())/(currentAnimation->delai))%(currentAnimation->nb_images))*64],      64*sizeof(short) );
   
      // rotate the part according to the north direction
      lib_image_direct_image(current_frame);
      
      if(switcher==1){
	switcher = 2 ;
	current_frame = current_frame2;
      }
      else{
	switcher = 1 ;
	current_frame = current_frame1;
      }

      // Select the good part of the animation according to: time and position
      set_image_pointer(current_frame);
    }
    vTaskDelay(150);
  }
}
