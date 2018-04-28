/*
 * gpio.h
 *
 *  Created on: May 24, 2016
 *      Author: agrigorev
 */

#ifndef GPIO_H_
#define GPIO_H_

#define PSR_OUTPUT  0x02800000		/* OBE | SMC */
#define PSR_INPUT   0x00080000		/* IBE */


/* GPO Write Data function */
void gpo_pin_update_pnt_fnc(uint8_t pcr_num , uint8_t data);

/* GPO Init Pcr as Output function */
void gpo_init_pcr_out_pnt_fnc( uint8_t pcr_num );

#endif /* GPIO_H_ */
