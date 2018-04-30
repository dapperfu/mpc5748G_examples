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
**     Project     : freertos_mpc5748g
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
#include "FreeRTOS.h"
#include "clockMan1.h"
#include "pin_mux.h"

volatile int exit_code = 0;
/* User includes (#include below this line is not maintained by Processor Expert) */
#include <stdint.h>
#include <stdbool.h>
#include "task.h"

#define mainLED_DELAY                        ( ( TickType_t ) 500 / portTICK_PERIOD_MS )
#define mainLED_TASK_PRIORITY                ( tskIDLE_PRIORITY + 2 )

/* This example is setup to work by default with DEVKIT. To use it with other boards
   please comment the following line
*/
#define DEVKIT
#define START_OS_DEMO 1

#ifdef DEVKIT
	#define LED1_PORT	PTA
    #define LED1 		0          /* pin PA[0] - LED1 (DS10) on DEV-KIT */
	#define LED2_PORT	PTJ
    #define LED2 		4	       /* pin PJ[4] - LED2 (DS9) on DEV-KIT */
	#define LED3_PORT	PTH
    #define LED3 		5          /* pin PH[5] - LED3 (DS8) on DEV-KIT */
	#define LED4_PORT	PTC
    #define LED4 		4          /* pin PC[4] - LED4 (DS7) on DEV-KIT */
#else
	#define LED1_PORT	PTG
    #define LED1 		2          /* pin PG[2] - LED1 on Motherboard */
	#define LED2_PORT	PTG
    #define LED2 		3          /* pin PG[3] - LED2 on Motherboard */
	#define LED3_PORT	PTG
    #define LED3 		4          /* pin PG[4] - LED3 on Motherboard */
	#define LED4_PORT	PTG
    #define LED4 		5          /* pin PG[5] - LED4 on Motherboard */
#endif

uint32_t leds[] = {LED1, LED2, LED3, LED4};
GPIO_Type * ports[] = {LED1_PORT, LED2_PORT, LED3_PORT, LED4_PORT};

#if START_OS_DEMO
void vLEDTask( void *pvParameters )
{
    unsigned int ID = (unsigned int)pvParameters;


    for( ;; )
    {
        /* Not very exciting - just delay... */
        vTaskDelay( mainLED_DELAY/(ID+1) );

        PINS_DRV_ClearPins(ports[ID], (1 << leds[ID]));

        /* delay */
        vTaskDelay( mainLED_DELAY/((3-ID)+1) );

        PINS_DRV_SetPins(ports[ID], (1 << leds[ID]));
    }
}
#endif

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

    /* Initialize clocks */
    CLOCK_SYS_Init(g_clockManConfigsArr,   CLOCK_MANAGER_CONFIG_CNT,
  		 g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

#if START_OS_DEMO
  xTaskCreate( vLEDTask, ( const char * const ) "LedTask", configMINIMAL_STACK_SIZE, (void*)0, mainLED_TASK_PRIORITY, NULL );
  xTaskCreate( vLEDTask, ( const char * const ) "LedTask", configMINIMAL_STACK_SIZE, (void*)1, mainLED_TASK_PRIORITY+1, NULL );
  xTaskCreate( vLEDTask, ( const char * const ) "LedTask", configMINIMAL_STACK_SIZE, (void*)2, mainLED_TASK_PRIORITY+2, NULL );
  xTaskCreate( vLEDTask, ( const char * const ) "LedTask", configMINIMAL_STACK_SIZE, (void*)3, mainLED_TASK_PRIORITY+3, NULL );

  // Start the scheduler.
  vTaskStartScheduler();
#endif
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
