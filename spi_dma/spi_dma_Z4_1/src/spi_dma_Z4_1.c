/*****************************************************************************/
/* FILE NAME: spi_dma_Z5_1.c          COPYRIGHT (c) NXP Semiconductors 2016  */
/*                                                      All Rights Reserved  */
/* DESCRIPTION: Main C program for core 0 (e200z4a) to call SPI  functions   */
/*				Configures DSPI_3 in Master mode and SPI_0 in slave mode.	 */
/*				In Master DMA sends data to DSPI transfer register and 		 */
/* 				Slave receives it and saves it to RAM using DMA	 			 */
/*                                                                           */
/*				DSPI_3 Pin Mapping:					SPI_0 Pin Mapping		 */
/*				CLK	  	PG4		J4_2				CLK 	PF7		J13_12	 */
/*				SOUT  	PG2		J4_1				SOUT	PG12	J14_3	 */
/*				SIN	  	PG5		J4_4				SIN		PI14	J14_13	 */
/*				SS/CS0	PG3		J4_3				SS/CS0	PI15	J14_15	 */
/*****************************************************************************/
/* REV      AUTHOR        DATE        DESCRIPTION OF CHANGE                  */
/* ---   -----------    ----------    ---------------------                  */
/* 1.0	 Scott Obrien   30 Apr 2015   Initial Version                        */
/* 1.1	 K Shah			16 Mar 2016	  Ported to S32DS						 */
/*****************************************************************************/

#include "derivative.h" /* include peripheral declarations */
#include "project.h"
#include "mode.h"
#include "spi.h"
#include "edma.h"

#define KEY_VALUE1 0x5AF0ul
#define KEY_VALUE2 0xA50Ful

extern void xcptn_xmpl(void);
void peri_clock_gating(void);	/* Configures gating/enabling peripheral(SPI, DSPI, DMAMUX) clocks */
void bridges_config(void);		/* Bridge configuration */
void crossbar_config_DMA_highest(void);	/* Crossbar priority configuration for DMA */

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
	unsigned int i = 0;

	xcptn_xmpl ();              	   /* Configure and Enable Interrupts */

	peri_clock_gating();               /* Configures gating/enabling peripheral(SPI, DSPI, DMAMUX) clocks for modes */
	                                   /* Configuration occurs after mode transition */
	system160mhz();                    /* sysclk=160MHz, dividers configured, mode trans */

	bridges_config();                  /* Configures PBridge(s) access rights and priorities */
	crossbar_config_DMA_highest();     /* PBridges, flash port 2: DMA highest priority */

	init_dma_mux();                    /* DMA MUX for SPI_0 Slave */
	init_edma_tcd_0();                /* DMA Channel for SPI_0 Slave */
	init_edma_tcd_16();                /* DMA Channel for DSPI_3 Master */
	init_edma_channel_arbitration();   /* DMA Channel priorities */

	init_dspi_3();                     /* Initialize DSPI_3 as master SPI and initialize CTAR0 */
	init_spi_0();                      /* Initialize SPI_0 as Slave SPI and initialize CTAR0 */
	init_spi_ports();                  /* DSPI_3 Master, SPI_0 Slave */

	DSPI_3.MCR.B.HALT = 0x0;           /* Exit HALT mode: go from STOPPED */
	                                   /*  to RUNNING state to start transfers */
	EDMA.SERQ.R = 0;                   /* Enable EDMA channel 16 SPI_0  RX */
	EDMA.SERQ.R = 16;                  /* Enable EDMA channel 17 DSPI_3 TX*/

	while( DSPI_3.SR.B.EOQF != 1 ){}   /* Wait until the End Of DSPI Queue: */
	                                   /* All data is transmitted & received by DMA */
	while( 1 )
	{
	  i++;
	}
	return 0;
}

/************************  End of Main ***************************************/

void peri_clock_gating (void) {		/* Configures gating/enabling peripheral(SPI, DSPI, DMAMUX) clocks for modes */
  MC_ME.RUN_PC[0].R = 0x00000000;  	/* gate off clock for all RUN modes */
  MC_ME.RUN_PC[1].R = 0x000000FE;  	/* configures peripheral clock for all RUN modes */
  MC_ME.PCTL[43].B.RUN_CFG = 0x1;  	/* DSPI_3: select peripheral configuration RUN_PC[1] */
  MC_ME.PCTL[96].B.RUN_CFG = 0x1;  	/* SPI_0: select peripheral configuration RUN_PC[1] */
  MC_ME.PCTL[90].B.RUN_CFG = 0x1;  	/* DMAMUX: select peripheral configuration RUN_PC[1] */
}

/* Configures bridges to provide desired RW and user/supervisor
	access and priorities by crossbar masters to crossbar slaves. */
void bridges_config (void) {
  AIPS_A.MPRA.R |= 0x77777770;       /* All masters have RW & user level access */
  AIPS_A.MPRB.R |= 0x07777700;       /* All masters have RW & user level access */
  AIPS_B.MPRA.R |= 0x77777770;       /* All masters have RW & user level access */
  AIPS_B.MPRB.R |= 0x07777700;       /* All masters have RW & user level access */
}

/* Configures desired crossbar priorities for crossbar slaves to give to crossbar masters. */
void crossbar_config_DMA_highest (void) {
  AXBS_0.PORT[6].PRS.R = 0x70654321; /* PBridge 0:    gives highest priority to DMA */
  AXBS_0.PORT[5].PRS.R = 0x70654321; /* PBridge 1:    gives highest priority to DMA */
  AXBS_0.PORT[2].PRS.R = 0x70654321; /* Flash Port 2: gives highest priority to DMA */
}
