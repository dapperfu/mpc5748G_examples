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

#include "Cpu.h"
#include "clockMan1.h"
#include "pin_mux.h"
#include "ethernet1.h"
#include "device_registers.h"

volatile int exit_code = 0;
/* User includes (#include below this line is not maintained by Processor Expert) */
#include <stdint.h>
#include <stdbool.h>
/* This example is setup to work by default with EVB. To use it with other boards
   please comment the following line
*/

#define LED_PORT	PTA
#define LED1_PIN	4	/* User LED DS11 */
#define LED2_PIN	0	/* User LED DS10 */
#define LED3_PIN	10	/* User LED DS4 */
#define LED4_PIN	7	/* User LED DS5 */

void delay(volatile int cycles)
{
    /* Delay function - do nothing for a number of cycles */
    while(cycles--);
}

/* Autonegotiation complete callback - LED1 will be switched on and LED3 will be switched off */
void autoneg_complete(uint8_t phy)
{
	phy_speed_t speed;

	PHY_GetLinkSpeed(phy, &speed);

	switch (speed)
	{
	case PHY_SPEED_10_MBPS:
		ENET_DRV_SetSpeed(INST_ETHERNET1, ENET_MII_SPEED_10M);
		PINS_DRV_ClearPins(LED_PORT, 1 << LED1_PIN);
		PINS_DRV_SetPins(LED_PORT, 1 << LED3_PIN);
		break;
	case PHY_SPEED_100_MBPS:
		ENET_DRV_SetSpeed(INST_ETHERNET1, ENET_MII_SPEED_100M);
		PINS_DRV_ClearPins(LED_PORT, 1 << LED1_PIN);
		PINS_DRV_SetPins(LED_PORT, 1 << LED3_PIN);
		break;
	default:
		PINS_DRV_ClearPins(LED_PORT, 1 << LED3_PIN);
	}
}

/* Autonegotiation failed callback - LED3 will be switched on and LED1 will be switched off */
void autoneg_failed(uint8_t phy)
{
	PINS_DRV_SetPins(LED_PORT, 1 << LED1_PIN);
	PINS_DRV_ClearPins(LED_PORT, 1 << LED3_PIN);
}

/* Link up callback - LED2 will be switched on and LED4 will be switched off */
void link_up(uint8_t phy)
{
	PINS_DRV_SetPins(LED_PORT, 1 << LED4_PIN);
	PINS_DRV_ClearPins(LED_PORT, 1 << LED2_PIN);
}

/* Link up callback - LED4 will be switched on and LED2 will be switched off */
void link_down(uint8_t phy)
{
	PINS_DRV_SetPins(LED_PORT, 1 << LED2_PIN);
	PINS_DRV_ClearPins(LED_PORT, 1 << LED4_PIN);
}

int main(void)
{

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* Initialize and configure clocks
   * 	-	see clock manager component for details
   */

  CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
  CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

  PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
  PINS_DRV_SetPins(LED_PORT, (1 << LED1_PIN) | (1 << LED2_PIN) | (1 << LED3_PIN) | (1 << LED4_PIN));

  /* Initialize ENET driver */
  ENET_DRV_Init(INST_ETHERNET1, &ethernet1_State, &ethernet1_InitConfig0, ethernet1_buffConfigArr0, ethernet1_MacAddr);
  ENET_DRV_EnableMDIO(INST_ETHERNET1, false);

  /* Initialize PHY framework and PHY 0 */
  PHY_FrameworkInit(phyConfig, phyDrivers);
  PHY_Init(0U);
  PHY_EnableAutoneg(0U);

  for (;;)
  {
      delay(720000);

      /* Periodically invoke the PHY main function to check for events */
      PHY_MainFunction(0U);
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
**     for the Freescale S32K series of microcontrollers.
**
** ###################################################################
*/
