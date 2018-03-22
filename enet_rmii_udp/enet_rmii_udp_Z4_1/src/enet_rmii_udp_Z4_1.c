/*****************************************************************************/
/* FILE NAME: 	 enet_mii_udp_Z4_1.c  COPYRIGHT (c) NXP Semiconductors 2016  */
/*                                                      All Rights Reserved  */
/* PLATFORM:	 MPC5748G-LCEVB											 	 */
/* DESCRIPTION:  Example continuously transmits the UDP packets of length 2  */
/* 				 and listens to the input packets using receive interrupt	 */
/*                                                                           */
/*****************************************************************************/
/* REV      AUTHOR              DATE        DESCRIPTION OF CHANGE            */
/* ---   -----------          ----------    ---------------------            */
/* 1.0		K Shah			  06 Apr 2016	Initial version for MPC5748G	 */
/*****************************************************************************/
#include "derivative.h" /* include peripheral declarations */
#include "mode.h"
#include "ENET_Config.h"
#include "ENET_UDP_Driver.h"

#define KEY_VALUE1 0x5AF0ul
#define KEY_VALUE2 0xA50Ful

extern void xcptn_xmpl(void);
void peri_clock_gating();

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
int main(void)
{
	xcptn_xmpl ();              /* Configure and Enable Interrupts */

	peri_clock_gating();
	system160mhz();


	// Initialise the ENET module
	ENET_UDP_Driver_Init();
	//Enable the Receiver Interrupt
	ENET_Driver_Rx_Init();
	int i,j;
	uint8_t testData[2] = "HI";/* Declare and Initialize the data buffer with some data */
	while(1)
	{
			ENET_UDP_Tx(testData, 2);   /* Send frames with different payload sizes */

			for(j = 0; j < 65535; j++);
	}
	
	return 0;
}

void peri_clock_gating (void) {
  MC_ME.RUN_PC[0].R = 0x00000000;  /* gate off clock for all RUN modes */
  MC_ME.RUN_PC[1].R = 0x000000FE;  /* config. peri clock for all RUN modes */

  MC_ME.PCTL[15].B.RUN_CFG = 0x1;   /* ENET 0: select peri. cfg. RUN_PC[1] */
}

