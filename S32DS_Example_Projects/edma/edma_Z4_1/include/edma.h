
#ifndef _EMDA_H
#define _EDMA_H

#include "project.h"

void initTCDs(void);
void init_edma_channel_arbitration(void);

extern uint8_t TCD0_Destination[]  __attribute__((aligned(16)));

#endif /* _EDMA_H */
