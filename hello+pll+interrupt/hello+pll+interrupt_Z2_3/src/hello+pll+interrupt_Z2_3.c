/********************************************************************************/
/* FILE NAME: hello-pll-interrupt_z4_3.c  COPYRIGHT (c) NXP Semiconductors 2016	*/
/*                                          	           	All Rights Reserved */
/* PLATFORM: DEVKIT-MPC5748G													*/
/* DESCRIPTION: Main C program for Core 2 (e200z2)      	                            */
/*              Implements the project "hello+pll" plus introduces 				*/
/*				to the interrupts. PIT0 generates the interrupt in Core 0 and 	*/
/* 				toggles the DS4 LED at every 1s. After every 4 PIT0 interrupt	*/
/* 				it generates the software interrupt in core 1 which toggles		*/
/* 				LED DS7 at every 4s.											*/
/* 				Core 1 also generates PIT1 interrupt which toggles 				*/
/* 				LED DS5 at every 0.5s. 											*/
/* 				Core 2 also generates PIT2 interrupt which toggles 				*/
/* 				LED DS6 at every 0.25s.											*/
/*                                                                           	*/
/********************************************************************************/
/* REV      AUTHOR        DATE        DESCRIPTION OF CHANGE                  	*/
/* ---   -----------    ----------    ---------------------                  	*/
/* 1.0	  A.Inzunza     25 Jul 2013   Initial Release                	     	*/
/* 1.1    S.Mihalik     13 Sep 2013   Reorganized code & small changes       	*/
/* 1.2    D.Haro        12 Feb 2014   Added SW IRQ 1 init. & generation      	*/
/* 1.3	  K Shah		25 Feb 2016	  Ported to S32DS						 	*/
/********************************************************************************/

#include "derivative.h" /* include peripheral declarations */
#include "pit.h"
#include "gpio.h"

extern void xcptn_xmpl(void);

__attribute__ ((section(".text")))

/************************************ Main ***********************************/

int main(void)
{
	xcptn_xmpl ();              /* Configure and Enable Interrupts */

	PIT2_init(10000000);
	             /* timeout= 10M  PITclks x 4 sysclks/1 PITclk x 1 sec/160Msysck */
	             /*        = 10M x 4 / 160M = 40/160 = 0.25 sec.  */

	while(1);
	
	return 0;
}

/********************  End of Main ***************************************/

void PIT2_isr(void) {
    LED_DS6 = ~LED_DS6;       /* Toggle DS6 LED port */
    PIT.TIMER[2].TFLG.R = 1;  /* Clear interrupt flag */
}
