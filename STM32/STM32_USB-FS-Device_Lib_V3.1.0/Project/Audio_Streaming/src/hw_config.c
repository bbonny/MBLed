/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : hw_config.c
* Author             : MCD Application Team
* Version            : V3.1.0
* Date               : 10/30/2009
* Description        : Hardware Configuration & Setup
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "usb_lib.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "hw_config.h"
#include "platform_config.h"
#include "usb_pwr.h"
#include "audio_codec_cs43l22.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;

/* Private function prototypes -----------------------------------------------*/
static void I2S_Config(void);
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_System(void)
{
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/   
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
 
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

#ifdef STM32F10X_CL
    /* Configure PLLs *********************************************************/
  #ifdef EXTERNAL_CRYSTAL_25MHz  
    /* PLL2 configuration: PLL2CLK = (HSE / 9) * 13 = 36.111 MHz */
    RCC_PREDIV2Config(RCC_PREDIV2_Div9);
    RCC_PLL2Config(RCC_PLL2Mul_13);

    /* Enable PLL2 */
    RCC_PLL2Cmd(ENABLE);

    /* Wait till PLL2 is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET)
    {}    
    
    /* PPL3 configuration: PLL3CLK = (HSE / 9) * 14 = PLL3_VCO = 77.777 MHz */
    RCC_PLL3Config(RCC_PLL3Mul_14);

    /* Enable PLL3 */
    RCC_PLL3Cmd(ENABLE);    

    /* Wait till PLL3 is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLL3RDY) == RESET)
    {}    

    /* PLL configuration: PLLCLK = (PLL2 / 4) * 8 = 72.222 MHz */ 
    RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div4);
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_8);    

  #elif defined (EXTERNAL_CRYSTAL_14_7456MHz)
    /* PLL2 configuration: PLL2CLK = (HSE / 4) * 13 = 47.932 MHz */
    RCC_PREDIV2Config(RCC_PREDIV2_Div4);
    RCC_PLL2Config(RCC_PLL2Mul_13);

    /* Enable PLL2 */
    RCC_PLL2Cmd(ENABLE);

    /* Wait till PLL2 is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET)
    {} 

    /* PPL3 configuration: PLL3CLK = (HSE / 4) * 20 = PLL3_VCO = 147.456 MHz */    
    RCC_PLL3Config(RCC_PLL3Mul_20);
    
    /* Enable PLL3 */
    RCC_PLL3Cmd(ENABLE);    
    
    /* Wait till PLL3 is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLL3RDY) == RESET)
    {}
    
    /* PPL1 configuration: PLL1CLK = (PLL2 / 4) * 6 = 71.8848 MHz */ 
    RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div4);
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_6);    
    
  #endif /* EXTERNAL_CRYSTAL_25MHz */
    
    /* Configure I2S clock source: PLL3 VCO clock */
    RCC_I2S2CLKConfig(RCC_I2S2CLKSource_PLL3_VCO);
    
#else
    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
#endif /* STM32F10X_CL */

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock configuration.
       User can add here some code to deal with this error */    

    /* Go to infinite loop */
    while (1)
    {
    }
  }
}

/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz)
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_USBClock(void)
{
#ifdef STM32F10X_CL
  /* Select USBCLK source */
  RCC_OTGFSCLKConfig(RCC_OTGFSCLKSource_PLLVCO_Div3);

  /* Enable the USB clock */ 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_OTG_FS, ENABLE) ;
#else
  /* Select USBCLK source */
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
  
  /* Enable the USB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
#endif /* STM32F10X_CL */

}

/*******************************************************************************
* Function Name  : Enter_LowPowerMode
* Description    : Power-off system clocks and power while entering suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode
* Description    : Restores system clocks and power while exiting suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else
  {
    bDeviceState = ATTACHED;
  }
}

/*******************************************************************************
* Function Name  : USB_Interrupts_Config
* Description    : Configures the USB interrupts
* Input          : None.
* Return         : None.
*******************************************************************************/
void USB_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* 2 bit for pre-emption priority, 2 bits for subpriority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
  
#ifdef STM32F10X_CL
  /* Enable the USB Interrupts */
  NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
#endif /* STM32F10X_CL */

  /* Configure the audio application interrupts */
  Audio_Interrupts_Config();
}

/*******************************************************************************
* Function Name  : Audio_Interrupts_Config
* Description    : Configures the Audio related interrupts
* Input          : None.
* Return         : None.
*******************************************************************************/
void Audio_Interrupts_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

#ifdef USE_STM3210C_EVAL
  /* SPI2 IRQ Channel configuration */
  NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif /* USE_STM3210C_EVAL */
}

/*******************************************************************************
* Function Name  : USB_Cable_Config
* Description    : Software Connection/Disconnection of USB Cable
* Input          : None.
* Return         : Status
*******************************************************************************/
void USB_Cable_Config (FunctionalState NewState)
{
#ifdef USE_STM3210C_EVAL  
  if (NewState != DISABLE)
  {
    USB_DevConnect();
  }
  else
  {
    USB_DevDisconnect();
  }
#else /* USE_STM3210B_EVAL or USE_STM3210E_EVAL */
  if (NewState != DISABLE)
  {
    GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
  else
  {
    GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
#endif /* USE_STM3210C_EVAL */
}

/*******************************************************************************
* Function Name  : Speaker_Config
* Description    : Configure and enable the I2S and the codec
* Input          : None.
* Return         : None.
*******************************************************************************/
void Speaker_Config(void)
{
  /* Configure the I2S peripheral */
  I2S_Config();
  
  /* Configure the initializatin parameters */
  CODEC_Config(OutputDevice_AUTO, I2S_Standard_Phillips, I2S_MCLKOutput_Enable, AUDIO_STREAMING_VOLUME);

  /* Enable the DMA Clock for I2S transfers */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}


/*******************************************************************************
* Function Name  : I2S_Config
* Description    : Initializes the I2S peripheral.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void I2S_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  I2S_InitTypeDef I2S_InitStructure;  

  /* Enable GPIOB, GPIOC and AFIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                         RCC_APB2Periph_AFIO, ENABLE);

  /* I2S2 SD, CK and WS pins configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* I2S2 MCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  /* Enable I2S2 APB1 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

  /* Deinitialize SPI2 peripheral */
  SPI_I2S_DeInit(SPI2);
  /* I2S2 peripheral configuration */
  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;
  I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
#ifdef AUDIO_FREQ_96K
  I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_96k;
#elif defined (AUDIO_FREQ_48K)
  I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_48k;
#elif defined (AUDIO_FREQ_44K) 
  I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_44k;
#elif defined (AUDIO_FREQ_32K) 
  I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_32k;
#elif defined (AUDIO_FREQ_22K)  
  I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_22k;
#elif defined  (AUDIO_FREQ_16K) 
  I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_16k;
#elif defined  (AUDIO_FREQ_11K) 
  I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_11k;
#elif defined  (AUDIO_FREQ_8K) 
  I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_8k;  
#endif /* AUDIO_FREQ_96K */  
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
  I2S_Init(SPI2, &I2S_InitStructure);
  
  /* Enable the SPI2/I2S2 peripheral */
  I2S_Cmd(SPI2, ENABLE);  
  
}

/*******************************************************************************
* Function Name  : Get_SerialNum.
* Description    : Create the serial number string descriptor.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Get_SerialNum(void)
{
  uint32_t Device_Serial0, Device_Serial1, Device_Serial2;

  Device_Serial0 = *(uint32_t*)(0x1FFFF7E8);
  Device_Serial1 = *(uint32_t*)(0x1FFFF7EC);
  Device_Serial2 = *(uint32_t*)(0x1FFFF7F0);

  if (Device_Serial0 != 0)
  {
    Speaker_StringSerial[2] = (uint8_t)(Device_Serial0 & 0x000000FF);
    Speaker_StringSerial[4] = (uint8_t)((Device_Serial0 & 0x0000FF00) >> 8);
    Speaker_StringSerial[6] = (uint8_t)((Device_Serial0 & 0x00FF0000) >> 16);
    Speaker_StringSerial[8] = (uint8_t)((Device_Serial0 & 0xFF000000) >> 24);

    Speaker_StringSerial[10] = (uint8_t)(Device_Serial1 & 0x000000FF);
    Speaker_StringSerial[12] = (uint8_t)((Device_Serial1 & 0x0000FF00) >> 8);
    Speaker_StringSerial[14] = (uint8_t)((Device_Serial1 & 0x00FF0000) >> 16);
    Speaker_StringSerial[16] = (uint8_t)((Device_Serial1 & 0xFF000000) >> 24);

    Speaker_StringSerial[18] = (uint8_t)(Device_Serial2 & 0x000000FF);
    Speaker_StringSerial[20] = (uint8_t)((Device_Serial2 & 0x0000FF00) >> 8);
    Speaker_StringSerial[22] = (uint8_t)((Device_Serial2 & 0x00FF0000) >> 16);
    Speaker_StringSerial[24] = (uint8_t)((Device_Serial2 & 0xFF000000) >> 24);
  }
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
