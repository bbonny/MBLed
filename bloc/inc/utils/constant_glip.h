#ifndef _constant_glip_h_
#define _constant_glip_h_

/*
 * constant.h
 *
 * In this file, the constants of the project are defined
 * for a 4x4 blocks game maximum.
 * They must be adapted to each configuration.
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

/
#define IMAGE_SIZE 64


typedef enum {
  BROADCAST,
  YOUARE,
  PING,
  PONG,
  YOURTIME
} command_t;

typedef enum {
  HORIZONTAL = 0,
  VERTICAL   = 1
} orientation_t;

#endif
