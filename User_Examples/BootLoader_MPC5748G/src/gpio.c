#include "derivative.h"
#include "gpio.h"

  void gpo_pin_update_pnt_fnc(uint8_t pcr_num, uint8_t data)
  {
    /* Write data to GPIO Pin Data output Registers  */
	  SIUL2.GPDO[pcr_num].R = data;
  }

  void gpo_init_pcr_out_pnt_fnc( uint8_t pcr_num )
  {
    /* Init PCR to Digital Output Configuration */
	  SIUL2.MSCR[pcr_num].R = PSR_OUTPUT;
  }
