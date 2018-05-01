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
**     Project     : rtc_alarm_mpc5748g
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
#include "pin_mux.h"
#include "rtcTimer1.h"

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
#define ALARM 5 /* GPIO 117 - pin PH[5] - LED1 (DS8) on DEV-KIT */
#define ALARM_PORT PTH
#define BTN_PIN 12 /* GPIO 76 - pin PE[12] - USER SWITCH 2 (SW2) on DEV-KIT */
#define BTN_PORT PTE
#else
#define ALARM 2 /* GPIO 98 - pin PG[2] - LED1 (DS2) on Motherboard */
#define ALARM_PORT PTG
#define BTN_PIN                                                                \
  11 /* GPIO 91 - pin PF[11] - USER SWITCH 3 (SW7) on Motherboard */
#define BTN_PORT PTF
#endif

/* Alarm interrupt handler */
void alarmISR(void) {
  /* Toggle the alarm led */
  PINS_DRV_TogglePins(ALARM_PORT, (1 << ALARM));
}

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void) {
  /* Write your local variable definition here */
  uint32_t tempSeconds;
  rtc_timedate_t tempTime;
/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
#ifdef PEX_RTOS_INIT
  PEX_RTOS_INIT(); /* Initialization of the selected RTOS. Macro is defined by
                      the RTOS component. */
#endif
  /*** End of Processor Expert internal initialization.                    ***/

  /* Initialize and configure clocks
   *  -   see clock manager component for details
   */
  CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                 g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
  CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

  /* Initialize pins
   *  -   See PinSettings component for more info
   */
  PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
  /* Initialize RTC instance
   *  - See RTC configuration component for options
   */
  /* Call the init function */
  RTC_DRV_Init(RTCTIMER1, &rtcTimer1_State, &rtcTimer1_Config0);

  /* Set the time and date */
  RTC_DRV_SetTimeDate(RTCTIMER1, &rtcTimer1_StartTime0);

  /* Start RTC counter */
  RTC_DRV_StartCounter(RTCTIMER1);

  /* Configure an alarm every 2 second */
  RTC_DRV_ConfigureAlarm(RTCTIMER1, &rtcTimer1_AlarmConfig0);

  while (1) {
    /* Check if the button was pressed */
    while ((PINS_DRV_ReadPins(BTN_PORT) & (1 << BTN_PIN)) == 0)
      ;

    /* Clear interrupt flag */
    PINS_DRV_ClearPins(BTN_PORT, BTN_PIN);
    /* Get current time */
    RTC_DRV_GetTimeDate(RTCTIMER1, &tempTime);
    /* Add to current time 5 seconds */
    RTC_DRV_ConvertTimeDateToSeconds(&tempTime, &tempSeconds);
    tempSeconds += 5;
    RTC_DRV_ConvertSecondsToTimeDate(&tempSeconds, &tempTime);
    /* Set new alarm time and date */
    rtcTimer1_AlarmConfig0.alarmTime = tempTime;
    /* Configure the alarm */
    RTC_DRV_ConfigureAlarm(RTCTIMER1, &rtcTimer1_AlarmConfig0);
  }

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
