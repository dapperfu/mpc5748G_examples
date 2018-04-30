/*
 * Copyright (c) 2015 - 2016 , Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */
/* ###################################################################
**     Filename    : main.c
**     Project     : crc_checksum_mpc5748g
**     Processor   : MPC5748G_324
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-11-02, 16:09, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.00
** @brief
**         Main module.
**         This module contains user's application code.
*/
/*!
**  @addtogroup main_module main module documentation
**  @{
*/
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "clockMan1.h"
#include "crc1.h"
#if CPU_INIT_CONFIG
  #include "Init_Config.h"
#endif

volatile int exit_code = 0;
extern const uint8_t CRC_data[];
#define CRC_DATA_SIZE (548U)
/* Define result CRC calculator with CCITT 16 bits standard */
#define RESULT_CRC_16_CCITT    (0xC78CU)

/* User includes (#include below this line is not maintained by Processor Expert) */
#include <stdint.h>
#include <stdbool.h>

/* This example is setup to work by default with DEVKIT. To use it with other boards
   please comment the following line
*/
#define DEVKIT

#ifdef DEVKIT
    #define ON           0U     /* LED ON */
    #define OFF          1U     /* LED OFF */
    #define LED1         4U     /* pin PJ[4] - LED1 (DS9) on DEV-KIT */
    #define LED1_GPIO    PTJ    /* LED1 GPIO type */
    #define LED2         0U     /* pin PA[0] - LED2 (DS10) on DEV-KIT */
    #define LED2_GPIO    PTA    /* LED2 GPIO type */
    #define SW           3U     /* pin PA[3] - SW (SW1_PA3) on DEV_KIT */
    #define SW_GPIO      PTA    /* SW GPIO type */

#else
    #define ON           0U     /* LED ON */
    #define OFF          1U     /* LED OFF */
    #define LED1         2U     /* pin PG[2] - LED1 (DS2) on Motherboard */
    #define LED1_GPIO    PTG    /* LED1 GPIO type */
    #define LED2         3U     /* pin PG[3] - LED2 (DS3) on Motherboard */
    #define LED2_GPIO    PTG    /* LED2 GPIO type */
    #define SW           2U     /* pin PA[2] - SW (PB_SW2) on Motherboard */
    #define SW_GPIO      PTA    /* SW GPIO type */
#endif

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - __start (startup asm routine)
 * - __init_hardware()
 * - main()
 *   - PE_low_level_init()
 *     - Common_Init()
 *     - Peripherals_Init()
*/
int main(void)
{
  /* Write your local variable definition here */
    uint32_t dataSize = CRC_DATA_SIZE;
    volatile uint32_t result;
    const uint8_t *data = CRC_data;

    /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
#ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT(); /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
#endif
    /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
    /* Initializes clocks for CRC instances used */
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0, CLOCK_MANAGER_POLICY_AGREEMENT);

    /* Initialize pins
         *  -   Setup output pins for LEDs
         *  -   See PinSettings component for more info
    */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
    /* Turn off both LEDs */
    PINS_DRV_WritePin(LED1_GPIO, LED1, OFF);
    PINS_DRV_WritePin(LED2_GPIO, LED2, OFF);

    /* CCITT 16 bits standard */
    /*
        .polynomial = CRC_BITS_16_CCITT,
        .writeTranspose = CRC_TRANSPOSE_BITS,
        .complementChecksum = false,
        .readTranspose = false,
        .seed = 0xFFFFU
     */
    /* result = 0xC78C */
    /* Initialize CRC module */
    CRC_DRV_Init(INST_CRC1, &crc1_InitConfig0);

    /* Turn on LED1 */
    PINS_DRV_WritePin(LED1_GPIO, LED1, ON);

    /* Infinite loop */
    for(;;) {
        /* Check button */
        if (PINS_DRV_ReadPins(SW_GPIO) & (1U << SW))
        {
            /* Calculate CRC value for CRC_data with configuration of 16 bit wide result */
            CRC_DRV_WriteData(INST_CRC1, data, dataSize);
            result = CRC_DRV_GetCrcResult(INST_CRC1);
            /* If result of CRC calculator is correct */
            if (result == RESULT_CRC_16_CCITT)
            {
                /* Turn off LED1 */
                PINS_DRV_WritePin(LED1_GPIO, LED1, OFF);
                /* Turn on LED2 */
                PINS_DRV_WritePin(LED2_GPIO, LED2, ON);
            }
            break;
        }
    }

    /* Cast the result to void in order to avoid triggering unused variable warning */
    (void)result;

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.1 [05.21]
**     for the NXP C55 series of microcontrollers.
**
** ###################################################################
*/

