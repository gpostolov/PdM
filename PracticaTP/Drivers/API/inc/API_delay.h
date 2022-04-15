/**
 ******************************************************************************
 * @file	API_delay.h
 * @author	Gabriel Postolov
 * @brief	API_delay
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef API_API_DELAY_H_
#define API_API_DELAY_H_

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdbool.h>

/* Exported types ------------------------------------------------------------*/
typedef uint32_t tick_t;
typedef bool bool_t;
typedef struct{
   tick_t startTime;
   tick_t duration;
   bool_t running;
} delay_t;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void delayInit( delay_t * delay, tick_t duration );
bool_t delayRead( delay_t * delay );
void delayWrite( delay_t * delay, tick_t duration );

#endif /* API_API_DELAY_H_ */
