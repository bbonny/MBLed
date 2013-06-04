/*
 * lib_image.h
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

#ifndef _LIB_IMAGE_H
#define _LIB_IMAGE_H

#ifdef MBLED
#include "image.h"
#else
#include "image_glip.h"
#endif


void lib_image_direct_image(image_t img);
void lib_image_set_pixel(image_t img, pixel_t value, unsigned char i, unsigned char j);
pixel_t lib_image_get_pixel(image_t img, unsigned char i, unsigned char j);
void lib_image_set_image(image_t img_dest, image_t img_source);
void lib_image_rotate90_left_image(image_t img);
void lib_image_rotate90_right_image(image_t img);
void lib_image_set_pixel(image_t img, pixel_t value, unsigned char i, unsigned char j);
void lib_image_set_image_from_buffer(image_t img_dest, image_t buffer_source, unsigned short position);
void lib_image_rotate180_image(image_t img);  

void lib_image_reset(image_t img);
void lib_image_rotImage (image_t img,int rot);


#define MATRICE_SIZE   8 

  
#endif
