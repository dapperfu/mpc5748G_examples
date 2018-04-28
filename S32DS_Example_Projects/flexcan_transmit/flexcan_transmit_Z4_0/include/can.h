/*
 * can.h
 *
 *  Created on: Mar 1, 2016
 *      Author: B55457
 */

#ifndef CAN_H_
#define CAN_H_

#include "derivative.h"
#include "project.h"

void initCAN_0_rx(void);
void initCAN_0_tx (void);
void TransmitMsg (void);
void ReceiveMsg (void);
void delay(void);

#endif /* CAN_H_ */
