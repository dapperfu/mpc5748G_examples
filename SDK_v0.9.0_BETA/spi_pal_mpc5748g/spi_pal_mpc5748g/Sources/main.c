/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/* ###################################################################
**     Filename    : main.c
**     Project     : spi_pal_mpc5748g
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
#include "pin_mux.h"
#include "spi1.h"
#include "spi2.h"
#include "dmaController1.h"

  volatile int exit_code = 0;
/* User includes (#include below this line is not maintained by Processor Expert) */
#include <stdint.h>
#include <stdbool.h>
#define TIMEOUT 1000U
#define NUMBER_OF_FRAMES 10U
#define BUFFER_SIZE 10U

/* This example is setup to work by default with DEVKIT. To use it with other boards
   please comment the following line
*/
#define DEVKIT

#ifdef DEVKIT
  #define LED1 10 /* User LED 1 (DS4) on DEVKIT */
  #define LED2 7 /* User LED 2 (DS5) on DEVKIT */
  #define LED_PORT PTA
#else
  #define LED1 2 /* User LED 1 (DS2) on Motherboard */
  #define LED2 3 /* User LED 2 (DS3) on Motherboard */
  #define LED_PORT PTG
#endif

void delay(volatile int cycles)
{
  /* Delay function - do nothing for a number of cycles */
  while(cycles--);
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
  uint8_t master_send[BUFFER_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  uint8_t master_receive[BUFFER_SIZE];
  uint8_t slave_send[BUFFER_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  uint8_t slave_receive[BUFFER_SIZE];
  uint8_t cnt;
  bool isTransferOk = true;

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of Processor Expert internal initialization.                    ***/

  CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
  CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

  /* Initialize pins
  *	See PinSettings component for more info
  */
  PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

  /* Turn off LEDs */
  PINS_DRV_SetPins(LED_PORT, 1 << LED1);
  PINS_DRV_SetPins(LED_PORT, 1 << LED2);

  /* Initialize SPI0 */
  SPI_MasterInit(&spi1Instance, &MasterConfig0);
  /* Initialize DSPI0 */
  SPI_SlaveInit(&spi2Instance, &SlaveConfig0);

  /* Infinite loop */
  while(1)
  {
	SPI_SlaveTransfer(&spi2Instance, slave_send, slave_receive, NUMBER_OF_FRAMES);
	SPI_MasterTransferBlocking(&spi1Instance, master_send, master_receive, NUMBER_OF_FRAMES, TIMEOUT);

	/* Check if transfer is completed with no errors */
	for (cnt = 0U; cnt < BUFFER_SIZE; cnt++)
	{
		/* If the values are not equal, break the loop and set isTransferOk to false */
		if((master_send[cnt] != slave_receive[cnt]) || (slave_send[cnt] != master_receive[cnt]))
		{
			isTransferOk = false;
			break;
		}
    }

	    if(isTransferOk == true)
	    {
	    	/* Turn on LED1 to indicate a successful transfer*/
	    	PINS_DRV_ClearPins(LED_PORT, 1 << LED1);
	    	PINS_DRV_SetPins(LED_PORT, 1 << LED2);
	    	delay(1000);
	    }
	    else
	    {
	    	/* Turn on LED2 to indicate an unsuccessful transfer*/
	    	PINS_DRV_ClearPins(LED_PORT, 1 << LED2);
			PINS_DRV_SetPins(LED_PORT, 1 << LED1);
			delay(1000);
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
