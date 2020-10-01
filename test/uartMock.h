/**
 * @file
 * @brief UART Mock for tests
 *
 * @note
 * Copyright © 2020 Evgeniy Ivanov. Contacts: <strelok1290@gmail.com>
 * All rights reserved.
 */

#ifndef UART_MOCK_H_
#define UART_MOCK_H_

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*UartMockTxCallback_t)(const unsigned char* data, unsigned int dataSize);

/**
  * @brief Sets tx callback. This callback will be called
  * when other modules trying to transmit some data by uart.
  *
  * @param[in] txCallback
  * @return nothing
  */
void uartMockSetTxCallback(UartMockTxCallback_t txCallback);

/**
  * @brief Emulates uart receive
  *
  * @param[in] data
  * @param[in] dataSize
  * @return nothing
  */
void uartMockRxData(unsigned char* data, unsigned int dataSize);


#ifdef __cplusplus
}
#endif

#endif //UART_MOCK_H_
