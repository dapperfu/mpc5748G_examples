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
**     Filename    : task_fatfs_fdisk_mkfs.c
**     Project     : sdhc_fatfs_mpc5748g
**     Processor   : MPC5748G_324
**     Version     : 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   :
**     Abstract    :
**         This module contains FATFS layer initialization test.
**     Settings    :
**     Contents    :
**
** ###################################################################*/

#include "Cpu.h"

#include <stdio.h>

#include "common/tasks/tasks.h"
#include "ff.h"

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 21.6, The standard library input/output
 * functions shall not be used. This is required for printing messages.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.9, Conversion from pointer to void into
 * pointer to object. This is need for DEV_ASSERT macro condition.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 14.4, The controlling expression of an if
 * statement and the controlling expression of an iteration-statement shall have
 * essentially Boolean type. This is required infinite loop.
 *
 */

/*******************************************************************************
 * Global variables
 *******************************************************************************/

/*******************************************************************************
 * Constants and macros
 *******************************************************************************/

#define DRIVE0 "0:"
#define DRIVE0_INST (0U)

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
 * Implements task_fatfs_fdisk_mkfs_Activity
 */
void task_fatfs_fdisk_mkfs(void) {
  /* Filesystem object */
  static FATFS fs;
  /* Divide drive into one partition */
  static DWORD partitionList[] = {99U, 1U, 0U, 0U};
  /* Working area */
  static ALIGNED(4096) BYTE fdiskWorkArea[FF_MAX_SS * 4] = {0U};
  static ALIGNED(4096) BYTE mkfsWorkArea[FF_MAX_SS * 8] = {0U};
  /* File function return code */
  FRESULT status;
  static const TCHAR string[] = DRIVE0;

  (void)printf("FATFS f_fdisk/f_mkfs test\r\n");
  (void)printf("This test may take a few minutes...\r\n");
  status = f_fdisk(DRIVE0_INST, partitionList, fdiskWorkArea);
  (void)printf("f_fdisk status: %u\r\n", status);
  if (FR_OK == status) {
    /* Create FAT volume on drive */
    status =
        f_mkfs(string, FM_FAT32, FF_MAX_SS, mkfsWorkArea, sizeof(mkfsWorkArea));
    (void)printf("f_mkfs status: %u\r\n", status);
    if (FR_OK == status) {
      /* Immediate mount. */
      status = f_mount(&fs, string, 1U);
      (void)printf("f_mount status: %u\r\n", status);
      if (FR_OK == status) {
        (void)f_unmount(string);
      }
    }
  }
  TEST_END
  if (FR_OK == status) {
    (void)printf("PASS\r\n");
  } else {
    (void)printf("ERROR\r\n");
#ifndef ENABLE_TESTS
    while (1) {
    }
#endif
  }
}
