/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : readme.txt
* Author             : MCD Application Team
* Version            : V3.1.0
* Date               : 10/30/2009
* Description        : Description of the USB JoyStickMouse Demo.
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
This Demo provides a description of how to use the USB-FS-Device on the STM32F10x 
devices.
The STM32F10x device is enumerated as an USB-FS-Device Joystick Mouse, that uses 
the native PC Host USB-FS-Device HID driver.
The Joystick mounted on the STM3210B-EVAL, STM3210C-EVAL and STM3210E-EVAL boards 
is used to emulate the Mouse directions.

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
This example runs on STMicroelectronics STM3210B-EVAL, STM3210C-EVAL and 
STM3210E-EVAL evaluation boards and can be easily tailored to any other hardware.
To select the STMicroelectronics evaluation board used to run the example, uncomment
the corresponding line in platform_config.h file.

    
How to use it
=============
 + RVMDK
    - Open the JoyStickMouse.Uv2 project
    - In the build toolbar select the project config:
        - STM3210B-EVAL: to configure the project for STM32 Medium-density devices
        - STM3210C-EVAL: to configure the project for STM32 Connectivity-Line devices
        - STM3210E-EVAL: to configure the project for STM32 High-density devices
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)
    Note: The intialization file "STM32DBG.ini" is used to allow debugging in stop 
    mode.     

 + EWARM5
    - Open the JoyStickMouse.eww workspace.
    - In the workspace toolbar select the project config:
        - STM3210B-EVAL: to configure the project for STM32 Medium-density devices
        - STM3210C-EVAL: to configure the project for STM32 Connectivity-Line devices
        - STM3210E-EVAL: to configure the project for STM32 High-density devices
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 + RIDE
    - Open the JoyStickMouse.rprj project.
    - In the configuration toolbar(Project->properties) select the project config:
        - STM3210B-EVAL: to configure the project for STM32 Medium-density devices
        - STM3210C-EVAL: to configure the project for STM32 Connectivity-Line devices
        - STM3210E-EVAL: to configure the project for STM32 High-density devices
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
