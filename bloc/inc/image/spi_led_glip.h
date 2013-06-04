#ifndef MBLED

#ifndef _SPI_H
#define _SPI_H

/*
 * spi.h
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

void spi_init(void);
void spi_tx(short);
void dma_spiInit(void);
void dma_spiWrite(unsigned short *);

#endif // _SPI_H

#endif
