
#ifndef _EMDA_H
#define _EDMA_H

#include "project.h"

#define NUMBER_OF_BYTES 256

void init_dma_mux(void);
void init_edma_tcd_0(void);
void init_edma_tcd_16(void);
void init_edma_channel_arbitration(void);

extern const unsigned int  TransmitBuffer[];
extern       unsigned char ReceiveBuffer[];

#endif /* _EDMA_H */
