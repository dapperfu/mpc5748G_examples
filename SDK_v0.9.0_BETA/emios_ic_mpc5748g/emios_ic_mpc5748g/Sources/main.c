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
**     Project     : emios_ic_mpc5748g
**     Processor   : MPC5748G_324
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
#include "eMIOS_Mc.h"
#include "eMIOS2.h"
#include "clockMan1.h"
#include "pin_mux.h"
#include "eMIOS1.h"
#include "linflexd_uart1.h"

volatile int exit_code = 0;
/* User includes (#include below this line is not maintained by Processor Expert) */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define MESSAGE  "**************************************\r\n Press 'c' to print duty value capture "

/* This example is setup to work by default with DEVKIT.
*/

void eMIOS_InitAndRun(void);
void print(const char *sourceStr);
static void convertToStr(const uint32_t *srcValue, char* destStr, uint8_t radix);
void readOptionFromConsole(uint8_t *option);

static char buff1[5];

void EMIOS1_02_03_IRQHandler(void)
{
    uint32_t retValue1 = 0;
    /* Capture value in interrupt */
    EMIOS_DRV_IC_GetLastMeasurement(1U,3U, &retValue1);
    convertToStr(&retValue1, buff1, 10);
    /* Clear flag */
    EMIOS_DRV_ClrFlagState(1U,3U);
}

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
    status_t status;
    /* Variable used for storing input key from keyboard */
    uint8_t option;

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

    /* Initialize LINFlexD module for UART usage */
    status = LINFLEXD_UART_DRV_Init(INST_LINFLEXD_UART1, &linflexd_uart1_State, &linflexd_uart1_InitConfig0);
    DEV_ASSERT(status == STATUS_SUCCESS);

    /* eMIOS initialize and run */
    eMIOS_InitAndRun();

    print((const char *)MESSAGE);
    while(1)
    {
        /* Read key user input from keyboard */
        readOptionFromConsole(&option);
        if(option == 'c')
        {
            print((const char *)"\r\n**************************************\r\nThe value capture : ");
            print((const char *)buff1);
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

/*FUNCTION**********************************************************************
*
* Description   :  Initialize and run eMIOS module
* param         :  void
* return        :  None
* END**************************************************************************/
void eMIOS_InitAndRun(void)
{
    /* If you want initialize eMIOS Global 0 with eMIOS_Mc_InitConfig0 */
    EMIOS_DRV_InitGlobal(1U,&eMIOS_Mc_InitConfig0);

    /* eMIOS Counter mode Initialization for eMIOS_Mc_CntChnConfig0 */
    EMIOS_DRV_MC_InitCounterMode(1U, EMIOS_CNT_BUSA_DRIVEN,&eMIOS_Mc_CntChnConfig0);

    /* Create source signal by emios_pwm mode */
    EMIOS_DRV_PWM_InitMode(1U, 30U, &eMIOS1_PWMChnConfig0);

    /* eMIOS IC mode Initialization for eMIOS_Ic_ICChnConfig1 */
    EMIOS_DRV_IC_InitInputCaptureMode(1U, 3U, &eMIOS2_ICChnConfig0);
    EMIOS_DRV_ChannelEnableInterruptRequest(1U, 3U);

    /* Enable eMIOS Global 0 */
    EMIOS_DRV_EnableGlobalEmios(1U);

    INT_SYS_EnableIRQ(EMIOS1_02_03_IRQn);
}


/*FUNCTION**********************************************************************
*
* Description   : Convert a integer number to char array
*
* param srcValue:  pointer to the source float value
* param destStr:   pointer to the destination string
* param radix:     radix of the digits
* return:          None
* END**************************************************************************/
static void convertToStr(const uint32_t *srcValue, char* destStr, uint8_t radix)
{
    uint32_t temp;
    temp = *srcValue;
    uint8_t stringLen = 1;
    while ((uint32_t)(temp / radix) != 0)
    {
        temp = (uint32_t)(temp / radix);
        stringLen++;
    }
    temp = *srcValue;
    while(stringLen != 0)
    {
        *(destStr + (stringLen - 1)) = (uint32_t)(temp % radix) + '0';
        temp = (uint32_t)(temp / radix);
        stringLen--;
    }
}


/*FUNCTION**********************************************************************
*
* Description   : Send a string to user
* param srcValue:  pointer to the array of characters that you wish to send.
* return:          None
* END**************************************************************************/
void print(const char *sourceStr)
{
    uint32_t byteRemaining = 0U;

    /* Send data via LINFLEXD */
    LINFLEXD_UART_DRV_SendData(INST_LINFLEXD_UART1,(uint8_t *) sourceStr, strlen(sourceStr));
    while(LINFLEXD_UART_DRV_GetTransmitStatus(INST_LINFLEXD_UART1,&byteRemaining) != STATUS_SUCCESS);
}

/*FUNCTION**********************************************************************
*
* Description   :  reads the option chosen by the user
* param option  :  pointer to the character input from keyboard.
* return:          None
* END**************************************************************************/
void readOptionFromConsole(uint8_t *option)
{
    bool strReceived = false;
    uint8_t buffer[255];
    uint32_t byteRemaining = 0U;

    while(strReceived == false)
    {
        /* Because the terminal appends new line to user data,
         * receive and store data into a buffer until it is received
         */
        LINFLEXD_UART_DRV_ReceiveData(INST_LINFLEXD_UART1,buffer, 1);
        while(LINFLEXD_UART_DRV_GetReceiveStatus(INST_LINFLEXD_UART1,&byteRemaining) != STATUS_SUCCESS);
        strReceived = true;
    }
    if(buffer[0] == 'c')
    {
        *option = buffer[0];
    }
    else
    {
        print((const char *)"Invalid key!\r\n");
        *option = 0;
    }
}
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.1 [05.21]
**     for the NXP C55 series of microcontrollers.
**
** ###################################################################
*/
