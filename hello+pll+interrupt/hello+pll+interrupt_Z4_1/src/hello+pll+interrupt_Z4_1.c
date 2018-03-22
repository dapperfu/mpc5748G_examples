/********************************************************************************/
/* FILE NAME: hello-pll-interrupt_z4_1.c  COPYRIGHT (c) NXP Semiconductors 2016	*/
/*                                          	           	All Rights Reserved */
/* PLATFORM: DEVKIT-MPC5748G													*/
/* DESCRIPTION: Main C program for e200z4a      	                            */
/*              Implements the project "hello+pll" plus introduces 				*/
/*				to the interrupts. PIT0 generates the interrupt in Core 0 and 	*/
/* 				toggles the DS4 LED at every 1s. After every 4 PIT0 interrupts	*/
/* 				it generates the software interrupt in core 1 which toggles		*/
/* 				LED DS7 at every 4s.											*/
/* 				Core 1 also generates PIT1 interrupt which toggles 				*/
/* 				LED DS5 at every 0.5s. 											*/
/* 				Core 2 also generates PIT2 interrupt which toggles 				*/
/* 				LED DS6 at every 0.25s.											*/
/*                                                                           	*/
/********************************************************************************/
/* REV      AUTHOR        DATE        DESCRIPTION OF CHANGE                  	*/
/* ---   -----------    ----------    ---------------------                  	*/
/* 1.0	  A.Inzunza     25 Jul 2013   Initial Release                	     	*/
/* 1.1    S.Mihalik     13 Sep 2013   Reorganized code & small changes       	*/
/* 1.2    D.Haro        12 Feb 2014   Added SW IRQ 1 init. & generation      	*/
/* 1.3    SM            27 Jan 2015   Removed unrequired SIUL ME_PCTL code   	*/
/* 1.4	  K Shah		25 Feb 2016	  Ported to S32DS						 	*/
/********************************************************************************/

#include "derivative.h" /* include peripheral declarations */
#include "pit.h"
#include "gpio.h"
#include "mode.h"

#define KEY_VALUE1 0x5AF0ul
#define KEY_VALUE2 0xA50Ful

#define LED_DS4 SIUL2.GPDO[PA10].B.PDO_4n


extern void xcptn_xmpl(void);
void peri_clock_gating(void); /* Configuration gating/enabling peripheral clocks for modes*/
void SW_INT_1_init(void); 	  /* Software Interrupt enable */

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
	xcptn_xmpl ();           /* Configure and Enable Interrupts */

    //Since We are using PIT- one of the peripherals. We need to enable peripheral clocks.
    peri_clock_gating();     /* Configure gating/enabling peripheral(PTI) clocks for modes*/
                             /* Configuration occurs after mode transition! */

    system160mhz();
    /* Sets clock dividers= max freq,
       calls PLL_160MHz function which:
       MC_ME.ME: enables all modes for Mode Entry module
       Connects XOSC to PLL
       PLLDIG: LOLIE=1, PLLCAL3=0x09C3_C000, no sigma delta, 160MHz
       MC_ME.DRUN_MC: configures sysclk = PLL
       Mode transition: re-enters DRUN which activates PLL=sysclk & peri clks
       */

    initGPIO();         /* Initializes LED, buttons & other general purpose pins for NXP EVB */

    PIT.MCR.B.MDIS = 0; /* Enable PIT module. NOTE: PIT module must be       */
                        /* enabled BEFORE writing to it's registers.         */
                        /* Other cores will write to PIT registers so the    */
                        /* PIT is enabled here before starting other cores.  */
    PIT.MCR.B.FRZ = 1;  /* Freeze PIT timers in debug mode */

    PIT0_init(40000000); /* Initialize PIT channel 0 for desired SYSCLK counts*/
             	 	 	 /* timeout= 40M  PITclks x 4 sysclks/1 PITclk x 1 sec/160Msysck */
    					 /*        = 40M x 4 / 160M = 160/160 = 1 sec.  */

    SW_INT_1_init();   	 /* Initialize SW INT1 (to be serviced by core 1) */

    while(1);
	
	return 0;
}

/************************  End of Main ***************************************/

void peri_clock_gating (void) {
  MC_ME.RUN_PC[0].R = 0x00000000;  /* Gate off clock for all RUN modes */
  MC_ME.RUN_PC[1].R = 0x000000FE;  /* Configures peripheral clock for all RUN modes */

  MC_ME.PCTL[91].B.RUN_CFG = 0x1;  /* PIT: select peripheral configuration RUN_PC[1] */
}

void PIT0_isr(void) {
	static uint8_t counter=0;	/* Increment ISR counter */

	counter++;
	LED_DS4 = ~LED_DS4;         /* Toggle DS4 LED port */
    if(counter == 4)
    {
    	counter = 0;
    	INTC_SSCIR1=0x02;
    }
    PIT.TIMER[0].TFLG.R = 1;  	/* Clear interrupt flag */
}

void SW_INT_1_init(void) {
    INTC.PSR[1].B.PRC_SELN = 0x4;  /* IRQ sent to Core 1 */
    INTC.PSR[1].B.PRIN =0x0F;      /* IRQ priority = 15 (15 = highest) */
}
