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
**     Filename    : uart.h
**     Project     : sdhc_freertos_mpc5748g
**     Processor   : MPC5748G_324
**     Version     : 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   :
**     Abstract    :
**         This module contains UART console implementation.
**     Settings    :
**     Contents    :
**
** ###################################################################*/

#ifndef UART_CUST_H
#define UART_CUST_H

#if defined __GNUC__
#include <ansi_parms.h>
#if !_EWL_CONSOLE_SUPPORT
#error _EWL_CONSOLE_SUPPORT must not be defined to 0.
#endif

#include <sys/UART.h>
#else

typedef int UARTError;

enum {
	kUARTNoError = 0,
	kUARTUnknownBaudRate,
	kUARTConfigurationError,
	kUARTBufferOverflow,				/* specified buffer was too small */
	kUARTNoData							/* no data available from polling */
};

typedef enum {
	kBaudHWSet = -1,					/* use HW settings such as DIP switches */
	kBaud300 = 300,						/* valid baud rates */
	kBaud600 = 600,
	kBaud1200 = 1200,
	kBaud1800 = 1800,
	kBaud2000 = 2000,
	kBaud2400 = 2400,
	kBaud3600 = 3600,
	kBaud4800 = 4800,
	kBaud7200 = 7200,
	kBaud9600 = 9600,
	kBaud19200 = 19200,
	kBaud38400 = 38400,
	kBaud57600 = 57600,
	kBaud115200 = 115200,
	kBaud230400 = 230400
} UARTBaudRate;
#endif

#define UART_CONSOLE_DEFAULT_BAUD_RATE (115200U)

UARTError InitializeUART(UARTBaudRate baudRate);
UARTError TerminateUART(void);

UARTError ReadUARTPoll(char *c);

UARTError ReadUART1(char *c);
UARTError ReadUARTN(void* bytes, unsigned long length);
UARTError ReadUARTString(char *string, unsigned long limit, char terminateChar);

UARTError WriteUART1(char c);
UARTError WriteUARTN(const void *bytes, unsigned long length);
UARTError WriteUARTString(const char *string);

void LINFlexD_2_Init(uint32_t mantissa, uint32_t fract);
void TransmitDataImpl(const char * const pBuf);

void TransmitData(const char *pBuf, const uint32_t cnt);
int32_t ReceiveData(char *const pBuf);

#endif /* UART_CUST_H */
