/*****************************************************************************/
/* FILE NAME: ema4_z4_2.c             COPYRIGHT (c) NXP Semiconductors 2016  */
/*                                                      All Rights Reserved  */
/* PPLATFORM: DEVKIT-MPC5748G												 */
/* DESCRIPTION: Main C program for e200z4b to demonstrate semaphores.        */
/*              When EVB button SW1 is pushed, core attempts to acquire 	 */
/* 				semaphore.													 */
/*              If successful, turns on DS11 LED until button is released.   */
/*              When button is released DS11 LED is turned off & semaphore 	 */
/* 				released.  													 */
/*****************************************************************************/
/* REV      AUTHOR        DATE        DESCRIPTION                            */
/* ---   -----------    ----------    ---------------------                  */
/* 1.0    D.Haro        25 Oct 2013  Init'l ver. based on AN4830 hello world */
/* 1.1    SM            12 Feb 2015  Removed unrequired SIUL ME_PCTL code	 */
/* 1.2	  K Shah		03 Mar 2016	 Ported to S32DS						 */
/*****************************************************************************/

#include "derivative.h" /* include peripheral declarations */
#include "project.h"
#include "gpio.h"

#define PRESSED 1

extern void xcptn_xmpl(void);

__attribute__ ((section(".text")))

/************************************ Main ***********************************/

int main(void)
{
	AXBS_0.PORT[3].CRS.B.ARB = 1;  	/* Round-robin (rotating) priority */
	
	xcptn_xmpl ();              	/* Configure and Enable Interrupts */

	uint32_t j = 0;
	uint32_t z4b_PIR_reg;

	z4b_PIR_reg = 1;				/* Processor identification number for e200z4b */

	while(1){
	  j++;

	  if(BTN_SW4 == PRESSED) {
	    SEMA42.GATE[0].R = z4b_PIR_reg + 1;      /* Attempt to acquire and lock */
	             /* semaphore 0's gate by writing Core's spr PIR value plus 1. */
	    if (SEMA42.GATE[0].R == z4b_PIR_reg + 1) { /* If attempt was successful */
	          /* its Gate register will contain the value written by this core */
	      LED_DS11 = LED_ON ;
	      while(BTN_SW4 == PRESSED);           	 /* Wait until Button is released. */
	      LED_DS11 = LED_OFF;
	      SEMA42.GATE[0].R = 0;                	 /* Release semaphore 0's gate. */
	    }
	  }
	}

	return 0;
}

/********************  End of Main ***************************************/
