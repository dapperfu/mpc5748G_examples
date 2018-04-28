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
**     Filename    : echoEmulation.c
**     Project     : sdhc_fatfs_mpc5748g
**     Processor   : MPC5748G_324
**     Version     : 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   :
**     Abstract    :
**         This module contains echo emulation.
**     Settings    :
**     Contents    :
**
** ###################################################################*/

#include "Cpu.h"

#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "console/ringBuffer.h"
#include "console/uart.h"
#include "console/echoEmulation.h"

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 21.6, The standard library input/output functions shall not be used.
 * This is required for getchar and getc functions.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.3, Cast performed between a pointer
 * to object type and a pointer to a different object type.
 * This cast is to required write 8bit or 16bit data into corresponding address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.1, Operands shall not be of an inappropiate essential type.
 * This is required for isprintf function from cctype.h.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.2, Expressions of essentially character type shall not be used
 * inappropriately in addition and substraction operations.
 * This is required for isprintf function from cctype.h.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.4, Both operands of an operator in which the usual arithmetic
 * conversions are performed shall have the same essential type category
 * This is required for isprintf function from cctype.h.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.9, Conversion from pointer to void into pointer to object.
 * This is need for DEV_ASSERT macro condition.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 18.4,The +, -, +=, -= operators should not be applied to an expression
 * of pointer type.
 * This is required for isprintf function from cctype.h.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 12.1,The precedence of operators within expressions should be made explicit
 * This is required for getchar function from stdio.h.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 7.1, Octal constants shall not be used.
 * This is required for isprintf function from cctype.h.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 13.3, A full expression containing an increment (++) or decrement (--) operator
 * should have no other potential side effects other than that caused by the increment or decrement operator.
 * This is required for  getchar function from stdio.h.
 *
 */

/*******************************************************************************
* Global variables
*******************************************************************************/

echoEmulation_t stdOutEmu = {(char)0U };

char csi_table[] = {
		'0',
		'1',
		'2',
		'3',
		'4',
		'5',
		'6',
		'7',
		'8',
		'9',
		CSI_SEMICOL_CHAR,
#define CSI_ENDCHARS_IND (11)
		CSI_CURSOR_CUU_CHAR,
		CSI_CURSOR_CUD_CHAR,
		CSI_CURSOR_CUF_CHAR,
		CSI_CURSOR_CUB_CHAR,
		/*
		 * CSI_CURSOR_RI_CHAR is identical
		 * to CSI_CURSOR_DL_CHAR
		 */
		CSI_CURSOR_DECSC_CHAR,
		CSI_CURSOR_DECSR_CHAR,
		CSI_CURSOR_CNL_CHAR,
		CSI_CURSOR_CPL_CHAR,
		CSI_CURSOR_CHA_CHAR,
		CSI_CURSOR_CUP_CHAR,
		CSI_CURSOR_VPA_CHAR,
		CSI_CURSOR_HVP_CHAR,
		CSI_CURSOR_ANSISYSSC_CHAR,
		CSI_CURSOR_ANSISYSSR_CHAR,
		CSI_CURSOR_ATT160_CHAR,
		CSI_CURSOR_DECTCEM_CHAR,
		CSI_CURSOR_SU_CHAR,
		CSI_CURSOR_SD_CHAR,
		CSI_CURSOR_ICH_CHAR,
		CSI_CURSOR_DCH_CHAR,
		CSI_CURSOR_ECH_CHAR,
		CSI_CURSOR_IL_CHAR,
		CSI_CURSOR_DL_CHAR,
		CSI_CURSOR_ED_CHAR,
		CSI_CURSOR_EL_CHAR,
		NULL_CHAR /* End of table */
};

/*******************************************************************************
* Constants and macros
*******************************************************************************/

#define TERM_MAX_LINES (80)
#define TERM_MAX_COLS (235)

/*******************************************************************************
* Local types
*******************************************************************************/

/*******************************************************************************
* Local function prototypes
*******************************************************************************/

/*!
 * @brief TX UART routine. Sends character via UART.
 *
 * @param emu console emulation structure.
 * @param ch char to be send
 * @retval void.
 */
static void TransmitDataCommon(echoEmulation_t *emu, char ch);

/*!
 * @brief Tests if input character in within CSI range (Control Sequence Introducer). Sends character via UART.
 *
 * @param ch input char
 * @retval int8_t index in internal array or -1 if not found.
 */
static int8_t findInCsi(const char ch);

/*!
 * @brief Handle echo in case of CSI_BR character. Sends character via UART.
 *
 * @param emu console emulation structure.
 * @param ch input char
 * @retval returns if found in internal array or -1 if not found.
 */
static int8_t TransmitDataIfBR(echoEmulation_t *emu, const char ch);

/*!
 * @brief Handle echo in case of CSI_CURSOR characters. Emulates cursor movement in UART console.
 *
 * @param emu console emulation structure.
 * @param ch input char
 * @param foundInd if found in internal array or -1 if not found.
 * @retval returns non-zero value if cursor is out-of-bound of view port (console visible area).
 */
static uint8_t HandleCsiCursor(echoEmulation_t *emu, const char ch, int8_t foundInd);

/*******************************************************************************
* Local variables
*******************************************************************************/

static char rxBuffer_buffer[20U];
static rBuffer_t rxBuffer =
{
        .bufferPtr = rxBuffer_buffer,
        .head = 0,
        .tail = 0,
        .maxSize = 20
};

/*******************************************************************************
* Local functions
*******************************************************************************/

static void TransmitDataCommon(echoEmulation_t *emu, char ch)
{
	static char ourCh;
	bool needBack = false;
	uint8_t useVport;
	ourCh = ch;
	useVport = emu->useVport;
	if((0 != isprint(ourCh)) && (0 == emu->escSeqPos))
	{
		if((1U == emu->useVport) && ((emu->cPos + 1U) > (emu->cPosV + emu->cPosD - 1U)))
		{
			needBack = true;
		}
		emu->cPos++;
		if(emu->cPos > TERM_MAX_COLS)
		{
			emu->cPos -= TERM_MAX_COLS;
		}
	}
	if('\b' == ourCh)
	{
		if(emu->cPos > 0U)
		{
			emu->cPos--;
		}
	}
	if('\r' == ourCh)
	{
		emu->cPos = 0U;
	}
	if('\n' == ourCh)
	{
		if(emu->lPos > 0U)
		{
			emu->lPos--;
		}
	}
	if(0U == emu->echoOff)
	{
		TransmitData(&ourCh, sizeof(ourCh));
		if(true == needBack)
		{
			emu->useVport = 0U;
			sendEscSeqCmdN(1U, CSI_CURSOR_CUB_CHAR);
			emu->useVport = useVport;
		}
	}
	if('\r' == ourCh)
	{
		if(1U == emu->useVport)
		{
			emu->useVport = 0U;
			sendEscSeqCmdN((uint8_t)(emu->cPosV - 1U), CSI_CURSOR_CUF_CHAR);
			emu->useVport = useVport;
		}
	}
	if('\n' == ourCh)
	{
		if((1U == emu->useVport) && ((emu->lPos - 1U) < emu->lPosI))
		{
			emu->useVport = 0U;
			sendEscSeqCmdN(1U, CSI_CURSOR_CUU_CHAR);
			emu->useVport = useVport;
		}
	}
}

static int8_t findInCsi(const char ch)
{
	uint8_t j;
	int8_t foundInd;
	j = 0U;

	foundInd = -1;
	do
	{
		if(ch == csi_table[j])
		{
			foundInd = (int8_t)j;
			break;
		}
		j++;
	}while(NULL_CHAR != csi_table[j]);
	return foundInd;
}

static int8_t TransmitDataIfBR(echoEmulation_t *emu, const char ch)
{
	char oppCh;
	char ourCh;
	int8_t foundInd = -1;

	ourCh = ch;
	if(1U == emu->escSeqPos)
	{
		foundInd = findInCsi(ourCh);
		if(((char)CSI_BR_CHAR != ourCh) &&
			(-1 == foundInd))
		{
			emu->escSeqPos = 0U;
		}
		else
		{
			emu->escSeqPos++;
			if(0U == emu->denyEscSeq)
			{
				if((char)CSI_BR_CHAR != ourCh)
				{
					oppCh = (char)CSI_BR_CHAR;
					if(0U == emu->echoOff)
					{
						TransmitDataCommon(emu, oppCh);
					}
				}
			}
		}
	}
	else
	{
		foundInd = findInCsi(ourCh);
		if(0U < emu->escSeqPos)
		{
			if(-1 == foundInd)
			{
				emu->escSeqPos = 0U;
			}
			else
			{
				emu->escSeqPos++;
			}
		}
	}
	return foundInd;
}

static uint8_t HandleCsiCursor(echoEmulation_t *emu, const char ch, int8_t foundInd)
{
	char ourCh;
    uint8_t status = 1U;

	ourCh = ch;
    if(1U == emu->denyEscSeq)
	{
		status = 0U;
	}
	if(CSI_ENDCHARS_IND <= foundInd)
	{
		if(CSI_CURSOR_CUU_CHAR == ourCh)
		{
			if((1U == emu->useVport) && (emu->lPos > emu->lPosV))
			{
				status = 0U;
			}
			else
			{
				emu->lPos++;
				if(emu->lPos > TERM_MAX_LINES)
				{
					emu->lPos -= TERM_MAX_LINES;
				}
			}
		}
		if(CSI_CURSOR_CUD_CHAR == ourCh)
		{
			if((1U == emu->useVport) && ((emu->lPos - 1U) == emu->lPosI))
			{
				status = 0U;
			}
			else
			{
				if(emu->lPos > 0U)
				{
					emu->lPos--;
				}
			}
		}
		if(CSI_CURSOR_CUF_CHAR == ourCh)
		{
			if((1U == emu->useVport) && ((emu->cPos + 1U) > (emu->cPosV + emu->cPosD - 1U)))
			{
				status = 0U;
			}
			else
			{
				emu->cPos++;
				if(emu->cPos > TERM_MAX_COLS)
				{
					emu->cPos -= TERM_MAX_COLS;
				}
			}
		}
		if(CSI_CURSOR_CUB_CHAR == ourCh)
		{
			if((1U == emu->useVport) && (emu->cPos < emu->cPosV))
			{
				status = 0U;
			}
			else
			{
				if(emu->cPos > 0U)
				{
					emu->cPos--;
				}
			}
		}
	}
	return status;
}

/*******************************************************************************
* Global functions
*******************************************************************************/

echoEmulation_t *getEchoEmuForUART(void)
{
	return &stdOutEmu;
}

void EchoEmulationInit(echoEmulation_t *emu)
{
	void *p = emu;
	if(NULL != emu)
	{
		(void)memset(p, 0U, sizeof(echoEmulation_t));
	}
}

void sendNullTerminatedBuffer(const char *buffer)
{
	static char sch;
	uint16_t j;
	echoEmulation_t *emu;

	(void)buffer;
	emu = getEchoEmuForUART();
	j = 0U;
	sch = buffer[j];
	while(sch != (char)NULL_CHAR)
	{
		EchoEmulationTXChar(emu, &sch);
		j++;
		sch = buffer[j];
	}
}

void sendEscSeqCmd(char command)
{
	static char sch;
	echoEmulation_t *emu;

	emu = getEchoEmuForUART();
	sch = (char)CSI_ESC_CHAR;
	EchoEmulationTXChar(emu, &sch);
	sch = (char)CSI_BR_CHAR;
	EchoEmulationTXChar(emu, &sch);
	sch = command;
	EchoEmulationTXChar(emu, &sch);
}

void sendEscSeqCompCmd(const char *numberBuffer, uint16_t length, char command)
{
	static char sch;
	uint16_t j;
	echoEmulation_t *emu;

	(void)numberBuffer;
	emu = getEchoEmuForUART();
	sch = (char)CSI_ESC_CHAR;
	EchoEmulationTXChar(emu, &sch);
	sch = (char)CSI_BR_CHAR;
	EchoEmulationTXChar(emu, &sch);
	for(j = 0U; j < length; j++)
	{
		sch = numberBuffer[j];
		EchoEmulationTXChar(emu, &sch);
	}
	sch = command;
	EchoEmulationTXChar(emu, &sch);
}

void sendEscSeqCmdN(uint8_t n, char command)
{
	uint16_t j;
	for(j = 0U; j < n; j++)
	{
		sendEscSeqCmd(command);
	}
}

void EscapeLineTrails(echoEmulation_t *emu, char ourCh)
{
	char oppCh = (char)0x00;
	uint8_t status = 1U;
	if('\r' == ourCh)
	{
		oppCh = '\n';
		emu->receivePos = 0U;
	}
	else
	{
		if('\n' == ourCh)
		{
			oppCh = '\r';
			emu->receivePos = 0U;
		}
		else
		{
			status = 0U;
		}
	}
	if(1U == status)
	{
		if((0U == emu->isPrevCh) ||
				((oppCh != emu->prevCh) && (ourCh != emu->prevCh)))
		{
			if(0U == emu->echoOff)
			{
				TransmitDataCommon(emu, oppCh);
			}
		}
	}
}

void TransmitDataWithEscape(echoEmulation_t *emu, const char ch)
{
	char oppCh;
	char ourCh;
	int8_t foundInd = -1;
    uint8_t status = 1U;

	ourCh = ch;
	if((char)CSI_ESC_CHAR == ourCh)
	{
		emu->escSeqPos = 1U;
	}
	else
	{
		foundInd = TransmitDataIfBR(emu, ch);
	}
	if(0U < emu->escSeqPos)
	{
		status = HandleCsiCursor(emu, ch, foundInd);
	}
	if(1U == status)
	{
		if('\n' == ourCh)
		{
			EscapeLineTrails(emu, ourCh);
		}
		if(0U == emu->echoOff)
		{
			TransmitDataCommon(emu, ourCh);
		}
		if('\r' == ourCh)
		{
			EscapeLineTrails(emu, ourCh);
		}
		if((1U != emu->inTransmit) &&
				('\r' != ourCh) && ('\n' != ourCh))
		{
			if(('\b' == ourCh) && ((uint16_t)1 <= emu->receivePos))
			{
				emu->receivePos--;
				if(0U == emu->echoOff)
				{
					oppCh = ' ';
					TransmitDataCommon(emu, oppCh);
					oppCh = '\b';
					TransmitDataCommon(emu, oppCh);
				}
			}
			else
			{
				emu->receivePos++;
			}
		}
	}
	if(0U < emu->escSeqPos)
	{
		if(CSI_ENDCHARS_IND <= foundInd)
		{
			emu->escSeqPos = 0U;
		}
	}
	if(0U == emu->isPrevCh)
	{
		emu->isPrevCh = 1U;
	}
	emu->prevCh = ourCh;
}

void EchoEmulationTXChar(echoEmulation_t *emu, const char *inCh)
{
	static char ch;
	static char oldCh;
	static char newCh;

	(void)inCh;
	/* Echo emulation on UART */
	if(NULL != emu)
	{
		DEV_ASSERT(NULL != inCh);
		ch = *inCh;
		while(0 != RBufferAdd(&rxBuffer, &ch))
		{
			if(0 == RBufferRemove(&rxBuffer, &oldCh))
			{
				TransmitDataWithEscape(emu, oldCh);
			}
		}
		while(0 == RBufferRemove(&rxBuffer, &newCh))
		{
			TransmitDataWithEscape(emu, newCh);
		}
	}
}

void EchoEmulationRXChar(const echoEmulation_t *emu, char *outCh)
{
	(void)emu;
	/* Echo emulation on UART */
	if(NULL != emu)
	{
		DEV_ASSERT(NULL != outCh);
		(void)ReceiveData(outCh);
	}
}

void ViewPortInit(echoEmulation_t *emu,
		uint16_t cPosV, uint16_t lPosV,
		uint16_t cPosVs, uint16_t lPosVs,
		uint16_t cPosD, uint16_t lPosI,
		bool needInit)
{
	if(true == needInit)
	{
		emu->cPos = 0U;
		emu->lPos = 0U;
		emu->cPosD = cPosD;
		emu->cPosV = cPosVs;
		emu->lPosV = lPosVs;
		emu->lPosI = lPosI;
		sendEscSeqCmdN((uint8_t)lPosV, CSI_CURSOR_CUU_CHAR);
		sendEscSeqCmdN((uint8_t)cPosV, CSI_CURSOR_CUF_CHAR);
		emu->useVport = 1U;
	}
	else
	{
		emu->useVport = 0U;
	}
}

void ViewPortDeinit(echoEmulation_t *emu)
{
	if(1U == emu->useVport)
	{
		emu->useVport = 0U;
		sendEscSeqCmdN((uint8_t)(emu->lPosV + (uint16_t)1 - emu->lPos), CSI_CURSOR_CUU_CHAR);
		sendEscSeqCmdN((uint8_t)emu->cPos, CSI_CURSOR_CUB_CHAR);
		/*sendEscSeqCmd(CSI_CURSOR_EL_CHAR);*/
		emu->cPos = 0U;
		emu->lPos = 0U;
		emu->cPosD = 0U;
		emu->cPosV = 0U;
		emu->lPosV = 0U;
		emu->lPosI = 0U;
	}
}

bool readCPUntilChar(char stopCh1, char stopCh2, char stopCh3,
		char *outCh, uint16_t *cPosR, uint16_t *lPosR)
{
	echoEmulation_t *emu;
	uint8_t escSeqPos;
	bool status = false;
	static char ch;

	DEV_ASSERT(NULL != outCh);
	emu = getEchoEmuForUART();
    bool ok = false;
	do
	{
		escSeqPos = emu->escSeqPos;
		EchoEmulationRXChar(emu, &ch);
		if(0U == escSeqPos)
		{
			escSeqPos = emu->escSeqPos;
		}
		if((stopCh1 == ch) ||
				(stopCh2 == ch) ||
				(stopCh3 == ch) ||
				((0 == escSeqPos) && (0 != isprint(ch))))
		{
			*outCh = ch;
			if((stopCh1 != ch) &&
					(stopCh2 != ch) &&
					(stopCh3 != ch))
			{
				if(NULL != cPosR)
				{
					*cPosR = emu->cPos;
				}
				if(NULL != lPosR)
				{
					*lPosR = emu->lPos;
				}
				status = true;
			}
			ok = true;
		}
	}while(true != ok);
	return status;
}

uint32_t readUntilChar(char buffer[], uint32_t bufferLength, char stopCh, bool denyEscSeq)
{
	echoEmulation_t *emu;
	uint32_t pos;
	int ch;
	void* p = buffer;

	emu = getEchoEmuForUART();
	if(true == denyEscSeq)
	{
		emu->denyEscSeq = 1U;
	}
	else
	{
		emu->denyEscSeq = 0U;
	}

	pos = 0U;
	(void)memset(p, 0U, bufferLength);
	ch = getchar();
	while(EOF != ch)
	{
		if((stopCh != (char)ch) && (0 != isprint(ch)))
		{
			buffer[pos] = (char)ch;
			pos++;
		}
		if((stopCh == (char)ch) || (pos == (bufferLength - 1U)))
		{
			break;
		}
		ch = getchar();
	}
	emu->denyEscSeq = 0U;
	return pos;
}
