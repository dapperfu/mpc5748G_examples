/*
 * Copyright (c) 2015 - 2016 , Freescale Semiconductor, Inc.
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

#include "edmaTransfer.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

extern volatile bool transferComplete;

/* configuration structures for simple loop transfers */
edma_loop_transfer_config_t loopConfig = {
    .majorLoopIterationCount = MAJOR_LOOP_COUNT,
    .srcOffsetEnable = false,
    .dstOffsetEnable = false,
    .minorLoopOffset = 0,
    .minorLoopChnLinkEnable = false,
    .minorLoopChnLinkNumber = 0,
    .majorLoopChnLinkEnable = false,
    .majorLoopChnLinkNumber = 0
};

edma_transfer_config_t transferConfig = {
    .srcAddr = 0,
    .destAddr = 0,
    .srcTransferSize = EDMA_TRANSFER_SIZE_1B,
    .destTransferSize = EDMA_TRANSFER_SIZE_1B,
    .srcOffset = 1,
    .destOffset = 1,
    .srcLastAddrAdjust = 0,
    .destLastAddrAdjust = 0,
    .srcModulo = EDMA_MODULO_OFF,
    .destModulo = EDMA_MODULO_OFF,
    .minorByteTransferCount = 0,
    .scatterGatherEnable = false,
    .scatterGatherNextDescAddr = 0,
    .interruptEnable = true,
    .loopTransferConfig = &loopConfig
};

/* This function triggers a single block memory-to-memory transfer. */
void triggerSingleBlock(uint8_t channel, uint8_t * srcBuff, uint8_t * dstBuff, uint32_t size)
{
    transferComplete = false;

    /* configure eDMA channel for a single block mem-to-mem transfer */
    EDMA_DRV_ConfigSingleBlockTransfer(channel, EDMA_TRANSFER_MEM2MEM, (uint32_t)srcBuff,
                                           (uint32_t)dstBuff, EDMA_TRANSFER_SIZE_1B, size);

    /* trigger a sw DMA request for this channel */
    EDMA_DRV_TriggerSwRequest(channel);

    /* wait for the channel to complete */
    while(!transferComplete) {}
}

/* This function triggers a loop memory-to-memory transfer. */
void triggerLoopTransfer(uint8_t channel, uint8_t * srcBuff, uint8_t * dstBuff, uint32_t size)
{
    transferComplete = false;

    /* configure transfer source and destination addresses */
    transferConfig.srcAddr = (uint32_t)srcBuff;
    transferConfig.destAddr = (uint32_t)dstBuff;
    transferConfig.minorByteTransferCount = size / MAJOR_LOOP_COUNT;

    /* configure the eDMA channel for a loop transfer (via transfer configuration structure */
    EDMA_DRV_ConfigLoopTransfer(channel, &transferConfig);

    /* select the always enabled hw request */
    EDMA_DRV_SetChannelRequest(channel, EDMA_REQ_MUX0_ALWAYS_ENABLED_0);
    /* disable the channel once the transfer is completed */
    EDMA_DRV_DisableRequestsOnTransferComplete(channel, true);
    /* start the channel */
    EDMA_DRV_StartChannel(channel);

    /* wait for the transfer to complete */
    while(!transferComplete) {}

    /* stop the channel */
    EDMA_DRV_StopChannel(channel);
    /* restore the default request (none) */
    EDMA_DRV_SetChannelRequest(channel, EDMA_REQ_DISABLED_MUX0);
}

/* This function triggers a scatter/gather memory-to-memory transfer. */
void triggerScatterGather(uint8_t channel, uint8_t * srcBuff, uint8_t * dstBuff, uint32_t size)
{
    uint8_t i;
    /* compute the transfer size for the scatter/gather transfers */
    uint32_t transferSize = size / SG_TCD_COUNT;

    /* configure the list of scatter/gather configuration structures */
    for(i = 0U; i < SG_TCD_COUNT; i++)
    {
        srcList[i].address = (uint32_t)srcBuff + i * transferSize;
        dstList[i].address = (uint32_t)dstBuff + i * transferSize;
        srcList[i].length = dstList[i].length = transferSize;
        srcList[i].type = dstList[i].type = EDMA_TRANSFER_MEM2MEM;
    }

    /* allocate a list of software TCD structures and pass it to the driver;
     * this buffer needs to contain 32 extra bytes, as the TCD structures must be 32-bytes aligned;
     */
    edma_software_tcd_t stcd[5];

    /* configure the channel for a scatter/gather list of transfers */
    EDMA_DRV_ConfigScatterGatherTransfer(channel, stcd, EDMA_TRANSFER_SIZE_2B, 12U, srcList, dstList, SG_TCD_COUNT);

    /* for each TCD in the scatter/gather list */
    for(i = 0U; i < SG_TCD_COUNT; i++)
    {
        transferComplete = false;

        /* trigger sw DMA requests for this channel */
        EDMA_DRV_TriggerSwRequest(channel);

        /* wait for the channel to complete */
        while(!transferComplete) {}
    }
}
