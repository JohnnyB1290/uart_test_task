/**
 * @file
 * @brief UART Retransmission module realization
 *
 * @note
 * Copyright © 2020 Evgeniy Ivanov. Contacts: <strelok1290@gmail.com>
 * All rights reserved.
 */

#include <stdlib.h>
#include "uartRetransmit.h"

static Uart_t* rxUart_ = NULL;
static Uart_t* txUart_ = NULL;
static unsigned char block_[RETRANSMIT_BLOCK_SIZE];
static unsigned int blockCounter_ = 0;

static void rxCallback(unsigned char byte, void* ctx);



int startRetransmition(Uart_t* rxUart, Uart_t* txUart)
{
	if((!rxUart) || (!txUart)){
		return -1;
	}
	rxUart_ = rxUart;
	txUart_ = txUart;
	rxUart_->setRxCallback(rxCallback);
	return 0;
}



void stopRetransmition(void)
{
	if(rxUart_){
		rxUart_->setRxCallback(NULL);
	}
	rxUart_ = NULL;
	txUart_ = NULL;
	blockCounter_ = 0;
}



static void rxCallback(unsigned char byte, void* ctx)
{
	/**
	 *	As this callback will be called from ISR,
	 *	we need to exit at least faster then next byte will be received.
	 *	This realization was written in assumption that copying to the tx buffer
	 *	is much faster operation, than receiving byte. In other case, we need to use
	 *	ring buffer, check it periodically if it contains enough data and make tx
	 *	outside this callback.
	 */
	block_[blockCounter_++] = byte;
	if((blockCounter_ % RETRANSMIT_BLOCK_SIZE) == 0){
		if(txUart_){
			txUart_->tx(block_, RETRANSMIT_BLOCK_SIZE);
		}
		blockCounter_ = 0;
	}
}
