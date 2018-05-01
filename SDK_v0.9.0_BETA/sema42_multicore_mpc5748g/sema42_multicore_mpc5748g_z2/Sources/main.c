/* ###################################################################
**     Filename    : main.c
**     Project     : sema42_multicore_mpc5748g_z2
**     Processor   : MPC5748G_324
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-05-31, 12:03, # CodeGen: 5
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
#include "pin_mux.h"

  volatile int exit_code = 0;
/* User includes (#include below this line is not maintained by Processor Expert) */
#define SEMA42_INSTANCE		0U
#define SEMA42_GATE			0U
#define LED0_PORT	PTA
#define LED0		10
#define LED1_PORT	PTA
#define LED1		7
#define LED2_PORT	PTH
#define LED2		13
#define LED3_PORT	PTC
#define LED3		4
#define LED4_PORT	PTH
#define LED4		5
#define LED5_PORT	PTJ
#define LED5		4
#define LED6_PORT	PTA
#define LED6		0
#define LED7_PORT	PTA
#define LED7		4

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

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of Processor Expert internal initialization.                    ***/

    int i = 0u;

	while (1)
    {
		/* Attempt to lock gate*/
		SEMA42_DRV_LockGate(SEMA42_INSTANCE, SEMA42_GATE);

		/* Check if gate is locked by current CPU*/
		if (SEMA42_DRV_GetGateLocker(SEMA42_INSTANCE, SEMA42_GATE) == (GET_CORE_ID() + 1))
        {

			/* Blink the first two LEDs to indicate that the gate is taken by the current CORE */
			delay(180000);
			PINS_DRV_TogglePins(LED0_PORT, (1 << LED0));
			PINS_DRV_TogglePins(LED1_PORT, (1 << LED1));
			delay(180000);
			PINS_DRV_TogglePins(LED0_PORT, (1 << LED0));
			PINS_DRV_TogglePins(LED1_PORT, (1 << LED1));

			/* Blink the common LED 5 times*/
			for (i = 0; i < 10; i++)
            {
				delay(180000);
				PINS_DRV_TogglePins(LED2_PORT, (1 << LED2));
			}

			/* Blink the first two LEDs to indicate that the gate is being released by this core*/
			delay(500000);
			PINS_DRV_TogglePins(LED0_PORT, (1 << LED0));
			PINS_DRV_TogglePins(LED1_PORT, (1 << LED1));
			delay(500000);
			PINS_DRV_TogglePins(LED0_PORT, (1 << LED0));
			PINS_DRV_TogglePins(LED1_PORT, (1 << LED1));

			/* Unlock the gate and delay so that the other core can acquire it*/
			SEMA42_DRV_UnlockGate(SEMA42_INSTANCE, SEMA42_GATE);
			delay(500000);
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
