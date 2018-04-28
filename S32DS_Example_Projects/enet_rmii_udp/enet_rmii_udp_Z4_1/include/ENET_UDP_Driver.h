/*
 * ENET_UDP_DRIVER.c
 *
 *  Created on: Oct 11, 2014
 *      Author: B34981
 *
 *  Modified: April, 2015
 *      Author: B46845
 */

#ifndef ENET_UDP_DRIVER_H_
#define ENET_UDP_DRIVER_H_

/**********************************************************************************************
* Includes
**********************************************************************************************/
#include "project.h"
#include "derivative.h"
#include "ENET_Config.h"

/**********************************************************************************************
* Constants
**********************************************************************************************/


/**********************************************************************************************
* Macros
**********************************************************************************************/
//{
/*
 * Legacy Bit level Buffer Descriptor definitions
 */
#define TX_BD_R         0x8000
#define TX_BD_INUSE     0x4000
#define TX_BD_TO1       0x4000
#define TX_BD_W         0x2000
#define TX_BD_TO2       0x1000
#define TX_BD_L         0x0800
#define TX_BD_TC        0x0400
#define TX_BD_DEF       0x0200
#define TX_BD_HB        0x0100
#define TX_BD_LC        0x0080
#define TX_BD_RL        0x0040
#define TX_BD_UN        0x0002
#define TX_BD_CSL       0x0001

#define RX_BD_E         0x8000
#define RX_BD_INUSE     0x4000
#define RX_BD_R01       0x4000
#define RX_BD_W         0x2000
#define RX_BD_R02       0x1000
#define RX_BD_L         0x0800
#define RX_BD_M         0x0100
#define RX_BD_BC        0x0080
#define RX_BD_MC        0x0040
#define RX_BD_LG        0x0020
#define RX_BD_NO        0x0010
#define RX_BD_SH        0x0008
#define RX_BD_CR        0x0004
#define RX_BD_OV        0x0002
#define RX_BD_TR        0x0001

/* Enhanced TX Buffer Descriptor Bit Level Definitions - Status0 */
#define TX_BD_S0_R              0x8000
#define TX_BD_S0_TO1            0x4000
#define TX_BD_S0_W              0x2000
#define TX_BD_S0_TO2            0x1000
#define TX_BD_S0_L              0x0800
#define TX_BD_S0_TC             0x0400

/* Enhanced TX Buffer Descriptor Bit Level Definitions - Status8 */
#define TX_BD_S8_INT            0x4000
#define TX_BD_S8_TS             0x2000
#define TX_BD_S8_PINS           0x1000
#define TX_BD_S8_IINS           0x0800
#define TX_BD_S8_FTYPE          0x00F0

/* Enhanced TX Buffer Descriptor Bit Level Definitions - StatusA */
#define TX_BD_SA_TXE            0x8000
#define TX_BD_SA_UE             0x2000
#define TX_BD_SA_EE             0x1000
#define TX_BD_SA_FE             0x0800
#define TX_BD_SA_LCE            0x0400
#define TX_BD_SA_OE             0x0200
#define TX_BD_SA_TSE            0x0100
/* Enhanced TX Buffer Descriptor Bit Level Definitions - Status10 */
#define TX_BD_S10_BDU           0x8000


/* Enhanced RX Buffer Descriptor Bit Level Definitions - Status0 */
#define RX_BD_S0_E              0x8000
#define RX_BD_S0_RO1            0x4000
#define RX_BD_S0_W              0x2000
#define RX_BD_S0_RO2            0x1000
#define RX_BD_S0_L              0x0800
#define RX_BD_S0_M              0x0100
#define RX_BD_S0_BC             0x0080
#define RX_BD_S0_MC             0x0040
#define RX_BD_S0_LG             0x0020
#define RX_BD_S0_NO             0x0010
#define RX_BD_S0_CR             0x0004
#define RX_BD_S0_OV             0x0002
#define RX_BD_S0_TR             0x0001

/* Enhanced RX Buffer Descriptor Bit Level Definitions - Status8 */
#define RX_BD_S8_ME             0x8000
#define RX_BD_S8_PE             0x0400
#define RX_BD_S8_CE             0x0200
#define RX_BD_S8_UC             0x0100
#define RX_BD_S8_INT            0x0080

/* Enhanced RX Buffer Descriptor Bit Level Definitions - StatusA */
#define RX_BD_SA_VPCP           0xE000
#define RX_BD_SA_ICE            0x0020
#define RX_BD_SA_PCR            0x0010
#define RX_BD_SA_VLAN           0x0004
#define RX_BD_SA_IPV6           0x0002
#define RX_BD_SA_FRAG           0x0001

/* Enhanced RX Buffer Descriptor Bit Level Definitions - StatusC */
#define RX_BD_SC_HDR_LEN        0xF800
#define RX_BD_SC_PROT_TYPE      0x00FF

/* Enhanced RX Buffer Descriptor Bit Level Definitions - Status10 */
#define RX_BD_S10_BDU           0x8000

/* Bit definitions and macros for MCF_FEC_RCR */
#define MPC_FEC_RCR_LOOP              (0x00000001)
#define MPC_FEC_RCR_DRT               (0x00000002)
#define MPC_FEC_RCR_MII_MODE          (0x00000004)
#define MPC_FEC_RCR_PROM              (0x00000008)
#define MPC_FEC_RCR_BC_REJ            (0x00000010)
#define MPC_FEC_RCR_FCE               (0x00000020)
#define MPC_FEC_RCR_MAX_FL(x)         (((x)&0x000007FF)<<16)


/* Code Definitions */
#define NULL    0
#define FAIL    0
#define OK  1
#define FALSE   0
#define TRUE    1
#define TRUE_REJ 2
#define BROADCAST 2

// Ethernet type values
#define VLAN 0x8100
#define IPv4 0x0800
#define ARP 0x0806
#define IPv6 0x86DD

// IPv4 protocol type values
#define ICMP 0x01
#define TCP 0x06
#define UDP 0x11


//}

/**********************************************************************************************
* Types
**********************************************************************************************/

typedef struct
{
    uint8_t 	destination[6];     /* Destination MAC */
    uint8_t 	source[6];     		/* Source MAC */
    uint16_t	length;				/* Data length/type */
    uint8_t     data[4];            /* Payload data */
    								/* CRC is written by hardware */
} EthernetFrame;

typedef struct
{
    uint16_t htype;     /* Hardware type */
    uint16_t ptype;     /* Protocol type */
    uint8_t hlen;      /* Hardware length */
    uint8_t plen;      /* Protocol length */
    uint16_t op;        /* Opcode */
    /* Assumes 6 byte MAC addresses, 4 byte IPv4 addresses */
    uint8_t sha[6];     /* Sender hardware address */
    uint8_t spa[4];     /* Send protocol address */
    uint8_t tha[6];     /* Target hardware address */
    uint8_t tpa[4];     /* Target protocol address */
    
} ARP_Message;

typedef struct
{
    uint8_t ver_ihl;        /* Version & header length */    
    uint8_t tos;            /* Type of service */
    uint16_t len;           /* Packet length (header + payload) */
    uint16_t fid;           /* Fragment ID */
    uint16_t flags_foffset; /* Flags & fragment offse */
    uint8_t ttl;            /* Time-to-live */
    uint8_t prot;           /* Protocol ID */
    uint16_t chksm;         /* IP header checksum */
    uint8_t src[4];           /* Source IP address */
    uint8_t dest[4];          /* Destination IP address */
    
} IPv4_Message;

typedef struct 
{
    uint8_t type;   /* Type information */   
    uint8_t code;   /* Code related to type */   
    uint16_t chksm; /* Checksum over datagram (header + payload) */   
    uint32_t msg;   /* Message data */   
    
} ICMP_Message;

/* Legacy Buffer Descriptor Format */
typedef struct
{
    uint16_t status;    /* control and status */
    uint16_t length;    /* transfer length */
    uint8_t  *data;     /* buffer address */
} NBUF;

/* Enhanced Buffer Descriptor Format */
typedef struct
{
    uint16_t status0;   /* control and status */
    uint16_t  length;   /* transfer length */
    uint8_t    *data;   /* buffer address */
    uint16_t status8;   /* control and status */
    uint16_t statusA;   /* control and status */
    uint16_t statusC;   /* control and status */
    uint16_t statusE;   /* control and status */
    uint16_t status10;  /* control and status */
    uint16_t dummy12;   /* control and status */
    uint32_t timestamp; /* control and status */
    uint16_t dummy18;   /* control and status */
    uint16_t dummy1A;   /* control and status */
    uint16_t dumym1C;   /* control and status */
    uint16_t dummy1E;   /* control and status */
} eNBUF;

typedef struct{
    uint16_t vlan;
    uint16_t ipv4;
    uint16_t arp;
    uint16_t ipv6;
} Rx_count;

/**********************************************************************************************
* Variables
**********************************************************************************************/


/**********************************************************************************************
* Global Variables
**********************************************************************************************/


/**********************************************************************************************
* Global Functions
**********************************************************************************************/

/***********************************************************************************************
*
* @brief    ENET_UDP_Driver_Init - ENET module, structures, descriptors and pins initialization routine
* @param    none
* @return   none
*
************************************************************************************************/
void ENET_UDP_Driver_Init(void);
void ENET_Driver_Rx_Init(void);

void ENET_UDP_Tx(uint8_t* dataBuffer, uint16_t len);
void ENET_frame_Tx(uint8_t *pkt, uint16_t head_len, uint16_t type, uint8_t *data, uint16_t data_len);



#endif /* ENET_UDP_DRIVER_H_ */
