/*
 * various_graphic.h
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

#ifndef H_VARIOUS
#define H_VARIOUS

#define COLOR     0x001, 0x002, 0x003, 0x004 ,0x005 ,0x006 ,0x007 ,0x008, \
                  0x009, 0x00a, 0x00b, 0x00c ,0x00d ,0x00e ,0x00f ,0xfff, \
                  0x010, 0x020, 0x030, 0x040 ,0x050 ,0x060 ,0x070 ,0x080,		\
                  0x090, 0x0a0, 0x0b0, 0x0c0 ,0x0d0 ,0x0e0 ,0x0f0 ,0xfff, \
                  0x100, 0x200, 0x300, 0x400 ,0x500 ,0x600 ,0x700 ,0x800, \
                  0x900, 0xa00, 0xb00, 0xc00 ,0xd00 ,0xe00 ,0xf00 ,0xfff, \
                  0x111, 0x222, 0x333, 0x444 ,0x555 ,0x666 ,0x777 ,0x888, \
                  0x999, 0xaaa, 0xbbb, 0xccc ,0xddd ,0xeee ,0xfff ,0xfff


#define ARROW(n)  0, 0, 0, n, n, 0, 0, 0, \
                  0, 0, n, n, n, n, 0, 0, \
                  0, n, n, n, n, n, n, 0, \
                  0, 0, 0, n, n, 0, 0, 0, \
                  0, 0, 0, n, n, 0, 0, 0, \
                  0, 0, 0, n, n, 0, 0, 0, \
                  0, 0, 0, n, n, 0, 0, 0, \
                  0, 0, 0, n, n, 0, 0, 0



#endif
