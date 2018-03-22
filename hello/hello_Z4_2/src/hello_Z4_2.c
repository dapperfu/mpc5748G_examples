/*****************************************************************************/
/* FILE NAME: hello_Z4_2.c	           COPYRIGHT (c) NXP Semiconductors 2016 */
/*                                                      All Rights Reserved  */
/* PLATFORM: DEVKIT-MPG5748G												 */
/* DESCRIPTION: Main C program for core 1 (e200z4b)                          */
/*				Turns on the the DS4, DS6 and DS8 LEDs. Also outpus FIRC  	 */
/* 				clock scaled to FIRC/10 on port pin PG7 (J3_16)				 */
/*				Core 1 : Turns on DS6										 */
/*                                                                           */
/*****************************************************************************/
/* REV      AUTHOR        DATE        DESCRIPTION OF CHANGE                  */
/* ---   -----------    ----------    ---------------------                  */
/* 1.0	  SM            29 Jul 2013  Initial Version                         */
/* 1.1    SM            12 Feb 2015  Removed unrequired SIUL ME_PCTL code    */
/* 1.2	  K Shah		23 Feb 2016  Ported to S32DS						 */
/*****************************************************************************/

#include "derivative.h" /* include peripheral declarations */
#include "project.h"

extern void xcptn_xmpl(void);

__attribute__ ((section(".text")))
int main(void)
{
	AXBS_0.PORT[3].CRS.B.ARB = 1;  /* Round-robin (rotating) priority */
	
	xcptn_xmpl ();              /* Configure and Enable Interrupts */

	SIUL2.MSCR[PH13].B.OBE = 1;  /* Pad PF15 (95) OBE=1. On EVB active low DS6 LED */

	while(1);
	
	return 0;
}
