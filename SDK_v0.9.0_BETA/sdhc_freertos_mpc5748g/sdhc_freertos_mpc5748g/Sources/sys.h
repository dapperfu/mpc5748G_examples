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
**     Filename    : sys.h
**     Project     : sdhc_freertos_mpc5748g
**     Processor   : MPC5748G_324
**     Version     : 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   :
**     Abstract    :
**         This module contains FreeRTOS thread-related routines.
**     Settings    :
**     Contents    :
**
** ###################################################################*/

#ifndef SYS_H
#define SYS_H

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

/*!
 * @addtogroup SYS
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef TaskHandle_t sys_thread_t;

typedef BaseType_t sys_base_type_t;

typedef void (*sys_thread_function)(void *arg);

/* Wrap a thread method and its argument for being passed to run_thread_function */
typedef struct thread_function_wrapper
{
	char *name;
	sys_thread_function function;
	void *arg;
	sys_base_type_t prio;
} thread_function_wrapper_t;

/*************************************************************************************************
 * API
 ************************************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

sys_thread_t sys_thread_new(const char *name, thread_function_wrapper_t *thread_wrapper, sys_thread_function thread_function, void *arg, int stacksize, int prio);

void sys_thread_delete(sys_thread_t thread);

#if defined(__cplusplus)
}
#endif
/*! @} */
#endif /* SYS_H */
