/********************************************************************************/
/* FILE NAME: hello-pll-interrupt_z4_2.c  COPYRIGHT (c) NXP Semiconductors 2016	*/
/*                                          	           	All Rights Reserved */
/* PLATFORM: DEVKIT-MPC5748G													*/
/* DESCRIPTION: Main C program for Core 1 (e200z4b)      	                            */
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
	AXBS_0.PORT[3].CRS.B.ARB = 1;  	/* Round-robin (rotating) priority */
	
	xcptn_xmpl ();              	/* Configure and Enable Interrupts */

	PIT1_init(20000000);
	             /* timeout= 20M  PITclks x 4 sysclks/1 PITclk x 1 sec/160Msysck */
	             /*        = 20M x 4 / 160M = 80/160 = 0.5 sec.  */

	while(1);
	
	return 0;
}

/********************  End of Main ***************************************/

void SW_INT_1_isr(void) {
    LED_DS7 = ~LED_DS7;       /* Toggle DS7 LED  */
    INTC_SSCIR1 = 0x01;       /* Clear interrupt flag */
}

void PIT1_isr(void) {
    LED_DS5 = ~LED_DS5;       /* Toggle DS5 LED port */
    PIT.TIMER[1].TFLG.R = 1;  /* Clear interrupt flag */
}
