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
**     Filename    : setup.c
**     Project     : sdhc_freertos_mpc5748g
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

#include "Cpu.h"
#include "pin_mux.h"
#include "clockMan1.h"
#include "rtcTimer1.h"

#include <stdint.h>
#include <string.h>

#include "project.h"
#include "setup.h"
#include "fatfs/usdhc_fatfs_impl.h"
#include "common/fatfs_impl.h"

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

static void delay(volatile int cycles);
static void Led_Select(uint32_t ledPin, PORT_Type **ledPort, GPIO_Type **ledIoPort, uint16_t *ledIoPin);

/*******************************************************************************
* Local variables
*******************************************************************************/

/*******************************************************************************
* Local functions
*******************************************************************************/

static void delay(volatile int cycles)
{
	/* Delay function - do nothing for a number of cycles */
	while(cycles--);
}

static void Led_Select(uint32_t ledPin, PORT_Type **ledPort, GPIO_Type **ledIoPort, uint16_t *ledIoPin)
{
	switch(ledPin)
	{
        case SD_TX_LED:
        	*ledPort = SD_TX_LED_PORT;
            *ledIoPort = SD_TX_LED_IOPORT;
            *ledIoPin = SD_TX_LED_IOPIN;
            break;
        case SD_RX_LED:
        	*ledPort = SD_RX_LED_PORT;
            *ledIoPort = SD_RX_LED_IOPORT;
            *ledIoPin = SD_RX_LED_IOPIN;
            break;
        case IDLE_LED:
	    default:
	    	*ledPort = IDLE_LED_PORT;
            *ledIoPort = IDLE_LED_IOPORT;
            *ledIoPin = IDLE_LED_IOPIN;
            break;
	}
}

/*******************************************************************************
* Global functions
*******************************************************************************/

void uSDHC_fatfs_user_initialize(void)
{
	uSDHC_fatfs_initialize();
	g_fatfsCallbacks[FATFS_DISKIO_READ]
		.postFunction = FatFs_DiskIo_Read_Callback;
	g_fatfsCallbacks[FATFS_DISKIO_WRITE]
		.postFunction = FatFs_DiskIo_Write_Callback;
}

void uSDHC_PadsInit(void)
{
	/* USDHC configuration via PINS PEX component. */
	/* PH8 USDHC_WP: IBE(1), OBE(1), SRC(3), IMCR_SSS(1), SSS(1) */
	/* PE7 USDHC_CLK: IBE(1), OBE(1), SRC(3), IMCR_SSS(1), SSS(5) */
	/* PE6 USDHC_CMD: IBE(1), OBE(1), SRC(3), PUE(1), PUS(1), IMCR_SSS(2), SSS(5) */
	/* PI3 USDHC_DAT0: IBE(1), OBE(1), SRC(3), PUE(1), PUS(1), IMCR_SSS(2), SSS(3) */
	/* PI2 USDHC_DAT1: IBE(1), OBE(1), SRC(3), PUE(1), PUS(1), IMCR_SSS(2), SSS(4) */
	/* PI1 USDHC_DAT2: IBE(1), OBE(1), SRC(3), PUE(1), PUS(1), IMCR_SSS(2), SSS(3) */
	/* PI0 USDHC_DAT3: IBE(1), OBE(1), SRC(3), PUE(1), PUS(2), IMCR_SSS(2), SSS(4) */
}

void LINFlexD_PadsInit(void)
{
	/* Configure pads UART */
	/* Pull function is enabled for output and is a pull-up */
	PINS_DRV_SetPullSel((PORT_Type *)UART_OUT_IOSIG_IOPORT, UART_OUT_IOSIG, PORT_INTERNAL_PULL_UP_ENABLED);
}

void SysClock_Init(void)
{
	CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
	CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);
}

void Gen_PadsInit(void)
{
	PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
}

void Led_Toggle(uint32_t ledPin)
{
	/* Toggle output value */
	GPIO_Type locLedIoPort = {0};
	GPIO_Type *ledIoPort = &locLedIoPort;
	PORT_Type locLedPort;
	PORT_Type *ledPort = &locLedPort;
	uint16_t ledIoPin;
	Led_Select(ledPin, &ledPort, &ledIoPort, &ledIoPin);
	PINS_DRV_TogglePins(ledIoPort, (pins_channel_type_t)(1U << ledIoPin));
}

void Led_Init(uint32_t ledPin)
{
	/* Initialize LED */
	GPIO_Type locLedIoPort = {0};
	GPIO_Type *ledIoPort = &locLedIoPort;
	PORT_Type locLedPort;
	PORT_Type *ledPort = &locLedPort;
	uint16_t ledIoPin;
	Led_Select(ledPin, &ledPort, &ledIoPort, &ledIoPin);
	/* Set up LED pin explicitly in API */
	PINS_DRV_SetOutputBuffer((PORT_Type *)ledPort, ledIoPin, true, (port_mux_t)PORT_INPUT_MUX_ALT0);
	/* Clear output data before enabling output buffer */
	PINS_DRV_ClearPins(ledIoPort, (pins_channel_type_t)(1U << ledIoPin));
	/* Set LED low */
	PINS_DRV_SetPins(ledIoPort, (pins_channel_type_t)(1U << ledIoPin));
}

void Led_Blink(uint32_t ledPin)
{
	/* Insert a small delay to make the blinking visible */
	delay(720000);
	Led_Toggle(ledPin);
}

void FatFs_DiskIo_Read_Callback(uint32_t status, void *param)
{
	(void)param;
	if(STATUS_SUCCESS == status)
	{
		Led_Toggle(SD_RX_LED);
	}
}

void FatFs_DiskIo_Write_Callback(uint32_t status, void *param)
{
	(void)param;
	if(STATUS_SUCCESS == status)
	{
		Led_Toggle(SD_TX_LED);
	}
}

