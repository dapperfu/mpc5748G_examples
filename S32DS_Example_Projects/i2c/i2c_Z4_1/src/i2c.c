/*
 * i2c.c
 *
 *  Created on: Mar 31, 2016
 *      Author: B55457
 */

#include "i2c.h"

void initI2C_1(
    void) { /* I2C1 module configured as a Master and in Transmit mode */
  // I2C_1 SDA SIGNAL
  SIUL2.MSCR[PE1].B.OBE = 1;
  SIUL2.MSCR[PE1].B.ODE = 1;
  SIUL2.MSCR[PE1].B.IBE = 1;
  SIUL2.MSCR[PE1].B.PUS = 0;
  SIUL2.MSCR[PE1].B.PUE = 1;
  SIUL2.MSCR[PE1].B.SSS = 3;
  SIUL2.IMCR[268].R = 1;

  // I2C_1 SCL SIGNAL
  SIUL2.MSCR[PE0].B.OBE = 1;
  SIUL2.MSCR[PE0].B.ODE = 1;
  SIUL2.MSCR[PE0].B.IBE = 1;
  SIUL2.MSCR[PE0].B.PUS = 0;
  SIUL2.MSCR[PE0].B.PUE = 1;
  SIUL2.MSCR[PE0].B.SSS = 2;
  SIUL2.IMCR[267].R = 1;

  I2C_1.IBFD.B.IBC =
      0x20; /* Division ratio to obtain SCL frequency from Platform clock/2. */

  I2C_1.IBAD.R = 0XFD; /* Address of slave is 1 */
  ;

  I2C_1.IBCR.B.MDIS = 0; /* I2C module is enabled */
  I2C_1.IBCR.B.IBIE = 0; /* Interrupt disabled */
  I2C_1.IBCR.B.TXRX = 1; /* Transmits Data */
}

void transmit_addr_master_I2C_1(uint8_t data) { /* Master transmit function */

  while (I2C_1.IBSR.B.IBB != 0)
    ;                    /* See if Bus is busy or not */
  I2C_1.IBCR.B.MSSL = 1; /* Master mode */
  I2C_1.IBCR.B.TXRX = 1; /* Transmit mode */
  I2C_1.IBDR.R = data;   /* Slave Address == 1111111, Transmit mode */
  while (I2C_1.IBSR.B.RXAK != 0)
    ; /* Check for Acknowledgement */
  while (I2C_1.IBSR.B.IBB != 1)
    ; /* Check if bus is acquired or not */
}
void transmit_data_master_I2C_1(uint8_t data) { /* Master transmit function */

  I2C_1.IBSR.B.IBIF = 0;
  I2C_1.IBDR.R = data;
  while (I2C_1.IBSR.B.TCF != 1)
    ; /* Data transfer in progress */
  while (I2C_1.IBSR.B.IBIF != 1)
    ; /* Data transfer in progress */
  (void)I2C_1.IBSR.R;
}

void initI2C_2(
    void) { /* I2C2 module configured as a Slave and in Receive mode */

  // I2C_2 SDA SIGNAL
  //	SIUL2.MSCR[PE8].B.OBE = 1;
  SIUL2.MSCR[PE8].B.ODE = 1;
  SIUL2.MSCR[PE8].B.IBE = 1;
  //	SIUL2.MSCR[PE8].B.PUS = 0;
  //	SIUL2.MSCR[PE8].B.PUE = 1;
  //	SIUL2.MSCR[PE8].B.SSS = 4;
  SIUL2.IMCR[270].R = 1;

  // I2C_2 SCL SIGNAL
  SIUL2.MSCR[PE9].B.OBE = 1;
  SIUL2.MSCR[PE9].B.ODE = 1;
  SIUL2.MSCR[PE9].B.IBE = 1;
  SIUL2.MSCR[PE9].B.PUS = 0;
  SIUL2.MSCR[PE9].B.PUE = 1;
  SIUL2.MSCR[PE9].B.SSS = 2;
  SIUL2.IMCR[269].R = 1;

  I2C_2.IBFD.B.IBC =
      0x20; /* Division ratio to obtain SCL frequency from Platform clock/2 */
  I2C_2.IBAD.R = 0XFE; /* Address of slave is 1 */
  ;

  I2C_2.IBCR.B.MDIS = 0;  /* I2C module is enabled */
  I2C_2.IBCR.B.IBIE = 0;  /* Interrupt disabled */
  I2C_2.IBCR.B.MSSL = 0;  /* Slave mode */
  I2C_2.IBCR.B.NOACK = 0; /* Acknowledge the received data */
  I2C_2.IBSR.B.IBIF = 0;
  (void)I2C_2.IBDR.R;
}

void receive_addr_slave_I2C_2(void) { /* Slave Address Receive function */

  while (I2C_2.IBSR.B.IBIF != 1)
    ; /* Data transfer in progress */
  while (I2C_2.IBSR.B.IAAS != 1)
    ; /* Addressed as a slave progress */

  if (I2C_2.IBSR.B.SRW == 1)
    I2C_2.IBCR.B.TXRX = 1; /* Slave Transmit Data */
  else
    I2C_2.IBCR.B.TXRX = 0; /* Slave receive Data */
  (void)I2C_2.IBDR.R;
  (void)I2C_2.IBSR.R;
  I2C_2.IBSR.R = 0x02;
}

uint8_t receive_data_slave_I2C_2(void) { /* Slave Data Receive function */

  while (I2C_2.IBSR.B.TCF != 1)
    ; /* Data transfer in progress */
  while (I2C_2.IBSR.B.IBIF != 1)
    ; /* Data transfer in progress */
  (void)I2C_2.IBSR.R;
  I2C_2.IBSR.R = 0x02;
  return I2C_2.IBDR.R;
}
