/*
 * Copyright 2017 NXP
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
/* ###################################################################
**     Filename    : ringBuffer.c
**     Project     : ssdhc_fatfs_mpc5748g
**     Processor   : MPC5748G_324
**     Version     : 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   :
**     Abstract    :
**         This module contains ring buffer.
**     Settings    :
**     Contents    :
**
** ###################################################################*/

#include "console/ringBuffer.h"

/*******************************************************************************
* Global variables
*******************************************************************************/

/*******************************************************************************
* Constants and macros
*******************************************************************************/

/*******************************************************************************
* Local types
*******************************************************************************/

/*******************************************************************************
* Local function prototypes
*******************************************************************************/

/*******************************************************************************
* Local variables
*******************************************************************************/

/*******************************************************************************
* Local functions
*******************************************************************************/

/*******************************************************************************
* Global functions
*******************************************************************************/

int32_t RBufferAdd(rBuffer_t *c, const char *data)
{
	int32_t state = -1;
	uint8_t status = 1U;
    int16_t next = (int16_t)(c->head + 1);
    if(next >= c->maxSize)
    {
        next = 0;
    }
    if(next == c->tail)
    {
        status = 0U;
    }
    if(1U == status)
    {
    	c->bufferPtr[c->head] = *data;
    	c->head = next;
    }
    if(1U == status)
    {
    	state = 0;
    }
    return state;
}

int32_t RBufferRemove(rBuffer_t *c, char *data)
{
	int32_t state = -1;
	uint8_t status = 1U;
    if(c->head == c->tail)
    {
        status = 0U;
    }
    if(1U == status)
    {
    	int16_t next = (int16_t)(c->tail + 1);
    	if(next >= c->maxSize)
    	{
    		next = 0;
    	}
    	*data = c->bufferPtr[c->tail];
    	c->tail = next;
    }
    if(1U == status)
    {
    	state = 0;
    }
    return state;
}

int16_t RBufferSize(const rBuffer_t *c)
{
	int16_t current = c->tail;
	int16_t count = 0;
	if(current >= c->maxSize)
	{
		current = 0;
	}
	if(c->head > current)
	{
		count = (int16_t)(c->head - current);
	}
	return count;
}
