/**
 * @file
 * @brief UART Test Task
 *
 * @note
 * Copyright © 2020 Evgeniy Ivanov. Contacts: <strelok1290@gmail.com>
 * All rights reserved.
 */

#include <stdlib.h>
#include "uartRetransmit.h"


int main(void) {
	Uart_t* uart1 = getUart(UART0);
	Uart_t* uart2 = getUart(UART1);
	if((!uart1) || (!uart2)){
		return 0;
	}

	UartConfig_t config = UART_DEFAULT_CONFIG();
	int result = uart2->initialize(config);
	if(result != UART_ERR_OK){
		return 0;
	}

	config.baudrate = 9600;
	result = uart1->initialize(config);
	if(result != UART_ERR_OK){
		uart2->deInitialize();
		return 0;
	}

	startRetransmition(uart1, uart2);

	while(1){

	}
	return 0;
}
