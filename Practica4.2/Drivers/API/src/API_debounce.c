/*
 * API_debounce.c
 *
 *  Created on: 26 mar. 2022
 *      Author: Gabriel
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */
#include "API_delay.h"
#include "API_debounce.h"

static delay_t delay_BUTTON;
static debounceState_t current_state;

static bool_t press;

void debounceFSM_init(){
	delayInit(&delay_BUTTON, DEBOUNCING_DELAY);
	current_state = BUTTON_UP;
	press = false;
}

void debounceFSM_update(){
	// debe leer las entradas, resolver la lógica de
	// transición de estados y actualizar las salidas
	switch (current_state) {
	case BUTTON_UP:
		if(BSP_PB_GetState(BUTTON_USER)){
			delayInit(&delay_BUTTON, DEBOUNCING_DELAY);
			current_state = BUTTON_FALLING;
		}
		break;
	case BUTTON_FALLING:
		if (delayRead(&delay_BUTTON)){
			if(BSP_PB_GetState(BUTTON_USER)){
				buttonPressed();
				current_state = BUTTON_DOWN;
			}else{
				current_state = BUTTON_UP;
			}
		}
		break;
	case BUTTON_DOWN:
		if(!BSP_PB_GetState(BUTTON_USER)){
			delayInit(&delay_BUTTON, DEBOUNCING_DELAY);
			current_state = BUTTON_RAISING;
		}
		break;
	case BUTTON_RAISING:
		if (delayRead(&delay_BUTTON)){
			if(BSP_PB_GetState(BUTTON_USER)){
				current_state = BUTTON_DOWN;
			}else{
				buttonReleased();
				current_state = BUTTON_UP;
				press = true;
			}
		}
		break;
	default:
		debounceFSM_init();
		break;
	}
}

void buttonPressed(){
	// debe togglear el LED1
	BSP_LED_Toggle(LED1);
}
void buttonReleased(){
	// debe togglear el LED3
	BSP_LED_Toggle(LED3);
}

bool readKey(){
	bool_t press_aux = press;
	press = false;
	return press_aux;
}
