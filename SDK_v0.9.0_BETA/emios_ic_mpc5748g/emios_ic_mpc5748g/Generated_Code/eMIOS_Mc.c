/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : eMIOS_Mc.c
**     Project     : emios_ic_mpc5748g
**     Processor   : MPC5748G_176
**     Component   : emios_mc
**     Version     : Component SDK_MPC574x_04, Driver 01.00, CPU db: 3.00.000
**     Repository  : SDK_MPC574x_04
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-04-30, 19:57, # CodeGen: 0
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
** @file eMIOS_Mc.c
** @version 01.00
*/
/*!
**  @addtogroup eMIOS_Mc_module eMIOS_Mc module documentation
**  @{
*/

/* Module eMIOS_Mc.
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.4, external symbol defined without a
 * prior declaration. The symbols are declared in the driver header as external;
 * the header is not included by this file.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made
 * static. The external variable will be used in other source files in
 * application code.
 */

#include "eMIOS_Mc.h"

/* eMIOS global configuration structure for eMIOS_Mc_InitConfig0 */
emios_common_param_t eMIOS_Mc_InitConfig0 = {
    false, /*!< If true, all channel in eMIOS group can enter debug mode */
    false, /*!< Low power mode or normal mode */
    1U,    /*!< Select the clock divider value for the global prescaler in range
              (1-256) */
    false, /*!< Enable or disable global prescaler */
    false  /*!< Enable or disable global timebase */
};

/* eMIOS counter mode configuration structure for eMIOS_Mc_CntChnConfig0 */
emios_mc_mode_param_t eMIOS_Mc_CntChnConfig0 = {
    EMIOS_MODE_MC_UP_CNT_CLR_START_INT_CLK,
    10000UL,                   /*!< Period value */
    EMIOS_CLOCK_DIVID_BY_1,    /*!< Internal prescaler value */
    true,                      /*!< Enable internal prescaler */
    EMIOS_INPUT_FILTER_BYPASS, /*!< Filter value, ignore if not select external
                                  clock mode */
    false, /*!< Input capture filter state, ignore if not select external clock
              mode */
    EMIOS_TRIGGER_EDGE_FALLING /*!< Input signal trigger mode, ignore if not
                                  select external clock mode */
};

/* END eMIOS_Mc. */

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
