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
**     Filename    : uart.c
**     Project     : sdhc_fatfs_mpc5748g
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

#include <stdint.h>

#include "Cpu.h"
#include "clock_manager.h"
#include "console/echoEmulation.h"
#include "console/uart.h"

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.4, A conversion should not be performed
 * between a pointer to pointer and an integer type This is required in
 * MPC5748_features.h header files.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, A cast shall not be performed between
 * pointer to void and an arithmetic type This is required in MPC5748_features.h
 * header files.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.8, The value of a composite expression
 * shall not be cast to a different essential type category or a wider essential
 * type. This is need for baudrate calculation.
 *
 * @section [global]
 * Violates MISRA 2012 Required Directive 4.6,typedef that indicate size and
 * signedness should be used in place of the basic numerical types. This is
 * required for keeping the prototype of  ReadUARTN(...) and  WriteUARTN(...)
 * functions.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.5,A conversion should not be performed
 * from pointer to void into pointer to oblect. This is required for reading and
 * writing data.
 *
 */

/*******************************************************************************
 * Global variables
 *******************************************************************************/

/*******************************************************************************
 * Constants and macros
 *******************************************************************************/

#define POLLING_ENABLED 0
#define FIFO_ENABLED 1

/*******************************************************************************
 * Local types
 *******************************************************************************/

/*******************************************************************************
 * Local function prototypes
 *******************************************************************************/

/*******************************************************************************
 * Local variables
 *******************************************************************************/

static uint8_t fifoState;
static uint8_t firstTxUARTByte;

/*******************************************************************************
 * Local functions
 *******************************************************************************/

/*******************************************************************************
 * Global functions
 *******************************************************************************/

void LINFlexD_2_Init(uint32_t mantissa, uint32_t fract) {
  uint32_t uartcr;
  firstTxUARTByte = 1U;
  /* Enter INIT mode */
  LINFlexD_2->LINCR1 = LINFlexD_LINCR1_INIT(1U);
  /* Exit Sleep Mode */
  LINFlexD_2->LINCR1 |= LINFlexD_LINCR1_SLEEP(0U);

  /* Wait until LINSR value isn't equal to LINFlexD_STATE_INIT_MODE */
  while (0x1 != (uint8_t)((LINFlexD_2->LINSR & LINFlexD_LINSR_LINS_MASK) >>
                          LINFlexD_LINSR_LINS_SHIFT)) {
  }

  /* Enable UART before UART configuration */
  LINFlexD_2->UARTCR = LINFlexD_UARTCR_UART(1U);

  uartcr = LINFlexD_2->UARTCR;
  /* 8-bit data */
  uartcr &= ~(LINFlexD_UARTCR_WL0_MASK);
  uartcr |= LINFlexD_UARTCR_WL0((uint32_t)1 & 1U);
  uartcr &= ~(LINFlexD_UARTCR_WL1_MASK);
  uartcr |= LINFlexD_UARTCR_WL1(((uint32_t)1 >> 1U) & 1U);
  /* No parity (therefore parity type not used) */
  uartcr &= ~(LINFlexD_UARTCR_PCE_MASK);
  uartcr |= LINFlexD_UARTCR_PCE(0UL);
  /* Tx enabled */
  uartcr &= ~(LINFlexD_UARTCR_TxEn_MASK);
  uartcr |= LINFlexD_UARTCR_TxEn(1UL);
  /* Rx enabled */
  uartcr &= ~(LINFlexD_UARTCR_RxEn_MASK);
  uartcr |= LINFlexD_UARTCR_RxEn(1UL);
  /* Stop bits: 1 */
  uartcr &= ~(LINFlexD_UARTCR_SBUR_MASK);
  uartcr |= LINFlexD_UARTCR_SBUR(1U);
  /* Tx buffer size */
  uartcr &= ~(LINFlexD_UARTCR_TDFL_TFC_MASK);
  uartcr |= LINFlexD_UARTCR_TDFL_TFC((uint8_t)1 >> 1U);
  /* Rx buffer size */
  uartcr &= ~(LINFlexD_UARTCR_RDFL_RFC_MASK);
  uartcr |= LINFlexD_UARTCR_RDFL_RFC((uint8_t)1 >> 1U);

  if ((uint8_t)FIFO_ENABLED == fifoState) {
    /* Tx mode FIFO */
    uartcr &= ~(LINFlexD_UARTCR_TFBM_MASK);
    uartcr |= LINFlexD_UARTCR_TFBM((uint32_t)1U);
    /* Rx mode FIFO */
    uartcr &= ~(LINFlexD_UARTCR_RFBM_MASK);
    uartcr |= LINFlexD_UARTCR_RFBM((uint32_t)1U);
  }
  LINFlexD_2->UARTCR = uartcr;

  /* Clear the Zero status bit */
  LINFlexD_2->UARTSR |= LINFlexD_UARTSR_SZF_MASK;
  /* Clear DRFRFE flag - W1C */
  LINFlexD_2->UARTSR |= LINFlexD_UARTSR_DRFRFE_MASK;

  /* Configure baud rate */
  /* Fractional part of baud rate */
  LINFlexD_2->LINFBRR = (LINFlexD_2->LINFBRR & ~(LINFlexD_LINFBRR_FBR_MASK)) |
                        LINFlexD_LINFBRR_FBR(fract);
  /* Integer part of baud rate */
  LINFlexD_2->LINIBRR = (LINFlexD_2->LINIBRR & ~(LINFlexD_LINIBRR_IBR_MASK)) |
                        LINFlexD_LINIBRR_IBR(mantissa);

  /* Enter NORMAL mode */
  LINFlexD_2->LINCR1 = LINFlexD_LINCR1_INIT(0U);
}

UARTError InitializeUART(UARTBaudRate baudRate) {
  uint32_t clockFrequency;
  uint32_t realBaudRate;
  uint32_t mantissa;
  uint32_t fract;

  EchoEmulationInit(&stdOutEmu);
  stdOutEmu.inUse = 1U;

  /* Calculate fraction & mantissa assuming F80 clock */
  (void)CLOCK_SYS_GetFreq(F80_CLK, &clockFrequency);

  realBaudRate = clockFrequency / (uint32_t)baudRate;
  mantissa = realBaudRate / 16U;
  fract = (uint32_t)(realBaudRate - (uint32_t)(4U << mantissa));
  fifoState = POLLING_ENABLED;
  LINFlexD_2_Init(mantissa, fract);
  return kUARTNoError;
}

void TransmitDataImpl(const char *const pBuf) {
  (void)pBuf;
  /* Write character to transmit buffer */
  LINFlexD_2->BDRL = (LINFlexD_2->BDRL & ~LINFlexD_BDRL_DATA0_MASK) |
                     LINFlexD_BDRL_DATA0(*pBuf);
  if (1U == firstTxUARTByte) {
    firstTxUARTByte = 0U;
  }
  if ((uint8_t)POLLING_ENABLED == fifoState) {
    if (1U != firstTxUARTByte) {
      /* Wait for data transmission complete */
      while (0U == (LINFlexD_2->UARTSR & LINFlexD_UARTSR_DTFTFF_MASK)) {
      }
      /* Clear DTFTFF flag - W1C */
      LINFlexD_2->UARTSR &= LINFlexD_UARTSR_DTFTFF_MASK;
    }
  } else {
    if (1U != firstTxUARTByte) {
      /* Wait for data transmission complete */
      while (1U == (LINFlexD_2->UARTSR & LINFlexD_UARTSR_DTFTFF_MASK)) {
      }
    }
  }
}

void TransmitData(const char *const pBuf, const uint32_t cnt) {
  uint32_t j = 0U;

  (void)pBuf;
  static char zeroChar = (char)0x00;
  for (j = 0U; j < cnt; j++) {
    TransmitDataImpl(&zeroChar);
    TransmitDataImpl(&zeroChar);
    TransmitDataImpl(&pBuf[j]);
  }
}

int32_t ReceiveData(char *const pBuf) {
  UARTError err = kUARTNoData;
  uint32_t tmp = 0U;
  if ((uint8_t)POLLING_ENABLED == fifoState) {
    /* Wait for data transmission complete */
    while (0U == (LINFlexD_2->UARTSR & LINFlexD_UARTSR_DRFRFE_MASK)) {
    }
    while (0U == (LINFlexD_2->UARTSR & LINFlexD_UARTSR_RMB_MASK)) {
    }
  } else {
    /* Wait for data reception completed flag */
    while (1U == (LINFlexD_2->UARTSR & LINFlexD_UARTSR_DRFRFE_MASK)) {
    }
  }

  /* Read data from receive buffer */
  tmp = (uint8_t)((LINFlexD_2->BDRM & LINFlexD_BDRM_DATA4_MASK) >>
                  LINFlexD_BDRM_DATA4_SHIFT);
  ;
  *pBuf = (char)tmp;
  err = kUARTNoError;

  /* Clear the RMB and DRFRFE flags by writing 1 to them */
  LINFlexD_2->UARTSR |= LINFlexD_UARTSR_DRFRFE_MASK;
  if ((uint8_t)FIFO_ENABLED == fifoState) {
    LINFlexD_2->UARTSR |= LINFlexD_UARTSR_RMB_MASK;
  }
  if (1U == stdOutEmu.inUse) {
    EchoEmulationTXChar(&stdOutEmu, pBuf);
  }
  return err;
}

/*
 * Methods called by MW MSL libraries to perform console IO
 */

UARTError ReadUARTPoll(char *c) {
  uint32_t rx_data;
  int32_t status;

  while (0U == (LINFlexD_2->UARTSR & LINFlexD_UARTSR_DRFRFE_MASK)) {
  }

  /* Read whole register */
  rx_data = LINFlexD_2->BDRM;

  if (0U == (LINFlexD_2->UARTSR & LINFlexD_UARTSR_RMB_MASK)) {
    status = kUARTNoData;
  } else {
    *c = (char)rx_data;
    if (1U == stdOutEmu.inUse) {
      EchoEmulationTXChar(&stdOutEmu, c);
    }
    status = kUARTNoError;
  }
  return status;
}

UARTError ReadUARTN(void *bytes, unsigned long length) {
  uint32_t count;
  UARTError err = kUARTNoError;
  char *pBuf = (char *)bytes;

  for (count = 0; ((count < length) && (err == kUARTNoError)); count++) {
    err = ReceiveData((char *)&pBuf[count]);
  }

  return err;
}

UARTError WriteUARTN(const void *bytes, unsigned long length) {
  uint32_t j = 0U;
  const char *pBuf = (const char *)bytes;

  (void)pBuf;
  stdOutEmu.inTransmit = 1U;
  for (j = 0U; j < length; j++) {
    TransmitDataWithEscape(&stdOutEmu, pBuf[j]);
  }
  stdOutEmu.inTransmit = 0U;
  return kUARTNoError;
}
