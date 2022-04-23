/**
 ******************************************************************************
 * @file	main.c
 * @author	Gabriel Postolov
 * @brief	Main File
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */

#include "main.h"
#include "API_debounce.h"
#include "API_delay.h"
#include "API_uart.h"
#include "API_i2c.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DELAY_LED3_DURATION 200
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static delay_t delay_LED3;

bool buttonPressed_current = false;
bool buttonPressed_old = false;

static const char * UART_CONFIG_OK = " \n\r ** UART Configuration finished successfully. ** \n\n\r ";
static const char * I2C_CONFIG_OK = " \n\r ** I2C Configuration finished successfully. ** \n\n\r ";
static const char * I2C_RESET_ALARM = " \n\r ** The alarm was reset. ** \n\n\r ";
static const char * I2C_MAX_TEMP_ABS_ALARM = " \n\r ** Alarm: Maximum absolute temperature reached. ** \n\n\r ";
static const char * I2C_MAX_TEMP_DIFF_ALARM = " \n\r ** Alarm: Maximum difference temperature reached. ** \n\n\r ";
static const char * I2C_MAX_HUM_DIFF_ALARM = " \n\r ** Alarm: Maximum difference temperature reached. ** \n\n\r ";

static char str_to_send_t[40] = "Current Temperature: ";
static char str_to_send_h[40] = "Current Humidity: ";
static char str_aux[10];

static alarmType_t my_alarm = NO_ALARM_;
static alarmType_t my_old_alarm = NO_ALARM_;

static uint32_t my_temp = 0;
static uint32_t my_hum = 0;

/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
static void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */

int main(void) {
	/* STM32F4xx HAL library initialization:
	 - Configure the Flash prefetch
	 - Systick timer is configured by default as source of time base, but user
	 can eventually implement his proper time base source (a general purpose
	 timer for example or other time source), keeping in mind that Time base
	 duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
	 handled in milliseconds basis.
	 - Low Level Initialization
	 */

	/* Initialize HAL */
	HAL_Init();

	/* Configure the system clock to 180 MHz */
	SystemClock_Config();

	/* Initialize BSP Led for LED1,LED2,LED3 */
	BSP_LED_Init(LED1);
	BSP_LED_Init(LED2);
	BSP_LED_Init(LED3);

	/* Initialize delay for LED2 */
	delayInit(&delay_LED3, DELAY_LED3_DURATION);

	if(uartinit()){
		uartsendString((uint8_t *)UART_CONFIG_OK);
	}else{
		/* Initialization Error */
		Error_Handler();
	}

	if(i2c_init()){
		uartsendString((uint8_t *)I2C_CONFIG_OK);
	}else{
		/* Initialization Error */
		Error_Handler();
	}

	/* Infinite loop */
	while (1) {
		debounceFSM_update();
		my_alarm = alarmTH_FSM_update();
		if(TH_updated(&my_temp, &my_hum)){
			sprintf(str_aux, "%u", (unsigned int)my_temp);
			strcat( str_to_send_t, str_aux );
			uartsendString((uint8_t *)str_to_send_t);

			sprintf(str_aux, "%u", (unsigned int)my_hum);
			strcat( str_to_send_h, str_aux );
			uartsendString((uint8_t *)str_to_send_h);
		}

		if(my_alarm == NO_ALARM_){
			BSP_LED_Off(LED3);
		}else{
			if (delayRead(&delay_LED3)) {
				BSP_LED_Toggle(LED3);
			}
		}

		if(my_alarm != my_old_alarm){
			my_old_alarm = my_alarm;
			switch (my_alarm) {
				case NO_ALARM_:
					uartsendString((uint8_t *)I2C_RESET_ALARM);
					break;
				case MAX_TEMP_ABS_:
					uartsendString((uint8_t *)I2C_MAX_TEMP_ABS_ALARM);
					break;
				case MAX_TEMP_DIFF_:
					uartsendString((uint8_t *)I2C_MAX_TEMP_DIFF_ALARM);
					break;
				case MAX_HUM_DIFF_:
					uartsendString((uint8_t *)I2C_MAX_HUM_DIFF_ALARM);
					break;
				default:
					break;
			}
		}
	}
}

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 180000000
 *            HCLK(Hz)                       = 180000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 4
 *            APB2 Prescaler                 = 2
 *            HSE Frequency(Hz)              = 8000000
 *            PLL_M                          = 8
 *            PLL_N                          = 360
 *            PLL_P                          = 2
 *            PLL_Q                          = 7
 *            PLL_R                          = 2
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale1 mode
 *            Flash Latency(WS)              = 5
 * @param  None
 * @retval None
 */
static void SystemClock_Config(void) {
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();

	/* The voltage scaling allows optimizing the power consumption when the device is
	 clocked below the maximum system frequency, to update the voltage scaling value
	 regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 360;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		/* Initialization Error */
		Error_Handler();
	}

	if (HAL_PWREx_EnableOverDrive() != HAL_OK) {
		/* Initialization Error */
		Error_Handler();
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
		/* Initialization Error */
		Error_Handler();
	}
}
/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
static void Error_Handler(void) {
	/* Turn LED2 on */
	BSP_LED_On(LED2);
	while (1) {
	}
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
	  printf("Wrong parameters value: file %s on line %d\r\n", file, line);
  }
}
#endif

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
