/**
 ******************************************************************************
 * @file	API_i2c.h
 * @author	Gabriel Postolov
 * @brief	API i2c
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef API_INC_API_I2C_H_
#define API_INC_API_I2C_H_

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdbool.h>

/* Exported types ------------------------------------------------------------*/
typedef enum {
	NO_ALARM_,
	MAX_TEMP_ABS_,
	MAX_TEMP_DIFF_,
	MAX_HUM_DIFF_,
} alarmType_t;
/* Exported constants --------------------------------------------------------*/
/* Definition for I2Cx clock resources */
#define I2Cx                             I2C1
#define I2Cx_CLK_ENABLE()                __HAL_RCC_I2C1_CLK_ENABLE()
#define I2Cx_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Cx_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()

#define I2Cx_FORCE_RESET()               __HAL_RCC_I2C1_FORCE_RESET()
#define I2Cx_RELEASE_RESET()             __HAL_RCC_I2C1_RELEASE_RESET()

/* Definition for I2Cx Pins */
#define I2Cx_SCL_PIN                    GPIO_PIN_6
#define I2Cx_SCL_GPIO_PORT              GPIOB
#define I2Cx_SCL_AF                     GPIO_AF4_I2C1
#define I2Cx_SDA_PIN                    GPIO_PIN_7
#define I2Cx_SDA_GPIO_PORT              GPIOB
#define I2Cx_SDA_AF                     GPIO_AF4_I2C1

/* Size of Transmission buffer */
#define TXBUFFERSIZE					20				//< TXBUFFERSIZE MAX: 64
/* Size of Reception buffer */
#define RXBUFFERSIZE					TXBUFFERSIZE	//< RXBUFFERSIZE MAX: 64

/* Slave DevAddress */
#define SLAVE_DEV_ADDRESS				0x44

/* When this temperature is reached the alarm is enabled */
#define MAX_TEMP_ABS					50
/* When this temperature difference is reached in less than 3 minutes the alarm is enabled */
#define MAX_TEMP_DIFF					5
/* When this humidity difference is reached in less than 3 minutes the alarm is enabled */
#define MAX_HUM_DIFF					20

/* Timeout for I2C */
#define I2C_TXRX_TIMEOUT				100

/* Timeout for reading Temperature and Humidity */
#define DELAY_READ_TH_DURATION			2000 //20000

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
bool i2c_init(void);
alarmType_t alarmTH_FSM_update(void);
bool TH_updated(uint32_t *, uint32_t *);
#endif /* API_INC_API_I2C_H_ */

/***************************************************************END OF FILE****/
