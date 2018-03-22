/*****************************************************************************/
/* FILE NAME: 	 i2c_Z4_1.c           COPYRIGHT (c) NXP Semiconductors 2016  */
/*                                                      All Rights Reserved  */
/* PLATFORM:	 DEVKIT-MPC5748G											 */
/* DESCRIPTION:  Initializes the I2C_1 in master mode and I2C_2 in slave mode*/
/*				 Master sends the address and data and Slave receives both   */
/*				 The address and the data									 */
/*				 Provide a pull-up of 3.3V via pull-up register(10k) to both */
/* 				 SCL and SDA lines 											 */
/*				 	I2C_1					I2C_2							 */
/*               SCL: PE0: J4_16		SCL: PE9: J2_8                       */
/*				 SDA: PE1: J4_14		SDA: PE8: J2_6						 */
/*****************************************************************************/
/* REV      AUTHOR              DATE        DESCRIPTION OF CHANGE            */
/* ---   -----------          ----------    ---------------------            */
/* 1.0	 	K Shah            31 Mar 2016   Initial working version			 */
/*																			 */
/*****************************************************************************/

#include "derivative.h" /* include peripheral declarations */
#include "project.h"
#include "mode.h"
#include "i2c.h"


#define KEY_VALUE1 0x5AF0ul
#define KEY_VALUE2 0xA50Ful

extern void xcptn_xmpl(void);
void peri_clock_gating(void); /* Configure gating/enabling peripheral(I2C) clocks */

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
	uint8_t data;

	xcptn_xmpl ();              /* Configure and Enable Interrupts */

	peri_clock_gating();		/* Configure gating/enabling peripheral(I2C) clocks */
	system160mhz();
    /* Sets clock dividers= max freq,
       calls PLL_160MHz function which:
       MC_ME.ME: enables all modes for Mode Entry module
       Connects XOSC to PLL
       PLLDIG: LOLIE=1, PLLCAL3=0x09C3_C000, no sigma delta, 160MHz
       MC_ME.DRUN_MC: configures sysclk = PLL
       Mode transition: re-enters DRUN which activates PLL=sysclk & peri clks
       */
	initI2C_1(); /* I2C1 module configured as a Master and in Transmit mode */
	initI2C_2(); /* I2C2 module configured as a Slave and in Receive mode */

	/* Transmit Address */
	transmit_addr_master_I2C_1(0x01);
	data = receive_slave_I2C_2();

	if(data == 1)
	{
		SIUL2.MSCR[PA10].B.OBE = 1;  /* Pad PA10 (10): OBE=1. On EVB active low DS4 LED */
	}

	while(1)
	{
		transmit_data_master_I2C_1('H');
		data = receive_slave_I2C_2();
		if(data == 'H')
		{
			SIUL2.MSCR[PA10].B.OBE ^= 1;  /* Pad PA10 (10): OBE=OBE xor 1. On EVB active low DS4 LED */
			data = 0;
		}
	}
	return 0;
}

/********************  End of Main ***************************************/

void peri_clock_gating() { /* Configure gating/enabling peripheral(I2C) clocks */
	MC_ME.RUN_PC[0].R = 0x00000000;  /* Gate off clock for all RUN modes */
	MC_ME.RUN_PC[1].R = 0x000000FE;  /* Configures peripheral clock for all RUN modes */
	MC_ME.PCTL[31].B.RUN_CFG = 0x1;  /* I2C_1: select peripheral configuration RUN_PC[1] */
	MC_ME.PCTL[32].B.RUN_CFG = 0x1;  /* I2C_2: select peripheral configuration RUN_PC[1] */
}
