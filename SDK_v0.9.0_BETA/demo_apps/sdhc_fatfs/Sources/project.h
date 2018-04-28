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
**     Project     : sdhc_fatfs_mpc5748g
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
#define PCR_PA1  1
#define PCR_PA2  2
#define PCR_PA3  3
#define PCR_PA4  4
#define PCR_PA5  5
#define PCR_PA6  6
#define PCR_PA7  7
#define PCR_PA8  8
#define PCR_PA9  9
#define PCR_PA10  10
#define PCR_PA11  11
#define PCR_PA12  12
#define PCR_PA13  13
#define PCR_PA14  14
#define PCR_PA15  15
#define PCR_PB0  16
#define PCR_PB1  17
#define PCR_PB2  18
#define PCR_PB3  19
#define PCR_PB4  20
#define PCR_PB5  21
#define PCR_PB6  22
#define PCR_PB7  23
#define PCR_PB8  24
#define PCR_PB9  25
#define PCR_PB10  26
#define PCR_PB11  27
#define PCR_PB12  28
#define PCR_PB13  29
#define PCR_PB14  30
#define PCR_PB15  31
#define PCR_PC0  32
#define PCR_PC1  33
#define PCR_PC2  34
#define PCR_PC3  35
#define PCR_PC4  36
#define PCR_PC5  37
#define PCR_PC6  38
#define PCR_PC7  39
#define PCR_PC8  40
#define PCR_PC9  41
#define PCR_PC10  42
#define PCR_PC11  43
#define PCR_PC12  44
#define PCR_PC13  45
#define PCR_PC14  46
#define PCR_PC15  47
#define PCR_PD0  48
#define PCR_PD1  49
#define PCR_PD2  50
#define PCR_PD3  51
#define PCR_PD4  52
#define PCR_PD5  53
#define PCR_PD6  54
#define PCR_PD7  55
#define PCR_PD8  56
#define PCR_PD9  57
#define PCR_PD10  58
#define PCR_PD11  59
#define PCR_PD12  60
#define PCR_PD13  61
#define PCR_PD14  62
#define PCR_PD15  63
#define PCR_PE0  64
#define PCR_PE1  65
#define PCR_PE2  66
#define PCR_PE3  67
#define PCR_PE4  68
#define PCR_PE5  69
#define PCR_PE6  70
#define PCR_PE7  71
#define PCR_PE8  72
#define PCR_PE9  73
#define PCR_PE10  74
#define PCR_PE11  75
#define PCR_PE12  76
#define PCR_PE13  77
#define PCR_PE14  78
#define PCR_PE15  79
#define PCR_PF0  80
#define PCR_PF1  81
#define PCR_PF2  82
#define PCR_PF3  83
#define PCR_PF4  84
#define PCR_PF5  85
#define PCR_PF6  86
#define PCR_PF7  87
#define PCR_PF8  88
#define PCR_PF9  89
#define PCR_PF10  90
#define PCR_PF11  91
#define PCR_PF12  92
#define PCR_PF13  93
#define PCR_PF14  94
#define PCR_PF15  95
#define PCR_PG0  96
#define PCR_PG1  97
#define PCR_PG2  98
#define PCR_PG3  99
#define PCR_PG4  100
#define PCR_PG5  101
#define PCR_PG6  102
#define PCR_PG7  103
#define PCR_PG8  104
#define PCR_PG9  105
#define PCR_PG10  106
#define PCR_PG11  107
#define PCR_PG12  108
#define PCR_PG13  109
#define PCR_PG14  110
#define PCR_PG15  111
#define PCR_PH0  112
#define PCR_PH1  113
#define PCR_PH2  114
#define PCR_PH3  115
#define PCR_PH4  116
#define PCR_PH5  117
#define PCR_PH6  118
#define PCR_PH7  119
#define PCR_PH8  120
#define PCR_PH9  121
#define PCR_PH10  122
#define PCR_PH11  123
#define PCR_PH12  124
#define PCR_PH13  125
#define PCR_PH14  126
#define PCR_PH15  127
#define PCR_PI0  128
#define PCR_PI1  129
#define PCR_PI2  130
#define PCR_PI3  131
#define PCR_PI4  132
#define PCR_PI5  133
#define PCR_PI6  134
#define PCR_PI7  135
#define PCR_PI8  136
#define PCR_PI9  137
#define PCR_PI10  138
#define PCR_PI11  139
#define PCR_PI12  140
#define PCR_PI13  141
#define PCR_PI14  142
#define PCR_PI15  143
#define PCR_PJ0  144
#define PCR_PJ1  145
#define PCR_PJ2  146
#define PCR_PJ3  147
#define PCR_PJ4  148
#define PCR_PJ5  149
#define PCR_PJ6  150
#define PCR_PJ7  151
#define PCR_PJ8  152
#define PCR_PJ9  153
#define PCR_PJ10  154
#define PCR_PJ11  155
#define PCR_PJ12  156
#define PCR_PJ13  157
#define PCR_PJ14  158
#define PCR_PJ15  159
#define PCR_PK0  160
#define PCR_PK1  161
#define PCR_PK2  162
#define PCR_PK3  163
#define PCR_PK4  164
#define PCR_PK5  165
#define PCR_PK6  166
#define PCR_PK7  167
#define PCR_PK8  168
#define PCR_PK9  169
#define PCR_PK10  170
#define PCR_PK11  171
#define PCR_PK12  172
#define PCR_PK13  173
#define PCR_PK14  174
#define PCR_PK15  175
#define PCR_PL0  176
#define PCR_PL1  177
#define PCR_PL2  178
#define PCR_PL3  179
#define PCR_PL4  180
#define PCR_PL5  181
#define PCR_PL6  182
#define PCR_PL7  183
#define PCR_PL8  184
#define PCR_PL9  185
#define PCR_PL10  186
#define PCR_PL11  187
#define PCR_PL12  188
#define PCR_PL13  189
#define PCR_PL14  190
#define PCR_PL15  191
#define PCR_PM0  192
#define PCR_PM1  193
#define PCR_PM2  194
#define PCR_PM3  195
#define PCR_PM4  196
#define PCR_PM5  197
#define PCR_PM6  198
#define PCR_PM7  199
#define PCR_PM8  200
#define PCR_PM9  201
#define PCR_PM10  202
#define PCR_PM11  203
#define PCR_PM12  204
#define PCR_PM13  205
#define PCR_PM14  206
#define PCR_PM15  207
#define PCR_PN0  208
#define PCR_PN1  209
#define PCR_PN2  210
#define PCR_PN3  211
#define PCR_PN4  212
#define PCR_PN5  213
#define PCR_PN6  214
#define PCR_PN7  215
#define PCR_PN8  216
#define PCR_PN9  217
#define PCR_PN10  218
#define PCR_PN11  219
#define PCR_PN12  220
#define PCR_PN13  221
#define PCR_PN14  222
#define PCR_PN15  223
#define PCR_PO0  224
#define PCR_PO1  225
#define PCR_PO2  226
#define PCR_PO3  227
#define PCR_PO4  228
#define PCR_PO5  229
#define PCR_PO6  230
#define PCR_PO7  231
#define PCR_PO8  232
#define PCR_PO9  233
#define PCR_PO10  234
#define PCR_PO11  235
#define PCR_PO12  236
#define PCR_PO13  237
#define PCR_PO14  238
#define PCR_PO15  239
#define PCR_PP0  240
#define PCR_PP1  241
#define PCR_PP2  242
#define PCR_PP3  243
#define PCR_PP4  244
#define PCR_PP5  245
#define PCR_PP6  246
#define PCR_PP7  247
#define PCR_PP8  248
#define PCR_PP9  249
#define PCR_PP10  250
#define PCR_PP11  251
#define PCR_PP12  252
#define PCR_PP13  253
#define PCR_PP14  254
#define PCR_PP15  255
#define PCR_PQ0  256
#define PCR_PQ1  257
#define PCR_PQ2  258
#define PCR_PQ3  259
#define PCR_PQ4  260
#define PCR_PQ5  261
#define PCR_PQ6  262
#define PCR_PQ7  263

/*! @brief GPIO available in LPU_RUN and Pad Keeper in STANDBY */
#define LPU_A0	0
#define LPU_A1	1
#define LPU_A2	2
#define LPU_A4	4
#define LPU_B12	12
#define LPU_B13	13
#define LPU_B14	14
#define LPU_B15	15
#define LPU_B0	16
#define LPU_B1	17
#define LPU_B3	19
#define LPU_C10	26
#define LPU_C11	27
#define LPU_C12	28
#define LPU_C0	32
#define LPU_C1	33
#define LPU_C6	38
#define LPU_C7	39
#define LPU_D9	41
#define LPU_D10	42
#define LPU_D11	43
#define LPU_E12	60
#define LPU_E13	61
#define LPU_E0	64
#define LPU_E3	67
#define LPU_E5	69
#define LPU_F9	73
#define LPU_F11	75
#define LPU_F0	80
#define LPU_F1	81
#define LPU_F2	82
#define LPU_F3	83
#define LPU_F4	84
#define LPU_F5	85
#define LPU_F6	86
#define LPU_F7	87
#define LPU_G8	88
#define LPU_G9	89
#define LPU_G10	90
#define LPU_G11	91
#define LPU_G12	92
#define LPU_G13	93
#define LPU_G3	99
#define LPU_G5	101
#define LPU_G6	102
#define LPU_G7	103
#define LPU_H8	104
#define LPU_H9	105
#define LPU_I9	121
#define LPU_I10	122
#define LPU_I1	129
#define LPU_I3	131
#define LPU_J9	137
#define LPU_J10	138
#define LPU_J14	142
#define LPU_J15	143
#define LPU_J0	144
#define LPU_J1	145
#define LPU_J2	146
#define LPU_J3	147
#define LPU_J5	149
#define LPU_J6	150
#define LPU_J7	151
#define LPU_K8	152
#define LPU_K9	153
#define LPU_K10	154
#define LPU_K11	155
#define LPU_K12	156
#define LPU_K13	157
#define LPU_K14	158
#define LPU_K15	159
#define LPU_K0	160
#define LPU_K1	161
#define LPU_K2	162
#define LPU_K3	163
#define LPU_K4	164
#define LPU_K5	165
#define LPU_K6	166
#define LPU_K7	167
#define LPU_L8	168
#define LPU_M15	207
#define LPU_M0	208
#define LPU_M1	209
#define LPU_M2	210
#define LPU_M3	211
#define LPU_M4	212
#define LPU_M5	213
#define LPU_M6	214
#define LPU_M7	215
#define LPU_N8	216
#define LPU_N9	217
#define LPU_N10	218
#define LPU_N11	219
#define LPU_N12	220
#define LPU_N13	221

#define PCTL_BCTU	0
#define PCTL_EMIOS_0	1
#define PCTL_EMIOS_1	2
#define PCTL_EMIOS_2	3
#define PCTL_USB	4
#define PCTL_USB_SPH	5
#define PCTL_MLB	6
#define PCTL_SDHC	7
#define PCTL_SAI0	10
#define PCTL_SAI1	11
#define PCTL_SAI2	12
#define PCTL_ENET	15
#define PCTL_CMP_0	20
#define PCTL_CMP_1	21
#define PCTL_CMP_2	22
#define PCTL_ADC_0	24
#define PCTL_ADC_1	25
#define PCTL_FLEXRAY	28
#define PCTL_IIC_0	30
#define PCTL_IIC_1	31
#define PCTL_IIC_2	32
#define PCTL_IIC_3	33
#define PCTL_DSPI_0	40
#define PCTL_DSPI_1	41
#define PCTL_DSPI_2	42
#define PCTL_DSPI_3	43
#define PCTL_LIN_0	50
#define PCTL_LIN_1	51
#define PCTL_LIN_2	52
#define PCTL_LIN_3	53
#define PCTL_LIN_4	54
#define PCTL_LIN_5	55
#define PCTL_LIN_6	56
#define PCTL_LIN_7	57
#define PCTL_LIN_8	58
#define PCTL_LIN_9	59
#define PCTL_LIN_10	60
#define PCTL_LIN_11	61
#define PCTL_LIN_12	62
#define PCTL_LIN_13	63
#define PCTL_LIN_14	64
#define PCTL_LIN_15	65
#define PCTL_LIN_16	66
#define PCTL_LIN_17	67
#define PCTL_FLEXCAN_0	70
#define PCTL_FLEXCAN_1	71
#define PCTL_FLEXCAN_2	72
#define PCTL_FLEXCAN_3	73
#define PCTL_FLEXCAN_4	74
#define PCTL_FLEXCAN_5	75
#define PCTL_FLEXCAN_6	76
#define PCTL_FLEXCAN_7	77
#define PCTL_HSM	80
#define PCTL_TDM	81
#define PCTL_JDC	82
#define PCTL_MEMU_0	83
#define PCTL_CRC	84
#define PCTL_DMAMUX	90
#define PCTL_PIT_RTI_0	91
#define PCTL_WKPU	93
#define PCTL_SIU 	94
#define PCTL_SPI0	96
#define PCTL_SPI1	97
#define PCTL_SPI2	98
#define PCTL_SPI3	99
#define PCTL_SPI4	100
#define PCTL_SPI5	101
#define PCTL_RTC_API	102
#define PCTL_PASS	104

/*!
 * @brief Define kit
 *
 * Please define it if you have DEVKIT. MAINBOARD is the default.
 *
 */
#if 0
#define DEVKIT 1
#else
#define MAINBOARD 1
#endif

#else

#endif /* PROJECT_H_ */
