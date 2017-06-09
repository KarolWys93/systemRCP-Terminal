/*
 * configMode.c
 *
 *  Created on: 3 kwi 2017
 *      Author: Karol
 */

#include "configMode.h"
#include "uart.h"
#include "string.h"
#include "stdbool.h"

static int8_t checkPass(UART_HandleTypeDef *h_configUart);

char HOST_NAME[64] = "192.168.0.157";
char HOST_PORT[6] = "8189";

/**
 * @brief  Funkcja implementuj¹ca tryb konfiguracyjny termianala
 * @param  h_configUart: WskaŸnik na UART_HandleTypeDef uk³adu
 * 					  uart s³u¿¹cego do po³¹czenia z PC
 * @param  h_wifiUart: WskaŸnik na UART_HandleTypeDef uk³adu uart
 * 					s³u¿¹cego do po³¹czenia z modu³em wifi
 */
void enterConfigMode(UART_HandleTypeDef *h_configUart, UART_HandleTypeDef *h_wifiUart){

	char rxBuff[128];

	//Wysy³a znak zachêty, nastêpnie czeka na komendê.
	uartWriteLine(h_configUart, ">");
	uartReadLine(h_configUart, rxBuff, sizeof rxBuff / sizeof rxBuff[0], 1000);

	//tryb komend AT dla modu³u WiFi
	if(strcmp(rxBuff, "config -a") == 0){
		if(checkPass(h_configUart) == 0){
			uartWriteLine(h_configUart, "OK!");
			__HAL_RCC_DMA1_CLK_ENABLE();

			__HAL_UART_FLUSH_DRREGISTER(h_configUart);
			__HAL_UART_FLUSH_DRREGISTER(h_wifiUart);

			h_configUart->Instance->CR3|= USART_CR3_DMAR;
			h_wifiUart->Instance->CR3|= USART_CR3_DMAR;

			DMA1_Channel3->CPAR = (uint32_t)&h_configUart->Instance->DR;
			DMA1_Channel3->CMAR = (uint32_t)&h_wifiUart->Instance->DR;
			DMA1_Channel3->CNDTR = 1;
			DMA1_Channel3->CCR = DMA_CCR_EN | DMA_CCR_PSIZE_0 | DMA_CCR_MSIZE_0 | DMA_CCR_CIRC;

			DMA1_Channel6->CPAR = (uint32_t)&h_wifiUart->Instance->DR;
			DMA1_Channel6->CMAR = (uint32_t)&h_configUart->Instance->DR;
			DMA1_Channel6->CNDTR = 1;
			DMA1_Channel6->CCR = DMA_CCR_EN | DMA_CCR_PSIZE_0 | DMA_CCR_MSIZE_0 | DMA_CCR_CIRC;
			while(1){};
		}
		uartWriteLine(h_configUart, "Access denied!");
	}else if(strcmp(rxBuff, "config -s") == 0){
		if(checkPass(h_configUart) == 0){
			uartWriteLine(h_configUart, "Host adress:");
			uartReadLine(h_configUart, rxBuff, sizeof rxBuff / sizeof rxBuff[0], 20000);
			char* portStr = strchr(rxBuff, ':');
			if(portStr != NULL){
				*portStr = '\0';
				strcpy(HOST_NAME, rxBuff);
				strcpy(HOST_PORT, portStr+1);
				uartWriteLine(h_configUart, "Host:");
				uartWriteLine(h_configUart, HOST_NAME);
				uartWriteLine(h_configUart, "Port:");
				uartWriteLine(h_configUart, HOST_PORT);
			}else{
				uartWriteLine(h_configUart, "Wrong host name!");
			}
		}else{
			uartWriteLine(h_configUart, "Access denied!");
		}
	}
}

int8_t checkPass(UART_HandleTypeDef *h_configUart){
	const char CONFIG_PASS [] = "12345678";	//Rozwi¹zanie chwilowe!!
	char passBuff[16];
	uartWriteLine(h_configUart, "Password:");
	uartReadLine(h_configUart, passBuff, sizeof passBuff/sizeof passBuff[0], 20000);	//Oczekiwanie 20s na podanie has³a
	return strcmp(passBuff, CONFIG_PASS);
}

