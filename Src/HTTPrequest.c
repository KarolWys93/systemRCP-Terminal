/*
 * HTTPrequest.c
 *
 *  Created on: 6 cze 2017
 *      Author: Karol
 */

#include "HTTPrequest.h"
#include "wifiModule.h"
#include "string.h"
#include <stdio.h>
#include "uart.h"

char httpBuffer[512];

RequestResult cardRequest(UART_HandleTypeDef * WiFi, UART_HandleTypeDef * uart, uint8_t* cardID, Terminal_mode mode, char* hostAdress, char* port){
	RequestResult result;
	result.HTTP_status = CHECK_ERROR;
	result.wifiStatus = WiFi_OK;
	result.HTTP_status = 0;

	char httpBody[48];
	uint bodySize = 0;



	bodySize = sprintf(httpBody, "{\"Mode\":\"%s\",\"CardID\":\"%02X%02X%02X%02X%02X\"}",
			mode == enterMode ? "enter" : "exit",
					cardID[0],
					cardID[1],
					cardID[2],
					cardID[3],
					cardID[4]);

	sprintf(httpBuffer, "POST /RFIDterminal HTTP/1.1\r\nHost: %s:%s\r\nConnection: keep-alive\r\nContent-Type: application/json\r\nContent-Length: %d\r\n\r\n",
			hostAdress, port, bodySize);

	strcat(httpBuffer, httpBody);

	//DEBUG!
	HAL_UART_Transmit(uart, (uint8_t *)httpBuffer, strlen(httpBuffer), 1000);

	result.wifiStatus = WiFi_openConnection(WiFi, hostAdress, port);

	if(result.wifiStatus == WiFi_OK){
		WiFi_sendData(WiFi, httpBuffer, strlen(httpBuffer));
		bodySize = WiFi_readData(WiFi, httpBuffer, sizeof httpBuffer / sizeof *httpBuffer, 5000);
		bodySize += WiFi_readData(WiFi, httpBuffer+bodySize, sizeof httpBuffer / sizeof *httpBuffer-bodySize, 5000);

		//WiFi_closeConnection(WiFi);
		//sprintf(httpBody, "%d", bodySize);
		//uartWriteLine(uart, (uint8_t *)httpBody);
		httpBuffer[bodySize] = '\0';
		//DEBUG!
		HAL_UART_Transmit(uart, (uint8_t *)httpBuffer, strlen(httpBuffer), 1000);

		result.HTTP_status = 200;
		result.result = ACCESS_DENIED;
	}
	return result;
}
