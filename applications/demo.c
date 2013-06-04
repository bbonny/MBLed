/*
 * demo.c
 *
 * Demo applications
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

#include <stm32f10x.h>
#ifdef MBLED
#include "image.h"
#else
#include "image_glip.h"
#endif
#include <FreeRTOS.h>
#include <task.h>
#include "lib_image.h"
#include "number.h"
#include "debug.h"
#include "graphic_barre.h"
#include "various_graphic.h"
#include "image_descriptor.h"
#include "constant.h"
#include "algo.h"
#include "demo.h"
#include "network.h"
#include "position.h"

// image variable
extern unsigned short bufferIMG[64];
// algorithm variable
extern int8_t  minI, minJ, maxI, maxJ;

extern uint8_t current_demo; // Used to check if it's always the turn of the task






void T_display_MBLED_22(){

  unsigned short current[64];
  //unsigned short zero[64]={LETTER_0(0xfff)};
  set_image_pointer(current);

  int8_t* me = get_block_me();

  for(;;){
    network_max_values();
    lib_image_set_image(current, (image_t)MBled_frame);
    if(getMembersInNetwork()==4){

      me = get_block_me();

      if(me[I]==minI && me[J]==minJ)
	lib_image_set_image(current, (image_t)MBled_frame);
      if(me[I]==minI && me[J]==minJ+1 )
	lib_image_set_image(current, (image_t)MBled_frame+64);
      if(me[I]==minI+1 && me[J]==minJ )
	lib_image_set_image(current, (image_t)MBled_frame+64*2);
      if(me[I]==minI+1 && me[J]==minJ+1 )
	lib_image_set_image(current, (image_t)MBled_frame+64*3);



    }
    lib_image_direct_image(current);
    vTaskDelay(200);
  }
}
  


/*
 * Nice colors
 *
 */
void T_alea(void *pvParameters){
  uint8_t my_id = (unsigned int) pvParameters;

  pixel_t val1=0;
  pixel_t val2=0x5ff;
  pixel_t color[64] = {COLOR};   
  set_image_pointer(color);
  for(;;){
    if(current_demo == my_id){
      set_image_pointer(color);
  
      for(int i=0; i<8; i++)
	for(int j=0; j<8; j++){
	  val1 = (val1+0x9f1)%0xfff;
	  val2 = (val2+0x610)%0xfff;
	  lib_image_set_pixel(color, val1, (i*j)%8, (j+i)%8);
	  lib_image_set_pixel(color, val2, i, j);
	  vTaskDelay(10);
	}
    }
    vTaskDelay(100);
      }
}


/*
 * degrade
 *
 */
void T_degrade(){
  unsigned short color[64] = {COLOR};   
  set_image_pointer(color);
  
  for(;;){
   	
  }
}


const  pixel_t B[64]={LETTER_B(0xf00)};
const  pixel_t E2[64]={LETTER_E(0x999)};
const  pixel_t L[64]={LETTER_L(0x999)};
const  pixel_t M[64]={LETTER_M(0x00f)};
const  pixel_t D2[64]={LETTER_D(0xddd)};

    
/*
 * Display "MB LED"
 *
 */
void T_MBLed(){

  
  set_image_pointer((image_t) E2);
  
  for(;;){
    vTaskDelay(500);
    set_image_pointer( (image_t) M);
    
    vTaskDelay(500);
    set_image_pointer( (image_t) B);
    
    vTaskDelay(500);
    set_image_pointer( (image_t) L);
    
    vTaskDelay(500);
    set_image_pointer( (image_t) E2);
    
    vTaskDelay(500);
    set_image_pointer( (image_t) D2);
    
  }
}



const pixel_t L_A[64]={LETTER_A(0xfff)};
const pixel_t L_B[64]={LETTER_B(0xfff)};
const pixel_t L_C[64]={LETTER_C(0xfff)};
const pixel_t L_D[64]={LETTER_D(0xfff)};
const pixel_t L_E[64]={LETTER_E(0xfff)};
const pixel_t L_F[64]={LETTER_F(0xFFF)};
const pixel_t L_G[64]={LETTER_G(0xFFF)};
const pixel_t L_H[64]={LETTER_H(0xfff)};
const pixel_t L_I2[64]={LETTER_I(0xfff)};
const pixel_t L_J2[64]={LETTER_J(0xfff)};
const pixel_t L_K[64]={LETTER_K(0xfff)};
const pixel_t L_L[64]={LETTER_L(0xfff)};
const pixel_t L_M[64]={LETTER_M(0xfff)};
const pixel_t L_N[64]={LETTER_N(0xFFF)};
const pixel_t L_O[64]={LETTER_O(0xfff)};
const pixel_t L_P[64]={LETTER_P(0xfff)};
const pixel_t L_Q[64]={LETTER_Q(0xfff)};
const pixel_t L_R[64]={LETTER_R(0xfff)};
const pixel_t L_S[64]={LETTER_S(0xfff)};
const pixel_t L_T[64]={LETTER_T(0xfff)};
const pixel_t L_U[64]={LETTER_U(0xfff)};
const pixel_t L_V[64]={LETTER_V(0xfff)};
const pixel_t L_W[64]={LETTER_W(0xfff)};
const pixel_t L_X[64]={LETTER_X(0xFFF)};
const pixel_t L_Y[64]={LETTER_Y(0xfff)};
const pixel_t L_Z[64]={LETTER_Z(0xfff)};
const pixel_t L_ZERO[64]={LETTER_0(0xfff)};
const pixel_t L_UN[64]={LETTER_1(0xfff)};
const pixel_t L_DEUX[64]={LETTER_2(0xfff)};
const pixel_t L_TROIS[64]={LETTER_3(0xfff)};
const pixel_t L_QUATRE[64]={LETTER_4(0xfff)};
const pixel_t L_CINQ[64]={LETTER_5(0xfff)};
const pixel_t L_SIX[64]={LETTER_6(0xfff)};
const pixel_t L_SEPT[64]={LETTER_7(0xfff)};
const pixel_t L_HUIT[64]={LETTER_8(0xfff)};
const pixel_t L_NEUF[64]={LETTER_9(0xfff)};
  

/*
 * Display letters
 *
 */
void T_switch_letter(){

  for(;;){
    vTaskDelay(500);
    
    set_image_pointer( (image_t) L_A);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_B);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_C);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_D);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_E);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_F);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_G);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_H);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_I2);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_J2);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_K);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_L);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_M);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_N);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_O);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_P);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_Q);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_R);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_S);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_T);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_U);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_V);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_W);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_X);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_Y);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_Z);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_ZERO);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_UN);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_DEUX);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_TROIS);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_QUATRE);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_CINQ);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_SIX);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_SEPT);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_HUIT);
    vTaskDelay(500);
    set_image_pointer( (image_t) L_NEUF);
    vTaskDelay(500);
  }
}
  

#ifdef MBLED
/* 
 * Use to debug the converting process of an image for the LED driver
 *
 */ 
void T_driver_debug(){
  static unsigned long  LED_value[24];
  static uint16_t message_spiDriver_test[18];  
  image_t image =  &bufferIMG[0];  

  for(;;){


    // Affiche l'image
    writeDebugInit("image\r\n");
    for(int line=0; line<8; line++){
      for(int i=0; i<8; i++){
	writeHexDebug((uint16_t)(bufferIMG[line*8+i]>>8)&0xFF);
	writeHexDebug((uint16_t)bufferIMG[line*8+i]&0xFF);
	writeDebugInit(" ");
 
      }
      writeDebugInit("\r\n");
    }
    writeDebugInit("\r\n");
    writeDebugInit("\r\n");
    writeDebugInit("\r\n");

    writeDebugInit("line pixels\r\n");
    for(int i=0; i<8; i++){
      transform_image_line_for_driver(image, i, LED_value); // Convert the PWM value of each LED on 12bits 
 
      writeDebugInit("line : ");
      writeHexDebug((uint16_t)i);
      writeDebugInit("\r\n");   
      for(int j =0; j<24; j++){
	writeHexDebug((uint16_t)j);
	writeDebugInit(" : ");
	writeHexDebug((uint16_t)(LED_value[j]>>8)&0xFF);
	writeHexDebug((uint16_t)(LED_value[j]&0xFF));
	writeDebugInit("  ");
    
      }
      writeDebugInit("\r\n");
    }
    writeDebugInit("\r\n");
    writeDebugInit("\r\n");
    writeDebugInit("\r\n");
  

#define b(n, p) (((n>>(11-p))&0x0001) ? 1<<(11-p) : 0)
#define b2(n, p)(((n>>(4-(p-11)))&0x0001) ? 1<<p : 0)
    for(int i=17; i>=0; i--)
      message_spiDriver_test[i]=0;


    transform_image_line_for_driver(image, 0, LED_value); // Convert thePWM value of each LED on 12bits 

    message_spiDriver_test[0] |=  LED_value[1]<<12 | LED_value[0] ;
    message_spiDriver_test[1] |=  LED_value[2]<<8 | LED_value[1]>>4;
    message_spiDriver_test[2] |=  LED_value[3]<<4 | LED_value[2]>>8;

    message_spiDriver_test[3] |=  LED_value[5]<<12 | LED_value[4]  ;
    message_spiDriver_test[4] |=  LED_value[6]<<8 | LED_value[5]>>4  ;
    message_spiDriver_test[5] |=  LED_value[7]<<4 | LED_value[6]>>8;

    message_spiDriver_test[6] |=  LED_value[9]<<12 | LED_value[8]  ;
    message_spiDriver_test[7] |=  LED_value[10]<<8 | LED_value[9]>>4;  
    message_spiDriver_test[8] |=  LED_value[11]<<4 | LED_value[10]>>8;

    message_spiDriver_test[9] |=  LED_value[13]<<12 | LED_value[12]  ;
    message_spiDriver_test[10] |=  LED_value[14]<<8 | LED_value[13]>>4 ;
    message_spiDriver_test[11] |=  LED_value[15]<<4 | LED_value[14]>>8;
  
    message_spiDriver_test[12] |=  LED_value[17]<<12 | LED_value[16]  ;
    message_spiDriver_test[13] |=  LED_value[18]<<8 | LED_value[17]>>4  ;
    message_spiDriver_test[14] |=  LED_value[19]<<4 | LED_value[18]>>8;
    
    message_spiDriver_test[15] |=  LED_value[21]<<12 | LED_value[20]  ;
    message_spiDriver_test[16] |=  LED_value[22]<<8 | LED_value[21]>>4  ;
    message_spiDriver_test[17] |=  LED_value[23]<<4 | LED_value[22]>>8;
  
    writeDebugInit("vector : line 0");
    writeDebugInit("\r\n");   
    
    for(int i=17; i>=0; i--){
      writeHex16Debug((uint16_t)(message_spiDriver_test[i]));
    }
    writeDebugInit("\r\n");   
    
  }
} 
#endif
void T_barre(){
  const unsigned short rouge0[64] = {barre0(0x00f)};   
  const unsigned short rouge1[64] = {barre1(0x00f)};  
  const unsigned short rouge2[64] = {barre2(0x00f)};
  const unsigned short rouge3[64] = {barre3(0x00f)};
  const unsigned short rouge4[64] = {barre4(0x00f)};
  const unsigned short rouge5[64] = {barre5(0x00f)};
  const unsigned short rouge6[64] = {barre6(0x00f)};
  const unsigned short rouge7[64] = {barre7(0x00f)};
  const unsigned short vert0[64] = {barre0(0x0f0)};   
  const unsigned short vert1[64] = {barre1(0x0f0)};  
  const unsigned short vert2[64] = {barre2(0x0f0)};
  const unsigned short vert3[64] = {barre3(0x0f0)};
  const unsigned short vert4[64] = {barre4(0x0f0)};
  const unsigned short vert5[64] = {barre5(0x0f0)};
  const unsigned short vert6[64] = {barre6(0x0f0)};
  const unsigned short vert7[64] = {barre7(0x0f0)};
  const unsigned short bleu0[64] = {barre0(0xfff)};   
  const unsigned short bleu1[64] = {barre1(0xf00)};  
  const unsigned short bleu2[64] = {barre2(0xf00)};
  const unsigned short bleu3[64] = {barre3(0xf00)};
  const unsigned short bleu4[64] = {barre4(0xf00)};
  const unsigned short bleu5[64] = {barre5(0xf00)};
  const unsigned short bleu6[64] = {barre6(0xf00)};
  const unsigned short bleu7[64] = {barre7(0xf00)};
 

  for(;;){
    lib_image_set_image(bufferIMG, (image_t) rouge0);
    vTaskDelay(100);
    lib_image_set_image(bufferIMG, (image_t) rouge1);
    vTaskDelay(100);
    lib_image_set_image(bufferIMG, (image_t) rouge2);
    vTaskDelay(100);
    lib_image_set_image(bufferIMG, (image_t) rouge3);
    vTaskDelay(100);
    lib_image_set_image(bufferIMG, (image_t) rouge4);
    vTaskDelay(100);
    lib_image_set_image(bufferIMG, (image_t) rouge5);
    vTaskDelay(100);
    lib_image_set_image(bufferIMG, (image_t) rouge6);
    vTaskDelay(100);
    lib_image_set_image(bufferIMG, (image_t) rouge7);
 
    vTaskDelay(100);
    lib_image_set_image(bufferIMG, (image_t) vert0);
    vTaskDelay(100);
    lib_image_set_image(bufferIMG, (image_t) vert1);
    vTaskDelay(100);
    lib_image_set_image(bufferIMG, (image_t) vert2);
    vTaskDelay(100);
    lib_image_set_image(bufferIMG, (image_t) vert3);
    vTaskDelay(100);
    lib_image_set_image(bufferIMG, (image_t) vert4);
    vTaskDelay(100);
    lib_image_set_image(bufferIMG, (image_t) vert5);
    vTaskDelay(100);
    lib_image_set_image(bufferIMG, (image_t) vert6);
    vTaskDelay(100);
    lib_image_set_image(bufferIMG, (image_t) vert7);
 
    vTaskDelay(100);
    lib_image_set_image(bufferIMG, (image_t) bleu0);
    vTaskDelay(100);
    lib_image_set_image(bufferIMG, (image_t) bleu1);
    vTaskDelay(100);
    lib_image_set_image(bufferIMG, (image_t) bleu2);
    vTaskDelay(100);
    lib_image_set_image(bufferIMG, (image_t) bleu3);
    vTaskDelay(100);
    lib_image_set_image(bufferIMG, (image_t) bleu4);
    vTaskDelay(100);
    lib_image_set_image(bufferIMG, (image_t) bleu5);
    vTaskDelay(100);
    lib_image_set_image(bufferIMG, (image_t) bleu6);
    vTaskDelay(100);
    lib_image_set_image(bufferIMG, (image_t) bleu7);
    vTaskDelay(100);
  }
} 


