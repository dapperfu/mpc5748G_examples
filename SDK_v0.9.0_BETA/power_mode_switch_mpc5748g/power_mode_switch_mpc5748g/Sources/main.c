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
**     Project     : power_mode_switch_mpc5748g
**     Processor   : MPC5748G_176
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-10-30, 12:21, # CodeGen: 6
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
**  @add to group main_module main module documentation
**  @{
*/
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "clockMan1.h"
#include "dmaController1.h"
#include "wkpu1.h"
#include "pin_mux.h"
#include "stm1.h"
#include "linflexd_uart1.h"
#include "pwrMan1.h"
#include "power_manager.h"

volatile int exit_code = 0;
/* User includes (#include below this line is not maintained by Processor Expert) */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define MENU_MESSAGE "Press:\r\n\
\t1) for SAFE\r\n\
\t2) for DRUN\r\n\
\t3) for RUN0\r\n\
\t4) for RUN1\r\n\
\t5) for RUN2\r\n\
\t6) for RUN3\r\n\
\t7) for STOP\r\n\
\t8) for STANDBY\r\n\
Enter your input:\r\n"
#define RX_MSG_LEN 1U
#define SEPARATOR "---------------------------------------\r\n"

/* Definition of power modes indexes, as configured in Power Manager Component
 *  Refer to the Reference Manual for details about the power modes.
 */
#define SAFE     (0u)  /* Safe         */
#define DRUN     (1u)  /* Drun         */
#define RUN0     (2u)  /* Run option 0 */
#define RUN1     (3u)  /* Run option 1 */
#define RUN2     (4u)  /* Run option 2 */
#define RUN3     (5u)  /* Run option 3 */
#define STOP0    (6u) /* Stop         */
#define STANDBY0 (7u) /* Standby      */

/* Function that initialize wake up */
void WakeUp_Init()
{
    WKPU_DRV_InitInterrupt(INST_WKPU1, WKPU_CHANNEL_COUNT0, wkpu1_InterruptCfg0);
    /* Set Channel 1 as Normal mode */
    WKPU_DRV_SetInterruptNormalMode(INST_WKPU1,WKPU_HW_CHANNEL1);

    /* Init STM0 1s */
    STM_DRV_Init(INST_STM1, &stm1_InitConfig0);
    STM_DRV_InitChannel(INST_STM1, &stm1_ChnConfig0);
    STM_DRV_EnableChannel(INST_STM1, 0);
    STM_DRV_StartTimer(INST_STM1);
}

/* brief: Send a string to user
 * param sourceStr: pointer to the array of characters
 *                  that you wish to send.
 * return:          None
 */
void print(const char *sourceStr)
{
    uint32_t byteRemaining = 0U;

    /* Send data via LINFLEXD */
    LINFLEXD_UART_DRV_SendData(INST_LINFLEXD_UART1,(uint8_t *) sourceStr, strlen(sourceStr));
    while(LINFLEXD_UART_DRV_GetTransmitStatus(INST_LINFLEXD_UART1,&byteRemaining) != STATUS_SUCCESS){}
}

/* Function that reads the option chosen by the user */
void readOptionFromConsole(uint8_t * option)
{
    uint8_t strReceived = 0U;
    uint8_t buffer[255];
    uint32_t byteRemaining = 0U;

    while(strReceived == 0U)
    {
        /* Because the terminal appends new line to user data,
         * receive and store data into a buffer until it is received
         */
        LINFLEXD_UART_DRV_ReceiveData(INST_LINFLEXD_UART1,buffer, 1);
        while(LINFLEXD_UART_DRV_GetReceiveStatus(INST_LINFLEXD_UART1,&byteRemaining) != STATUS_SUCCESS){}
        strReceived = 1U;
    }
    if(buffer[0] >= '1' && buffer[0] <= '8')
    {
        *option = buffer[0];
    }
    else
    {
        print((const char *)"Invalid option!\r\n");
        *option = 0;
    }
}

void delay(uint32_t time)
{
    while (time--);
}

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
    /* Variable used for storing user option */
    uint8_t option;
    /* Status inform that power mode switch successfully */
    status_t stSwitchMode;

    /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
    #ifdef PEX_RTOS_INIT
        PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
    #endif
    /*** End of Processor Expert internal initialization.                    ***/

    /* Initialize and configure clocks
     *  -   Setup system clocks, dividers
     *  -   see clock manager component for more details
    */
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                   g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_FORCIBLE);

    /* Initialize pins
     *  -   See PinSettings component for more info
    */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
    /* Initialize Power Manager */
    POWER_SYS_Init(&powerConfigsArr, POWER_MANAGER_CONFIG_CNT, &powerStaticCallbacksConfigsArr, POWER_MANAGER_CALLBACK_CNT);

    /* Initialize LINFlexD module for UART usage
     * -   See LINFlexD_UARTSettings component for more info
    */
    LINFLEXD_UART_DRV_Init(INST_LINFLEXD_UART1, &linflexd_uart1_State, &linflexd_uart1_InitConfig0);

    /* Infinite loop */
    while(1)
    {
        /* Print menu */
        print((const char *)MENU_MESSAGE);
        /* Read user option */
        readOptionFromConsole(&option);
        switch (option)
                {
                /* SAFE */
                case '1':
                    print((const char *)"******** CPU is going in SAFE mode. Wait about 1 second to exit SAFE mode and return DRUN mode.\r\n");
                    /* Set power mode to SAFE */
                    stSwitchMode = POWER_SYS_SetMode(SAFE, POWER_MANAGER_POLICY_AGREEMENT);
                    delay(1000000U);
                    if(stSwitchMode == STATUS_SUCCESS)
                    {
                        /* Set power mode to DRUN */
                        stSwitchMode = POWER_SYS_SetMode(DRUN, POWER_MANAGER_POLICY_AGREEMENT);
                        if(stSwitchMode == STATUS_SUCCESS)
                        {
                            print((const char *)"******** CPU exit from SAFE mode to DRUN mode.\r\n");
                        }
                        else
                        {
                            print((const char *)"******** CPU entered SAFE mode, did not switch to DRUN mode\r\n");
                        }
                    }
                    else
                    {
                        print((const char *)"CPU switches SAFE mode unsuccessfully.\r\n");
                    }
                    break;
                /* DRUN */
                case '2':
                    print((const char *)"******** CPU is going in DRUN mode...\r\n");
                    /* Set power mode to SAFE */
                    stSwitchMode = POWER_SYS_SetMode(DRUN, POWER_MANAGER_POLICY_AGREEMENT);
                    if (stSwitchMode == STATUS_SUCCESS)
                    {
                        print((const char *)"CPU switches DRUN mode successfully.\r\n");
                    }
                    else
                    {
                        print((const char *)"CPU switches DRUN mode unsuccessfully.\r\n");
                    }
                    break;
                /* RUN0 */
                case '3':
                    print((const char *)"******** CPU is going in RUN0 mode...\r\n");
                    /* Set power mode to RUN0 */
                    stSwitchMode = POWER_SYS_SetMode(RUN0, POWER_MANAGER_POLICY_AGREEMENT);
                    if (stSwitchMode == STATUS_SUCCESS)
                    {
                        print((const char *)"CPU switches RUN0 mode successfully.\r\n");
                    }
                    else
                    {
                        print((const char *)"CPU switches RUN0 mode unsuccessfully.\r\n");
                    }
                    break;
                /* RUN1 */
                case '4':
                    print((const char *)"******** CPU is going in RUN1 mode...\r\n");
                    /* Set power mode to RUN1 */
                    stSwitchMode = POWER_SYS_SetMode(RUN1, POWER_MANAGER_POLICY_AGREEMENT);
                    if (stSwitchMode == STATUS_SUCCESS)
                    {
                        print((const char *)"CPU switches RUN1 mode successfully.\r\n");
                    }
                    else
                    {
                        print((const char *)"CPU switches RUN1 mode unsuccessfully.\r\n");
                    }
                    break;
                /* RUN2 */
                case '5':
                    print((const char *)"******** CPU is going in RUN2 mode...\r\n");
                    /* Set power mode to RUN2 */
                    stSwitchMode = POWER_SYS_SetMode(RUN2, POWER_MANAGER_POLICY_AGREEMENT);
                    if (stSwitchMode == STATUS_SUCCESS)
                    {
                        print((const char *)"CPU switches RUN2 mode successfully.\r\n");
                    }
                    else
                    {
                        print((const char *)"CPU switches RUN2 mode unsuccessfully.\r\n");
                    }
                    break;
                /* RUN3 */
                case '6':
                    print((const char *)"******** CPU is going in RUN3 mode...\r\n");
                    /* Set power mode to RUN3 */
                    stSwitchMode = POWER_SYS_SetMode(RUN3, POWER_MANAGER_POLICY_AGREEMENT);
                    if (stSwitchMode == STATUS_SUCCESS)
                    {
                        print((const char *)"CPU switches RUN3 mode successfully.\r\n");
                    }
                    else
                    {
                        print((const char *)"CPU switches RUN3 mode unsuccessfully.\r\n");
                    }
                    break;
                /* STOP */
                case '7':
                    print((const char *)"******** CPU is going in STOP mode. Wait about 1 second to exit STOP mode.\r\n");
                    /* Call wake up function */
                    WakeUp_Init();
                    /* Set power mode to STOP */
                    stSwitchMode = POWER_SYS_SetMode(STOP0, POWER_MANAGER_POLICY_AGREEMENT);
                    if (stSwitchMode == STATUS_SUCCESS)
                    {
                        print((const char *)"CPU was entered STOP0 mode successfully and then woke up to exit STOP0 mode.\r\n");
                    }
                    else
                    {
                        print((const char *)"CPU was entered STOP0 mode unsuccessfully.\r\n");
                    }
                    break;
                /* STANDBY */
                case '8':
                    print((const char *)"******** CPU is going in STANDBY mode. Wait about 1 second to exit STANDBY mode and reset CPU.\r\n");
                    /* Call wake up function */
                    WakeUp_Init();
                    /* Configure wake-up address for z4a core */
                    /* Address start of start up */
                    MC_ME->CADDR1 = 0x01000001;
                    /* Set power mode to STANDBY */
                    stSwitchMode = POWER_SYS_SetMode(STANDBY0, POWER_MANAGER_POLICY_AGREEMENT);
                    break;
                default:
                    /* This statement should not be reached */
                    break;
                }
                /* Print separator */
                print((const char *)SEPARATOR);

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
