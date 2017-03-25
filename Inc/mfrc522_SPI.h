/*
 * mfrc522_SPI.h
 *
 *  Created on: 24 mar 2017
 *      Author: Karol
 */

#ifndef MFRC522_SPI_H_
#define MFRC522_SPI_H_

#include "stm32f1xx_hal.h"

typedef enum {
	NSS_LOW,
	NSS_HIGH
} MFRC522_NSS_state_t;

void mfrc522_SPI_setHandler(SPI_HandleTypeDef* spiHandler, GPIO_TypeDef* NSS_port,uint16_t NSS_pin);
uint8_t mfrc522_SPI_Send(uint8_t data);
void mfrc522_SPI_NSS(MFRC522_NSS_state_t state);

#endif /* MFRC522_SPI_H_ */
