/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "Cpu.h"
#include "clockMan1.h"
#include "pin_mux.h"
#include "ethernet1.h"
#include "device_registers.h"

volatile int exit_code = 0;
/* User includes (#include below this line is not maintained by Processor Expert) */
#include <stdint.h>
#include <stdbool.h>
/* This example is setup to work by default with EVB. To use it with other boards
   please comment the following line
*/

/* This example is setup to work by default with DEVKIT. To use it with other boards
   please comment the following line
*/
#define DEVKIT

#ifdef DEVKIT
    #define LED 148       /* pin PJ[4] - LED1 (DS9) on DEV-KIT */
#else
    #define LED 98        /* pin PG[2] - LED1 (DS2) on Motherboard */
#endif

#define FRAME_ETH_DEST_MAC_OFFSET   (0U)
#define FRAME_ETH_SRC_MAC_OFFSET    (6U)

#define FRAME_IP_PROTO_OFFSET       (23U)
#define FRAME_IP_CHECKSUM_OFFSET    (24U)
#define FRAME_IP_SRC_OFFSET	        (26U)
#define FRAME_IP_DEST_OFFSET        (30U)
#define FRAME_IP_PROTO_ICMP         (1U)

#define FRAME_ICMP_TYPE_OFFSET          (34U)
#define FRAME_ICMP_CHECKSUM_OFFSET      (36U)
#define FRAME_ICMP_TYPE_ECHO_REQUEST    (8U)
#define FRAME_ICMP_TYPE_ECHO_REPLY      (0U)

void copy_buff(uint8_t *dest, uint8_t *src, uint8_t len)
{
	uint8_t i;

	for (i = 0; i < len; i++)
	{
		dest[i] = src[i];
	}
}

void delay(volatile int cycles)
{
    /* Delay function - do nothing for a number of cycles */
    while(cycles--);
}

void build_ping_reply(uint8_t *data)
{
	uint8_t src_mac[6]  = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 };	/* MAC address of board */
	uint8_t src_ip[4]   = { 0xc0, 0xa8, 0x01, 0x13 };				/* 192.168.1.19 IP of board */

	/* Clear IP and ICMP checksums (to be filled in by the MAC) */
	data[FRAME_IP_CHECKSUM_OFFSET] = 0;
	data[FRAME_IP_CHECKSUM_OFFSET + 1] = 0;
	data[FRAME_ICMP_CHECKSUM_OFFSET] = 0;
	data[FRAME_ICMP_CHECKSUM_OFFSET + 1] = 0;

	/* Update ICMP type to represent an echo reply */
	data[FRAME_ICMP_TYPE_OFFSET] = FRAME_ICMP_TYPE_ECHO_REPLY;

	/* Update MAC and IP addresses */
	copy_buff(&data[FRAME_ETH_DEST_MAC_OFFSET], &data[FRAME_ETH_SRC_MAC_OFFSET], 6);
	copy_buff(&data[FRAME_ETH_SRC_MAC_OFFSET], src_mac, 6);
	copy_buff(&data[FRAME_IP_DEST_OFFSET], &data[FRAME_IP_SRC_OFFSET], 4);
	copy_buff(&data[FRAME_IP_SRC_OFFSET], src_ip, 4);
}

void rx_callback(uint8_t instance, enet_event_t event)
{
	status_t stat;

	if (event == ENET_RX_EVENT)
	{
		enet_buffer_t rxBuff;

		stat = ENET_DRV_ReadFrame(INST_ETHERNET1, &rxBuff, NULL);

		if (stat == STATUS_SUCCESS)
		{
			if (rxBuff.length < FRAME_ICMP_TYPE_OFFSET)
			{
				ENET_DRV_ProvideRxBuff(INST_ETHERNET1, &rxBuff);
				return;
			}

			/* Check that the received frame was an ICMP echo request */
			if (rxBuff.data[FRAME_IP_PROTO_OFFSET] == FRAME_IP_PROTO_ICMP &&
					rxBuff.data[FRAME_ICMP_TYPE_OFFSET] == FRAME_ICMP_TYPE_ECHO_REQUEST)
			{
				build_ping_reply(rxBuff.data);

				rxBuff.length = rxBuff.length - 4;

				/* Send back an ICMP echo reply frame */
				ENET_DRV_SendFrame(INST_ETHERNET1, &rxBuff);
			}

			ENET_DRV_ProvideRxBuff(INST_ETHERNET1, &rxBuff);
		}
	}
}

int main(void)
{

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* Initialize and configure clocks
   * 	-	see clock manager component for details
   */

  CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
  CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

  PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

#ifdef DEVKIT
  /* Only RMII mode is supported on DEV-KIT */
  ethernet1_InitConfig0.miiMode = ENET_RMII_MODE;
#endif
  ENET_DRV_Init(INST_ETHERNET1, &ethernet1_State, &ethernet1_InitConfig0, &ethernet1_buffConfig0, ethernet1_MacAddr);

  for (;;)
  {
      /* Insert a small delay to make the blinking visible */
      delay(720000);

      /* Toggle output value LED1 */
      SIUL2->GPDO[LED/4] = SIUL2->GPDO[LED/4] ^ (SIUL2_GPDO_PDO_4n_WIDTH << (SIUL2_GPDO_PDO_4n_SHIFT - (8 * (LED & 0x03))));
  }

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.1 [05.21]
**     for the Freescale S32K series of microcontrollers.
**
** ###################################################################
*/
