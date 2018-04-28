/*****************************************************************************/
/* FILE NAME: can.c                   COPYRIGHT (c) NXP Semiconductors 2016  */
/*                                                      All Rights Reserved  */
/* PLATFORM: DEVKIT-MPC5748G												 */
/* DESCRIPTION: Transmit and receives CAN messages				             */
/*                                                                           */
/*****************************************************************************/
/* REV      AUTHOR               DATE        DESCRIPTION OF CHANGE           */
/* ---   -----------          ----------    -----------------------          */
/* 1.0	 S Mihalik/H Osornio  07 Mar 2014   Initial Version                  */
/* 1.1	 S Mihalik            09 Mar 2015   Change from MPC5748G: 96 buffers.*/
/* 1.2	 K Shah				  01 Mar 2016	Ported to S32DS					 */
/*****************************************************************************/

#include "can.h"

void initCAN_0_rx(void) {         	/* General init.  MB IDs: MB4 ID_STD=0x555 */
  uint16_t   i;

  CAN_0.MCR.B.MDIS = 1;       	/* Disable module before selecting clock source*/
  CAN_0.CTRL1.B.CLKSRC=0;     	/* Clock Source = oscillator clock (40 MHz) */
  CAN_0.MCR.B.MDIS = 0;       	/* Enable module for config. (Sets FRZ, HALT)*/
  while (!CAN_0.MCR.B.FRZACK) {}/* Wait for freeze acknowledge to set */
                 	 	 	 	/* Good practice: wait for FRZACK on freeze mode entry/exit */
  CAN_0.CTRL1.R = 0x04DB0086; 	/* CAN bus: 40 MHz clksrc, 500K bps with 16 tq */
                              	/* PRESDIV+1 = Fclksrc/Ftq = 40 MHz/8MHz = 5 */
                              	/*    so PRESDIV = 4 */
                              	/* PSEG2 = Phase_Seg2 - 1 = 4 - 1 = 3 */
                              	/* PSEG1 = PSEG2 = 3 */
                              	/* PROPSEG= Prop_Seg - 1 = 7 - 1 = 6 */
                              	/* RJW = Resync Jump Width - 1 = 4 = 1 */
  	  	  	  	  	  	  	  	/* SMP = 1: use 3 bits per CAN sample */
                              	/* CLKSRC=0 (unchanged): Fcanclk= Fxtal= 40 MHz*/
  for (i=0; i<96; i++) {      	/* MPC574xG has 96 buffers after MPC5748G rev 0*/
    CAN_0.MB[i].CS.B.CODE = 0;  /* Inactivate all message buffers */
  }
//  CAN_0.MB[4].CS.B.IDE = 0;     /* MB 4 will look for a standard ID */
//  CAN_0.MB[4].ID.B.ID_STD = 0x555; /* MB 4 will look for ID = 0x555 */
//  CAN_0.MB[4].CS.B.CODE = 4;    /* MB 4 set to RX EMPTY */
  CAN_0.RXMGMASK.R = 0x1FFFFFFF;/* Global acceptance mask */

  SIUL2.MSCR[16].B.SSS = 1;    	/* Pad PB0: Source signal is CAN0_TX  */
  SIUL2.MSCR[16].B.OBE = 1;    	/* Pad PB0: Output Buffer Enable */
  SIUL2.MSCR[16].B.SRC = 3;    	/* Pad PB0: Maximum slew rate */
  SIUL2.MSCR[17].B.IBE = 1;    	/* Pad PB1: Enable pad for input - CAN0_RX */
  SIUL2.IMCR[188].B.SSS = 2;   	/* CAN0_RX: connected to pad PB1 */

  /* If DMA is used for receiving, it is necessary to enable RX FIFO */
  /* RX FIFO structure is placed at address CAN_BASE_ADDRESS + 0x80 */
  /* First "ID filter table element 0" is placed at CAN_BASE_ADDRESS + 0xE0 */

  CAN_0.CTRL2.B.RFFN = 0x0;	/* Message buffers 0-7 are occupied by RX FIFO and ID filter table */
  /* There are 8 ID filter table elements */

  CAN_0.MCR.B.IDAM = 0x0;	/* One full ID (standard and extended) per ID Filter Table element */

  /* Initialize  ID filter table elements 0-7 to receive messages with standard ID 0x555 (RTR and IDE = 0) */
  for(i = 0xE0; i<0xFF ;i=i+4)
      *(unsigned int*)(CAN0_BASE_ADDRESS + i) = 0x555<<19;

  /* Enable DMA, enable RX FIFO, negate FlexCAN 0 halt state for 64 MBs */
  CAN_0.MCR.R = 0x2000803F;
  while (CAN_0.MCR.B.FRZACK & CAN_0.MCR.B.NOTRDY) {} /* Wait to clear */
                 /* Good practice: wait for FRZACK on freeze mode entry/exit */
}

void initCAN_0_tx(void) {          /* General init. No MB IDs iniialized */
  uint8_t	i;

  CAN_0.MCR.B.MDIS = 1;       	/* Disable module before selecting clock source*/
  CAN_0.CTRL1.B.CLKSRC=0;     	/* Clock Source = oscillator clock (40 MHz) */
  CAN_0.MCR.B.MDIS = 0;       	/* Enable module for config. (Sets FRZ, HALT)*/
  while (!CAN_0.MCR.B.FRZACK) {}/* Wait for freeze acknowledge to set */
  CAN_0.CTRL1.R = 0x04DB0086;  	/* CAN bus: same as for CAN_0 */
  for (i=0; i<96; i++) {      	/* MPC574xG has 96 buffers after MPC5748G rev 0*/
    CAN_0.MB[i].CS.B.CODE = 0;  /* Inactivate all message buffers */
  }
  CAN_0.MB[0].CS.B.CODE = 8;    /* Message Buffer 0 set to TX INACTIVE */

  SIUL2.MSCR[16].B.SSS = 1;    	/* Pad PB0: Source signal is CAN0_TX  */
  SIUL2.MSCR[16].B.OBE = 1;    	/* Pad PB0: Output Buffer Enable */
  SIUL2.MSCR[16].B.SRC = 3;    	/* Pad PB0: Maximum slew rate */
  SIUL2.MSCR[17].B.IBE = 1;    	/* Pad PB1: Enable pad for input - CAN0_RX */
  SIUL2.IMCR[188].B.SSS = 2;   	/* CAN0_RX: connected to pad PB1 */

  CAN_0.MCR.R = 0x0000003F;     /* Negate FlexCAN 0 halt state for 64 MB */
  while (CAN_0.MCR.B.FRZACK & CAN_0.MCR.B.NOTRDY) {} /* Wait to clear */
                 	 	 	 	/* Good practice: wait for FRZACK on freeze mode entry/exit */
}

void TransmitMsg(void) {   		/* Assumption:  Message buffer CODE is INACTIVE */
  uint8_t	i;
  const uint8_t TxData[] = {"Hello"};  /* Transmit string*/

  CAN_0.MB[0].CS.B.IDE = 0;       /* Use standard ID length */
  CAN_0.MB[0].ID.B.ID_STD = 0x555;/* Transmit ID is 0x555 */
  CAN_0.MB[0].CS.B.RTR = 0;       /* Data frame, not remote Tx request frame */
  CAN_0.MB[0].CS.B.DLC = sizeof(TxData) -1 ;/*#bytes to transmit w/o null*/
  for (i=0; i<sizeof(TxData); i++) {
    CAN_0.MB[0].DATA.B[i] = TxData[i];      /* Data to be transmitted */
  }
  CAN_0.MB[0].CS.B.SRR = 1;     /* Tx frame (not req'd for standard frame)*/
  CAN_0.MB[0].CS.B.CODE =0xC;   /* Activate msg. buf. to transmit data frame */

}
