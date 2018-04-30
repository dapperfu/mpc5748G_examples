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
**     Filename    : task_fatfs_diskio.c
**     Project     : sdhc_fatfs_mpc5748g
**     Processor   : MPC5748G_324
**     Version     : 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   :
**     Abstract    :
**         This module contains FATFS layer diskio tests.
**     Settings    :
**     Contents    :
**
** ###################################################################*/

#include "Cpu.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ff.h"
#include "diskio.h"
#include "common/tasks/tasks.h"

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 21.6, The standard library input/output functions shall not be used.
 * This is required for printing messages.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.9, Conversion from pointer to void into pointer to object.
 * This is need for DEV_ASSERT macro condition.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.3, All declarations of an object or function shall use the same
 * names and type qualifiers.
 * This is required for printing messages.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.4, A compatible declaration shall be visible when an object or
 * function with external linkage is defined
 * This is required for printing messages.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 14.4, The controlling expression of an if statement and the controlling
 *  expression of an iteration-statement shall have essentially Boolean type.
 * This is required infinite loop.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.1, External identifiers shall be distinct.
 * This is required for  gdb script.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.2, Identifiers declared in yhe same scope and name space shall be distinct.
 * This is required for  gdb script.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.4, Macro identifiers shall be distinct.
 * This is required for  gdb script.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.5, Identifiers shall be distinct from macro names.
 * This is required for  gdb script.
 *
 */

/*******************************************************************************
* Global variables
*******************************************************************************/

__attribute((section(".stack")))
uint8_t g_dataWrite[DATA_BUFFER_SIZE];

__attribute((section(".stack")))
uint8_t g_dataRead[DATA_BUFFER_SIZE];

/*******************************************************************************
* Constants and macros
*******************************************************************************/

/*! brief (1 << 31) */
#define FORTH_GB_BARRIER (0x80000000U)

#define DRIVE0_INST (0U)
#define LOOP_COUNT  (5U)

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

void randomizeInit(void)
{
	srand(0x10000);
}

void randomizeBuffer(uint8_t *data, uint32_t size)
{
	uint32_t i = 0U;
	for(i = 0U; i < size; i++)
	{
		data[i] = (uint8_t)(rand() % 255);
	}
}

uint8_t randomizeByte(bool toBeInitialized)
{
	if(true == toBeInitialized)
	{
		randomizeInit();
	}
	return (uint8_t)(rand() % 255);
}

/*
 * Implements task_diskio_init_Activity
 */
DISKIO_RESULT task_diskio_init(
		BYTE pdrv      /* Physical drive number */
)
{
	DSTATUS dstatus;
	DISKIO_RESULT status;
	/* Initialization */
	status = DISKIO_RESULT_OK;
	(void)printf(" disk_initialize(%u)", (UINT)pdrv);
	dstatus = disk_initialize(pdrv);
	if(0U != (dstatus & (BYTE)STA_NOINIT))
	{
		(void)printf(" - failed.\r\n");
		status = DISKIO_RESULT_ERROR_INIT;
	}
	else if(0U != (dstatus & (BYTE)STA_PROTECT))
	{
		(void)printf(" - failed due to write protection lock enabled.\r\n");
		status = DISKIO_RESULT_ERROR_WRITE_PROTECT;
	}
	else
	{
		(void)printf(" - ok.\r\n");
	}
	TEST_END
	return status;
}

/*
 * Implements task_diskio_get_drive_space_Activity
 */
DISKIO_RESULT task_diskio_get_drive_space(
		BYTE pdrv,      /* Physical drive number */
		DWORD *sz_drv   /* Pointer to drive size in octets */
)
{
	DEV_ASSERT(NULL != sz_drv);
	DRESULT dresult;
	DISKIO_RESULT status;
	/* Get drive space */
	status = DISKIO_RESULT_OK;
	(void)printf(" disk_ioctl(%u, GET_SECTOR_COUNT, 0x%08X)", (UINT)pdrv, (UINT)(*sz_drv));
	*sz_drv = 0U;
	dresult = disk_ioctl(pdrv, GET_SECTOR_COUNT, sz_drv);
	if(RES_OK == dresult)
	{
		if(128U > (UINT)(*sz_drv))
		{
			(void)printf(" - failed: Insufficient drive space to test.\r\n");
			status = DISKIO_RESULT_ERROR_INSUFFICIENT_DISK_SPACE;
		}
		else
		{
			(void)printf(" - ok.\r\n");
			(void)printf(" Number of sectors on the drive %u is %u.\r\n", (UINT)pdrv, (UINT)(*sz_drv));
		}
	}
	else
	{
		(void)printf(" - failed.\r\n");
		status = DISKIO_RESULT_ERROR_IOCTL_GET_SECTOR_COUNT;
	}
	TEST_END
	return status;
}

/*
 * Implements task_diskio_get_sector_size_Activity
 */
DISKIO_RESULT task_diskio_get_sector_size(
		BYTE pdrv,      /* Physical drive number */
		WORD *sz_sect   /* Pointer to drive sector size in octets */
)
{
	DEV_ASSERT(NULL != sz_sect);
	DISKIO_RESULT status = DISKIO_RESULT_OK;
#if FF_MAX_SS != FF_MIN_SS
	DRESULT dresult;
	/* Get sector size */
	printf(" disk_ioctl(%u, GET_SECTOR_SIZE, 0x%X)", pdrv, (UINT)(*sz_sect));
	*sz_sect = 0U;
	dresult = disk_ioctl(pdrv, GET_SECTOR_SIZE, sz_sect);
	if(RES_OK == dresult)
	{
		printf(" - ok.\r\n");
		printf(" Size of sector is %u bytes.\r\n", (UINT)(*sz_sect));
	}
	else
	{
		printf(" - failed.\r\n");
		status = DISKIO_RESULT_ERROR_IOCTL_GET_SECTOR_SIZE;
	}
#else
	(void)pdrv;
	*sz_sect = FF_MAX_SS;
#endif
	TEST_END
	return status;
}

/*
 * Implements task_diskio_get_erase_block_size_Activity
 */
DISKIO_RESULT task_diskio_get_erase_block_size(
		BYTE pdrv,      /* Physical drive number */
		DWORD *sz_eblk  /* Pointer to drive erase sector size in octets */
)
{
	DEV_ASSERT(NULL != sz_eblk);
	DRESULT dresult;
	DISKIO_RESULT status = DISKIO_RESULT_OK;
	/* Get erase block size */
	(void)printf(" disk_ioctl(%u, GET_BLOCK_SIZE, 0x%X)", (UINT)pdrv, (UINT)(*sz_eblk));
	*sz_eblk = 0U;
	dresult = disk_ioctl(pdrv, GET_BLOCK_SIZE, sz_eblk);
	if(RES_OK == dresult)
	{
		(void)printf(" - ok.\r\n");
	}
	else
	{
		(void)printf(" - failed.\r\n");
		status = DISKIO_RESULT_ERROR_IOCTL_GET_BLOCK_SIZE;
	}
	if((RES_OK == dresult) || (2U <= (UINT)(*sz_eblk)))
	{
		(void)printf(" Size of the erase block is %u sectors.\r\n", (UINT)(*sz_eblk));
	}
	else
	{
		(void)printf(" Size of the erase block is unknown.\r\n");
	}
	TEST_END
	return status;
}

/*
 * Implements task_diskio_single_sector_write_Activity
 */
DISKIO_RESULT task_diskio_single_sector_write(
		BYTE pdrv,       /* Physical drive number */
		WORD sz_sect,    /* Drive sector size in octets */
		DWORD *buff      /* Pointer to the working buffer */
)
{
	DEV_ASSERT(NULL != buff);
	DWORD lba;
	UINT n;
	DRESULT dresult;
	DISKIO_RESULT status = DISKIO_RESULT_OK;
	BYTE *pbuff = (BYTE *)buff;
	/* Single sector write test */
	lba = 0U;
	(void)randomizeByte(true);
	for(n = 0U; n < sz_sect; n++)
	{
		pbuff[n] = (BYTE)randomizeByte(false);
	}
	(void)printf("Single sector write\r\n");
	(void)printf(" disk_write(%u, 0x%p, %lu, 1)", (UINT)pdrv, pbuff, lba);
	dresult = disk_write(pdrv, pbuff, lba, 1U);
	if(RES_OK == dresult)
	{
		(void)printf(" - ok.\r\n");
		(void)printf(" disk_ioctl(%u, CTRL_SYNC, NULL)", (UINT)pdrv);
		dresult = disk_ioctl(pdrv, CTRL_SYNC, NULL);
		if(RES_OK == dresult)
		{
			(void)printf(" - ok.\r\n");
			(void)memset(pbuff, 0U, (DWORD)sz_sect);
			(void)printf(" disk_read(%u, 0x%p, %lu, 1)", (UINT)pdrv, pbuff, lba);
			dresult = disk_read(pdrv, pbuff, lba, 1U);
			if(RES_OK == dresult)
			{
				(void)printf(" - ok.\r\n");
				(void)randomizeByte(true);
				for(n = 0U; n < sz_sect; )
				{
					if(pbuff[n] == (BYTE)randomizeByte(false))
					{
						n++;
					}
					else
					{
						break;
					}
				}
				if(n == (UINT)sz_sect)
				{
					(void)printf(" Test data matched.\r\n");
				}
				else
				{
					(void)printf(" - failed: Read data differs from the data written.\r\n");
					status = DISKIO_RESULT_ERROR_DATA_INCONSISTENT_1;
				}
			}
			else
			{
				(void)printf(" - failed.\r\n");
				status = DISKIO_RESULT_ERROR_READ_1;
			}
		}
		else
		{
			(void)printf(" - failed.\r\n");
			status = DISKIO_RESULT_ERROR_IOCTL_CTRL_SYNC_WRITE_1;
		}
	}
	else
	{
		(void)printf(" - failed.\r\n");
		status = DISKIO_RESULT_ERROR_WRITE_1;
	}
	TEST_END
	return status;
}

/*
 * Implements task_diskio_multiple_sector_write_Activity
 */
DISKIO_RESULT task_diskio_multiple_sector_write(
		BYTE pdrv,       /* Physical drive number */
		WORD sz_sect,    /* Drive sector size in octets */
		DWORD *buff,     /* Pointer to the working buffer */
		INT sz_buff      /* Size of the working buffer in unit of byte */
)
{
	DEV_ASSERT(NULL != buff);
	DEV_ASSERT(0U < sz_sect);
	DWORD lba;
	UINT n;
	UINT ns;
	DRESULT dresult;
	DISKIO_RESULT status = DISKIO_RESULT_OK;
	BYTE *pbuff = (BYTE *)buff;
	/* Multiple sector write test */
	lba = 1U;
	ns = (UINT)(sz_buff) / sz_sect;
	if((UINT)4 < ns)
	{
		ns = 4;
	}
	(void)randomizeByte(true);
	for(n = 0U; n < (UINT)(sz_sect * ns); n++)
	{
		pbuff[n] = (BYTE)randomizeByte(false);
	}
	(void)printf("Multiple sector write\r\n");
	(void)printf(" disk_write(%u, 0x%p, %lu, %u)", (UINT)pdrv, pbuff, lba, ns);
	dresult = disk_write(pdrv, pbuff, lba, ns);
	if(RES_OK == dresult)
	{
		(void)printf(" - ok.\r\n");
		(void)printf(" disk_ioctl(%u, CTRL_SYNC, NULL)", (UINT)pdrv);
		dresult = disk_ioctl(pdrv, CTRL_SYNC, NULL);
		if(RES_OK == dresult)
		{
			(void)printf(" - ok.\r\n");
			(void)memset(pbuff, 0U, (DWORD)(sz_sect * ns));
			(void)printf(" disk_read(%u, 0x%p, %lu, %u)", (UINT)pdrv, pbuff, lba, ns);
			dresult = disk_read(pdrv, pbuff, lba, ns);
			if(RES_OK == dresult)
			{
				(void)printf(" - ok.\r\n");
				(void)randomizeByte(true);
				for(n = 0U; n < ((UINT)sz_sect * ns); )
				{
					if(pbuff[n] == (BYTE)randomizeByte(false))
					{
						n++;
					}
					else
					{
						break;
					}
				}
				if(n == (UINT)(sz_sect * ns))
				{
					(void)printf(" Test data matched.\r\n");
				}
				else
				{
					(void)printf(" - failed: Read data differs from the data written.\r\n");
					status = DISKIO_RESULT_ERROR_DATA_INCONSISTENT_MULTI;
				}
			}
			else
			{
				(void)printf(" - failed.\r\n");
				status = DISKIO_RESULT_ERROR_READ_MULTI;
			}
		}
		else
		{
			(void)printf(" - failed.\r\n");
			status = DISKIO_RESULT_ERROR_IOCTL_CTRL_SYNC_WRITE_MULTI;
		}
	}
	else
	{
		(void)printf(" - failed.\r\n");
		status = DISKIO_RESULT_ERROR_WRITE_MULTI;
	}
	TEST_END
	return status;
}

/*
 * Implements task_diskio_single_sector_write_unalign_Activity
 */
DISKIO_RESULT task_diskio_single_sector_write_unalign(
		BYTE pdrv,       /* Physical drive number */
		WORD sz_sect,    /* Drive sector size in octets */
		DWORD *buff      /* Pointer to the working buffer */
)
{
	DEV_ASSERT(NULL != buff);
	DWORD lba;
	UINT n;
	DRESULT dresult;
	DISKIO_RESULT status = DISKIO_RESULT_OK;
	BYTE *pbuff = (BYTE *)buff;
	/* Single sector write test (unaligned memory address) */
	lba = 5U;
	(void)randomizeByte(true);
	for(n = 0U; n < sz_sect; n++)
	{
		pbuff[n + 3U] = (BYTE)randomizeByte(false);
	}
	(void)printf("Single sector write unaligned\r\n");
	(void)printf("disk_write(%u, 0x%p, %lu, 1)", (UINT)pdrv, &pbuff[3], lba);
	dresult = disk_write(pdrv, &pbuff[3], lba, 1U);
	if(RES_OK == dresult)
	{
		(void)printf(" - ok.\r\n");
		(void)printf(" disk_ioctl(%u, CTRL_SYNC, NULL)", (UINT)pdrv);
		dresult = disk_ioctl(pdrv, CTRL_SYNC, NULL);
		if(RES_OK == dresult)
		{
			(void)printf(" - ok.\r\n");
			(void)memset( &pbuff[5], 0U, (DWORD)sz_sect);
			(void)printf(" disk_read(%u, 0x%p, %lu, 1)", (UINT)pdrv, &pbuff[5], lba);
			dresult = disk_read(pdrv, &pbuff[5], lba, 1U);
			if(RES_OK == dresult)
			{
				(void)printf(" - ok.\r\n");
				(void)randomizeByte(true);
				for(n = 0U; n < sz_sect; )
				{
					if(pbuff[n + 5U] == (BYTE)randomizeByte(false))
					{
						n++;
					}
					else
					{
						break;
					}
				}
				if(n == (UINT)sz_sect)
				{
					(void)printf(" Test data matched.\r\n");
				}
				else
				{
					(void)printf(" - failed: Read data differs from the data written.\r\n");
					status = DISKIO_RESULT_ERROR_DATA_INCONSISTENT_NOTALIGNED;
				}
			}
			else
			{
				(void)printf(" - failed.\r\n");
				status = DISKIO_RESULT_ERROR_READ_NOTALIGNED;
			}
		}
		else
		{
			(void)printf(" - failed.\r\n");
			status = DISKIO_RESULT_ERROR_IOCTL_CTRL_SYNC_WRITE_NOTALIGNED;
		}
	}
	else
	{
		(void)printf(" - failed.\r\n");
		status = DISKIO_RESULT_ERROR_WRITE_NOTALIGNED;
	}
	TEST_END
	return status;
}

/*
 * Implements task_diskio_4GB_barrier_Activity
 */
DISKIO_RESULT task_diskio_4GB_barrier(
		BYTE pdrv,       /* Physical drive number */
		DWORD sz_drv,    /* Drive size in sectors */
		WORD sz_sect,    /* Drive sector size in octets */
		DWORD *buff      /* Pointer to the working buffer */
)
{
	DEV_ASSERT(NULL != buff);
	DEV_ASSERT(0U < sz_sect);
	DWORD lba;
	DWORD lba2;
	UINT n;
	DRESULT dresult;
	DISKIO_RESULT status = DISKIO_RESULT_OK;
	BYTE *pbuff = (BYTE *)buff;
	/* 4GB barrier test */
	if(sz_drv >= (128U + (FORTH_GB_BARRIER / ((DWORD)sz_sect / 2U))))
	{
		lba = 6U;
		lba2 = lba + (FORTH_GB_BARRIER / ((DWORD)sz_sect / 2U));
		(void)randomizeByte(true);
		for(n = 0U; n < ((UINT)sz_sect * 2U); n++)
		{
			pbuff[n] = (BYTE)randomizeByte(false);
		}
		(void)printf("4GB barrier \r\n");
		(void)printf("disk_write(%u, 0x%p, %lu, 1)", (UINT)pdrv, pbuff, lba);
		dresult = disk_write(pdrv, pbuff, lba, 1U);
		if(RES_OK == dresult)
		{
			(void)printf(" - ok.\r\n");
			(void)printf(" disk_write(%u, 0x%p, %lu, 1)", (UINT)pdrv, &pbuff[sz_sect], lba2);
			dresult = disk_write(pdrv, &pbuff[sz_sect], lba2, 1U);
			if(RES_OK == dresult)
			{
				(void)printf(" - ok.\r\n");
				(void)printf(" disk_ioctl(%u, CTRL_SYNC, NULL)", (UINT)pdrv);
				dresult = disk_ioctl(pdrv, CTRL_SYNC, NULL);
				if(RES_OK == dresult)
				{
					(void)printf(" - ok.\r\n");
					(void)memset(pbuff, 0U, ((DWORD)(sz_sect) * 2U));
					(void)printf(" disk_read(%u, 0x%p, %lu, 1)", (UINT)pdrv, pbuff, lba);
					dresult = disk_read(pdrv, pbuff, lba, 1U);
					if(RES_OK == dresult)
					{
						(void)printf(" - ok.\r\n");
						(void)printf(" disk_read(%u, 0x%p, %lu, 1)", (UINT)pdrv, &pbuff[sz_sect], lba2);
						dresult = disk_read(pdrv,  &pbuff[sz_sect], lba2, 1U);
						if(RES_OK == dresult)
						{
							(void)printf(" - ok.\r\n");
							(void)randomizeByte(true);
							for(n = 0U; (pbuff[n] == (BYTE)randomizeByte(false)) && (n < ((UINT)sz_sect * 2U)); n++) {};
							if(n == ((UINT)sz_sect * 2U))
							{
								(void)printf("Test data matched.\r\n");
							}
							else
							{
								(void)printf("Failed: Read data differs from the data written.\r\n");
								status = DISKIO_RESULT_ERROR_DATA_INCONSISTENT_4GB;
							}
						}
						else
						{
							(void)printf(" - failed.\r\n");
							status = DISKIO_RESULT_ERROR_READ_4GB_2;
						}
					}
					else
					{
						(void)printf(" - failed.\r\n");
						status = DISKIO_RESULT_ERROR_READ_4GB_1;
					}
				}
				else
				{
					(void)printf(" - failed.\r\n");
					status = DISKIO_RESULT_ERROR_IOCTL_CTRL_SYNC_4GB_1;
				}
			}
			else
			{
				(void)printf(" - failed.\r\n");
				status = DISKIO_RESULT_ERROR_WRITE_4GB_2;
			}
		}
		else
		{
			(void)printf(" - failed.\r\n");
			status = DISKIO_RESULT_ERROR_WRITE_4GB_1;
		}
	}
	else
	{
		(void)printf(" 4GB barrier test skipped since current disk size is less than 4GB.\r\n");
	}
	TEST_END
	return status;
}

DISKIO_RESULT task_diskio_all(
		BYTE pdrv,      /* Physical drive number to be checked (all data on the drive will be lost) */
		UINT numCycles,     /* Number of test cycles */
		DWORD *buff,        /* Pointer to the working buffer */
		INT sz_buff         /* Size of the working buffer in unit of byte */
)
{
	UINT cycleCount;
	DWORD sz_drv;
	DWORD sz_eblk;
	WORD sz_sect;
	DISKIO_RESULT status;

	status = DISKIO_RESULT_OK;
	printf(" task_diskio_all(%u, %u, 0x%08X, 0x%08X)\r\n", pdrv, numCycles, (UINT)buff, sz_buff);
	if(sz_buff < (FF_MAX_SS + 4))
	{
		printf("Insufficient work area.\r\n");
		status = DISKIO_RESULT_ERROR_INSUFFICIENT_AREA;
	}
	else
	{
		for(cycleCount = 1U; cycleCount <= numCycles; cycleCount++)
		{
			status = task_diskio_init(pdrv);
			if(DISKIO_RESULT_OK != status)
			{
				break;
			}
			status = task_diskio_get_drive_space(pdrv, &sz_drv);
			if(DISKIO_RESULT_OK != status)
			{
				break;
			}
			status = task_diskio_get_sector_size(pdrv, &sz_sect);
			if(DISKIO_RESULT_OK != status)
			{
				break;
			}
			status = task_diskio_get_erase_block_size(pdrv, &sz_eblk);
			if(DISKIO_RESULT_OK != status)
			{
				break;
			}
			status = task_diskio_single_sector_write(pdrv, sz_sect, buff);
			if(DISKIO_RESULT_OK != status)
			{
				break;
			}
			status = task_diskio_multiple_sector_write(pdrv, sz_sect, buff, sz_buff);
			if(DISKIO_RESULT_OK != status)
			{
				break;
			}
			status = task_diskio_single_sector_write_unalign(pdrv, sz_sect, buff);
			if(DISKIO_RESULT_OK != status)
			{
				break;
			}
			status = task_diskio_4GB_barrier(pdrv, sz_drv, sz_sect, buff);
			if(DISKIO_RESULT_OK != status)
			{
				break;
			}
			printf("Test cycle %u of %u completed\r\n", cycleCount, numCycles);
		}
	}
	return status;
}

/*
 * Implements task_fatfs_diskio_Activity
 */
void task_fatfs_diskio(void)
{
	/* diskio result code */
	DISKIO_RESULT status;
	(void)printf("FATFS diskio tests\r\n");
	status = task_diskio_all((BYTE)DRIVE0_INST, (UINT)LOOP_COUNT, (DWORD *)g_dataWrite, (INT)sizeof(g_dataWrite));
	(void)printf("diskio tests result: %u\r\n", (UINT)status);
	if(DISKIO_RESULT_OK == status)
	{
		(void)printf("PASS\r\n");
	}
	else
	{
		(void)printf("ERROR\r\n");
#ifndef ENABLE_TESTS
    	while(1) {}
#endif
	}
}
