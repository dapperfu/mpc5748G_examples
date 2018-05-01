/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : pit1.c
**     Project     : interrupt_control_multicore_mpc5748g_z4_1
**     Processor   : MPC5748G_176
**     Component   : pit
**     Version     : Component SDK_MPC574x_04, Driver 01.00, CPU db: 3.00.000
**     Repository  : SDK_MPC574x_04
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-05-01, 00:30, # CodeGen: 0
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
** @file pit1.c
** @version 01.00
*/
/*!
**  @addtogroup pit1_module pit1 module documentation
**  @{
*/

/* MODULE pit1.
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made
 * static. The external variables will be used in other source files in
 * application code.
 */

#include "pit1.h"

/*! Global configuration of pit1 */
const pit_config_t pit1_InitConfig = {.enableStandardTimers = true,
                                      .enableRTITimer = false,
                                      .stopRunInDebug = false};

/*! User channel configuration 0 */
pit_channel_config_t pit1_ChnConfig0 = {.hwChannel = 1U,
                                        .periodUnit =
                                            PIT_PERIOD_UNITS_MICROSECONDS,
                                        .period = 1000000U,
                                        .enableChain = false,
                                        .enableInterrupt = true};
/* END pit1. */
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
