/*
 * led_driver_config.h
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

#ifndef H_LED_DRIVER
#define H_LED_DRIVER

/*
 * led_driver_config.h
 *
 * config for the LED driver
 */

//6-0 Dot correction data bits 6-0 for OUTR0
#define DOT_CORRECTION_R0 0xff

//13-7 Dot correction data bits 6-0 for OUTG0
#define DOT_CORRECTION_G0 0xff

//20-14 Dot correction data bits 6-0 for OUTB0
#define DOT_CORRECTION_B0 0xff

//27-21 Dot correction data bits 6-0 for OUTR1
#define DOT_CORRECTION_R1 0xff

//34-28 Dot correction data bits 6-0 for OUTG1
#define DOT_CORRECTION_G1 0xff

//41-35 Dot correction data bits 6-0 for OUTB1
#define DOT_CORRECTION_B1 0xff

//48-42 Dot correction data bits 6-0 for OUTR2
#define DOT_CORRECTION_R2 0xff

//55-49 Dot correction data bits 6-0 for OUTG2
#define DOT_CORRECTION_G2 0xff

//62-56 Dot correction data bits 6-0 for OUTB2
#define DOT_CORRECTION_B2 0xff

//69-63 Dot correction data bits 6-0 for OUTR3
#define DOT_CORRECTION_R3 0xff

//76-70 Dot correction data bits 6-0 for OUTG3
#define DOT_CORRECTION_G3 0xff

//83-77 Dot correction data bits 6-0 for OUTB3
#define DOT_CORRECTION_B3 0xff

//90-84 Dot correction data bits 6-0 for OUTR4
#define DOT_CORRECTION_R4 0xff

//97-91 Dot correction data bits 6-0 for OUTG4
#define DOT_CORRECTION_G4 0xff

//104-98 Dot correction data bits 6-0 for OUTB4
#define DOT_CORRECTION_B4 0xff

//111-105 Dot correction data bits 6-0 for OUTR5
#define DOT_CORRECTION_R5 0xff

//118-112 Dot correction data bits 6-0 for OUTG5
#define DOT_CORRECTION_G5 0xff

//125-119 Dot correction data bits 6-0 for OUTB5
#define DOT_CORRECTION_B5 0xff

//132-126 Dot correction data bits 6-0 for OUTR6
#define DOT_CORRECTION_R6 0xff

//139-133 Dot correction data bits 6-0 for OUTG6
#define DOT_CORRECTION_G6 0xff

//146-140 Dot correction data bits 6-0 for OUTB6
#define DOT_CORRECTION_B6 0xff

//153-147 Dot correction data bits 6-0 for OUTR7
#define DOT_CORRECTION_R7 0xff

//160-154 Dot correction data bits 6-0 for OUTG7
#define DOT_CORRECTION_G7 0xff

//167-161 Dot correction data bits 6-0 for OUTB7
#define DOT_CORRECTION_B7 0xff

//175-168 Global brightness control data bits 7-0 for OUTR0-OUTR7 group
#define GLOBAL_BRIGHTNESS_RED   0xff  

//183-176 Global brightness control data bits 7-0 for OUTG0-OUTG7 group
#define GLOBAL_BRIGHTNESS_GREEN 0x20

//191-184 Global brightness control data bits 7-0 for OUTB0-OUTB7 group
#define GLOBAL_BRIGHTNESS_BLUE  0x30

//198-192 Function control data bits 6-0
#define FUNCTION_CONTROL 0x3f

//215-199 User-defined data bits 16-0
#define USER_DEFINED 0x00

//238-216 Reserved for TI test
#define RESERVED 0x00

//239 Thermal error flag (TEF)
  //1 = High temperature condition, 0 = Normal temperature condition
#define TEF 0x0  

//247-240 LED short detection (LSD) data for OUTR7-OUTR0
   //1 = LED is short, 0 = Normal operation
#define LSD_R 0x00

//255-248 LSD data for OUTG7-OUTG0
   //1 = LED is short, 0 = Normal operation
#define LSD_G 0x00

//263-256 LSD data for OUTB7-OUTB0
   //1 = LED is short, 0 = Normal operation
#define LSD_B 0x00
  
//271-264 LED open detection (LOD) data for OUTR7-OUTR0
   //1 = LED is open or connected to GND, 0 = Normal operation
#define LOD_R 0x00

//279-272 LOD data for OUTG7-OUTG0
   //1 = LED is open or connected to GND, 0 = Normal operation
#define LOD_G 0x00

//287-280 LOD data for OUTB7-OUTB0
   //1 = LED is open or connected to GND, 0 = Normal operation
#define LOD_B 0x00

#endif
