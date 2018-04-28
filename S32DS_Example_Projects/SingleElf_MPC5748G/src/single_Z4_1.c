/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */

extern void xcptn_xmpl(void (*)(void));

extern void VTABLE1(void);

__attribute__ ((section(".text")))
int main1(void)
{
	int counter = 0;
	AXBS_0.PORT[3].CRS.B.ARB = 1;  /* Round-robin (rotating) priority */
	
	xcptn_xmpl (VTABLE1);              /* Configure and Enable Interrupts */

	for(;;) {	   
	   	counter++;
	}
	
	return 0;
}
