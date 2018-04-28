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
**     Filename    : project.h
**     Project     : sdhc_freertos_mpc5748g
**     Processor   : MPC5748G_324
**     Version     : 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   :
**     Abstract    :
**         This module contains macro declaration for pin pads, LPU and PCTL.
**     Settings    :
**     Contents    :
**
** ###################################################################*/

#ifndef PROJECT_H_
#define PROJECT_H_

#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief GPIO PCRs */

#define PCR_PA0  0
#define PCR_PA4  4
#define PCR_PA10  10
#define PCR_PC8  40
#define PCR_PG2  98
#define PCR_PG3  99
#define PCR_PG4  100
#define PCR_PG5  101
#define PCR_PJ4  148

/*!
 * @brief Define kit
 *
 * Please define it if you have DEVKIT. MAINBOARD is the default.
 *
 */
#if 1
#define DEVKIT 1
#else
#define MAINBOARD 1
#endif

#else

#endif /* PROJECT_H_ */


