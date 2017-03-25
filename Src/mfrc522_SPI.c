/*
 * mfrc522_SPI.c
 *
 *  Created on: 24 mar 2017
 *      Author: Karol
 */

#include "mfrc522_SPI.h"
#include "stm32f1xx_hal.h"

SPI_HandleTypeDef* spiHandler;
GPIO_TypeDef* NSS_Port;
uint16_t NSS_Pin;

void mfrc522_SPI_setHandler(SPI_HandleTypeDef* hspi, GPIO_TypeDef* NSS_port, uint16_t NSS_pin){
	spiHandler = hspi;
	NSS_Port = NSS_port;
	NSS_Pin = NSS_pin;
}

uint8_t mfrc522_SPI_Send(uint8_t data){
	uint8_t value = 0;
	HAL_SPI_TransmitReceive(spiHandler, &data, &value, 1, 100);
	return value;
}
void mfrc522_SPI_NSS(MFRC522_NSS_state_t state){
	if(state == NSS_LOW){
		HAL_GPIO_WritePin(NSS_Port, NSS_Pin, GPIO_PIN_RESET);
	}else{
		HAL_GPIO_WritePin(NSS_Port, NSS_Pin, GPIO_PIN_SET);
	}
}
