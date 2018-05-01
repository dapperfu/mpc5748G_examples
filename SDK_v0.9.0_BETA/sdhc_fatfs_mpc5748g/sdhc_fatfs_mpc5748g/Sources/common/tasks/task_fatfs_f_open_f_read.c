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
**     Filename    : task_fatfs_f_open_f_read.c
**     Project     : sdhc_fatfs_mpc5748g
**     Processor   : MPC5748G_324
**     Version     : 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   :
**     Abstract    :
**         This module contains FATFS layer read file test.
**     Settings    :
**     Contents    :
**
** ###################################################################*/

#include "Cpu.h"

#include <stdio.h>
#include <string.h>

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
#define FILE_COUNT (200U)

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
 * Implements task_fatfs_f_open_f_read_Activity
 */
void task_fatfs_f_open_f_read(void) {
  /* Filesystem object */
  FATFS fs;
  /* File object */
  FIL fd;
  /* File stat object */
  FILINFO fst;

  /* API result code */
  FRESULT status;
  UINT i;
  static UINT bytesRead;
  UINT totalBytesRead;
  static TCHAR path[255];
  (void)printf("FATFS f_open(FA_READ) test\r\n");
  static const TCHAR string[] = DRIVE0;
  /* Immediate mount. */
  status = f_mount(&fs, string, 1U);
  (void)printf("f_mount status: %u\r\n", status);
  if (FR_OK == status) {
    for (i = 0U; i < FILE_COUNT; i++) {
      (void)sprintf(path, "%s%u%s", "newFile", i, ".bin");
      status = f_open(&fd, path, FA_READ);
      if (FR_OK == status) {
        totalBytesRead = 0U;
        do {
          (void)memset(g_dataRead, 0U, sizeof(g_dataRead));
          bytesRead = 0U;
          status = f_read(&fd, g_dataRead, sizeof(g_dataRead), &bytesRead);
          if (FR_OK != status) {
            break;
          }
          totalBytesRead += bytesRead;
        } while (sizeof(g_dataRead) == bytesRead);
        (void)printf("Read byte(s): %u\r\n", totalBytesRead);
        if (FR_OK == status) {
          status = f_stat(path, &fst);
        }
        if (FR_OK == status) {
          status = f_close(&fd);
        } else {
          (void)f_close(&fd);
        }
      }
    }
    (void)f_unmount(string);
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
