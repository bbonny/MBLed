/*
 * glip-image.c
 *
 * This module shows animations on LED. Since we use a LED driver,
 * we can only light a line of the matrix ; we use persistence of vision
 * to show complete images.
 * We use 3-colors-LED (RGB) ; this module uses PWM to show more different
 * colors.
 *
 *
 * Copyright 2010 - Mickaël Camus <mickaelcamus.mc@gmail.com>,
 *                  Enzo Casasola <enzocasasola@gmail.com>,
 *                  Julie Estivie <juliestivie@gmail.com>,
 *                  Florent Matignon <florent.matignon@gmail.com>
 *
 * All rights reserved.
 * GLiP (Great LEDs interactive Puzzle)
 * Telecom ParisTech - ELECINF344/ELECINF381
 *
 * This file is part of GLiP Project.
 *
 * GLiP Project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GLiP Project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GLiP Project.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 */

#ifndef MBLED

// STM32 features
#include <stm32f10x.h>

// GLiP features
#include "constant.h"
#include "image_glip.h"
#include "spi_led_glip.h"

// Theses macros let to use LED Drivers
#define OUTPUT_ENABLE do{GPIOB->BSRR= 0x00010000;} while(0) //PB0 LOW
#define OUTPUT_DISABLE do{GPIOB->BSRR= 0x00000001;} while(0) //PB0 HIGH

#define LATCH_ENABLE do{GPIOA->BSRR= 0x00000010;} while(0) //PA4 HIGH
#define LATCH_DISABLE do{GPIOA->BSRR= 0x00100000;} while(0) //PA4 LOW

#define PCn(n) ((1 << (16+(n))) | (0xff & ~(1 << (n))))
static const uint32_t bsrrs[8] = {PCn(4), PCn(5), PCn(6), PCn(7),
				  PCn(0), PCn(1), PCn(2), PCn(3)};
#undef PCn

#define NOLINE GPIOC->BSRR= 0x000000ff
#define LINE(n) GPIOC->BSRR = bsrrs[n]

// current image modified by the display task
image_t bufferIMG;



void set_image_pointer(image_t source){
  bufferIMG = source;
}

/*
 *
 * This function gets a vector of 32 bits (or integer) that represents
 * the state of each LED of a line (comming from shift_pwm_line) and 
 * transforms it to two words of 16 bits each one that are send to the
 * SPI drivers to light the corresponding LEDS.
 *
 * The format which is going to be translated to, depends on the 
 * associations between drivers' outputs and LEDs, which are: 
 *
 * First Driver
 * OUT0  -> G4   OUT1  -> R4   OUT2  -> G3   OUT3  -> B2
 * OUT4  -> R2   OUT5  -> G1   OUT6  -> B0   OUT7  -> R0
 * OUT8  -> -    OUT9  -> -    OUT8  -> -    OUT11 -> -
 * OUT12 -> B5   OUT13 -> R6   OUT14 -> G6   OUT15 -> B7
 *
 * Second Driver
 * OUT0  -> -    OUT1  -> -    OUT2  -> -    OUT3  -> R3
 * OUT4  -> G2   OUT5  -> B1   OUT6  -> R1   OUT7  -> G0
 * OUT8  -> B3   OUT9  -> B4   OUT8  -> R5   OUT11 -> G5
 * OUT12 -> B6   OUT13 -> R7   OUT14 -> G7   OUT15 -> -
 *
 */

void shift_rgb(unsigned int value)
{
  // Theses macros are used to set pixel's values
#define R(n) (value & (1 << (3*(n))))
#define G(n) (value & (1 << (3*(n)+1)))
#define B(n) (value & (1 << (3*(n)+2)))
#define P(c, b) ((c) ? (1 << (b)) : 0)

  static unsigned short message_spiDrivers[2];

  // Message for the first driver (see associations)
  message_spiDrivers[1]= 
    P(G(4), 15) |
    P(R(4), 14) |
    P(G(3), 13) |
    P(B(2), 12) |
    P(R(2), 11) |
    P(G(1), 10) |
    P(B(0), 9)  |
    P(R(0), 8)  |
    P(B(5), 3)  |
    P(R(6), 2)  |
    P(G(6), 1)  |
    P(B(7), 0);

  // Message for the second driver (see associations)
  message_spiDrivers[0]=
    P(R(3), 12) |
    P(G(2), 11) |
    P(B(1), 10) |
    P(R(1), 9)  |
    P(G(0), 8)  |
    P(B(3), 7)  |
    P(B(4), 6)  |
    P(R(5), 5)  |
    P(G(5), 4)  |
    P(B(6), 3)  |
    P(R(7), 2)  |
    P(G(7), 1);

#undef R
#undef G
#undef B
#undef P

  // Messages are sent to drivers through DMA
  dma_spiWrite(&message_spiDrivers[0]);
}

/*
 *
 * This function builds a vector of 32bits (or integer) that represents the on(1) / off(0)
 * state of each RGB-LED of one line in the image. 
 * 
 * The format of the vector : (Only the 24 LSB are important)
 * X X X X X X X X B7 G7 R7 B6 G6 R6 B5 G5 R5 B4 G4 R4 B3 G3 R3 B2 G2 R2 B1 G1 R1 B0 G0 R0
 *
 * Given that the variable image contains the level of intensité of one color of one 
 * Led in an image (for example 4), the idea is to compare this value with a polling
 * reference (see TIM2_IRQHandler) pwm_level to know if the led must be on or off. 
 * In the example, that Led is going to be on while pwm_level is 0, 1, 2 or 3.
 *
 */
void shift_pwm_line(unsigned int line, unsigned int pwm_level)
{
  unsigned short *l = &bufferIMG[line*8];
  unsigned int value = 0;	//Vector of 32 bits
  for (unsigned i = 0; i < 8; i++)
  {
    //Find if the intensity of the RED element of the LED i is bigger than the reference
    if((unsigned)((l[i]&0x0F00)>>8) > pwm_level)
      value |= 1 << 3*i;	// If so, set the corresponding bit
    //Find if the intensity of the GREEN element of the LED i is bigger than the reference
    if((unsigned)((l[i]&0x00F0)>>4) > pwm_level)
      value |= 1 << (3*i+1);
    //Find if the intensity of the BLUE element of the LED i is bigger than the reference
    if((l[i]&0x000F) > pwm_level)
      value |= 1 << (3*i+2);
  }
  //Send the resulting vector to be translated to can be interpreted by the spiDrivers
  shift_rgb(value);
}

void TIM2_IRQHandler()
{
  static int line=0;
  static unsigned int pwm_level = 0;
  static char firstTime=1;
  TIM2->SR &= ~(1);		//UIF -> Update interrupt flag cleared

  if(!firstTime)
    goto _labeltmr;
  // The first time we enter here . . .
  OUTPUT_ENABLE;
  NOLINE;
  LATCH_DISABLE;
  firstTime=0;

  for(;;)
  {
    /*
     *Here we make the polling of pwm_level, from 0 to PWM_MAX_LEVEL, that represents the maximum
     *power level of the leds.
     */
    for (pwm_level = 0; pwm_level < PWM_MAX_LEVEL; pwm_level++)
    {
      //For a given pwm_level value, we make the calculations for all lines
      for (line=0;line<8;line++)
      {
        shift_pwm_line(line, pwm_level); //Send the current line to know the on/off states
        return;
	// Except for the first time we enter the routine, after clearing the flag, we start here
      _labeltmr:
        NOLINE;		//All line in OFF
        LATCH_ENABLE;	
        LINE(line);	//Turn on the corresponding line
        asm volatile("nop");	//Delay for the LATCH
        LATCH_DISABLE;
      }
    }
  }
}


/*
 *
 * This function configures timer2 to be used as PWM generator
 *
 */
static void timer2_init()
{
  // Enable TIM2
  RCC->APB1ENR |= 1;

  // Configure prescaler
  // Fck_psk = 72MHz; PSC = 899 => Fck_cnt = (Fck_psk)/(PSC+1) = 80KHz
  TIM2->PSC = 0x383;
  TIM2->ARR = 10;

  // Configure timer
  TIM2->CR1 = (1<<7)|		//ARR Register buffered
              (0<<5)|		//Counter edge-aligned mode
              (0<<4);		//Upcounter

  TIM2->DIER = 1;		//Update interrupt enabled

  TIM2->CR1 |= 1;		//Counter enabled

  // Configure interrupt's priority and enable it in NVIC
  NVIC->IP[28] = 100;
  NVIC->ISER[0] = (1<<28);
}

/*
 *
 * This functions configures STM32 to show images
 * through LED drivers
 *
 */
void image_init()
{
  // Enable clocks
  RCC->APB2ENR |= 0x00000018;
  // Configure GPIOs
  GPIOC->CRL = 0x33333333;
  GPIOB->CRL = 0x00000003;

  // Initialize periphericals (timer and spi/dma)
  timer2_init();
  spi_init();
  dma_spiInit();
}


#endif
