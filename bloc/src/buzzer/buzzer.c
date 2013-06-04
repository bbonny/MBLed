/*
 * buzzer.c
 *
 * Functions to define how the buzzer works (only for MBLed)
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

#ifdef MBLED
void buzzer_init(){
 // TIM3 Clock on B port enable
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
  
  // PB0 in output 50MHz -> 10
  GPIOB->CRL |=   GPIO_CRL_MODE0_1 | GPIO_CRL_MODE0_0;

  // PB0 in alternate function push pull :10
  GPIOB->CRL |=  GPIO_CRL_CNF0_1;
  GPIOB->CRL &=  ~GPIO_CRL_CNF0_0;  
 
  // autorise IOPB                                                           
  RCC->APB2ENR |= (1<<3);                                                    
 
  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
  
  // B9 in output 2MHz -> 10
  GPIOB->CRH |=   GPIO_CRH_MODE9_1 | GPIO_CRH_MODE9_0;

  // B9 en alternate function push pull :10
  GPIOB->CRH |=  GPIO_CRH_CNF9_1;
  GPIOB->CRH &=  ~GPIO_CRH_CNF9_0;  
 
  //Counter enable
  TIM4->CR1 |= TIM_CR1_CEN;
  
  // pwm1 mode 110
  TIM4->CCMR2 |=  TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1;
  TIM4->CCMR2 &=  ~TIM_CCMR2_OC4M_0 ;
  
  // Output compare 3 preload enable
  TIM4->CCMR2 |= TIM_CCMR2_OC4PE;
  // TIM3_CH3 is configured as output
  TIM4->CCMR2 &= ~TIM_CCMR2_CC4S_0 & ~TIM_CCMR2_CC4S_1;

  // Configure timer
  TIM4->CR1 |=  TIM_CR1_ARPE ; //ARPE on
  TIM4->CR1 &= ~TIM_CR1_DIR  ; //Upcounter 
  TIM4->CR1 &= ~TIM_CR1_CMS_0; //edge aligned mode
  TIM4->CR1 &= ~TIM_CR1_CMS_1;

  // active OC3 of TIM3 on PB0
  TIM4->CCER |= TIM_CCER_CC4E;

  // CCR3 value = 1 and ARR value = 2
  // -> We generate a clock signal 
  TIM4->CCR4 = 999;
  TIM4->ARR = 1000;
 
  // fck_psc = 72MHz/(71+1) -> 1MHz 
  TIM4->PSC = 9999 ;
  
  // Main output enabled;
  TIM4->BDTR |= TIM_BDTR_MOE;
  // Generate update event
  TIM4->EGR |= TIM_EGR_UG;
 
  TIM4->BDTR &= ~(1<<15);
  TIM4->CCR4 = 1001;
 
}


void buzzer_put_freq(unsigned long freq){
  if(freq==0){
    TIM4->BDTR &= ~(1<<15);
    TIM4->CCR4 = 1001;
 
  }
  else{
    TIM4->CCR4 = BUZZER_VOLUME;
   // Output enabled;
    TIM4->BDTR |= (1<<15);
    // Generate update event
    TIM4->EGR |= 1<<4 | 1<<6 | 1<<0;
    TIM4->PSC = (72000/freq) -1;
  }
}


void buzzer_check(){
  for(;;){
    buzzer_put_freq(DO5);
    vTaskDelay(10);
    buzzer_put_freq(0);
    vTaskDelay(3000);
  }
}

void buzzer_play_gamme(){
  for(;;){
    buzzer_put_freq(DO5);
    writeDebugInit("buzz!!!\n\r");
    vTaskDelay(500);
    buzzer_put_freq(RE5);
    vTaskDelay(500);
    buzzer_put_freq(MI5);
    vTaskDelay(500);
    buzzer_put_freq(FA5);
    vTaskDelay(500);
    buzzer_put_freq(SOL5);
    vTaskDelay(500);
    buzzer_put_freq(LA5);
    vTaskDelay(500);
    buzzer_put_freq(SI5);
    vTaskDelay(500);
  }
}

#else


void buzzer_init(){}
void buzzer_put_freq(unsigned long freq){}
void buzzer_check(){}
void buzzer_play_gamme(){}


#endif
