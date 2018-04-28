/*
 * Copyright 2018 NXP
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
**     Project     : mpu_pal_memory_protection_mpc5748g
**     Processor   : MPC5748G_176
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-10-26 12:00, # CodeGen: 1
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

  volatile int exit_code = 0;
/* User includes (#include below this line is not maintained by Processor Expert) */
#include <stdint.h>
#include <stdbool.h>

/* This example is setup to work by default with DEVKIT. To use it with other boards
   please comment the following line
*/
#define DEVKIT

#ifdef DEVKIT
    #define ON           0U     /* LED ON */
    #define OFF          1U     /* LED OFF */
    #define LED1_GPIO    PTJ    /* LED1 GPIO type */
    #define LED1         4U     /* pin PJ[4] - LED1 (DS9) on DEV-KIT */
    #define LED2_GPIO    PTA    /* LED2 GPIO type */
    #define LED2         0U     /* pin PA[0] - LED2 (DS10) on DEV-KIT */
    #define SW_GPIO      PTA    /* SW GPIO type */
    #define SW           3U     /* pin PA[3] - SW (SW1_PA3) on DEV_KIT */
#else
    #define ON           0U     /* LED ON */
    #define OFF          1U     /* LED OFF */
    #define LED1_GPIO    PTG    /* LED1 GPIO type */
    #define LED1         2U     /* pin PG[2] - LED1 (DS2) on Motherboard */
    #define LED2_GPIO    PTG    /* LED2 GPIO type */
    #define LED2         3U     /* pin PG[3] - LED2 (DS3) on Motherboard */
    #define SW_GPIO      PTA    /* SW GPIO type */
    #define SW           2U     /* pin PA[2] - SW (SW4) on Motherboard */
#endif

/* Protected address in Flash memory */
#define ADDRESS_PROTECT    0x01138804U

/* Expected error */
mpu_error_info_t expectedError =
{
    .master     = FEATURE_SMPU_MASTER_CORE_Z4A,          /* Error Core Z4A master */
    .overrun    = false,                                 /* Error overrun */
    .attributes = MPU_ERROR_SUPERVISOR_MODE_DATA_ACCESS, /* Error data access in supervisor mode */
    .accessType = MPU_ERROR_TYPE_READ,                   /* Error read access */
    .accessCtr  = 0x400000U,                             /* Error occurs on region 1 */
    .addr       = ADDRESS_PROTECT,                       /* Error address */
    .processId  = 0U                                     /* Error processor identifier */
};

/*!
 * @brief The error comparator
 *
 * @param[in] error1 The error access.
 * @param[in] error2 The error access.
 * @return Status
 *         - true : The errors are the same.
 *         - false: The errors are different.
 */
bool ErrorCompare(mpu_error_info_t error1, mpu_error_info_t error2)
{
    bool status = false;

    if ((error1.master == error2.master)
        && (error1.overrun == error2.overrun)
        && (error1.attributes == error2.attributes)
        && (error1.accessType == error2.accessType)
        && (error1.accessCtr  == error2.accessCtr)
        && (error1.addr == error2.addr))
    {
        status = true;
    #if FEATURE_SMPU_HAS_PROCESS_IDENTIFIER
        if(error1.processId != error2.processId)
        {
            status = false;
        }
    #endif
    }

    return status;
}

/* Data Storage Exception */
void IVOR2_Handler(void)
{
    /* Enables region 2 to grant Core Z4A read permission */
    (void)MPU_EnableRegion(&mpu_pal1_Instance, 2U, true);
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
    bool status = false;
    status_t returnCode = STATUS_ERROR;
    volatile uint32_t test;
    mpu_error_info_t reportedError;

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of Processor Expert internal initialization.                    ***/

    /* Initialize clock module */
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    /* Initialize LEDs and Button configuration */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

    /* LEDs off */
    PINS_DRV_WritePin(LED1_GPIO, LED1, OFF);
    PINS_DRV_WritePin(LED2_GPIO, LED2, OFF);

    /* Initializes MPU module */
    returnCode = MPU_Init(&mpu_pal1_Instance, MPU_PAL_REGION_CFG_CNT0, mpu_pal1_RegionConfig0);

    /* Check initialization */
    if (returnCode == STATUS_SUCCESS)
    {
        /* Turn on led1 */
        PINS_DRV_WritePin(LED1_GPIO, LED1, ON);
    }

    /* Infinite loop */
    for(;;)
    {
        /* Checks button */
        if (PINS_DRV_ReadPins(SW_GPIO) & (1U << SW))
        {
            /* Disables region 2 to ignore Core Z4A read permission */
            (void)MPU_EnableRegion(&mpu_pal1_Instance, 2U, false);
        }

        /* Read address in flash memory where protected by SMPU */
        test = *((uint32_t *)ADDRESS_PROTECT);
        /* Casting to void to avoid "Set but unused warnings" */
        (void)test;

        /* Get the detail of error access on master Core Z4A */
        status = MPU_GetError(&mpu_pal1_Instance, FEATURE_SMPU_MASTER_CORE_Z4A, &reportedError);

        /* Check error status */
        if (status)
        {
            /* Compare with expected error */
            status = ErrorCompare(reportedError, expectedError);

            /* If true */
            if (status)
            {
                /* Turn on led2 and turn off led1 */
                PINS_DRV_WritePin(LED2_GPIO, LED2, ON);
                PINS_DRV_WritePin(LED1_GPIO, LED1, OFF);
                break;
            }
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
