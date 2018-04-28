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
**     Filename    : tasks.h
**     Project     : sdhc_fatfs_mpc5748g
**     Processor   : MPC5748G_324
**     Version     : 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   :
**     Abstract    :
**         This module contains different use-cases for SDHC/SD/FATFS.
**     Settings    :
**     Contents    :
**
** ###################################################################*/

#ifndef TASKS_TASKS_H_
#define TASKS_TASKS_H_

#include <stdbool.h>
#include <stdint.h>

#include "ff.h"

/*!
 * @addtogroup TASKS
 * @{
 */

#define PATH_LENGTH (256U)
#define DATA_BLOCK_COUNT 			(14U)
#define DATA_BLOCK_START 			(0U)
#define DATA_DEFAULT_BLOCK_SIZE		(512U)
#define DATA_BUFFER_SIZE 			(DATA_DEFAULT_BLOCK_SIZE * DATA_BLOCK_COUNT)

typedef enum _DISKIO_RESULT
{
	DISKIO_RESULT_OK = 0,
	DISKIO_RESULT_ERROR_INSUFFICIENT_AREA,
	DISKIO_RESULT_ERROR_INIT,
	DISKIO_RESULT_ERROR_WRITE_PROTECT,
	DISKIO_RESULT_ERROR_IOCTL_GET_SECTOR_COUNT,
	DISKIO_RESULT_ERROR_INSUFFICIENT_DISK_SPACE,
	DISKIO_RESULT_ERROR_IOCTL_GET_SECTOR_SIZE,
	DISKIO_RESULT_ERROR_IOCTL_GET_BLOCK_SIZE,
	DISKIO_RESULT_ERROR_READ_1,
	DISKIO_RESULT_ERROR_WRITE_1,
	DISKIO_RESULT_ERROR_DATA_INCONSISTENT_1,
	DISKIO_RESULT_ERROR_WRITE_MULTI,
	DISKIO_RESULT_ERROR_READ_MULTI,
	DISKIO_RESULT_ERROR_IOCTL_CTRL_SYNC_WRITE_1,
	DISKIO_RESULT_ERROR_IOCTL_CTRL_SYNC_WRITE_MULTI,
	DISKIO_RESULT_ERROR_DATA_INCONSISTENT_MULTI,
	DISKIO_RESULT_ERROR_WRITE_NOTALIGNED,
	DISKIO_RESULT_ERROR_IOCTL_CTRL_SYNC_WRITE_NOTALIGNED,
	DISKIO_RESULT_ERROR_READ_NOTALIGNED,
	DISKIO_RESULT_ERROR_DATA_INCONSISTENT_NOTALIGNED,
	DISKIO_RESULT_ERROR_WRITE_4GB_1,
	DISKIO_RESULT_ERROR_WRITE_4GB_2,
	DISKIO_RESULT_ERROR_IOCTL_CTRL_SYNC_4GB_1,
	DISKIO_RESULT_ERROR_READ_4GB_1,
	DISKIO_RESULT_ERROR_READ_4GB_2,
	DISKIO_RESULT_ERROR_DATA_INCONSISTENT_4GB
} DISKIO_RESULT;

#ifdef ENABLE_TESTS
#define TEST_END \
{ \
	goto END; \
    END: \
	{ \
		/* For testing purposes. */ \
	} \
}
#else
#define TEST_END
#endif

extern uint8_t g_dataWrite[DATA_BUFFER_SIZE];

extern uint8_t g_dataRead[DATA_BUFFER_SIZE];

/*************************************************************************************************
 * API
 ************************************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
* @brief
*
* uSDHC Init/Deinit test routine
*
* @param void.
* @retval void.
*/
void task_sdhc_init_deinit(void);

/*!
* @brief
*
* SD Write/Read test routine
*
* @param void.
* @retval void.
*/
void task_sd_writeblocks_readblocks(void);

/*!
* @brief
*
* SD Write/Read speed test routine
*
* @param void.
* @retval void.
*/
void task_sd_writeread_speed(void);

/*!
* @brief
*
* SD Erase/Read test routine
*
* @param void.
* @retval void.
*/
void task_sd_eraseblocks(void);

/*!
* @brief
*
* FATFS diskio test routine
*
* @param void.
* @retval void.
*/
void task_fatfs_diskio(void);

/*!
* @brief
*
* FATFS diskio routine
* It calls all tests from that TU.
*
* @param BYTE.
* @param UINT.
* @param DWORD *.
* @param INT.
* @retval DISKIO_RESULT.
*/
DISKIO_RESULT task_diskio_all(
		BYTE pdrv,      /* Physical drive number to be checked (all data on the drive will be lost) */
		UINT numCycles,     /* Number of test cycles */
		DWORD *buff,        /* Pointer to the working buffer */
		INT sz_buff         /* Size of the working buffer in unit of byte */
);

/*!
* @brief
*
* FATFS diskio task_diskio_init routine
* It tests disk_initialize() call.
*
* @param BYTE.
* @retval DISKIO_RESULT.
*/
DISKIO_RESULT task_diskio_init(
		BYTE pdrv      /* Physical drive number */
);

/*!
* @brief
*
* FATFS diskio task_diskio_get_drive_space routine
* It tests disk_ioctl(GET_SECTOR_COUNT)
*
* @param BYTE.
* @param DWORD.
* @retval DISKIO_RESULT.
*/
DISKIO_RESULT task_diskio_get_drive_space(
		BYTE pdrv,      /* Physical drive number */
		DWORD *sz_drv   /* Pointer to drive size in octets */
);

/*!
* @brief
*
* FATFS diskio task_diskio_get_sector_size routine
* It tests disk_ioctl(GET_SECTOR_SIZE)
*
* @param BYTE.
* @param WORD.
* @retval DISKIO_RESULT.
*/
DISKIO_RESULT task_diskio_get_sector_size(
		BYTE pdrv,      /* Physical drive number */
		WORD *sz_sect  /* Pointer to drive sector size in octets */
);

/*!
* @brief
*
* FATFS diskio task_diskio_get_erase_block_size routine
* It tests disk_ioctl(GET_BLOCK_SIZET)
*
* @param BYTE.
* @param DWORD.
* @retval DISKIO_RESULT.
*/
DISKIO_RESULT task_diskio_get_erase_block_size(
		BYTE pdrv,      /* Physical drive number */
		DWORD *sz_eblk  /* Pointer to drive erase sector size in octets */
);

/*!
* @brief
*
* FATFS diskio task_diskio_single_sector_write routine
* It tests single block write operation.
*
* @param BYTE.
* @param WORD.
* @param DWORD.
* @retval DISKIO_RESULT.
*/
DISKIO_RESULT task_diskio_single_sector_write(
		BYTE pdrv,       /* Physical drive number */
		WORD sz_sect,   /* Drive sector size in octets */
		DWORD *buff      /* Pointer to the working buffer */
);

/*!
* @brief
*
* FATFS diskio task_diskio_multiple_sector_write routine
* It tests multiple block write operation.
*
* @param BYTE.
* @param WORD.
* @param DWORD.
* @retval DISKIO_RESULT.
*/
DISKIO_RESULT task_diskio_multiple_sector_write(
		BYTE pdrv,       /* Physical drive number */
		WORD sz_sect,   /* Drive sector size in octets */
		DWORD *buff,     /* Pointer to the working buffer */
		INT sz_buff      /* Size of the working buffer in unit of byte */
);

/*!
* @brief
*
* FATFS diskio task_diskio_single_sector_write_unalign routine
* It tests single block write operation which accesses data in unaligned memory block.
*
* @param BYTE.
* @param WORD.
* @param DWORD.
* @retval DISKIO_RESULT.
*/
DISKIO_RESULT task_diskio_single_sector_write_unalign(
		BYTE pdrv,       /* Physical drive number */
		WORD sz_sect,   /* Drive sector size in octets */
		DWORD *buff     /* Pointer to the working buffer */
);

/*!
* @brief
*
* FATFS diskio task_diskio_4GB_barrier routine
* It tests diskio on 4 GB barrier problem.
*
* @param BYTE.
* @param DWORD.
* @param WORD.
* @param DWORD.
* @retval DISKIO_RESULT.
*/
DISKIO_RESULT task_diskio_4GB_barrier(
		BYTE pdrv,       /* Physical drive number */
		DWORD sz_drv,    /* Drive size in sectors */
		WORD sz_sect,   /* Drive sector size in octets */
		DWORD *buff     /* Pointer to the working buffer */
);

/*!
* @brief
*
* FATFS mkfs test routine
* It tests fatfs f_mkfs routine.
*
* @param void.
* @retval void.
*/
void task_fatfs_fdisk_mkfs(void);

/*!
* @brief
*
* FATFS f_open(FA_CREATE_ALWAYS) test routine
*
* @param void.
* @retval void.
*/
void task_fatfs_f_open_f_write(void);

/*!
* @brief
*
* FATFS make file copy test routine
*
* @param void.
* @retval void.
*/
void task_fatfs_copy_file(void);

/*!
* @brief
*
* FATFS f_open(FA_READ) test routine
*
* @param void.
* @retval void.
*/
void task_fatfs_f_open_f_read(void);

/*!
* @brief
*
* FATFS f_opendir/f_readdir test routine
*
* @param void.
* @retval void.
*/
void task_fatfs_f_opendir_f_readdir(void);

/*!
* @brief
*
* FATFS routine that calls srand()
*
* @param void.
* @retval void.
*/
void randomizeInit(void);

/*!
* @brief
*
* FATFS routine that calls rand() in order to randomize byte value.
* On flag toBeInitialized true, random generator is initialized.
*
* @param bool toBeInitialized.
* @retval void.
*/
uint8_t randomizeByte(bool toBeInitialized);

/*!
* @brief
*
* FATFS routine that calls rand() in order to fill buffer with randomized values.
*
* @param uint8_t data pointer to data buffer.
* @param uint32_t size data buffer size.
* @retval void.
*/
void randomizeBuffer(uint8_t *data, uint32_t size);

/*!
* @brief
*
* Print FILINFO FATFS structure content.
*
* @param char *path.
* @param FILINFO * pointer to FILINFO structure.
* @retval void.
*/
void printFileInfo(char *path, FILINFO *fst);

/*!
* @brief
*
* Lists files on given path.
*
* @param char *path
* @retval FRESULT
*/
FRESULT listFiles(char* path);

/*!
* @brief
*
* Creates file with randomized content.
*
* @param TCHAR filename path to file.
* @param uint32_t number of 4K blocks to be written.
* @retval FRESULT
*/
FRESULT createBinaryFile(TCHAR filename[], uint32_t fileBlocks);

#if defined(__cplusplus)
}
#endif
/*! @} */
#endif /* TASKS_TASKS_H_ */
