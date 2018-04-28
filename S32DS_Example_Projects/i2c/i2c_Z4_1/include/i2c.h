/*
 * i2c.h
 *
 *  Created on: Mar 31, 2016
 *      Author: B55457
 */

#ifndef I2C_H_
#define I2C_H_

#include "derivative.h"
#include "project.h"

void initI2C_1(void);
void transmit_addr_master_I2C_1(uint8_t data);
void transmit_data_master_I2C_1(uint8_t data);
void initI2C_2(void);
void receive_addr_slave_I2C_2(void);
uint8_t receive_data_slave_I2C_2(void);


#endif /* I2C_H_ */
