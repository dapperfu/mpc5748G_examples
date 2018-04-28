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
**     Filename    : task_sdhc_init_deinit.c
**     Project     : sdhc_fatfs_mpc5748g
**     Processor   : MPC5748G_324
**     Version     : 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   :
**     Abstract    :
**         This module contains uSDHC driver initialization/de-initialization sequence test.
**     Settings    :
**     Contents    :
**
** ###################################################################*/

#include <stdbool.h>
#include <stdio.h>

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
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.8, The value of composite expression shall not be cast to a different
 *  essential type category or a wider essential type.
 * This is required casting randomized values.
 *
 */

/*******************************************************************************
* Global variables
*******************************************************************************/

/*******************************************************************************
* Constants and macros
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
 * Implements task_sdhc_init_deinit_Activity
 */
void task_sdhc_init_deinit(void)
{
	common_status_t status;
	(void)printf("uSDHC Init/Deinit test\r\n");
    status = uSDHC_Init(true);
    (void)printf("uSDHC/SD status: %ld\r\n", status);
    if((common_status_t)STATUS_SUCCESS == status)
    {
    	(void)uSDHC_DeInit();
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
