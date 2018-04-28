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
**     Filename    : setup.h
**     Project     : sdhc_fatfs_mpc5748g
**     Processor   : MPC5748G_324
**     Version     : 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   :
**     Abstract    :
**         This module contains miscellaneous.
**     Settings    :
**     Contents    :
**
** ###################################################################*/

#ifndef SETUP_H
#define SETUP_H

#include <stdint.h>

#include "project.h"

/*!
 * @addtogroup MISC
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief GPIO for RS232 pins
 *
 */
#ifdef DEVKIT
#define UART_IN_IOSIG PCR_PC9
#define UART_IN_IOSIG_IOPORT PTC
#define UART_IN_IOSIG_IOPIN 9
#define UART_OUT_IOSIG PCR_PC8
#define UART_OUT_IOSIG_IOPORT PTC
#define UART_OUT_IOSIG_IOPIN 8
#endif
#ifdef MAINBOARD
#define UART_IN_IOSIG PCR_PC9
#define UART_IN_IOSIG_IOPORT PTC
#define UART_IN_IOSIG_IOPIN 9
#define UART_OUT_IOSIG PCR_PC8
#define UART_OUT_IOSIG_IOPORT PTC
#define UART_OUT_IOSIG_IOPIN 8
#endif

/*!
 * @brief GPIO for LEDs
 *
 * General rule: search in SCH file by GPIOXX/LEDYY, XX is the value.
 *
 */
#ifdef DEVKIT
#define LED1 PCR_PJ4      /* pin PJ[4] - LED1 (DS9) on DEV-KIT */
#define LED1_PORT PORTJ
#define LED1_IOPORT	PTJ
#define LED1_IOPIN	4
#define LED2 PCR_PA0	  /* pin PA[0] - LED2 (DS10) on DEV-KIT */
#define LED2_PORT PORTA
#define LED2_IOPORT	PTA
#define LED2_IOPIN	0
#define LED3 PCR_PA10	  /* pin PA[10] - LED3 (DS4) on DEV-KIT */
#define LED3_PORT PORTA
#define LED3_IOPORT	PTA
#define LED3_IOPIN	10
#define LED4 PCR_PA4	  /* pin PA[4] - LED4 (DS11) on DEV-KIT */
#define LED4_PORT PORTA
#define LED4_IOPORT	PTA
#define LED4_IOPIN	4
#define LED5 PCR_PA7	  /* pin PA[7] - LED5 (DS5) on DEV-KIT */
#define LED5_PORT PORTA
#define LED5_IOPORT	PTA
#define LED5_IOPIN	7
#define LED6 PCR_PC4	  /* pin PC[4] - LED6 (DS7) on DEV-KIT */
#define LED6_PORT PORTC
#define LED6_IOPORT	PTC
#define LED6_IOPIN	4
#define LED7 PCR_PH13	  /* pin PH[13] - LED7 (DS6) on DEV-KIT */
#define LED7_PORT PORTH
#define LED7_IOPORT	PTH
#define LED7_IOPIN	13
#endif
#ifdef MAINBOARD
#define LED1 PCR_PG2       /* pin PG[2] - LED1 (DS2) on Mainboard */
#define LED1_PORT PORTG
#define LED1_IOPORT	PTG
#define LED1_IOPIN	2
#define LED2 PCR_PG3       /* pin PG[3] - LED2 (DS2) on Mainboard */
#define LED2_PORT PORTG
#define LED2_IOPORT	PTG
#define LED2_IOPIN	3
#define LED3 PCR_PG4       /* pin PG[4] - LED3 (DS2) on Mainboard */
#define LED3_PORT PORTG
#define LED3_IOPORT	PTG
#define LED3_IOPIN	4
#define LED4 PCR_PG5       /* pin PG[5] - LED4 (DS2) on Mainboard */
#define LED4_PORT PORTG
#define LED4_IOPORT	PTG
#define LED4_IOPIN	5
#endif

#define IDLE_LED LED1
#define IDLE_LED_PORT LED1_PORT
#define IDLE_LED_IOPORT LED1_IOPORT
#define IDLE_LED_IOPIN LED1_IOPIN
#define SD_RX_LED LED2
#define SD_RX_LED_PORT LED2_PORT
#define SD_RX_LED_IOPORT LED2_IOPORT
#define SD_RX_LED_IOPIN LED2_IOPIN
#define SD_TX_LED LED3
#define SD_TX_LED_PORT LED3_PORT
#define SD_TX_LED_IOPORT LED3_IOPORT
#define SD_TX_LED_IOPIN LED3_IOPIN

/*************************************************************************************************
 * API
 ************************************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief uSDHC fatfs user callbacks initialization routine. Must be called before FATFS use.
 *
 */
void uSDHC_fatfs_user_initialize(void);

/*!
 * @brief Clock setup
 *
 * @param void.
 * @retval void.
 */
void SysClock_Init(void);

/*!
 * @brief Initialized led
 *
 * @param uint32_t index in GPDO/MSCR tables.
 * @retval void.
 */
void Led_Init(uint32_t ledPin);

/*!
 * @brief Toggle led state
 *
 * @param uint32_t index in GPDO/MSCR tables.
 * @retval void.
 */
void Led_Toggle(uint32_t ledPin);

/*!
 * @brief Delay and toggle led state
 *
 * @param uint32_t index in GPDO/MSCR tables.
 * @retval void.
 */
void Led_Blink(uint32_t ledPin);

/*!
 * @brief pins driver setup
 *
 * @param void.
 * @retval void.
 */
void Gen_PadsInit(void);

/*!
 * @brief uSDHC pads setup
 *
 * @param void.
 * @retval void.
 */
void uSDHC_PadsInit(void);

/*!
* @brief Linflex pads setup
*
* @param void.
* @retval void.
*/
void LINFlexD_PadsInit(void);

/*!
* @brief fatfs diskio read callback
*
* @param uint32_t operation result status.
* @param void * pointer to user data.
* @retval void.
*/
void FatFs_DiskIo_Read_Callback(uint32_t status, void *param);

/*!
* @brief fatfs diskio write callback
*
* @param uint32_t operation result status.
* @param void * pointer to user data.
* @retval void.
*/
void FatFs_DiskIo_Write_Callback(uint32_t status, void *param);

#if defined(__cplusplus)
}
#endif
/*! @} */
#endif /* SETUP_H */
