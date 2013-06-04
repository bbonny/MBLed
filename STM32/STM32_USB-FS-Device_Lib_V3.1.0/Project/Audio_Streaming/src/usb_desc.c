/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : usb_desc.c
* Author             : MCD Application Team
* Version            : V3.1.0
* Date               : 10/30/2009
* Description        : Descriptors for Audio Speaker Demo
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
#include "usb_desc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants----------------------------------------------------------*/
/* USB Standard Device Descriptor */
const uint8_t Speaker_DeviceDescriptor[] =
  {
    SPEAKER_SIZ_DEVICE_DESC,          /* bLength */
    USB_DEVICE_DESCRIPTOR_TYPE,           /* bDescriptorType */
    0x00,          /* 1.10 */             /* bcdUSB */
    0x02,
    0x00,                                 /* bDeviceClass */
    0x00,                                 /* bDeviceSubClass */
    0x00,                                 /* bDeviceProtocol */
    0x40,                                 /* bMaxPacketSize 40 */
    0x83,                                 /* idVendor */
    0x04,
    0x30,                                 /* idProduct  = 0x5730*/
    0x57,
    0x00,          /* 2.00 */             /* bcdDevice */
    0x02,
    1,                                 /* iManufacturer */
    2,                                 /* iProduct */
    3,                                 /* iSerialNumber */
    0x01                                  /* bNumConfigurations */
  };

/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
const uint8_t Speaker_ConfigDescriptor[] =
  {
    /* Configuration 1 */
    0x09,                                /* bLength */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,   /* bDescriptorType */
    0x6D,                               /* wTotalLength  110 bytes*/
    0x00,
    0x02,                                 /* bNumInterfaces */
    0x01,                                 /* bConfigurationValue */
    0x00,                                 /* iConfiguration */
    0xC0,                                 /* bmAttributes  BUS Powred*/
    0x32,                                 /* bMaxPower = 100 mA*/
    /* 09 byte*/

    /* USB Speaker Standard interface descriptor */
    SPEAKER_SIZ_INTERFACE_DESC_SIZE,  /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
    0x00,                                 /* bInterfaceNumber */
    0x00,                                 /* bAlternateSetting */
    0x00,                                 /* bNumEndpoints */
    USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass */
    AUDIO_SUBCLASS_AUDIOCONTROL,          /* bInterfaceSubClass */
    AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol */
    0x00,                                 /* iInterface */
    /* 09 byte*/

    /* USB Speaker Class-specific AC Interface Descriptor */
    SPEAKER_SIZ_INTERFACE_DESC_SIZE,   /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    AUDIO_CONTROL_HEADER,                 /* bDescriptorSubtype */
    0x00,          /* 1.00 */             /* bcdADC */
    0x01,
    0x27,                                /* wTotalLength = 39*/
    0x00,
    0x01,                                 /* bInCollection */
    0x01,                                 /* baInterfaceNr */
    /* 09 byte*/

    /* USB Speaker Input Terminal Descriptor */
    AUDIO_INPUT_TERMINAL_DESC_SIZE,       /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    AUDIO_CONTROL_INPUT_TERMINAL,         /* bDescriptorSubtype */
    0x01,                                 /* bTerminalID */
    0x01,                                 /* wTerminalType AUDIO_TERMINAL_USB_STREAMING   0x0101 */
    0x01,
    0x00,                                 /* bAssocTerminal */
    0x01,                                 /* bNrChannels */
    0x00,                                 /* wChannelConfig 0x0000  Mono */
    0x00,
    0x00,                                 /* iChannelNames */
    0x00,                                 /* iTerminal */
    /* 12 byte*/

    /* USB Speaker Audio Feature Unit Descriptor */
    0x09,                                 /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    AUDIO_CONTROL_FEATURE_UNIT,           /* bDescriptorSubtype */
    0x02,                                 /* bUnitID */
    0x01,                                 /* bSourceID */
    0x01,                                 /* bControlSize */
    AUDIO_CONTROL_MUTE,                   /* bmaControls(0) */
    0x00,                                 /* bmaControls(1) */
    0x00,                                 /* iTerminal */
    /* 09 byte*/

    /*USB Speaker Output Terminal Descriptor */
    0x09,      /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    AUDIO_CONTROL_OUTPUT_TERMINAL,        /* bDescriptorSubtype */
    0x03,                                 /* bTerminalID */
    0x01,                                 /* wTerminalType  0x0301*/
    0x03,
    0x00,                                 /* bAssocTerminal */
    0x02,                                 /* bSourceID */
    0x00,                                 /* iTerminal */
    /* 09 byte*/

    /* USB Speaker Standard AS Interface Descriptor - Audio Streaming Zero Bandwith */
    /* Interface 1, Alternate Setting 0                                             */
    SPEAKER_SIZ_INTERFACE_DESC_SIZE,  /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
    0x01,                                 /* bInterfaceNumber */
    0x00,                                 /* bAlternateSetting */
    0x00,                                 /* bNumEndpoints */
    USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass */
    AUDIO_SUBCLASS_AUDIOSTREAMING,        /* bInterfaceSubClass */
    AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol */
    0x00,                                 /* iInterface */
    /* 09 byte*/

    /* USB Speaker Standard AS Interface Descriptor - Audio Streaming Operational */
    /* Interface 1, Alternate Setting 1                                           */
    SPEAKER_SIZ_INTERFACE_DESC_SIZE,  /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
    0x01,                                 /* bInterfaceNumber */
    0x01,                                 /* bAlternateSetting */
    0x01,                                 /* bNumEndpoints */
    USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass */
    AUDIO_SUBCLASS_AUDIOSTREAMING,        /* bInterfaceSubClass */
    AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol */
    0x00,                                 /* iInterface */
    /* 09 byte*/

    /* USB Speaker Audio Streaming Interface Descriptor */
    AUDIO_STREAMING_INTERFACE_DESC_SIZE,  /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    AUDIO_STREAMING_GENERAL,              /* bDescriptorSubtype */
    0x01,                                 /* bTerminalLink */
    0x01,                                 /* bDelay */
    0x01,                                 /* wFormatTag AUDIO_FORMAT_PCM  0x0001*/
    0x00,
    /* 07 byte*/

    /* USB Speaker Audio Type I Format Interface Descriptor */
    0x0B,                                 /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    AUDIO_STREAMING_FORMAT_TYPE,          /* bDescriptorSubtype */
    AUDIO_FORMAT_TYPE_III,                /* bFormatType/// */ 
    0x02,                                 /* bNrChannels */
    0x02,                                 /* bSubFrameSize :  2 Bytes per frame (16bits) */
    16,                                   /* bBitResolution (16-bits per sample) */ 
    0x01,                                 /* bSamFreqType only one frequency supported */
#ifdef AUDIO_FREQ_96K
    0x00,                                 /* tSamFreq 96000 = 0x017700 */
    0x77,
    0x01,    
#elif defined (AUDIO_FREQ_48K)
    0x80,                                 /* tSamFreq 48000 = 0xBB80 */
    0xBB,
    0x00,    
#elif defined (AUDIO_FREQ_44K)
    0xE0,                                 /* tSamFreq 44000 = 0xABE0 */
    0xAB,
    0x00,        
#elif defined (AUDIO_FREQ_32K)
    0x00,                                 /* tSamFreq 32000 = 0x7D00 */
    0x7D,
    0x00,    
#elif defined (AUDIO_FREQ_22K)    
    0xF0,                                 /* tSamFreq 22000 = 0x55F0 */
    0x55,
    0x00,
#elif defined (AUDIO_FREQ_16K)
    0x80,                                 /* tSamFreq 16000 = 0x3E80 */
    0x3E,
    0x00,
#elif defined (AUDIO_FREQ_11K)
    0xF8,                                 /* tSamFreq 11000 = 0x2AF8 */
    0x2A,
    0x00,    
#elif defined (AUDIO_FREQ_8K)
    0x40,                                 /* tSamFreq 8000 = 0x1F40 */
    0x1F,
    0x00,    
#endif /* AUDIO_FREQ_96K */
    /* 11 byte*/

    /* Endpoint 1 - Standard Descriptor */
    AUDIO_STANDARD_ENDPOINT_DESC_SIZE,    /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,         /* bDescriptorType */
    0x01,                                 /* bEndpointAddress 1 out endpoint*/
    USB_ENDPOINT_TYPE_ISOCHRONOUS,        /* bmAttributes */
#ifdef AUDIO_FREQ_96K
    0x80,                                 /* wMaxPacketSize 384 bytes (96(Samples)*2(Stereo)*2(HalfWord)) */
    0x01, 
#elif defined (AUDIO_FREQ_48K)
    0xC0,                                 /* wMaxPacketSize 192 bytes (48(Samples)*2(Stereo)*2(HalfWord)) */
    0x00,
#elif defined (AUDIO_FREQ_44K)
    0xB0,                                 /* wMaxPacketSize 176 bytes (44(Samples)*2(Stereo)*2(HalfWord)) */
    0x00,
#elif defined (AUDIO_FREQ_32K)
    0x80,                                 /* wMaxPacketSize 128 bytes (32(Samples)*2(Stereo)*2(HalfWord))*/
    0x00,    
#elif defined (AUDIO_FREQ_22K)
    0x58,                                 /* wMaxPacketSize 88 bytes (22(Samples)*2(Stereo)*2(HalfWord))*/
    0x00,
#elif defined (AUDIO_FREQ_16K)
    0x40,                                 /* wMaxPacketSize 64 bytes (16(Samples)*2(Stereo)*2(HalfWord)) */
    0x00,
#elif defined (AUDIO_FREQ_11K)
    0x2C,                                 /* wMaxPacketSize 44 bytes (11(Samples)*2(Stereo)*2(HalfWord)) */
    0x00,
#elif defined (AUDIO_FREQ_8K)
    0x20,                                 /* wMaxPacketSize 32 bytes (8(Samples)*2(Stereo)*2(HalfWord)) */
    0x00,    
#endif /* AUDIO_FREQ_96K */
    0x01,                                 /* bInterval */
    0x00,                                 /* bRefresh */
    0x00,                                 /* bSynchAddress */
    /* 09 byte*/

    /* Endpoint - Audio Streaming Descriptor*/
    AUDIO_STREAMING_ENDPOINT_DESC_SIZE,   /* bLength */
    AUDIO_ENDPOINT_DESCRIPTOR_TYPE,       /* bDescriptorType */
    AUDIO_ENDPOINT_GENERAL,               /* bDescriptor */
    0x00,                                 /* bmAttributes */
    0x00,                                 /* bLockDelayUnits */
    0x00,                                 /* wLockDelay */
    0x00,
    /* 07 byte*/
  };

/* USB String Descriptor (optional) */
const uint8_t Speaker_StringLangID[SPEAKER_SIZ_STRING_LANGID] =
  {
    0x04,
    0x03,
    0x09,
    0x04
  }
  ; /* LangID = 0x0409: U.S. English */

const uint8_t Speaker_StringVendor[SPEAKER_SIZ_STRING_VENDOR] =
  {
    SPEAKER_SIZ_STRING_VENDOR, /* Size of manufaturer string */
    USB_STRING_DESCRIPTOR_TYPE,  /* bDescriptorType*/
    /* Manufacturer: "STMicroelectronics" */
    'S', 0, 'T', 0, 'M', 0, 'i', 0, 'c', 0, 'r', 0, 'o', 0, 'e', 0,
    'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0,
    'c', 0, 's', 0
  };

const uint8_t Speaker_StringProduct[SPEAKER_SIZ_STRING_PRODUCT] =
  {
    SPEAKER_SIZ_STRING_PRODUCT,  /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, ' ', 0,
    'S', 0, 'p', 0, 'e', 0, 'a', 0, 'k', 0, 'e', 0, 'r', 0
  };

uint8_t Speaker_StringSerial[SPEAKER_SIZ_STRING_SERIAL] =
  {
    SPEAKER_SIZ_STRING_SERIAL,  /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, '1', 0, '0', 0
  };
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
