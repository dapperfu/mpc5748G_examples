/*****************************************************************************/
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
**     Filename    : sys.c
**     Project     : sdhc_freertos_mpc5748g
**     Processor   : MPC5748G_324
**     Version     : 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   :
**     Abstract    :
**         This module contains FreeRTOS-related routines (imported from lwip stack and adopted for the needs).
**     Settings    :
**     Contents    :
**
** ###################################################################*/

#include "Cpu.h"

#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#include "sys.h"

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

/*!
 * @brief A routine that wraps a thread function call and provides a way
 * to add initialization - e.g. per-thread semaphores
 *
 * @param void * pointer to void.
 * @retval void.
 */
static void run_thread_function(void *arg);

/*******************************************************************************
* Local variables
*******************************************************************************/

/*******************************************************************************
* Local functions
*******************************************************************************/

static void run_thread_function(void *arg)
{
	thread_function_wrapper_t *t =
			(thread_function_wrapper_t *)arg;
	t->function(t->arg);
	/* Regular FreeRTOS tasks don't return. But in case they do, make them exit cleanly. */
	sys_thread_delete(NULL);
}

/*******************************************************************************
* Global functions
*******************************************************************************/

/*!
 * @brief A routine that starts a new thread.
 * The id of the new thread is returned.
 * Both the id and the priority are system dependent.
 *
 * @param name thread name.
 * @param thread_wrapper a pointer to thread wrapper structure.
 * @param thread_function a pointer to function.
 * @param arg a pointer to user-defined structure.
 * @param stack_size size of thread stack to be created.
 * @param prio thread priority.
 * @retval sys_thread_t new created thread structure.
 */
sys_thread_t sys_thread_new(const char *name, thread_function_wrapper_t *thread_wrapper, sys_thread_function thread_function, void *arg, int stacksize, int prio)
{
	sys_thread_t createdTask;
	BaseType_t creationResult;

	createdTask = NULL;
	thread_wrapper->name = (char *)name;
	thread_wrapper->function = thread_function;
	thread_wrapper->prio = prio;
	thread_wrapper->arg = arg;

	if (0U == stacksize)
	{
		stacksize = configMINIMAL_STACK_SIZE;
	}

	creationResult = xTaskCreate(run_thread_function, thread_wrapper->name, (uint16_t)stacksize, thread_wrapper, thread_wrapper->prio, &createdTask);
	if ((BaseType_t)1 != creationResult)
	{
		DEV_ASSERT(false);
	}
	return createdTask;
}

/*!
 * @brief A routine that frees a thread resource.
 *
 * @param thread thread structure.
 * @retval void.
 */
void sys_thread_delete(sys_thread_t thread)
{
    vTaskDelete(thread);
}
