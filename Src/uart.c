/*
 * uart.c
 *
 *  Created on: 3 kwi 2017
 *      Author: Karol
 */

#include "uart.h"
#include "string.h"

/**
 * @brief  Odczytuje jedn¹ liniê wys³an¹ do uk³adu uart.
 *
 * @note	Odczytany ci¹g znaków nie ma znaków koñca linni.
 * 		Jeœli odbierany ci¹g znaków nie bêdzie d³u¿szy ni¿ buffer_size,
 * 		odczytany tekst zostanie dociêty. Ciag znaków zakonczony jest symbolem '\0'
 *
 * @param  handler: WskaŸnik na UART_HandleTypeDef
 * @param  buffer: WskaŸnik do buforu
 * @param  buffer_size: Wielkoœc buforu
 * @param  Timeout: wielkoœc timeoutu
 */
void uartReadLine(UART_HandleTypeDef *handler, char * buffer, uint16_t buffer_size, uint32_t timeout){
	HAL_StatusTypeDef status;
	char currentChar;
	uint16_t counter = 0;

	/* Get tick */
	uint32_t tickstart = 0;
	tickstart = HAL_GetTick();

	while(((HAL_GetTick() - tickstart ) < timeout) && counter < buffer_size-1){
		status = HAL_UART_Receive(handler, (uint8_t *)&currentChar, 1, 1);
		if(status == HAL_OK){
			if(currentChar == '\r' || currentChar == '\n'){
				if(counter == 0) continue;
				else break;
			}
			*(buffer + counter++) = currentChar;
		}
	}
	*(buffer + counter) = '\0';
}

/**
 * @brief  Wysy³a podany ci¹g znaków i zakañcza go znakami koñca linni.
 *
 * @note Podany ci¹g znaków musi byc stringiem, tj. musi posiadac znak koñca tekstu '\0'
 *
 * @param  handler: WskaŸnik na UART_HandleTypeDef
 * @param  text: WskaŸnik na wysylany tekst
 */
void uartWriteLine(UART_HandleTypeDef *handler, char * text){
	HAL_UART_Transmit(handler, (uint8_t *)text, strlen(text), 1000);
	HAL_UART_Transmit(handler, (uint8_t *)"\r\n", 2, 100);
}

/**
 * @brief  Odczytuje pojedyñczy znak odebrany przez uk³ad uart.
 *
 * @param  handler: WskaŸnik na UART_HandleTypeDef
 * @retval Odczytany znak
 */
char uartReadChar(UART_HandleTypeDef *handler){
	char buffer = '\0';
	HAL_UART_Receive(handler, (uint8_t *)&buffer, 1, 1000);
	return buffer;
}
