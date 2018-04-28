/*****************************************************************************/
/* FILE NAME: emios_Z4_1.c            COPYRIGHT (c) NXP Semiconductors 2016  */
/*                                                      All Rights Reserved  */
/* PLATFORM: DEVKIT-MPC5748G												 */
/* DESCRIPTION: Main C program for core 0 (e200z4a) to call eMIOS functions  */
/*				MCB: eMIOS1_CH23 is configured to count up to 1000, hence 	 */
/*				it demos a timer to generate trigger at 1000/1MHz = 1us		 */
/*				OPWMB: eMIOS1_CH18 is configured to generate the waveform 	 */
/*				with 25% duty cycle on PJ4 (DS9 LED) 						 */
/*				OPWFMB: eMIOS0_CH23 is configured to generate waveform with	 */
/*				200us(200 cycles) period and 10%(20 cycles) duty cycle		 */
/*				on pin PF6(J13_6)											 */
/*				IPM: eMIOS1_CH05 is configured to measure the period of input*/
/*				waveform on PH3(PJ4_7)										 */
/*				IPWM: eMIOS1_CH06 is configured to measure the duty cycle	 */
/*				of the input waveform on PH4(PJ4_5)							 */
/*                                                                           */
/*****************************************************************************/
/* REV      AUTHOR        DATE        DESCRIPTION OF CHANGE                  */
/* ---   -----------    ----------    ---------------------                  */
/* 1.0	  S Mihalik     05 Mar 2014   Initial Version                        */
/* 2.0	  S Mihalik     31 Mar 2014   Modified for adding input functions    */
/* 2.1    SM            27 Jan 2015   Moved SWT_disable function to boot code*/
/* 2.2	  K Shah		26 Feb 2016   Ported to S32DS						 */
/*****************************************************************************/

#include "derivative.h" /* include peripheral declarations */
#include "emios.h"
#include "mode.h"

#define KEY_VALUE1 0x5AF0ul
#define KEY_VALUE2 0xA50Ful
#define MAXCNT 1000 /* Count value for MCB channel. (used for OPWMB, IPM, IPWM)*/

extern void xcptn_xmpl(void);
void peri_clock_gating (void); /* Configure gating/enabling peripheral(eMIOS0, eMIOS1) clocks */

uint32_t period_1st_edge_cnt = 0; /* Input period 1st edge count value */
uint32_t period_2nd_edge_cnt =0;  /* Input period 2nd edge count value */
uint32_t period_in_usec = 0;      /* Calculated period. 1 count = 1 usec */
uint32_t duty_1st_edge_cnt = 0;   /* Input pulse 1st edge count value*/
uint32_t duty_2nd_edge_cnt = 0;   /* Input pulse 2nd edge count value*/
uint32_t duty_in_usec = 0;        /* Calculated pulse width (duty cycle) */

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
	uint32_t i = 0;
	uint32_t period_flag_count = 0;
	uint32_t duty_flag_count = 0;

	xcptn_xmpl ();              /* Configure and Enable Interrupts */

	peri_clock_gating();   		/* Configure gating/enabling peripheral clocks(eMIOS0, eMIOS1) for modes*/
	                         	/* Configuration occurs after mode transition */
	system160mhz();        		/* sysclk=160MHz, dividers configured, mode transition*/

	init_emios_global_prescalers();  /* Initialize eMIOS 1,2: pre-scale 80MHz/80=1MHz */

	emios1_ch23_MCB();    /* Configures eMIOS 1 channel 23: MCB, 1K count (1K usec period) */
	emios1_ch18_OPWMB();  /* Configures eMIOS 1 channel 18: OPWMB with time base eMIOS1 ch0 */
	emios0_ch23_OPWFMB(); /* Configures eMIOS 0 channel 23: OPWFMB 200 counts-200 us period*/
	emios1_ch05_IPM();    /* Configures eMIOS 1 channel 5:  IPM   with time base eMIOS1 ch0 */
	emios1_ch06_IPWM();   /* Configures eMIOS 1 channel 6:  IPWM  with time base eMIOS1 ch0 */
	enable_emios();       /* Enable eMIOS clocks to channels */

	while(1) {
        /* Measure period of signal connected to port PH3 (expect 200) */
		if (eMIOS_UC_1.UC[5].S.B.FLAG) {
			period_flag_count ++;                         /* Increment period counter*/
			period_2nd_edge_cnt  = eMIOS_UC_1.UC[5].A.R;  /* Read 2nd edge count */
			period_1st_edge_cnt  = eMIOS_UC_1.UC[5].B.R;  /* Read 2st edge count */
			if (period_1st_edge_cnt < period_2nd_edge_cnt) { /* Normal Case */
				period_in_usec = period_2nd_edge_cnt  - period_1st_edge_cnt ;
			}
			else {   /* Counter overflow case.  (Assumption: only 1 overflow.) */
				period_in_usec = period_2nd_edge_cnt + (MAXCNT - period_1st_edge_cnt);
			}
			eMIOS_UC_1.UC[5].S.B.FLAG = 1;            	/* Clear flag */
		}

        /* Measure duty cycle of signal connected to port PH4 (expect 20) */
		if (eMIOS_UC_1.UC[6].S.B.FLAG) {
			duty_flag_count ++;                          /* Increment duty counter */
			duty_2nd_edge_cnt  = eMIOS_UC_1.UC[6].A.R;   /* Read 2nd edge count */
			duty_1st_edge_cnt  = eMIOS_UC_1.UC[6].B.R;   /* Read 1st edge count */
			if (duty_1st_edge_cnt < duty_2nd_edge_cnt) { /* Normal Case */
				duty_in_usec = duty_2nd_edge_cnt  - duty_1st_edge_cnt ;
			}
			else {   /* Counter overflow case.  (Assumption: only 1 overflow.) */
				duty_in_usec = duty_2nd_edge_cnt + (MAXCNT - duty_1st_edge_cnt);
			}
			eMIOS_UC_1.UC[6].S.B.FLAG = 1;            	/* Clear flag */
		}
		i++;
	}
	
	return 0;
}

/********************  End of Main ***************************************/

void peri_clock_gating (void) {		/* Configure gating/enabling peripheral clocks(eMIOS0, eMIOS1) for modes*/
  MC_ME.RUN_PC[0].R = 0x00000000;  	/* gate off clock for all RUN modes */
  MC_ME.RUN_PC[1].R = 0x000000FE;  	/* config. peri clock for all RUN modes */

  MC_ME.PCTL[1].B.RUN_CFG = 0x1;   	/* eMIOS 0: select peri. cfg. RUN_PC[1] */
  MC_ME.PCTL[2].B.RUN_CFG = 0x1;   	/* eMIOS 1: select peri. cfg. RUN_PC[1] */
}

