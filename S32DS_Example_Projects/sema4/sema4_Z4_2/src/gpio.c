/*
 * gpio.c
 *
 *  Created on: Feb 23, 2016
 *      Author: B55457
 */

#include "gpio.h"

void initGPIO(void)
{
	/* LEDS on CalypsoEVB */
	SIUL2.MSCR[PA10].B.SSS = 0;			/* Pin functionality as GPIO */
	SIUL2.MSCR[PA10].B.OBE = 1;         /* Output Buffer Enable on */
	SIUL2.MSCR[PA10].B.IBE = 0;			/* Input Buffer Enable off */
	SIUL2.GPDO[PA10].B.PDO_4n = 1;		/* Turn LED off, note that the LEDs are connected backwards 0 for ON, 1 for OFF */

	SIUL2.MSCR[PA7].B.SSS = 0;			/* Pin functionality as GPIO */
	SIUL2.MSCR[PA7].B.OBE = 1;          /* Output Buffer Enable on */
	SIUL2.MSCR[PA7].B.IBE = 0;			/* Input Buffer Enable off */
	SIUL2.GPDO[PA7].B.PDO_4n = 1;		/* Turn LED off, note that the LEDs are connected backwards 0 for ON, 1 for OFF */

	SIUL2.MSCR[PH13].B.SSS = 0;			/* Pin functionality as GPIO */
	SIUL2.MSCR[PH13].B.OBE = 1;         /* Output Buffer Enable on */
	SIUL2.MSCR[PH13].B.IBE = 0;			/* Input Buffer Enable off */
	SIUL2.GPDO[PH13].B.PDO_4n = 1;		/* Turn LED off, note that the LEDs are connected backwards 0 for ON, 1 for OFF */

	SIUL2.MSCR[PC4].B.SSS = 0;			/* Pin functionality as GPIO */
	SIUL2.MSCR[PC4].B.OBE = 1;          /* Output Buffer Enable on */
	SIUL2.MSCR[PC4].B.IBE = 0;			/* Input Buffer Enable off */
	SIUL2.GPDO[PC4].B.PDO_4n = 1;		/* Turn LED off, note that the LEDs are connected backwards 0 for ON, 1 for OFF */

	SIUL2.MSCR[PH5].B.SSS = 0;			/* Pin functionality as GPIO */
	SIUL2.MSCR[PH5].B.OBE = 1;          /* Output Buffer Enable on */
	SIUL2.MSCR[PH5].B.IBE = 0;			/* Input Buffer Enable off */
	SIUL2.GPDO[PH5].B.PDO_4n = 1;		/* Turn LED off, note that the LEDs are connected backwards 0 for ON, 1 for OFF */

	SIUL2.MSCR[PJ4].B.SSS = 0;			/* Pin functionality as GPIO */
	SIUL2.MSCR[PJ4].B.OBE = 1;          /* Output Buffer Enable on */
	SIUL2.MSCR[PJ4].B.IBE = 0;			/* Input Buffer Enable off */
	SIUL2.GPDO[PJ4].B.PDO_4n = 1;		/* Turn LED off, note that the LEDs are connected backwards 0 for ON, 1 for OFF */

	SIUL2.MSCR[PA0].B.SSS = 0;			/* Pin functionality as GPIO */
	SIUL2.MSCR[PA0].B.OBE = 1;          /* Output Buffer Enable on */
	SIUL2.MSCR[PA0].B.IBE = 0;			/* Input Buffer Enable off */
	SIUL2.GPDO[PA0].B.PDO_4n = 1;		/* Turn LED off, note that the LEDs are connected backwards 0 for ON, 1 for OFF */

	SIUL2.MSCR[PA4].B.SSS = 0;			/* Pin functionality as GPIO */
	SIUL2.MSCR[PA4].B.OBE = 1;          /* Output Buffer Enable on */
	SIUL2.MSCR[PA4].B.IBE = 0;			/* Input Buffer Enable off */
	SIUL2.GPDO[PA4].B.PDO_4n = 1;		/* Turn LED off, note that the LEDs are connected backwards 0 for ON, 1 for OFF */

	/* Buttons on CalypsoEVB */
	SIUL2.MSCR[PA3].B.SSS = 0;			/* Pin functionality as GPIO */
	SIUL2.MSCR[PA3].B.OBE = 0;          /* Output Buffer Enable off */
	SIUL2.MSCR[PA3].B.IBE = 1;			/* Input Buffer Enable on */

	SIUL2.MSCR[PE12].B.SSS = 0;			/* Pin functionality as GPIO */
	SIUL2.MSCR[PE12].B.OBE = 0;         /* Output Buffer Enable off */
	SIUL2.MSCR[PE12].B.IBE = 1;			/* Input Buffer Enable on */

/* General purpose output pins for test: */
	SIUL2.MSCR[103].B.SSS = 0;			/* PG7: Pin functionality as GPIO */
	SIUL2.MSCR[103].B.OBE = 1;          /* Output Buffer Enable on */
	SIUL2.MSCR[103].B.IBE = 0;			/* Input Buffer Enable off */
	SIUL2.GPDO[103].B.PDO_4n = 0;		/* Initialize low */

	SIUL2.MSCR[104].B.SSS = 0;			/* PG8: Pin functionality as GPIO */
	SIUL2.MSCR[104].B.OBE = 1;          /* Output Buffer Enable on */
	SIUL2.MSCR[104].B.IBE = 0;			/* Input Buffer Enable off */
	SIUL2.GPDO[104].B.PDO_4n = 0;		/* Initialize low */

}

void GPIO_toggle(uint16_t GPIO, uint32_t TOGGLES, uint32_t DELAY)
{
  uint32_t i, j;

  SIUL2.MSCR[GPIO].B.OBE   = 1;
  for(i=0;i<TOGGLES*2;i++)
  {
   for(j=0;j<DELAY;j++);
   SIUL2.GPDO[GPIO].R ^= 1;
  }
}

void DebounceDelay(void)
{
    vuint32_t DelayCounter;
    for (DelayCounter=0; DelayCounter<DEBOUNCEDELAYTIME; DelayCounter++){};
}

void DebouncedWaitTilLow(uint16_t GPIO)
{
    /* enable GPIO as input */
    SIUL2.MSCR[GPIO].B.IBE  = 1;
    /* debounce */
    while(!SIUL2.GPDI[GPIO].R)  {};   /* wait here until level is high, if not already */
    do
    {
      while(SIUL2.GPDI[GPIO].R) {};   /* wait for falling edge */
      DebounceDelay();
    }while(SIUL2.GPDI[GPIO].R);       /* repeat loop, if after debounce delay high */
}

void DebouncedWaitTilHigh(uint16_t GPIO)
{
    /* enable GPIO as input */
    SIUL2.MSCR[GPIO].B.IBE  = 1;
    /* debounce */
    while(SIUL2.GPDI[GPIO].R)  {};   /* wait here until level is low, if not already */
    do
    {
      while(!SIUL2.GPDI[GPIO].R) {}; /* wait for rising edge */
      DebounceDelay();
    }while(!SIUL2.GPDI[GPIO].R);     /* repeat loop, if after debounce delay low */

}
