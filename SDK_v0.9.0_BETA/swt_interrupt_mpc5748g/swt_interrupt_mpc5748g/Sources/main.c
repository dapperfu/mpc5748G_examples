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
**     Project     : swt_interrupt_mpc5748g
**     Processor   : MPC5748G_176
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-10-31, 12:00, # CodeGen: 1
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
#include "pin_mux.h"
#include "swt1.h"

volatile int exit_code = 0;
/* User includes (#include below this line is not maintained by Processor
 * Expert) */
#include <stdbool.h>
#include <stdint.h>

/* This example is setup to work by default with DEVKIT. To use it with other
   boards please comment the following line
*/
#define DEVKIT

#ifdef DEVKIT
#define ON 0U        /* LED ON */
#define OFF 1U       /* LED OFF */
#define LED_GPIO PTJ /* GPIO type */
#define LED 4U       /* pin PJ[4] - LED1 (DS9) on DEV-KIT */
#else
#define ON 0U        /* LED ON */
#define OFF 1U       /* LED OFF */
#define LED_GPIO PTG /* GPIO type */
#define LED 2U       /* pin PG[2] - LED1 (DS2) on Motherboard */
#endif

/*!
 * @brief ISR for SWT timeout interrupt
 */
void SWT_ISR(void) {
  /* Clear SWT interrupt flag */
  SWT_DRV_ClearIntFlag(INST_SWT1);
  /* Service SWT 0 */
  SWT_DRV_Service(INST_SWT1);
  /* Toggle output value LED */
  PINS_DRV_TogglePins(LED_GPIO, (1U << LED));
}

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void) {
  /* Write your local variable definition here */
  IRQn_Type swtIrqId[SWT_INSTANCE_COUNT] = SWT_IRQS;

/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
#ifdef PEX_RTOS_INIT
  PEX_RTOS_INIT(); /* Initialization of the selected RTOS. Macro is defined by
                      the RTOS component. */
#endif
  /*** End of Processor Expert internal initialization.                    ***/

  /* Initialize clock module */
  CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                 g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
  CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

  /* Initialize LED configuration */
  PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

  /* LED off */
  PINS_DRV_WritePin(LED_GPIO, LED, OFF);

  /* Install the handler for SWT interrupt */
  INT_SYS_InstallHandler(swtIrqId[INST_SWT1], SWT_ISR, (isr_t *)NULL);

  /* Initialize Software Watchdog */
  SWT_DRV_Init(INST_SWT1, &swt1_Config0);

/*** Don't write any code pass this line, or it will be deleted during code
 * generation. ***/
/*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component.
 * DON'T MODIFY THIS CODE!!! ***/
#ifdef PEX_RTOS_START
  PEX_RTOS_START(); /* Startup of the selected RTOS. Macro is defined by the
                       RTOS component. */
#endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for (;;) {
    if (exit_code != 0) {
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
