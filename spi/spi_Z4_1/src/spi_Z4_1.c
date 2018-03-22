/*****************************************************************************/
/* FILE NAME: spi_Z4_1.c              COPYRIGHT (c) NXP Semiconductors 2016  */
/*                                                      All Rights Reserved  */
/* PLATFORM: DEVKIT-MPC5748G												 */
/* DESCRIPTION: Main C program for core 0 (e200z4a) to call SPI  functions   */
/*				Configures DSPI_3 in Master mode and SPI_0 in slave mode.	 */
/*				Master continuously sends data to Slave and Slave replies	 */
/* 				to master.													 */
/*                                                                           */
/*				DSPI_3 Pin Mapping:					SPI_0 Pin Mapping		 */
/*				CLK	  	PG4		J4_2				CLK 	PF7		J13_12	 */
/*				SOUT  	PG2		J4_1				SOUT	PG12	J14_3	 */
/*				SIN	  	PG5		J4_4				SIN		PI14	J14_13	 */
/*				SS/CS0	PG3		J4_3				SS/CS0	PI15	J14_15	 */
/*****************************************************************************/
/* REV      AUTHOR        DATE        DESCRIPTION OF CHANGE                  */
/* ---   -----------    ----------    ---------------------                  */
/* 1.0	 Scott Obrien   21 Apr 2015   Initial Version                        */
/* 1.1	 K Shah			16 Mar 2016	  Ported to S32DS						 */
/*****************************************************************************/

#include "derivative.h" /* include peripheral declarations */
#include "project.h"
#include "spi.h"
#include "mode.h"

#define KEY_VALUE1 0x5AF0ul
#define KEY_VALUE2 0xA50Ful

extern void xcptn_xmpl(void);
void peri_clock_gating (void); /* Configures gating/enabling peripheral(DSPI, SPI) clocks */

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

	xcptn_xmpl ();              			  /* Configure and Enable Interrupts */

    peri_clock_gating();                      /* Configures gating/enabling peripheral(DSPI, SPI) clocks for modes*/
	                                          /* Configuration occurs after mode transition */
	system160mhz();                           /* sysclk=160MHz, dividers configured, mode trans*/

	init_DSPI_3();                            /* Initialize DSPI_3 as master SPI and initialize CTAR0 */
	init_SPI_0();                             /* Initialize SPI_0 as Slave SPI and initialize CTAR0 */
	init_spi_ports();                         /* DSPI3 Master, SPI_0 Slave */

	while( 1 )
	{
	  SPI_0.PUSHR.PUSHR.R = 0x00001234;       /* Initialize slave SPI_0's response to master */
	  DSPI_3.PUSHR.PUSHR.R  = 0x08015678;     /* Transmit data from master DSPI to slave SPI with EOQ */
	  read_data_SPI_0();                      /* Read data on slave SPI */
	  read_data_DSPI_3();                     /* Read data on master DSPI */
	  i++;
	}
	return 0;
}

/************************  End of Main ***************************************/

void peri_clock_gating (void) {		/* Configures gating/enabling peripheral(DSPI, SPI) clocks */

  MC_ME.RUN_PC[0].R = 0x00000000;  	/* Gate off clock for all RUN modes */
  MC_ME.RUN_PC[1].R = 0x000000FE;  	/* Configures peripheral clock for all RUN modes */
  MC_ME.PCTL[43].B.RUN_CFG = 0x1;  	/* DSPI 2: select peripheral configuration RUN_PC[1] */
  MC_ME.PCTL[96].B.RUN_CFG = 0x1;  	/* SPI 0: select peripheral configuration RUN_PC[1] */
}
