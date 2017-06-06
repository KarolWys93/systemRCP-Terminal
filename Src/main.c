/**
 ******************************************************************************
 * File Name          : main.c
 * Description        : Main program body
 ******************************************************************************
 *
 * COPYRIGHT(c) 2017 STMicroelectronics
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */
#include "stdbool.h"
#include "mfrc522.h"
#include "mfrc522_SPI.h"
#include "configMode.h"
#include "uart.h"
#include "wifiModule.h"
#include "string.h"
#include "defines.h"
#include "HTTPrequest.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

#define uartPC huart3
#define uartWiFi huart2



Terminal_mode terminalMode = enterMode;
uint8_t cardID[5];

//char WiFibuffor[512];
uint16_t size = 0;
char cardIDtext[20];


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART2_UART_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

void initTerminal(void);
void checkModeChange(void);
bool newCardDetected(uint8_t* id);
//void prepareRequest(char* request, uint8_t* cardID, Terminal_mode mode, char* hostAdress, char* port);

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
void checkModeChange(void){
	if(HAL_GPIO_ReadPin(enterButton_GPIO_Port, enterButton_Pin) == GPIO_PIN_RESET){
		HAL_GPIO_WritePin(enterLed_GPIO_Port, enterLed_Pin, RESET);
		HAL_GPIO_WritePin(exitLed_GPIO_Port, exitLed_Pin, SET);
		terminalMode = enterMode;
	}

	if(HAL_GPIO_ReadPin(exitButton_GPIO_Port, exitButton_Pin) == GPIO_PIN_RESET){
		HAL_GPIO_WritePin(enterLed_GPIO_Port, enterLed_Pin, SET);
		HAL_GPIO_WritePin(exitLed_GPIO_Port, exitLed_Pin, RESET);
		terminalMode = exitMode;
	}
}

void initTerminal(void){

}

bool newCardDetected(uint8_t* id){
	static uint8_t lastCardStatus = 0;
	if(MFRC522_Check(id) == MI_OK){
		if(lastCardStatus != MI_OK){
			lastCardStatus = MI_OK;
			return true;
		}
	}else{
		lastCardStatus = MI_NOTAGERR;
	}
	return false;
}

//void prepareRequest(char* request, uint8_t* cardID, Terminal_mode mode, char* hostAdress, char* port){
//
//	char cardNumStr[11];
//	sprintf(cardNumStr, "%02X%02X%02X%02X%02X", cardID[0], cardID[1], cardID[2], cardID[3], cardID[4]);
//
//	//nag³ówek
//	strcpy(request, "GET / HTTP/1.1\r\n");
//	strcat(request, "Host:");
//	strcat(request, hostAdress);
//	strcat(request, ":");
//	strcat(request, port);
//	strcat(request, "\r\n");
//	strcat(request, "Connection: keep-alive\r\n\r\n");
//	//koniec nag³ówka
//
//	strcat(request, "{\"Mode\": ");
//
//	if(mode == enterMode){
//		strcat(request, "\"enter\", ");
//	}else{
//		strcat(request, "\"exit\", ");
//	}
//
//	strcat(request, "\"CardID\": \"");
//	strcat(request, cardNumStr);
//	strcat(request, "\"}");
//}
//

/* USER CODE END 0 */

int main(void)
{

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration----------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_SPI1_Init();
	MX_USART3_UART_Init();
	MX_USART2_UART_Init();

	/* USER CODE BEGIN 2 */

	//próba wejœcia w tryb konfiguracji
	enterConfigMode(&uartPC, &uartWiFi);

	uartWriteLine(&uartPC, "Starting!");

	mfrc522_SPI_setHandler(&hspi1, SPI_NSS_GPIO_Port, SPI_NSS_Pin);
	MFRC522_Init();

	if(WiFi_restart(&uartWiFi, 10000) != WiFi_OK)
		Error_Handler();
	uartWriteLine(&uartPC, "WiFi OK!");

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		HAL_Delay(250);

		checkModeChange();

		if(newCardDetected(cardID)){
			HAL_GPIO_WritePin(redLed_GPIO_Port, redLed_Pin, RESET);	//Zapalenie czerwonej diody
			//sprintf(cardIDtext, "ID: %02X%02X%02X%02X%02X", cardID[0], cardID[1], cardID[2], cardID[3], cardID[4]);
			//uartWriteLine(&uartPC, cardIDtext);

			//testowe -> otwiera po³¹czenie, ale go nie zamyka
			if(WiFi_checkAPconnection(&uartWiFi) == WiFi_OK){

				RequestResult status = cardRequest(&uartWiFi, &uartPC, cardID, terminalMode, "192.168.0.157", "8189");

				//WiFI_Status status = WiFi_openConnection(&uartWiFi, "Karol-Lenovo", "8189");	//Nawi¹zanie ³¹cznoœci z serwerem
				//WiFI_Status status = WiFi_openConnection(&uartWiFi, "192.168.0.157", "8189");
				switch (status.wifiStatus) {
					case WiFi_OK:
						uartWriteLine(&uartPC, "OK");
//						strcpy(WiFibuffor, cardIDtext);
//						strcat(WiFibuffor, "\r\n\r\n");
//						prepareRequest(WiFibuffor, cardID, terminalMode, "192.168.0.157", "8189");
//						HAL_UART_Transmit(&uartPC, (uint8_t *)WiFibuffor, strlen(WiFibuffor), 1000);
//						WiFi_sendData(&uartWiFi, WiFibuffor, strlen(WiFibuffor));
//						size = WiFi_readData(&uartWiFi, WiFibuffor, 256, 5000);
//						uartWriteLine(&uartPC, "send OK!");
//						HAL_UART_Transmit(&uartPC, (uint8_t *)WiFibuffor, size, 1000);

						break;
					case WiFi_NO_IP:
						uartWriteLine(&uartPC, "no ip");
						break;
					case WiFi_ERROR:
						uartWriteLine(&uartPC, "ERROR");
						break;
					case WiFi_DNS_FAIL:
						uartWriteLine(&uartPC, "DNS Fail");
						break;
					case WiFi_ALREADY_CONNECT:
						uartWriteLine(&uartPC, "ALREAY CONNECT");
						break;
					default:
						break;
				}
			}else{
				uartWriteLine(&uartPC, "no AP");
			}
			HAL_Delay(1000);
		}else{
			HAL_GPIO_WritePin(redLed_GPIO_Port, redLed_Pin, SET);	//zgaszenie diody
		}

	}
	/* USER CODE END 3 */

}

/** System Clock Configuration
 */
void SystemClock_Config(void)
{

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	{
		Error_Handler();
	}

	/**Configure the Systick interrupt time
	 */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	/**Configure the Systick
	 */
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
	hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
		Error_Handler();
	}

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_Handler();
	}

}

/* USART3 init function */
static void MX_USART3_UART_Init(void)
{

	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart3) != HAL_OK)
	{
		Error_Handler();
	}

}

/** Configure pins as 
 * Analog
 * Input
 * Output
 * EVENT_OUT
 * EXTI
 */
static void MX_GPIO_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(redLed_GPIO_Port, redLed_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(enterLed_GPIO_Port, enterLed_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, exitLed_Pin|SPI_NSS_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin : redLed_Pin */
	GPIO_InitStruct.Pin = redLed_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(redLed_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : enterLed_Pin exitLed_Pin */
	GPIO_InitStruct.Pin = enterLed_Pin|exitLed_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : SPI_NSS_Pin */
	GPIO_InitStruct.Pin = SPI_NSS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(SPI_NSS_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : exitButton_Pin enterButton_Pin */
	GPIO_InitStruct.Pin = exitButton_Pin|enterButton_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler */
	/* User can add his own implementation to report the HAL error return state */
	while(1)
	{
	}
	/* USER CODE END Error_Handler */
}

#ifdef USE_FULL_ASSERT

/**
 * @brief Reports the name of the source file and the source line number
 * where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */

}

#endif

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
