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
**     Filename    : task_fatfs_f_open_f_write.c
**     Project     : sdhc_fatfs_mpc5748g
**     Processor   : MPC5748G_324
**     Version     : 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   :
**     Abstract    :
**         This module contains FATFS layer create file test.
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
 * Violates MISRA 2012 Required Rule 8.3, All declarations of an object or
 * function shall use the same names and type qualifiers. This is required for
 * printing messages in files.
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
#define BINARY_FILE_COUNT (2U)

#define FILE_BLOCKS (393U)

/*******************************************************************************
 * Local types
 *******************************************************************************/

/*******************************************************************************
 * Local function prototypes
 *******************************************************************************/

static FRESULT createTextFile(TCHAR filename[]);

static FRESULT copyFile(TCHAR filename[], TCHAR outFilename[]);

/*******************************************************************************
 * Local variables
 *******************************************************************************/

/*******************************************************************************
 * Local functions
 *******************************************************************************/

static FRESULT createTextFile(TCHAR filename[]) {
  /* File object */
  FIL fd;
  /* File stat object */
  FILINFO fst;
  /* API result code */
  FRESULT status;
  static UINT bytesWritten;
  static UINT bytesToWrite;
  TCHAR path[PATH_LENGTH];
  TCHAR fileBuffer[] = "Sample file content\r\n";
  (void)printf("createTextFile\r\n");
  (void)sprintf(path, "%s%s", DRIVE0, filename);
  status = f_open(&fd, path, (BYTE)FA_WRITE | (BYTE)FA_CREATE_ALWAYS);
  if (FR_OK == status) {
    bytesToWrite = (UINT)sizeof(fileBuffer) - 1U;
    status = f_write(&fd, fileBuffer, bytesToWrite, &bytesWritten);
    (void)printf("Written byte(s): %u\r\n", bytesWritten);
    if ((FR_OK == status) && (bytesWritten != bytesToWrite)) {
      status = FR_INT_ERR;
    }
    if (FR_OK == status) {
      status = f_sync(&fd);
    }
    if (FR_OK == status) {
      status = f_stat(path, &fst);
    }
    if (FR_OK == status) {
      printFileInfo(path, &fst);
      status = f_close(&fd);
    } else {
      (void)f_close(&fd);
    }
  }
  return status;
}

static FRESULT copyFile(TCHAR filename[], TCHAR outFilename[]) {
  /* File object */
  FIL fd;
  /* File object */
  FIL copy_fd;
  /* File stat object */
  FILINFO fst;
  /* API result code */
  FRESULT status;
  UINT i;
  static UINT bytesWritten;
  static UINT bytesToWrite;
  UINT bytesToRead;
  TCHAR path[255];
  (void)printf("copyFile\r\n");
  (void)sprintf(path, "%s%s", DRIVE0, filename);
  status = f_stat(path, &fst);
  if (FR_OK == status) {
    status = f_open(&fd, path, FA_READ);
  }
  (void)sprintf(path, "%s%s", DRIVE0, outFilename);
  if (FR_OK == status) {
    status = f_open(&copy_fd, path, (BYTE)(FA_WRITE | FA_CREATE_ALWAYS));
  }
  if (FR_OK == status) {
    for (i = 0; i < fst.fsize; i++) {
      bytesToWrite = sizeof(g_dataWrite);
      bytesToRead = bytesToWrite;
      status = f_read(&fd, g_dataWrite, bytesToRead, &bytesToWrite);
      if (FR_OK == status) {
        bytesWritten = 0U;
        if (0U < bytesToWrite) {
          status = f_write(&copy_fd, g_dataWrite, bytesToWrite, &bytesWritten);
          if (FR_OK == status) {
            status = f_sync(&copy_fd);
          }
        }
      }
      if ((FR_OK != status) || (0U == bytesToWrite)) {
        break;
      }
    }
    if (FR_OK == status) {
      status = f_sync(&copy_fd);
    }
    if (FR_OK == status) {
      status = f_stat(path, &fst);
    }
    if (FR_OK == status) {
      (void)printf("fdate: %u ftime: %u\r\n", fst.fdate, fst.ftime);
      status = f_close(&copy_fd);
    } else {
      (void)f_close(&copy_fd);
      (void)f_close(&fd);
    }
  }
  return status;
}

/*******************************************************************************
 * Global functions
 *******************************************************************************/

FRESULT createBinaryFile(TCHAR filename[], uint32_t fileBlocks) {
  /* File object */
  FIL fd;
  /* File stat object */
  FILINFO fst;
  /* API result code */
  FRESULT status;
  UINT i;
  static UINT bytesWritten;
  UINT bytesToWrite;
  TCHAR path[255];

  (void)printf("createBinaryFile\r\n");
  (void)sprintf(path, "%s%s", DRIVE0, filename);
  status = f_open(&fd, path, (BYTE)(FA_WRITE | FA_CREATE_ALWAYS));
  if (FR_OK == status) {
    randomizeInit();
    bytesToWrite = sizeof(g_dataWrite);
    bytesWritten = 0U;
    for (i = 0; i < fileBlocks; i++) {
      randomizeBuffer((uint8_t *)&g_dataWrite[0], bytesToWrite);
      status = f_write(&fd, g_dataWrite, bytesToWrite, &bytesWritten);
      if (FR_OK == status) {
        status = f_sync(&fd);
      }
      if (FR_OK != status) {
        break;
      }
    }
    if (FR_OK == status) {
      status = f_sync(&fd);
    }
    if (FR_OK == status) {
      status = f_stat(path, &fst);
    }
    if (FR_OK == status) {
      printFileInfo(path, &fst);
      status = f_close(&fd);
    } else {
      (void)f_close(&fd);
    }
  }
  return status;
}

/*
 * Implements task_fatfs_f_open_f_write_Activity
 */
void task_fatfs_f_open_f_write(void) {
  /* Filesystem object */
  FATFS fs;
  /* API result code */
  FRESULT status;
  UINT i;
  TCHAR path[255];
  TCHAR string[] = DRIVE0;
  (void)printf("FATFS f_open(FA_CREATE_ALWAYS) test\r\n");
  /* Immediate mount. */
  status = f_mount(&fs, string, 1U);
  (void)printf("f_mount status: %d\r\n", status);
  if (FR_OK == status) {
    for (i = 0U; i < FILE_COUNT; i++) {
      (void)sprintf(path, "%s%u%s", "newFile", i, ".bin");
      if ((i > 0U) && (i < BINARY_FILE_COUNT)) {
        status = createBinaryFile(path, FILE_BLOCKS);
      } else {
        status = createTextFile(path);
      }
      if (FR_OK != status) {
        break;
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

/*
 * Implements task_fatfs_copy_file_Activity
 */
void task_fatfs_copy_file(void) {
  /* Filesystem object */
  FATFS fs;
  /* API result code */
  FRESULT status;
  UINT i;
  TCHAR path[255];
  TCHAR path2[255];
  TCHAR string[] = DRIVE0;
  (void)printf("FATFS task_fatfs_copy_file test\r\n");
  /* Immediate mount. */
  status = f_mount(&fs, string, 1U);
  (void)printf("f_mount status: %u\r\n", status);
  if (FR_OK == status) {
    for (i = 0U; i < BINARY_FILE_COUNT; i++) {
      (void)sprintf(path, "%s%u%s", "newFile", i, ".bin");
      (void)sprintf(path2, "%s%u%s", "newFile", i, "copy.bin");
      status = copyFile(path, path2);
      if (FR_OK != status) {
        break;
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
