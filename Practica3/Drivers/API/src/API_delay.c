/**
 ******************************************************************************
 * @file	main.c
 * @author	Gabriel Postolov
 * @brief	Main File para la Practica2
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "API_delay.h"

#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */

/** @addtogroup STM32F4xx_HAL_Examples
 * @{
 */

/** @addtogroup UART_Printf
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Initializes the led delays
 * @param  delay: delay to be initialized.
 * @param  duration: duration in milliseconds of the delay
 * @retval None
 */
void delayInit(delay_t *delay, tick_t duration) {
	if (delay == NULL) {
		delay->startTime = HAL_GetTick();
		return;
	} else {
		delay->duration = duration;
		delay->running = false;
		delay->startTime = HAL_GetTick();
	}
}

/**
 * @brief  The state of the running flag is checked. If false, it
 * takes the timestamp and changes the execution flag to 'true'.
 * If it is true, it makes the account to know if the delay time has
 * been fulfilled or not. Returns a boolean value indicating
 * whether or not the time has expired.
 * @param  delay: delay to be checked.
 * @retval bool_t: value indicating whether the time has expired or not.
 */
bool_t delayRead(delay_t *delay) {
	if (delay == NULL) {
		return false;
	} else {
		if (delay->running) {
			if ((HAL_GetTick() - delay->startTime) >= delay->duration) {
				delay->startTime = HAL_GetTick();
				return true;
			} else {
				return false;
			}
		} else {
			delay->startTime = HAL_GetTick();
			delay->running = true;
			return false;
		}
	}
}

/**
 * @brief  The function allows to change the duration time
 * of an existing delay.
 * @param  delay: delay to be modificated.
 * @param  duration: duration in milliseconds of the delay
 * @retval None
 */
void delayWrite(delay_t *delay, tick_t duration) {
	if (delay == NULL) {
		return;
	} else {
		delay->duration = duration;
	}
}
