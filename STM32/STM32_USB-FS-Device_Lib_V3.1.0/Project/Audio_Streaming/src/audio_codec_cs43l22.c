/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : audiocodec_cs43l22.c
* Author             : MCD Application Team
* Version            : V3.1.0
* Date               : 10/30/2009
* Description        : This file includes the I2S Codec driver for CS43L22
*                       Audio Codec.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "audio_codec_cs43l22.h"
#include "stm3210c_eval_ioe.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Uncomment this line to enable verifying data sent to codec after each write opeation */
//#define VERIFY_WRITTENDATA

/* The 7 bits Codec adress mask */
#define CODEC_ADDRESS         0x94  /* b00100111 */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void CODEC_GPIO_Config(void);
static void I2C_Config(void);

/*******************************************************************************
* Function Name  : CODEC_Config
* Description    : Configure the Codec in Headphone mode.
* Input          :  - OutputDevice: OutputDeviceHEADPHONE or OutputDeviceSPEAKER
*                :  - I2S_Standard: I2S communication standard could be I2S_Standard_Phillips
*                :       I2S_Standard_MSB or I2S_Standard_LSB.
*                :  - I2S_MCLKOutput: could be I2S_MCLKOutput_
*                :  - Volume:
* Output         : None
* Return         : 0-> correct communication, else wrong communication
*******************************************************************************/
uint32_t CODEC_Config(uint16_t OutputDevice, uint16_t I2S_Standard, uint16_t I2S_MCLKOutput, uint8_t Volume)
{
   uint32_t Standard = 0, counter = 0; 
 
  /* Configure the IO Expander (Codec Reset pin) */
  IOE_Config();
  
  /* Configure the Codec related IOs */
  CODEC_GPIO_Config();
  
  /* Reset the Codec Registers */
  CODEC_Reset();
  
  /* Determine the I2S standard used */
  switch (I2S_Standard)
  {
    case I2S_Standard_Phillips:
      Standard = 0x04;
      break;
    case I2S_Standard_MSB:
      Standard = 0x00;
      break;
    default :
      Standard = 0x08;
      break;
  }

  /* Initialization phase --------------------------------------*/
  /* Keep Codec powered OFF */
  counter += CODEC_WriteRegister(0x02, 0x01);  

  switch (OutputDevice)
  {
    case OutputDevice_SPEAKER:
      counter += CODEC_WriteRegister(0x04, 0xFA); // SPK always ON & HP always OFF
      break;
      
    case OutputDevice_HEADPHONE:
      counter += CODEC_WriteRegister(0x04, 0xAF); // SPK always OFF & HP always ON 
      break;
      
    case OutputDevice_BOTH:
      counter += CODEC_WriteRegister(0x04, 0xAA); // SPK always ON & HP always ON
      break;
      
    case OutputDevice_AUTO:
      counter += CODEC_WriteRegister(0x04, 0x05);// Detect the HP or the SPK automatically 
      break;      
  }
  
  /* Clock configuration: Auto detection */  
  counter += CODEC_WriteRegister(0x05, 0x81); //Auto speed detection
  
  /* Config the Slave Mode and the audio Standard */  
   counter += CODEC_WriteRegister(0x06, Standard);
      
  /* Set the Master volume */
  CODEC_ControlVolume(Volume); 
  
  /* Power on the Codec */
  counter += CODEC_WriteRegister(0x02, 0x9E);

  /* Return the counter value */
  return counter;
}

/*******************************************************************************
* Function Name  : CODEC_ControlVolume
* Description    : Controls the audio volume.
* Input          :  - Volume: the volume level to be set.
* Output         : None
* Return         : 0-> correct communication, else wrong communication
*******************************************************************************/
uint32_t CODEC_ControlVolume(uint8_t Volume)
{
  uint32_t counter = 0;

  /* 
     Max  = 0xFF --> 0x18, 
     Mean = 0x7F --> 0x00 
     Min  = 0x00 --> 0x19 
  */
  
  if (Volume > 0xE6)
  {
    /* Set the Master volume */
    counter += CODEC_WriteRegister(0x20, Volume - 0xE7); 
    counter += CODEC_WriteRegister(0x21, Volume - 0xE7);     
  }
  else
  {
    /* Set the Master volume */
    counter += CODEC_WriteRegister(0x20, Volume + 0x19); 
    counter += CODEC_WriteRegister(0x21, Volume + 0x19); 
  }

  return counter;
}

/*******************************************************************************
* Function Name  : CODEC_Mute
* Description    : Enable or disable the MUTE mode by software
* Input          : - Command: could be MUTE_ON to mute sound or MUTE_OFF to restore volume
* Output         : None.
* Return         : None.
*******************************************************************************/
void CODEC_Mute(uint32_t Command)
{ 
  /* Set the Mute mode */
  if (Command == MUTE_ON)
  {
    CODEC_WriteRegister(0x0D, 0x63);
    CODEC_WriteRegister(0x0F, 0xF8);
  }
  else /* MUTE_OFF Disable the Mute */
  {
    CODEC_WriteRegister(0x0D, 0x60);
    CODEC_WriteRegister(0x0F, 0x08);
  }
}

/*******************************************************************************
* Function Name  : CODEC_PowerDown
* Description    : Power down the Audio Codec.
* Input          : - CodecPowerDown_Mode: could be CodecPowerDown_SW for softare
*                  power down (by register writing), CodecPowerDown_HW just shut
*                  down the codec physically.
* Output         : None
* Return         : None
*******************************************************************************/
void CODEC_PowerDown(uint32_t CodecPowerDown_Mode)
{
  if (CodecPowerDown_Mode == CodecPowerDown_SW)
  {
    /* Power down the DAC and the speaker (PMDAC and PMSPK bits)*/
    CODEC_WriteRegister(0x02, 0x9F);
  }
  else /* CodecPowerDown_HW */
  {
    /* Mute the output first */
    CODEC_WriteRegister(0x1A, 0x80);
    CODEC_WriteRegister(0x1B, 0x80);    
    
    /* Power down the DAC components */
    CODEC_WriteRegister(0x02, 0x9F);
    
    /* Wait at least 100µs */
    CODEC_Delay(0xFFF);
    
    /* Reset The pin */
    IOE_WriteIOPin(AUDIO_RESET_PIN, BitReset);
  }
}

/*******************************************************************************
* Function Name  : CODEC_Reset
* Description    : Reset the Audio Codec.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CODEC_Reset(void)
{
  /* Power Down the codec */
  IOE_WriteIOPin(AUDIO_RESET_PIN, BitReset);

  /* wait for a delay to insure registers erasing */
  CODEC_Delay(0x2FFF);
  
  /* Power on the codec */
  IOE_WriteIOPin(AUDIO_RESET_PIN, BitSet);  
}

/*******************************************************************************
* Function Name  : CODEC_WriteRegister
* Description    : Writes a value in a register of the audio Codec through I2C.
* Input          :  - RegisterAddr: The target register adress (between 00x and 0x24)
*                :  - RegisterValue: The target register value to be written
*                :  - Verify: 0-> Don't verify the written data, 1-> Verify the written data
* Output         : None
* Return         : - 0  -> Correct write operation
*                : - !0 -> Incorrect write operation
*******************************************************************************/
uint32_t CODEC_WriteRegister(uint32_t RegisterAddr, uint32_t RegisterValue)
{
  uint32_t read_verif = 0;

  /* Reset all I2C2 registers */
  I2C_SoftwareResetCmd(I2C1, ENABLE);
  I2C_SoftwareResetCmd(I2C1, DISABLE);

  /* Enable the I2C1 peripheral  */
  I2C_Cmd(I2C1, ENABLE);

  /* Configure the I2C peripheral */
  I2C_Config();

  /* Begin the config sequence */
  I2C_GenerateSTART(I2C1, ENABLE);

  /* Test on EV5 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {}
  /* Transmit the slave address and enable writing operation */
  I2C_Send7bitAddress(I2C1, CODEC_ADDRESS, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {}
  /* Transmit the first address for r/w operations */
  I2C_SendData(I2C1, RegisterAddr);

  /* Test on EV8 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {}
  /* Prepare the register value to be sent */
  I2C_SendData(I2C1, RegisterValue);

  /* Test on EV8 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {}
  /* End the configuration sequence */
  I2C_GenerateSTOP(I2C1, ENABLE);

  /* Verify (if needed) that the loaded data is correct  */
#ifdef VERIFY_WRITTENDATA
  /* Read the just written register*/
  read_verif = CODEC_ReadRegister(RegisterAddr);

  /* Load the register and verify its value  */
  if (read_verif != RegisterValue)
  {
    /* Control data wrongly tranfered */
    read_verif = 1;
  }
  else
  {
    /* Control data correctly transfered */
    read_verif = 0;
  }
#endif

  /* Return the verifying value: 0 (Passed) or 1 (Failed) */
  return read_verif;
}

/*******************************************************************************
* Function Name  : CODEC_ReadRegister
* Description    : Reads a register of the audio Codec through I2C.
* Input          :  - RegisterAddr: The target register adress (between 00x and 0x24)
* Output         : None
* Return         : The value of the read register
*******************************************************************************/
uint32_t CODEC_ReadRegister(uint32_t RegisterAddr)
{
  uint32_t tmp = 0;

  /* Disable the I2C1 peripheral  */
  I2C_Cmd(I2C1, DISABLE);

  /* Reset all I2C2 registers */
  I2C_SoftwareResetCmd(I2C1, ENABLE);
  I2C_SoftwareResetCmd(I2C1, DISABLE);

  /* Configure the I2C peripheral */
  I2C_Config();

  /* Enable the I2C peripheral */
  I2C_GenerateSTART(I2C1, ENABLE);

  /* Test on EV5 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {}
  /* Disable Acknowledgement */
  I2C_AcknowledgeConfig(I2C1, DISABLE);

  /* Transmit the slave address and enable writing operation */
  I2C_Send7bitAddress(I2C1, CODEC_ADDRESS, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {}
  /* Transmit the first address for r/w operations */
  I2C_SendData(I2C1, RegisterAddr);

  /* Test on EV8 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {}
  /* Regenerate a start condition */
  I2C_GenerateSTART(I2C1, ENABLE);

  /* Test on EV5 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {}
  /* Transmit the slave address and enable writing operation */
  I2C_Send7bitAddress(I2C1, CODEC_ADDRESS, I2C_Direction_Receiver);

  /* Test on EV6 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
  {}
  /* Test on EV7 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
  {}
  /* End the configuration sequence */
  I2C_GenerateSTOP(I2C1, ENABLE);

  /* Load the register value */
  tmp = I2C_ReceiveData(I2C1);

  /* Disable Acknowledgement */
  I2C_AcknowledgeConfig(I2C1, ENABLE);

  /* Return the read value */
  return tmp;
}

/*******************************************************************************
* Function Name  : GPIO_Config
* Description    : Initializes the GPIO pins used by the codec application.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void CODEC_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIOB and AFIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

  /* I2C1 SCL PB6 and SDA PB7 pins configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Enable the I2C1 APB1 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
}

/*******************************************************************************
* Function Name  : I2C_Config
* Description    : Configure the I2C1 Peripheral.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void I2C_Config(void)
{
  I2C_InitTypeDef I2C_InitStructure;

  /* I2C1 configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x33;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = 100000;
  I2C_Init(I2C1, &I2C_InitStructure);
}

/*******************************************************************************
* Function Name  : CODEC_Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length
* Output         : None
* Return         : None
*******************************************************************************/
void CODEC_Delay( __IO uint32_t nCount)
{
  for (; nCount != 0; nCount--);
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
