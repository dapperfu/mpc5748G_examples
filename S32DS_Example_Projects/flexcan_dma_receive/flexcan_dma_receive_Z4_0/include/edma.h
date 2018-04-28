
#ifndef _EMDA_H
#define _EDMA_H

#include "project.h"

#define DMA_CHANNEL 0
#define CAN0_BASE_ADDRESS ((unsigned int)&CAN_0)

void AIPS_Init(void);
void DMA_Init(void);
void DMA_16_ISR(void);

#endif /* _EDMA_H */
