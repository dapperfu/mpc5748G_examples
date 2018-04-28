/*
 * pit.c
 *
 *  Created on: Feb 25, 2016
 *      Author: B55457
 */


#include "pit.h"

void PIT0_init(uint32_t LDVAL) {
    PIT.TIMER[0].LDVAL.R = LDVAL;  	/* Load # Pit clocks to count */
    PIT.TIMER[0].TCTRL.B.TIE = 1;      	/* Enable interrupt */
    INTC.PSR[226].B.PRC_SELN = 0x8;  /* IRQ sent to Core 0 */
    INTC.PSR[226].B.PRIN =10;        /* IRQ priority = 10 (15 = highest) */
    PIT.TIMER[0].TCTRL.B.TEN = 1;      	/* enable channel */
}

void PIT1_init(uint32_t LDVAL) {
    PIT.TIMER[1].LDVAL.R = LDVAL;  	/* load PIT counter */
    PIT.TIMER[1].TCTRL.B.TIE = 1;      	/* enable interrupt */
    INTC.PSR[227].B.PRC_SELN = 0x4;     /* IRQ sent to Core 1 */
    INTC.PSR[227].B.PRIN = 0x9;         /* IRQ priority = 9 (15 = highest) */
    PIT.TIMER[1].TCTRL.B.TEN = 1;      	/* enable channel */
}

void PIT2_init(uint32_t LDVAL) {
    PIT.TIMER[2].LDVAL.R = LDVAL;  	/* load PIT counter */
    PIT.TIMER[2].TCTRL.B.TIE = 1;      	/* enable interrupt */
    INTC.PSR[228].B.PRC_SELN = 0x2;     /* IRQ sent to Core 2 */
    INTC.PSR[228].B.PRIN =11;           /* IRQ priority = 10 (15 = highest) */
    PIT.TIMER[2].TCTRL.B.TEN = 1;      	/* enable channel */
}
