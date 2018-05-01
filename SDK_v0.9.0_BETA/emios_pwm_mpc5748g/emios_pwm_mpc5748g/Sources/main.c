/*
 * Copyright 2017 - 2018 NXP
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
**     Project     : emios_pwm_mpc5748g
**     Processor   : MPC5748G_324
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-03-15, 12:21, # CodeGen: 6
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
#include "eMIOS_Mc.h"
#include "eMIOS1.h"
#include "clockMan1.h"
#include "adConv1.h"
#include "pin_mux.h"

volatile int exit_code = 0;

/* User includes (#include below this line is not maintained by Processor Expert) */
#include <stdint.h>
#include <stdbool.h>
#define   DEVKIT
#define   EMIOS_INST1            (0U)

#ifdef DEVKIT
    #define   ADC_CHAN_NUM       (9U)         /* ADC1_P[0] corresponding to PB[4] */
    #define   EMIOS_CHANNEL      (7U)         /* Channel 7 eMIOS0 */
#else
    #define   ADC_CHAN_NUM       (9U)         /* ADC1_P[0] corresponding to PB[4] */
    #define   EMIOS_CHANNEL      (5U)         /* Channel 5 eMIOS0 */
#endif

/* brief : Get duty cycle value in ADC module
 * param : None
 * return: Duty cycle
 */
uint16_t getDutyCycle(void)
{
    uint16_t adcValue;
    uint8_t result;
    uint32_t len;
    ADC_DRV_StartConversion(INST_ADCONV1, ADC_CONV_CHAIN_NORMAL);
    while(!(ADC_DRV_GetStatusFlags(INST_ADCONV1) & ADC_FLAG_NORMAL_ENDCHAIN));
    len = ADC_DRV_GetConvResultsToArray(INST_ADCONV1, ADC_CONV_CHAIN_NORMAL, &adcValue, 1U);
    /* only one value should be read from hardware registers */
    DEV_ASSERT(len == 1U);
    (void)len;
    ADC_DRV_ClearStatusFlags(INST_ADCONV1, ADC_FLAG_NORMAL_ENDCHAIN);

    /* Convert adc value to duty cycle */
    result = (uint8_t)((adcValue *100 )/ 4095);
    return result;
}

/* brief: Setup duty cycle of waveforms
 * param dutyCycle: New duty cycle value.
 * return:          None
 */
void setDutyCycle(uint8_t dutyCycle)
{
    uint32_t periodCnt = EMIOS_DRV_PWM_GetPeriod(0U, EMIOS_CHANNEL);
    uint32_t dutyCycleCnt = (dutyCycle * periodCnt) / 100;
    EMIOS_DRV_PWM_SetDutyCycle(EMIOS_INST1, EMIOS_CHANNEL, dutyCycleCnt);
}

/* brief : Initialize and configure pins, clock, adc and emios driver
 * param : None
 * return: None
 */
void sysInit(void)
{
    /* Initialize and configure pins */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
    /* Initialize clocks */
    CLOCK_SYS_Init(g_clockManConfigsArr,   CLOCK_MANAGER_CONFIG_CNT,
                   g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    /* Initialize ADC */
    ADC_DRV_Reset(INST_ADCONV1);
    ADC_DRV_DoCalibration(INST_ADCONV1);
    ADC_DRV_ConfigConverter(INST_ADCONV1,&adConv1_ConvCfg0);
    ADC_DRV_EnableChannel(INST_ADCONV1, ADC_CONV_CHAIN_NORMAL, ADC_CHAN_NUM);

    /* If you want initialize eMIOS Global with eMIOS_Mc_InitConfig0 */
    EMIOS_DRV_InitGlobal(EMIOS_INST1, &eMIOS_Mc_InitConfig0);
    /* eMIOS Counter mode Initialization for eMIOS_Mc_CntChnConfig0 */
    EMIOS_DRV_MC_InitCounterMode(EMIOS_INST1, EMIOS_CNT_BUSF_DRIVEN, &eMIOS_Mc_CntChnConfig0);
    /* eMIOS PWM mode Initialization for eMIOS1_PWMChnConfig0 */
    EMIOS_DRV_PWM_InitMode(EMIOS_INST1, EMIOS_CHANNEL, &eMIOS1_PWMChnConfig0);
    /* Enable eMIOS Global */
    EMIOS_DRV_EnableGlobalEmios(EMIOS_INST1);
}

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/

/* This example is setup to work by default with DEVKIT.
*/
int main(void)
{
    uint16_t dutyCycle = 0U;
    /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
    #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
    #endif
    /*** End of Processor Expert internal initialization.                    ***/

    /* Initialize peripheral driver */
    sysInit();
    /* Infinite loop */
    while(1)
    {
        /* Get duty cycle */
        dutyCycle = getDutyCycle();
        /* Update new duty cycle */
        setDutyCycle(dutyCycle);
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
