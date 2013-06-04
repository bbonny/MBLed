/**
  ******************************************************************************
  * @file    flash_if.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    07/16/2010 
  * @brief   This file provides high level routines to manage internal Flash 
  *          programming (erase and write). 
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "flash_if.h"
#include "debug.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FLASH_KEY1               ((uint32_t)0x45670123)
#define FLASH_KEY2               ((uint32_t)0xCDEF89AB)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Unlocks Flash for write access
  * @param  None
  * @retval None
  */
void FLASH_If_Init(void)
{ 
  /* Authorize the FPEC Access */
  FLASH->KEYR = FLASH_KEY1;
  FLASH->KEYR = FLASH_KEY2;
}

/**
  * This function does an erase of all pages between StartSector and EndSector
  */
int8_t FLASH_If_Erase(uint32_t StartSector, uint32_t EndSector )
{
  uint32_t FlashAddress;
  FlashAddress = StartSector;
  while(FlashAddress<=EndSector){
    if (FLASH_ErasePage(FlashAddress) != FLASH_COMPLETE)
      {
	return -1;
    }
    FlashAddress += FLASH_PAGE_SIZE;
  }  
  return 0;
}

/**
  * @brief  This function writes a data buffer in flash (data are 32-bit aligned)
  * @param  FlashAddress: start address for writing data buffer
  * @param  Data: pointer on data buffer
  * @param  DataLength: length of data buffer (unit is 32-bit word)   
  * @retval None
  */
void FLASH_If_Write(__IO uint32_t* FlashAddress, uint32_t* Data ,uint32_t DataLength)
{
  uint32_t i = 0;
  
  for (i = 0; i < DataLength/4; i++)
  {      
      if (FLASH_ProgramWord((uint32_t)FlashAddress, *(uint32_t*)(Data + i)) == FLASH_COMPLETE)
      {
        FlashAddress += 1;
      }
      else
      {
        return;
      }
    }
}

void FLASH__If_unlock(){
  writeDebugInit("Flash if unlock\r\n");
  FLASH_If_Init();
  writeDebugInit(" Write Flash protection is :");
  writeHex32Debug(FLASH_GetWriteProtectionOptionByte());
  writeDebugInit("\r\n");
  if (FLASH_GetWriteProtectionOptionByte()!= 0xffffffff)
    FLASH_EraseOptionBytes();
}



/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
