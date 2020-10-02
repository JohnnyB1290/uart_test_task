/**
 * @file
 * @brief UART Retransmission module header
 *
 * @note
 * Copyright © 2020 Evgeniy Ivanov. Contacts: <strelok1290@gmail.com>
 * All rights reserved.
 */


#ifndef UARTRETRANSMIT_H_
#define UARTRETRANSMIT_H_

#include "uart.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Retransmition block size
 */
#define RETRANSMIT_BLOCK_SIZE 128

/**
  * @brief Starts retransmission. You need to initialize UART
  * modules before calling this function. Set rx callback function
  * to NULL, because module will replace it anyway.
  *
  * @param[in] rxUart UART instance pointer to uart, that receives data
  * @param[in] txUart UART instance pointer to uart, that transmit data
  * @return
  *  - 0 on success
  *  - -1 on error
  */
int startRetransmition(Uart_t* rxUart, Uart_t* txUart);

/**
  * @brief Stops retransmission. Resets block counter.
  *
  * @return nothing
  */
void stopRetransmition(void);

#ifdef __cplusplus
}
#endif

#endif /* UARTRETRANSMIT_H_ */
