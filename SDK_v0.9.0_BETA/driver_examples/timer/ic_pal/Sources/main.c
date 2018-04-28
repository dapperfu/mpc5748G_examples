/*
 * Copyright 2018 NXP
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
**     Project     : ic_pal_mpc5748g
**     Processor   : MPC5748G_324
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-01-24, 12:21, # CodeGen: 6
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
#include "eMIOS_Mc.h"
#include "eMIOS_Pwm.h"
#include "clockMan1.h"
#include "pin_mux.h"
#include "ic_pal1.h"


volatile int exit_code = 0;
/* User includes (#include below this line is not maintained by Processor Expert) */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* Length of the message to be received from the console */
#define RX_MSG_LEN  256U

/***********************************
 * @brief: Wait for a number of cycles
 * @param cycles is number of cycles to be waited for
 ***********************************/
void delayCycles(volatile uint32_t cycles)
{
    while (cycles--)
    {
    }
}

/***********************************
 * @brief: The callback application
 * @param event: An event which is triggered from the input capture callback
 * @param userData: The input parameter
 ***********************************/
void ic_pal1_channel_callBack0(ic_event_t event, void * userData)
{
    static uint32_t index = 0;
    index++;
    userData = (void*) index;
    (void)userData;
    (void)event;
}

/***********************************
 * @brief:           Method used to print a string with LPUART
 * @param sourceStr: pointer to the string beginning
 ***********************************/
void print(const char * sourceStr)
{
    status_t retVal;
    retVal = LINFLEXD_UART_DRV_SendDataBlocking(INST_LINFLEXD_UART1, (uint8_t*)sourceStr, strlen(sourceStr), 10000U);
    DEV_ASSERT(retVal == STATUS_SUCCESS);
}

/* This example is setup to work by default with DEVKIT. */
/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
    /* Variables used to store PWM frequency,
     * input capture measurement value
     */
    uint32_t inputCaptureMeas = 0;
    uint32_t frequency;
    uint32_t freqPeriod;

    bool conversionComplete = false;
    /* Buffer for string processing */
    char txBuff[255];
    /* Buffer to store the received message */
    uint8_t rxBuff[RX_MSG_LEN + 1];
    uint32_t bytesRemaining;
    uint8_t i = 0;

    /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
    #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
    #endif
    /*** End of Processor Expert internal initialization.                    ***/

    /* Initialize clock gate*/
    CLOCK_SYS_Init(g_clockManConfigsArr,   CLOCK_MANAGER_CONFIG_CNT,
                   g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);
    /* Initialize and configure pins */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

    /* Initialize eMIOS instances, PWM and Input capture
     *  -   See eMIOS component for more details
     */
    EMIOS_DRV_InitGlobal(INST_EMIOS_MC,&eMIOS_Mc_InitConfig0);

    EMIOS_DRV_MC_InitCounterMode(INST_EMIOS_MC,EMIOS_CNT_BUSC_DRIVEN,&eMIOS_Mc_CntChnConfig0);

    EMIOS_DRV_PWM_InitMode(INST_EMIOS_MC, 11U, &eMIOS_Pwm_PWMChnConfig0);

    EMIOS_DRV_EnableGlobalEmios(INST_EMIOS_MC);

    /* Initialize LINFlexD module for UART usage */
    LINFLEXD_UART_DRV_Init(INST_LINFLEXD_UART1, &linflexd_uart1_State, &linflexd_uart1_InitConfig0);

    /* initialize the input capture mode */
    IC_Init(&ic_pal1_instance, &ic_pal1_InitConfig);

    /* Get the eMIOS frequency to calculate
     * the frequency of the measured signal.
     */
    CLOCK_SYS_GetFreq(eMIOS0_CLK, &frequency);

    print("This example will show you how to use IC's signal measurement feature.\
            \rTo achieve that we will generate a modifiable frequency PWM and read \r\nit with Input Capture\r\n");

    /* Infinite loop
     *  -   Wait for user input
     *  -   Measure and calculate the signal frequency
     *  -   Send original and measured freq via LPUART
     *  -   Modify the PWM frequency
     */
    while (1)
    {
        print("Press any key to initiate a new conversion...\r\n");
        /* Wait for user input */
        /* Because the terminal appends new line to user data,
         * receive and store data into a buffer until it is received
         */
        LINFLEXD_UART_DRV_ReceiveData(INST_LINFLEXD_UART1, &rxBuff[i], 1UL);
        /* Wait for transfer to be completed */
        while(LINFLEXD_UART_DRV_GetReceiveStatus(INST_LINFLEXD_UART1, &bytesRemaining) != STATUS_SUCCESS);
        conversionComplete = false;

        while(conversionComplete == false)
        {
            /* Get values */
            inputCaptureMeas = IC_GetMeasurement(&ic_pal1_instance, 4U);
            /* Calculate the signal frequency using recorded data*/
            inputCaptureMeas = frequency / (inputCaptureMeas);
            freqPeriod = frequency / (eMIOS_Mc_CntChnConfig0.period);

            /* Convert the integer to char array and send it*/
            sprintf(txBuff, "PWM frequency:\t%lu\tMeasured frequency:\t%lu\t[Hz]\r\n", \
                    freqPeriod, inputCaptureMeas);
            print(txBuff);

            /* Stop PWM */
            /* Disable channels in the input capture over EMIOS */
            EMIOS_DRV_DeInitChannel(INST_EMIOS_MC, 11U);
            EMIOS_DRV_DeInitChannel(INST_EMIOS_MC, EMIOS_CNT_BUSC_DRIVEN);

            /* Increase frequency */
            if (freqPeriod < 10000U)
            {
                freqPeriod += 100U;
            }
            else
            {
                freqPeriod = 8000U;
                conversionComplete = true;
            }

            eMIOS_Mc_CntChnConfig0.period = frequency / freqPeriod;
            /* Restart PWM with new settings */
            EMIOS_DRV_InitGlobal(INST_EMIOS_MC,&eMIOS_Mc_InitConfig0);

            EMIOS_DRV_MC_InitCounterMode(INST_EMIOS_MC,EMIOS_CNT_BUSC_DRIVEN,&eMIOS_Mc_CntChnConfig0);

            EMIOS_DRV_PWM_InitMode(INST_EMIOS_MC, 11U, &eMIOS_Pwm_PWMChnConfig0);

            EMIOS_DRV_EnableGlobalEmios(INST_EMIOS_MC);

            delayCycles(0x7FFFF);
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
