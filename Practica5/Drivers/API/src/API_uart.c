/**
 ******************************************************************************
 * @file	API_uart.c
 * @author	Gabriel Postolov
 * @brief	API uart para la Practica5
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */

#include "API_uart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef UartHandle;

static const char * UART_CONFIG_M01 = "\n\rUART CONFIGURATION\n\r";
static const char * UART_CONFIG_M02 = "UART Instance: USART3\n\r";
static const char * UART_CONFIG_M03 = "UART BaudRate: 9600\n\r";
static const char * UART_CONFIG_M04 = "UART WordLength: UART_WORDLENGTH_8B\n\r";
static const char * UART_CONFIG_M05 = "UART StopBits: UART_STOPBITS_1\n\r";
static const char * UART_CONFIG_M06 = "UART Parity: UART_PARITY_ODD\n\r";
static const char * UART_CONFIG_M07 = "UART HwFlowCtl: UART_HWCONTROL_NONE\n\r";
static const char * UART_CONFIG_M08 = "UART Mode: UART_MODE_TX_RX\n\r";
static const char * UART_CONFIG_M09 = "UART OverSampling: UART_OVERSAMPLING_16\n\r";
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/
bool uartinit() {
	/* Put the USART peripheral in the Asynchronous mode (UART Mode) */
	/* UART configured as follows:
	 - Word Length = 8 Bits (7 data bit + 1 parity bit) :
	 BE CAREFUL : Program 7 data bits + 1 parity bit in PC HyperTerminal
	 - Stop Bit    = One Stop bit
	 - Parity      = ODD parity
	 - BaudRate    = 9600 baud
	 - Hardware flow control disabled (RTS and CTS signals) */
	UartHandle.Instance = USARTx;

	UartHandle.Init.BaudRate = 9600;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits = UART_STOPBITS_1;
	UartHandle.Init.Parity = UART_PARITY_ODD;
	UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode = UART_MODE_TX_RX;
	UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&UartHandle) != HAL_OK) {
		return false;
	}

	/* Output a message on Hyperterminal using printf function */
	uartsendString((uint8_t *)UART_CONFIG_M01);
	uartsendString((uint8_t *)UART_CONFIG_M02);
	uartsendString((uint8_t *)UART_CONFIG_M03);
	uartsendString((uint8_t *)UART_CONFIG_M04);
	uartsendString((uint8_t *)UART_CONFIG_M05);
	uartsendString((uint8_t *)UART_CONFIG_M06);
	uartsendString((uint8_t *)UART_CONFIG_M07);
	uartsendString((uint8_t *)UART_CONFIG_M08);
	uartsendString((uint8_t *)UART_CONFIG_M09);

	return true;
}

void uartsendString(uint8_t *pstring) {
	if (pstring == NULL)
		return;

	uint16_t str_long = strlen((const char*) pstring);
	if (str_long > 0) {
		HAL_UART_Transmit(&UartHandle, pstring, str_long, UART_TX_TIMEOUT);
	}
}

void uartSendStringSize(uint8_t * pstring, uint16_t size){
	if (pstring == NULL) return;
	if (size == 0) return;

	uint16_t str_long = strlen((const char*) pstring);

	if (str_long > 0) {
		if(size<str_long) str_long = size; //Como maximo mando el largo de la cadena.
		HAL_UART_Transmit(&UartHandle, pstring, str_long, UART_TX_TIMEOUT);
	}
}

void uartReceiveStringSize(uint8_t * pstring, uint16_t size){
	HAL_StatusTypeDef rx_status;

	if (pstring == NULL) return;
	if (size == 0) return;

	rx_status = HAL_UART_Receive(&UartHandle, pstring, size, UART_RX_TIMEOUT);

	switch (rx_status) {
		case HAL_OK:
			//TODO
			break;
		case HAL_ERROR:
			//TODO
			break;
		case HAL_BUSY:
			//TODO
			break;
		case HAL_TIMEOUT:
			//TODO
			break;
		default:
			//TODO
			break;
	}
}
/***************************************************************END OF FILE****/
