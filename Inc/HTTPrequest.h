/*
 * HTTPrequest.h
 *
 *  Created on: 6 cze 2017
 *      Author: Karol
 */

#ifndef HTTPREQUEST_H_
#define HTTPREQUEST_H_

#include <stdbool.h>
#include "defines.h"
#include "wifiModule.h"

typedef enum {
	ACCESS_GRANTED = 0,
	ACCESS_DENIED,
	CHECK_ERROR
} CardCheckResult;


typedef struct {
	CardCheckResult result;
	WiFI_Status wifiStatus;
	uint16_t HTTP_status;
} RequestResult;

RequestResult cardRequest(UART_HandleTypeDef * WiFi, uint8_t* cardID, Terminal_mode mode, char* hostAdress, char* port);

#endif /* HTTPREQUEST_H_ */
