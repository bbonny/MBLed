#ifndef _animation_desc_h_
#define _animation_desc_h_

/*
 * animationDescriptor.h
 *
 * This module defines the structure which describes a GLiP animation.
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

typedef struct {
  unsigned char name[20]; //Name
  unsigned int nb_images; //Number of images (frames)
  unsigned int delai; //Delay between frames
  unsigned int nb_lignes; //Number of lines
  unsigned int nb_colonnes; //Number of columns
  const unsigned short *data; //Colors of the pixels
  unsigned int stopMap; //0 if the map is computed 
  unsigned int desynchronize; //0 if blocks are synchronized
} AnimationDescriptor;

#endif
