
#ifndef _EMDA_H
#define _EDMA_H

#include "project.h"

void initTCDs(void);
void init_edma_channel_arbitration(void);

#pragma alignvar (16)
extern uint8_t TCD0_Destination[];

#endif /* _EDMA_H */
