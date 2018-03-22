/*****************************************************************************/
/* FILE NAME: linflexd_uart_Z4_1.c    COPYRIGHT (c) NXP Semiconductors 2016  */
/*                                                      All Rights Reserved  */
/* PLATFORM: DEVKIT-MPC5748G												 */
/* DESCRIPTION: Main C program for core 0 (e200z4a) to call UART functions   */
/*              to transmit and receive data on LINFlex_1 					 */
/* 				at PC8 and PC9 respectively. It is integrated with openSDA 	 */
/* 				interface. So use openSDA micro-usb interface 				 */
/*				to connect to computer										 */
/*				Example sends the welcome message in the beginning of 		 */
/*				the program. Then receives the character and sends the		 */
/* 				received character back to the TX pin.						 */
/*                                                                           */
/*****************************************************************************/
/* REV      AUTHOR        DATE        DESCRIPTION OF CHANGE                  */
/* ---   -----------    ----------    ---------------------                  */
/* 1.0	 Scott Obrien   31 Mar 2014   Initial Version                        */
/* 1.1   S Mihalik      12 Feb 2015   Removed unrequired SIUL ME_PCTL code   */
/* 1.2	 K Shah			02 Mar 2016	  Ported to S32DS						 */
/*****************************************************************************/

#include "derivative.h" /* include peripheral declarations */
#include "project.h"
#include "linflexd_uart.h"
#include "mode.h"

#define KEY_VALUE1 0x5AF0ul
#define KEY_VALUE2 0xA50Ful

extern void xcptn_xmpl(void);
void peri_clock_gating (void); /* Configures gating/enabling peripheral(LIN) clocks */

void hw_init(void)
{
#if defined(DEBUG_SECONDARY_CORES)
	uint32_t mctl = MC_ME.MCTL.R;
#if defined(TURN_ON_CPU1)
	/* enable core 1 in all modes */
	MC_ME.CCTL[2].R = 0x00FE;
	/* Set Start address for core 1: Will reset and start */
	MC_ME.CADDR[2].R = 0x11d0000 | 0x1;
#endif	
#if defined(TURN_ON_CPU2)
	/* enable core 2 in all modes */
	MC_ME.CCTL[3].R = 0x00FE;
	/* Set Start address for core 2: Will reset and start */
	MC_ME.CADDR[3].R = 0x13a0000 | 0x1;
#endif
	MC_ME.MCTL.R = (mctl & 0xffff0000ul) | KEY_VALUE1;
	MC_ME.MCTL.R =  mctl; /* key value 2 always from MCTL */
#endif /* defined(DEBUG_SECONDARY_CORES) */
}

__attribute__ ((section(".text")))

/************************************ Main ***********************************/

int main(void)
{

	xcptn_xmpl ();              /* Configures and Enables Interrupts */

	unsigned char Input;

	peri_clock_gating();		/* Configures gating/enabling peripheral clocks for modes*/
                             	/* Configuration occurs after mode transition */
	system160mhz();            	/* sysclk=160MHz, dividers configured, mode transition*/

	hw_init();

  	initLINFlexD_2( 80, 57600 );/* Initialize LINFlex2: UART Mode 80MHz, 57600 Baud */
  	testLINFlexD_2();			/* Display a message on the terminal */
//  	initLINFlexD_1( 80, 57600 );/* Initialize LINFlex1: UART Mode 80MHz, 57600 Baud */
//  	testLINFlexD_1();			/* Display a message on the terminal */

  	  while (1) {
  		  Input = rxLINFlexD_2();	/* Wait for & receive one byte */
  		  txLINFlexD_2( Input );	/* Transmit one byte */
//  		  Input = rxLINFlexD_1();	/* Wait for & receive one byte */
//  		  txLINFlexD_1( Input );	/* Transmit one byte */
  	  }

	return 0;
}

/************************  End of Main ***************************************/

void peri_clock_gating (void) {		/* Configures gating/enabling peripheral clocks for modes*/
  MC_ME.RUN_PC[0].R = 0x00000000;  	/* Gate off clock for all RUN modes */
  MC_ME.RUN_PC[1].R = 0x000000FE;  	/* Configures peripheral clock for all RUN modes */
  MC_ME.PCTL[52].B.RUN_CFG = 0x1;  	/* LINFlex 2: select peripheral configuration RUN_PC[1] */
//  MC_ME.PCTL[51].B.RUN_CFG = 0x1;  	/* LINFlex 2: select peripheral configuration RUN_PC[1] */
}
