/*
 * uart.h
 *
 *  Created on: 3 kwi 2017
 *      Author: Karol
 */

#ifndef UART_H_
#define UART_H_


#include "stm32f1xx_hal.h"

void uartReadLine(UART_HandleTypeDef * handler, char * buffer, uint16_t buffer_size, uint32_t Timeout);
void uartWriteLine(UART_HandleTypeDef * handler, char * text);
char uartReadChar(UART_HandleTypeDef * handler);

#endif /* UART_H_ */
