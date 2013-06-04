/*
 * main.c
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


// STM32 features
#include <stm32f10x.h>
#include <core_cm3.h>

// MBLed features

#include "flash_constant.h"
#include "flash_if.h"



void firmware_execute_firmware(){
  // Execute firmware
  void (*f)(void) = (void *)((unsigned char *)FIRMWARE_FIRST_PAGE_ADDRESS+0x1); 
  f();
}
 
    
/*
 *
 * Erase the fimware sector
 *
 */
void firmware_erase_firmware(){
 /* Enable flash writing */
  FLASH_If_Init();
  FLASH_If_Erase((uint32_t)FIRMWARE_FIRST_PAGE_ADDRESS, (uint32_t)FIRMWARE_LAST_PAGE_ADDRESS);  
}

/*
 *
 * Erase the new_fimware sector
 *
 */
void firmware_erase_new_firmware(){
  /* Enable flash writing */
  FLASH_If_Init();
  FLASH_If_Erase((uint32_t)NEW_FIRMWARE_FIRST_PAGE_ADDRESS, (uint32_t)NEW_FIRMWARE_LAST_PAGE_ADDRESS);
}

void firmware_erase_firmware_temp(){
  /* Enable flash writing */
  FLASH_If_Init();
  FLASH_If_Erase((uint32_t)FIRMWARE_TEMP_FIRST_PAGE_ADDRESS, (uint32_t)FIRMWARE_TEMP_LAST_PAGE_ADDRESS);
}

/*
 *
 * move the new_fimware to the firmware sector
 *
 */
void firmware_write_firmware_with_new_firmware(){
  firmware_erase_firmware();
  FLASH_If_Write((uint32_t*)FIRMWARE_FIRST_PAGE_ADDRESS, (uint32_t*)NEW_FIRMWARE_FIRST_PAGE_ADDRESS, FIRMWARE_SIZE);
}

/*
 *
 * move the new_fimware to the firmware sector
 *
 */
void firmware_write_firmware_temp_with_firmware(){
  firmware_erase_firmware_temp();
  FLASH_If_Write((uint32_t*)FIRMWARE_TEMP_FIRST_PAGE_ADDRESS, (uint32_t*)FIRMWARE_FIRST_PAGE_ADDRESS, FIRMWARE_SIZE);
}

/*
 *
 * move the new_fimware to the firmware sector
 *
 */
void firmware_write_new_firmware_with_firmware_temp(){
  firmware_erase_new_firmware();
  FLASH_If_Write((uint32_t*)NEW_FIRMWARE_FIRST_PAGE_ADDRESS, (uint32_t*)FIRMWARE_TEMP_FIRST_PAGE_ADDRESS, FIRMWARE_SIZE);
}


/*
 *
 * move the fimware to the new_firmware sector
 *
 */
void firmware_write_new_firmware_with_firmware(){
  firmware_erase_new_firmware();
  FLASH_If_Write((uint32_t*)NEW_FIRMWARE_FIRST_PAGE_ADDRESS, (uint32_t*)FIRMWARE_FIRST_PAGE_ADDRESS, FIRMWARE_SIZE);
}


int main()
{

  //firmware_write_new_firmware_with_firmware();

  
  // Copie le fimware courant dans le firmware temporaire
  firmware_write_firmware_temp_with_firmware();
  
  // Copie le new fimware dans le firmware
  firmware_write_firmware_with_new_firmware();
    
  // copie le firmware temp dans le new firmware
  firmware_write_new_firmware_with_firmware_temp();
  
  //firmware_write_new_firmware_with_firmware();
  

  // Execute firmware
  firmware_execute_firmware();
  while(1){}
}

