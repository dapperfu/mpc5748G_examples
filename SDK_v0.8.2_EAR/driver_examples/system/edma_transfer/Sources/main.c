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
**     Processor   : MPC5748G
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
#include "dmaController1.h"
#include "clockMan1.h"
#include "console_uart.h"
#include "dma_uart.h"
#include "interrupts_uart.h"
#include "pin_mux.h"

  volatile int exit_code = 0;
/* User includes (#include below this line is not maintained by Processor Expert) */
#include "edmaTransfer.h"
#include <string.h>

/* Global variable storing the number of bytes yet to be received */
uint32_t bytesRemaining;
/* Macro defined to display text to console; it sends the
 * buffer via UART and waits the transmission to complete
 */
#define PRINTF(text) LINFLEXD_UART_DRV_SendData(INST_CONSOLE_UART, (uint8_t *)text, strlen(text));\
                     while(LINFLEXD_UART_DRV_GetTransmitStatus(INST_CONSOLE_UART, &bytesRemaining) != STATUS_SUCCESS);

/* Flag updated in DMA isr to signal transfer completion */
volatile bool transferComplete;

/* Text to be displayed on the console */
#define menuText "\r\nThis is a demo illustrating eDMA driver usage;\r\n\
Select one of the following options:\r\n\
  - press '1' to trigger a single block memory-to-memory transfer\r\n\
  - press '2' to trigger a loop memory-to-memory transfer\r\n\
  - press '3' to trigger a memory-to-memory transfer using scatter/gather feature\r\n\
  - press '4' to trigger a DMA based serial transmission from LINFlexD_1 to LINFlexD_3\r\n\
  - press '5' to trigger a DMA based serial reception from LINFlexD_3 to LINFlexD_1\r\n\
  - press '0' to exit the demo\r\n"

/* Test buffer transferred by DMA */
#define dmaSampleBuffer "'This buffer is transferred via eDMA requests'\r\n"
/* Declare a buffer used to store the received data */
uint8_t destBuffer[255];

/* Callback for DMA channels */
void edmaCallback(void *parameter, edma_chn_status_t status)
{
    (void)status;
    (void)parameter;
    /* signal transfer completion */
    transferComplete = true;
}

/* Static function used to clear all elements in a buffer */
static void clearBuff(uint8_t *buff, uint32_t size)
{
    uint32_t i;
    for(i = 0U; i < size; i++)
        buff[i] = 0U;
}

/* Function that reads the option chosen by the user */
void readOptionFromConsole(uint8_t * option)
{
    bool strReceived = false;
    uint8_t i = 0;
    uint8_t buffer[255];

    while(strReceived == false)
    {
        /* Because the terminal appends new line to user data,
         * receive and store data into a buffer until it is received
         */
        LINFLEXD_UART_DRV_ReceiveData(INST_CONSOLE_UART, &buffer[i], 1UL);
        /* Wait for transfer to be completed */
        while(LINFLEXD_UART_DRV_GetReceiveStatus(INST_CONSOLE_UART, &bytesRemaining) != STATUS_SUCCESS);
         /* Check if current byte is new line */
        if(buffer[i++] == '\n')
            strReceived = true;
    }

    /* Check for invalid options */
    if(i > 2 || buffer[0] < '0' || buffer[0] > '5')
    {
        PRINTF("Invalid option!\r\n");
        *option = 0;
    }
    else
    {
        *option = buffer[0];
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

  /* Declare a variable to store the option entered by user */
  uint8_t option = 0;
  /* Declare a boolean flag to control the demo loop */
  bool exitDemo = false;
  /* Declare a variable to store the status of last operation */
  status_t status;

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                   g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    /* Initialize pins
     *    -    See PinSettings component for more info
     */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

    /* Initialize the LINFlexD instance used for console message display */
    LINFLEXD_UART_DRV_Init(INST_CONSOLE_UART, &console_uart_State, &console_uart_InitConfig0);
    /* Initialize the LINFlexD instance used for DMA based communication */
    LINFLEXD_UART_DRV_Init(INST_DMA_UART, &dma_uart_State, &dma_uart_InitConfig0);
    /* Initialize the LINFlexD instance used for interrupt-based communication */
    LINFLEXD_UART_DRV_Init(INST_INTERRUPTS_UART, &interrupts_uart_State, &interrupts_uart_InitConfig0);

    /* Initialize eDMA module & channels */
    EDMA_DRV_Init(&dmaController1_State, &dmaController1_InitConfig0,
                  edmaChnStateArray, edmaChnConfigArray, EDMA_CONFIGURED_CHANNELS_COUNT);

     /* Demo loop:
     *     - Receive option from user
     *     - Trigger the appropriate action
     */
    while (!exitDemo)
    {
        /* Display the menu message */
        PRINTF(menuText);

        /* Read the user option */
        readOptionFromConsole(&option);

        /* If the option is not valid, display the menu again */
        if(option == 0)
            continue;

        /* Trigger the action chosen by user */
        switch(option)
        {
            case '1':
                /* Single block memory-to-memory transfer */
                triggerSingleBlock(EDMA_CHN2_NUMBER, (uint8_t *)dmaSampleBuffer,
                                   destBuffer, strlen(dmaSampleBuffer));

                /* Display the source buffer */
                PRINTF("Source buffer:\r\n");
                PRINTF(dmaSampleBuffer);

                /* Display the destination buffer */
                PRINTF("Destination buffer (transferred in single block mode):\r\n");
                PRINTF((char *)destBuffer);

                /* Clear the destination buffer */
                clearBuff(destBuffer, 255);
                break;
            case '2':
                /* Loop memory-to-memory transfer */
                triggerLoopTransfer(EDMA_CHN2_NUMBER, (uint8_t *)dmaSampleBuffer,
                                    destBuffer, strlen(dmaSampleBuffer));

                /* Display the source buffer */
                PRINTF("Source buffer:\r\n");
                PRINTF(dmaSampleBuffer);

                /* Display the destination buffer */
                PRINTF("Destination buffer (transferred in loop mode):\r\n");
                PRINTF((char *)destBuffer);

                /* Clear the destination buffer */
                clearBuff(destBuffer, 255);
                break;
            case '3':
                /* Scatter/gather memory-to-memory transfer */
                triggerScatterGather(EDMA_CHN2_NUMBER, (uint8_t *)dmaSampleBuffer,
                                     destBuffer, strlen(dmaSampleBuffer));

                /* Display the source buffer */
                PRINTF("Source buffer:\r\n");
                PRINTF(dmaSampleBuffer);

                /* Display the destination buffer */
                PRINTF("Destination buffer (transferred in scatter/gather mode):\r\n");
                PRINTF((char *)destBuffer);

                /* Clear the destination buffer */
                clearBuff(destBuffer, 255);
                break;
            case '4':
                /* Send a buffer over LINFlexD-UART in DMA-based mode;
                 * received on a different instance using interrupt-based method */
                LINFLEXD_UART_DRV_SendData(INST_DMA_UART, (uint8_t*)dmaSampleBuffer, strlen(dmaSampleBuffer));
                status = LINFLEXD_UART_DRV_ReceiveDataBlocking(INST_INTERRUPTS_UART, destBuffer, strlen(dmaSampleBuffer), TIMEOUT);

                if(status == STATUS_SUCCESS)
                {
                    /* Display the source buffer */
                    PRINTF("Source buffer:\r\n");
                    PRINTF(dmaSampleBuffer);

                    /* Display the destination buffer */
                    PRINTF("Destination buffer (transmitted via DMA based serial communication):\r\n");
                    PRINTF((char *)destBuffer);
                }
                else
                {
                    PRINTF("There was an error transmitting the buffer; please make sure the LINFlexD pins are properly connected.\r\n");
                }
                /* Clear the destination buffer */
                clearBuff(destBuffer, 255);
                break;
            case '5':
                /* Send a buffer over LINFlexD-UART in DMA-based mode;
                 * received on a different instance using interrupt-based method */
                LINFLEXD_UART_DRV_SendData(INST_INTERRUPTS_UART, (uint8_t*)dmaSampleBuffer, strlen(dmaSampleBuffer));
                status = LINFLEXD_UART_DRV_ReceiveDataBlocking(INST_DMA_UART, destBuffer, strlen(dmaSampleBuffer), TIMEOUT);

                if(status == STATUS_SUCCESS)
                {
                    /* Display the source buffer */
                    PRINTF("Source buffer:\r\n");
                    PRINTF(dmaSampleBuffer);

                    /* Display the destination buffer */
                    PRINTF("Destination buffer (received via DMA based serial communication):\r\n");
                    PRINTF((char *)destBuffer);
                }
                else
                {
                    PRINTF("There was an error receiving the buffer; please make sure the LINFlexD pins are properly connected.\r\n");
                }

                /* Clear the destination buffer */
                clearBuff(destBuffer, 255);
                break;
            case '0':
                /* Exit the demo */
                PRINTF("Goodbye!");
                EDMA_DRV_Deinit();
                LINFLEXD_UART_DRV_Deinit(INST_CONSOLE_UART);
                LINFLEXD_UART_DRV_Deinit(INST_DMA_UART);
                LINFLEXD_UART_DRV_Deinit(INST_INTERRUPTS_UART);
                exitDemo = true;
                break;
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
