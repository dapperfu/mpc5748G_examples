/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : pin_mux.c
**     Project     : hsm_freertos_mpc5748g
**     Processor   : MPC5748G_176
**     Component   : PinSettings
**     Version     : Component 1.2.0, Driver 1.4, CPU db: 3.00.000
**     Repository  : SDK_MPC574x_04
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-05-01, 00:44, # CodeGen: 0
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
** @file pin_mux.c
** @version 1.4
** @brief
**
*/
/*!
**  @addtogroup pin_mux_module pin_mux module documentation
**  @{
*/

/* MODULE pin_mux. */
#include "pin_mux.h"
#include "device_registers.h"

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, A conversion should not be performed
 * between a pointer to object and an integer type.
 * The cast is required to initialize a pointer with an unsigned int define,
 * representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, A cast shall not be performed
 * between pointer to void and an arithmetic type.
 * The cast is required to initialize a pointer with an unsigned int define,
 * representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made
 * static. The external variables will be used in other source files in
 * application code.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 9.3, partial array initialization.
 * The object array is initialized sequentially.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 9.4, Duplicate initialization of object
 * element. The object array is initialized sequentially.
 *
 */

/*! @brief Array of pin configuration structures */
pin_settings_config_t g_pin_mux_InitConfigArr[NUM_OF_CONFIGURED_PINS] = {
    {
        .base = SIUL2,
        .pinPortIdx = 41u,
        .mux = PORT_MUX_AS_GPIO,
        .outputBuffer = PORT_OUTPUT_BUFFER_DISABLED,
        .openDrain = PORT_OPEN_DRAIN_DISABLED,
        .slewRateCtrlSel = HALF_STRENGTH_WITH_SLEWRATE_CONTROL,
        .hysteresisSelect = PORT_HYSTERESYS_DISABLED,
        .safeMode = PORT_SAFE_MODE_DISABLED,
        .analogPadCtrlSel = PORT_ANALOG_PAD_CONTROL_DISABLED,
        .inputBuffer = PORT_INPUT_BUFFER_ENABLED,
        .pullConfig = PORT_INTERNAL_PULL_NOT_ENABLED,
        .gpioBase = PTC,
        .intConfig = {.intEdgeSel = SIUL2_INT_DISABLE},
        .inputMux[0] = PORT_INPUT_MUX_ALT2,
        .inputMuxReg[0] = 202,
        .inputMux[1] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[2] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[3] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[4] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[5] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[6] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[7] = PORT_INPUT_MUX_NO_INIT,
    },
    {
        .base = SIUL2,
        .pinPortIdx = 25u,
        .mux = PORT_MUX_AS_GPIO,
        .outputBuffer = PORT_OUTPUT_BUFFER_DISABLED,
        .openDrain = PORT_OPEN_DRAIN_DISABLED,
        .slewRateCtrlSel = HALF_STRENGTH_WITH_SLEWRATE_CONTROL,
        .hysteresisSelect = PORT_HYSTERESYS_DISABLED,
        .safeMode = PORT_SAFE_MODE_DISABLED,
        .analogPadCtrlSel = PORT_ANALOG_PAD_CONTROL_ENABLED,
        .inputBuffer = PORT_INPUT_BUFFER_ENABLED,
        .pullConfig = PORT_INTERNAL_PULL_NOT_ENABLED,
        .gpioBase = PTB,
        .intConfig = {.intEdgeSel = SIUL2_INT_DISABLE},
        .inputMux[0] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[1] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[2] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[3] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[4] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[5] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[6] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[7] = PORT_INPUT_MUX_NO_INIT,
    },
    {
        .base = SIUL2,
        .pinPortIdx = 24u,
        .mux = PORT_MUX_AS_GPIO,
        .outputBuffer = PORT_OUTPUT_BUFFER_DISABLED,
        .openDrain = PORT_OPEN_DRAIN_DISABLED,
        .slewRateCtrlSel = HALF_STRENGTH_WITH_SLEWRATE_CONTROL,
        .hysteresisSelect = PORT_HYSTERESYS_DISABLED,
        .safeMode = PORT_SAFE_MODE_DISABLED,
        .analogPadCtrlSel = PORT_ANALOG_PAD_CONTROL_ENABLED,
        .inputBuffer = PORT_INPUT_BUFFER_ENABLED,
        .pullConfig = PORT_INTERNAL_PULL_NOT_ENABLED,
        .gpioBase = PTB,
        .intConfig = {.intEdgeSel = SIUL2_INT_DISABLE},
        .inputMux[0] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[1] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[2] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[3] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[4] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[5] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[6] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[7] = PORT_INPUT_MUX_NO_INIT,
    },
    {
        .base = SIUL2,
        .pinPortIdx = 48u,
        .mux = PORT_MUX_AS_GPIO,
        .outputBuffer = PORT_OUTPUT_BUFFER_DISABLED,
        .openDrain = PORT_OPEN_DRAIN_DISABLED,
        .slewRateCtrlSel = HALF_STRENGTH_WITH_SLEWRATE_CONTROL,
        .hysteresisSelect = PORT_HYSTERESYS_DISABLED,
        .safeMode = PORT_SAFE_MODE_DISABLED,
        .analogPadCtrlSel = PORT_ANALOG_PAD_CONTROL_ENABLED,
        .inputBuffer = PORT_INPUT_BUFFER_ENABLED,
        .pullConfig = PORT_INTERNAL_PULL_NOT_ENABLED,
        .gpioBase = PTD,
        .intConfig = {.intEdgeSel = SIUL2_INT_DISABLE},
        .inputMux[0] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[1] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[2] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[3] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[4] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[5] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[6] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[7] = PORT_INPUT_MUX_NO_INIT,
    },
    {
        .base = SIUL2,
        .pinPortIdx = 49u,
        .mux = PORT_MUX_AS_GPIO,
        .outputBuffer = PORT_OUTPUT_BUFFER_DISABLED,
        .openDrain = PORT_OPEN_DRAIN_DISABLED,
        .slewRateCtrlSel = HALF_STRENGTH_WITH_SLEWRATE_CONTROL,
        .hysteresisSelect = PORT_HYSTERESYS_DISABLED,
        .safeMode = PORT_SAFE_MODE_DISABLED,
        .analogPadCtrlSel = PORT_ANALOG_PAD_CONTROL_ENABLED,
        .inputBuffer = PORT_INPUT_BUFFER_ENABLED,
        .pullConfig = PORT_INTERNAL_PULL_NOT_ENABLED,
        .gpioBase = PTD,
        .intConfig = {.intEdgeSel = SIUL2_INT_DISABLE},
        .inputMux[0] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[1] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[2] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[3] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[4] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[5] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[6] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[7] = PORT_INPUT_MUX_NO_INIT,
    },
    {
        .base = SIUL2,
        .pinPortIdx = 50u,
        .mux = PORT_MUX_AS_GPIO,
        .outputBuffer = PORT_OUTPUT_BUFFER_DISABLED,
        .openDrain = PORT_OPEN_DRAIN_DISABLED,
        .slewRateCtrlSel = HALF_STRENGTH_WITH_SLEWRATE_CONTROL,
        .hysteresisSelect = PORT_HYSTERESYS_DISABLED,
        .safeMode = PORT_SAFE_MODE_DISABLED,
        .analogPadCtrlSel = PORT_ANALOG_PAD_CONTROL_ENABLED,
        .inputBuffer = PORT_INPUT_BUFFER_ENABLED,
        .pullConfig = PORT_INTERNAL_PULL_NOT_ENABLED,
        .gpioBase = PTD,
        .intConfig = {.intEdgeSel = SIUL2_INT_DISABLE},
        .inputMux[0] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[1] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[2] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[3] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[4] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[5] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[6] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[7] = PORT_INPUT_MUX_NO_INIT,
    },
    {
        .base = SIUL2,
        .pinPortIdx = 51u,
        .mux = PORT_MUX_AS_GPIO,
        .outputBuffer = PORT_OUTPUT_BUFFER_DISABLED,
        .openDrain = PORT_OPEN_DRAIN_DISABLED,
        .slewRateCtrlSel = HALF_STRENGTH_WITH_SLEWRATE_CONTROL,
        .hysteresisSelect = PORT_HYSTERESYS_DISABLED,
        .safeMode = PORT_SAFE_MODE_DISABLED,
        .analogPadCtrlSel = PORT_ANALOG_PAD_CONTROL_ENABLED,
        .inputBuffer = PORT_INPUT_BUFFER_ENABLED,
        .pullConfig = PORT_INTERNAL_PULL_NOT_ENABLED,
        .gpioBase = PTD,
        .intConfig = {.intEdgeSel = SIUL2_INT_DISABLE},
        .inputMux[0] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[1] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[2] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[3] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[4] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[5] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[6] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[7] = PORT_INPUT_MUX_NO_INIT,
    },
    {
        .base = SIUL2,
        .pinPortIdx = 52u,
        .mux = PORT_MUX_AS_GPIO,
        .outputBuffer = PORT_OUTPUT_BUFFER_DISABLED,
        .openDrain = PORT_OPEN_DRAIN_DISABLED,
        .slewRateCtrlSel = HALF_STRENGTH_WITH_SLEWRATE_CONTROL,
        .hysteresisSelect = PORT_HYSTERESYS_DISABLED,
        .safeMode = PORT_SAFE_MODE_DISABLED,
        .analogPadCtrlSel = PORT_ANALOG_PAD_CONTROL_ENABLED,
        .inputBuffer = PORT_INPUT_BUFFER_ENABLED,
        .pullConfig = PORT_INTERNAL_PULL_NOT_ENABLED,
        .gpioBase = PTD,
        .intConfig = {.intEdgeSel = SIUL2_INT_DISABLE},
        .inputMux[0] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[1] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[2] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[3] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[4] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[5] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[6] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[7] = PORT_INPUT_MUX_NO_INIT,
    },
    {
        .base = SIUL2,
        .pinPortIdx = 53u,
        .mux = PORT_MUX_AS_GPIO,
        .outputBuffer = PORT_OUTPUT_BUFFER_DISABLED,
        .openDrain = PORT_OPEN_DRAIN_DISABLED,
        .slewRateCtrlSel = HALF_STRENGTH_WITH_SLEWRATE_CONTROL,
        .hysteresisSelect = PORT_HYSTERESYS_DISABLED,
        .safeMode = PORT_SAFE_MODE_DISABLED,
        .analogPadCtrlSel = PORT_ANALOG_PAD_CONTROL_ENABLED,
        .inputBuffer = PORT_INPUT_BUFFER_ENABLED,
        .pullConfig = PORT_INTERNAL_PULL_NOT_ENABLED,
        .gpioBase = PTD,
        .intConfig = {.intEdgeSel = SIUL2_INT_DISABLE},
        .inputMux[0] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[1] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[2] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[3] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[4] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[5] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[6] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[7] = PORT_INPUT_MUX_NO_INIT,
    },
    {
        .base = SIUL2,
        .pinPortIdx = 54u,
        .mux = PORT_MUX_AS_GPIO,
        .outputBuffer = PORT_OUTPUT_BUFFER_DISABLED,
        .openDrain = PORT_OPEN_DRAIN_DISABLED,
        .slewRateCtrlSel = HALF_STRENGTH_WITH_SLEWRATE_CONTROL,
        .hysteresisSelect = PORT_HYSTERESYS_DISABLED,
        .safeMode = PORT_SAFE_MODE_DISABLED,
        .analogPadCtrlSel = PORT_ANALOG_PAD_CONTROL_ENABLED,
        .inputBuffer = PORT_INPUT_BUFFER_ENABLED,
        .pullConfig = PORT_INTERNAL_PULL_NOT_ENABLED,
        .gpioBase = PTD,
        .intConfig = {.intEdgeSel = SIUL2_INT_DISABLE},
        .inputMux[0] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[1] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[2] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[3] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[4] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[5] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[6] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[7] = PORT_INPUT_MUX_NO_INIT,
    },
    {
        .base = SIUL2,
        .pinPortIdx = 55u,
        .mux = PORT_MUX_AS_GPIO,
        .outputBuffer = PORT_OUTPUT_BUFFER_DISABLED,
        .openDrain = PORT_OPEN_DRAIN_DISABLED,
        .slewRateCtrlSel = HALF_STRENGTH_WITH_SLEWRATE_CONTROL,
        .hysteresisSelect = PORT_HYSTERESYS_DISABLED,
        .safeMode = PORT_SAFE_MODE_DISABLED,
        .analogPadCtrlSel = PORT_ANALOG_PAD_CONTROL_ENABLED,
        .inputBuffer = PORT_INPUT_BUFFER_ENABLED,
        .pullConfig = PORT_INTERNAL_PULL_NOT_ENABLED,
        .gpioBase = PTD,
        .intConfig = {.intEdgeSel = SIUL2_INT_DISABLE},
        .inputMux[0] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[1] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[2] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[3] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[4] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[5] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[6] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[7] = PORT_INPUT_MUX_NO_INIT,
    },
    {
        .base = SIUL2,
        .pinPortIdx = 56u,
        .mux = PORT_MUX_AS_GPIO,
        .outputBuffer = PORT_OUTPUT_BUFFER_DISABLED,
        .openDrain = PORT_OPEN_DRAIN_DISABLED,
        .slewRateCtrlSel = HALF_STRENGTH_WITH_SLEWRATE_CONTROL,
        .hysteresisSelect = PORT_HYSTERESYS_DISABLED,
        .safeMode = PORT_SAFE_MODE_DISABLED,
        .analogPadCtrlSel = PORT_ANALOG_PAD_CONTROL_ENABLED,
        .inputBuffer = PORT_INPUT_BUFFER_ENABLED,
        .pullConfig = PORT_INTERNAL_PULL_NOT_ENABLED,
        .gpioBase = PTD,
        .intConfig = {.intEdgeSel = SIUL2_INT_DISABLE},
        .inputMux[0] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[1] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[2] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[3] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[4] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[5] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[6] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[7] = PORT_INPUT_MUX_NO_INIT,
    },
    {
        .base = SIUL2,
        .pinPortIdx = 20u,
        .mux = PORT_MUX_AS_GPIO,
        .outputBuffer = PORT_OUTPUT_BUFFER_DISABLED,
        .openDrain = PORT_OPEN_DRAIN_DISABLED,
        .slewRateCtrlSel = HALF_STRENGTH_WITH_SLEWRATE_CONTROL,
        .hysteresisSelect = PORT_HYSTERESYS_DISABLED,
        .safeMode = PORT_SAFE_MODE_DISABLED,
        .analogPadCtrlSel = PORT_ANALOG_PAD_CONTROL_ENABLED,
        .inputBuffer = PORT_INPUT_BUFFER_ENABLED,
        .pullConfig = PORT_INTERNAL_PULL_NOT_ENABLED,
        .gpioBase = PTB,
        .intConfig = {.intEdgeSel = SIUL2_INT_DISABLE},
        .inputMux[0] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[1] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[2] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[3] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[4] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[5] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[6] = PORT_INPUT_MUX_NO_INIT,
        .inputMux[7] = PORT_INPUT_MUX_NO_INIT,
    },
    {
        .base = SIUL2,
        .pinPortIdx = 40u,
        .mux = PORT_MUX_ALT1,
        .outputBuffer = PORT_OUTPUT_BUFFER_ENABLED,
        .openDrain = PORT_OPEN_DRAIN_DISABLED,
        .slewRateCtrlSel = HALF_STRENGTH_WITH_SLEWRATE_CONTROL,
        .hysteresisSelect = PORT_HYSTERESYS_DISABLED,
        .safeMode = PORT_SAFE_MODE_DISABLED,
        .analogPadCtrlSel = PORT_ANALOG_PAD_CONTROL_DISABLED,
        .inputBuffer = PORT_INPUT_BUFFER_DISABLED,
        .pullConfig = PORT_INTERNAL_PULL_UP_ENABLED,
        .gpioBase = PTC,
    },
};

/* END pin_mux. */
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
