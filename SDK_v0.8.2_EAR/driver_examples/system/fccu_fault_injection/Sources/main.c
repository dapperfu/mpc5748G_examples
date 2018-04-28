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
**     Project     : fccu_fault_injection_mpc5748g
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
#include "fccu1.h"
#include "pin_mux.h"

#define VAL(x) #x
#define PRINT_VAL(x) VAL(x)

#define MAX_SIZE_MSG_BUFF (100U)
#define UART_TIME_OUT (1000000U)
  volatile int exit_code = 0;
  volatile int g_testAlarm=0;
  uint8_t msgBuff[MAX_SIZE_MSG_BUFF];

/* User includes (#include below this line is not maintained by Processor Expert) */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/* This example is setup to work by default with DEVKIT. To use it with other boards
   please comment the following line. Check if GPIO PINs are configured from
   PinSettings accordingly with the board USED.
*/
#define DEVKIT

#ifdef DEVKIT
    #define LED 4U       /* pin PJ[4] - LED1 (DS9) on DEV-KIT */
	#define PORT PTJ
	#define SW1_PORT PTA  /* pin PA[3]  - SW4 on DEV-KIT */
	#define SW1	3U
	#define SW2_PORT PTE  /* pin PE[12] - SW2 on DEV-KIT */
	#define SW2 12U
#else
    #define LED 2U         /* pin PG[2] - LED1 (DS2) on Motherboard */
	#define PORT PTG
	#define SW1_PORT PTA   /* pin PA[1] - SW3 on DEV-Motherboard */
	#define SW1	1U
	#define SW2_PORT PTF   /* pin PF[11]- SW6 on DEV-Motherboard */
	#define SW2 11U
#endif

void delay(volatile int cycles)
{
    /* Delay function - do nothing for a number of cycles */
    while(cycles--);
}

void TestAlarm(void * param)
{
	g_testAlarm++;
	/* Set LED Off*/
	PINS_DRV_SetPins(PORT,(1<<LED));
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
DEV_ASSERT(INST_FCCU1 < FCCU_INSTANCE_COUNT);
fccu_irq_status_t irq_status;
status_t status;
uint8_t faultNo, sizeMsg;

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of Processor Expert internal initialization.                    ***/

  /* Initialize clock gate*/
  CLOCK_SYS_Init(g_clockManConfigsArr,   CLOCK_MANAGER_CONFIG_CNT,
                 g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
  CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);
  /* Initialize pin settings for EOUT and SW Pins, LED ON */
  PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
  /* Enable EOUT Pins PF[10] and PF[12] in SAFE MODE */
  SIUL2->MSCR[90] |= SIUL2_MSCR_SMC(1);
  SIUL2->MSCR[92] |= SIUL2_MSCR_SMC(1);

  /* JTAG Pins Enable in SAFE MODE When Chip pass in SAFE MODE all Pads without SMC bit enabled will be disabled
  * PC[0] -> TDI -> MSCR[32] */
  SIUL2->MSCR[32] |= SIUL2_MSCR_SMC(1);
  /* PC[1] -> TDO -> MSCR[33] */
  SIUL2->MSCR[33] |= SIUL2_MSCR_SMC(1);
  /* PH[9] -> TCLK -> MSCR[121] */
  SIUL2->MSCR[121] |= SIUL2_MSCR_SMC(1);
  /* PH[10] -> TMS -> MSCR[122] */
  SIUL2->MSCR[122] |= SIUL2_MSCR_SMC(1);
  LINFLEXD_UART_DRV_Init(INST_LINFLEXD_UART1, &linflexd_uart1_State, &linflexd_uart1_InitConfig0);
  /* Initialize FCCU driver with settings */
  status = FCCU_DRV_Init(INST_FCCU1, fccu1_Control0, fccu1_NcfConfig0);
  if( status == STATUS_SUCCESS )
  {
	  /* Enable Interruptions */
	  INT_SYS_EnableIRQGlobal();
      (void)LINFLEXD_UART_DRV_SendDataBlocking(INST_LINFLEXD_UART1,(uint8_t *)"FCCU Example !\n\r", sizeof("FCCU Example !\n\r"), UART_TIME_OUT);
      (void)LINFLEXD_UART_DRV_SendDataBlocking(INST_LINFLEXD_UART1,(uint8_t *)"\n\r Press SW2 - DEVKIT or SW6 - Motherboard !\n\r",
    		  	  	  	  	  	  	  	  	   sizeof("\n\r Press SW2 - DEVKIT or SW6 - Motherboard !\n\r"), UART_TIME_OUT);
	  while(1)
	  {   /* Pressing SW2 from DEVKIT or SW6 from Motherboard, will Inject Fake Fault 31 and trigger TestAlarm */
		  if(PINS_DRV_ReadPins(SW2_PORT)&(1<<SW2))
		  {
			  /*Injection Fake Function ID 31, that will trigger a TestAlarm and Increment g_testAlarm and Turn OFF LED */
			  FCCU_DRV_SetNcfFake(INST_FCCU1,fccu1_NcfConfig0[2U]->functionID);
			  break;
		  }
	  }
	  /* Check if IRQ Status is updated to FCCU_ISR_IRQ_ALARM and then clear the status */
	  FCCU_DRV_IrqStatusGetEvent(INST_FCCU1, &irq_status);
	  sizeMsg=sprintf((char *)msgBuff, "IRQ Status = %s\n\r", (irq_status!=FCCU_ISR_IRQ_ALARM) ? PRINT_VAL(irq_status) : "FCCU_ISR_IRQ_ALARM");
	  (void)LINFLEXD_UART_DRV_SendDataBlocking(INST_LINFLEXD_UART1, msgBuff, sizeMsg, UART_TIME_OUT);
	  FCCU_DRV_IrqStatusSetEvent(INST_FCCU1, FCCU_ISR_NO);

	  /* Get Fault Source that changed from Normal to Alarm state, Fault No will be
	   * (fault ID + 1) in case of multiple sources will be 0xFF*/
	  FCCU_DRV_GetFreezeStatus(INST_FCCU1,FCCU_FRZ_NORMAL_ALARM, &faultNo);
	  sizeMsg=sprintf((char *)msgBuff, "Val Freeze Normal to Alarm = %d \n\r", (faultNo-1));
	  (void)LINFLEXD_UART_DRV_SendDataBlocking(INST_LINFLEXD_UART1, msgBuff, sizeMsg, UART_TIME_OUT);
	  /*Injection Fake Function ID 25, that will trigger an FAULT but is HW corrected
	   * activate EOUT Signals PF(10) and PF(11)*/
	  FCCU_DRV_SetNcfFake(INST_FCCU1,fccu1_NcfConfig0[1U]->functionID);
	  /* Check if IRQ Status is updated to FCCU_ISR_NO */
	  FCCU_DRV_IrqStatusGetEvent(INST_FCCU1, &irq_status);
	  sizeMsg=sprintf((char *)msgBuff, "IRQ Status = %s \n\r", irq_status ? PRINT_VAL(irq_status) : "ISR_NO");
	  (void)LINFLEXD_UART_DRV_SendDataBlocking(INST_LINFLEXD_UART1, msgBuff, sizeMsg, UART_TIME_OUT);
	  /* Get Fault Source that changed from Normal to Fault state, Fault No will be
	   * (fault ID + 1) in case of multiple sources will be 0xFF*/
	  FCCU_DRV_GetFreezeStatus(INST_FCCU1,FCCU_FRZ_NORMAL_FAULT, &faultNo);
	  sizeMsg=sprintf((char *)msgBuff, "Val Freeze Normal to Fault = %d \n\r", (faultNo-1));
	  (void)LINFLEXD_UART_DRV_SendDataBlocking(INST_LINFLEXD_UART1, msgBuff, sizeMsg, UART_TIME_OUT);
  }
  (void)LINFLEXD_UART_DRV_SendDataBlocking(INST_LINFLEXD_UART1,(uint8_t *)"\n\r Press SW4 - DEVKIT or SW3 - Motherboard will trigger fault Reset!\n\r",
      		  	  	  	  	  	  	  	  	   sizeof("\n\r Press SW4 - DEVKIT or SW3 - Motherboard will trigger fault Reset!\n\r"), UART_TIME_OUT);
  while(1)
  {
	  /* Pressing SW1 from DEVKIT or SW3 from Motherboard, will Inject Fake Fault 39 and reset chip */
	  if(PINS_DRV_ReadPins(SW1_PORT)&(1<<SW1))
	  {
		  /* Injection Fake Function ID 39, that will trigger a Chip Reset, g_testAlarm will be 0 and LED ON after reset */
		  FCCU_DRV_SetNcfFake(INST_FCCU1,fccu1_NcfConfig0[0U]->functionID);
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
