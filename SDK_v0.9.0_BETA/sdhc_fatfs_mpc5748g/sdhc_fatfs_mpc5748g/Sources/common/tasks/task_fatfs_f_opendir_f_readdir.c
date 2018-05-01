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
**     Filename    : task_fatfs_f_opendir_f_readdir.c
**     Project     : sdhc_fatfs_mpc5748g
**     Processor   : MPC5748G_324
**     Version     : 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   :
**     Abstract    :
**         This module contains FATFS layer directory access test.
**     Settings    :
**     Contents    :
**
** ###################################################################*/

#include "Cpu.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "common/fatfs_impl.h"
#include "common/tasks/tasks.h"
#include "ff.h"
#include "rtc/rtc_timer.h"

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
 * @section [global]
 * Violates MISRA 2012 Required Rule 18.2, Substarction between pointers shall
 * only be applied to pointers that address elements of the same array. This is
 * need for printing an array.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 18.3, CThe relational operators >, >=, <,
 * <= shall not be applied to objects of pointerstype except where tey point
 * into the same object. This is need for printing an array.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 12.1, The precedence of operators within
 * expressions should be made explicit. This is need for printing an array.
 *
 */

/*******************************************************************************
 * Global variables
 *******************************************************************************/

/*******************************************************************************
 * Constants and macros
 *******************************************************************************/

#define DRIVE0 "0:"

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

void printFileInfo(char *path, FILINFO *fst) {
  static DWORD ftm;
  static rtc_timedate_t ftimeDate;
  const char *tPath = NULL;

  (void)tPath;
  tPath = strrchr(path, '/');
  if (NULL == tPath) {
    tPath = path;
  }

  if (0U != (fst->fattrib & (BYTE)AM_DIR)) {
    (void)printf("d ");
  } else {
    (void)printf("f ");
  }

  if (0U != (fst->fattrib & (BYTE)AM_RDO)) {
    (void)printf("r-");
  } else {
    (void)printf("rw");
  }

  if (0U != (fst->fattrib & (BYTE)AM_HID)) {
    (void)printf(" hid ");
  } else {
    (void)printf(" noh ");
  }

  if (0U != (fst->fattrib & (BYTE)AM_SYS)) {
    (void)printf(" sys ");
  } else {
    (void)printf(" reg ");
  }
  (void)printf(" %lu octets ", fst->fsize);
  ftm = ((DWORD)(fst->fdate << 16) | (DWORD)(fst->ftime));
  convertFFTimeToRTCTimeDate(ftm, &ftimeDate);
  (void)printf(" %04d-%02d-%02d %02d:%02d:%02d ", (int)ftimeDate.year,
               (int)ftimeDate.month, (int)ftimeDate.day, (int)ftimeDate.hour,
               (int)ftimeDate.minutes, (int)ftimeDate.seconds);
  (void)printf("%.*s%s%s\n", tPath - path, path, (tPath - path > 0) ? "/" : "",
               fst->fname);
}

FRESULT listFiles(char *path) {
  /* Directory object */
  DIR dir;
  /* File stat object */
  FILINFO fst;
  void *p = &fst;
  FRESULT status;
  DWORD filesNumber = 0U;
  QWORD filesBytes = 0U;
  status = f_opendir(&dir, path);
  if (FR_OK == status) {
    do {
      (void)memset(p, 0U, sizeof(FILINFO));
      status = f_readdir(&dir, &fst);
      if (FR_OK == status) {
        filesNumber++;
        filesBytes += fst.fsize;
        if ((TCHAR)0 == fst.fname[0]) {
          break;
        }
        printFileInfo(path, &fst);
      }
    } while (FR_OK == status);
    (void)printf("File(s): %lu, Space: %llu octets\r\n", filesNumber,
                 filesBytes);
    if (FR_OK == status) {
      status = f_closedir(&dir);
    } else {
      (void)f_closedir(&dir);
    }
  }
  return status;
}

/*
 * Implements task_fatfs_f_opendir_f_readdir_Activity
 */
void task_fatfs_f_opendir_f_readdir(void) {
  /* Filesystem object */
  FATFS fs;
  static const TCHAR string[] = DRIVE0;
  static TCHAR root_dir[] = "/";
  /* API result code */
  FRESULT status;
  (void)printf("FATFS f_opendir()/f_readdir() test\r\n");
  /* Immediate mount. */
  status = f_mount(&fs, string, 1U);
  (void)printf("f_mount status: %d\r\n", status);
  if (FR_OK == status) {
    status = listFiles(root_dir);
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
