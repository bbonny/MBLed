/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : readme.txt
* Author             : MCD Application Team
* Version            : V3.1.0
* Date               : 10/30/2009
* Description        : Description of the USB Audio_Streaming Demo.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

Example description
===================
The USB-FS-Device Audio Streaming demo gives examples of how to use the STM32F10x5/7 
USB-OTG peripheral in Device mode to communicate with the PC host in the isochronous 
transfer mode.
It provides a demo of the correct method for configuring an isochronous
endpoint, receiving or transmitting data from/to the host.
This demo provides possibilities to configure high audio quality streaming and 
low CPU charge using:
 - The USB-OTG peripheral with its dedicated RAM. 
 - The audio-class I2S peripheral and the available dedicated PLLs.
 - The DMA for Audio transfer from USB Memory to the I2S peripheral.

NOTE:
 - LED2 and LED3 of the STM3210C-EVAL evaluation board are configured to toggle
   periodically in order to check each time that firmware is still running
   correctly.

More details about this Demo implementation is given in the User manual 
"UM0424 STM32F10xxx USB development kit", available for download from the ST
microcontrollers website: www.st.com/stm32


Directory contents
==================
 + \inc: contains the Demo firmware header files
 + \EWARMv5: contains pre-configured projects for EWARM5 toolchain
 + \RIDE: contains pre-configured projects for RIDE toolchain
 + \RVMDK: contains pre-configured projects for RVMDK toolchain          
 + \src: contains the Demo firmware source files

Hardware environment
====================
This example runs on STMicroelectronics STM3210C-EVAL evaluation board and can be
easily tailored to any other hardware.
Basically the demo provides the configuration with 25 MHz wich allows getting 
high accuracy on both I2S and USB clock for only some audio frequencies.
For higher audio quality (in combination with an acceptable accuracy on the USB 
clock) on all audio frequencies, a specific external clock source may be used instead 
of the usual 25MHz crystal. Thus a 14.7456MHz crystal can be used to obtain this 
quality.

For each Audio output frequency (96KHz, 48KHz, ... ,8KHz) the PLL multipliers and 
dividers values shall be correctly configured in order to have optimum accuracy 
on both I2S clock and USB clock.
This demo provides the optimum configuration for the following audio frequencies
(no need for modifying the PLL configuration for these audio frequencies):

 When using 25 MHz crystal the PLLs configuration is optimum for these frequencies:
  - 16 KHz 
  -  8 KHz
 When using 14.7456 MHz crystal the PLLs configuration is optimum for these frequencies:  
  - 96 KHz
  - 48 KHz
  - 32 KHz
  - 16 KHz
  -  8 KHz
 
To select the audio frequency, uncomment the relative define in usb_conf.h file. 
 
For other frequencies, PLL1MUL, PLL2MUL, PLL3MUL, PREDIV1 and PREDIV2 values have
to be re-ajusted to get the optimum value on the I2S and USB clocks.


NOTE:
 The audio frequencies leading to non integer number of data (44.1KHz, 22.05KHz, 
 11.025KHz...) will not allow an optimum audio quality since one data will be lost
 every two frame.

How to use it
=============
 + RVMDK
    - Open the AudioSpeaker.Uv2 project
    - In the build toolbar select the project config:
        - STM3210C-EVAL-25MHz: to configure the project for boards with 25MHz 
          external clock.
        - STM3210C-EVAL-14MHz: to configure the project for boards with 14.7456MHz 
          external clock.
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)    

 + EWARM5
    - Open the AudioSpeaker.eww workspace.
    - In the workspace toolbar select the project config:
        - STM3210C-EVAL-25MHz: to configure the project for boards with 25MHz 
          external clock.
        - STM3210C-EVAL-14MHz: to configure the project for boards with 14.7456MHz 
          external clock.
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 + RIDE
    - Open the AudioSpeaker.rprj project.
    - In the configuration toolbar(Project->properties) select the project config:
        - STM3210C-EVAL-25MHz: to configure the project for boards with 25MHz 
          external clock.
        - STM3210C-EVAL-14MHz: to configure the project for boards with 14.7456MHz 
          external clock.
    - Rebuild all files: Project->build project
    - Load project image: Debug->start(ctrl+D)
    - Run program: Debug->Run(ctrl+F9)  

NOTE:
 - Low-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 16 and 32 Kbytes.
 - Medium-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 32 and 128 Kbytes.
 - High-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes.
 - Connectivity-Line devices are STM32F105xx and STM32F107xx microcontrollers.

******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE******
