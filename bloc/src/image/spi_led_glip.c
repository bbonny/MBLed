/*
 * spi.c
 *
 * This module configures STM32 registers for SPI and DMA.
 * It also allows to write data to SPI devices through DMA.
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

/*
 *
 * This function configures STM32 registers for SPI
 * as far as LED drivers are SPI devices.
 *
 */
void spi_init()
{
  // Enable SPI, PA and AF clocks
  RCC->APB2ENR |= 0x00001005;
  // Configure GPIOs
  GPIOA->CRL = (GPIOA->CRL & 0x0000FFFF) | 0xB4B30000;

  // Configure SPI
  SPI1->CR1 = (1<<11)| // 16 bits Data Frame Format
    (1<<9)|            // Software slave management enabled
    (1<<8)|            // Internal slave select
    (1<<7)|            // LSB transmitted first in frame
    (1<<6)|            // Enable SPI
    (6<<3)|            // Clock Frequence : fPCLK/128
    (1<<2)|            // Configured as Master
    (0<<1)|           // Steady value = 0
    (0);             // First edge (Rising edge)
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
  DMA1_Channel3->CPAR = (unsigned int) &SPI1->DR;

  // Configure DMA
  DMA1_Channel3->CNDTR = 2;     // 2 data to transfer
  DMA1_Channel3->CCR = (1<<10)| // 16 bit bits Memory
    (1<<8)|                     // 16 bits Peripheral
    (1<<7)|                     // Memory increment
    (1<<4);                    // From memory to peripherical
}

/*
 *
 * This function is used to write data to SPI device through DMA
 *
 */
void dma_spiWrite(unsigned short *n)
{
  // Disable DMA
  DMA1_Channel3->CCR &= ~1;
  // 2 data to transfer
  DMA1_Channel3->CNDTR = 2;
  // Where we will write
  DMA1_Channel3->CMAR = (unsigned int) n;
  // Enable DMA
  DMA1_Channel3->CCR |= 1;
  // Enable TX Buffer
  SPI1->CR2 |= (1<<1);
}

#endif
