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
**     Filename    : task_sd_writeblocks_readblocks.c
**     Project     : sdhc_fatfs_mpc5748g
**     Processor   : MPC5748G_324
**     Version     : 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   :
**     Abstract    :
**         This module contains SD layer raw blocks read/write test.
**     Settings    :
**     Contents    :
**
** ###################################################################*/

#include <stdbool.h>
#include <float.h>
#include <stdio.h>
#include <string.h>

#include "osif.h"

#include "sd/card.h"
#include "usdhc/usdhc_impl.h"
#include "common/tasks/tasks.h"

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 21.6, The standard library input/output functions shall not be used.
 * This is required for printing messages.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 14.4, The controlling expression of an if statement and the controlling
 *  expression of an iteration-statement shall have essentially Boolean type.
 * This is required infinite loop.
 *
 */

/*******************************************************************************
* Constants and macros
*******************************************************************************/

#define LOOP_COUNT                  (50U)

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
 * Implements task_sd_writeblocks_readblocks_Activity
 */
void task_sd_writeblocks_readblocks(void)
{
	common_status_t status;
	uint32_t i = 0U;
	uint32_t blockAddress = DATA_BLOCK_START;
	uint32_t errorCount = 0U;
	sd_card_t *card;
	common_status_t state1 = (int32_t)STATUS_SUCCESS;
	common_status_t state2 = (int32_t)STATUS_SUCCESS;

	(void)printf("Write/Read test\r\n");
    status = uSDHC_Init(true);
    (void)printf("uSDHC/SD status: %ld\r\n", status);
    if((common_status_t)STATUS_SUCCESS == status)
    {
    	card = uSDHC_GetSDCard();
    	randomizeInit();
    	for(i = 0U; i < LOOP_COUNT; i++)
    	{
    		(void)printf("%2lu: Testing block %lu\r\n", i, blockAddress);
    		randomizeBuffer(g_dataWrite, sizeof(g_dataWrite));
    		state1 = SD_WriteBlocks(card, g_dataWrite, blockAddress, 1U);
    		if((common_status_t)STATUS_SUCCESS == state1)
    		{
        		(void)memset(g_dataRead, 0x00U, sizeof(g_dataRead));
        		state2 = SD_ReadBlocks(card, g_dataRead, blockAddress, 1U);
    		}
    		else
    		{
    			(void)printf("Write one data block failed.\r\n");
    			errorCount++;
    		}

    		if((common_status_t)STATUS_SUCCESS == state2)
    		{}
    		else
    		{
    			(void)printf("Read one data block failed.\r\n");
    			errorCount++;
    		}
    		if((state1  != (int32_t)STATUS_SUCCESS) || (state2  != (int32_t)STATUS_SUCCESS))
    		{
    			break;
    		}
    		/* Compare the content of g_dataRead and g_dataWrite */
    		if(0 != memcmp(g_dataRead, g_dataWrite, DATA_DEFAULT_BLOCK_SIZE))
    		{
    			errorCount++;
    		}
    		blockAddress++;
    		if ((DATA_BLOCK_START + DATA_BLOCK_COUNT) <= blockAddress)
    		{
    			blockAddress = DATA_BLOCK_START;
    		}
    	}
    	(void)uSDHC_DeInit();
    }
	if(0U == errorCount)
	{
		status = (common_status_t)STATUS_SUCCESS;
	}
	else
	{
		status = (common_status_t)STATUS_ERROR;
	}
	TEST_END
	if((common_status_t)STATUS_SUCCESS == status)
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

/*
 * Implements task_sd_writeread_speed_Activity
 */
void task_sd_writeread_speed(void)
{
	common_status_t status = (common_status_t)STATUS_ERROR;
	uint32_t i = 0U;
	uint32_t blockAddress;
	uint32_t errorCount = 0U;
	uint32_t tickCount = 0U;
	uint32_t tickDelta = 0U;
	float writeSpeed = 0;
	float readSpeed = 0;
	sd_card_t *card;
	printf("Write/Read speed test\r\n");
    status = uSDHC_Init(true);
    printf("uSDHC/SD status: %ld\r\n", status);
    if((common_status_t)STATUS_SUCCESS == status)
    {
    	card = uSDHC_GetSDCard();
    	randomizeInit();
    	blockAddress = DATA_BLOCK_START;
    	tickCount = OSIF_GetMilliseconds();
		randomizeBuffer(g_dataRead, sizeof(g_dataRead));
    	for(i = 0U; i < LOOP_COUNT; i++)
    	{
    		memcpy(g_dataWrite, g_dataRead, sizeof(g_dataWrite));
    		if((common_status_t)STATUS_SUCCESS != SD_WriteBlocks(card, g_dataWrite, blockAddress, DATA_BLOCK_COUNT))
    		{
    			printf("Write one data block failed.\r\n");
    			errorCount++;
    			break;
    		}
    		blockAddress += DATA_BLOCK_COUNT;
    	}
    	tickDelta = OSIF_GetMilliseconds() - tickCount;
    	writeSpeed = ((float)(LOOP_COUNT * DATA_BLOCK_COUNT * DATA_DEFAULT_BLOCK_SIZE)) / ((float)tickDelta / 1000U);
    	printf("Written %u octets in %.3f sec or %.3f B/s\r\n",
    			(LOOP_COUNT * DATA_BLOCK_COUNT * DATA_DEFAULT_BLOCK_SIZE),
				((float)tickDelta / 1000U), writeSpeed);
    	blockAddress = DATA_BLOCK_START;
    	tickCount = OSIF_GetMilliseconds();
		memset(g_dataRead, 0x00U, sizeof(g_dataRead));
    	for(i = 0U; i < LOOP_COUNT; i++)
    	{
			if((common_status_t)STATUS_SUCCESS != SD_ReadBlocks(card, g_dataRead, blockAddress, DATA_BLOCK_COUNT))
			{
				printf("Read one data block failed.\r\n");
				errorCount++;
				break;
			}
			if((LOOP_COUNT - 1 == i))
			{
				/* Compare the content of g_dataRead and g_dataWrite */
				if(memcmp(g_dataRead, g_dataWrite, DATA_BUFFER_SIZE))
				{
					errorCount++;
				}
			}
    		blockAddress += DATA_BLOCK_COUNT;
    	}
    	tickDelta = OSIF_GetMilliseconds() - tickCount;
    	readSpeed = ((float)(LOOP_COUNT * DATA_BLOCK_COUNT * DATA_DEFAULT_BLOCK_SIZE)) / ((float)tickDelta / 1000U);
    	printf("Read %u octets in %.3f sec or %.3f B/s\r\n",
    			(LOOP_COUNT * DATA_BLOCK_COUNT * DATA_DEFAULT_BLOCK_SIZE),
				((float)tickDelta / 1000U), readSpeed);
    	uSDHC_DeInit();
    }
	if(0U == errorCount)
	{
		status = (common_status_t)STATUS_SUCCESS;
	}
	else
	{
		status = (common_status_t)STATUS_ERROR;
	}
	TEST_END
	if((common_status_t)STATUS_SUCCESS == status)
	{
		printf("PASS\r\n");
	}
	else
	{
		printf("ERROR\r\n");
#ifndef ENABLE_TESTS
    	while(1) {};
#endif
	}
}
