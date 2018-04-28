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

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "clockMan1.h"
#include "pin_mux.h"
#if CPU_INIT_CONFIG
#include "Init_Config.h"
#endif

/* User includes (#include below this line is not maintained by Processor Expert) */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* Definition of the data transfer size */
#define BUFF_SIZE 8

/* Initialization of slave buffers */
uint16_t SendBuff[BUFF_SIZE];
uint16_t RecvBuff[BUFF_SIZE];
uint8_t cnt = 0;
bool CheckResult;

/* Send and receive two times */
void callback(i2s_event_t e, void *userData)
{
	uint32_t instance;
	instance = (uint32_t) userData;

	if (cnt < 2)
	{
        if (e == I2S_EVENT_TX_EMPTY)
            I2S_SetTxBuffer((uint8_t) instance, (const uint8_t*) SendBuff, BUFF_SIZE);
        if (e == I2S_EVENT_RX_FULL)
        {
            I2S_SetRxBuffer((uint8_t) instance, (uint8_t*) RecvBuff, BUFF_SIZE);
            cnt++;
        }
	}
}

volatile int exit_code = 0;

int main(void)
{
    uint32_t i;
    /* Initialize the data buffer */
    for (i = 0; i < BUFF_SIZE; i++)
    {
        SendBuff[i] = i;
    }

    /* Initialize clock */
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
            g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_FORCIBLE);
    /* Initialize dma */
    EDMA_DRV_Init (&dmaController1_State, &dmaController1_InitConfig0, edmaChnStateArray, edmaChnConfigArray, EDMA_CONFIGURED_CHANNELS_COUNT);
    /* Initialize pins */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
    /* Enables system interrupt */
    INT_SYS_EnableIRQGlobal();
    /* Initialize I2S pal */
    i2s_pal1_Config0.callbackParam = (void *) INST_I2S_PAL1;
    i2s_pal2_Config0.callbackParam = (void *) INST_I2S_PAL2;

	I2S_Init(INST_I2S_PAL1, &i2s_pal1_Config0);
    I2S_Init(INST_I2S_PAL2, &i2s_pal2_Config0);

    /* Call slave transfer function first */
    I2S_ReceiveData(INST_I2S_PAL2, (uint8_t*) RecvBuff, BUFF_SIZE);
    /* Master sends */
    I2S_SendData(INST_I2S_PAL1, (const uint8_t*) SendBuff, BUFF_SIZE);
    /* Check if transfer is completed with no errors */
    while (I2S_GetStatus(INST_I2S_PAL2, NULL) == STATUS_BUSY)
    {
    }
    I2S_Deinit(INST_I2S_PAL2);
    I2S_Deinit(INST_I2S_PAL1);
    CheckResult = true;
    for (i = 0U; i < BUFF_SIZE; i++)
    {
        if(SendBuff[i] != RecvBuff[i])
        {
            CheckResult = false;
            break;
        }
    }
    /* End of the driver example */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
#ifdef PEX_RTOS_INIT
	PEX_RTOS_INIT(); /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
#endif
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */
	/* Initialize and configure clocks
	 * 	-	see clock manager component for details
	 */

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
