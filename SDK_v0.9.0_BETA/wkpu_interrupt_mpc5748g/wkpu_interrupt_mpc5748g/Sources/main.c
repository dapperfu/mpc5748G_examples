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
 **     Project     : wkpu_interrupt_mpc5748g
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
#include "wkpu1.h"
#include "stm1.h"
#include "pwrMan1.h"
#include "pin_mux.h"

volatile int exit_code = 0;
/* User includes (#include below this line is not maintained by Processor Expert) */
#include <stdint.h>
#include <stdbool.h>

/* This example is setup to work by default with DEVKIT (1) and other board (0) */
#define DEV_KIT 1

#if defined(DEV_KIT) && DEV_KIT
    #define LED_GPIO     PTJ            /* LED GPIO type */
    #define LED          (1UL << 4U)    /* pin PJ[4] - LED1 (DS9) on DEV-KIT */
    #define SW_GPIO      PTA            /* SW4 GPIO type */
    #define SW           (1UL << 3U)    /* pin PA[3] - SW4 on DEV_KIT */
#else
    #define LED_GPIO     PTG            /* LED1 GPIO type */
    #define LED          (1UL << 2U)    /* pin PG[2] - LED1 (DS2) on Motherboard */
    #define SW_GPIO      PTA            /* SW4 GPIO type */
    #define SW           (1UL << 2U)    /* pin PA[2] - SW4 on Motherboard */
#endif

/*!
 \brief The main function for the project.
 \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
 */
int main(void)
{
    /* Write your local variable definition here */
    volatile uint32_t i;

    /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
#ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT(); /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
#endif
    /*** End of Processor Expert internal initialization.                    ***/

    /* Initialize clocks */
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0, CLOCK_MANAGER_POLICY_AGREEMENT);

    /* Initialize LEDs and Button configuration */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

    /* Blinking led while not press button */
    while ((PINS_DRV_ReadPins(SW_GPIO) & SW) == 0U)
    {
        /* Insert a small delay to make the blinking visible */
        for (i = 0; i < 800000; i++)
        {
            __asm("e_nop");
        }

        /* Toggle output value LED */
        PINS_DRV_TogglePins(LED_GPIO, LED);
    }

    /* Initial wkpu */
    WKPU_DRV_InitInterrupt(INST_WKPU1, WKPU_CHANNEL_COUNT0, wkpu1_InterruptCfg0);
    /* Set Channel 1 as Normal mode */
    WKPU_DRV_SetInterruptNormalMode(INST_WKPU1, WKPU_HW_CHANNEL1);

    /* Configure wake-up address for z4a core */
    /* Address start of start up */
    MC_ME->CADDR1 = 0x1000000;

    /* Init STM0 1s */
    STM_DRV_Init(INST_STM1, &stm1_InitConfig0);
    STM_DRV_InitChannel(INST_STM1, &stm1_ChnConfig0);
    STM_DRV_StartTimer(INST_STM1);

    /* Enable FXOSC in STANDBY */
    MC_ME->STANDBY_MC |= MC_ME_STANDBY_MC_FXOSCON_MASK;

    /* Enter STANDBY */
    POWER_SYS_Init(&powerConfigsArr,1, &powerStaticCallbacksConfigsArr, 0);
    POWER_SYS_SetMode(0, POWER_MANAGER_POLICY_AGREEMENT);

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
