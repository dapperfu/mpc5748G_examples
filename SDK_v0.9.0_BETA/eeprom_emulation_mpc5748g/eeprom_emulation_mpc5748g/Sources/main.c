/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED  WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 * CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
/* ###################################################################
**     Filename    : main.c
**     Project     : eeprom_emulation_mpc5748g
**     Processor   : MPC5748G_324
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-06-30, 12:21, # CodeGen: 6
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
#include "eee1.h"


volatile int exit_code = 0;
/* User includes (#include below this line is not maintained by Processor Expert) */

/* Lock State */
#define UNLOCK_LOW_BLOCKS               0x00000000U
#define UNLOCK_MID_BLOCKS               0x00000000U
#define UNLOCK_HIGH_BLOCKS              0x00000000U

/* buffer size */
#define BUFFER_SIZE_BYTE                0x80U

#define BLOCK_START_ADDRS               0x00F90000U
#define BLOCK_END_ADDRS                 0x00F9FFFFU

#define NUMBER_OF_WORD_BLANK_CHECK      0x90

/* Platform Flash */
#define FLASH_FMC                       PFLASH_BASE
#define FLASH_PFCR1                     0x000000000U
#define FLASH_PFCR2                     0x000000004U
#define FLASH_FMC_BFEN_MASK             0x000000001U

static uint32_t s_swapNum = 0;
uint8_t buffer[BUFFER_SIZE_BYTE] = {NULL};

/**************************************************************
*                          Disable Flash Cache                *
***************************************************************/
void DisableFlashControllerCache(uint32_t flashConfigReg,
                                 uint32_t disableVal,
                                 uint32_t *origin_pflash_pfcr)
{
    /* Read the values of the register of flash configuration */
    *origin_pflash_pfcr = REG_READ32(FLASH_FMC + flashConfigReg);

    /* Disable Caches */
    REG_BIT_CLEAR32(FLASH_FMC + flashConfigReg, disableVal);
}

/*****************************************************************
*               Restore configuration register of FCM            *
******************************************************************/
void RestoreFlashControllerCache(uint32_t flashConfigReg,
                                 uint32_t pflash_pfcr)
{
    REG_WRITE32(FLASH_FMC + flashConfigReg, pflash_pfcr);
}

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
    status_t ret = STATUS_SUCCESS;
    uint32_t i;                    /* index variable */
    uint16_t id;                   /* id of data record */
    uint16_t size;                 /* size of data record */
    bool stop_flag = false;        /* control when to stop writing records*/
    bool re_init_flag = false;     /* control if it needs to re-init eeprom system*/
    uint32_t recordAddr;
    uint32_t numOfWordCycle;
    uint32_t dest;                 /* destination address */
    uint32_t failedAddress;        /* save the failed address in flash */
    uint32_t pflash_pfcr1, pflash_pfcr2;
    flash_block_select_t blockSelect;
    flash_state_t opResult;
    eee_state_t state;

    /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
    #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
    #endif
    /*** End of Processor Expert internal initialization.                    ***/

    /* Initialize clock gate*/
    CLOCK_SYS_Init(g_clockManConfigsArr,   CLOCK_MANAGER_CONFIG_CNT,
                   g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    /* Invalidate flash controller cache */
    DisableFlashControllerCache(FLASH_PFCR1, FLASH_FMC_BFEN_MASK, &pflash_pfcr1);
    DisableFlashControllerCache(FLASH_PFCR2, FLASH_FMC_BFEN_MASK, &pflash_pfcr2);

    /* Unlock all eeprom blocks */
    /**************************************************************************/
    /* Unlock all blocks in low address space                                 */
    /**************************************************************************/
    ret = FLASH_DRV_SetLock(C55_BLOCK_LOW, UNLOCK_LOW_BLOCKS);
    DEV_ASSERT(STATUS_SUCCESS == ret);

    /**************************************************************************/
    /* Unlock all blocks in mid address space                                 */
    /**************************************************************************/
    ret = FLASH_DRV_SetLock(C55_BLOCK_MID, UNLOCK_MID_BLOCKS);
    DEV_ASSERT(STATUS_SUCCESS == ret);

    /**************************************************************************/
    /* Unlock all blocks in high address space                                */
    /**************************************************************************/
    ret = FLASH_DRV_SetLock(C55_BLOCK_HIGH, UNLOCK_HIGH_BLOCKS);
    DEV_ASSERT(STATUS_SUCCESS == ret);

    /********************************************************************************/
    /* Call FLASH_DRV_Erase, FLASH_DRV_BlankCheck functions to erase all EEE blocks */
    /* Which are used in EEPROM emulation                                           */
    /********************************************************************************/
    blockSelect.lowBlockSelect = 0x0U;
    blockSelect.midBlockSelect = 0xFU;
    blockSelect.highBlockSelect = 0x0U;
    blockSelect.first256KBlockSelect = 0x0U;
    blockSelect.second256KBlockSelect = 0x0U;

    /* Erase block */
    ret = FLASH_DRV_Erase(ERS_OPT_MAIN_SPACE, &blockSelect);

    if (STATUS_SUCCESS == ret)
    {
        do
        {
            ret = FLASH_DRV_CheckEraseStatus(&opResult);
        }while(ret == STATUS_FLASH_INPROGRESS);
    }

    DEV_ASSERT(STATUS_SUCCESS == ret);

    dest = BLOCK_START_ADDRS;
    size = (uint16_t)(BLOCK_END_ADDRS + 1U - BLOCK_START_ADDRS);
    numOfWordCycle = NUMBER_OF_WORD_BLANK_CHECK;

    /* Blank check */
    ret = FLASH_DRV_BlankCheck(dest,
                               size,
                               numOfWordCycle,
                               &failedAddress,
                               NULL_CALLBACK);
    DEV_ASSERT(STATUS_SUCCESS == ret);

    /*---------------------------------------------------------------------*/
    /* Init eeprom emulation                                               */
    /*---------------------------------------------------------------------*/
    ret = EEE_DRV_InitEeprom(&userConf, &state);
    DEV_ASSERT(STATUS_SUCCESS == ret);

    /*---------------------------------------------------------------------*/
    /* Write eeprom data records                                           */
    /*---------------------------------------------------------------------*/
    s_swapNum = 0U;
    id = 4U;

    if (state.sizeField == 0U)
    {
        size = state.dataSize;
    }

    while (stop_flag == false)
    {
        if (state.sizeField != 0U)
        {
            size = id;
            if(size > BUFFER_SIZE_BYTE)
            {
                size = BUFFER_SIZE_BYTE;
            }
        }

        /* Initialize buffer */
        for (i = 0U; i < size; i++)
        {
            *(volatile uint8_t*)(buffer + i) = (uint8_t)(i+size);
        }

        ret = EEE_DRV_WriteEeprom(id,
                                  size,
                                  (uint32_t)buffer,
                                  EEE_IMMEDIATE_NONE);
        if(ret == STATUS_SUCCESS)
        {
            /*  Call to EEE_DRV_MainFunction if there is a swap operation */
            while (g_eraseStatusFlag == EEE_ERASE_IN_PROGRESS)
            {
                stop_flag = true;
                ret = EEE_DRV_MainFunction();
            }
            if (true == stop_flag)
            {
                s_swapNum++;
                if (s_swapNum < 2U )
                {
                    stop_flag = false;
                }
            }
            if (g_eraseStatusFlag == EEE_ERASE_SWAP_ERROR)
            {
                re_init_flag = true;
            }
        }
        else
        {
            re_init_flag = true;
        }

        if (re_init_flag == true)
        {
            /* Call to EEE_DRV_InitEeprom to try recovering eeprom system */
            ret = EEE_DRV_InitEeprom(&userConf, &state);
            DEV_ASSERT(STATUS_SUCCESS == ret);
        }

        id++;
        id = id % 0x50U;
    }

    /*---------------------------------------------------------------------*/
    /* Read data from eeprom                                               */
    /*---------------------------------------------------------------------*/
    for(i = 0U; i < BUFFER_SIZE_BYTE; i++)
    {
        buffer[i] = 0xFFU;
    }

    id = 0x10U;
    ret = EEE_DRV_ReadEeprom(id,
                             size,
                             (uint32_t)buffer,
                             &recordAddr,
                             EEE_IMMEDIATE_NONE);
    DEV_ASSERT(STATUS_SUCCESS == ret);

    /*---------------------------------------------------------------------*/
    /* Delete data from eeprom                                             */
    /*---------------------------------------------------------------------*/
    ret = EEE_DRV_DeleteRecord(id,
                               EEE_IMMEDIATE_NONE);
    DEV_ASSERT(STATUS_SUCCESS == ret);

    for(i = 0U; i < BUFFER_SIZE_BYTE; i++)
    {
        buffer[i] = 0xFFU;
    }

    ret = EEE_DRV_ReadEeprom(id,
                             size,
                             (uint32_t)buffer,
                             &recordAddr,
                             EEE_IMMEDIATE_NONE);
    DEV_ASSERT(STATUS_EEE_ERROR_DATA_NOT_FOUND == ret);

    /*---------------------------------------------------------------------*/
    /* Report eeprom status */
    /*---------------------------------------------------------------------*/
    ret = EEE_DRV_ReportEepromStatus(&i);
    DEV_ASSERT(STATUS_SUCCESS == ret);

    /* Erase all eeprom blocks*/
    ret = EEE_DRV_RemoveEeprom();
    DEV_ASSERT(STATUS_SUCCESS == ret);

    /* Restore flash controller cache */
    RestoreFlashControllerCache(FLASH_PFCR1, pflash_pfcr1);
    RestoreFlashControllerCache(FLASH_PFCR2, pflash_pfcr2);


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
