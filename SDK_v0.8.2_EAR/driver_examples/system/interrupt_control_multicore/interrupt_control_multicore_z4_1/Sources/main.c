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
**     Project     : interrupt_control_multicore_Z4_1
**     Processor   : MPC5748G_324
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-09-11, 12:03, # CodeGen: 4
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
#include "pit1.h"
#include "pin_mux.h"

volatile int exit_code = 0;
uint32_t PIT0_ctr = 0;      /* Counter for # of PIT 0 ISRs */
/* User includes (#include below this line is not maintained by Processor Expert) */

#define LED1        (1<<10)   /* GPIO  10 - pin PA[10] */
#define LED3        (1<<13)   /* GPIO 125 - pin PH[13] */
#define LED5        (1<<5)    /* GPIO 117 - pin PH[5] */
#define LED7        (1<<0)    /* GPIO   0 - pin PA[0] */
#define LED1_PORT   PTA       /* base pointer */
#define LED3_PORT   PTH       /* base pointer */
#define LED5_PORT   PTH       /* base pointer */
#define LED7_PORT   PTA       /* base pointer */

/*
 * PIT's Channel 1 interrupt handler
 * It toggles the LED3 once a second and generates a software interrupt on core Z2 every 5 seconds
 */
void PIT_Ch1_IRQHandler(void)
{
    PINS_DRV_TogglePins(LED3_PORT, LED3);           /* LED toggle */
    PIT0_ctr++;                                     /* Increment ISR counter */
    if (PIT0_ctr == 5) {
        PIT0_ctr = 0;                               /* Clear counter */
        INT_SYS_SetSoftwareIRQRequest(SS1_IRQn);    /* Generate SW interrupt request that is sent to core 2 */
    }
    PIT_DRV_ClearStatusFlags(INST_PIT1, pit1_ChnConfig0.hwChannel);     /* Clear channel 0 interrupt flag */
}

  /*!
    \brief The main function for the project.
    \details The startup initialization sequence is the following:
   * - startup asm routine
   * - main()
  */
int main(void)
{
    /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
    #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
    #endif
    /*** End of Processor Expert internal initialization.                    ***/

    /* Initialize clock gate*/
    CLOCK_SYS_Init(g_clockManConfigsArr,   CLOCK_MANAGER_CONFIG_CNT,
                   g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    /* Disable channel 1 interrupt for all cores*/
    /* This is done because all interrupts are enabled on core Z4_0 by default*/
    INT_SYS_DisableIRQ_MC_All(PIT_Ch1_IRQn);

    /* Initialize channel 1 */
    PIT_DRV_InitChannel(INST_PIT1, &pit1_ChnConfig0);

    /* Start channel 1 counting */
    PIT_DRV_StartChannel(INST_PIT1, pit1_ChnConfig0.hwChannel);

    /* Disable sw interrupt 1 for all cores*/
    /* This is done because all interrupts are enabled on core Z4_0 by default*/
    INT_SYS_DisableIRQ_MC_All(SS1_IRQn);

    /* Enable SWT interrupt on core Z2 */
    uint8_t cores[1] = { 2 };
    INT_SYS_EnableIRQ_MC(SS1_IRQn, cores, 1);

    /* Set priority for software interrupt */
    INT_SYS_SetPriority(SS1_IRQn, 7);

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
