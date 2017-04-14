/*
 * uart.c
 *
 *  Created on: 3 kwi 2017
 *      Author: Karol
 */

#include "uart.h"
#include "string.h"

/**
 * @brief  Odczytuje jedn� lini� wys�an� do uk�adu uart.
 *
 * @note	Odczytany ci�g znak�w nie ma znak�w ko�ca linni.
 * 		Je�li odbierany ci�g znak�w nie b�dzie d�u�szy ni� buffer_size,
 * 		odczytany tekst zostanie doci�ty. Ciag znak�w zakonczony jest symbolem '\0'
 *
 * @param  handler: Wska�nik na UART_HandleTypeDef
 * @param  buffer: Wska�nik do buforu
 * @param  buffer_size: Wielko�c buforu
 * @param  Timeout: wielko�c timeoutu
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
 * @brief  Wysy�a podany ci�g znak�w i zaka�cza go znakami ko�ca linni.
 *
 * @note Podany ci�g znak�w musi byc stringiem, tj. musi posiadac znak ko�ca tekstu '\0'
 *
 * @param  handler: Wska�nik na UART_HandleTypeDef
 * @param  text: Wska�nik na wysylany tekst
 */
void uartWriteLine(UART_HandleTypeDef *handler, char * text){
	HAL_UART_Transmit(handler, (uint8_t *)text, strlen(text), 1000);
	HAL_UART_Transmit(handler, (uint8_t *)"\r\n", 2, 100);
}

/**
 * @brief  Odczytuje pojedy�czy znak odebrany przez uk�ad uart.
 *
 * @param  handler: Wska�nik na UART_HandleTypeDef
 * @retval Odczytany znak
 */
char uartReadChar(UART_HandleTypeDef *handler){
	char buffer = '\0';
	HAL_UART_Receive(handler, (uint8_t *)&buffer, 1, 1000);
	return buffer;
}
