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
**     Filename    : task_sd_eraseblocks.c
**     Project     : sdhc_fatfs_mpc5748g
**     Processor   : MPC5748G_324
**     Version     : 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   :
**     Abstract    :
**         This module contains SD layer erase verification test.
**     Settings    :
**     Contents    :
**
** ###################################################################*/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "common/tasks/tasks.h"
#include "usdhc/usdhc_impl.h"

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 21.6, The standard library input/output
 * functions shall not be used. This is required for printing messages.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 14.4, The controlling expression of an if
 * statement and the controlling expression of an iteration-statement shall have
 * essentially Boolean type. This is required infinite loop.
 *
 */

/*******************************************************************************
 * Constants and macros
 *******************************************************************************/

#define LOOP_COUNT (50U)

/*******************************************************************************
 * Global variables
 *******************************************************************************/

/*******************************************************************************
 * Local types
 *******************************************************************************/

/*******************************************************************************
 * Local function prototypes
 *******************************************************************************/

/*******************************************************************************
 * Local variables
 *******************************************************************************/

/*******************************************************************************
 * Local functions
 *******************************************************************************/

/*******************************************************************************
 * Global functions
 *******************************************************************************/

/*
 * Implements task_sd_eraseblocks_Activity
 */
void task_sd_eraseblocks(void) {
  common_status_t status;
  uint32_t i = 0U;
  uint32_t blockAddress = DATA_BLOCK_START;
  uint32_t errorCount = 0U;
  sd_card_t *card;
  common_status_t state = (int32_t)STATUS_SUCCESS;

  (void)printf("Erase/Read test\r\n");
  status = uSDHC_Init(true);
  (void)printf("uSDHC/SD status: %ld\r\n", status);
  if ((common_status_t)STATUS_SUCCESS == status) {
    card = uSDHC_GetSDCard();
    randomizeInit();
    for (i = 0U; i < LOOP_COUNT; i++) {
      (void)printf("%2lu: Testing block %lu \r\n", i, blockAddress);
      randomizeBuffer(g_dataWrite, sizeof(g_dataWrite));
      state = SD_WriteBlocks(card, g_dataWrite, blockAddress, 1U);
      if (state == (int32_t)STATUS_SUCCESS) {
        (void)memset(g_dataRead, 0xEEU, sizeof(g_dataRead));
        state = SD_ReadBlocks(card, g_dataRead, blockAddress, 1U);
      } else {
        (void)printf("Write one data block failed.\r\n");
        errorCount++;
      }
      if (state == (int32_t)STATUS_SUCCESS) {
        state = SD_EraseBlocks(card, blockAddress, 1U);
      } else {
        (void)printf("Read one data block failed.\r\n");
        errorCount++;
      }
      /* Erase */
      if (state == (int32_t)STATUS_SUCCESS) {
        (void)memset(g_dataRead, 0xFFU, sizeof(g_dataRead));
        state = SD_ReadBlocks(card, g_dataRead, blockAddress, 1U);
      } else {
        (void)printf("Erase multiple data blocks failed.\r\n");
        errorCount++;
      }
      /* Verify content after erase */
      if (state != (int32_t)STATUS_SUCCESS) {
        (void)printf("Read multiple data blocks failed.\r\n");
        errorCount++;
      }
      if (state != (int32_t)STATUS_SUCCESS) {
        break;
      }
      /* Compare the content of g_dataRead and g_dataWrite */
      if (0 == memcmp(g_dataRead, g_dataWrite, DATA_DEFAULT_BLOCK_SIZE)) {
        errorCount++;
      }
      blockAddress++;
      if ((DATA_BLOCK_START + DATA_BLOCK_COUNT) <= blockAddress) {
        blockAddress = DATA_BLOCK_START;
      }
    }
    (void)uSDHC_DeInit();
  }
  if (0U == errorCount) {
    status = (common_status_t)STATUS_SUCCESS;
  } else {
    status = (common_status_t)STATUS_ERROR;
  }
  TEST_END
  if ((common_status_t)STATUS_SUCCESS == status) {
    (void)printf("PASS\r\n");
  } else {
    (void)printf("ERROR\r\n");
#ifndef ENABLE_TESTS
    while (1) {
    }
#endif
  }
}
