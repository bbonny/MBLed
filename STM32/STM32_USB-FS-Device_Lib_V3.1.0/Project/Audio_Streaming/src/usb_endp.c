/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : usb_endp.c
* Author             : MCD Application Team
* Version            : V3.1.0
* Date               : 10/30/2009
* Description        : Endpoint routines
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_istr.h"
#include "audio_codec_cs43l22.h"
#include "hw_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SPI2_DR_Address    0x4000380C

#ifdef AUDIO_FREQ_96K
 #define EDP1_BUFFER_SZE   384
#elif  defined (AUDIO_FREQ_48K)
 #define EDP1_BUFFER_SZE   192
#elif defined (AUDIO_FREQ_44K)
 #define EDP1_BUFFER_SZE   176
#elif defined (AUDIO_FREQ_32K)
 #define EDP1_BUFFER_SZE   128
#elif defined (AUDIO_FREQ_22K)
 #define EDP1_BUFFER_SZE   88
#elif defined (AUDIO_FREQ_16K)
 #define EDP1_BUFFER_SZE   64
#elif defined (AUDIO_FREQ_11K)
 #define EDP1_BUFFER_SZE   44
#elif defined (AUDIO_FREQ_8K)
 #define EDP1_BUFFER_SZE   32
#endif /* AUDIO_FREQ_96K */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef USE_STM3210C_EVAL
static  USB_OTG_EP *ep;
#endif /* USE_STM3210C_EVAL */
/* Extern variables ----------------------------------------------------------*/
extern uint8_t IsocBuff [];
extern __IO uint32_t IsocBufferIdx;

/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : EP1_OUT_Callback
* Description    : Endpoint 1 out callback routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP1_OUT_Callback(void)
{
  DMA_InitTypeDef DMA_InitStructure;

#ifdef USE_STM3210C_EVAL
  /* Get the Endpoint descriptor pointer */
  ep = OTGD_FS_PCD_GetOutEP(ENDP1 & 0x7F);

  /* Enable the Endppoint transfer (since the the Read Endpoint function is 
     not called) */
  OTGD_FS_EPStartXfer(ep);
  
  /* Toggle the data PID */
  if (ep->even_odd_frame != 0)
  {
    ep->even_odd_frame = 0;
  }
  else
  {
    ep->even_odd_frame = 1;    
  }
  
  /* If Half of global buffer has been filled and DMA is still not enabled, 
     Re-configure the DMA and unMute the codec */
#if (NUM_SUB_BUFFERS == 2)
  if (((DMA1_Channel5->CCR & 0x1) == 0) && (IsocBufferIdx == 1))
#else
  if (((DMA1_Channel5->CCR & 0x1) == 0) && (IsocBufferIdx == ((NUM_SUB_BUFFERS / 2) - 1)))
#endif /* NUM_SUB_BUFFERS */    
  {       
    /* Disable the mute mode */
    CODEC_Mute(MUTE_OFF);    
    
    /* Disable the I2S before starting configuration to avoid sporious transfer */
    SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_TXE, DISABLE);
    I2S_Cmd(SPI2, DISABLE);
    
    /* Initialize the DMA1 Channel5 to its default values */
    DMA_DeInit(DMA1_Channel5);
    
    /* Configure the DMA parameters */
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)((uint16_t*)IsocBuff);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPI2_DR_Address;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = (ISOC_BUFFER_SZE * NUM_SUB_BUFFERS) / 2; /* Divided by 2 because USB data are 8 bit and I2S buffer is 16 bit */
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;        
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);
  }  
#endif /* USE_STM3210C_EVAL */
}


/*******************************************************************************
* Function Name  : INTR_RXSTSQLVL_ISODU_Callback
* Description    : Rx FIFO Status Queue Level, Data Update interrupt callback routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void INTR_RXSTSQLVL_ISODU_Callback(void)
{
  /* Check if the USB transfe has reached the half of the global buffer */
  if(IsocBufferIdx == (NUM_SUB_BUFFERS / 2))
  {
    /* Check if the DMA is already enabled or not */
    if ((DMA1_Channel5->CCR & 0x1) == 0x0)
    {   
      /* Re-Initialize the number of data to be transferred */
      DMA1_Channel5->CNDTR = (uint16_t)((ISOC_BUFFER_SZE * NUM_SUB_BUFFERS) / 2);
      
      /* Enable DMA1 Channel5 */
      DMA_Cmd(DMA1_Channel5, ENABLE);  
      
      /* Enable I2S DMA request for TX */
      SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE); 
      /* Enable the I2S cell */
      I2S_Cmd(SPI2, ENABLE);  
    }

    /* Wait till the TC flag is ON (direct register access to optimize time) */
    while (((DMA1->ISR & (uint32_t)0x00020000) == 0))
    {}
    
    /* Clear All DMA1 channel 5 flags */
    DMA_ClearFlag(DMA1_FLAG_GL5);
  } 
}
/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

