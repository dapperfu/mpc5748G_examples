/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */

extern void xcptn_xmpl(void);

int main(void)
{
	volatile int counter = 0;
	AXBS_0.PORT[3].CRS.B.ARB = 1;  /* Round-robin (rotating) priority */
	
	xcptn_xmpl ();              /* Configure and Enable Interrupts */

	/* Loop forever */
	for(;;) {	   
	   	counter++;
	}
}
