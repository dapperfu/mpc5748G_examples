/*
 * Copyright 2017 NXP
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
**     Project     : adc_swtrigger_mpc5748g
**     Processor   : MPC5748G_176
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
#include "adConv1.h"
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
  #define LED_PORT PTJ
  #define LED 4			/* pin PJ[4] - LED1 (DS9) on DEV-KIT */
  #define ADC_CHAN_NUM (9u)	/* ADC1_P[0] corresponding to PB[4] */
#else
  #define LED_PORT PTG
  #define LED 2        	/* pin PG[2] - LED1 (DS2) on Motherboard */
  #define ADC_CHAN_NUM (9u)	/* ADC1_P[0] corresponding to PB[4] */
#endif

uint16_t result;
#define ADC_THRESHOLD (2048u) /* should be around 1.6V for 12-bit resolution, right aligned */

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

  /* Initialize and configure pins */
  PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
  /* Initialize clocks */
  CLOCK_SYS_Init(g_clockManConfigsArr,   CLOCK_MANAGER_CONFIG_CNT,
		 g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
  CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

  /* Initialize ADC */
  ADC_DRV_Reset(INST_ADCONV1);
  ADC_DRV_DoCalibration(INST_ADCONV1);
  ADC_DRV_ConfigConverter(INST_ADCONV1,&adConv1_ConvCfg0);
  ADC_DRV_EnableChannel(INST_ADCONV1, ADC_CONV_CHAIN_NORMAL, ADC_CHAN_NUM);

  /* Start a loop to read converted values and blink an LED */
  while(1)
  {
    ADC_DRV_StartConversion(INST_ADCONV1, ADC_CONV_CHAIN_NORMAL);
    while(!(ADC_DRV_GetStatusFlags(INST_ADCONV1) & ADC_FLAG_NORMAL_ENDCHAIN));
    uint32_t len = ADC_DRV_GetConvResultsToArray(INST_ADCONV1, ADC_CONV_CHAIN_NORMAL, &result, 1u);
    DEV_ASSERT(len == 1u); /* only one value should be read from hardware registers */
    (void)len;
    ADC_DRV_ClearStatusFlags(INST_ADCONV1, ADC_FLAG_NORMAL_ENDCHAIN);
    if (result > ADC_THRESHOLD)
    {
    	PINS_DRV_ClearPins(LED_PORT, (1 << LED));
    }
    else
    {
    	PINS_DRV_SetPins(LED_PORT, (1 << LED));
    }
  }

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
