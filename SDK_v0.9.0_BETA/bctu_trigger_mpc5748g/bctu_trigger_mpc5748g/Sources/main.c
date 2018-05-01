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
 **     Project     : bctu_trigger_mpc5748g
 **     Processor   : MPC5748G_176
 **     Version     : Driver 01.00
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2017-06-29, 14:08, # CodeGen: 1
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
#include "adConv1.h"
#include "bctu1.h"
#include "clockMan1.h"
#include "osif.h"
#include "pin_mux.h"

volatile int exit_code = 0;
/* User includes (#include below this line is not maintained by Processor
 * Expert) */
#include <stdbool.h>
#include <stdint.h>

/* This example is setup to work by default with DEVKIT. To use it with other
   boards please comment the following line
 */
#define DEVKIT

#ifdef DEVKIT
#define LED_PORT PTJ
#define LED 4             /* pin PJ[4] - LED1 (DS9) on DEV-KIT */
#define ADC_CHAN_NUM (9u) /* ADC1_P[0] corresponding to PB[4] */
#else
#define LED_PORT PTG
#define LED 2             /* pin PG[2] - LED1 (DS2) on Motherboard */
#define ADC_CHAN_NUM (9u) /* ADC1_P[0] corresponding to PB[4] */
#endif

#define BCTU_EXAMPLE_PART_1_EN (1u) /* Enable/disable PART 1 of the example */

#define NUM_AVG_CONVS (15u)
#define TRIG_INDEX (0u)
#define TOTAL_NUM_SERIES                                                       \
  (2000u) /* total number of series of conversions executed during example     \
             Part 1 */
#define ADC_THRESHOLD                                                          \
  (2048u) /* should be around 1.6V for 12-bit resolution, right aligned */
#define NUM_LED_BLINKS (5u)

volatile uint16_t result[NUM_AVG_CONVS];
volatile uint16_t resultIdx = 0u;
volatile bool listConvsComplete = false;
volatile bool convsComplete = false;
volatile uint16_t exampleExecPart = 1u;

void BCTU_ListLast_IRQHandler() {
  DEV_ASSERT(BCTU_DRV_GetStatusFlag(INST_BCTU1, INST_ADCONV1,
                                    BCTU_FLAG_DATA_OVERRUN) == false);

  if (BCTU_DRV_GetStatusFlag(INST_BCTU1, INST_ADCONV1,
                             BCTU_FLAG_NEW_DATA_AVAILABLE)) {
    /* For last element in the list, the BCTU_ListLast_IRQ is triggered before
     * BCTU_ConvUpdate_IRQ (for new data).
     * So result for last conversion in the list must be read here. */
    result[resultIdx % NUM_AVG_CONVS] =
        BCTU_DRV_GetConvData(INST_BCTU1, INST_ADCONV1);

    resultIdx++;
  }

  listConvsComplete = true;
  DEV_ASSERT(resultIdx == NUM_AVG_CONVS); /* Make sure results have been stored
                                             for all conversions in the list */
  resultIdx = 0u;

  BCTU_DRV_ClearAllStatusFlags(INST_BCTU1, INST_ADCONV1);
}

void BCTU_ConvUpdate_IRQHandler() {
  /* This interrupt may occur for different events: NewData, DataOverrun or
   * Trigger occurrence (if enabled)*/
  if (BCTU_DRV_GetStatusFlag(
          INST_BCTU1, INST_ADCONV1,
          BCTU_FLAG_NEW_DATA_AVAILABLE)) /* Read data only for NewData Available
                                          */
  {
    result[resultIdx % NUM_AVG_CONVS] =
        BCTU_DRV_GetConvData(INST_BCTU1, INST_ADCONV1);

    resultIdx++;

#if (BCTU_EXAMPLE_PART_1_EN == 1u)
    if (exampleExecPart == 1u) /* executed only for PART 1 of the example */
    {
      if (resultIdx >= NUM_AVG_CONVS) {
        BCTU_DRV_StopLoopConversions(INST_BCTU1,
                                     TRIG_INDEX); /* End triggering loop */
        /* An additional conversion may get executed, because of a pending
         * trigger */
        convsComplete = true;

        resultIdx = 0;
      }
    }
#endif /* (BCTU_EXAMPLE_PART_1_EN == 1u) */
  }

  DEV_ASSERT(BCTU_DRV_GetStatusFlag(INST_BCTU1, INST_ADCONV1,
                                    BCTU_FLAG_DATA_OVERRUN) == false);

  /* Clear the flags for clearing the interrupt */
  BCTU_DRV_ClearStatusFlag(INST_BCTU1, INST_ADCONV1, BCTU_FLAG_DATA_OVERRUN);
  BCTU_DRV_ClearStatusFlag(INST_BCTU1, INST_ADCONV1,
                           BCTU_FLAG_NEW_DATA_AVAILABLE);
}

void ComputeAvgAndToggleLed() {
  uint32_t sum = 0u;
  uint32_t avgResult = 0u;
  uint8_t idx;

  for (idx = 0; idx < NUM_AVG_CONVS; idx++) {
    sum += result[idx];
  }
  avgResult = sum / NUM_AVG_CONVS;

  if (avgResult > ADC_THRESHOLD) {
    PINS_DRV_ClearPins(LED_PORT, (1 << LED));
  } else {
    PINS_DRV_SetPins(LED_PORT, (1 << LED));
  }
}

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
 */
int main(void) {
  /* Write your local variable definition here */

  uint16_t trigIndex = TRIG_INDEX;
  status_t status;
  uint8_t adcChans[NUM_AVG_CONVS], idx;
  const uint8_t startElemPosition =
      0u; /* May be set to any value from 0 -> maximum number of BCTU list
             entries. */
  uint32_t numSeries = 0u;

  (void)status;
  (void)numSeries;

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
#ifdef PEX_RTOS_INIT
  PEX_RTOS_INIT(); /* Initialization of the selected RTOS. Macro is defined by
                      the RTOS component. */
#endif
  /*** End of Processor Expert internal initialization.                    ***/

  /* Initialize and configure pins */
  PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
  /* Initialize clocks */
  CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                 g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
  CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

  /* Enable BCTU IRQs in interrupt controller */
  INT_SYS_EnableIRQ(BCTU_ListLast_IRQn);
  INT_SYS_EnableIRQ(BCTU_ConvUpdate_IRQn);

  /* Initialize ADC */
  ADC_DRV_Reset(INST_ADCONV1);
  ADC_DRV_DoCalibration(INST_ADCONV1);

  DEV_ASSERT(adConv1_ConvCfg0.ctuMode !=
             ADC_CTU_MODE_DISABLED); /* ADC CTU mode should be in Control or in
                                        Trigger modes */
  ADC_DRV_ConfigConverter(INST_ADCONV1, &adConv1_ConvCfg0);
  ADC_DRV_EnableChannel(INST_ADCONV1, ADC_CONV_CHAIN_NORMAL, ADC_CHAN_NUM);

  /* Configure BCTU */
  BCTU_DRV_Config(INST_BCTU1, &bctu1_globalConfig0);

#if (BCTU_EXAMPLE_PART_1_EN == 1u)
  /* <<< PART 1 >>>
   * Use a BCTU single trigger with loop activated, for triggering a series of
   * conversions. Compute the average value. Depending on this, turn an LED on
   * or off. */
  exampleExecPart = 1u;

  status = BCTU_DRV_ConfigTriggerSingle(INST_BCTU1, trigIndex,
                                        &bctu1_trigConfig0, ADC_CHAN_NUM);
  DEV_ASSERT(status == STATUS_SUCCESS);

  /* Move out of configuration mode */
  BCTU_DRV_WriteGlobalTriggerEn(INST_BCTU1, true);

  /* Temporarily disable write protect */
  BCTU_DRV_DisableWriteProtect(INST_BCTU1, BCTU_WR_PROT_TEMPORARY);
  BCTU_DRV_SwTriggerConv(INST_BCTU1, trigIndex);

  while (numSeries < TOTAL_NUM_SERIES) {
    if (convsComplete == true) {
      ComputeAvgAndToggleLed();

      convsComplete = false;
      numSeries++;

      if (numSeries < TOTAL_NUM_SERIES) /* On the last iteration, do not restart
                                           the triggering loop. */
      {
        OSIF_TimeDelay(10);

        /* Reconfigure the trigger with LoopEnable */
        status = STATUS_ERROR;
        idx = 0;
        while ((status != STATUS_SUCCESS) &&
               (idx < 10)) /* Make multiple attempts to configure. Assert if it
                              doesn't succeed. */
        {
          status = BCTU_DRV_ConfigTriggerSingle(
              INST_BCTU1, trigIndex, &bctu1_trigConfig0, ADC_CHAN_NUM);
          idx++;
        }
        DEV_ASSERT(status == STATUS_SUCCESS);

        /* Temporarily disable write protect */
        BCTU_DRV_DisableWriteProtect(INST_BCTU1, BCTU_WR_PROT_TEMPORARY);
        /* Trigger again the loop of triggers */
        BCTU_DRV_SwTriggerConv(INST_BCTU1, trigIndex);
      }
    }
  }
#endif /* (BCTU_EXAMPLE_PART_1_EN == 1u) */

  /* Blink led to signal transition between PART 1 and PART 2 of the example */
  for (idx = 0; idx < NUM_LED_BLINKS; idx++) {
    PINS_DRV_SetPins(LED_PORT, (1 << LED));
    OSIF_TimeDelay(400);
    PINS_DRV_ClearPins(LED_PORT, (1 << LED));
    OSIF_TimeDelay(400);
  }

  /* <<< PART 2 >>>
   * Use a BCTU list for triggering a series of conversions.
   * Compute the average value. Depending on this, turn an LED on or off. */
  exampleExecPart = 2u;
  resultIdx = 0u;

  status = BCTU_DRV_ResetTrigger(
      INST_BCTU1, trigIndex,
      1 /*ms*/); /* May be used same/different trigIndex as in Part 1 */
  DEV_ASSERT(status == STATUS_SUCCESS);

  for (idx = 0; idx < NUM_AVG_CONVS; idx++) {
    /* Set each list entry to trigger a conversion on the same ADC channel */
    /* because on evaluation boards there is only one potentiometer.
     * For other setups, each list entry may be set to trigger a different ADC
     * channel. */
    adcChans[idx] = ADC_CHAN_NUM;
  }

  BCTU_DRV_SetConvListArray(INST_BCTU1, startElemPosition, adcChans,
                            NUM_AVG_CONVS);
  status = BCTU_DRV_ConfigTriggerList(INST_BCTU1, TRIG_INDEX,
                                      &bctu1_trigConfig1, startElemPosition);
  DEV_ASSERT(status == STATUS_SUCCESS);

  /* Move out of configuration mode */
  BCTU_DRV_WriteGlobalTriggerEn(INST_BCTU1, true);

  BCTU_DRV_DisableWriteProtect(INST_BCTU1, BCTU_WR_PROT_TEMPORARY);
  BCTU_DRV_SwTriggerConv(INST_BCTU1, trigIndex);

  while (1) {
    if (listConvsComplete == true) {
      ComputeAvgAndToggleLed();

      listConvsComplete = false;

      /* Temporarily disable write protect */
      BCTU_DRV_DisableWriteProtect(INST_BCTU1, BCTU_WR_PROT_TEMPORARY);
      /* Trigger again the BCTU list */
      BCTU_DRV_SwTriggerConv(INST_BCTU1, trigIndex);
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
