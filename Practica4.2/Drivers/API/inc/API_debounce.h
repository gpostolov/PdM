/**
  ******************************************************************************
  * @file	API_debounce.h
  * @author Gabriel Postolov
  * @brief	API debounce para la Practica4
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void debounceFSM_init();
void debounceFSM_update();
bool readKey();
#endif /* API_INC_API_DEBOUNCE_H_ */

/***************************************************************END OF FILE****/
