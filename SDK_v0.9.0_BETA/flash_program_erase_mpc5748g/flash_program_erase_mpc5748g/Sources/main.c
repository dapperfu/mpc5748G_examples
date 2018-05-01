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
**     Project     : flash_program_erase_mpc5748g
**     Processor   : MPC5748G_176
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-10-31, 12:21, # CodeGen: 6
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
#include "flash_c55_driver.h"

volatile int exit_code = 0;
/* User includes (#include below this line is not maintained by Processor Expert) */

/* Lock State */
#define UNLOCK_LOW_BLOCKS               0x00000000U
#define UNLOCK_MID_BLOCKS               0x00000000U
#define UNLOCK_HIGH_BLOCKS              0x00000000U
#define UNLOCK_FIRST256_BLOCKS          0x00000000U
#define UNLOCK_SECOND256_BLOCKS         0x00000000U

/* buffer size */
#define BUFFER_SIZE_BYTE                0x1000U

#define BLOCK_START_ADDRS               0x00FC0000U
#define BLOCK_END_ADDRS                 0x00FC7FFFU

#define NUMBER_OF_WORD_BLANK_CHECK      0x90
#define NUMBER_OF_WORD_PGM_VERIFY       0x80
#define NUMBER_OF_WORD_CHECK_SUM        0x120

/* Platform Flash */
#define FLASH_FMC                       PFLASH_BASE
#define FLASH_PFCR1                     0x000000000U
#define FLASH_PFCR2                     0x000000004U
#define FLASH_FMC_BFEN_MASK             0x000000001U

uint32_t g_usrCnt = 0U;
uint32_t buffer[BUFFER_SIZE_BYTE / C55_WORD_SIZE];
flash_context_data_t pCtxData;

void UserCallBack(void)
{
    /* increase g_usrCnt */
    g_usrCnt++;
}

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
    uint32_t blkLockState;         /* block lock status to be retrieved */
    flash_block_select_t blockSelect;
    flash_state_t opResult;
    uint32_t numOfWordCycle;
    uint32_t dest;                 /* destination address */
    uint32_t size;                 /* size applicable */
    uint32_t source;               /* source address for program and verify */
    uint32_t failedAddress;        /* save the failed address in flash */
    uint32_t sum;                  /* check sum result */
    uint32_t i;                    /* Index */
    uint32_t pflash_pfcr1, pflash_pfcr2;

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

    /**************************************************************************/
    /* Initialize source buffer                                               */
    /**************************************************************************/
    for (i = 0; i < (BUFFER_SIZE_BYTE / C55_WORD_SIZE); i++)
    {
        buffer[i] = 0x0U;
    }

    /* Flash Initialization */
    ret = FLASH_DRV_Init();
    DEV_ASSERT(ret == STATUS_SUCCESS);

    /**************************************************************************/
    /* Lock to protect UTest address space                                    */
    /**************************************************************************/
    ret = FLASH_DRV_GetLock(C55_BLOCK_UTEST, &blkLockState);

    if (!(blkLockState & 0x00000001U))
    {
        ret = FLASH_DRV_SetLock(C55_BLOCK_UTEST, 0x1U);
        if (STATUS_SUCCESS != ret)
        {
            return ret;
        }
    }

    /**************************************************************************/
    /* Unlock all blocks in low address space                                 */
    /**************************************************************************/
    ret = FLASH_DRV_SetLock(C55_BLOCK_LOW, UNLOCK_LOW_BLOCKS);
    if (STATUS_SUCCESS != ret)
    {
        return ret;
    }

    /**************************************************************************/
    /* Unlock all blocks in mid address space                                 */
    /**************************************************************************/
    ret = FLASH_DRV_SetLock(C55_BLOCK_MID, UNLOCK_MID_BLOCKS);
    if (STATUS_SUCCESS != ret)
    {
        return ret;
    }

    /**************************************************************************/
    /* Unlock all blocks in high address space                                */
    /**************************************************************************/
    ret = FLASH_DRV_SetLock(C55_BLOCK_HIGH, UNLOCK_HIGH_BLOCKS);
    if (STATUS_SUCCESS != ret)
    {
        return ret;
    }

    /**************************************************************************/
    /* Unlock all blocks in 256K blocks                                       */
    /**************************************************************************/
    ret = FLASH_DRV_SetLock(C55_BLOCK_256K_FIRST, UNLOCK_FIRST256_BLOCKS);
    if (STATUS_SUCCESS != ret)
    {
        return ret;
    }

    ret = FLASH_DRV_SetLock(C55_BLOCK_256K_SECOND, UNLOCK_SECOND256_BLOCKS);
    if (STATUS_SUCCESS != ret)
    {
        return ret;
    }

    /**************************************************************************/
    /* Call FLASH_DRV_Erase, FLASH_DRV_BlankCheck, FLASH_DRV_Program,         */
    /* FLASH_DRV_ProgramVerify, FLASH_DRV_CheckSum functions                  */
    /**************************************************************************/
    blockSelect.lowBlockSelect = 0x4U;
    blockSelect.midBlockSelect = 0x0U;
    blockSelect.highBlockSelect = 0x0U;
    blockSelect.first256KBlockSelect = 0x0U;
    blockSelect.second256KBlockSelect = 0x0U;
    g_usrCnt = 0U;

    /* Erase block */
    ret = FLASH_DRV_Erase(ERS_OPT_MAIN_SPACE, &blockSelect);

    if (STATUS_SUCCESS == ret)
    {
        do
        {
            /* The user can do any tasks while check status function is still in progress */
            UserCallBack();
            ret = FLASH_DRV_CheckEraseStatus(&opResult);
        }while(ret == STATUS_FLASH_INPROGRESS);
    }

    if (STATUS_SUCCESS != ret)
    {
        return ret;
    }

    dest = BLOCK_START_ADDRS;
    size = BLOCK_END_ADDRS + 1 - BLOCK_START_ADDRS;
    numOfWordCycle = NUMBER_OF_WORD_BLANK_CHECK;

    /* Blank check */
    ret = FLASH_DRV_BlankCheck(dest,
                               size,
                               numOfWordCycle,
                               &failedAddress,
                               NULL_CALLBACK);

    if (STATUS_SUCCESS != ret)
    {
        return ret;
    }

    /* Program to beginning of block */
    dest = BLOCK_START_ADDRS;
    size = BUFFER_SIZE_BYTE;
    source = (uint32_t)buffer;
    g_usrCnt = 0U;
    ret = FLASH_DRV_Program(&pCtxData,
                            dest,
                            size,
                            source);

    if (STATUS_SUCCESS == ret)
    {
        do
        {
            /* The user can do any tasks while check status function is still in progress */
            UserCallBack();
            ret = FLASH_DRV_CheckProgramStatus(&pCtxData, &opResult);
        }while(ret == STATUS_FLASH_INPROGRESS);
    }

    if (STATUS_SUCCESS != ret)
    {
        return ret;
    }

    numOfWordCycle = NUMBER_OF_WORD_PGM_VERIFY;
    /* Program verify */
    ret = FLASH_DRV_ProgramVerify(dest,
                                  size,
                                  source,
                                  numOfWordCycle,
                                  &failedAddress,
                                  NULL_CALLBACK);

    if (STATUS_SUCCESS != ret)
    {
        return ret;
    }

    numOfWordCycle = NUMBER_OF_WORD_CHECK_SUM;
    /* Check sum */
    ret = FLASH_DRV_CheckSum(dest,
                             size,
                             numOfWordCycle,
                             &sum,
                             NULL_CALLBACK);

    if ((STATUS_SUCCESS != ret) && (sum != 0U))
    {
        return ret;
    }

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
