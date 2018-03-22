/*****************************************************************************/
/* FILE NAME: smpu_Z4_1.c             COPYRIGHT (c) NXP Semiconductors 2016  */
/*                                                      All Rights Reserved  */
/* PLATFORM DEVKIT-MPC5748G													 */
/* DESCRIPTION: Demonstrates the functioning of System Memory Protection	 */
/* 				Unit(SMPU). Locks the last 4kB of RAM to allow just read	 */
/* 				access. So any attempt to write to those locations will		 */
/* 				generate the machine check exception.						 */
/*                                                                           */
/*****************************************************************************/
/* REV      AUTHOR              DATE        DESCRIPTION OF CHANGE            */
/* ---   -----------          ----------    ---------------------            */
/* 1.0	  b01802 			  17 Mar 2015	Initial Version					 */
/* 1.2	  K Shah			  05 Apr 2016   Ported to S32DS					 */
/*****************************************************************************/

#include "derivative.h" /* include peripheral declarations */
#include "project.h"
#include "mode.h"
#include "smpu.h"

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

/********************************* Main ***************************************/

__attribute__ ((section(".text")))
int main(void)
{
	int counter = 0;

	xcptn_xmpl (); 	/* Configure and Enable Interrupts */

	system160mhz(); /* sysclk=160MHz, dividers configured, mode transition*/

	/* Write within last 4kB SRAM - this access will not trigger any exception */
    *(unsigned int *)0x400bf000 = 0xAAAAAAAA;

	initSMPU(); /* Initialize the SMPU and particularly configured to read only access to last 4kB of SRM memory region */

	/* Write within last 4kB SRAM - this access will trigger exception due to SMPU access violation */
	*(unsigned int *)0x400bf000 = 0x55555555;

	/* Read from last 4kB SRAM - this access will not trigger any exception */
	/* Verify the read value in Variables Window */
   	uint32_t *mem_ptr = (uint32_t *)0x400bf000;


	for(;;) {	   
	   	counter++;
	}

	return 0;
}

/******************************  End of Main **********************************/
