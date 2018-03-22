/*****************************************************************************/
/* FILE NAME: 	 bctu_pit_adc_Z4_1.c  COPYRIGHT (c) NXP Semiconductors 2016  */
/*                                                      All Rights Reserved  */
/* PLATFORM:	 DEVKIT-MPC5748G											 */
/* DESCRIPTION:  Implements ADC driver and demonstrates the usage of		 */
/*				 ADC in BCTU mode									         */
/*				 When PIt timer exceeds, trigger is sent to BCTU			 */
/*				 and BCTU triggers ADC1_CH9 conversion. The result will	     */
/*				 blink user LEDs (according to its value) after each 		 */
/*				 conversion.												 */
/*                                                                           */
/*****************************************************************************/
/* REV      AUTHOR              DATE        DESCRIPTION OF CHANGE            */
/* ---   -----------          ----------    ---------------------            */
/* 1.0	  V Peter	          29 Oct 2014   Initial working version          */
/* 1.1	  K Shah			  18 Mar 2016	Ported program to make compatible*/
/* 							  with S32DS & DEVKIT-MPC5748G					 */
/*****************************************************************************/

#include "derivative.h" /* include peripheral declarations */
#include "project.h"
#include "mode.h"
#include "adc.h"

#define KEY_VALUE1 0x5AF0ul
#define KEY_VALUE2 0xA50Ful

extern void xcptn_xmpl(void);
void peri_clock_gating(void);  	/* Configure gating/enabling peripheral(ADC, PIT, BCTU) clocks */
void PIT_Init (void);			/* Configures the PIT_0 timer */
void BCTU_Init (void);			/* Configures BCTU to trigger ADC_1 CH3 */
void LED_Config(void);			/* Assign LED ports on NXP EVB as GPIO outputs */
void update_LEDs(void);			/* Update LEDs with scaled channel 9 Result */

extern uint16_t Result;     	/* ADC channel conversion result */

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
	xcptn_xmpl ();				/* Configure and Enable Interrupts */
    peri_clock_gating();    	/* Configure gating/enabling peripheral(ADC, PIT, BCTU) clocks for modes*/
	system160mhz();         	/* sysclk=160MHz, dividers configured, mode transition*/
	LED_Config();           	/* Assign LED ports on NXP LED as GPIO outputs*/
	ADC1_PadConfig_ChanSelect();/* Configures ADC pads & select scan channel */
	SAR_ADC1_Init();			/* Initialize ADC_1 in Cross Trigger mode */
	PIT_Init();					/* Configures the PIT_0 timer */
	BCTU_Init();				/* Configures BCTU to trigger ADC_1 CH9 */

    int i;

	while(1) {
	  if (BCTU.MSR.B.NDATA1) { 		/* If normal scan channels finished converting */
	    ADC1_Read_Chan();    		/* Read conversion results */
	    update_LEDs();       		/* Update LEDs with scaled channel 9 result */

	    for(i=0; i<655350; i++);	/* Wait for some random time but less than PIT overflow time */

	    Result = 0x0000;			/* Manipulate result value to turn off the LEDS */
	    update_LEDs();       		/* Update LEDs with scaled channel 9 result */

	    BCTU.MSR.B.NDATA1_CLR = 1;	/* Clear End of CHain (ECH) status bit */
	  }
	}
	return 0;
}

/********************  End of Main ***************************************/

void peri_clock_gating (void) {		/* Configure gating/enabling peripheral(ADC, PIT, BCTU) clocks for modes*/
  MC_ME.RUN_PC[0].R = 0x00000000;	/* Gate off clock for all RUN modes */
  MC_ME.RUN_PC[1].R = 0x000000FE;	/* Enable peripheral clock for all RUN modes */
  	  	  	  	  	  	  	  	  	/* Note: RUN_PC0 is default configured for PCTLs */
  MC_ME.PCTL[25].B.RUN_CFG = 0x1;	/* ADC1: select peripheral configuration to RUN_PC[1] */
  MC_ME.PCTL[91].B.RUN_CFG = 0x1;	/* PIT: select peripheral configuration RUN_PC[1] */
  MC_ME.PCTL[0].B.RUN_CFG = 0x1;	/* BCTU: select peripheral configuration RUN_PC[1] */
}

void PIT_Init (void) {				/* Configures the PIT_0 timer */
    // PIT_3 BCTU[23] BCTU input trigger number 23
    // enable PIT clocks
    PIT.MCR.R = 0;					/* PIT Clock for RTI Timers is enabled */
	                                /* Clock for standard PIT timers is enabled */
    PIT.TIMER[3].LDVAL.R = 40000000;/* Init value for PIT counter */
    PIT.TIMER[3].TCTRL.R = 1;		/* Enable timer */
}

void BCTU_Init (void) {				/* Configures BCTU to trigger ADC_1 CH9 */
    /* Trigger Configuration */
    BCTU.TRGCFG[23].B.TRIGEN = 1;	/* Trigger enable */
    BCTU.TRGCFG[23].B.TRS = 0;		/* Single conversion */
    BCTU.TRGCFG[23].B.ADC_SEL1 = 1;	/* Selects ADC1 */
    BCTU.TRGCFG[23].B.CHANNEL_VALUE_OR_LADDR = 9;	/* Selects ADC0 Channel 9 */

    /* Global Trigger Enable. */
    BCTU.MCR.B.GTRGEN = 1;			/* Enables all trigger inputs at the same time */
}

void LED_Config(void) {   /* Assign LED ports on NXP LED as GPIO outputs*/
	  SIUL2.GPDO[PA4].R = 1;  /* LED1 Initial value: 1 = LED off on NXP EVB */
	  SIUL2.GPDO[PA0].R = 1;  /* LED2 Initial value: 1 = LED off on NXP EVB */
	  SIUL2.GPDO[PJ4].R = 1;  /* LED3 Initial value: 1 = LED off on NXP EVB */
	  SIUL2.GPDO[PH5].R = 1;  /* LED4 Initial value: 1 = LED off on NXP EVB */
	  SIUL2.GPDO[PC4].R = 1;  /* LED5 Initial value: 1 = LED off on NXP EVB */
	  SIUL2.GPDO[PH13].R = 1; /* LED6 Initial value: 1 = LED off on NXP EVB */
	  SIUL2.GPDO[PA7].R = 1;  /* LED7 Initial value: 1 = LED off on NXP EVB */
	  SIUL2.GPDO[PA10].R = 1; /* LED8 Initial value: 1 = LED off on NXP EVB */

	  SIUL2.MSCR[PA4].B.OBE = 1;   /* Port PA4 - LED 1 on NXP EVB */
	  SIUL2.MSCR[PA0].B.OBE = 1;   /* Port PA0 - LED 2 on NXP EVB  */
	  SIUL2.MSCR[PJ4].B.OBE = 1;   /* Port PJ4 - LED 3 on NXP EVB  */
	  SIUL2.MSCR[PH5].B.OBE = 1;   /* Port PH5 - LED 4 on NXP EVB  */
	  SIUL2.MSCR[PC4].B.OBE = 1;   /* Port PC4 - LED 5 on NXP EVB */
	  SIUL2.MSCR[PH13].B.OBE = 1;  /* Port PF15 - LED 6 on NXP EVB  */
	  SIUL2.MSCR[PA7].B.OBE = 1;   /* Port PA7 - LED 7 on NXP EVB  */
	  SIUL2.MSCR[PA10].B.OBE = 1;  /* Port PA10 - LED 8 on NXP EVB  */
}

void update_LEDs(void) {     /* Update LEDs with scaled channel 9 result  */
                             /* If Result bit is 0, then LED is turned ON */
                             /* If Result bit is 1, then LED is turned OFF */
    SIUL2.GPDO[PA4].R  = ~(Result & 0x0800)>>11;/* LED1: scaled channel 9 LSB */
    SIUL2.GPDO[PA0].R  = ~(Result & 0x0400)>>10;/* LED2 */
    SIUL2.GPDO[PJ4].R = ~(Result & 0x0200)>>9;  /* LED3 */
    SIUL2.GPDO[PH5].R = ~(Result & 0x0100)>>8;  /* LED4 */
    SIUL2.GPDO[PC4].R  = ~(Result & 0x0080)>>7; /* LED5 */
    SIUL2.GPDO[PH13].R  = ~(Result & 0x0040)>>6;/* LED6 */
    SIUL2.GPDO[PA7].R = ~(Result & 0x0020)>>5;  /* LED7 */
    SIUL2.GPDO[PA10].R = ~(Result & 0x0010)>>4; /* LED8: scaled channel 9 MSB */
}
