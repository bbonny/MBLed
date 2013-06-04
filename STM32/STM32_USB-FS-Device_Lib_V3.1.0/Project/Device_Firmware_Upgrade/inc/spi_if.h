/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : dfu_mal.h
* Author             : MCD Application Team
* Version            : V3.1.0
* Date               : 10/30/2009
* Description        : Header for dfu_mal.c file.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_IF_MAL_H
#define __SPI_IF_MAL_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

uint16_t SPI_If_Init(void);
uint16_t SPI_If_Erase (uint32_t SectorAddress);
uint16_t SPI_If_Write (uint32_t SectorAddress, uint32_t DataLength);
uint8_t *SPI_If_Read (uint32_t SectorAddress, uint32_t DataLength);

#endif /* __SPI_IF_MAL_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
