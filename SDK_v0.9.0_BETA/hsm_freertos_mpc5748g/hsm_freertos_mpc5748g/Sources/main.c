/* ###################################################################
**     Filename    : main.c
**     Project     : hsm_freertos_mpc5748g
**     Processor   : MPC5748G_324
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-06-15, 15:35, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.00
** @brief
**
**        The demo presents the HSM driver functionalities.
**        TASK0 sends an encrypted message to TASK1 using both
**        ECB and CBC encryption.
**
*/
/*!
**  @addtogroup main_module main module documentation
**  @{
*/
/* MODULE main */

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "FreeRTOS.h"
#include "clockMan1.h"
#include "hsm1.h"

volatile int exit_code = 0;

/* User includes (#include below this line is not maintained by Processor
 * Expert) */
#include "hsm_driver.h" /* HSM driver include */
#include "queue.h"
#include "task.h"
#include <string.h>

/* FreeRTOS defines: */
#define TASK0_DELAY ((TickType_t)100 / portTICK_PERIOD_MS)
#define TASK0_PRIORITY (tskIDLE_PRIORITY + 1)
#define TASK1_DELAY ((TickType_t)110 / portTICK_PERIOD_MS)
#define TASK1_PRIORITY (tskIDLE_PRIORITY + 2)
#define NOF_QUEUES 1
#define MESSAGE_LENGTH 16

/* Application defines: */
#define TIMEOUT_ENCRYPTION (1000U)
#define PLAINTEXT "AccessCode:01234"
#define MSG_ECB_OK "\nAES ECB Encryption/Decryption OK\n"
#define MSG_CBC_OK "\nAES CBC Encryption/Decryption OK\n"
#define MSG_ERROR "\nAn error occurred during the cryptographic operations!\n"

/* Enums: */
typedef enum {
  ENCRYPT_NONE = 0,
  ENCRYPT_ECB = 1,
  ENCRYPT_CBC = 2
} EncryptionType_e;

/* Structures: */
typedef struct {
  uint8_t ucInitVector[MESSAGE_LENGTH];
  uint8_t ucEncMsg[MESSAGE_LENGTH];
} Data_t;

/* Global variables: */
static QueueHandle_t g_tQueueHandle;

/* Functions: */
void vTASK0(void *pvParam) {
  const uint8_t ucMsg[MESSAGE_LENGTH] = {PLAINTEXT};
  static Data_t tTxData = {.ucInitVector = "1234567887654321"};
  static EncryptionType_e eEncType = ENCRYPT_NONE;

  /** Initialize HSM Driver: */
  const uint8_t ucPlainKey[MESSAGE_LENGTH] = {
      0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
      0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
  static hsm_state_t state;
  HSM_DRV_Init(&state);
  HSM_DRV_LoadPlainKey(ucPlainKey, TIMEOUT_ENCRYPTION);

  do {
    /* Every 'TASK0_DELAY' encrypt message and send it to TASK1: */
    vTaskDelay(TASK0_DELAY);
    switch (eEncType) {
    case ENCRYPT_NONE:
      xQueueSend(g_tQueueHandle, (void *)&tTxData, (TickType_t)0);
      eEncType = ENCRYPT_ECB;
      break;
    case ENCRYPT_ECB:
      HSM_DRV_EncryptECB(HSM_RAM_KEY, (uint8_t *)ucMsg, MESSAGE_LENGTH,
                         (uint8_t *)tTxData.ucEncMsg, TIMEOUT_ENCRYPTION);
      xQueueSend(g_tQueueHandle, (void *)&tTxData, (TickType_t)0);
      eEncType = ENCRYPT_CBC;
      break;
    case ENCRYPT_CBC:
      HSM_DRV_EncryptCBC(HSM_RAM_KEY, (uint8_t *)ucMsg, MESSAGE_LENGTH,
                         (uint8_t *)tTxData.ucInitVector,
                         (uint8_t *)tTxData.ucEncMsg, TIMEOUT_ENCRYPTION);
      xQueueSend(g_tQueueHandle, (void *)&tTxData, (TickType_t)0);
      eEncType = ENCRYPT_NONE;
      break;
    default:
      /* Do nothing... */
      break;
    }
  } while (1);
}

void vTASK1(void *pvParam) {
  static uint8_t ucMsg[MESSAGE_LENGTH];
  static Data_t tRxData;
  static EncryptionType_e eEncType = ENCRYPT_NONE;
  status_t stat;

  /* Initialize LINFLEXD peripheral for UART echo to console */
  LINFLEXD_UART_DRV_Init(INST_LINFLEXD_UART1, &linflexd_uart1_State,
                         &linflexd_uart1_InitConfig0);

  do {
    /* Receive message from TASK0 and decrypt: */
    if (xQueueReceive(g_tQueueHandle, &tRxData, (TickType_t)TASK1_DELAY)) {
      switch (eEncType) {
      case ENCRYPT_NONE:
        eEncType = ENCRYPT_ECB;
        break;
      case ENCRYPT_ECB:
        stat = HSM_DRV_DecryptECB(HSM_RAM_KEY, (uint8_t *)tRxData.ucEncMsg,
                                  MESSAGE_LENGTH, (uint8_t *)ucMsg,
                                  TIMEOUT_ENCRYPTION);
        eEncType = ENCRYPT_CBC;
        /* Send the status to the console */
        if ((stat == STATUS_SUCCESS) &&
            (strcmp((char *)ucMsg, PLAINTEXT) == 0)) {
          LINFLEXD_UART_DRV_SendDataBlocking(
              INST_LINFLEXD_UART1, (uint8_t *)MSG_ECB_OK, strlen(MSG_ECB_OK),
              TIMEOUT_ENCRYPTION);
        } else {
          LINFLEXD_UART_DRV_SendDataBlocking(
              INST_LINFLEXD_UART1, (uint8_t *)MSG_ERROR, strlen(MSG_ERROR),
              TIMEOUT_ENCRYPTION);
        }
        break;
      case ENCRYPT_CBC:
        HSM_DRV_DecryptCBC(HSM_RAM_KEY, (uint8_t *)tRxData.ucEncMsg,
                           MESSAGE_LENGTH, (uint8_t *)tRxData.ucInitVector,
                           (uint8_t *)ucMsg, TIMEOUT_ENCRYPTION);
        eEncType = ENCRYPT_NONE;
        /* Send the status to the console */
        if ((stat == STATUS_SUCCESS) &&
            (strcmp((char *)ucMsg, PLAINTEXT) == 0)) {
          LINFLEXD_UART_DRV_SendDataBlocking(
              INST_LINFLEXD_UART1, (uint8_t *)MSG_CBC_OK, strlen(MSG_CBC_OK),
              TIMEOUT_ENCRYPTION);
        } else {
          LINFLEXD_UART_DRV_SendDataBlocking(
              INST_LINFLEXD_UART1, (uint8_t *)MSG_ERROR, strlen(MSG_ERROR),
              TIMEOUT_ENCRYPTION);
        }
        break;
      default:
        /* Do nothing... */
        break;
      }
    }
  } while (1);
}

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void) {
/* Write your local variable definition here */

/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
#ifdef PEX_RTOS_INIT
  PEX_RTOS_INIT(); /* Initialization of the selected RTOS. Macro is defined by
                      the RTOS component. */
#endif
  /*** End of Processor Expert internal initialization.                    ***/
  /* Write your code here */

  /* Initialize clocks */
  CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                 g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
  CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

  /* Initialize pins */
  PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

  /* Initialize FreeRTOS: */
  xTaskCreate(vTASK0, (const char *const) "TASK0", configMINIMAL_STACK_SIZE,
              (void *)0, TASK0_PRIORITY, NULL);
  xTaskCreate(vTASK1, (const char *const) "TASK1", configMINIMAL_STACK_SIZE,
              (void *)1, TASK1_PRIORITY, NULL);
  g_tQueueHandle = xQueueCreate(NOF_QUEUES, sizeof(Data_t));
  vTaskStartScheduler();

/*** Don't write any code pass this line, or it will be deleted during code
 * generation. ***/
/*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component.
 * DON'T MODIFY THIS CODE!!! ***/
#ifdef PEX_RTOS_START
  PEX_RTOS_START(); /* Startup of the selected RTOS. Macro is defined by the
                       RTOS component. */
#endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for (;;) {
    if (exit_code != 0) {
      break;
    }
  }
  return exit_code;
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.1 [05.21]
**     for the NXP C55 series of microcontrollers.
**
** ###################################################################
*/
