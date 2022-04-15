/**
 ******************************************************************************
 * @file	API_debounce.c
 * @author	Gabriel Postolov
 * @brief	API debounce
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */

#include "API_delay.h"
#include "API_debounce.h"
/* Private typedef -----------------------------------------------------------*/
typedef enum {
	BUTTON_UP, BUTTON_FALLING, BUTTON_DOWN, BUTTON_RAISING,
} debounceState_t;
/* Private define ------------------------------------------------------------*/
#define DEBOUNCING_DELAY 40
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static delay_t delay_BUTTON;
static debounceState_t current_state;
static bool_t pressed;
/* Private function prototypes -----------------------------------------------*/
void buttonPressed(void);
void buttonReleased(void);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Initialize FSM.
 * @param  None.
 * @retval None.
 */
void debounceFSM_init(void) {
	delayInit(&delay_BUTTON, DEBOUNCING_DELAY);
	current_state = BUTTON_UP;
	pressed = false;
}

/**
 * @brief  Update FSM.
 * @param  None.
 * @retval None.
 */
void debounceFSM_update(void) {
	switch (current_state) {
	case BUTTON_UP:
		if (BSP_PB_GetState(BUTTON_USER)) {
			delayInit(&delay_BUTTON, DEBOUNCING_DELAY);
			current_state = BUTTON_FALLING;
		}
		break;
	case BUTTON_FALLING:
		if (delayRead(&delay_BUTTON)) {
			if (BSP_PB_GetState(BUTTON_USER)) {
				current_state = BUTTON_DOWN;
				buttonPressed();
			} else {
				current_state = BUTTON_UP;
			}
		}
		break;
	case BUTTON_DOWN:
		if (!BSP_PB_GetState(BUTTON_USER)) {
			delayInit(&delay_BUTTON, DEBOUNCING_DELAY);
			current_state = BUTTON_RAISING;
		}
		break;
	case BUTTON_RAISING:
		if (delayRead(&delay_BUTTON)) {
			if (BSP_PB_GetState(BUTTON_USER)) {
				current_state = BUTTON_DOWN;
			} else {
				current_state = BUTTON_UP;
				buttonReleased();
			}
		}
		break;
	default:
		debounceFSM_init();
		break;
	}
}

/**
 * @brief  Button User pressed.
 * @param  None.
 * @retval None.
 */
void buttonPressed(void) {
	pressed = true;
}

/**
 * @brief  Button User released.
 * @param  None.
 * @retval None.
 */
void buttonReleased(void) {
	pressed = false;
}

/* Public functions ---------------------------------------------------------*/
/**
 * @brief  Read key.
 * @param  None.
 * @retval true: Button User pressed.
 * @retval false: Button User not pressed.
 */
bool readKey(void) {
	return pressed;
}

/***************************************************************END OF FILE****/
