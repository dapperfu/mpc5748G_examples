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

#ifndef SOURCES_EDMATRANSFER_H_
#define SOURCES_EDMATRANSFER_H_

#include "dmaController1.h"
#include "device_registers.h"
#include "edma_driver.h"
#include <stdint.h>
#include <stdbool.h>

/* Major loop count for loop transfers */
#define MAJOR_LOOP_COUNT        6U
/* Number of descriptors used for scatter/gather */
#define SG_TCD_COUNT            4U
/* Timeout for blocking communication - 1s */
#define TIMEOUT                 1000U

/* Configuration structures for simple loop transfers */
extern edma_loop_transfer_config_t loopConfig;
extern edma_transfer_config_t transferConfig;

extern void edmaCallback(void *parameter, edma_chn_status_t status);

/* Scatter/gather list of descriptors */
edma_scatter_gather_list_t srcList[4];
edma_scatter_gather_list_t dstList[4];

/*!
 * @brief This function triggers a single block memory-to-memory transfer.
 *
 * @param chnState Pointer to the channel state structure.
 * @param srcBuff Source buffer.
 * @param dstBuff Destination buffer.
 * @param size Transfer size.
 */
void triggerSingleBlock(uint8_t channel, uint8_t * srcBuff, uint8_t * dstBuff, uint32_t size);

/*!
 * @brief This function triggers a loop memory-to-memory transfer.
 * The loop transfer configuration is defined in 'transferConfig' structure declared
 * above.
 *
 * @param chnState Pointer to the channel state structure.
 * @param srcBuff Source buffer.
 * @param dstBuff Destination buffer.
 * @param size Transfer size.
 */
void triggerLoopTransfer(uint8_t channel, uint8_t * srcBuff, uint8_t * dstBuff, uint32_t size);

/*!
 * @brief This function triggers a scatter/gather memory-to-memory transfer.
 * The scatter/gather list of descriptors is configured by 'srcList' and 'dstList' arrays
 * declared above.
 *
 * @param chnState Pointer to the channel state structure.
 * @param srcBuff Source buffer.
 * @param dstBuff Destination buffer.
 * @param size Transfer size.
 */
void triggerScatterGather(uint8_t channel, uint8_t * srcBuff, uint8_t * dstBuff, uint32_t size);

#endif /* SOURCES_EDMATRANSFER_H_ */
