/*
 * configMode.h
 *
 *  Created on: 3 kwi 2017
 *      Author: Karol
 */

#ifndef CONFIGMODE_H_
#define CONFIGMODE_H_

#include "stm32f1xx_hal.h"

void enterConfigMode(UART_HandleTypeDef *h_configUart, UART_HandleTypeDef *h_wifiUart);

#endif /* CONFIGMODE_H_ */
