/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : usb_conf.h
* Author             : MCD Application Team
* Version            : V3.1.0
* Date               : 10/30/2009
* Description        : Mass Storage Demo configuration  header
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_CONF_H
#define __USB_CONF_H

/*------------------------------------------------------------------------------
                    Select the audio frequency 
   WARNING:
   Before selecting the audio frequency, make sure the configuration of the PLLs
   allows acceptable accuracy for both the I2S clock and the USB clock !
   PLLs are configured to have optimum accuracy on the following audio frequencies.
------------------------------------------------------------------------------*/

/* PLLs in this demo are well configured for all these frequencies */
#ifdef EXTERNAL_CRYSTAL_14_7456MHz
 #define AUDIO_FREQ_96K
 //#define AUDIO_FREQ_48K
 //#define AUDIO_FREQ_32K 
 //#define AUDIO_FREQ_16K
 //#define AUDIO_FREQ_8K
#endif /* EXTERNAL_CRYSTAL_14_7456MHz */

/* PLLs in this demo are well configured for all these frequencies */
#ifdef EXTERNAL_CRYSTAL_25MHz             
 #define AUDIO_FREQ_16K
 //#define AUDIO_FREQ_8K
#endif /* EXTERNAL_CRYSTAL_25MHz */ 

/* These frequencies lead to non integer number of data per frame --> low quality */
//#define AUDIO_FREQ_44K
//#define AUDIO_FREQ_22K
//#define AUDIO_FREQ_11K

/*----------------------------------------------------------------------------*/

/* Size of the receive Buffer = NumberOfSamplesPerSecond x 2 x 2 (Stereo, 16bits) 
    Do not modify these lines ! */
#ifdef AUDIO_FREQ_96K
 #define ISOC_BUFFER_SZE  (uint32_t)(2*2*96)
#elif defined AUDIO_FREQ_48K
 #define ISOC_BUFFER_SZE  (uint32_t)(2*2*48)
#elif defined AUDIO_FREQ_44K
 #define ISOC_BUFFER_SZE  (uint32_t)(2*2*44)
#elif defined AUDIO_FREQ_32K
 #define ISOC_BUFFER_SZE  (uint32_t)(2*2*32)
#elif defined AUDIO_FREQ_22K
 #define ISOC_BUFFER_SZE  (uint32_t)(2*2*22)
#elif defined AUDIO_FREQ_16K
 #define ISOC_BUFFER_SZE  (uint32_t)(2*2*16)
#elif defined AUDIO_FREQ_11K
 #define ISOC_BUFFER_SZE  (uint32_t)(2*2*11)
#elif defined AUDIO_FREQ_8K
 #define ISOC_BUFFER_SZE  (uint32_t)(2*2*8)
#endif

/*------------------------------------------------------------------------------
               Select the number of sub-buffers (even number)
       The choice of this value depends on the accuracy of the I2S clock
                  and the availabitlity of the RAM space.
------------------------------------------------------------------------------*/
//#define NUM_SUB_BUFFERS             4             /* Min value */
//#define NUM_SUB_BUFFERS             8
//#define NUM_SUB_BUFFERS             16
//#define NUM_SUB_BUFFERS             32
//#define NUM_SUB_BUFFERS             64
#define NUM_SUB_BUFFERS             100
//#define NUM_SUB_BUFFERS             128
//#define NUM_SUB_BUFFERS             200
//#define NUM_SUB_BUFFERS             192
//#define NUM_SUB_BUFFERS             256
//#define NUM_SUB_BUFFERS             300           
/*----------------------------------------------------------------------------*/

/* Define for the audio volume output */
#define AUDIO_STREAMING_VOLUME        0xB8

/*-------------------------------------------------------------*/
/* EP_NUM */
/* defines how many endpoints are used by the device */
/*-------------------------------------------------------------*/

#define EP_NUM              (2)

#ifndef STM32F10X_CL
/*-------------------------------------------------------------*/
/* --------------   Buffer Description Table  -----------------*/
/*-------------------------------------------------------------*/
/* buffer table base address */
/* buffer table base address */
#define BTABLE_ADDRESS      (0x00)

/* EP0  */
/* rx/tx buffer base address */
#define ENDP0_RXADDR        (0x10)
#define ENDP0_TXADDR        (0x50)

/* EP1  */
/* buffer base address */
#define ENDP1_BUF0Addr      (0x90)
#define ENDP1_BUF1Addr      (0xC0)

/*-------------------------------------------------------------*/
/* -------------------   ISTR events  -------------------------*/
/*-------------------------------------------------------------*/
/* IMR_MSK */
/* mask defining which events has to be handled */
/* by the device application software */
#define IMR_MSK (CNTR_CTRM  | CNTR_SOFM  | CNTR_RESETM )

/*#define CTR_CALLBACK*/
/*#define DOVR_CALLBACK*/
/*#define ERR_CALLBACK*/
/*#define WKUP_CALLBACK*/
/*#define SUSP_CALLBACK*/
/*#define RESET_CALLBACK*/
#define SOF_CALLBACK
/*#define ESOF_CALLBACK*/

#endif /* STM32F10X_CL */

#ifdef STM32F10X_CL

/* OTGD-FS-DEVICE IP interrupts Enable definitions */
/* Uncomment the define to enable the selected interrupt */
//#define INTR_MODEMISMATCH
#define INTR_SOFINTR
#define INTR_RXSTSQLVL           /* Mandatory */
//#define INTR_NPTXFEMPTY
//#define INTR_GINNAKEFF
//#define INTR_GOUTNAKEFF
//#define INTR_ERLYSUSPEND
#define INTR_USBSUSPEND          /* Mandatory */
#define INTR_USBRESET            /* Mandatory */
#define INTR_ENUMDONE            /* Mandatory */
//#define INTR_ISOOUTDROP
//#define INTR_EOPFRAME
//#define INTR_EPMISMATCH
#define INTR_INEPINTR            /* Mandatory */
#define INTR_OUTEPINTR           /* Mandatory */
//#define INTR_INCOMPLISOIN
//#define INTR_INCOMPLISOOUT
#define INTR_WKUPINTR            /* Mandatory */

/* OTGD-FS-DEVICE IP interrupts subroutines */
/* Comment the define to enable the selected interrupt subroutine and replace it
   by user code */
#define  INTR_MODEMISMATCH_Callback      NOP_Process
/* #define  INTR_SOFINTR_Callback           NOP_Process */
#define  INTR_RXSTSQLVL_Callback         NOP_Process
#define  INTR_NPTXFEMPTY_Callback        NOP_Process
#define  INTR_NPTXFEMPTY_Callback        NOP_Process
#define  INTR_GINNAKEFF_Callback         NOP_Process
#define  INTR_GOUTNAKEFF_Callback        NOP_Process
#define  INTR_ERLYSUSPEND_Callback       NOP_Process
#define  INTR_USBSUSPEND_Callback        NOP_Process
#define  INTR_USBRESET_Callback          NOP_Process
#define  INTR_ENUMDONE_Callback          NOP_Process
#define  INTR_ISOOUTDROP_Callback        NOP_Process
#define  INTR_EOPFRAME_Callback          NOP_Process
#define  INTR_EPMISMATCH_Callback        NOP_Process
#define  INTR_INEPINTR_Callback          NOP_Process
#define  INTR_OUTEPINTR_Callback         NOP_Process
#define  INTR_INCOMPLISOIN_Callback      NOP_Process
#define  INTR_INCOMPLISOOUT_Callback     NOP_Process
#define  INTR_WKUPINTR_Callback          NOP_Process

/* Isochronous data update */
/* #define  INTR_RXSTSQLVL_ISODU_Callback   NOP_Process */

#endif /* STM32F10X_CL */

/* CTR service routines */
/* associated to defined endpoints */
#define  EP1_IN_Callback   NOP_Process
#define  EP2_IN_Callback   NOP_Process
#define  EP3_IN_Callback   NOP_Process
#define  EP4_IN_Callback   NOP_Process
#define  EP5_IN_Callback   NOP_Process
#define  EP6_IN_Callback   NOP_Process
#define  EP7_IN_Callback   NOP_Process

/*#define  EP1_OUT_Callback   NOP_Process*/
#define  EP2_OUT_Callback   NOP_Process
#define  EP3_OUT_Callback   NOP_Process
#define  EP4_OUT_Callback   NOP_Process
#define  EP5_OUT_Callback   NOP_Process
#define  EP6_OUT_Callback   NOP_Process
#define  EP7_OUT_Callback   NOP_Process


#endif /* __USB_CONF_H */
/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
