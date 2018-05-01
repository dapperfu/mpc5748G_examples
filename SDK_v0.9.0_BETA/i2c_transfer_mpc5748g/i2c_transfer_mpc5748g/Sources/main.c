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

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "clockMan1.h"
#include "dmaController1.h"
#include "i2c1.h"
#include "pin_mux.h"
#include <devassert.h>

/* User includes (#include below this line is not maintained by Processor
 * Expert) */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* Definition of the data transfer size */
#define TRANSFER_SIZE 16

/* Initialization of slave buffers */
uint8_t slaveTxBuffer[TRANSFER_SIZE] = {0x0, 0x1, 0x02, 0x3, 0x4, 0x5,
                                        0x6, 0x7, 0x8,  0x9, 0xa, 0xb,
                                        0xc, 0xd, 0xe,  0xf};
uint8_t slaveRxBuffer[TRANSFER_SIZE] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

i2c_master_state_t i2c0MasterState;
i2c_slave_state_t i2c1SlaveState;

/*!
 *  @brief I2C Slave Callback
 *
 *  @param [in] instance   I2C instance number
 *  @param [in] slaveEvent Event received on the I2C bus
 *  @param [in] userData   User defined data that is passed to the callback
 *  @return None
 *
 *  @details This function will be called by I2C interrupt handler and it
 *  will assign the buffer for TX or RX events.
 *  If an error event occurs, it will abort the current transfer.
 */
void i2c1_SlaveCallback0(i2c_slave_event_t slaveEvent, void *userData) {
  /* Get instance number from userData */
  uint32_t instance;
  instance = (uint32_t)userData;

  /* Check the event type:
   *  - set RX or TX buffers depending on the master request type
   */
  if (slaveEvent == I2C_SLAVE_EVENT_RX_REQ)
    I2C_DRV_SlaveSetRxBuffer((uint8_t)instance, slaveRxBuffer, TRANSFER_SIZE);
  if (slaveEvent == I2C_SLAVE_EVENT_TX_REQ)
    I2C_DRV_SlaveSetTxBuffer((uint8_t)instance, slaveTxBuffer, TRANSFER_SIZE);
}

volatile int exit_code = 0;

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/

int main(void) {
  /* Declaration of the I2C transfer buffer */
  uint8_t masterTxBuffer[TRANSFER_SIZE];
  /* Variable that is used to initialize the buffers */
  uint8_t cnt;
  bool isTransferOk = true;

  /* Variable used for the loop that initializes the data buffer */
  uint16_t i;

  /* Use as callback parameter for slave module the I2C instance number */
  i2c1_SlaveConfig0.callbackParam = (uint32_t *)INST_I2C3;

  /* Initialize and configure clocks
   *  - Configure system clocks and dividers
   *  - Configure LPI2C clock gating
   *  -   see clock manager component for details
   */
  CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                 g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
  CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_FORCIBLE);

  /* Initialize pins
   *  - Configure I2C pins
   *  -   See PinSettings component for more info
   */
  PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

  /* Initialize I2C Master configuration
   *  See I2C component for configuration details
   */
  I2C_DRV_MasterInit(INST_I2C4, &i2c1_MasterConfig0, &i2c0MasterState);

  /* Initialize I2C Master configuration
   *  See I2C component for configuration details
   */
  I2C_DRV_SlaveInit(INST_I2C3, &i2c1_SlaveConfig0, &i2c1SlaveState);

  /* Initialize the data buffer */
  for (i = 0u; i < TRANSFER_SIZE; i++) {
    masterTxBuffer[i] = i;
  }

  /* Send a packet of data to the bus slave */
  (void)I2C_DRV_MasterSendDataBlocking(INST_I2C4, masterTxBuffer, TRANSFER_SIZE,
                                       true, OSIF_WAIT_FOREVER);

  /* Check if transfer is completed with no errors */
  for (cnt = 0U; cnt < TRANSFER_SIZE; cnt++) {
    /* If the values are not equal, break the loop and set isTransferOk to false
     */
    if ((masterTxBuffer[cnt] != slaveRxBuffer[cnt])) {
      isTransferOk = false;
      break;
    }
  }

  /* Cast isTransferOk to avoid "set but not used" warnings */
  (void)isTransferOk;

  /* End of the driver example */

/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
#ifdef PEX_RTOS_INIT
  PEX_RTOS_INIT(); /* Initialization of the selected RTOS. Macro is defined by
                      the RTOS component. */
#endif
/*** End of Processor Expert internal initialization.                    ***/

/* Write your code here */
/* For example: for(;;) { } */

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
