/*
 * Copyright 2017-2018 NXP
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
**     Project     : wdg_pal_interrupt_mpc5748g
**     Processor   : MPC5748G_176
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-10-30, 14:08, # CodeGen: 1
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

volatile int exit_code = 0;
/* User includes (#include below this line is not maintained by Processor Expert) */
#include <stdint.h>
#include <stdbool.h>

/* This example is setup to work by default with DEVKIT. To use it with other boards
   please comment the following line
*/
#define DEVKIT

#ifdef DEVKIT
    #define LED_GPIO    PTA             /* GPIO type */
    #define LED1        (1UL)           /* pin PA[0] - LED1 (DS10) on DEV-KIT */
    #define LED2        (1UL << 10U)    /* pin PA[10] - LED2 (DS4) on DEV-KIT */
#else
    #define LED_GPIO    PTG            /* GPIO type */
    #define LED1        (1UL << 2U)    /* pin PG[2] - LED1 (DS2) on Motherboard */
    #define LED2        (1UL << 3U)    /* pin PG[3] - LED2 (DS3) on Motherboard */
#endif

/*!
 * @brief ISR for WDG PAl timeout interrupt
 */
void wdgPAL_Handler(void)
{
    /* Variable that stores the number of WDG PAL Refresh */
    static uint8_t s_refreshNumber = 0;
    /* After 30 watchdog pal refreshes, the interrupt will no longer reset the wdg pal counter */
    if(s_refreshNumber < 30)
    {
        /* Clear SWT interrupt flag */
    	WDG_ClearIntFlag(&wdg_pal1_Instance);
        /* Reset WDG PAL counter */
        WDG_Refresh(&wdg_pal1_Instance);
        /* Toggle output value LED 1 */
        PINS_DRV_TogglePins(LED_GPIO, LED1);
        /* Increment refresh number */
        s_refreshNumber++;
    }
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
    IRQn_Type wdgPALIrqId[SWT_INSTANCE_COUNT] = SWT_IRQS;

    /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
    #ifdef PEX_RTOS_INIT
      PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
    #endif
    /*** End of Processor Expert internal initialization.                    ***/

    /* Initialize clock module */
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    /* Initialize LED configuration */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

    /* Stop if the previous reset was caused by the WDG PAL(SWT0) */
    if(((MC_RGM->DES & MC_RGM_DES_F_SWT0_RES_MASK) >> MC_RGM_DES_F_SWT0_RES_SHIFT) == 1U)
    {
        /* Clear reset flag */
        MC_RGM->DES |= MC_RGM_DES_F_SWT0_RES(1U);

        /* Turn on both LEDs */
        PINS_DRV_ClearPins(LED_GPIO, LED1);
        PINS_DRV_ClearPins(LED_GPIO, LED2);
    }
    else
    {
        /* Turn off both LEDs */
        PINS_DRV_SetPins(LED_GPIO, LED1);
        PINS_DRV_SetPins(LED_GPIO, LED2);

        /* Install the handler for WDG PAL interrupt */
        INT_SYS_InstallHandler(wdgPALIrqId[wdg_pal1_Instance.instIdx], wdgPAL_Handler, (isr_t *)NULL);

        /* Initialize WDG PAL */
        WDG_Init(&wdg_pal1_Instance, &wdg_pal1_Config0);
    }

    /* Forever loop */
    while(1)
    {
      __asm("e_nop");
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
