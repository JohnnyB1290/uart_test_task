/**
 * @file
 * @brief UART API
 *
 * @note
 * Copyright © 2020 Evgeniy Ivanov. Contacts: <strelok1290@gmail.com>
 * All rights reserved.
 */

#ifndef UART_H_
#define UART_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Size of the transmit ring buffer */
#define UART_TX_BUFFER_SIZE		2048

typedef void (*UartRxCallback_t)(unsigned char byte, void* ctx);

#define UART_DEFAULT_CONFIG() { 			\
	.baudrate 			= 115200,			\
	.parity 			= PARITY_NO_PARITY, \
	.stopBitDuration 	= STOP_BIT_1,		\
	.interruptPriority 	= 0,				\
	.rxCallback 		= NULL,				\
	.ctx				= NULL,				\
}

typedef enum
{
	UART_ERR_OK = 0,
	UART_ERR_UNDEFINED = -1,
}UartErrorCodes_t;

typedef enum
{
	UART0 = 0,
	UART1 = 1,
	UART2 = 2,
	UART_MAX
}UartNum_t;

typedef enum
{
	PARITY_NO_PARITY = 0,
	PARITY_EVEN,
	PARITY_ODD,
}UartParity_t;

typedef enum
{
	STOP_BIT_1 = 0,
	STOP_BIT_1_5,
	STOP_BIT_2
}UartStopBitDuration_t;

/**
 * @brief   UART Configuration Structure
 *
 * @note    Use UART_DEFAULT_CONFIG() to initialize the configuration
 *          to a default value
 */
typedef struct
{
	unsigned int baudrate;
	UartParity_t parity;
	UartStopBitDuration_t stopBitDuration;
	int interruptPriority;
	/**
	 * Receive byte callback. This function will be called from ISR,
	 * so you should exit from it as soon as possible.
	 */
	UartRxCallback_t rxCallback;
	/**
	 * User context. This context will be sent in parameters
	 * of rx callback.
	 */
	void* ctx;
}UartConfig_t;

typedef struct
{
	/**
	  * @brief Initializes and start UART module
	  *
	  * @param[in] initData Pointer to structure with configuration
	  * @return error code
	  * 	- UART_ERR_OK on success
	  * 	- other error codes on fail
	  */
	int (*initialize)(const UartConfig_t* config);

	/**
	  * @brief Deinitializes UART module
	  *
	  * @return nothing
	  */
	void (*deInitialize)(void);

	/**
	  * @brief Sets baud rate
	  *
	  * @param[in] baudrate
	  * @return error code
	  * 	- UART_ERR_OK on success
	  * 	- other error codes on fail
	  */
	int (*setBaudrate)(unsigned int baudrate);

	/**
	  * @brief Sets parity
	  *
	  * @param[in] parity
	  * @return error code
	  * 	- UART_ERR_OK on success
	  * 	- other error codes on fail
	  */
	int (*setParity)(UartParity_t parity);

	/**
	  * @brief Sets stop bit duration
	  *
	  * @param[in] duration Stop bit duration
	  * @return error code
	  * 	- UART_ERR_OK on success
	  * 	- other error codes on fail
	  */
	int (*setStopBit)(UartStopBitDuration_t duration);

	/**
	  * @brief Sets receive callback function
	  *
	  * @param[in] rxCallback Receive byte callback
	  * @return nothing
	  */
	void (*setRxCallback)(UartRxCallback_t rxCallback);

	/**
	  * @brief Sets user context
	  *
	  * @param[in] ctx User context.
	  * @return nothing
	  */
	void (*setCtx)(void* ctx);

	/**
	  * @brief Transmits 1 byte.
	  *
	  * Module uses interrupts so function doesn't wait until tx is done.
	  * It just puts data to tx ring buffer.
	  *
	  * @param[in] byte Byte to transmit
	  * @return
	  * 	- number of bytes successfully added to buffer
	  * 	- other error codes on fail
	  */
	int (*txByte)(unsigned char byte);

	/**
	  * @brief Transmits data buffer.
	  *
	  * Module uses interrupts so function doesn't wait until tx is done.
	  * It just puts data to tx ring buffer.
	  *
	  * @param[in] data Pointer to data needed to transmit
	  * @param[in] dataSize Size of data
	  * @return
	  * 	- number of bytes successfully added to buffer
	  * 	- other error codes on fail
	  */
	int (*tx)(const unsigned char* data, unsigned int dataSize);
}Uart_t;

/**
  * @brief Returns pointer to instance of UART module.
  *
  * @param[in] num UART number. Must be < UART_MAX
  * @return Pointer to instance of UART.
  */
Uart_t* getUart(UartNum_t num);

#ifdef __cplusplus
}
#endif

#endif /* UART_H_ */
