/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */												//able to write2

  GPIO_PinState SW1_SwitchState[2];											//define variable that type is GPIO_PinState
  GPIO_PinState SW2_SwitchState[2];
  GPIO_PinState SW3_SwitchState[2];
  GPIO_PinState SW4_SwitchState[2];

  uint16_t LED1_Period = 2000;												//LED1 default frequency at 0.5Hz
  uint16_t LED4_Period = 0;													//LED4 default duty-cycle at 0%

  uint32_t TimeStamp_button = 0;
  uint32_t TimeStamp_LED1 = 0;												//time default at 0 s
  uint32_t TimeStamp_LED3 = 0;
  uint32_t TimeStamp_LED4 = 0;

  uint8_t LED3_State0 = 0;													//LED3 default at mode0

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */													//able to write3

	  //Button
	  if(HAL_GetTick()-TimeStamp_button >= 100)									//control settling time for debouncing switch
	  {
		  TimeStamp_button = HAL_GetTick();

		  //Task1
		  SW1_SwitchState[0] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10); 				//read SW1 from PA10

		  if((SW1_SwitchState[0]==1) && (SW1_SwitchState[1]==0))					//button is unpressed
		  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  			//GPIO_PIN_RESET is LOW, GPIO_PIN_SET is HIGH (enum)
		  {
			  switch(LED1_Period)
			  {
				case 2000:
					LED1_Period = 1000;												//change frequency from 0.5Hz to 1Hz
					break;

				case 1000:
					LED1_Period = 500;												//change frequency from 1Hz to 2Hz
					break;

				case 500:
					LED1_Period = 333;												//change frequency from 2Hz to 3Hz
					break;

				case 333:
					LED1_Period = 2000;												//change frequency from 3Hz to 0.5Hz
					break;

				default:
					break;
			  }
		  }
		  SW1_SwitchState[1] = SW1_SwitchState[0];									//save SW1 history

		  //Task2
		  SW2_SwitchState[0] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3);					//read SW2 from PB3

		  if((SW2_SwitchState[0]==1) && (SW2_SwitchState[1]==0))					//button is unpressed
		  {
			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7,
					  !(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7)));						//switch state LED D3
		  }
		  SW2_SwitchState[1] = SW2_SwitchState[0];									//save SW2 history

		  //Task3
		  SW3_SwitchState[0] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);					//read SW3 from PB5

		  if((SW3_SwitchState[0]==1) && (SW3_SwitchState[1]==0))					//button is unpressed
		  {
			  LED3_State0 = !LED3_State0;
		  }
		  SW3_SwitchState[1] = SW3_SwitchState[0];									//save SW3 history

		  //Task4 (Challenge)
		  SW4_SwitchState[0] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);					//read SW4 from PB4

		  if((SW4_SwitchState[0]==1) && (SW4_SwitchState[1]==0))					//button is unpressed
		  {
			  LED4_Period++;
			  LED4_Period = LED4_Period%5;
		  }
		  SW4_SwitchState[1] = SW4_SwitchState[0];									//save SW4 history
	  }

	  //Time

	  //Task1
	  if(HAL_GetTick()-TimeStamp_LED1 >= LED1_Period/2)							//time in millisecond
	  {
		  TimeStamp_LED1 = HAL_GetTick();

		  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == GPIO_PIN_SET)			//blink LED D1 at PA9
		  {
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
		  } else {
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
		  }
	  }

	  //Task3
	  if(!LED3_State0)														//blink LED D5 at PB6
	  {																		//ON 0.5s and OFF 1.5s
	  	  if((HAL_GetTick()-TimeStamp_LED3 >= 500) && (!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)))
	  	  {
	  		  TimeStamp_LED3 = HAL_GetTick();
	  		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
	  	  }
	  	  else if((HAL_GetTick()-TimeStamp_LED3 >= 1500) && (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)))
	  	  {
	  		  TimeStamp_LED3 = HAL_GetTick();
	  		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
	  	  }
	  }
	  else																	//ON 1.5s and OFF 0.5s
	  {
		  if((HAL_GetTick()-TimeStamp_LED3 >= 500) && (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)))
		  {
		  	  TimeStamp_LED3 = HAL_GetTick();
		  	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
		  }
		  else if((HAL_GetTick()-TimeStamp_LED3 >= 1500) && (!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)))
		  {
		  	  TimeStamp_LED3 = HAL_GetTick();
		  	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
		  }
	  }

	  //Task4 (Challenge)
	  //
	  //try period at 4 millisecond => frequency at 250 Hz
	  //
	  if((HAL_GetTick()-TimeStamp_LED4 >= LED4_Period) && (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7)))	//blink LED D7 at PA7
	  {
		  TimeStamp_LED4 = HAL_GetTick();
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
	  }
	  else if((HAL_GetTick()-TimeStamp_LED4 >= 4-LED4_Period) && (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7)))
	  {
		  TimeStamp_LED4 = HAL_GetTick();
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
	  }
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|GPIO_PIN_7|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin PA7 PA9 */
  GPIO_InitStruct.Pin = LD2_Pin|GPIO_PIN_7|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PC7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB3 PB4 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
