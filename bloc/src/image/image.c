/*
 * image.c
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

#ifdef MBLED


// STM32 features
#include <stm32f10x.h>

// MBLed features
#include "image.h"
#include "lib_image.h"
#include "spi_led.h"
#include "debug.h"
#include "led_driver_config.h"


// current image modified by the display task
image_t bufferIMG;
// current line
static unsigned short line=0;




// Theses macros allows the LED Drivers to control(or not) its output
#define OUTPUT_ENABLE  do{GPIOC->BSRR= 0x00000020;} while(0) //LED_XBLNK HIGH
#define OUTPUT_DISABLE do{GPIOC->BRR = 0x00000020;} while(0) //LED_XBLNK LOW

// Theses macros allows the LED Drivers to control(or not) its output
#define LATCH_ENABLE  do{GPIOB->BSRR= 0x00001000;} while(0) //LED_GSLAT HIGH
#define LATCH_DISABLE do{GPIOB->BRR = 0x00001000;} while(0) //LED_GSLAT LOW

// States of the GPIOC->BSRR register putting only one ROW_x LOW and the others HIGH
static const uint32_t bsrrs[8] = {0x020001cf, 0x010002cf, 0x0080034f, 0x0040038f,
				  0x000803c7, 0x000403cb, 0x000203cd, 0x000103ce};

#define NOLINE GPIOC->BSRR= 0x000003cf
#define LINE(n)GPIOC->BSRR = bsrrs[n]

// invert the bits of a 12 bit value
#define INVERT_12(n) 0 | \
    (n & 0x001)<<11 |				    \
    ((n & 0x002)>>1)<<10 |			    \
    ((n & 0x004)>>2)<<9 |				    \
    ((n & 0x008)>>3)<<8 |				    \
    ((n & 0x010)>>4)<<7 |				    \
    ((n & 0x020)>>5)<<6 |					    \
    ((n & 0x040)>>6)<<5 |					    \
    ((n & 0x080)>>7)<<4 |					    \
    ((n & 0x100)>>8)<<3 |					    \
    ((n & 0x200)>>9)<<2 |					    \
    ((n & 0x400)>>10)<<1 |					    \
    ((n & 0x800)>>11)<<0 

// invert the bits of a 8 bit value
#define INVERT_8(n) 0 | \
    (n & 0x01)<<7 |				    \
    ((n & 0x02)>>1)<<6 |			    \
    ((n & 0x04)>>2)<<5 |				    \
    ((n & 0x08)>>3)<<4 |				    \
    ((n & 0x10)>>4)<<3 |				    \
    ((n & 0x20)>>5)<<2 |					    \
    ((n & 0x40)>>6)<<1 |					    \
    ((n & 0x80)>>7)<<0  
  



void set_image_pointer(image_t source){
  bufferIMG = source;
}


/*
 * This function convert the 4 bit value of a RED or BLUE or GREEN in a 
 * 12 bit value
 *
 */
unsigned short value_convert(unsigned short val){

  switch(val){


  case 0x0: return INVERT_12(0x000);
  case 0x1: return INVERT_12(0x001);
  case 0x2: return INVERT_12(0x002);
  case 0x3: return INVERT_12(0x004);
  case 0x4: return INVERT_12(0x008);
  case 0x5: return INVERT_12(0x00f);
  case 0x6: return INVERT_12(0x010);
  case 0x7: return INVERT_12(0x020);
  case 0x8: return INVERT_12(0x022);
  case 0x9: return INVERT_12(0x024);
  case 0xa: return INVERT_12(0x028);
  case 0xb: return INVERT_12(0x02f);
  case 0xc: return INVERT_12(0x040);
  case 0xd: return INVERT_12(0x04f);
  case 0xe: return INVERT_12(0x080);
  case 0xf: return INVERT_12(0xfff);
  }
  return INVERT_12(0xfff);
}

/* 
 * !If the image format change this function must be modified!
 * 
 * This function transform the given line of the given image
 * into the driver format 12bits/LED 
 *
 */
void transform_image_line_for_driver(image_t image, unsigned int line, unsigned long *LED_value)
{
   unsigned short *l = &image[line*8]; // Table of the 8 pixels of the line
   
   // Theses macros are used to take pixel's values
#define B(n) ((l[n] & 0x0F00)>>8)
#define G(n) ((l[n] & 0x00F0)>>4)
#define R(n) ((l[n] & 0x000F)>>0)
   for(int i=0; i<8; i++){
     LED_value[i*(3)+0] = value_convert(R(i));
     LED_value[i*(3)+1] = value_convert(G(i) );
     LED_value[i*(3)+2] = value_convert(B(i));
  }
 
#undef R
#undef G
#undef B
}


/*
 * Send the line from image to be:
 *  - transformed(into 24 12bit LED value)
 *  - built(into a 288bit vector)
 *  - and sended to the driver with SPI
 * For each LED, 12 bit are used, we have 24 LED per line (8 RGB pixels)
 * So one pixel is made of 12x3 bits = 36 bits
 * And the line id represented by a 36*8pixels = 288 bits vector as:
 * B7(11)B7(10)...B7(0)G7(11)...G7(0)R7(11)...R7(0)... B0(11)...R0(0)
 */
void transform_build_and_send_line(unsigned int line){

  static unsigned long  LED_value[24];
  static uint16_t message_spiDriver[18];    
  image_t image =  &bufferIMG[0];
  
  // Convert the PWM value of each LED on 12bits 
  transform_image_line_for_driver(image, (line+4)%8, LED_value); 
 
  // Fill the 18 messages of 16bits with the good sequence of bit 
  for(int i=17; i>=0; i--)
    message_spiDriver[i]=0;
  message_spiDriver[0] |=  LED_value[1]<<12 | LED_value[0] ;
  message_spiDriver[1] |=  LED_value[2]<<8  | LED_value[1]>>4;
  message_spiDriver[2] |=  LED_value[3]<<4     | LED_value[2]>>8;

  message_spiDriver[3] |=  LED_value[5]<<12 | LED_value[4]   ;
  message_spiDriver[4] |=  LED_value[6]<<8 | LED_value[5]>>4  ;
  message_spiDriver[5] |=  LED_value[7]<<4    | LED_value[6]>>8;

  message_spiDriver[6] |=  LED_value[9]<<12 | LED_value[8]   ;
  message_spiDriver[7] |=  LED_value[10]<<8 | LED_value[9]>>4;  
  message_spiDriver[8] |=  LED_value[11]<<4 | LED_value[10]>>8;

  
  message_spiDriver[9]  |=  LED_value[13]<<12 | LED_value[12]   ;
  message_spiDriver[10] |=  LED_value[14]<<8 | LED_value[13]>>4 ;
  message_spiDriver[11] |=  LED_value[15]<<4 | LED_value[14]>>8;
   
  message_spiDriver[12] |=  LED_value[17]<<12 | LED_value[16]   ;
  message_spiDriver[13] |=  LED_value[18]<<8  | LED_value[17]>>4  ;
  message_spiDriver[14] |=  LED_value[19]<<4  | LED_value[18]>>8;
    
  message_spiDriver[15] |=  LED_value[21]<<12 | LED_value[20]  ;
  message_spiDriver[16] |=  LED_value[22]<<8  | LED_value[21]>>4  ;
  message_spiDriver[17] |=  LED_value[23]<<4  | LED_value[22]>>8;


  // Messages are sent to the driver through DMA
  dma_spiWrite(&message_spiDriver[0]);
}




/*
 *
 * Each time the programm enter here, a new line is displayed or refreshed
 * The frequence depends of the timer 2 configuration
 *
 */ 
void TIM2_IRQHandler()
{
    //UIF -> Update interrupt flag cleared
    TIM2->SR &= ~(1);
    // next line
    line = (line+1)%8;
    //Send the current line to be modified and send to the driver with SPI
    transform_build_and_send_line(line); 
    // Send the line from the driver to the current line on the matrix
    LATCH_ENABLE;
    //Delay for the LATCH > 30ns
    asm volatile("nop");
    LATCH_DISABLE;
    //Turn on the corresponding line
    LINE(line);
}




/*
 * 
 * Init and activate timer2 to throw at a frequency of 10KHz
 * So the image is refreshed at a frequency of 80KHz 
 *
 */
static void timer2_init()                                                                 
{     
  // Timer 2 clock enable                                                                 
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

  // Configure prescaler                                                                  
  // Fck_psk = 72MHz; PSC = 7199 => Fck_cnt = (Fck_psk)/(PSC+1) = 10KHz                   
  TIM2->PSC =0x1c1f;
  // TIM2->PSC =0xfc1f;
          
  //  TIM2->PSC = 0x1;
  TIM2->ARR = 1;
  
  // Configure timer
  TIM2->CR1 |=  TIM_CR1_ARPE;   //AUto-reload preload enable
  TIM2->CR1 &= ~TIM_CR1_CMS_0 & //Counter edge-aligned mode
               ~TIM_CR1_DIR;    //Upcounter
  
  TIM2->DIER |= TIM_DIER_UIE ;//Update interrupt enabled
  
  TIM2->CR1 |= TIM_CR1_CEN;//Counter enabled

  // Configure interrupt's priority and enable it in NVIC
  NVIC->IP[28] = 100;
  NVIC->ISER[0] = (1<<28);
  
}


/*
 * Init and configure timer 3 to be 
 * a 1MHz clock generator on his channel 3
 * -> PB0 -> LED_PWM
 *
 */
static void timer3_init()
{
  // TIM3 Clock on B port enable
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
  
  // PB0 in output 50MHz -> 10
  GPIOB->CRL |=   GPIO_CRL_MODE0_1 | GPIO_CRL_MODE0_0;

  // PB0 in alternate function push pull :10
  GPIOB->CRL |=  GPIO_CRL_CNF0_1;
  GPIOB->CRL &=  ~GPIO_CRL_CNF0_0;  
 
  //Counter enable
  TIM3->CR1 |= TIM_CR1_CEN;
  
  // pwm1 mode 110
  TIM3->CCMR2 |=  TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1;
  TIM3->CCMR2 &=  ~TIM_CCMR2_OC3M_0 ;
  
  // Output compare 3 preload enable
  TIM3->CCMR2 |= TIM_CCMR2_OC3PE;
  // TIM3_CH3 is configured as output
  TIM3->CCMR2 &= ~TIM_CCMR2_CC3S_0 & ~TIM_CCMR2_CC3S_1;

  // Configure timer
  TIM3->CR1 |=  TIM_CR1_ARPE ; //ARPE on
  TIM3->CR1 &= ~TIM_CR1_DIR  ; //Upcounter
  TIM3->CR1 &= ~TIM_CR1_CMS_0; //edge aligned mode
  TIM3->CR1 &= ~TIM_CR1_CMS_1;

  // active OC3 of TIM3 on PB0
  TIM3->CCER |= TIM_CCER_CC3E;

  // CCR3 value = 1 and ARR value = 2
  // -> We generate a clock signal 
  TIM3->CCR3 = 1;
  TIM3->ARR = 1;
 
  // fck_psc = 72MHz/(71+1) -> 1MHz 
  TIM3->PSC = 71;
  
  // Main output enabled;
  TIM3->BDTR |= TIM_BDTR_MOE;
  // Generate update event
  TIM3->EGR |= TIM_EGR_UG;
}



/*
 *
 * This function:
 *  - Initialize the clock for the control of the PWM for the three groups of color
 *  - We use TIM3_CH3 to generate the clock on out LED_PWM (PB0)
 *  - Configure the brightness level for each color group 
 *    288 vector format for the configutation:
 *    Dot correction GlobalBrightnessR GlobalBrightnessG GlobalBrightnessB  Options
 *    42xF           [R7R6R5R4R3R2R1R0][G7G6G5G4G3G2G1G0][B7B6B5B4B3B2B1B0] 24x0
 */
void driver_config(){
  OUTPUT_DISABLE;

  // Build the 288bits vector
  static unsigned short message_config_spiDriver[18];  
  for(int i=0; i<18; i++)
    message_config_spiDriver[i]=0x0000;

 
  message_config_spiDriver[0]  = LOD_G<<8 | LOD_B;
  message_config_spiDriver[1]  = LSD_B<<8 | LOD_R;
  message_config_spiDriver[2]  = LSD_R<<8 | LSD_G;
  message_config_spiDriver[3]  = TEF&0x1;
  message_config_spiDriver[4]  = USER_DEFINED<<7;
  message_config_spiDriver[5]  = FUNCTION_CONTROL<<9 | USER_DEFINED>>7;
  
  message_config_spiDriver[6]  = (INVERT_8((GLOBAL_BRIGHTNESS_GREEN) & 0xFF))<<8 | (INVERT_8(GLOBAL_BRIGHTNESS_BLUE & 0xFF));
  message_config_spiDriver[7]  = (DOT_CORRECTION_G7&0x1)<<15 | (DOT_CORRECTION_B7)<<8 | INVERT_8(GLOBAL_BRIGHTNESS_RED & 0xFF);
  message_config_spiDriver[8]  = (DOT_CORRECTION_B6&0x3)<<13 | (DOT_CORRECTION_R7)<<6 | (DOT_CORRECTION_G7)>>1 ;
  message_config_spiDriver[9]  = (DOT_CORRECTION_R6&0x5)<<11 | (DOT_CORRECTION_G6)<<4 | (DOT_CORRECTION_B6)>>2;
  message_config_spiDriver[10] = (DOT_CORRECTION_G5&0x7)<<9  | (DOT_CORRECTION_B5)<<2 | (DOT_CORRECTION_R6)>>5;
  message_config_spiDriver[11] = (DOT_CORRECTION_G4&0x2)<<14 | (DOT_CORRECTION_B4)<<7 | (DOT_CORRECTION_R5);
  message_config_spiDriver[12] = (DOT_CORRECTION_B3&0x4)<<12 | (DOT_CORRECTION_R4)<<5 | (DOT_CORRECTION_G4)>>2 ;
  message_config_spiDriver[13] = (DOT_CORRECTION_R3&0x6)<<10 | (DOT_CORRECTION_G3)<<3 | (DOT_CORRECTION_B3)>>4 ;
  message_config_spiDriver[14] = (DOT_CORRECTION_R2&0x1)<<15 | (DOT_CORRECTION_G2)<<8 | (DOT_CORRECTION_B2)<<1 | (DOT_CORRECTION_R3)>>6;
  message_config_spiDriver[15] = (DOT_CORRECTION_G1&0x3)<<13 | (DOT_CORRECTION_B1)<<6 | (DOT_CORRECTION_R2)>>1;
  message_config_spiDriver[16] = (DOT_CORRECTION_B0&0x5)<<11 | (DOT_CORRECTION_R1)<<4 | (DOT_CORRECTION_G1)>>2;
  message_config_spiDriver[17] = (DOT_CORRECTION_R0&0x7)<<9 | (DOT_CORRECTION_G0)<<2 | (DOT_CORRECTION_B0)>>5;
  

  
  LATCH_ENABLE;// Send the line from the driver to the current line on the matrix
    
  // Send the config vector
  dma_spiWrite(&message_config_spiDriver[0]); 
  while((SPI2->SR & SPI_SR_BSY)>>7){}

  LATCH_DISABLE;// Send the config vector to the driver
  for(int i=0; i<100; i++)
    asm volatile("nop");//Delay for the LATCH > 30ns
  LATCH_ENABLE;


}



/*
 *
 * This functions configures STM32 to show images
 * through LED drivers
 *
 */
void image_init()
{
  // Enable clocks for the port controling the led driver and the led matrix 
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN |
  RCC_APB2ENR_IOPBEN;

  // Configure GPIOs
  // For C port
  GPIOC->CRL  = 0x33303333;
  
  GPIOC->CRH &= 0xFF0FFF00;
  GPIOC->CRH |= 0x00300033;
  
  // For B port
  GPIOB->CRL &= 0x00FFFFF0;
  GPIOB->CRL |= 0x33000003;


  // Initialize periphericals (timer and spi/dma)
  spi_init(); 
  dma_spiInit();
  timer3_init(); 
  driver_config();
  timer2_init();
  OUTPUT_ENABLE;

  //  lib_image_reset(bufferIMG);
    
}


#endif
