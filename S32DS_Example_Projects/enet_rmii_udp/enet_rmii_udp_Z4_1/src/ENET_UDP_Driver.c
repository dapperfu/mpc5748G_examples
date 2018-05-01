/*
 * ENET_UDP_DRIVER.c
 *
 *  Created on: Oct 11, 2014
 *      Author: B34981
 *
 *  Modified: April, 2015
 *      Author: B46845
 */

/**********************************************************************************************
 * Includes
 **********************************************************************************************/
#include "ENET_UDP_Driver.h"
//#include "Interrupts/MPC56xx_55xx_Dual_PRC0_Interrupt_Init.h"

/**********************************************************************************************
 * Constants and macros
 **********************************************************************************************/
/* Buffer sizes in bytes -- The maximum allowable frame size is 1518
 * (or 1522 if VLAN tags are supported). The buffer size must be divisable
 * by 64. 1536 is the smallest 64 multiple that fits 1522 bytes.
 */
#define RX_BUFFER_SIZE 1536
#define TX_BUFFER_SIZE 1536

/* Number of Receive and Transmit Buffers and Buffer Descriptors per Queue */
#define NUM_RXBDS 2
#define NUM_TXBDS 2

/**********************************************************************************************
 * External objects
 **********************************************************************************************/

/**********************************************************************************************
 * Global variables
 **********************************************************************************************/
//{
// Receive queues
eNBUF *arp_queue[NUM_RXBDS];
eNBUF *ipv4_queue[NUM_RXBDS];

// Buffer descriptors
eNBUF txbd[NUM_TXBDS]
    __attribute__((aligned(64))); /* Create Transmission Buffer Descriptors */
eNBUF rxbd[NUM_RXBDS]
    __attribute__((aligned(64))); /* Create Reception Buffer Descriptors */
// Data buffers
uint8_t TxBuffer[NUM_TXBDS][TX_BUFFER_SIZE] __attribute__((aligned(
    64))); /* Create transmission buffer. Size must be divisible over 16 */
uint8_t RxBuffer[NUM_RXBDS][RX_BUFFER_SIZE] __attribute__((
    aligned(64))); /* Create reception buffer. Size must be divisible over 16 */

// Next buffer to use
uint8_t nextTxBD = 0;
uint8_t nextRxBD = 0;

uint8_t local_IP[4] = {ORIGIP1, ORIGIP2, ORIGIP3, ORIGIP4};

// Receive packet counter
// vlan, ipv4, arp, ipv6
Rx_count counter = {0, 0, 0, 0};
//}

/**********************************************************************************************
 * Global variables - Frame + packet templates
 **********************************************************************************************/
//{

#define ETH_HEADER_LEN 14

/* ARP response packet format*/
#define ARP_PACKET_LEN 42
uint8_t ARP_packet[] = {
    /* Starts Ethernet frame */
    DESTMAC1, DESTMAC2, DESTMAC3, DESTMAC4, DESTMAC5,
    DESTMAC6, // Destination MAC Address
    ORIGMAC1, ORIGMAC2, ORIGMAC3, ORIGMAC4, ORIGMAC5,
    ORIGMAC6,   // Origin MAC Address
    0x08, 0x06, // EtherType (ARP)
    /* Starts ARP response */
    0x00, 0x01, // Hardware type (Ethernet) 14, 15
    0x08, 0x00, // Protocol type (IPv4) 16, 17
    0x06,       // Hardware address length (6 bytes)    18
    0x04,       // Protocol address length (4 bytes)    19
    0x00, 0x02, // Opcode (ARP Reply)   20, 21
    ORIGMAC1, ORIGMAC2, ORIGMAC3, ORIGMAC4, ORIGMAC5,
    ORIGMAC6, // Sender HW address    22, 23, 24, 25, 26, 27
    ORIGIP1, ORIGIP2, ORIGIP3,
    ORIGIP4, // Sender protocol address  28, 29, 30, 31
    DESTMAC1, DESTMAC2, DESTMAC3, DESTMAC4, DESTMAC5,
    DESTMAC6, // Target HW address    32, 33, 34, 35, 36, 37
    DESTIP1, DESTIP2, DESTIP3,
    DESTIP4, // Target protocol address  38, 39, 40, 41
    /* End of ARP frame */
    0x00, 0x00, 0x00, 0x00 // Ethernet frame checksum
                           /* End of Ethernet frame */
};

/* UDP Packet format*/
#define PACKET_LEN_BEFORE_UDP_PAYLOAD 42
#define UDP_HEADER_SIZE 8
#define IPV4_HEADER_SIZE 20
uint8_t UDP_packet[] = {
    /* Starts Ethernet frame */
    DESTMAC1, DESTMAC2, DESTMAC3, DESTMAC4, DESTMAC5,
    DESTMAC6, // Destination MAC Address
    ORIGMAC1, ORIGMAC2, ORIGMAC3, ORIGMAC4, ORIGMAC5,
    ORIGMAC6,   // Origin MAC Address
    0x08, 0x00, // EtherType (IPV4)
    /* Starts IPV4 frame */
    0x45,       // Version and Header
    0x00,       // Services
    0x00, 0x20, // Total Length, whole packet size including header and data
    0xF5, 0x4F, // Identification
    0x00, 0x00, // Flags
    0x80,       // Time To Live
    UDP,        // Protocol (UDP)
    0x00, 0x00, // IPV4 Header Checksum (calculated by ENET module)
    ORIGIP1, ORIGIP2, ORIGIP3, ORIGIP4, // Origin IP
    DESTIP1, DESTIP2, DESTIP3, DESTIP4, // Destination IP
    /* End of IPV4 frame */
    /* Start UDP frame */
    ((ORIGPORT >> 8) & 0xFF), (ORIGPORT & 0xFF), // Source Port
    ((DESTPORT >> 8) & 0xFF), (DESTPORT & 0xFF), // Dest Port
    0x00, 0x0C, // Length of the UDP frame including header and data
    0x00, 0x00, // UDP Checksum of header and data (uses pseudo header with IPV4
                // frame)
    /* UDP payload goes here */
    /* End of UDP frame */
    0x00, 0x00, 0x00, 0x00 // Ethernet frame checksum
                           /* End of Ethernet frame */
};

/* ICMP Packet format*/
#define ICMP_PACKET_LENGTH 42
uint8_t ICMP_packet[] = {
    /* Starts Ethernet frame */
    DESTMAC1, DESTMAC2, DESTMAC3, DESTMAC4, DESTMAC5,
    DESTMAC6, // Destination MAC Address
    ORIGMAC1, ORIGMAC2, ORIGMAC3, ORIGMAC4, ORIGMAC5,
    ORIGMAC6,   // Origin MAC Address
    0x08, 0x00, // EtherType (IPV4)
    /* Starts IPV4 frame */
    0x45,       // Version and Header
    0x00,       // Services
    0x00, 0x20, // Total Length, whole packet size including header and data
    0xF5, 0x4F, // Identification
    0x00, 0x00, // Flags
    0x80,       // Time To Live
    ICMP,       // Protocol (ICMP)
    0x00, 0x00, // IPV4 Header Checksum (calculated by ENET module)
    ORIGIP1, ORIGIP2, ORIGIP3, ORIGIP4, // Origin IP
    DESTIP1, DESTIP2, DESTIP3, DESTIP4, // Destination IP
    /* End of IPV4 frame */
    /* Start ICMP frame */
    0x00, 0x00, // ICMP echo reply
    0x00, 0x00, // Header checksum
    0x00, 0x00, // Identifier
    0x00, 0x00, // Sequence number
    /* ICMP data goes here */
    /* End of UDP frame */
    0x00, 0x00, 0x00, 0x00 // Ethernet frame checksum
                           /* End of Ethernet frame */
};
//}

/**********************************************************************************************
 * Local types
 **********************************************************************************************/

/**********************************************************************************************
 * Local function prototypes
 *********************************************************************************************/
void ENET_Rx_Handler(void);

void ENET_ARP_Handler(ARP_Message *);
void ENET_IPv4_Handler(IPv4_Message *msg);
void ENET_ICMP_Handler(ICMP_Message *msg);

void markRxBDEmpty(eNBUF *used);
eNBUF *getActiveRxBD(void);

/**********************************************************************************************
 * Local variables
 **********************************************************************************************/

/**********************************************************************************************
 * Local functions
 **********************************************************************************************/
void ENET_GPIO_Init(void) {
  /* MDIO */
  SIUL2.MSCR[PF14].B.SSS = 4;
  SIUL2.MSCR[PF14].B.OBE = 1;
  SIUL2.MSCR[PF14].B.SRC = 3;
  SIUL2.MSCR[PF14].B.IBE = 1;
  SIUL2.IMCR[450].B.SSS = 1;
  // Input multiplexing?

  /* MDC */
  SIUL2.MSCR[PG0].B.SSS = 3;
  SIUL2.MSCR[PG0].B.OBE = 1;
  SIUL2.MSCR[PG0].B.SRC = 3;
  SIUL2.MSCR[PG0].B.PUE = 1;

  /* TX_EN */
  SIUL2.MSCR[PH2].B.SSS = 4;
  SIUL2.MSCR[PH2].B.OBE = 1;
  SIUL2.MSCR[PH2].B.SRC = 3;

  /* TDXD0 */
  SIUL2.MSCR[PH1].B.SSS = 4;
  SIUL2.MSCR[PH1].B.OBE = 1;
  SIUL2.MSCR[PH1].B.SRC = 3;

  /* TDXD1 */
  SIUL2.MSCR[PH0].B.SSS = 3;
  SIUL2.MSCR[PH0].B.OBE = 1;
  SIUL2.MSCR[PH0].B.SRC = 3;

  /* TDXD2 */
  //    SIUL2.MSCR[PG12].B.SSS = 4;
  //    SIUL2.MSCR[PG12].B.OBE = 1;
  //    SIUL2.MSCR[PG12].B.SRC = 3;

  /* TDXD3 */
  //    SIUL2.MSCR[PG13].B.SSS = 4;
  //    SIUL2.MSCR[PG13].B.OBE = 1;
  //    SIUL2.MSCR[PG13].B.SRC = 3;

  /* COL */
  /*
      SIUL2.MSCR[PA10].B.SSS = 0;
  SIUL2.MSCR[PA10].B.IBE = 1;
  SIUL2.MSCR[PA10].B.PUE = 1;
  SIUL2.IMCR[456].B.SSS = 1;
      */

  /* TX_CLK */
  SIUL2.MSCR[PG1].B.SSS = 0;
  SIUL2.MSCR[PG1].B.IBE = 1;
  SIUL2.MSCR[PG1].B.PUE = 1;
  SIUL2.IMCR[449].B.SSS = 1;

  /* CRS */
  //	/*
  //    SIUL2.MSCR[PE12].B.SSS = 0;
  //    SIUL2.MSCR[PE12].B.IBE = 1;
  //    SIUL2.MSCR[PE12].B.PUE = 1;
  //    SIUL2.IMCR[458].B.SSS = 1;
  //	*/

  /* RX_CLK */
  //    SIUL2.MSCR[PA3].B.SSS = 0;
  //    SIUL2.MSCR[PA3].B.IBE = 1;
  //    SIUL2.MSCR[PA3].B.PUE = 1;
  //    SIUL2.IMCR[448].B.SSS = 1;

  /* RX_DV */
  SIUL2.MSCR[PF15].B.SSS = 0;
  SIUL2.MSCR[PF15].B.IBE = 1;
  SIUL2.MSCR[PF15].B.PUE = 1;
  SIUL2.IMCR[457].B.SSS = 1;

  /* RXD0 */
  SIUL2.MSCR[PA9].B.SSS = 0;
  SIUL2.MSCR[PA9].B.IBE = 1;
  SIUL2.MSCR[PA9].B.PUE = 1;
  SIUL2.IMCR[451].B.SSS = 1;

  /* RXD1 */
  SIUL2.MSCR[PA8].B.SSS = 0;
  SIUL2.MSCR[PA8].B.IBE = 1;
  SIUL2.MSCR[PA8].B.PUE = 1;
  SIUL2.IMCR[452].B.SSS = 1;

  /* RXD2 */
  //    SIUL2.MSCR[PA7].B.SSS = 0;
  //    SIUL2.MSCR[PA7].B.IBE = 1;
  //    SIUL2.MSCR[PA7].B.PUE = 1;
  //    SIUL2.IMCR[453].B.SSS = 1;

  /* PE13 RXD3 */
  //    SIUL2.MSCR[PE13].B.SSS = 0;
  //    SIUL2.MSCR[PE13].B.IBE = 1;
  //    SIUL2.MSCR[PE13].B.PUE = 1;
  //    SIUL2.IMCR[454].B.SSS = 1;

  /* RX_ER */
  //	/*
  SIUL2.MSCR[PA11].B.SSS = 0;
  SIUL2.MSCR[PA11].B.IBE = 1;
  SIUL2.MSCR[PA11].B.PUE = 1;
  SIUL2.IMCR[455].B.SSS = 1;
  //	*/

  /* TX_ER */
  /*
SIUL2.MSCR[115].B.SSS = 3;
SIUL2.MSCR[115].B.OBE = 1;
SIUL2.MSCR[115].B.SRC = 3;
  */
}

void ENET_BUFFER_Init(void) {
  uint8_t j = 0;

  /* Initialize receive descriptor ring */
  for (j = 0; j < NUM_RXBDS; j++) {
    rxbd[j].status0 = RX_BD_S0_E; // Mark buffer as empty
    rxbd[j].length = 0;
    rxbd[j].status8 = RX_BD_S8_INT; // Enable transmit and receive flags
    rxbd[j].data = &RxBuffer[j][0]; // Set address of this data Buffer

    rxbd[j].statusA = 0;
    rxbd[j].statusC = 0;
    rxbd[j].statusE = 0;
    rxbd[j].status10 = 0;
    rxbd[j].timestamp = 0;
  }
  rxbd[NUM_RXBDS - 1].status0 |=
      RX_BD_S0_W; // Set the Wrap Bit on the last BD of each queue

  /* Initialize transmit descriptor ring only for Queue0 */
  for (j = 0; j < NUM_TXBDS; j++) {
    txbd[j].status0 =
        TX_BD_S0_L |
        TX_BD_S0_TC; // Mark buffer as the last one and transmit CRC
    txbd[j].status8 =
        TX_BD_S8_PINS | TX_BD_S8_IINS |
        TX_BD_S8_INT; // Add Protocol header checksum and IP header checksum
    txbd[j].length = 0;
    txbd[j].data = &TxBuffer[j][0]; // Set address of this data Buffer
    txbd[j].status8 |= 0; // FTYPE 0=non-avb, 1=avb class a, 2=avb class b

    txbd[j].statusA = 0;
    txbd[j].statusC = 0;
    txbd[j].statusE = 0;
    txbd[j].status10 = 0;
    txbd[j].timestamp = 0;
  }
  txbd[NUM_TXBDS - 1].status0 |=
      TX_BD_S0_W; // Set the Wrap Bit on the last BD of each queue
}

void ENET_Rx_Handler() {
  eNBUF *rxBD;
  EthernetFrame *rxFrame;

  // Get the active BD and increment pointer
  rxBD = getActiveRxBD();

  // If the data buffer has not been filled do nothing
  if ((rxBD->status0 & RX_BD_S0_E) == RX_BD_S0_E)
    return;

  // Extract the frame
  rxFrame = (EthernetFrame *)rxBD->data;

  switch (rxFrame->length) {
  case VLAN:
    counter.vlan++;
    break;
  case IPv4:
    counter.ipv4++;
    ENET_IPv4_Handler((IPv4_Message *)(&(rxFrame->length) + 1)); // ERROR
    break;
  case ARP:
    counter.arp++;
    ENET_ARP_Handler((ARP_Message *)(&(rxFrame->length) + 1)); // ERROR
    break;
  case IPv6:
    counter.ipv6++;
    break;
  }

  // Mark the receive buffer empty
  markRxBDEmpty(rxBD);

  // Flag that descriptors are available to allow reception
  ENET.RDAR.R = 0x0100000;

  // Clear interrupt
  ENET.EIR.R = 0x02000000;

  // To check if receiver is working or not....
  // If receiver handler exists successfully then LED will toggle at each
  // receive event
  static int flag = 0;
  if (flag == 0) {
    SIUL2.MSCR[PA10].B.OBE = 1;
    flag = 1;
  } else {
    SIUL2.MSCR[PA10].B.OBE = 0;
    flag = 0;
  }

  return;
}

void ENET_IPv4_Handler(IPv4_Message *msg) {
  // Ensure we have an IPv4 datagram with no options field
  if (msg->ver_ihl != 0x45)
    return;

  if (msg->prot == ICMP)
    ENET_ICMP_Handler((ICMP_Message *)(&(msg->dest) + 1));

  return;
}

void ENET_ICMP_Handler(ICMP_Message *msg) {
  if (msg->type == 8 && msg->code == 0) {
  };
  if (msg->type == 0 && msg->code == 0) {
  };
}

void ENET_ARP_Handler(ARP_Message *msg) {
  // Make sure the MAC and IP addresses are correct length
  if ((msg->hlen == 6) && (msg->plen == 4)) {
    // ARP request
    if (msg->op == 1) {
      // Check the target IP address for match on local
      if (*(uint32_t *)msg->tpa == *(uint32_t *)local_IP) {
        ENET_frame_Tx(ARP_packet, ARP_PACKET_LEN, ARP, ARP_packet, 0);
#if 0
                {
                uint32_t i;
                /**
                START OF PACKET Tx CODE
                **/
                // Copy the packet into the data buffer - don't include Ethernet checksum
                for(i = 0; i < ARP_PACKET_LEN; i++){
                    TxBuffer[nextTxBD][i] = ARP_packet[i];
                }
                
                    txbd[nextTxBD].status8 = //TX_BD_S8_PINS |
                                             //TX_BD_S8_IINS |
                                             TX_BD_S8_INT;
                    txbd[nextTxBD].data = &TxBuffer[nextTxBD][0];  // Set address of this data Buffer
                    txbd[nextTxBD].status10 = 0;                 // Clear status 10
                    
                    /* Set the length of the packet */
                    txbd[nextTxBD].length = ARP_PACKET_LEN;
                    
                    /* Mark packet as ready to send */   
                    txbd[nextTxBD].status0 = TX_BD_S0_L | TX_BD_S0_TC; 	// Mark buffer as the last one and transmit CRC    
                    txbd[NUM_TXBDS-1].status0 |= TX_BD_S0_W;     		// Set the Wrap Bit on the last BD of each queue	
                    txbd[nextTxBD].status0 |= TX_BD_S0_R;					// Mark buffer as ready
                    
                    ENET.TDAR.B.TDAR = 1;  // Start transmission

                    nextTxBD++;   // Next frame will be processed on the next buffer descriptor
                    if(nextTxBD >= NUM_TXBDS)  // Wrap if reached last buffer descriptor
                    {
                        nextTxBD = 0;
                    }
                /**
                END OF PACKET Tx CODE
                **/
                }
#endif
      }
    }
  }
}

/* Returns the active Rx buffer descriptor. Increments the pointer */
eNBUF *getActiveRxBD() {
  eNBUF *ret = &rxbd[nextRxBD];

  // If current Rx bd is the last, wrap around
  nextRxBD++;
  if (nextRxBD >= NUM_RXBDS) // Wrap if reached last buffer descriptor
    nextRxBD = 0;

  return ret;
}

/* Mark specified Rx descriptor as empty */
void markRxBDEmpty(eNBUF *used) {
  if (used ==
      &rxbd[NUM_RXBDS - 1]) // If it is the last buffer descriptor in the ring
  {
    used->status0 = RX_BD_S0_W | RX_BD_S0_E; // Wrap and Empty
    used->length = 0;
  } else {
    used->status0 = RX_BD_S0_E; // Empty
    used->length = 0;
  }
}

/**********************************************************************************************
 * Global functions
 **********************************************************************************************/

/***********************************************************************************************
 *
 * @brief    ENET_UDP_Driver_Init - ENET module, structures, descriptors and
 *pins initialization routine
 * @param    none
 * @return   none
 *
 ************************************************************************************************/
void ENET_UDP_Driver_Init(void) {
  ENET_GPIO_Init();   // Initialize PADs
  ENET_BUFFER_Init(); // Initialize buffer descriptors and data buffers

  /* reset the ENET - clears EIR, etc. */
  ENET.ECR.B.RESET = 1;

  ENET.ECR.B.EN1588 = 1; /* Enable enhanced functionalities */

  /* Set the source address for the controller */
  ENET.PALR.R = ((ORIGMAC1 << 24) + (ORIGMAC2 << 16) + (ORIGMAC3 << 8) +
                 ORIGMAC4); // set to match DA in packet
  ENET.PAUR.R = ((ORIGMAC5 << 24) + (ORIGMAC6 << 16));

  // initially no hash matching
  ENET.IALR.R = 0x00000000; // no individual hash matching
  ENET.IAUR.R = 0x00000000;
  ENET.GALR.R = 0x00000000; // no group hash matching
  ENET.GAUR.R = 0x00000000;

  // Set Receive Buffer Size
  ENET.MRBR.R = (uint16_t)RX_BUFFER_SIZE;
  // Point to the start of the circular Rx buffer descriptor queue
  ENET.RDSR.R = (uint32_t)&rxbd[0];
  // Point to the start of the circular Tx buffer descriptor queue
  ENET.TDSR.R = (uint32_t)&txbd[0];

  ENET.RCR.B.PADEN = 1; /* Enable Padding removing when receiving frames */

  /* Set the transceiver interface to MII mode */
  ENET.RCR.B.MAX_FL = 1518; // MAX_FL = 1500 payload + 18(for DA(6), SA(6),
                            // Ethertype(2) and CRC(4))
  ENET.RCR.B.MII_MODE =
      1; // mode enabled and indicated by ENETx_RCR[RMII_MODE] - 18 signals;
  ENET.RCR.B.RMII_MODE = 1; // Enabled the RMII mode
  // ENET.RCR.B.RMII_10T = 1;	// 10Mb/s

  /* LOOPBACK */
  ENET.RCR.B.LOOP = 0; /* Disable loopback mode */

  /* Operate in full-duplex mode */
  ENET.TCR.R = 0x0004;

  /* Set MII speed to be 50/((MSCR+1)*2) = 2.5Mhz */
  ENET.MSCR.R = 0x9;

  /* Enable FEC */
  ENET.ECR.B.ETHEREN = 1;
}

/*
Initialise the receive path
Install the interrupt handler and enable interrupts on Rx frame
*/
void ENET_Driver_Rx_Init() {
  // Install the receive frame interrupt handler
  //    INTC_InstallINTCInterruptHandler(ENET_Rx_Handler, 218, CPU0, 1);
  INTC.PSR[211].B.PRC_SELN = 0x8; /* IRQ sent to Core 0 */
  INTC.PSR[211].B.PRIN = 10;      /* IRQ priority = 10 (15 = highest) */

  // Enable the generation of Rx frame interrupts
  ENET.EIMR.B.RXF = 1;

  // Indicate Empty buffers have been produced
  ENET.RDAR.B.RDAR = 1;
}

void ENET_UDP_Tx(uint8_t *dataBuffer, uint16_t len) {
  ENET_frame_Tx(UDP_packet, PACKET_LEN_BEFORE_UDP_PAYLOAD, IPv4, dataBuffer,
                len);
}

/* Perpare a Tx buffer for frame transmission
pkt      - pointer to packet structure to be transmitted
head_len - number of bytes in this packet
type     - layer 3 type of the packet (ARP, IPv4)
data     - pointer to the data buffer that will be attached as payload for IPv4
packets (can be null) data_len - number of bytes in the data buffer (can be 0)

*/
void ENET_frame_Tx(uint8_t *pkt, uint16_t head_len, uint16_t type,
                   uint8_t *data, uint16_t data_len) {
  uint32_t i;

  /* Copy the indicated packet into the data buffer
  Includes Ethernet header + layer 3 (which can contain a transport protocol
  header)
  */
  for (i = 0; i < head_len; i++) {
    TxBuffer[nextTxBD][i] = pkt[i];
  }

  txbd[nextTxBD].status8 = TX_BD_S8_INT;
  txbd[nextTxBD].data =
      &TxBuffer[nextTxBD][0]; // Set address of this data Buffer
  txbd[nextTxBD].status10 = 0;

  /* Set the length of the packet */
  txbd[nextTxBD].length = head_len + data_len;

  /* IPv4 Tx */
  if (type == IPv4) {
    /* Copy the payload of this frame */
    for (i = 0; i < data_len; i++) {
      TxBuffer[nextTxBD][head_len + i] = (uint8_t) * (data + i);
    }

    /* Change frame sizes on the packet before transmitting */
    TxBuffer[nextTxBD][16] =
        ((head_len - ETH_HEADER_LEN + data_len + 4) >> 8) &
        0xFF; /* Set the IPV4 frame length on the packet, high byte */
    TxBuffer[nextTxBD][17] =
        (head_len - ETH_HEADER_LEN + data_len + 4) &
        0xFF; /* Set the IPV4 frame length on the packet, low byte */

    // Check the IPv4 protocol field
    switch (TxBuffer[nextTxBD][23]) {
    case ICMP:

    case UDP:
      TxBuffer[nextTxBD][38] =
          ((UDP_HEADER_SIZE + data_len) >> 8) &
          0xFF; /* Set the UDP frame length on the packet, high byte */
      TxBuffer[nextTxBD][39] =
          (UDP_HEADER_SIZE + data_len) &
          0xFF; /* Set the UDP frame length on the packet, low byte */
      break;
    case TCP:
      break;
    default:
      break;
    }

    txbd[nextTxBD].status8 |=
        TX_BD_S8_IINS |
        TX_BD_S8_INT; // Add Protocol header checksum and IP header checksum

  }
  /* ARP Tx */
  else if (type == ARP) {
    // Clear status 10
  }

  /* Mark packet as ready to send */
  txbd[nextTxBD].status0 =
      TX_BD_S0_L | TX_BD_S0_TC; // Mark buffer as the last one and transmit CRC
  txbd[NUM_TXBDS - 1].status0 |=
      TX_BD_S0_W; // Set the Wrap Bit on the last BD of each queue
  txbd[nextTxBD].status0 |= TX_BD_S0_R; // Mark buffer as ready

  ENET.TDAR.B.TDAR = 1; // Start transmission

  nextTxBD++; // Next frame will be processed on the next buffer descriptor
  if (nextTxBD >= NUM_TXBDS) // Wrap if reached last buffer descriptor
  {
    nextTxBD = 0;
  }
}

#if 0 // ENET_UDP_Driver_FrameTX(uint8_t* dataBuffer, uint16_t len)
void ENET_UDP_Driver_FrameTX(uint8_t* dataBuffer, uint16_t len)
{
    uint16_t i = 0;
    uint16_t frameLen = PACKET_LEN_BEFORE_UDP_PAYLOAD + len;  // Doesn't need CRC as it is added by MAC //42 + len
	uint16_t UDPFrameLen = UDP_HEADER_SIZE + len;   // 8 + len
	uint16_t IPV4FrameLen = IPV4_HEADER_SIZE + UDPFrameLen + 4;  // +4 for checksum (appended by MAC)
	
    /* Copy the packet up to before UDP payload. Will not include checksum */
    for(i = 0; i < PACKET_LEN_BEFORE_UDP_PAYLOAD; i++)
    {
        TxBuffer[nextTxBD][i] = UDP_packet[i];
    }
    /* Copy the payload of this frame */
        for(i = 0; i < len; i++)
    {
        TxBuffer[nextTxBD][PACKET_LEN_BEFORE_UDP_PAYLOAD + i] = (uint8_t)*(dataBuffer + i);
    }
	
	/* Change frame sizes on the packet before transmitting */
	TxBuffer[nextTxBD][16] = (IPV4FrameLen >> 8) & 0xFF;			/* Set the IPV4 frame length on the packet, high byte */
	TxBuffer[nextTxBD][17] = (IPV4FrameLen) & 0xFF;				/* Set the IPV4 frame length on the packet, low byte */
	TxBuffer[nextTxBD][38] = (UDPFrameLen >> 8) & 0xFF;			/* Set the UDP frame length on the packet, high byte */
	TxBuffer[nextTxBD][39] = (UDPFrameLen) & 0xFF;				/* Set the UDP frame length on the packet, low byte */	
	
    txbd[nextTxBD].status8 = TX_BD_S8_PINS |
                           TX_BD_S8_IINS |
                           TX_BD_S8_INT;       // Add Protocol header checksum and IP header checksum
    txbd[nextTxBD].data = &TxBuffer[nextTxBD][0];  // Set address of this data Buffer
    txbd[nextTxBD].status10 = 0;                 // Clear status 10

    /* Set the length of the packet */
    txbd[nextTxBD].length = frameLen;

	/* Mark packet as ready to send */   
	txbd[nextTxBD].status0 = TX_BD_S0_L | TX_BD_S0_TC; 	// Mark buffer as the last one and transmit CRC    
    txbd[NUM_TXBDS-1].status0 |= TX_BD_S0_W;     		// Set the Wrap Bit on the last BD of each queue	
    txbd[nextTxBD].status0 |= TX_BD_S0_R;					// Mark buffer as ready
	
	ENET.TDAR.B.TDAR = 1;  // Start transmission

    nextTxBD++;   // Next frame will be processed on the next buffer descriptor
    if(nextTxBD >= NUM_TXBDS)  // Wrap if reached last buffer descriptor
    {
        nextTxBD = 0;
    }
}
#endif
