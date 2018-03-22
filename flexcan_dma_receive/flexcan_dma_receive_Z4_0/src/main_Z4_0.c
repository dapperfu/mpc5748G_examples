/*****************************************************************************/
/* FILE NAME: 	 main_Z4_0.c          COPYRIGHT (c) NXP Semiconductors 2016  */
/*                                                      All Rights Reserved  */
/* PLATFORM:	 DEVKIT-MPC5748G											 */
/* DESCRIPTION:  CAN0 module is configured to use DMA to receive the message.*/
/* 				 Once the DMA module reads the received frame,  			 */
/* 				 the interrupt is triggered.				                 */
/*				 Use this project with "flexcan_transmit" project			 */
/* 				 running on the other board						 			 */
/*****************************************************************************/
/* REV      AUTHOR              DATE        DESCRIPTION OF CHANGE            */
/* ---   -----------          ----------    ---------------------            */
/* 1.0	  	B01802            15 Oct 2015   Initial Version          		 */
/* 1.1      K Shah			  30 March 2016 Ported to S32DS			         */
/*****************************************************************************/

#include "derivative.h" /* include peripheral declarations */
#include "project.h"
#include "mode.h"
#include "can.h"
#include "edma.h"

#define KEY_VALUE1 0x5AF0ul
#define KEY_VALUE2 0xA50Ful

void peri_clock_gating (void);	/* Configure gating/enabling peripheral clocks(CANs,DMAMUX) for modes*/

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
int main(void)
{
	int counter = 0;

	peri_clock_gating();   /* Configure gating/enabling peripheral clocks(CANs,DMAMUX) for modes*/
	                       /* Configuration occurs after mode transition */
	system160mhz();        /* sysclk=160MHz, dividers configured, mode transition*/

    initCAN_0_rx();        /* Initialize FLEXCAN 0 & one of its buffers for receive*/

    AIPS_Init();

    /* Configure DMA channel 16 to receive one frame from CAN1 */
    DMA_Init();

	while(1) {
	   	counter++;
	}

	return 0;
}

/********************  End of Main ***************************************/

void peri_clock_gating (void) {		/* Configure gating/enabling peripheral clocks(CANs,DMAMUX) for modes*/
  MC_ME.RUN_PC[0].R = 0x00000000;  	/* Gate off clock for all RUN modes */
  MC_ME.RUN_PC[1].R = 0x000000FE;  	/* Configure peripheral clock for all RUN modes */

  MC_ME.PCTL[70].B.RUN_CFG = 0x1;   /* FlexCAN 0: select peripheral configuration RUN_PC[1] */
  MC_ME.PCTL[90].B.RUN_CFG = 0x1;  	/* DMAMUX: select peripheral configuration RUN_PC[1] */
}
