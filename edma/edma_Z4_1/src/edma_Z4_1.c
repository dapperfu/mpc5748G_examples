/*****************************************************************************/
/* FILE NAME: edma_Z4_1.c             COPYRIGHT (c) NXP Semiconductors 2016  */
/*                                                      All Rights Reserved  */
/* DESCRIPTION: Main C program for core 0 to exercise simple software DMA.   */
/*              TCD 0: String move from RAM to RAM                           */
/*              TCD 1: Alternate writing 1's & 0's to port pin  PA10         */
/*                                                                           */
/*****************************************************************************/
/* REV      AUTHOR        DATE        DESCRIPTION OF CHANGE                  */
/* ---   -----------    ----------    ---------------------                  */
/* 1.0	 S. Mihalik     30 Apr 2015   Initial Version                        */
/* 1.1	 K Shah			15 Mar 2016	  Ported to S32DS						 */
/*****************************************************************************/

#include "derivative.h" /* include peripheral declarations */
#include "project.h"
#include "mode.h"
#include "edma.h"
#include "smpu.h"

#define KEY_VALUE1 0x5AF0ul
#define KEY_VALUE2 0xA50Ful

extern void xcptn_xmpl(void);
void bridges_config (void);

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

/************************************ Main ***********************************/

__attribute__ ((section(".text")))
int main()
{
	xcptn_xmpl ();			/* Configure and Enable Interrupts */

	bridges_config();       /* Enable R/W to peripherals by DMA & all masters*/
	smpu_config();          /* Cache inhibit a RAM region for shared data */
	system160mhz();         /* sysclk=160MHz, dividers configured, mode trans*/

	SIUL2.GPDO[PA10].R = 1;      /* Pad PA10 (10): Output=1 (EVB's LED DS4 off) */
	SIUL2.MSCR[PA10].B.OBE = 1;  /* Pad PA10 (10): Enable output to pad */

	init_edma_channel_arbitration();/* Initialize arbitration among channels */
	initTCDs();             		/* Initialize DMA Transfer Control Descriptors */

	EDMA.SERQ.R = 0;	/* Enable EDMA channel 0 */
                        /* Initiate DMA service using software activation: */

	EDMA.SSRT.R = 0;	/* Set channel 0 START bit to initiate 1st minor loop */

	while (EDMA.TCD[0].CITER.ELINKNO.B.CITER != 1) {	/* While CITER != 1 (not on last minor loop), */
		while ((EDMA.TCD[0].CSR.B.START == 1) | (EDMA.TCD[0].CSR.B.ACTIVE == 1));	/* Wait for START=0 and ACTIVE=0 */
		EDMA.SSRT.R = 0;	/* Set channel 0 START bit again for next minor loop */
	}

	EDMA.SSRT.R = 1;        /* Set channel 1 START bit to initiate 1st minor loop */

	while (EDMA.TCD[1].CITER.ELINKNO.B.CITER != 1) {	/* While CITER != 1 (not on last minor loop), */
		while ((EDMA.TCD[1].CSR.B.START == 1) | (EDMA.TCD[1].CSR.B.ACTIVE == 1)); /* Wait for START=0 and ACTIVE=0 */
		EDMA.SSRT.R = 1;     /* Set channel 0 START bit again for next minor loop */
	}

	while(1);

	return 0;

}

/********************  End of Main ***************************************/

/*****************************************************************************/


/* Configures bridges to provide desired RWX and user/supervisor access
 * and priorities by crossbar masters to crossbar slaves. */
void bridges_config (void) {
       AIPS_A.MPRA.R |= 0x00007000;   /* Enable DMA RWU for PBRIDGE A */
}
