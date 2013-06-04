/*
 * lib_image.c
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

#include "FreeRTOS.h"
#include "task.h"

#include "lib_image.h"
#ifdef MBLED
#include "image.h"
#else
#include "image_glip.h"
#endif
#include "debug.h"
#include "constant.h"
#include "algo.h"




/*
 *
 *  Direct the image up->north, etc.
 *  using direction
 *
 */
void lib_image_direct_image(image_t img){
  switch(getDirection()%4){
  case 1:
    lib_image_rotate90_left_image(img);
    break;
  case 2:
    lib_image_rotate180_image(img);
    break;
  case 3: 
    lib_image_rotate90_right_image(img);
    break;
  default:
    break;
  } 
}

/*
 *
 * Reset or init an image (fill it with zeros)
 *
 */
void lib_image_reset(image_t img){
  for(int i=0; i<64; i++)
    img[i] = 0;
}


/*
 *
 *  Set the pixel (x,y) of img with value
 *
 */
void lib_image_set_pixel(image_t img, pixel_t value, unsigned char i, unsigned char j){
  if((i*MATRICE_SIZE + j >= 0) && (i*MATRICE_SIZE + j < 64) )
    img[i*MATRICE_SIZE + j] = value;
}

/*
 *
 *  Get the pixel (x,y) of img with value
 *
 */
unsigned short lib_image_get_pixel(image_t img, unsigned char i, unsigned char j){
  if((i*MATRICE_SIZE + j >= 0) && (i*MATRICE_SIZE + j < 64) )
    return img[i*MATRICE_SIZE + j];
  else return 0;
}


/*
 *
 *  Replace img_dest with img_source
 *
 */
void lib_image_set_image(image_t img_dest, image_t img_source){
  for(int i=0; i< MATRICE_SIZE*MATRICE_SIZE; i++)
    img_dest[i] = img_source[i];
}

/*
 *
 * Find in the buffer image in position position
 * and put it into img_dest
 *
 */
void lib_image_set_image_from_buffer(image_t img_dest, image_t buffer_source, unsigned short position){
  for(int i=0; i< MATRICE_SIZE*MATRICE_SIZE; i++)
    img_dest[i] = buffer_source[i+MATRICE_SIZE*MATRICE_SIZE*position];
}


/*
 *
 * rotate img from 90 degre on the left
 *
 */
void lib_image_rotate90_left_image(image_t img){
  for (unsigned char j=0; j<MATRICE_SIZE/2; ++j){
    for (unsigned char i=j;i<MATRICE_SIZE-1-j;i++){
      pixel_t temp = lib_image_get_pixel(img,j,i);
      lib_image_set_pixel(img,lib_image_get_pixel(img,MATRICE_SIZE-1-i,j),j,i);
      lib_image_set_pixel(img,lib_image_get_pixel(img,MATRICE_SIZE-1-j,MATRICE_SIZE-1-i),MATRICE_SIZE-1-i,j);
      lib_image_set_pixel(img,lib_image_get_pixel(img,i,MATRICE_SIZE-1-j),MATRICE_SIZE-1-j,MATRICE_SIZE-1-i);
      lib_image_set_pixel(img,temp,i,MATRICE_SIZE-1-j);
    }
  }
}

/*
 *
 * rotate img from 90 degre on the right
 *
 */  
void lib_image_rotate90_right_image(image_t img){
  for (unsigned char j=0; j<MATRICE_SIZE/2; ++j){
    for (unsigned char i=j;i<MATRICE_SIZE-1-j;i++){
      pixel_t temp = lib_image_get_pixel(img,i,j);
      lib_image_set_pixel(img,lib_image_get_pixel(img,j,MATRICE_SIZE-1-i),i,j);
      lib_image_set_pixel(img,lib_image_get_pixel(img,MATRICE_SIZE-1-i,MATRICE_SIZE-1-j),j,MATRICE_SIZE-1-i);
      lib_image_set_pixel(img,lib_image_get_pixel(img,MATRICE_SIZE-1-j,i),MATRICE_SIZE-1-i,MATRICE_SIZE-1-j);
      lib_image_set_pixel(img,temp,MATRICE_SIZE-1-j,i);
    }
  }
}
  

/*
 *
 * rotate img from 180 degre on the right
 *
 */  
void lib_image_rotate180_image(image_t img){
  lib_image_rotate90_right_image(img);
  lib_image_rotate90_right_image(img);
}
  
  
/*
 *
 * This function is called from glip-animation.c with a rot parameter
 *
 */
void lib_image_rotImage (image_t img,int rot) {
  switch (rot) {
  case 1:
    lib_image_rotate90_right_image(img);
    break;
  case 2:
    lib_image_rotate180_image(img);
    break;
  case 3:
    lib_image_rotate90_left_image(img);
    break;
  default:
    break;
  }
}
