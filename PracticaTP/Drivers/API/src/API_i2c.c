/**
 ******************************************************************************
 * @file	API_i2c.c
 * @author	Gabriel Postolov
 * @brief	API i2c
 ******************************************************************************
 */

//COMENTARIO: Se crean archivos separados y modularizados API
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */

//COMENTARIO: Se hace uso de la API_debounce y API_delay
#include "API_i2c.h"
#include "API_debounce.h"
#include "API_delay.h"
/* Private typedef -----------------------------------------------------------*/
typedef enum {
	READ,
	DATA_ANALYSIS,
	WAIT,
	ALARM,
} alarmState_t;

/* Private define ------------------------------------------------------------*/
//COMENTARIO: No se hardcodean valores
#define I2C_CLOCKSPEED_100K 100000U
#define I2C_OWN_ADDRESS1 0x01U
#define I2C_OWN_ADDRESS2 0xFFU
#define MAXBUFFERSIZE 64
#define ARRAY_TH 9
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef I2cHandle;
static uint8_t tx_buffer[TXBUFFERSIZE];
static uint8_t rx_buffer[RXBUFFERSIZE];
static alarmState_t current_state;
static alarmType_t current_alarm_state;
static uint32_t temperature;
static uint32_t humidity;
static uint8_t read_counter;
static uint32_t temperature_array[ARRAY_TH];
static uint32_t humidity_array[ARRAY_TH];
static bool new_data;
static delay_t delay_readTH;

/* Private function prototypes -----------------------------------------------*/
static bool i2c_config(void);
static alarmType_t dataAnalysis(void);
static bool readTH(uint32_t *, uint32_t *);
static void alarmTH_FSM_init(void);
/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Configuration of the I2C peripheral.
 * @param  None.
 * @retval true: The configuration was successful.
 * @retval false: The configuration was not successful.
 */
static bool i2c_config(void) { //COMENTARIO: Separo en funciones privadas y publicas
	I2cHandle.Instance = I2Cx;
	I2cHandle.Init.ClockSpeed = I2C_CLOCKSPEED_100K;
	I2cHandle.Init.OwnAddress1 = I2C_OWN_ADDRESS1;
	I2cHandle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	I2cHandle.Init.OwnAddress2 = I2C_OWN_ADDRESS2;
	I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	I2cHandle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	if (HAL_I2C_Init(&I2cHandle) != HAL_OK) {
		/* Configuration Error */
		// TODO Error_Handler();
		return false;
	}
	return true;
}


/**
 * @brief  It analyzes the data received and returns the status of the alarm.
 * @param  None.
 * @return Alarm status for received data.
 * @retval NO_ALARM_: No Alarm.
 * @retval MAX_TEMP_ABS_: Alarm. Maximum temperature was reached.
 * @retval MAX_TEMP_DIFF_: Alarm. Temperature difference was reached in less than 3 minutes.
 * @retval MAX_HUM_DIFF_: Alarm. Humidity difference was reached in less than 3 minutes.
 */
alarmType_t dataAnalysis(void){
	uint8_t i;
	uint32_t temp_diff = 0;
	uint32_t hum_diff = 0;

	if (temperature > MAX_TEMP_ABS) return MAX_TEMP_ABS_;

	if(read_counter>=ARRAY_TH) read_counter=0;
	temperature_array[read_counter]=temperature;
	humidity_array[read_counter]=humidity;
	read_counter++;

	for(i=0;i<ARRAY_TH;i++){
		temp_diff = (temperature>temperature_array[i])?(temperature-temperature_array[i]):(temperature_array[i]-temperature);
		if (temp_diff > MAX_TEMP_DIFF) return MAX_TEMP_DIFF_;
		hum_diff = (humidity>humidity_array[i])?(humidity-humidity_array[i]):(humidity_array[i]-humidity);
		if (hum_diff > MAX_HUM_DIFF) return MAX_HUM_DIFF_;
	}

	return NO_ALARM_;
}


/**
 * @brief  Read Temperature and Humidity by I2C
 * @param  temp: Temperature to be read.
 * @param  hum: Humidity to be read.
 * @retval true: The reading was successful..
 * @retval false: The reading was not successful.
 */
static bool readTH(uint32_t *temp, uint32_t *hum) {
	if(temp == NULL) return false;
	if(hum == NULL) return false;

	//Command to read from SHT3X-DIS
	tx_buffer[0] = 0x2C;
	tx_buffer[1] = 0x06;
	memset(rx_buffer, 0, RXBUFFERSIZE);

	if ((HAL_I2C_Master_Transmit(&I2cHandle, SLAVE_DEV_ADDRESS, tx_buffer, 2,
			I2C_TXRX_TIMEOUT)) != HAL_OK)
		return false;
	if ((HAL_I2C_Master_Receive(&I2cHandle, SLAVE_DEV_ADDRESS, rx_buffer, 6,
			I2C_TXRX_TIMEOUT)) != HAL_OK)
		return false;

	*temp = (uint32_t)(((rx_buffer[0]*256) + rx_buffer[1])*175)/65535.0-45.0;
	*hum =  (uint32_t)(((rx_buffer[3]*256) + rx_buffer[4]))*100.0/65535.0;
	return true;
}


/**
 * @brief  Initialize FSM (alarmTH_FSM).
 * @param  None.
 * @retval None.
 */
static void alarmTH_FSM_init(void) {
	uint8_t i;

	temperature = 0;
	humidity = 0;
	read_counter = 0;

	for(i=0; i<ARRAY_TH; i++){
		temperature_array[i]=0;
		humidity_array[i]=0;
	}

	current_state = READ;
	current_alarm_state = NO_ALARM_;
}


/* Public functions ----------------------------------------------------------*/
/**
 * @brief  Initialize the API_I2C: I2C peripheral, Button User, Buffers, FSM alarm.
 * @param  None.
 * @retval true: The initialization was successful.
 * @retval false: The initialization was not successful.
 */
bool i2c_init(void) {

	if (TXBUFFERSIZE > MAXBUFFERSIZE)
		return false;	//COMENTARIO: Chequeo valores
	if (RXBUFFERSIZE > MAXBUFFERSIZE)
		return false;
	memset(tx_buffer, 0, TXBUFFERSIZE); //COMENTARIO: Inicializo todas las variables que se van a usar.
	memset(rx_buffer, 0, RXBUFFERSIZE);

	alarmTH_FSM_init();

	new_data = false;

	/* Initialize BSP PB for BUTTON_USER */
	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

	debounceFSM_init();

	/* Initialize delay for read Temperature and Humidity */
	delayInit(&delay_readTH, DELAY_READ_TH_DURATION);

	return i2c_config();
	/* Initialization Error */
	// TODO Error_Handler();
}


/**
 * @brief  Update FSM.
 * @param  None.
 * @return Current alarm state.
 * @retval NO_ALARM_: No Alarm.
 * @retval MAX_TEMP_ABS_: Alarm. Maximum temperature was reached.
 * @retval MAX_TEMP_DIFF_: Alarm. Temperature difference was reached in less than 3 minutes.
 * @retval MAX_HUM_DIFF_: Alarm. Humidity difference was reached in less than 3 minutes.
 */
alarmType_t alarmTH_FSM_update(void) {
	switch (current_state) {
	case READ:
		if (readTH(&temperature,&humidity)) {
			new_data = true;
			current_state = DATA_ANALYSIS;
		}
		break;
	case DATA_ANALYSIS:
		current_alarm_state = dataAnalysis();
		if (current_alarm_state != NO_ALARM_) {
			current_state = ALARM;
		}else{
			current_state = WAIT;
			delayInit(&delay_readTH, DELAY_READ_TH_DURATION);
		}
		break;
	case WAIT:
		if (delayRead(&delay_readTH)) {
			current_state = READ;
		}
		break;
	case ALARM:
		if(readKey()){
			alarmTH_FSM_init();
		}
		break;
	default:
		alarmTH_FSM_init();
		break;
	}

	return current_alarm_state;

}


/**
 * @brief  Update Temperature and Humidity.
 * @param  temp: Temperature to be updated.
 * @param  hum: Humidity to be updated.
 * @retval true: Updated successful.
 * @retval false: Updated was not successful.
 */
bool TH_updated(uint32_t *temp, uint32_t *hum){
	if(temp == NULL) return false;
	if(hum == NULL) return false;

	if(new_data){
		new_data = false;
		*temp=temperature;
		*hum=humidity;
		return true;
	}

	return false;
}
