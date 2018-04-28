/* edma.c - DMA example for CAN transfer */
/* Rev 1.0 09 Apr 2015 S Mihalik - Initial version */
/*****************************************************************************/

#include "project.h" 
#include "edma.h"

uint8_t RxDATA[16];	/* Array used to read the data received by CAN  */

/*****************************************************************************/

/*******************************************************************************
Function Name : AIPS_Init
Engineer      : b01802
Date          : Oct-15-2015
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : Enable DMA access on both Peripheral Bridges
Issues        : NONE
*******************************************************************************/
void AIPS_Init(void)
{
    /* Enable DMA access on Peripheral Bridges*/
    AIPS_A.MPRA.B.MTR4 = 1;
    AIPS_A.MPRA.B.MTW4 = 1;
    AIPS_A.MPRA.B.MPL4 = 1;

    AIPS_B.MPRA.B.MTR4 = 1;
    AIPS_B.MPRA.B.MTW4 = 1;
    AIPS_B.MPRA.B.MPL4 = 1;
}//AIPS_Init

/*******************************************************************************
Function Name : DMA_Init
Engineer      : b01802
Date          : Oct-15-2015
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : Configure DMA channel 16 to receive one frame from CAN1.
                Received data are placed to RxDATA array.
Issues        : NONE
*******************************************************************************/
void DMA_Init(void)
{
    DMAMUX_0.CHCFG[0].R = 0x80 | 5;	//Route FlexCAN1 to DMA CHANNEL 0

    EDMA.TCD[DMA_CHANNEL].SADDR.R = CAN0_BASE_ADDRESS + 0x80;	/* Load Address of Source data */
    EDMA.TCD[DMA_CHANNEL].ATTR.B.SSIZE = 2;	/* Read 2**0 = 1 byte per transfer */
    EDMA.TCD[DMA_CHANNEL].ATTR.B.SMOD = 0;	/* Source Modulo feature not used */
    EDMA.TCD[DMA_CHANNEL].SOFF.R = 4;		/* After tranfer, add 0 to src addr */
    EDMA.TCD[DMA_CHANNEL].SLAST.R = 0;	/* No address Adjustment after major loops */

    EDMA.TCD[DMA_CHANNEL].DADDR.R = (unsigned int)&RxDATA[0];	/* Destination Address */
    EDMA.TCD[DMA_CHANNEL].ATTR.B.DSIZE = 2;	/* Write 2**0 = 1 byte per transfer */
    EDMA.TCD[DMA_CHANNEL].ATTR.B.DMOD = 0;	/* Destination modulo feature not used  */
    EDMA.TCD[DMA_CHANNEL].DOFF.R = 4;	/* Increment destination addr by 1 */
    EDMA.TCD[DMA_CHANNEL].DLASTSGA.R = 0;	/* No addr adjustment after major loop */

    EDMA.TCD[DMA_CHANNEL].CSR.B.DREQ = 1;		/* Disable Request */
    EDMA.TCD[DMA_CHANNEL].NBYTES.MLNO.B.NBYTES = 16;	/* NBYTES - Transfer 1 byte per minor loop */
    /* TCD Current Minor Loop Link, Major Loop Count (Channel Linking Disabled) */
    EDMA.TCD[DMA_CHANNEL].CITER.ELINKNO.B.ELINK = 0;	/* Enable channel-to-channel linking on minor-loop complete */
    EDMA.TCD[DMA_CHANNEL].CITER.ELINKNO.B.CITER = 1;	/* Current Major Iteration Count */
    /* TCD Beginning Minor Loop Link, Major Loop Count (Channel Linking Disabled) */
    EDMA.TCD[DMA_CHANNEL].BITER.ELINKNO.B.ELINK = 0;	/* Enables channel-to-channel linking on minor loop complete */
    EDMA.TCD[DMA_CHANNEL].BITER.ELINKNO.B.BITER = 1;	/* Starting Major Iteration Count */
    EDMA.TCD[DMA_CHANNEL].CSR.B.MAJORELINK = 0;	/* Enable channel-to-channel linking on major loop complete */
    EDMA.TCD[DMA_CHANNEL].CSR.B.ESG = 0;		/* Enable Scatter/Gather Processing */
    EDMA.TCD[DMA_CHANNEL].CSR.B.BWC = 0;		/* Bandwidth Control */
    EDMA.TCD[DMA_CHANNEL].CSR.B.INTHALF = 0;	/* Disable an interrupt when major counter is half complete. */
    EDMA.TCD[DMA_CHANNEL].CSR.B.INTMAJOR = 1;	/* Disable an interrupt when major iteration count completes */
    EDMA.TCD[DMA_CHANNEL].CSR.B.MAJORLINKCH = 0;	/* Link Channel Number */
    EDMA.TCD[DMA_CHANNEL].CSR.B.START = 0;		/* Channel Start */
    EDMA.TCD[DMA_CHANNEL].CSR.B.DONE = 0;		/* Channel Done */
    EDMA.TCD[DMA_CHANNEL].CSR.B.ACTIVE = 0;		/* Channel Active */
    INTC.PSR[53].B.PRC_SELN = 0x8;  /* IRQ sent to Core 0 */
    INTC.PSR[53].B.PRIN =10;        /* IRQ priority = 10 (15 = highest) */

    EDMA.SERQ.R = 0;	/* Enable channel 0 */
}

/*******************************************************************************
Function Name : DMA_16_ISR
Engineer      : b01802
Date          : Oct-15-2015
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : Once frame with ID 0x555 is received on CAN1 and DMA channel 16
                moves the frame to RxDATA array, this interrupt is triggered.
Issues        : NONE
*******************************************************************************/
void DMA_0_ISR(void)
{
    /* Received frame is available in array RxDATA */

    EDMA.INT.R = 0x00000001;	/* Clear interrupt flag for DMA channel 0 */
}
