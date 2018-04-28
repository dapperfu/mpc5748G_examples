/*
 * eMIOS.h
 *
 *  Created on: Feb 26, 2016
 *      Author: B55457
 */

#ifndef EMIOS_H_
#define EMIOS_H_

#include "project.h"

void init_emios_global_prescalers(void);
void emios0_ch23_OPWFMB(); 		/* Configures eMIOS 0 channel 23: OPWFMB 200 counts-200 us period*/
void emios1_ch23_MCB();    		/* Configures eMIOS 1 channel 23: MCB, 1K count (1K usec period) */
void emios1_ch18_OPWMB (void); 	/* Configures eMIOS 1 channel 18: OPWMB with time base eMIOS1 ch0 */
void emios1_ch05_IPM();			/* Configures eMIOS 1 channel 5:  IPM   with time base eMIOS1 ch0 */
void emios1_ch06_IPWM();		/* Configures eMIOS 1 channel 6:  IPWM  with time base eMIOS1 ch0 */
void enable_emios (void);

#endif /* EMIOS_H_ */
