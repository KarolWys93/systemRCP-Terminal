/*
 * wifiModule.c
 *
 *  Created on: 4 kwi 2017
 *      Author: Karol
 */

#include "wifiModule.h"
#include "uart.h"
#include "string.h"

//private variables
static uint32_t tickstart = 0;
static uint32_t timeoutValue = 0;
static char WiFi_cmdBuffor[64];

//private functions prototypes
void WiFi_TimeoutSet(uint32_t);
bool WiFi_TimeoutCheck(void);


WiFI_Status WiFi_restart(UART_HandleTypeDef * huart, uint32_t timeout){

	WiFI_Status status = WiFi_ERROR;
	WiFi_TimeoutSet(timeout);
	uartWriteLine(huart, "AT+RST");
	while(!WiFi_TimeoutCheck()){
		uartReadLine(huart, WiFi_cmdBuffor, sizeof WiFi_cmdBuffor / sizeof *WiFi_cmdBuffor, 100);
		if(strcmp(WiFi_cmdBuffor, "ready") == 0){
			status = WiFi_OK;
			break;
		}
	}
	return status;
}

WiFI_Status WiFi_checkAPconnection(UART_HandleTypeDef * huart){
	WiFI_Status status = WiFi_ERROR;
	bool waitForAnswer = true;

	uartWriteLine(huart, "AT+CWJAP?");
	do{
		uartReadLine(huart, WiFi_cmdBuffor, sizeof WiFi_cmdBuffor / sizeof *WiFi_cmdBuffor, 100);
		if(strcmp(WiFi_cmdBuffor, "OK") == 0){
			waitForAnswer = false;
			status = WiFi_OK;
		}else if(strcmp(WiFi_cmdBuffor, "ERROR") == 0){
			waitForAnswer = false;
		}
	}while(waitForAnswer);

	return status;
}

WiFI_Status WiFi_openConnection(UART_HandleTypeDef * huart, char * adress, char * port){
	WiFI_Status status = WiFi_ERROR;
	bool waitForAnswer = true;

	strcpy(WiFi_cmdBuffor, "AT+CIPSTART=\"TCP\",\"");
	strcat(WiFi_cmdBuffor, adress);
	strcat(WiFi_cmdBuffor, "\",");
	strcat(WiFi_cmdBuffor, port);

	uartWriteLine(huart, WiFi_cmdBuffor);
	do{
		uartReadLine(huart, WiFi_cmdBuffor, sizeof WiFi_cmdBuffor / sizeof *WiFi_cmdBuffor, 100);
		if(strcmp(WiFi_cmdBuffor, "OK") == 0){
			waitForAnswer = false;
			status = WiFi_OK;
		}else if(strcmp(WiFi_cmdBuffor, "ERROR") == 0){
			waitForAnswer = false;
			status = WiFi_ERROR;
		}else if(strcmp(WiFi_cmdBuffor, "no ip") == 0){
			waitForAnswer = false;
			status = WiFi_NO_IP;
		}else if(strcmp(WiFi_cmdBuffor, "ALREAY CONNECT") == 0){
			waitForAnswer = false;
			status = WiFi_ALREADY_CONNECT;
		}else if(strcmp(WiFi_cmdBuffor, "DNS Fail") == 0){
			waitForAnswer = false;
			status = WiFi_DNS_FAIL;
		}
	}while(waitForAnswer);

	return status;
}

WiFI_Status WiFi_closeConnection(UART_HandleTypeDef * huart){
	WiFI_Status status = WiFi_ERROR;
	bool waitForAnswer = true;

	uartWriteLine(huart, "AT+CIPCLOSE");
	do{
		uartReadLine(huart, WiFi_cmdBuffor, sizeof WiFi_cmdBuffor / sizeof *WiFi_cmdBuffor, 100);
		if(strcmp(WiFi_cmdBuffor, "OK") == 0){
			waitForAnswer = false;
			status = WiFi_OK;
		}else if(strcmp(WiFi_cmdBuffor, "ERROR") == 0){
			waitForAnswer = false;
		}
	}while(waitForAnswer);

	return status;
}

WiFI_Status WiFi_sendData(UART_HandleTypeDef * huart, char * data, uint16_t dataLength){
	WiFI_Status status = WiFi_ERROR;

	//TODO Do zaimplementowania

	return status;
}

//private functions


void WiFi_TimeoutSet(uint32_t timeout){
	timeoutValue = timeout;
	tickstart = HAL_GetTick();
}

bool WiFi_TimeoutCheck(void){
	if(HAL_GetTick() - tickstart >= timeoutValue){
		return true;
	}else{
		return false;
	}
}
