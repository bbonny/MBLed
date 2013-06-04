/**
  ******************************************************************************
  * @file    flash_if.h 
  * @author  MCD Application Team
  * @modified MBLed team
  * @version V1.0.0
  * @date    07/16/2010 
  * @brief   Header for flash_if.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_IF_H
#define __FLASH_IF_H

/* Includes ------------------------------------------------------------------*/
#include <stm32f10x.h>
#include "stm32f10x_conf.h"
#include "flash_constant.h"



/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define FLASH_PAGE_SIZE   0x400

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void FLASH_If_Write(__IO uint32_t *Address, uint32_t *Data, uint32_t DataLength);
int8_t FLASH_If_Erase(uint32_t StartSector, uint32_t EndSector);
void FLASH_If_Init(void);
void FLASH__If_unlock();
#endif /* __FLASH_IF_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
