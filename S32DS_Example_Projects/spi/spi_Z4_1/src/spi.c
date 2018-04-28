/*****************************************************************************/
/* FILE NAME: spi.c                   COPYRIGHT (c) NXP Semiconductors 2015  */
/*                                                      All Rights Reserved  */
/* PLATFORM: DEVKIT-MPC5748G												 */
/* DESCRIPTION: SPI functions for initialization and reading received data.  */
/*                                                                           */
/*****************************************************************************/	
/* REV      AUTHOR        DATE        DESCRIPTION OF CHANGE                  */
/* ---   -----------    ----------    ---------------------                  */
/* 1.0	 Scott Obrien   29 Apr 2015   Initial Version                        */
/* 1.1		K Shah		16 Mar 2016   Ported to S32DS						 */
/*****************************************************************************/

#include "project.h"
#include "spi.h"

unsigned int RecDataMaster;
unsigned int RecDataSlave;

/*****************************************************************************/
/* There are only 2 options available on the EVB
** the "x" signifies the channel used
** 
** DSPI_3 MASTER
** CLK	  PG4x(J4_2)	PH12
** SOUT	  PG2x(J4_1)	PH11
** SIN	  PG5x(J4_4)	PI11
** SS/CS0 PG3x(J4_3)	PH13
**
** SPI_0  SLAVE
** CLK	  PF7x(J13_12)  PG13  PH14  PI5  PK11
** SOUT	  PG12x(J14_3)  PH13  PI4  PJ1  PK10
** SIN	  PG10  PI14x(J14_13)  PK9
** SS/CS0 PG11  PH11  PI6  PI15x(J14_15)  PK12
*/
/*****************************************************************************/

void init_spi_ports(){
  /* Master - DSPI_3 */
  SIUL2.MSCR[PG2].B.SSS = 2;                /* Pad PG2: Source signal is DSPI_3 SOUT  */
  SIUL2.MSCR[PG2].B.OBE = 1;                /* Pad PG2: OBE=1. */
  SIUL2.MSCR[PG2].B.SRC = 3;                /* Pad PG2: Full strength slew rate */
  
  SIUL2.MSCR[PG4].B.SSS = 2;                /* Pad PG4: Source signal is DSPI_3 CLK  */
  SIUL2.MSCR[PG4].B.OBE = 1;                /* Pad PG4: OBE=1. */
  SIUL2.MSCR[PG4].B.SRC = 3;                /* Pad PG4: Full strength slew rate */
  
  SIUL2.MSCR[PG5].B.IBE = 1;                /* Pad PG5: Enable pad for input DSPI_3 SIN */
  SIUL2.IMCR[297].B.SSS = 1;            	/* Pad PG5: connected to pad PG5 */

  SIUL2.MSCR[PG3].B.SSS = 2;                /* Pad PG3: Source signal is DSPI_3 CS0  */
  SIUL2.MSCR[PG3].B.OBE = 1;                /* Pad PG3: OBE=1. */
  SIUL2.MSCR[PG3].B.SRC = 3;                /* Pad PG3: Full strength slew rate */
  
  /* Slave - SPI_0 */
  SIUL2.MSCR[PF7].B.SSS = 1;                /* Pad PF7: Source signal is SPI_0 CLK */
  SIUL2.MSCR[PF7].B.IBE = 1;                /* Pad PF7: IBE=1. */
  SIUL2.IMCR[301].B.SSS = 1;            	/* Pad PF7: SPI_0 CLK */
  
  SIUL2.MSCR[PG12].B.SSS = 2;               /* Pad PG12: Source signal is SPI_0 SOUT */
  SIUL2.MSCR[PG12].B.OBE = 1;               /* Pad PG12: OBE=1. */
  SIUL2.MSCR[PG12].B.SRC = 3;               /* Pad PG12: Full strength slew rate */
  
  SIUL2.MSCR[PI14].B.IBE = 1;               /* Pad PI14: Enable pad for input SPI_0 SIN */
  SIUL2.IMCR[300].B.SSS = 2;            	/* Pad PI14: connected to pad */

  SIUL2.MSCR[PI15].B.IBE = 1;               /* Pad PI15: IBE=1. SPI_0 SS */
  SIUL2.IMCR[302].B.SSS = 4;            	/* Pad PI15: connected to pad */
}

void init_DSPI_3(void) {
  DSPI_3.MCR.R = 0x80010001;                /* Configure DSPI as master */
  DSPI_3.MODE.CTAR[0].R = 0x78021004;       /* Configure CTAR0  */
  DSPI_3.MCR.B.HALT = 0x0;                  /* Exit HALT mode: go from STOPPED to RUNNING state*/
}

void read_data_DSPI_3(void) {
  while (DSPI_3.SR.B.RFDF != 1){}           /* Wait for Receive FIFO Drain Flag = 1 */
  RecDataSlave = DSPI_3.POPR.R;             /* Read data received by slave SPI */
  DSPI_3.SR.R = 0xFCFE0000;                 /* Clear ALL status flags by writing 1 to them */
}

void init_SPI_0(void) {
  SPI_0.MCR.R = 0x00010001;                 /* Configure DSPI_0 as slave */
  SPI_0.MODE.CTAR[0].R = 0x78021004;        /* Configure CTAR0  */
  SPI_0.MCR.B.HALT = 0x0;                   /* Exit HALT mode: go from STOPPED to RUNNING state*/
}

void read_data_SPI_0(void) {
  while (SPI_0.SR.B.RFDF != 1){}            /* Wait for Receive FIFO Drain Flag = 1 */
  RecDataMaster = SPI_0.POPR.R;             /* Read data received by master SPI */
  SPI_0.SR.R = 0xFCFE0000;                  /* Clear ALL status flags by writing 1 */
}

