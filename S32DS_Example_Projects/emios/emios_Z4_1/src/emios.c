/*****************************************************************************/
/* FILE NAME: eMIOS.c                  COPYRIGHT (c) NXP Semiconductors 2016 */
/*                                                      All Rights Reserved  */
/* PLATFORM: DEVKIT-MPC5748G												 */
/* DESCRIPTION: eMIOS modules functions                                      */
/*                                                                           */
/*****************************************************************************/
/* REV      AUTHOR        DATE        DESCRIPTION OF CHANGE                  */
/* ---   -----------    ----------    ---------------------                  */
/* 1.0	  S Mihalik     05 Mar 2014   Initial Version                        */
/* 2.0	  S.M./S.O.     31 Mar 2014   Modified for adding input functions    */
/* 2.1	  K Shah		26 Feb 2016	  Ported to S32DS						 */
/*****************************************************************************/


#include "emios.h"
#include "derivative.h"

void init_emios_global_prescalers(void){
  eMIOS_0.MCR.B.GPREN = 0;  /* Disable global pre-scaler (reset default) */
  eMIOS_0.MCR.B.GPRE = 79;  /* Divide 80 MHz clock to module by 80 */
  eMIOS_0.MCR.B.FRZ = 0;    /* No Freeze channel registers in debug mode if channel FREN=0 */
  eMIOS_1.MCR.B.GPREN = 0;  /* Disable global pre-scaler (reset default) */
  eMIOS_1.MCR.B.GPRE = 79;  /* Divide 80 MHz clock to module by 80 */
  eMIOS_1.MCR.B.FRZ = 0;    /* No Freeze channel registers in debug mode if channel FREN=0 */
}

void emios0_ch23_OPWFMB (void) { 	/* Configures eMIOS 0 channel 23: OPWFMB, 200 count*/
  eMIOS_UC_0.UC[23].C.R = 0x0;   	/* Disable channel pre-scaler (reset default) */
  eMIOS_UC_0.UC[23].A.R =  19;   	/* OPWFMB mode:  duty cycle count */
  eMIOS_UC_0.UC[23].B.R = 200;   	/* OPWFMB mode:  period will be 200 counts */
  eMIOS_UC_0.UC[23].CNT.R = 1;      /* OPWFMB start counter between 1 & B register value*/
  eMIOS_UC_0.UC[23].C.B.EDPOL = 1;  /* Output polarity on A match */
  eMIOS_UC_0.UC[23].C.B.MODE = 0x58;/* Output Pulse Width & Frequency Modulation*/
  eMIOS_UC_0.UC[23].C.B.UCPRE = 0;  /* Pre-scale channel clock by 0+1=1 */
  eMIOS_UC_0.UC[23].C.B.UCPREN= 1;  /* Enable pre-scaler */

  SIUL2.MSCR[PF6].B.SSS = 1;      /* Pad PF6: Source signal is E0UC_23_X */
  SIUL2.MSCR[PF6].B.OBE = 1;      /* Pad PF6: OBE=1. */
  SIUL2.MSCR[PF6].B.SRC = 3;      /* Pad PF6: Full strength slew rate */
}

void emios1_ch23_MCB(void) {    	/* Configures eMIOS 1 channel 23: MCB, 1K count */
  eMIOS_UC_1.UC[23].C.R = 0x0;  	/* Disable channel pre-scaler (reset default) */
  eMIOS_UC_1.UC[23].A.R = 1000; 	/* MCB mode: period will be 1K clocks */
  eMIOS_UC_1.UC[23].C.B.MODE = 0x50;/* Modulus Counter Buffered (Up ctr) */
  eMIOS_UC_1.UC[23].C.B.UCPRE = 0;  /* Pre-scale channel clock by 0+1=1 */
  eMIOS_UC_1.UC[23].C.B.UCPREN= 1;  /* Enable pre-scaler */
}

void emios1_ch18_OPWMB(void) {      /* Configures eMIOS 1 channel 18: OPWMB */
  eMIOS_UC_1.UC[18].C.R = 0x0;  	/* Disable channel pre-scaler (reset default) */
  eMIOS_UC_1.UC[18].A.R = 250;  	/* PWM leading edge count value */
  eMIOS_UC_1.UC[18].B.R = 500;  	/* PWM trailing edge count value */
  eMIOS_UC_1.UC[18].C.B.BSL = 0;
  eMIOS_UC_1.UC[18].C.B.EDPOL = 1;  /* Output polarity on A match */
  eMIOS_UC_1.UC[18].C.B.MODE = 0x60;/* Output Pulse Width Modulation Buf'd*/

  SIUL2.MSCR[PJ4].B.OBE = 1;     /* Pad PJ4: OBE=1 (DS9 LED on NXP EVB) */
  SIUL2.MSCR[PJ4].B.SSS = 2;     /* Pad PJ4: Source signal is E1UC_18_Y  */
  SIUL2.MSCR[PJ4].B.SRC = 3;     /* Pad PJ4: Full strength slew rate */
}

void emios1_ch05_IPM(void) {      	/* Configures eMIOS 1 channel 05: IPM */
  eMIOS_UC_1.UC[5].C.R = 0x0;    	/* Disable channel pre-scaler (reset default) */
  eMIOS_UC_1.UC[5].C.B.BSL = 0;     /* Time base: counter bus A (channel 23) */
  eMIOS_UC_1.UC[5].C.B.MODE = 5;    /* Input Period Measurement (IPM) mode */
  eMIOS_UC_1.UC[5].C.B.IF = 2;      /* Input filter:  8 filter clocks */
  eMIOS_UC_1.UC[5].C.B.FCK = 1;     /* Filter clock: eMIOS module clock */

  SIUL2.MSCR[PH3].B.IBE = 1;       	/* Pad PH3: Enable pad for input */
  SIUL2.IMCR[41].B.SSS = 2;        	/* eMIOS1 channel 05, E1UC_5_H : connected to PH3 */
}

void emios1_ch06_IPWM(void) {   	/* Configures eMIOS 1 channel 06: IPWM */
  eMIOS_UC_1.UC[6].C.R = 0x0;   	/* Disable channel pre-scaler (reset default) */
  eMIOS_UC_1.UC[6].C.B.BSL = 0;     /* Time base: counter bus A (channel 23) */
  eMIOS_UC_1.UC[6].C.B.MODE = 4;    /* Input Pulse Width mode */
  eMIOS_UC_1.UC[6].C.B.IF = 2;      /* Input filter:  8 filter clocks */
  eMIOS_UC_1.UC[6].C.B.FCK = 1;     /* Filter clock: eMIOS module clock */

  SIUL2.MSCR[PH4].B.IBE = 1;  		/* Pad PH4: Enable pad for input */
  SIUL2.IMCR[42].B.SSS = 2;   		/* eMIOS1 channel 6, E1UC_6_H : connected to pad PH4 */
}

void enable_emios(void) {     /* Enable pre-scaled clocks to channels */
  eMIOS_0.MCR.B.GPREN = 1;    /* Enable global pre-scaled clocks  */
  eMIOS_0.MCR.B.GTBE = 1;     /* Enable global time base */
  eMIOS_1.MCR.B.GPREN = 1;    /* Enable global pre-scaled clocks  */
  eMIOS_1.MCR.B.GTBE = 1;     /* Enable global time base */
}
