 /*
 * all_demos.c
 *
 * Visual effect for the ping emission
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
#include "lib_image.h"
#include "number.h"
#include "graphic_barre.h"
#include "various_graphic.h"
#include "manager.h"


// Graphical elements
static uint8_t frame;
static unsigned short current[64];
static unsigned short current1[64];
static unsigned short current2[64];
const static pixel_t arrow_sprite[64] = {ARROW(0x0ff)};
const static pixel_t arrow_sprite_leader[64] = {ARROW(0xf0f)};
const static pixel_t leader_sprite[64] = {LETTER_L(0x00f)};
const static pixel_t common_sprite[64] = {LETTER_C(0x0f0)};


// Sprites used to display alphanumeric char
const static pixel_t L_ZERO[64]={LETTER_0(0xfff)};
const static pixel_t L_ONE[64]={LETTER_1(0xfff)};
const static pixel_t L_TWO[64]={LETTER_2(0xfff)};
const static pixel_t L_THREE[64]={LETTER_3(0xfff)};
const static pixel_t L_FOUR[64]={LETTER_4(0xfff)};
const static pixel_t L_FIVE[64]={LETTER_5(0xfff)};
const static pixel_t L_SIX[64]={LETTER_6(0xfff)};
const static pixel_t L_SEVEN[64]={LETTER_7(0xfff)};
const static pixel_t L_EIGHT[64]={LETTER_8(0xfff)};

const static pixel_t L_ZERO_R[64]={LETTER_0(0xf00)};
const static pixel_t L_ONE_R[64]={LETTER_1(0xf00)};
const static pixel_t L_TWO_R[64]={LETTER_2(0xf00)};
const static pixel_t L_THREE_R[64]={LETTER_3(0xf00)};
const static pixel_t L_FOUR_R[64]={LETTER_4(0xf00)};
const static pixel_t L_FIVE_R[64]={LETTER_5(0xf00)};
const static pixel_t L_SIX_R[64]={LETTER_6(0xf00)};
const static pixel_t L_SEVEN_R[64]={LETTER_7(0xf00)};
const static pixel_t L_EIGHT_R[64]={LETTER_8(0xf00)};
const static pixel_t L_NINE_R[64]={LETTER_9(0xf00)};
const static pixel_t L_TEN_R[64]={LETTER_10(0xf00)};
const static pixel_t L_ELEVEN_R[64]={LETTER_11(0xf00)};
const static pixel_t L_TWELVE_R[64]={LETTER_12(0xf00)};
const static pixel_t L_THIRTEEN_R[64]={LETTER_13(0xf00)};
const static pixel_t L_FOURTEEN_R[64]={LETTER_14(0xf00)};
const static pixel_t L_FIFTEEN_R[64]={LETTER_15(0xf00)};
const static pixel_t L_SIXTEEN_R[64]={LETTER_16(0xf00)};
const static pixel_t L_SEVENTEEN_R[64]={LETTER_17(0xf00)};
const static pixel_t L_HEIGHTEEN_R[64]={LETTER_18(0xf00)};
const static pixel_t L_NINETEEN_R[64]={LETTER_19(0xf00)};
const static pixel_t L_TWENTY_R[64]={LETTER_20(0xf00)};
const static pixel_t L_TWENTYONE_R[64]={LETTER_21(0xf00)};
const static pixel_t L_TWENTYTWO_R[64]={LETTER_22(0xf00)};
const static pixel_t L_TWENTYTHREE_R[64]={LETTER_23(0xf00)};
const static pixel_t L_TWENTYFOUR_R[64]={LETTER_24(0xf00)};



/*
 *  demo0_ping();
 *
 *  Graphical function used to symbolize the ping
 *
 */
void demo0_ping(){
  set_image_pointer(current);
  lib_image_reset(current);
  
  switch(frame){
  case 0: 
    // Central square
    lib_image_set_pixel(current, 0xfff,3, 3);
    lib_image_set_pixel(current, 0xfff,4, 4); 
    lib_image_set_pixel(current, 0xfff,3, 4);
    lib_image_set_pixel(current, 0xfff,4, 3);
    break;
  case 1: 
    // second square
    lib_image_set_pixel(current, 0xfff,2, 3);
    lib_image_set_pixel(current, 0xfff,2, 4);
    lib_image_set_pixel(current, 0xfff,3, 2);
    lib_image_set_pixel(current, 0xfff,3, 5);
    lib_image_set_pixel(current, 0xfff,4, 2);
    lib_image_set_pixel(current, 0xfff,4, 5);
    lib_image_set_pixel(current, 0xfff,5, 3);
    lib_image_set_pixel(current, 0xfff,5, 4);
    break;
  case 2: 
    // third square
    lib_image_set_pixel(current, 0xfff,1, 3);
    lib_image_set_pixel(current, 0xfff,1, 4);
    lib_image_set_pixel(current, 0xfff,3, 1);
    lib_image_set_pixel(current, 0xfff,3, 6);
    lib_image_set_pixel(current, 0xfff,4, 1);
    lib_image_set_pixel(current, 0xfff,4, 6);
    lib_image_set_pixel(current, 0xfff,6, 3);
    lib_image_set_pixel(current, 0xfff,6, 4);
    break;
  case 3: 
    // bigest square
    lib_image_set_pixel(current, 0xfff,0, 3);
    lib_image_set_pixel(current, 0xfff,0, 4);
    lib_image_set_pixel(current, 0xfff,3, 0);
    lib_image_set_pixel(current, 0xfff,3, 7);
    lib_image_set_pixel(current, 0xfff,4, 0);
    lib_image_set_pixel(current, 0xfff,4, 7);
    lib_image_set_pixel(current, 0xfff,7, 3);
    lib_image_set_pixel(current, 0xfff,7, 4);
    break;
  }      
  frame = (frame + 1) % 4;
}

/*
 * demo1_links()
 * 
 * Show the links with the neighbors
 */
void demo1_links(){
  set_image_pointer(current);
  lib_image_reset(current);
  lib_image_set_pixel(current, 0xfff,3, 3);
  lib_image_set_pixel(current, 0xfff,4, 4);
  lib_image_set_pixel(current, 0xfff,3, 4);
  lib_image_set_pixel(current, 0xfff,4, 3);
  if(interface_is_busy(0)){
    lib_image_set_pixel(current, 0xf00,0, 3);
    lib_image_set_pixel(current, 0xf00,0, 4);
    lib_image_set_pixel(current, 0x0f0,1, 3);
    lib_image_set_pixel(current, 0x0f0,1, 4);
    lib_image_set_pixel(current, 0x0f0,2, 3);
    lib_image_set_pixel(current, 0x0f0,2, 4);
  }
  
  if(interface_is_busy(2)){
    lib_image_set_pixel(current, 0xf00,7, 3);
    lib_image_set_pixel(current, 0xf00,7, 4);          
    lib_image_set_pixel(current, 0x0f0,6, 3);
    lib_image_set_pixel(current, 0x0f0,6, 4);          
    lib_image_set_pixel(current, 0x0f0,5, 3);
    lib_image_set_pixel(current, 0x0f0,5, 4);           
  }
  
  if(interface_is_busy(1)){
    lib_image_set_pixel(current, 0xf00,3, 7);
    lib_image_set_pixel(current, 0xf00,4, 7);
    lib_image_set_pixel(current, 0x0f0,3, 6);
    lib_image_set_pixel(current, 0x0f0,4, 6);      
    lib_image_set_pixel(current, 0x0f0,3, 5);
    lib_image_set_pixel(current, 0x0f0,4, 5);      
  }
  
  if(interface_is_busy(3)){
    lib_image_set_pixel(current, 0xf00,3, 0);
    lib_image_set_pixel(current, 0xf00,4, 0); 
    lib_image_set_pixel(current, 0x0f0,3, 1);
    lib_image_set_pixel(current, 0x0f0,4, 1); 
    lib_image_set_pixel(current, 0x0f0,3, 2);
    lib_image_set_pixel(current, 0x0f0,4, 2);      
  }
}


/*
 * demo2_leader()
 *
 * Display a 'L' if the block is the leader and a 'C' if it is a common block
 * 
 */
void demo2_leader(){
  set_image_pointer(current);
  lib_image_reset(current);
  if(isLeader()) // Leader detection
    lib_image_set_image(current,(image_t) leader_sprite);
  else
    lib_image_set_image(current, (image_t) common_sprite);
  
}


/*
 * demo3_north()
 *
 * Display an arrow pointing the known north 
 *
 */
void demo3_north(){
  lib_image_reset(current1);        
  if(isLeader())
    lib_image_set_image(current1, (image_t) arrow_sprite_leader);  
  else
    lib_image_set_image(current1, (image_t) arrow_sprite);
  lib_image_direct_image(current1);
  set_image_pointer(current1);
  vTaskDelay(250);
  lib_image_reset(current2);        
  if(isLeader())
    lib_image_set_image(current2, (image_t) arrow_sprite_leader);  
  else
    lib_image_set_image(current2, (image_t) arrow_sprite);
  lib_image_set_image(current2, (image_t) arrow_sprite);  
  lib_image_direct_image(current2);
  set_image_pointer(current2);
}

/*
 * demo4_topo()
 * 
 * Display the known topology of the network, the block is:
 *  - in red if he is a common block
 *  - in green if he is the leader or if it is the leader
 *  - in white if it is an other common block
 */
void demo4_topo(){
  int8_t* block;
  
  lib_image_reset(current1);
  network_max_values();
  
  for(int i=0; i<NB_BLOCKS; i++){
    block = get_block(i);
    
    if(block[IN]){
	  if(i==getLeaderID()){
	    lib_image_set_pixel(current1, 0x0f0, (block[I] - minI), (block[J] - minJ));
	  }	 
	  else{
	    if(i==getMyID())
	      lib_image_set_pixel(current1, 0xf00, (block[I] - minI), (block[J] - minJ));
	    else
	      lib_image_set_pixel(current1, 0xfff, (block[I] - minI), (block[J] - minJ));
	  }
    }
  }
  lib_image_direct_image(current1);
  set_image_pointer(current1);
  
  lib_image_reset(current2);
  network_max_values();
  
  for(int i=0; i<NB_BLOCKS; i++){
    block = get_block(i);
    
    if(block[IN]){
      if(i==getLeaderID()){
	lib_image_set_pixel(current2, 0x0f0, (block[I] - minI), (block[J] - minJ));
	  }	 
      else{
	if(i==getMyID())
	  lib_image_set_pixel(current2, 0xf00, (block[I] - minI), (block[J] - minJ));
	else
	  lib_image_set_pixel(current2, 0xfff, (block[I] - minI), (block[J] - minJ));
      }
    }
  }
  lib_image_direct_image(current2);
  set_image_pointer(current2);
  
}




/*
 * uint8_t calcul_position()
 *
 * Calcul my position in the network depending
 * of the biggest rectangular shape determined by
 * minI, minJ, maxI, maxJ
 *
 */
static uint8_t calcul_position(){
  int8_t* me;
  uint8_t position;
  network_max_values();
  me = get_block_me();
  int8_t size_i = maxI - minI + 1;
  int8_t size_j = maxJ - minJ + 1;
  position = -1;
  for(int8_t i=0; i < size_i; i++) {  
    for(int8_t j=0; j < size_j ; j++){	
      if(me[I] == minI + i && me[J] == minJ + j){
	position =  size_j * i + j;
      }
    }
  }
  if(position > 15) 
    position = -1;
  return position;
}   


/*
 * demo5_refresh_position(unsigned short *current)
 *
 * Display the position calculated by calcul_position()
 *
 */
void demo5_refresh_position(unsigned short *current){
  set_image_pointer(current);
  lib_image_reset(current);        
  uint8_t position = calcul_position();

  if(position != -1){
    switch(position){
    case 0: 
      lib_image_set_image(current, (image_t) L_ONE_R);
      break;
    case 1: 
      lib_image_set_image(current, (image_t) L_TWO_R);
      break;
    case 2: 
      lib_image_set_image(current, (image_t) L_THREE_R);
      break;
    case 3: 
      lib_image_set_image(current, (image_t) L_FOUR_R);
      break;
    case 4: 
      lib_image_set_image(current, (image_t) L_FIVE_R);
      break;
    case 5: 
      lib_image_set_image(current, (image_t) L_SIX_R);
      break;
    case 6: 
      lib_image_set_image(current, (image_t) L_SEVEN_R);
      break;
    case 7: 
      lib_image_set_image(current, (image_t) L_EIGHT_R);
      break; 
    case 8: 
      lib_image_set_image(current, (image_t) L_NINE_R);
      break;
   case 9: 
      lib_image_set_image(current, (image_t) L_TEN_R);
      break;
    case 10: 
      lib_image_set_image(current, (image_t) L_ELEVEN_R);
      break;
    case 11: 
      lib_image_set_image(current, (image_t) L_TWELVE_R);
      break;
    case 12: 
      lib_image_set_image(current, (image_t) L_THIRTEEN_R);
      break;
    case 13: 
      lib_image_set_image(current, (image_t) L_FOURTEEN_R);
      break; 
    case 14: 
      lib_image_set_image(current, (image_t) L_FIFTEEN_R);
      break;
   case 15: 
      lib_image_set_image(current, (image_t) L_SIXTEEN_R);
      break;
   case 16: 
      lib_image_set_image(current, (image_t) L_SEVENTEEN_R);
      break;
   case 17: 
      lib_image_set_image(current, (image_t) L_HEIGHTEEN_R);
      break;    
   case 18: 
      lib_image_set_image(current, (image_t) L_NINETEEN_R);
      break;
    case 19: 
      lib_image_set_image(current, (image_t) L_TWENTY_R);
      break;
   case 20: 
      lib_image_set_image(current, (image_t) L_TWENTYONE_R);
      break;
   case 21: 
      lib_image_set_image(current, (image_t) L_TWENTYTWO_R);
      break;    
   case 22: 
     lib_image_set_image(current, (image_t) L_TWENTYTHREE_R);
     break;
    case 23: 
      lib_image_set_image(current, (image_t) L_TWENTYFOUR_R);
      break;      
    } 
  }
  lib_image_direct_image(current);
}



/*
 * demo_task()
 *
 * Launch the good displaying function depending of the current_demo variable
 *
 */
void demo_task(){
  frame = 0;
  for(;;){
    switch(current_demo){
    case MANAGER_DEMO0:
      demo0_ping();
      break;
    case MANAGER_DEMO1:
      demo1_links();
      break;
    case MANAGER_DEMO2:
      demo2_leader();
      break;
    case MANAGER_DEMO3:
      demo3_north();
      break;
    case MANAGER_DEMO4:
      demo4_topo();
      break;
    case MANAGER_DEMO5:
      demo5_refresh_position(current1);
      vTaskDelay(150);
      demo5_refresh_position(current2);
      break;
    default:
      break;
    }
    vTaskDelay(150);
  }
}
