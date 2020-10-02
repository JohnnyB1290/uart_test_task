/**
 * @file
 * @brief Tests
 *
 * @note
 * Copyright 2020 Evgeniy Ivanov. Contacts: <strelok1290@gmail.com>
 * All rights reserved.
 */

#include <stdlib.h>
#include "string.h"
#include "unity.h"
#include "uartRetransmit.h"
#include "uartMock.h"


#define TX_DATA_BUFFER_SIZE 4096

static unsigned char txData[TX_DATA_BUFFER_SIZE];
static unsigned int txDataCounter = 0;

static void uartTxCallback(const unsigned char* data, unsigned int dataSize);

void setUp()
{
	txDataCounter = 0;
	uartMockSetTxCallback(uartTxCallback);
}

void tearDown()
{
	stopRetransmition();
}

static void uartTxCallback(const unsigned char* data, unsigned int dataSize)
{
	if((TX_DATA_BUFFER_SIZE - txDataCounter) < dataSize){
		txDataCounter = 0;
	}
	memcpy(&txData[txDataCounter], data, dataSize);
	txDataCounter += dataSize;
}

void test_retransmitioNullPtr()
{
	int startResult = startRetransmition(NULL, getUart(UART1));
	TEST_ASSERT_EQUAL_INT(-1, startResult);
	startResult = startRetransmition(getUart(UART0), NULL);
	TEST_ASSERT_EQUAL_INT(-1, startResult);
	startResult = startRetransmition(NULL, NULL);
	TEST_ASSERT_EQUAL_INT(-1, startResult);
}

void test_retransmition1byte()
{
	unsigned char inputData = 0xff;
	int startResult = startRetransmition(getUart(UART0), getUart(UART1));
	TEST_ASSERT_EQUAL_INT(0, startResult);
	uartMockRxData(&inputData, 1);
	TEST_ASSERT_EQUAL_UINT(0, txDataCounter);
}

void test_retransmitionBlockMinus1()
{
	unsigned int inputDataSize = RETRANSMIT_BLOCK_SIZE - 1;
	unsigned char inputData[inputDataSize];
	int startResult = startRetransmition(getUart(UART0), getUart(UART1));
	TEST_ASSERT_EQUAL_INT(0, startResult);
	uartMockRxData(inputData, inputDataSize);
	TEST_ASSERT_EQUAL_UINT(0, txDataCounter);
}

void test_retransmitionBlock()
{
	unsigned int inputDataSize = RETRANSMIT_BLOCK_SIZE;
	unsigned char inputData[inputDataSize];
	for(unsigned int i = 0; i < inputDataSize; i++){
		inputData[i] = i;
	}
	int startResult = startRetransmition(getUart(UART0), getUart(UART1));
	TEST_ASSERT_EQUAL_INT(0, startResult);
	uartMockRxData(inputData, inputDataSize);
	TEST_ASSERT_EQUAL_UINT(inputDataSize, txDataCounter);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(inputData, txData, inputDataSize);
}

void test_retransmition2BlocksMinus1()
{
	unsigned int inputDataSize = 2*RETRANSMIT_BLOCK_SIZE - 1;
	unsigned char inputData[inputDataSize];
	for(unsigned int i = 0; i < inputDataSize; i++){
		inputData[i] = i;
	}
	int startResult = startRetransmition(getUart(UART0), getUart(UART1));
	TEST_ASSERT_EQUAL_INT(0, startResult);
	uartMockRxData(inputData, inputDataSize);
	TEST_ASSERT_EQUAL_UINT(RETRANSMIT_BLOCK_SIZE, txDataCounter);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(inputData, txData, RETRANSMIT_BLOCK_SIZE);
}

void test_retransmition2Blocks()
{
	unsigned int inputDataSize = 2*RETRANSMIT_BLOCK_SIZE;
	unsigned char inputData[inputDataSize];
	for(unsigned int i = 0; i < inputDataSize; i++){
		inputData[i] = i;
	}
	int startResult = startRetransmition(getUart(UART0), getUart(UART1));
	TEST_ASSERT_EQUAL_INT(0, startResult);
	uartMockRxData(inputData, inputDataSize);
	TEST_ASSERT_EQUAL_UINT(inputDataSize, txDataCounter);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(inputData, txData, inputDataSize);
}
