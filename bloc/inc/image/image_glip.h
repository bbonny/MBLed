#ifndef _GLIP_IMAGE_H
#define _GLIP_IMAGE_H

/*
 * glip-image.h
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


#define PWM_MAX_LEVEL 10


// This type describes an image to show
typedef unsigned short pixel_t;
typedef pixel_t *image_t;


void image_init(void);
void set_image_pointer(image_t source);

#endif
