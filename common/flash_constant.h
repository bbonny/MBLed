/*
 * flash_constant.h
 *
 * North showing application
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

#ifndef __FLASH_CONSTANT_H
#define __FLASH_CONSTANT_H

#define FIRMWARE_FIRST_PAGE_ADDRESS  0x08003000
#define FIRMWARE_LAST_PAGE_ADDRESS   0x08006000

#define NEW_FIRMWARE_FIRST_PAGE_ADDRESS  0x08012000
#define NEW_FIRMWARE_LAST_PAGE_ADDRESS   0x08015000

#define FIRMWARE_TEMP_FIRST_PAGE_ADDRESS  0x08018000
#define FIRMWARE_TEMP_LAST_PAGE_ADDRESS   0x0801B000


#define FIRMWARE_SIZE (FIRMWARE_LAST_PAGE_ADDRESS - FIRMWARE_FIRST_PAGE_ADDRESS)

#define FLASH_PROGRAMMER 0x08000000

#define USER_FLASH_END_ADDRESS        0x0803FFFF  


#endif

