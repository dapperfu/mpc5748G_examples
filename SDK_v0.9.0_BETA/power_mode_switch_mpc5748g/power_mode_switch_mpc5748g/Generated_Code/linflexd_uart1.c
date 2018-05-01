/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : linflexd_uart1.c
**     Project     : power_mode_switch_mpc5748g
**     Processor   : MPC5748G_176
**     Component   : linflexd_uart
**     Version     : Component SDK_MPC574x_04, Driver 01.00, CPU db: 3.00.000
**     Repository  : SDK_MPC574x_04
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-05-01, 00:21, # CodeGen: 0
**     Abstract    :
**
**
**     Copyright 1997 - 2015 Freescale Semiconductor, Inc.
**     Copyright 2016-2017 NXP
**     All Rights Reserved.
**
**     THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
**     IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
**     OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
**     IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
**     INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
**     SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
**     HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
**     STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
**     IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
**     THE POSSIBILITY OF SUCH DAMAGE.
** ###################################################################*/
/*!
** @file linflexd_uart1.c
** @version 01.00
** @brief
**
*/
/*!
**  @addtogroup linflexd_uart1_module linflexd_uart1 module documentation
**  @{
*/

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The structures are defined for application use.
 */

/* MODULE linflexd_uart1. */

#include "linflexd_uart1.h"

/*! linflexd_uart1 configuration structure */

linflexd_uart_state_t linflexd_uart1_State;

const linflexd_uart_user_config_t linflexd_uart1_InitConfig0 = {
    .transferType = LINFLEXD_UART_USING_INTERRUPTS,
    .baudRate = 9600U,
    .parityCheck = false,
    .parityType = LINFLEXD_UART_PARITY_EVEN,
    .stopBitsCount = LINFLEXD_UART_ONE_STOP_BIT,
    .wordLength = LINFLEXD_UART_8_BITS,
    .rxDMAChannel = 0U,
    .txDMAChannel = 0U,
};

/* END linflexd_uart1. */
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
