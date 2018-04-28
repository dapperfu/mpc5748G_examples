/*****************************************************************************/
/* FILE NAME: linflexd_lin.c          COPYRIGHT (c) NXP Semiconductors 2016  */
/*                                                      All Rights Reserved  */
/* PLATFORM: DEVKIT-MPC5748G												 */
/* DESCRIPTION: Transmit & receive LIN messages using LINflexD modules.      */
/*  - LINFlexD_0 module divides its 80 MHz LIN_CLK input to get 10.417K baud.*/
/*  - Transmit function sends 'hello   ' as 8 bytes with ID 0x35.            */
/*  - Receive function requests data from slave with ID 0x35. An external    */
/*    node or LIN tool is needed complete reception.  Without the node or    */
/*    tool, code will wait forever for the receive flag.                     */
/*                                                                           */
/*****************************************************************************/
/* REV      AUTHOR        DATE        DESCRIPTION OF CHANGE                  */
/* ---   -----------    ----------    ---------------------                  */
/* 1.0	 R. Delgado     Jan 13 2015   Initial Version                        */
/*****************************************************************************/
#include "linflexd_lin.h"

void initLINFlexD_0 (void) {     	/* Master at 10.417K baud with 80MHz LIN_CLK */

  LINFlexD_0.LINCR1.B.SLEEP = 0;    /* Put LINFlex hardware in init mode */
  LINFlexD_0.LINCR1.B.INIT = 1;    	/* Put LINFlex hardware in init mode */
  LINFlexD_0.LINCR1.R= 0x00000311; 	/* Configure module as LIN master & header */
  LINFlexD_0.LINIBRR.B.IBR= 480; 	/* Mantissa baud rate divider component */
        							/* Baud rate divider = 80 MHz LIN_CLK input / (16*10417K bps) ~=480 */
  LINFlexD_0.LINFBRR.B.FBR = 0; 	/* Fraction baud rate divider component */
  LINFlexD_0.LINCR1.R= 0x00000310; 	/* Change module mode from init to normal */
  SIUL2.MSCR[PB2].B.SSS = 1;      	/* Pad PB2: Source signal is LIN0_TX  */
  SIUL2.MSCR[PB2].B.OBE = 1;      	/* Pad PB2: OBE=1. */
  SIUL2.MSCR[PB2].B.SRC = 3;      	/* Pad PB2: Full strength slew rate */
  SIUL2.MSCR[PB3].B.IBE = 1;      	/* Pad PB3: Enable pad for input */
  SIUL2.IMCR[200].B.SSS = 2;     	/* LIN0_RX : connected to pad PC6 */
}

void transmitLINframe (void) {   	/* Transmit one frame 'hello    ' to ID 0x35*/
  LINFlexD_0.BDRM.R = 0x2020206F; 	/* Load most significant bytes '   o' */
  LINFlexD_0.BDRL.R = 0x6C6C6548; 	/* Load least significant bytes 'lleh' */
  LINFlexD_0.BIDR.R = 0x00001E35; 	/* Init header: ID=0x35, 8 B, Tx, enh cksum*/
  LINFlexD_0.LINCR2.B.HTRQ = 1;   	/* Request header transmission */

  while (!LINFlexD_0.LINSR.B.DTF); 	/* Wait for data transfer complete flag */
  LINFlexD_0.LINSR.R = 0x00000002;  /* Clear DTF flag */
}

void receiveLINframe (void) {      	/* Request data from ID 0x15 */
	uint8_t __attribute__ ((unused)) RxBuffer[8] = {0};
	uint8_t i;

  LINFlexD_0.BIDR.R = 0x00001C15; 	/* Init header: ID=0x15, 8 B, Rx, enh cksum */
  LINFlexD_0.LINCR2.B.HTRQ = 1;   	/* Request header transmission */
  while (!LINFlexD_0.LINSR.B.DRF); 	/* Wait for data receive complete flag */
                              	  	/* Code waits here if no slave response */
  for (i=0; i<4;i++){         		/* If received less than or equal 4 data bytes */
	RxBuffer[i]= (LINFlexD_0.BDRL.R>>(i*8)); /* Fill buffer in reverse order */
  }
  for (i=4; i<8;i++){         		/* If received more than 4 data bytes: */
	RxBuffer[i]= (LINFlexD_0.BDRM.R>>((i-4)*8)); /* Fill rest in reverse order */
  }
  LINFlexD_0.LINSR.R = 0x00000004;  /* Clear DRF flag */

}

