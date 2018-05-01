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
**     Project     : timing_pal_mpc5748g
**     Processor   : MPC5748G_176
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-03-15, 12:21, # CodeGen: 6
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
#include "timing_pal1.h"
#include "timing_pal2.h"

volatile int exit_code = 0;
/* User includes (#include below this line is not maintained by Processor
 * Expert) */

/* This example is setup to work by default with DEVKIT. To use it with other
 boards please comment the following line
*/
#define DEVKIT

#ifdef DEVKIT
#define PORT PTA
#define LED2 0  /* pin PA[0] - USER LED2 (DS10) on DEV-KIT */
#define LED3 10 /* pin PA[10] - USER LED3 (DS4) on DEV-KIT */
#else
#define PORT PTG
#define LED2 3 /* pin PG[3] - USER LED2 (DS3) on Motherboard */
#define LED3 4 /* pin PG[4] - USER LED3 (DS7) on Motherboard */
#endif

/* PIT channel used */
#define PIT_CHANNEL 0UL
/* STM channel used */
#define STM_CHANNEL 0UL

/* Period in nanosecond unit */
#define PERIOD_BY_NS 1000000000UL /* The period is 1 second */

/*!
 * @brief: The callback function of timing over PIT channel 0.
 * 	       The callback function toggles LED2
 */
void timing_pal1_channel_callBack0(void *userData) {
  (void)userData;
  /* Toggle LED2 */
  PINS_DRV_TogglePins(PORT, (1 << LED2));
}

/*!
 * @brief: The callback function of timing over STM channel 0.
 * 	       The callback function toggles LED3
 */
void timing_pal2_channel_callBack0(void *userData) {
  (void)userData;
  /* Toggle LED3 */
  PINS_DRV_TogglePins(PORT, (1 << LED3));
}

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void) {
  uint64_t pitResolution;
  uint64_t stmResolution;
/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
#ifdef PEX_RTOS_INIT
  PEX_RTOS_INIT(); /* Initialization of the selected RTOS. Macro is defined by
                      the RTOS component. */
#endif
  /*** End of Processor Expert internal initialization.                    ***/

  /* Initialize clock gate*/
  CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                 g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
  CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);
  /* Initialize and configure pins */
  PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
  /* Turn off LED2 */
  PINS_DRV_SetPins(PORT, (1 << LED2));
  /* Turn off LED3 */
  PINS_DRV_SetPins(PORT, (1 << LED3));

  /* Initialize TIMING over PIT */
  TIMING_Init(&timing_pal1_instance, &timing_pal1_InitConfig);
  /* Initialize TIMING over STM */
  TIMING_Init(&timing_pal2_instance, &timing_pal2_InitConfig);

  /* Get tick resolution in nanosecond unit for TIMING over PIT */
  TIMING_GetResolution(&timing_pal1_instance, TIMER_RESOLUTION_TYPE_NANOSECOND,
                       &pitResolution);
  /* Get tick resolution in nanosecond unit for TIMING over STM */
  TIMING_GetResolution(&timing_pal2_instance, TIMER_RESOLUTION_TYPE_NANOSECOND,
                       &stmResolution);

  /* Start PIT channel 0 counting with the period is 1 second,
 the period in tick = the period in nanosecond / PIT tick resolution in
 nanosecond */
  TIMING_StartChannel(&timing_pal1_instance, PIT_CHANNEL,
                      PERIOD_BY_NS / pitResolution);
  /* Start STM channel 0 counting with the period is 1 second,
     the period in tick = the period in nanosecond / STM tick resolution
     nanosecond*/
  TIMING_StartChannel(&timing_pal2_instance, STM_CHANNEL,
                      PERIOD_BY_NS / stmResolution);

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
