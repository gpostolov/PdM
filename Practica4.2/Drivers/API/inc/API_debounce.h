/*
 * API_debounce.h
 *
 *  Created on: 26 mar. 2022
 *      Author: Gabriel
 */

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define DEBOUNCING_DELAY 40

typedef enum{
BUTTON_UP,
BUTTON_FALLING,
BUTTON_DOWN,
BUTTON_RAISING,
} debounceState_t;

void debounceFSM_init();		// debe cargar el estado inicial
void debounceFSM_update();	    // debe leer las entradas, resolver la lógica de
					            // transición de estados y actualizar las salidas
bool readKey();

//
void buttonPressed();			// debe togglear el LED1
void buttonReleased();		    // debe togglear el LED3

#endif /* API_INC_API_DEBOUNCE_H_ */
