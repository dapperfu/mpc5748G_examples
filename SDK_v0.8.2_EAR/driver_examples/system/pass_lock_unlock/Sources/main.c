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
**     Project     : pass_lock_unlock_mpc5748g
**     Processor   : MPC5748G_324
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-06-11, 12:21, # CodeGen: 6
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
#include "string.h"
#include "Cpu.h"
#include "clockMan1.h"
#include "pin_mux.h"
#include "flash_c55_driver.h"
#include "linflexd_uart_driver.h"
#include "pass_driver.h"
#include "osif.h"

volatile int exit_code = 0;
uint32_t flashConfigReg1, flashConfigReg2;    /* These variables are used to save the flash configuration registers before disabling cache */
/* User includes (#include below this line is not maintained by Processor Expert) */

/* Macro for handling BUTTONS */
#define SW3_PRESS       (PINS_DRV_ReadPins(PTA) & (1 << 1))
#define SW4_PRESS       (PINS_DRV_ReadPins(PTA) & (1 << 2))

/* Some information for flash memory */
#define FLASH_FMC                       PFLASH_BASE
#define FLASH_PFCR1                     0x000000000U
#define FLASH_PFCR2                     0x000000004U
#define FLASH_FMC_BFEN_MASK             0x000000001U

/* Lock State for locking/unlocking FLASH memory using FLASH driver */
#define UNLOCK_LOW_BLOCKS               0x00000000U
#define UNLOCK_MID_BLOCKS               0x00000000U
#define UNLOCK_HIGH_BLOCKS              0x00000000U
#define UNLOCK_FIRST256_BLOCKS          0x00000000U
#define UNLOCK_SECOND256_BLOCKS         0x00000000U

/* Definition for start and end address of flash memory that we want to test */
#define FLASH_TEST_START_ADDR           0x00F90000U
#define FLASH_TEST_END_ADDR             0x00FBFFFFU     /* 192K */

#define BUFFER_SIZE                     256


/* Password to unlock passgroup 0. This password must be consistent with the password stored in "PASS Password Group 0" area in UTEST memory */
uint32_t passwordGroup0[] = { 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU };

/* Password to unlock passgroup 1. This password must be consistent with the password stored in "PASS Password Group 1" area in UTEST memory */
uint32_t passwordGroup1[] = { 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU };

/* Password to unlock passgroup 2. This password must be consistent with the password stored in "PASS Password Group 2" area in UTEST memory */
uint32_t passwordGroup2[] = { 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU };

/* Password to unlock passgroup 3. This password must be consistent with the password stored in "PASS Password Group 3" area in UTEST memory */
uint32_t passwordGroup3[] = { 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU };

void PUTS(const char* str)
{
    LINFLEXD_UART_DRV_SendDataBlocking(INST_LINFLEXD_UART1, (uint8_t*)str, strlen(str), 1000U);
}

/*
 * Verify the returned status of each function
 */
void CheckError(bool condition)
{
    if (condition == false)
    {
        PUTS("ERROR");
    }
}

/*
 * Disable Flash Cache
 */
void DisableFlashControllerCache(uint32_t flashConfigReg,
                                 uint32_t disableVal,
                                 uint32_t *origin_pflash_pfcr)
{
    /* Read the values of the register of flash configuration */
    *origin_pflash_pfcr = REG_READ32(FLASH_FMC + flashConfigReg);

    /* Disable Caches */
    REG_BIT_CLEAR32(FLASH_FMC + flashConfigReg, disableVal);
}

/*
 * Restore configuration register of FCM
 */
void RestoreFlashControllerCache(uint32_t flashConfigReg,
                                 uint32_t pflash_pfcr)
{
    REG_WRITE32(FLASH_FMC + flashConfigReg, pflash_pfcr);
}

/*
 * Board initialization
 */
void BoardInit()
{
    status_t ret = STATUS_SUCCESS;
    pass_life_cycle_t lifeCycle;
    uint8_t passgroup = 0;
    uint32_t newState = 0x00000000U;

    /* Initialize clock gate*/
    ret = CLOCK_SYS_Init(g_clockManConfigsArr,   CLOCK_MANAGER_CONFIG_CNT,
                         g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CheckError(ret == STATUS_SUCCESS);
    ret = CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);
    CheckError(ret == STATUS_SUCCESS);
    /* Initialize and configure pins */
    ret = PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
    CheckError(ret == STATUS_SUCCESS);

    /* Initialize LINFlexD module for UART usage */
    LINFLEXD_UART_DRV_Init(INST_LINFLEXD_UART1, &linflexd_uart1_State, &linflexd_uart1_InitConfig0);

    PUTS("\n\r PASS EXAMPLE\n\r");

    /* Invalidate flash controller cache */
    DisableFlashControllerCache(FLASH_PFCR1, FLASH_FMC_BFEN_MASK, &flashConfigReg1);
    DisableFlashControllerCache(FLASH_PFCR2, FLASH_FMC_BFEN_MASK, &flashConfigReg2);

    /* Initialize flash driver */
    ret = FLASH_DRV_Init();
    CheckError(ret == STATUS_SUCCESS);

    /* Unlock all blocks in low, middle, high, 256K spaces.
     * To be able to write to flash memory, you have to unlock it using both FLASH and PASS drivers. */
    ret = FLASH_DRV_SetLock(C55_BLOCK_LOW, UNLOCK_LOW_BLOCKS);
    CheckError(ret == STATUS_SUCCESS);
    ret = FLASH_DRV_SetLock(C55_BLOCK_MID, UNLOCK_MID_BLOCKS);
    CheckError(ret == STATUS_SUCCESS);
    ret = FLASH_DRV_SetLock(C55_BLOCK_HIGH, UNLOCK_HIGH_BLOCKS);
    CheckError(ret == STATUS_SUCCESS);
    ret = FLASH_DRV_SetLock(C55_BLOCK_256K_FIRST, UNLOCK_FIRST256_BLOCKS);
    CheckError(ret == STATUS_SUCCESS);
    ret = FLASH_DRV_SetLock(C55_BLOCK_256K_SECOND, UNLOCK_SECOND256_BLOCKS);
    CheckError(ret == STATUS_SUCCESS);

    /* Get the current life cycle of the device.
     * To be able to run this example, the lifecycle of the device must be PASS_LC_OEM_PRODUCTION or later */
    lifeCycle = PASS_DRV_GetLifeCycle();
    CheckError(lifeCycle >= PASS_LC_OEM_PRODUCTION);

    /* Unlock for accessing to the PASS registers */
    ret = PASS_DRV_UnlockPassgroup(0, passwordGroup0);
    CheckError(ret == STATUS_SUCCESS);
    ret = PASS_DRV_UnlockPassgroup(1, passwordGroup1);
    CheckError(ret == STATUS_SUCCESS);
    ret = PASS_DRV_UnlockPassgroup(2, passwordGroup2);
    CheckError(ret == STATUS_SUCCESS);
    ret = PASS_DRV_UnlockPassgroup(3, passwordGroup3);
    CheckError(ret == STATUS_SUCCESS);
    ret = STATUS_ERROR;

    /* Unlock for read operation for all blocks in LOW_MIDDLE area. */
    for (passgroup = 0; passgroup < 4; passgroup++)
    {
        /* Unlock for read operation for all blocks in LOW_MIDDLE area of Flash memory */
        ret = PASS_DRV_SetReadState(PASS_FLASH_BLOCK_LOW_MIDDLE, passgroup, newState);
        CheckError(ret == STATUS_SUCCESS);
    }

    // Initialize the randomization
    srand(0x10000);

    /* Initialize OSIF */
    OSIF_TimeDelay(0);
    PUTS("\n\r Initialization is successful.");
    PUTS("\n\r Press SW3 to unlock for the write operation \n\r and then test the accessing the flash memory");
    PUTS("\n\r Press SW4 to test the accessing the flash memory without unlocking\n\r");
}

void UnlockFlash()
{
    status_t ret = STATUS_SUCCESS;
    uint8_t passgroup = 0;
    /* The state used to unlock for all blocks in a given area of flash memory */
    uint32_t newState = 0x00000000U;

    PUTS("\n\r\n\r Unlocking for write operation... ");

    /* PASS module supports 4 levels of password. To be able to unlock the flash memory, you have to unlock for both 4 levels */
    for (passgroup = 0; passgroup < 4; passgroup++)
    {
        /* Unlock for write and erase operation for all blocks in LOW_MIDDLE area of Flash memory */
        ret = PASS_DRV_SetWriteEraseState(PASS_FLASH_BLOCK_LOW_MIDDLE, passgroup, newState);
        CheckError(ret == STATUS_SUCCESS);
    }
    PUTS("SUCCESS");
}

void LockFlash()
{
    status_t ret = STATUS_SUCCESS;
    uint8_t passgroup = 0;
    /* The state used to unlock for all blocks in a given area of flash memory */
    uint32_t newState = 0xFFFFFFFFU;

    PUTS("\n\r\n\r Locking for write operation... ");

    /* PASS module supports 4 levels of password. To be able to unlock the flash memory, you have to unlock for both 4 levels */
    for (passgroup = 0; passgroup < 4; passgroup++)
    {
        /* Unlock for write and erase operation for all blocks in LOW_MIDDLE area of Flash memory */
        ret = PASS_DRV_SetWriteEraseState(PASS_FLASH_BLOCK_LOW_MIDDLE, passgroup, newState);
        CheckError(ret == STATUS_SUCCESS);
    }
    PUTS("SUCCESS");
}

void TestFlash()
{
    flash_block_select_t blocksTobeErased;
    flash_state_t opResult;
    status_t ret = STATUS_SUCCESS;
    uint32_t i = 0;
    uint8_t* p;
    flash_context_data_t flashContext;
    uint8_t data[BUFFER_SIZE];

    /* Erase all blocks in MIDDLE area */
    blocksTobeErased.lowBlockSelect = 0U;
    blocksTobeErased.midBlockSelect = 0x03FFU;
    blocksTobeErased.highBlockSelect = 0U;
    blocksTobeErased.first256KBlockSelect = 0U;
    blocksTobeErased.second256KBlockSelect = 0x0U;

    PUTS("\n\r Erasing all blocks in MIDDLE area... ");

    ret = FLASH_DRV_Erase(ERS_OPT_MAIN_SPACE, &blocksTobeErased);
    CheckError(ret == STATUS_SUCCESS);

    /* Wait for erase operation to be completed */
    do
    {
        ret = FLASH_DRV_CheckEraseStatus(&opResult);
    }while (ret == STATUS_FLASH_INPROGRESS);

    /* Blank check */
    p = (uint8_t*)FLASH_TEST_START_ADDR;
    while ((uint32_t)p <= FLASH_TEST_END_ADDR)
    {
        if (*p != 0xFF)
        {
            ret = STATUS_ERROR;
            break;
        }
        p++;
    }
    if (ret != STATUS_SUCCESS)
    {
        PUTS("ERROR");
        return;
    }
    PUTS("SUCCESS");

    /* Test write operation */
    /* Randomize the buffer */
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        data[i] = (uint8_t)(rand() % 255);
    }
    PUTS("\n\r Writing to the flash... ");
    ret = FLASH_DRV_Program(&flashContext, FLASH_TEST_START_ADDR, BUFFER_SIZE, (uint32_t)data);
    CheckError(ret == STATUS_SUCCESS);

    /* Wait for program operation is finished */
    do
    {
        ret = FLASH_DRV_CheckProgramStatus(&flashContext, &opResult);
    }while (ret == STATUS_FLASH_INPROGRESS);
    PUTS("SUCCESS");

    /* Verify data */
    PUTS("\n\r Verifying the written data... ");
    p = (uint8_t*)FLASH_TEST_START_ADDR;
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        if (p[i] != data[i])
        {
            ret = STATUS_ERROR;
            break;
        }
    }
    if (ret != STATUS_SUCCESS)
    {
        PUTS("ERROR");
        return;
    }
    PUTS("SUCCESS");
}


/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
    /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
    #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
    #endif
    /*** End of Processor Expert internal initialization.                    ***/

    BoardInit();

    while (1)
    {
        if (SW3_PRESS)
        {
            /* If press SW3, unlock the flash using PASS driver and test the erase/write/read */
            UnlockFlash();
            TestFlash();
            while (SW3_PRESS);  /* Wait for SW3 to be released */
        }
        else if (SW4_PRESS)
        {
            /* If press SW4, lock the flash using PASS driver and test the erase/write/read */
            LockFlash();
            TestFlash();
            while (SW4_PRESS);  /* Wait for SW4 to be released */
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
