/*
 * spi_led.c
 *
 * Initialization for the SPI2 used to communicate with the LED Driver
 * Initialization for DMA used with SPI2
 * Sends data to the driver
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
#ifdef MBLED

#include <stm32f10x.h>



/*
 *
 * This function is used to write data to SPI device through DMA
 *
 */
void dma_spiWrite(unsigned short *n)
{
  // Disable DMA
  DMA1_Channel5->CCR &= ~1;
  // 18 data to transfer
  DMA1_Channel5->CNDTR = 18;
  // Where we will write
  DMA1_Channel5->CMAR = (unsigned int) n;
  // Enable DMA
  DMA1_Channel5->CCR |= 1;
  // Enable TX Buffer
  SPI2->CR2 |= (1<<1);
  
  // Wait the end of the transfer
  while((SPI2->SR & SPI_SR_BSY)>>7){}
}


/*
 *
 * This functions configures STM32 registers to send
 * data to SPI through DMA.
 *
 */
void dma_spiInit()
{
  // Enable DMA1 Clock
  RCC->AHBENR |= 1;
  // Where we will write
  DMA1_Channel5->CPAR = (unsigned int) &SPI2->DR;
  // 18 data to transfer
  DMA1_Channel5->CNDTR = 18;
  // Configure DMA
   DMA1_Channel5->CCR = (1<<10)| // 16 bit bits Memory
    (1<<8)|                     // 16 bits Peripheral
    (1<<7)|                     // Memory increment
    (1<<4);                    // From memory to peripherical
}



/*
 *
 * This function configures STM32 registers for SPI
 * to transmit data to the LED driver.
 *
 */
void spi_init()
{
  
  // Alternate function I/O enable
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN ;
  // Port B clock enable
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

  // SPI2 clock enable
  RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
 
  // Configure GPIOs for SPI2
  GPIOB->CRH &= 0x0000FFFF;
  GPIOB->CRH |= 0xB4B30000;

  
  // Configure SPI2
  SPI2->CR1 &= 0;
  SPI2->CR1 |= 
    SPI_CR1_DFF      | // 16 bits Data Frame Format
    SPI_CR1_SSM      | // Software slave management enabled
    SPI_CR1_SSI      | // Internal slave select
    SPI_CR1_LSBFIRST | // LSB transmitted first in frame
    (0<<3)           | // Clock Frequence : fPCLK/2 = 36MHz/2 = 18MHz
    SPI_CR1_MSTR     ; // Configured as Master
  SPI2->CR1 &=
    ~SPI_CR1_CPOL    & // CK to 0 when idle
    ~SPI_CR1_CPHA    ; // First edge (Rising edge)

  // Enable SPI
  SPI2->CR1 |= SPI_CR1_SPE;  
}
#endif
