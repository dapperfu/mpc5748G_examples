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
**     Filename    : echoEmulation.h
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

#ifndef CONSOLE_ECHOEMULATION_H
#define CONSOLE_ECHOEMULATION_H

#include <stdint.h>

/* ASCII control codes definitions. */
#define NULL_CHAR ('\0')
#define ASCII_CC_STX (0x02)
#define ASCII_CC_ETX (0x03)
#define ASCII_CC_EOT (0x04)
/* CSI definitions. */
#define CSI_ESC_CHAR (0x1B)
#define CSI_BR_CHAR (0x5B)
#define CSI_SEMICOL_CHAR (';')
/* Cursor simple positioning. */
#define CSI_CURSOR_CUU_CHAR ('A')
#define CSI_CURSOR_CUD_CHAR ('B')
#define CSI_CURSOR_CUF_CHAR ('C')
#define CSI_CURSOR_CUB_CHAR ('D')
#define CSI_CURSOR_RI_CHAR ('M')
#define CSI_CURSOR_DECSC_CHAR ('7')
#define CSI_CURSOR_DECSR_CHAR ('8')
/* Cursor positioning. */
#define CSI_CURSOR_CNL_CHAR ('E')
#define CSI_CURSOR_CPL_CHAR ('F')
#define CSI_CURSOR_CHA_CHAR ('G')
#define CSI_CURSOR_CUP_CHAR ('H')
#define CSI_CURSOR_VPA_CHAR ('d')
#define CSI_CURSOR_HVP_CHAR ('f')
#define CSI_CURSOR_ANSISYSSC_CHAR ('s')
#define CSI_CURSOR_ANSISYSSR_CHAR ('u')
/* Cursor visibility. */
#define CSI_CURSOR_ATT160_CHAR ('h')
#define CSI_CURSOR_DECTCEM_CHAR ('l')
/* Viewport positioning. */
#define CSI_CURSOR_SU_CHAR ('S')
#define CSI_CURSOR_SD_CHAR ('T')
/* Text modification. */
#define CSI_CURSOR_ICH_CHAR ('@')
#define CSI_CURSOR_DCH_CHAR ('P')
#define CSI_CURSOR_ECH_CHAR ('X')
#define CSI_CURSOR_IL_CHAR ('L')
#define CSI_CURSOR_DL_CHAR ('M')
#define CSI_CURSOR_ED_CHAR ('J')
#define CSI_CURSOR_EL_CHAR ('K')

extern char csi_table[];

typedef struct
{
	char prevCh;
	uint8_t isPrevCh;
	uint8_t inUse;
	uint8_t echoOff;
	uint8_t escSeqPos;
	uint8_t denyEscSeq;
	uint8_t inTransmit;
	uint8_t useVport;
	uint16_t receivePos;
	uint16_t lPos;
	uint16_t cPos;
	uint16_t lPosV;
	uint16_t cPosV;
	uint16_t cPosD;
	uint16_t lPosI;
}echoEmulation_t;

extern echoEmulation_t stdOutEmu;

echoEmulation_t *getEchoEmuForUART(void);
void EchoEmulationInit(echoEmulation_t *emu);
void EchoEmulationRXChar(const echoEmulation_t *emu, char *outCh);
void EchoEmulationTXChar(echoEmulation_t *emu, const char *inCh);
void TransmitDataWithEscape(echoEmulation_t *emu, const char ch);
void EscapeLineTrails(echoEmulation_t *emu, char ourCh);

void sendNullTerminatedBuffer(const char *buffer);
void sendEscSeqCmd(char command);
void sendEscSeqCompCmd(const char *numberBuffer, uint16_t length, char command);
void sendEscSeqCmdN(uint8_t n, char command);

void ViewPortInit(echoEmulation_t *emu,
		uint16_t cPosV, uint16_t lPosV,
		uint16_t cPosVs, uint16_t lPosVs,
		uint16_t cPosD, uint16_t lPosI,
		bool needInit);
void ViewPortDeinit(echoEmulation_t *emu);
bool readCPUntilChar(char stopCh1, char stopCh2, char stopCh3, char *outCh, uint16_t *cPosR, uint16_t *lPosR);
uint32_t readUntilChar(char buffer[], uint32_t bufferLength, char stopCh, bool denyEscSeq);

#endif /* CONSOLE_ECHOEMULATION_H */
