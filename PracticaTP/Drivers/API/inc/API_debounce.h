/**
 ******************************************************************************
 * @file	API_debounce.h
 * @author Gabriel Postolov
 * @brief	API debounce
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void debounceFSM_init(void);
void debounceFSM_update(void);
bool readKey(void);
#endif /* API_INC_API_DEBOUNCE_H_ */

/***************************************************************END OF FILE****/
