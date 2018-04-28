/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */

extern void xcptn_xmpl(void (*)(void));
void VTABLE2(void);

__attribute__ ((section(".text")))
int main2(void)
{
	int counter = 0;
	
	xcptn_xmpl (VTABLE2);              /* Configure and Enable Interrupts */

	for(;;) {	   
	   	counter++;
	}
	
	return 0;
}
