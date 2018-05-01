/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : i2s_pal1.c
**     Project     : i2s_pal_mpc5748g
**     Processor   : MPC5748G_176
**     Component   : i2s_pal
**     Version     : Component SDK_MPC574x_04, Driver 01.00, CPU db: 3.00.000
**     Repository  : SDK_MPC574x_04
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-05-01, 00:34, # CodeGen: 0
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
** @file i2s_pal1.c
** @version 01.00
*/
/*!
**  @addtogroup i2s_pal1_module i2s_pal1 module documentation
**  @{
*/
#include "i2s_pal1.h"

/*! @brief Define external callbacks */
extern void callback(i2s_event_t event, void *userData);
/* Serial User Configurations */
i2s_user_config_t i2s_pal1_Config0 = {.baudRate = 100000,
                                      .mode = I2S_MASTER,
                                      .transferType = I2S_USING_INTERRUPT,
                                      .wordWidth = 16,
                                      .rxDMAChannel = 0U,
                                      .txDMAChannel = 0U,
                                      .callback = callback,
                                      .extension = NULL};

/*! @brief Device instance number */
const i2s_instance_t i2sOverSai0Instance = {I2S_INST_TYPE_SAI, 0UL};

/* END i2s_pal1. */
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
