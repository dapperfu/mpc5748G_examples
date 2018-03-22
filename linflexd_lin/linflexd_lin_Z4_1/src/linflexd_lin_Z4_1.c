/*****************************************************************************/
/* FILE NAME: linflexed_lin_Z4_1.c    COPYRIGHT (c) NXP Semiconductors 2016  */
/*                                                      All Rights Reserved  */
/* PLATFORM: DEVKIT-MPC5748G												 */
/* DESCRIPTION: Main C program for core 0 (e200z4a) to call LIN functions.   */
/*				Continuously transfers data on LIN0 port (P3).				 */
/*				Using oscilloscope one can observe transmitted waveforms.	 */
/*                                                                           */
/*****************************************************************************/
/* REV      AUTHOR        DATE        DESCRIPTION OF CHANGE                  */
/* ---   -----------    ----------    ---------------------                  */
/* 1.0	 R. Delgado     Jan 09 2015  Initial Version                         */
/* 1.1   S. Mihalik     12 Feb 2015  Moved SWT_disable function to boot code */
/*                                   & removed unrequired SIUL ME_PCTL code  */
/* 1.2	 K Shah			02 Mar 2016  Ported to S32DS						 */
/*****************************************************************************/

#include "derivative.h" /* include peripheral declarations */
#include "project.h"
#include "mode.h"
#include "linflexd_lin.h"

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

	xcptn_xmpl ();              	/* Configure and Enable Interrupts */

	uint32_t LIN_msg_count = 0;   	/* Count of LIN messages transmitted */

	peri_clock_gating();    		/* Configure gating/enabling peri. clocks for modes*/
									/* Configuration occurs after mode transition */
	system160mhz();         		/* sysclk=160MHz, dividers configured, mode transition*/

	initLINFlexD_0();       		/* Initialize LINFlexD_1 as master */

	while (1) {
	  transmitLINframe();     		/* Transmit one frame from master */
//	  receiveLINframe();      		/* Request data (requires extra LIN node)*/
	  LIN_msg_count++;        		/* Increment LIN message transmit count */
	}
	
	return 0;
}

/******************************End of Main ***********************************/

void peri_clock_gating (void) {	   /* Configure gating/enabling peri. clocks for modes*/
  MC_ME.RUN_PC[0].R = 0x00000000;  /* gate off clock for all RUN modes */
  MC_ME.RUN_PC[1].R = 0x000000FE;  /* config. peri clock for all RUN modes */

  MC_ME.PCTL[50].B.RUN_CFG = 0x1;   /* LINFlex 0: select peri. cfg. RUN_PC[1]*/
}
