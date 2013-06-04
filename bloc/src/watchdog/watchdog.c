/*
 * watchdog.c
 *
 *
 * instantiate a watchdog which will reset the micro-controler if no activity during 400ms
 *
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


#include <stm32f10x.h>


/*
 * watchdog_init
 *
 */
void watchdog_init(){
  IWDG->KR = 0x5555; // access to IWDG_PR and IWDG_RLR enable
  IWDG->PR = 0x0;    // prescaler divider = 4
  IWDG->KR = 0x5555; // access to IWDG_PR and IWDG_RLR enable 
  IWDG->RLR = 0xFFF; // reset after 409,6 ms
  IWDG->KR = 0xCCCC; // begin decount

}

/*
 *
 * reload watchdog
 */
void watchdog_reload(){
  IWDG->KR = 0x5555;
  IWDG->KR = 0xAAAA;
}
