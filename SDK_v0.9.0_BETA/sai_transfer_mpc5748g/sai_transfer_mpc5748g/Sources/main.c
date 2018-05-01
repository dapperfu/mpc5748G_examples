/*
 * Copyright (c) 2015 - 2016 , Freescale Semiconductor, Inc.
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
**     Project     : sai_transfer_mpc5748g
**     Processor   : MPC5748G
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
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
#include "sai0.h"
#include "sai1.h"
#include "dmaController1.h"
#include "pin_mux.h"

#if CPU_INIT_CONFIG
  #include "Init_Config.h"
#endif

volatile int exit_code = 0;
/* User includes (#include below this line is not maintained by Processor Expert) */

#define BUFF_SIZE 100

uint16_t LeftData[BUFF_SIZE];
uint16_t RightData[BUFF_SIZE];
uint16_t* SendData[2] = {LeftData, RightData};
uint16_t RecvBuff[BUFF_SIZE*2];
uint16_t* RecvData[1] = {RecvBuff};
bool CheckResult;

/* check data from a certain index */
bool CheckData(uint32_t from)
{
    uint32_t j = 0;
    uint32_t i;
    for (i = from; i < BUFF_SIZE; i++)
    {
        if (RecvBuff[j*2] != LeftData[i] || RecvBuff[j*2+1] != RightData[i])
        {
            return false;
        }
        j++;
    }
    return true;
}

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - __start (startup asm routine)
 * - main()
*/
int main(void)
{
/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
#ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                 /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
#endif
/*** End of Processor Expert internal initialization.                    ***/

  /* initialize send data */
  uint32_t i;

  for (i = 0; i < BUFF_SIZE; i++)
  {
      LeftData[i] = i*2;
      RightData[i] = i*2+1;
  }
  /* Initialize and configure clocks
   *    -   see clock manager component for details
   */
  CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                 g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
  CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);
  /* Initialize edma
   *    -   See edma component for more info
   */
  EDMA_DRV_Init (&dmaController1_State, &dmaController1_InitConfig0, edmaChnStateArray, edmaChnConfigArray, EDMA_CONFIGURED_CHANNELS_COUNT);
  /* Initialize pins
   *    -   See PinSettings component for more info
   */
  PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
  /* Enables system interrupt */
  INT_SYS_EnableIRQGlobal();
  /* Initialize SAI module */

  SAI_DRV_FCDInit(INST_SAI0, SAI_FCD_PLL, 200000UL, false);
  SAI_DRV_RxInit(INST_SAI1, &sai1_InitConfig0, &sai1RxState);
  SAI_DRV_TxInit(INST_SAI0, &sai0_InitConfig0, &sai0TxState);

  /* Call receive data first because rx is slave */
  SAI_DRV_Receive(INST_SAI1, (uint8_t**) RecvData, BUFF_SIZE*2);
  /* Send data from SAI0 to SAI1 */
  SAI_DRV_Send(INST_SAI0, (const uint8_t**) SendData, BUFF_SIZE);
  /* Wait for transfer complete */
  while (SAI_DRV_GetReceivingStatus(INST_SAI1, NULL) == STATUS_BUSY)
  {
  }

  SAI_DRV_TxDeinit(INST_SAI0);
  SAI_DRV_RxDeinit(INST_SAI1);

  /* Check if data is received properly, result of the check is stored in CheckResult */
  CheckResult = CheckData(0);

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
**     for the Freescale S32K series of microcontrollers.
**
** ###################################################################
*/

