/*****************************************************************************/
/* FILE NAME: hello+pll_z4_1.c        COPYRIGHT (c) NXP Semiconductors 2016  */
/*                                                      All Rights Reserved  */
/* PLATFORM: DEVKIT-MPC5748G												 */
/* DESCRIPTION: Main C program for core 0 (e200z4a)                          */
/*				Configures the system clock to 160 MHz and also configures	 */
/*				different clock groups.										 */
/*				Turns on the the DS4, DS6 and DS8 LEDs. Also outputs PLL  	 */
/* 				clock scaled to PLL/10 on port pin PG7 (J3_16)				 */
/*				Core 0 : Configures System clock and Turns on DS4			 */
/*                                                                           */
/*****************************************************************************/
/* REV      AUTHOR        DATE        DESCRIPTION OF CHANGE                  */
/* ---   -----------    ----------    ---------------------                  */
/* 1.0	  SM            29 Jul 2013   Initial Version                        */
/* 1.2    SM            12 Feb 2015   Removed unrequired SIUL ME_PCTL code 	 */
/* 1.3	  K Shah		25 Feb 2016	  Ported to S32DS						 */
/*****************************************************************************/

#include "derivative.h" /* include peripheral declarations */
#include "clk.h"
#include "project.h"
#include "mode.h"

#define KEY_VALUE1 0x5AF0ul
#define KEY_VALUE2 0xA50Ful

extern void xcptn_xmpl(void);

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
	xcptn_xmpl (); /* Configure and Enable Interrupts */

	system160mhz();
	             /* Sets clock dividers= max freq, calls PLL_160MHz function which:
	                MC_ME.ME: enables all modes for Mode Entry module
		            Connects XOSC to PLL
		            PLLDIG: LOLIE=1, PLLCAL3=0x09C3_C000, no sigma delta, 160MHz
		            MC_ME.DRUN_MC: configures sysclk = PLL
		            Mode transition: re-enters DRUN mode
	             */

	hw_init();

	SIUL2.MSCR[PA10].B.OBE = 1; /* Pad PA10 (10): OBE=1. On EVB active low DS4 LED */
	clock_out_FMPLL();          /* Pad PG7(J3_16) = CLOCKOUT = PLL0/10 */


	while(1);
	
	return 0;
}

/********************  End of Main ***************************************/


