/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
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
**     Project     : cmp_dac_mpc5748g
**     Processor   : MPC5748G_324
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-03-14, 14:08, # CodeGen: 1
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
#include "comparator1.h"
#include "pin_mux.h"

volatile int exit_code = 0;
/* User includes (#include below this line is not maintained by Processor Expert) */
#include <stdint.h>
#include <stdbool.h>

/* This example is setup to work by default with DEVKIT. To use it with other boards
   please comment the following line
*/
#define DEVKIT

#ifdef DEVKIT
	#define LED1 4	/* GPIO 148 - pin PJ[4] - LED1 (DS9) on DEV-KIT */
	#define LED1_PORT PTJ
	#define LED2 0	/* GPIO 0 - pin PA[0] - LED1 (DS10) on DEV-KIT */
	#define LED2_PORT PTA

#else
	#define LED1 2	/* GPIO 98 - pin PG[2] - LED1 (DS2) on Motherboard */
	#define LED1_PORT PTG
	#define LED2 3	/* GPIO 99 - pin PG[3] - LED1 (DS3) on Motherboard */
	#define LED2_PORT PTG
#endif

/*! brief Comparator Interrupt Service routine
 * It will read the output flags and based on the result will light the
 * required LED:
 *  -   DS9 if Vin is greater than DAC voltage
 *  -   DS10 if Vin is lower than DAC voltage
 */

void comparatorISR(void)
{
	/* Variable used to store comparator output flags */
	cmp_output_trigger_t cmpOutputFlags;
	/* Get output flags */
	CMP_DRV_GetOutputFlags(INST_COMPARATOR1, &cmpOutputFlags);

	 switch(cmpOutputFlags)
	    {
	    /* Vin is greater than DAC voltage */
	    case CMP_FALLING_EDGE:
	        /* Light the RED led */
	        PINS_DRV_ClearPins(LED1_PORT, (1 << LED1));
	        PINS_DRV_SetPins(LED2_PORT, (1 << LED2));
	        break;
	    /* Vin is lower than DAC voltage */
	    case CMP_RISING_EDGE:
	        PINS_DRV_ClearPins(LED2_PORT, (1 << LED2));
	        PINS_DRV_SetPins(LED1_PORT, (1 << LED1));
	        break;
	    default:
	        /* Light both LEDs */
	        PINS_DRV_ClearPins(LED1_PORT, 1 << LED1);
	        PINS_DRV_ClearPins(LED2_PORT, 1 << LED2);
	        break;
	    }

	CMP_DRV_ClearOutputFlags(INST_COMPARATOR1);
}

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of Processor Expert internal initialization.                    ***/

  /* Initialize and configure clocks
  *  -   see clock manager component for details
  */
  CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
			g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
  CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

  /* Initialize pins
  *  -   Setup input pins for Comparator
  *  -   Setup output pins for LED
  *  -   See PinSettings component for more info
  */
  PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
  PINS_DRV_SetPins(LED1_PORT, 1 << LED1);
  PINS_DRV_SetPins(LED2_PORT, 1 << LED2);
  /* Initialize Comparator
  *  -   Positive source from AIN0
  *  -   Negative source from internal DAC with half the reference voltage
  *  5V/2 = 2.5V(For EVB). For other boards please contact the boards reference
  *  manual.
  *  -   Output interrupt enabled for both edges
  */
  CMP_DRV_Init(INST_COMPARATOR1, &cmp_general_config);

  /* Install Comparator interrupt handler */
  INT_SYS_InstallHandler(CMP0_IRQn, &comparatorISR, (isr_t *)0);
  /* Enable Comparator interrupt */
  INT_SYS_EnableIRQ(CMP0_IRQn);

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
