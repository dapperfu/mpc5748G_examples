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
**     Filename    : run.c
**     Project     : sdhc_freertos_mpc5748g
**     Processor   : MPC5748G_324
**     Version     : 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   :
**     Abstract    :
**         This module contains FreeRTOS tasks.
**     Settings    :
**     Contents    :
**
** ###################################################################*/

#include "Cpu.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "osif.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#include "project.h"
#include "setup.h"
#include "run.h"
#include "sys.h"
#include "rtc/rtc_timer.h"

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.9, Conversion from pointer to void into pointer to object.
 * This is need for DEV_ASSERT macro condition.
 *
 */

/*******************************************************************************
* Constants and macros
*******************************************************************************/
/* MAX_NUM_TASKS = maximum number tasks allowed to be created */
#define MAX_NUM_TASKS (10)
#define NUM_TASKS (9)
#define FATFS_THREAD0_WRAPPER_IND (0)

/* Priorities at which the tasks are created. */
#define MAIN_QUEUE_SEND_TASK_PRIORITY (tskIDLE_PRIORITY + 1)

/*!
 * @brief Stack sizes for the following threads: main loop thread, user FATFS thread(s), scheduler, timer.
 * Measurement unity is word.
 */
#define FATFS_STACK_SIZE (1224)
#define TASKS_STACK_SIZE (1024 + NUM_TASKS * FATFS_STACK_SIZE + 128 + configMINIMAL_STACK_SIZE)
#define DRIVE0 "0:"
#define FILE_COUNT (10U)

/*******************************************************************************
* Global functions
*******************************************************************************/

/*******************************************************************************
* Global variables
*******************************************************************************/

thread_function_wrapper_t g_thread_wrappers[NUM_TASKS];

/*******************************************************************************
* Local types
*******************************************************************************/

/*******************************************************************************
* Local function prototypes
*******************************************************************************/

/*!
 * @brief Create file with random content.
 *
 *@param Not name of notification
 * @param filename name of file.
 * @retval FR_OK on success.
 */
static FRESULT create_file(TCHAR filename[], TCHAR *Not);

/*!
 * @brief Mount a filesystem and create a file.
 *
 * @param Not name of notification.
 * @retval void.
 */
static void task_fatfs_write_sample(TCHAR *task);

/*!
 * @brief Thread routine that runs various filesystem operations.
 *
 * @param void p* pointer to void.
 * @retval void.
 */
static void task_fatfs_sample(void *arg);

/*!
 * @brief Main thread routine that handles sibling threads.
 *
 * @param void * pointer to void.
 * @retval void.
 */

/*******************************************************************************
* Local variables
*******************************************************************************/

/* Filesystem object */
static FATFS fs;
static uint8_t counter;

/*******************************************************************************
* Local functions
*******************************************************************************/

static void task_fatfs_write_sample(TCHAR *task)
{
    FRESULT status = FR_OK;
    TCHAR filePath[32];

    printf(task);
    memset(filePath,0,sizeof(filePath));
    sprintf(filePath, "freeRTOS%s", ".bin");

    status = f_mount(&fs, DRIVE0, 1U);
    if(FR_OK == status)
    {
        status = create_file(filePath, task);
        if(FR_OK == status)
        {
            printf("PASS\r\n");
        }
        else
        {
            printf("ERROR\r\n");
            while(1) {};
        }
        f_unmount(DRIVE0);
    }
}


static FRESULT create_file(TCHAR filename[], TCHAR *task)
{
    /* File object */
    FIL fd;
    /* File stat object */
    FILINFO fst;
    /* API result code */
    FRESULT status = FR_OK;
    UINT i;
    UINT bytesWritten;
    UINT bytesToWrite;
    TCHAR path[255];
    UINT fileBufferSize = 80;
    TCHAR fileBuffer[fileBufferSize];


    sprintf(path, "%s%s", DRIVE0, filename);
    status = f_open(&fd, path, FA_CREATE_NEW | FA_WRITE);
    if(status == FR_EXIST)
    {
        if (0 == counter)
        {
            status = f_open(&fd, path, FA_CREATE_ALWAYS | FA_WRITE);
        }
        else
        {
            status = f_open(&fd, path, FA_OPEN_APPEND | FA_WRITE);
        }
    }
    else if(status != FR_OK)
    {
        status = f_open(&fd, path, FA_CREATE_ALWAYS | FA_WRITE);
    }
    else
    {} /* the file is already opened */

     if(FR_OK == status)
    {

        for(i = 0U; i < FILE_COUNT; i++)
        {
            bytesWritten = 0U;
            memset(fileBuffer, 0, fileBufferSize);
            sprintf(fileBuffer, task);
            sprintf(&fileBuffer[strlen(task)], "I was here %u times", i);
            if((i == 0) && (counter == 0))
            {
                strcat(fileBuffer, " and created the freeRTOS.bin file");
            }
            strcat(fileBuffer, ". \n");
            bytesToWrite = strlen(fileBuffer);
            status = f_write(&fd, fileBuffer, bytesToWrite, &bytesWritten);
            if(FR_OK == status)
            {
                status = f_sync(&fd);
            }
            if((FR_OK != status) || (0U == bytesWritten))
            {
                break;
            }
        }
        if(FR_OK == status)
        {
            status = f_sync(&fd);
        }
        if(FR_OK == status)
        {
            status = f_stat(path, &fst);
        }
        if(FR_OK == status)
        {
            status = f_close(&fd);
        }
        else
        {
            f_close(&fd);
        }
    }
    return status;
}

static void task_fatfs_sample(void *arg)
{
    transmit_param_t *notSem;
    DEV_ASSERT(NULL != arg);
    DEV_ASSERT(NUM_TASKS <= MAX_NUM_TASKS);
    notSem = (transmit_param_t *)arg;
    status_t status = STATUS_ERROR;

    /* wait for the semaphore to be free by the prior task. */
    do
    {
        status = OSIF_SemaWait(notSem->Sem, OSIF_WAIT_FOREVER);
        if(STATUS_SUCCESS == status)
        {
            printf("Task_Read success to pass the semaphore!\n");
            break;
        }
        else
        {
            printf("Task_Read failed to pass the semaphore!\n");
            break;
        }
    }
    while(1);

    if (status == STATUS_SUCCESS)
    {
        printf("counter = %u\n", counter);
        task_fatfs_write_sample(notSem->notification);
        (void)OSIF_SemaPost(notSem->Sem);

    }
    counter++;
    if(NUM_TASKS == counter)
    {
        (void)OSIF_SemaDestroy(notSem->Sem);
    }
}

/*******************************************************************************
* Global functions
*******************************************************************************/

/*!
 * @brief A routine that runs main thread and starts thread scheduler.
 * It is called from main module.
 *
 * @param void.
 * @retval void.
 */
void run_tasks(void)
{

    uint8_t i;
    TCHAR name[16];
    semaphore_t Sem;
    transmit_param_t notSem[NUM_TASKS];
    uint8_t prior = 0;
    status_t status = OSIF_SemaCreate(&Sem, 1U);
    DEV_ASSERT(NUM_TASKS <= MAX_NUM_TASKS);

    if(STATUS_SUCCESS == status)
    {
        srand(100);
        for(i = 0U; i < NUM_TASKS; i++)
        {
            /* create thread */
            notSem[i].Sem = &Sem;
            prior = rand() % (configMAX_PRIORITIES);
            memset(notSem[i].notification, 0, sizeof(notSem[i].notification));
            sprintf(notSem[i].notification, "Task %u of %u priority ", i, prior);
            if(STATUS_SUCCESS == status)
            {
                sprintf(name, "fatfs_test_%u", i);
                sys_thread_new(name, &g_thread_wrappers[i], task_fatfs_sample, &notSem[i], FATFS_STACK_SIZE, prior);
            }
        }
    }

    /* Start the tasks and timer running. */
    vTaskStartScheduler();

    /* If all is well, the scheduler will now be running, and the following
    line will never be reached.  If the following line does execute, then
    there was insufficient FreeRTOS heap memory available for the idle and/or
    timer tasks to be created.  See the memory management section on the
    FreeRTOS web site for more details. */
    while(1)
    {
        OSIF_TimeDelay(200);
        Led_Blink(ERR_LED);
    };

    return;
}

/*!
 * @brief Prototypes for the standard FreeRTOS application hook (callback) functions
 * implemented within this file.  See http://www.freertos.org/a00016.html .
 */
void vApplicationMallocFailedHook(void);
void vApplicationIdleHook(void);
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName);
void vAssertCalled(unsigned long ulLine, const char * const pcFileName);

void vAssertCalled(unsigned long ulLine, const char * const pcFileName)
{
    /* Called if an assertion passed to configASSERT() fails.  See
    http://www.freertos.org/a00110.html#configASSERT for more information. */

    printf("ASSERT! Line %lu, file %s\r\n", ulLine, pcFileName);

    portENTER_CRITICAL();
    {
        DEV_ASSERT(false);
    }
    portEXIT_CRITICAL();
}

void vApplicationMallocFailedHook(void)
{
    /* vApplicationMallocFailedHook() will only be called if
    configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
    function that will get called if a call to pvPortMalloc() fails.
    pvPortMalloc() is called internally by the kernel whenever a task, queue,
    timer or semaphore is created.  It is also called by various parts of the
    demo application.  If heap_1.c, heap_2.c or heap_4.c is being used, then the
    size of the heap available to pvPortMalloc() is defined by
    configTOTAL_HEAP_SIZE in FreeRTOSConfig.h, and the xPortGetFreeHeapSize()
    API function can be used to query the size of free heap space that remains
    (although it does not provide information on how the remaining heap might be
    fragmented).  See http://www.freertos.org/a00111.html for more
    information. */
    vAssertCalled(__LINE__, __FILE__);
}

void vApplicationIdleHook(void)
{
    /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
    to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
    task.  It is essential that code added to this hook function never attempts
    to block in any way (for example, call xQueueReceive() with a block time
    specified, or call vTaskDelay()).  If application tasks make use of the
    vTaskDelete() API function to delete themselves then it is also important
    that vApplicationIdleHook() is permitted to return to its calling function,
    because it is the responsibility of the idle task to clean up memory
    allocated by the kernel to any task that has since deleted itself. */
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    (void)pcTaskName;
    (void)pxTask;

    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected.  This function is
    provided as an example only as stack overflow checking does not function
    when running the FreeRTOS Windows port. */
    vAssertCalled(__LINE__, __FILE__);
}
