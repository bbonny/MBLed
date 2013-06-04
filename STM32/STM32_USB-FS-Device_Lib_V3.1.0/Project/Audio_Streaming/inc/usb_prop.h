/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : usb_prop.h
* Author             : MCD Application Team
* Version            : V3.1.0
* Date               : 10/30/2009
* Description        : All processing related to Mass Storage Demo (Endpoint 0)
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usb_prop_H
#define __usb_prop_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Speaker_init(void);
void Speaker_Reset(void);
void Speaker_SetConfiguration(void);
void Speaker_SetDeviceAddress (void);
void Speaker_Status_In (void);
void Speaker_Status_Out (void);
RESULT Speaker_Data_Setup(uint8_t);
RESULT Speaker_NoData_Setup(uint8_t);
RESULT Speaker_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting);
uint8_t *Speaker_GetDeviceDescriptor(uint16_t );
uint8_t *Speaker_GetConfigDescriptor(uint16_t);
uint8_t *Speaker_GetStringDescriptor(uint16_t);
uint8_t *Mute_Command(uint16_t Length);

/* Exported define -----------------------------------------------------------*/
#define Speaker_GetConfiguration          NOP_Process
//#define Speaker_SetConfiguration          NOP_Process
#define Speaker_GetInterface              NOP_Process
#define Speaker_SetInterface              NOP_Process
#define Speaker_GetStatus                 NOP_Process
#define Speaker_ClearFeature              NOP_Process
#define Speaker_SetEndPointFeature        NOP_Process
#define Speaker_SetDeviceFeature          NOP_Process
//#define Speaker_SetDeviceAddress          NOP_Process
#define GET_CUR                           0x81
#define SET_CUR                           0x01

#endif /* __usb_prop_H */
/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

