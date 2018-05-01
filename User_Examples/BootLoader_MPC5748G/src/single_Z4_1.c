/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */
#include "gpio.h"

extern void xcptn_xmpl(void (*)(void));

extern void VTABLE1(void);

__attribute__((section(".text"))) int main1(void) {
  int counter = 0;
  uint8_t t = 0;
  AXBS_0.PORT[3].CRS.B.ARB = 1; /* Round-robin (rotating) priority */

  xcptn_xmpl(VTABLE1); /* Configure and Enable Interrupts */

#define PCR_NUM 99 // PG3

  gpo_init_pcr_out_pnt_fnc(PCR_NUM);

  for (;;) {
    counter++;

    if (!(counter % 50000)) {
      t ^= 1;
      gpo_pin_update_pnt_fnc(PCR_NUM, t);
    }
  }

  return 0;
}
