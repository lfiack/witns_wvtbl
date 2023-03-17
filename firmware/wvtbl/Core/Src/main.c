/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>

#include "drv/led_pwm.h"
#include "drv/encoder.h"
#include "drv/analog.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
// typedef enum adc_index_enum
// {
//     X_PARAM_INDEX, 
//     Y_PARAM_INDEX, 
//     X_ATTV_INDEX,
//     X_INPUT_INDEX,
//     Y_ATTV_INDEX,
//     Y_INPUT_INDEX,
//     PITCH_INPUT_INDEX,
//     UNUSED_INDEX,
//     ADC_BUFFER_LENGTH 
// } adc_index_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TIM_CHANNEL_BLUE TIM_CHANNEL_1
#define TIM_CHANNEL_GREEN TIM_CHANNEL_2
#define TIM_CHANNEL_RED TIM_CHANNEL_3

//#define ADC_BUFFER_LENGTH_WORDS (ADC_BUFFER_LENGTH/2)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
h_led_pwm_t h_led_red = {
  .timer_handle = &htim1,
  .timer_channel = TIM_CHANNEL_RED
};

h_led_pwm_t h_led_green = {
  .timer_handle = &htim1,
  .timer_channel = TIM_CHANNEL_GREEN
};

h_led_pwm_t h_led_blue = {
  .timer_handle = &htim1,
  .timer_channel = TIM_CHANNEL_BLUE
};

h_encoder_t h_encoder = {
  .A_GPIOx = ENCODER_A_GPIO_Port,
  .A_GPIO_pin = ENCODER_A_Pin,
  .B_GPIOx = ENCODER_B_GPIO_Port,
  .B_GPIO_pin = ENCODER_B_Pin
};

h_analog_t h_analog = {
  .adc_handles = {&hadc1, &hadc2},
  .timer_handle = &htim6,
  .dac_handle = &hdac1,
  .dac_channel = DAC_CHANNEL_1
};

// static int32_t counter = 0;
static int32_t counter_SW = 0;

static int32_t old_counter_SW = 0;

//static uint16_t adc_buffer[ADC_BUFFER_LENGTH] = {0};
static uint16_t potentiometers[4] = {0};
static uint16_t old_potentiometers[4] = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int _write(int file, char *ptr, int len)
{
	(void) file;

	HAL_UART_Transmit(&huart1, (uint8_t *)ptr, (uint16_t) len, HAL_MAX_DELAY);

	return len;
}

// void read_encoder(void)
// {
// 	static GPIO_PinState state_A = GPIO_PIN_SET;

// 	switch (state_A)
// 	{
// 	case GPIO_PIN_SET:
// 		if (HAL_GPIO_ReadPin(ENCODER_A_GPIO_Port, ENCODER_A_Pin) == GPIO_PIN_RESET)
// 		{
// 			state_A = GPIO_PIN_RESET;
// 			if (HAL_GPIO_ReadPin(ENCODER_B_GPIO_Port, ENCODER_B_Pin) == GPIO_PIN_SET)
// 			{
// 				counter++;
// 			}
// 			else
// 			{
// 				counter--;
// 			}
// 		}

// 		break;
// 	case GPIO_PIN_RESET:
// 		if (HAL_GPIO_ReadPin(ENCODER_A_GPIO_Port, ENCODER_A_Pin) == GPIO_PIN_SET)
// 		{
// 			state_A = GPIO_PIN_SET;
// 		}
// 		break;
// 	default:
// 		break;
// 	}
// }

void read_switch(void)
{
	static uint8_t state_SW = 0;

	if (state_SW == 0)
	{
		if (HAL_GPIO_ReadPin(ENCODER_SWITCH_GPIO_Port, ENCODER_SWITCH_Pin) == GPIO_PIN_SET)
		{
			counter_SW++;

//      counter = 0; // Reset the encoder counter

			state_SW = 1;
		}
	}
	else if (state_SW == 1)
	{
		if (HAL_GPIO_ReadPin(ENCODER_SWITCH_GPIO_Port, ENCODER_SWITCH_Pin) == GPIO_PIN_RESET)
		{
			state_SW = 0;
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (TIM7 == htim->Instance)
	{
    encoder_process(&h_encoder);
		// read_encoder();
		read_switch();
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  if (ADC1 == hadc->Instance)
  {
    HAL_GPIO_WritePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin, GPIO_PIN_SET);

    potentiometers[0] = 4095 - analog_get_adc(&h_analog, ANALOG_ADC_X_PARAM_INDEX);
    potentiometers[1] = 4095 - analog_get_adc(&h_analog, ANALOG_ADC_Y_PARAM_INDEX);
    potentiometers[2] = 4095 - analog_get_adc(&h_analog, ANALOG_ADC_X_ATTV_INDEX);
    potentiometers[3] = 4095 - analog_get_adc(&h_analog, ANALOG_ADC_Y_ATTV_INDEX);

    int32_t counter = encoder_value(&h_encoder);
    uint16_t red = counter;
    if (counter > 255) red = 255;
    else if (counter < 0) red = 0;
    
    uint16_t green = potentiometers[0] >> 4;
    uint16_t blue = potentiometers[1] >> 4;

    led_pwm_set_brightness(&h_led_red, (float)(red)/255.f);
    led_pwm_set_brightness(&h_led_green, (float)(green)/255.f);
    led_pwm_set_brightness(&h_led_blue, (float)(blue)/255.f);

    // uint32_t dac = adc_buffer[PITCH_INPUT_INDEX];
    // HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac);
    uint16_t dac = analog_get_adc(&h_analog, ANALOG_ADC_PITCH_INPUT_INDEX);
    analog_set_dac(&h_analog, dac);

    HAL_GPIO_WritePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin, GPIO_PIN_RESET);
  }

}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_DAC1_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
  printf("===== WVTBL =====\r\n");

  // if (HAL_OK != HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED))
  // {
  //   Error_Handler();
  // }
  // if (HAL_OK != HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED))
  // {
  //   Error_Handler();
  // }

  // if (HAL_OK != HAL_ADCEx_MultiModeStart_DMA(&hadc1, (uint32_t *)adc_buffer, ADC_BUFFER_LENGTH_WORDS)) // Length = 4(channels ADC1) * 4(bytes) = 16?
  // {
  //   Error_Handler();
  // }
  // if (HAL_OK != HAL_TIM_Base_Start(&htim6))
  // {
  //   Error_Handler();
  // }

  // We want to be able to calibrate the v/oct input
  // If encoder is pressed, enter calibration mode
  // Blink one color (eg. blue) while calibrating, say 1V (C2)
  // Waits second encoder press
  // We might light green LED for 1 or 2 seconds to send feedback to the user
  // Blink another color (eg. red) while calibrating, say 3V (C4)

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  encoder_init(&h_encoder);
  
  if (0 != led_pwm_init(&h_led_red))
  {
    Error_Handler();
  }
  if (0 != led_pwm_init(&h_led_green))
  {
    Error_Handler();
  }
  if (0 != led_pwm_init(&h_led_blue))
  {
    Error_Handler();
  }

  if (0 != analog_init(&h_analog))
  {
    Error_Handler();
  }
  
//  HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
	if (HAL_OK != HAL_TIM_Base_Start_IT(&htim7))
  {
    Error_Handler();
  }

  while (1)
  {
#ifdef DEBUG_ENCODER
    static int32_t value = 0;

    if (h_encoder.value != value)
    {
      value = h_encoder.value;
      printf("Encoder = %ld\r\n", value);
    }
#endif // DEBUG_ENCODER

		// if (counter_SW != old_counter_SW)
		// {
		// 	old_counter_SW = counter_SW;
		// 	printf("SW=%ld\r\n", counter_SW);
		// }

    // for (int it = 0 ; it < 4 ; it++)
    // {
    //   int diff = potentiometers[it] - old_potentiometers[it];
    //   if ((diff > 10) || (diff < -10))
    //   {
    //     old_potentiometers[it] = potentiometers[it];

    //     printf("Pot[%d] = %u\r\n", it, potentiometers[it]);
    //   }
    // }
    // If the interrupt flag is available
    // Get ADC values from params (pots) and inputs (Audio/CV) <- maybe in interruption
    // dsp_process() : reads params and inputs, generates outputs and lights
    // Display lights and sends outputs to DAC
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();

  HAL_GPIO_WritePin(LED_ERROR_GPIO_Port, LED_ERROR_Pin, GPIO_PIN_SET);

  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
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
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
