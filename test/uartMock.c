/**
 * @file
 * @brief UART Mock for tests
 *
 * @note
 * Copyright © 2020 Evgeniy Ivanov. Contacts: <strelok1290@gmail.com>
 * All rights reserved.
 */

#include "uart.h"
#include "uartMock.h"

static Uart_t uartInstance_;
static UartRxCallback_t rxCallback_ = NULL;
static void* ctx_ = NULL;
static UartMockTxCallback_t txCallback_ = NULL;


static int initialize(const UartConfig_t* config);
static void deInitialize(void){ }
static int setBaudrate(unsigned int baudrate){ return UART_ERR_OK; }
static int setParity(UartParity_t parity){ return UART_ERR_OK; }
static int setStopBit(UartStopBitDuration_t duration){ return UART_ERR_OK; }
void setCtx(void* ctx){ ctx_ = ctx;}
static int txByte(unsigned char byte);
static int tx(const unsigned char* data, unsigned int dataSize);


static int initialize(const UartConfig_t* config)
{
	rxCallback_ = config->rxCallback;
	return UART_ERR_OK;
}

static void setRxCallback(UartRxCallback_t rxCallback)
{
	rxCallback_ = rxCallback;
}

void uartMockSetTxCallback(UartMockTxCallback_t txCallback)
{
	txCallback_ = txCallback;
}

void uartMockRxData(unsigned char* data, unsigned int dataSize)
{
	if(rxCallback_){
		for(unsigned int i = 0; i < dataSize; i++){
			rxCallback_(data[i], ctx_);
		}
	}
}

static int txByte(unsigned char byte)
{
	return tx(&byte, 1);
}

static int tx(const unsigned char* data, unsigned int dataSize)
{
	if(txCallback_){
		txCallback_(data, dataSize);
	}
	return dataSize;
}

Uart_t* getUart(UartNum_t num)
{
	static unsigned char ini = 0;
	if(!ini){
		uartInstance_.initialize = initialize;
		uartInstance_.deInitialize = deInitialize;
		uartInstance_.setBaudrate = setBaudrate;
		uartInstance_.setParity = setParity;
		uartInstance_.setStopBit = setStopBit;
		uartInstance_.setRxCallback = setRxCallback;
		uartInstance_.setCtx = setCtx;
		uartInstance_.txByte = txByte;
		uartInstance_.tx = tx;
		ini = 1;
	}
	return &uartInstance_;
}
