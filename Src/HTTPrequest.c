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
#include <stdlib.h>
#include "uart.h"
#include "jsmn.h"

char httpBuffer[256];

static int8_t jsoneq(const char *json, jsmntok_t *tok, const char *s);

RequestResult cardRequest(UART_HandleTypeDef * WiFi, uint8_t* cardID, Terminal_mode mode, char* hostAdress, char* port){
	RequestResult result;
	result.HTTP_status = CHECK_ERROR;
	result.wifiStatus = WiFi_OK;
	result.HTTP_status = 0;
	jsmn_parser jsonParser;
	jsmntok_t jsonTokens [6];

	char httpBody[48];
	uint size = 0;

	//Przygotowanie zapytania

	size = sprintf(httpBody, "{\"Mode\":\"%s\",\"CardID\":\"%02X%02X%02X%02X%02X\"}",
			mode == enterMode ? "enter" : "exit",
					cardID[0],
					cardID[1],
					cardID[2],
					cardID[3],
					cardID[4]);

	sprintf(httpBuffer, "POST /RFIDterminal HTTP/1.1\r\nHost: %s:%s\r\nConnection: keep-alive\r\nContent-Type: application/json\r\nContent-Length: %d\r\n\r\n",
			hostAdress, port, size);

	strcat(httpBuffer, httpBody);


	//wys³anie zapytania
	result.wifiStatus = WiFi_openConnection(WiFi, hostAdress, port);

	if(result.wifiStatus == WiFi_OK){

		WiFi_sendData(WiFi, httpBuffer, strlen(httpBuffer));

		//odbiór odpowiedzi
		size = WiFi_readData(WiFi, httpBuffer, sizeof httpBuffer / sizeof *httpBuffer, 5000);
		size += WiFi_readData(WiFi, httpBuffer+size, (sizeof httpBuffer-size) / sizeof *httpBuffer, 5000);

		httpBuffer[size] = '\0';

		//interpretacja nag³ówka
		result.HTTP_status = atoi(httpBuffer+9);

		if(result.HTTP_status == 200){
			//interpretacja odebranych danych

			char* responseBody = strstr(httpBuffer, "\r\n\r\n")+4;	//\r\n\r\n -> 4 znaki

			jsmn_init(&jsonParser);
			int8_t numOfTokens = jsmn_parse(&jsonParser, responseBody, strlen(responseBody), jsonTokens, sizeof(jsonTokens)/sizeof(jsonTokens[0]));

			if ((numOfTokens < 0) || numOfTokens < 1 || jsonTokens[0].type != JSMN_OBJECT) {
				result.result = CHECK_ERROR;
				return result;
			}

			for(uint8_t i = 1; i < numOfTokens; i++){
				if(jsoneq(responseBody, &jsonTokens[i], "Access") == 0){
					if(jsoneq(responseBody, &jsonTokens[i+1], "granted") == 0){
						result.result = ACCESS_GRANTED;
					}else if(jsoneq(responseBody, &jsonTokens[i+1], "denied") == 0){
						result.result = ACCESS_DENIED;
					}else{
						result.result = CHECK_ERROR;
					}
				}
			}
		}
	}
	return result;
}

static int8_t jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}
