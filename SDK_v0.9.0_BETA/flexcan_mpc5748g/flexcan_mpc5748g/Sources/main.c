/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
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
**     Project     : flexcan_mpc5748g
**     Processor   : MPC5748G_176
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-11-06, 17:35, # CodeGen: 2
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

#include "Cpu.h"
#include "canCom1.h"
#include "clockMan1.h"
#include "dmaController1.h"
#include "pin_mux.h"
#if CPU_INIT_CONFIG
#include "Init_Config.h"
#endif

#include <stdbool.h>
#include <stdint.h>

/******************************************************************************
 * Definitions
 ******************************************************************************/

#define LED_PORT PTA
#define LED0 10U
#define LED1 7U

#define BTN0_PORT PTE
#define BTN1_PORT PTA
#define BTN0_PIN 12U
#define BTN1_PIN 3U
#define BTN0_EIRQ 11U
#define BTN1_EIRQ 0U

/* Use this define to specify if the application runs as master or slave */
#define MASTER
/* #define SLAVE */

/* Definition of the TX and RX message buffers depending on the bus role */
#if defined(MASTER)
#define TX_MAILBOX (1UL)
#define TX_MSG_ID (1UL)
#define RX_MAILBOX (0UL)
#define RX_MSG_ID (2UL)
#elif defined(SLAVE)
#define TX_MAILBOX (0UL)
#define TX_MSG_ID (2UL)
#define RX_MAILBOX (1UL)
#define RX_MSG_ID (1UL)
#endif

typedef enum {
  LED0_CHANGE_REQUESTED = 0x00U,
  LED1_CHANGE_REQUESTED = 0x01U
} can_commands_list;

uint8_t ledRequested = (uint8_t)LED0_CHANGE_REQUESTED;

/******************************************************************************
 * Function prototypes
 ******************************************************************************/
void SendCANData(uint32_t mailbox, uint32_t messageId, uint8_t *data,
                 uint32_t len);
void buttonISR(void);
void BoardInit(void);
void GPIOInit(void);

/******************************************************************************
 * Functions
 ******************************************************************************/

/**
 * Button interrupt handler
 */
void buttonISR(void) {
  /* Check if one of the buttons was pressed */
  uint32_t button0 = PINS_DRV_GetPinExIntFlag(BTN0_EIRQ);
  uint32_t button1 = PINS_DRV_GetPinExIntFlag(BTN1_EIRQ);

  bool sendFrame = false;

  /* Set FlexCAN TX value according to the button pressed */
  if (button0 != 0) {
    ledRequested = LED0_CHANGE_REQUESTED;
    sendFrame = true;
    /* Clear interrupt flag */
    PINS_DRV_ClearPinExIntFlag(BTN0_EIRQ);
  } else if (button1 != 0) {
    ledRequested = LED1_CHANGE_REQUESTED;
    sendFrame = true;
    /* Clear interrupt flag */
    PINS_DRV_ClearPinExIntFlag(BTN1_EIRQ);
  } else {
    PINS_DRV_ClearExIntFlag();
  }

  if (sendFrame) {
    /* Send the information via CAN */
    SendCANData(TX_MAILBOX, TX_MSG_ID, &ledRequested, 1UL);
  }
}

/*
 * @brief: Send data via CAN to the specified mailbox with the specified message
 * id
 * @param mailbox   : Destination mailbox number
 * @param messageId : Message ID
 * @param data      : Pointer to the TX data
 * @param len       : Length of the TX data
 * @return          : None
 */
void SendCANData(uint32_t mailbox, uint32_t messageId, uint8_t *data,
                 uint32_t len) {
  /* Set information about the data to be sent
   *  - 1 byte in length
   *  - Standard message ID
   *  - Bit rate switch enabled to use a different bitrate for the data segment
   *  - Flexible data rate enabled
   *  - Use zeros for FD padding
   */
  flexcan_data_info_t dataInfo = {.data_length = len,
                                  .msg_id_type = FLEXCAN_MSG_ID_STD,
                                  .enable_brs = false,
                                  .fd_enable = false,
                                  .fd_padding = 0U};

  /* Configure TX message buffer with index TX_MSG_ID and TX_MAILBOX*/
  FLEXCAN_DRV_ConfigTxMb(INST_CANCOM1, mailbox, &dataInfo, messageId);

  /* Execute send non-blocking */
  FLEXCAN_DRV_Send(INST_CANCOM1, mailbox, &dataInfo, messageId, data);
}

/*
 * @brief : Initialize clocks, pins and power modes
 */
void BoardInit(void) {

  /* Initialize and configure clocks
   *  -   Setup system clocks, dividers
   *  -   Configure FlexCAN clock, GPIO, LPSPI
   *  -   see clock manager component for more details
   */
  CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                 g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
  CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_FORCIBLE);

  /* Initialize pins
   *  -   Init FlexCAN, LPSPI and GPIO pins
   *  -   See PinSettings component for more info
   */
  PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
}

/*
 * @brief Function which configures the LEDs and Buttons
 */
void GPIOInit(void) {
  /* Set Output value LEDs */
  PINS_DRV_ClearPins(LED_PORT, (1 << LED0) | (1 << LED1));

  SIUL2->IMCR[155] = SIUL2_IMCR_SSS(1U);
  SIUL2->IMCR[144] = SIUL2_IMCR_SSS(1U);

  /* Install buttons ISR */
  INT_SYS_InstallHandler(SIUL_EIRQ_00_07_IRQn, &buttonISR, NULL);
  INT_SYS_InstallHandler(SIUL_EIRQ_08_15_IRQn, &buttonISR, NULL);

  /* Enable buttons interrupt */
  INT_SYS_EnableIRQ(SIUL_EIRQ_00_07_IRQn);
  INT_SYS_EnableIRQ(SIUL_EIRQ_08_15_IRQn);
}

volatile int exit_code = 0;
/* User includes (#include below this line is not maintained by Processor
 * Expert) */

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - __start (startup asm routine)
 * - __init_hardware()
 * - main()
 *   - PE_low_level_init()
 *     - Common_Init()
 *     - Peripherals_Init()
*/
int main(void) {
/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
#ifdef PEX_RTOS_INIT
  PEX_RTOS_INIT(); /* Initialization of the selected RTOS. Macro is defined by
                      the RTOS component. */
#endif
  /*** End of Processor Expert internal initialization.                    ***/

  /* Do the initializations required for this application */
  BoardInit();
  GPIOInit();

  /*
   * Initialize FlexCAN driver
   *  - 8 byte payload size
   *  - FD enabled
   *  - Bus clock as peripheral engine clock
   */
  FLEXCAN_DRV_Init(INST_CANCOM1, &canCom1_State, &canCom1_InitConfig0);

  /* Set information about the data to be received
   *  - 1 byte in length
   *  - Standard message ID
   *  - Bit rate switch enabled to use a different bitrate for the data segment
   *  - Flexible data rate enabled
   *  - Use zeros for FD padding
   */
  flexcan_data_info_t dataInfo = {.data_length = 1U,
                                  .msg_id_type = FLEXCAN_MSG_ID_STD,
                                  .enable_brs = false,
                                  .fd_enable = false,
                                  .fd_padding = 0U};

  /* Configure RX message buffer with index RX_MSG_ID and RX_MAILBOX */
  FLEXCAN_DRV_ConfigRxMb(INST_CANCOM1, RX_MAILBOX, &dataInfo, RX_MSG_ID);

  while (1) {
    /* Define receive buffer */
    flexcan_msgbuff_t recvBuff;

    /* Start receiving data in RX_MAILBOX. */
    FLEXCAN_DRV_Receive(INST_CANCOM1, RX_MAILBOX, &recvBuff);

    /* Wait until the previous FlexCAN receive is completed */
    while (FLEXCAN_DRV_GetTransferStatus(INST_CANCOM1, RX_MAILBOX) ==
           STATUS_BUSY)
      ;

    /* Check the received message ID and payload */
    if ((recvBuff.data[0] == LED0_CHANGE_REQUESTED) &&
        recvBuff.msgId == RX_MSG_ID) {
      /* Toggle output value LED1 */
      PINS_DRV_TogglePins(LED_PORT, (1 << LED0));

    } else if ((recvBuff.data[0] == LED1_CHANGE_REQUESTED) &&
               recvBuff.msgId == RX_MSG_ID) {
      /* Toggle output value LED0 */
      PINS_DRV_TogglePins(LED_PORT, (1 << LED1));
    }
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
