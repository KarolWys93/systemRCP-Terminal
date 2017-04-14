/*
 * wifiModule.h
 *
 *  Created on: 4 kwi 2017
 *      Author: Karol
 */

#ifndef WIFIMODULE_H_
#define WIFIMODULE_H_

#include "stm32f1xx_hal.h"
#include "stdbool.h"

typedef enum
{
	WiFi_OK = 0,
	WiFi_ALREADY_CONNECT,
	WiFi_ERROR,
	WiFi_NO_IP,
	WiFi_DNS_FAIL

}WiFI_Status;

//void WiFi_init(UART_HandleTypeDef * huart);
WiFI_Status WiFi_restart(UART_HandleTypeDef * huart, uint32_t timeout);
WiFI_Status WiFi_checkAPconnection(UART_HandleTypeDef * huart);
WiFI_Status WiFi_openConnection(UART_HandleTypeDef * huart, char * adress, char * port);
WiFI_Status WiFi_closeConnection(UART_HandleTypeDef * huart);
WiFI_Status WiFi_sendData(UART_HandleTypeDef * huart, char * data, uint16_t dataLength);
uint16_t WiFi_readData(UART_HandleTypeDef * huart, char * data, uint16_t bufforLen, uint32_t timeout);

//TODO odbiór danych

#endif /* WIFIMODULE_H_ */
