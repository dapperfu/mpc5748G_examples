/*****************************************************************************/
/* FILE NAME: ema4_z4_1.c             COPYRIGHT (c) NXP Semiconductors 2016  */
/*                                                      All Rights Reserved  */
/* PLATFORM: DEVKIT-MPC5748G												 */
/* DESCRIPTION: Main C program for e200z4a to demonstrate semaphores.        */
/*              When EVB button SW2 is pushed, core attempts to acquire 	 */
/* 				semaphore.													 */
/*              If successful, turns on DS4 LED until button is released.When*/
/*              button is released DS4 LED is turned off & semaphore released*/
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
#include "mode.h"

#define KEY_VALUE1 0x5AF0ul
#define KEY_VALUE2 0xA50Ful
#define PRESSED 1

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

	xcptn_xmpl ();              /* Configure and Enable Interrupts */


	uint32_t i = 0;          	/* Dummy loop delay counter for core 0 */
	uint32_t z4a_PIR_reg;

	system160mhz();          	/* sysclk=160MHz, dividers configured, mode trans*/
	hw_init();
	initGPIO();              	/* Initialize LED, buttons & other general purpose pins on NXP EVB */

	SEMA42.RSTGT.W.R = 0xE200;  /* Reset ALL Semaphores Gates 1st write */
	SEMA42.RSTGT.W.R = 0x1DFF;  /* Reset ALL Semaphores Gates 2nd write */

	z4a_PIR_reg = 0;			/* Processor identification number for e200z4a */

	while(1){
	i++;
  	  if(BTN_SW2 == PRESSED) {
	  SEMA42.GATE[0].R = z4a_PIR_reg + 1;        	/* If attempt was successful */
	  /* Semaphore 0's gate by writing Core's spr PIR value plus 1. */
    	if (SEMA42.GATE[0].R == z4a_PIR_reg + 1) { 	/* If attempt was successful */
    	/* Its Gate register will contain the value written by this core */
    		LED_DS4 = LED_ON;
    		while(BTN_SW2 == PRESSED);            	/* Wait until Button is released. */
    		LED_DS4 = LED_OFF;
    		SEMA42.GATE[0].R = 0;              		/* Release semaphore 0's gate. */
		}
  	  }
	}

	
	return 0;
}

/********************  End of Main ***************************************/
