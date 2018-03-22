/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */
#include "gpio.h"

extern void xcptn_xmpl(void (*)(void));
extern void VTABLE2(void);

__attribute__ ((section(".text")))
int main2(void)
{
	int counter = 0;
	uint8_t t = 0;
	
	xcptn_xmpl (VTABLE2);

#define PCR_NUM 101		//PG5

	gpo_init_pcr_out_pnt_fnc( PCR_NUM );

	for(;;) {	   
	   	counter++;

	   	if (!(counter % 200000))
		{
	   		t ^= 1;
	   		gpo_pin_update_pnt_fnc(PCR_NUM, t);
		}

	}
	
	return 0;
}
