/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : sai1.c
**     Project     : sai_transfer_mpc5748g
**     Processor   : MPC5748G_256
**     Component   : sai
**     Version     : Component SDK_MPC574x_04, Driver 01.00, CPU db: 3.00.000
**     Repository  : SDK_MPC574x_04
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-05-01, 01:20, # CodeGen: 0
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
** @file sai1.c
** @version 01.00
** @brief
**
*/
/*!
**  @addtogroup sai1_module sai1 module documentation
**  @{
*/

/* MODULE sai1. */

#include "sai1.h"

/*! @brief Define external callbacks */
/*! sai1 configuration structure */
sai_user_config_t sai1_InitConfig0 = {
    .BitClkDiv = 0U,
    .BitClkFreq = 0U,
    .BitClkInternal = false,
    .ChannelEnable = SAI_CHANNEL_0,
    .BitClkNegPolar = false,
    .ElementSize = 2U,
    .FirstBitIndex = 0U,
    .FrameSize = 2U,
    .MsbFirst = false,
    .MuxMode = SAI_MUX_DISABLED,
    .ChannelCount = 2U,
    .DmaChannel[0] = 16U,
    .DmaChannel[1] = 0U,
    .DmaChannel[2] = 0U,
    .DmaChannel[3] = 0U,
    .RunErrorReport = false,
    .SyncErrorReport = false,
    .FrameStartReport = false,
    .SyncEarly = false,
    .SyncInternal = false,
    .SyncMode = SAI_ASYNC,
    .SyncNegPolar = false,
    .SyncWidth = 16U,
    .TransferType = SAI_INTERRUPT,
    .Word0Width = 16U,
    .WordNWidth = 16U,
    .callback = NULL,
};
/*! Driver state structure */
sai_state_t sai1RxState;
/* END sai1. */
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
