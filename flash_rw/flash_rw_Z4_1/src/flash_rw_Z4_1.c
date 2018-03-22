/*****************************************************************************/
/* FILE NAME: 	 flash_rw_Z4_1.c      COPYRIGHT (c) NXP Semiconductors 2016  */
/*                                                      All Rights Reserved  */
/* PLATFORM:	 DEVKIT-MPC5748G											 */
/* DESCRIPTION:  Examples demonstrates how to Unlock, erase and program 	 */
/* 				 flash mid-block 0x00FB_8000 - 0x00FB_FFFF.					 */
/*				 Mid-block: 0x00FB_8000 - 0x00FB_FFFF, 32K, partition 3 	 */
/*				 It first writes the value at the mid-block locations and	 */
/*  			 then it erases the mid-block locations.					 */
/*                                                                           */
/*****************************************************************************/
/* REV      AUTHOR              DATE        DESCRIPTION OF CHANGE            */
/* ---   -----------          ----------    ---------------------            */
/* 1.0	  	b01802			  03 Jan 2016	Initial Version					 */
/* 1.1		K Shah			  06 Apr 2016	Ported to S32DS					 */
/*****************************************************************************/

#include "derivative.h" /* include peripheral declarations */
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
	int counter = 0;

	xcptn_xmpl ();	/* Configure and Enable Interrupts */
	system160mhz(); /* sysclk=160MHz, dividers configured, mode transform*/

    /* Unlock mid block 0x00FB_8000 - 0x00FB_FFFF */
    C55FMC.LOCK0.R &= 0xFFFFFDFF;

    /* Program one page in the mid block */
    C55FMC.MCR.B.PGM = 1;
    *(unsigned int*)0x00FB8000 = 0x11111111;	/* Interlock write */
    *(unsigned int*)0x00FB8004 = 0x22222222;	/* Up to 8 words (one 256bit page) can be programmed at once */
    *(unsigned int*)0x00FB8008 = 0x33333333;
    *(unsigned int*)0x00FB800C = 0x44444444;
    *(unsigned int*)0x00FB8010 = 0x55555555;
    *(unsigned int*)0x00FB8014 = 0x66666666;
    *(unsigned int*)0x00FB8018 = 0x77777777;
    *(unsigned int*)0x00FB801C = 0x88888888;
    C55FMC.MCR.B.EHV = 1;
    while(C55FMC.MCR.B.DONE == 0);
    C55FMC.MCR.B.EHV = 0;
    C55FMC.MCR.B.PGM = 0;

    /* Erase the mid block */
    C55FMC.MCR.B.ERS = 1;
    C55FMC.SEL0.R = 0x00000200;	/* Select the mid block */
    *(unsigned int*)0x00FB8000 = 0xFFFFFFFF;	/* Interlock write */
    *(unsigned int*)0x00FB8004 = 0xFFFFFFFF;
    *(unsigned int*)0x00FB8008 = 0xFFFFFFFF;
    *(unsigned int*)0x00FB800C = 0xFFFFFFFF;
    *(unsigned int*)0x00FB8010 = 0xFFFFFFFF;
    *(unsigned int*)0x00FB8014 = 0xFFFFFFFF;
    *(unsigned int*)0x00FB8018 = 0xFFFFFFFF;
    *(unsigned int*)0x00FB801C = 0xFFFFFFFF;
    C55FMC.MCR.B.EHV = 1;
    while(C55FMC.MCR.B.DONE == 0);
    C55FMC.MCR.B.EHV = 0;
    C55FMC.MCR.B.ERS = 0;

	for(;;) {	   
	   	counter++;
	}
	
	return 0;
}

/********************  End of Main ***************************************/
