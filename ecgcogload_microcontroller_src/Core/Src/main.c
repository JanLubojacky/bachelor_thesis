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
#include <stdio.h>
#include <math.h>
#include "input_parser.h"
#include "ecg.h"
#include "ecg_data.h"
#include "ecg_processing.h"
// #include "ai_functions.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/*
 *  Define static parameters
 */
#define UPSAMPLING_FREQUENCY 500 // what is received
#define NUMBER_OF_INPUTS 2500 // window length (5 s) * UPSAMPLING_FREQUENCY
#define send_signal 0 // if set to 1, processed input signal is transmitted over the serial line to PC,
#define time_the_execution 0 // if set to 1, execution time is sent to the PC

#define BATCH_OF_INPUTS 250 // 2 * number_of_inputs % BATCH_OF_INPUTS == 0
#define MAX_MESSAGE_LENGTH 10 // depends on num of characters of input messages, set by the ECG simulator
#define RECIEVE_BUFFER_SZ 5001 // 2 * number_of_inputs + 1
#define RECIEVE_BATCH_SZ_FIRST_HALF 2500 // recieve_buffer_sz / 2, first half of the double buffer
#define RECIEVE_BATCH_SZ_SECOND_HALF 5000 // recieve_buffer_sz, second half of the double buffer
#define FIRST_HALF 0
#define SECOND_HALF 2500 // recieve_buffer_sz / 2

#define SAMPLING_FREQUENCY 100 // what the neural network expects
#define OK 0
#define EMPTY 0
#define ECG_BATCHES 1
#define READY 1
#define NOT_READY 0

/********COMMUNICATION SECTION BEGIN********/
uint8_t UART_RECIEVE_BUFFER[RECIEVE_BUFFER_SZ] = {0};

int UART_RECIEVE_BUFFER_SIZE = sizeof(UART_RECIEVE_BUFFER) - 1;

float PROCESSED_INPUTS[NUMBER_OF_INPUTS];

uint16_t timer_val;
int write_idx = 0;
char buf[50];
int buf_len = 0;
double prediction;
int input_ready = NOT_READY;
/********COMMUNICATION SECTION END********/

/********AI SECTION BEGIN********/
static ai_handle ecg = AI_HANDLE_NULL;
AI_ALIGNED(32)
static ai_u8 activations[AI_ECG_DATA_ACTIVATIONS_SIZE];

/* Data payload for input tensor */
#if defined(AI_NETWORK_INPUTS_IN_ACTIVATIONS)
static ai_float in_data;
#else
  /* Buffer should be allocated by the application
     in this case: input->data == NULL */
AI_ALIGNED(32)
static ai_float in_data[AI_ECG_IN_1_SIZE];
#endif

/* Data payload for the output tensor */
AI_ALIGNED(32)
static ai_float out_data[AI_ECG_OUT_1_SIZE];
/********AI SECTION END********/


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CRC_HandleTypeDef hcrc;

TIM_HandleTypeDef htim16;

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_DMA_Init(void);
static void MX_CRC_Init(void);
static void MX_TIM16_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/********AI FUNCTIONS BEGIN********/
int ai_init(void) {
  ai_error err;
  char buf[50];
  int buf_len = 0;

  /* 1 - Create an instance of the model */
  err = ai_ecg_create(&ecg, AI_ECG_DATA_CONFIG /* or NULL */);
  if (err.type != AI_ERROR_NONE) {
    printf("ERROR: ai_ecg_create type=%d code=%d!\r\n", err.type, err.code);
    return -1;
    };

  /* 2 - Initialize the instance */
  const ai_network_params params = AI_NETWORK_PARAMS_INIT(
      AI_ECG_DATA_WEIGHTS(ai_ecg_data_weights_get()),
      AI_ECG_DATA_ACTIVATIONS(activations)
  );

  if (!ai_ecg_init(ecg, &params)) {
      err = ai_ecg_get_error(ecg);
      buf_len = sprintf(buf, "ERROR: ai_ecg_init type=%d code=%d!\r\n", err.type, err.code);
      HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_len, 100);
      return -1;
    }

  return 0;
}

int ai_run(const void *in_data, void *out_data)
{
	char buf[100];
	int buf_len = 0;
	ai_i32 n_batch;
	ai_error err;

	/* 1 - Create the AI buffer IO handlers with the default definition */
	ai_buffer ai_input[AI_ECG_IN_NUM] = AI_ECG_IN ;
	ai_buffer ai_output[AI_ECG_OUT_NUM] = AI_ECG_OUT ;

	/* 2 - Update IO handlers with the data payload */
	ai_input[0].n_batches = ECG_BATCHES;
	ai_input[0].data = AI_HANDLE_PTR(in_data);
	ai_output[0].n_batches = ECG_BATCHES;
	ai_output[0].data = AI_HANDLE_PTR(out_data);

	/* 3 - Perform the inference */
	n_batch = ai_ecg_run(ecg, &ai_input[0], &ai_output[0]);
	if (n_batch != 1) {
		err = ai_ecg_get_error(ecg);

		buf_len = sprintf(buf, "ERROR: ai_ecg_run type=%d code=%d!\r\n", err.type, err.code);
		HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_len, 100);
		return -1;
  };

  return 0;
}

float perform_inference(void){
	// fill input tensor
	standardization(PROCESSED_INPUTS, NUMBER_OF_INPUTS);
	filt_and_downsample(in_data, PROCESSED_INPUTS, NUMBER_OF_INPUTS, UPSAMPLING_FREQUENCY, SAMPLING_FREQUENCY);
	min_max_normalization(in_data, AI_ECG_IN_1_SIZE);

	if (send_signal == True){
		for(int i = 0; i<AI_ECG_IN_1_SIZE; i++){
					buf_len = sprintf(buf, "n%f\n", in_data[i]);
					HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_len, 5);
				}
		buf_len = sprintf(buf, "sig_end\n");
		HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_len, 1);
	}

//	timer_val = __HAL_TIM_GET_COUNTER(&htim16); // optionally we can time the inference
	// run inference
	if(ai_run(in_data, out_data) != OK){
		Error_Handler();
	}

//	timer_val = __HAL_TIM_GET_COUNTER(&htim16) - timer_val;
//	buf_len = sprintf(buf, "exec_time: %u us\n", timer_val);
//	HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_len, 100);

	// return output
	prediction = ((float *)out_data)[0];

	return prediction;
}
/********AI FUNCTIONS END*******/
/********COMMUNICATIONS FUNCTIONS BEGIN********/

// copy lower half of the buffer while DMA writes into the upper part
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
	// check that lower half has correct amount of numbers
	int numbers_recieved = numbers_count(UART_RECIEVE_BUFFER, RECIEVE_BATCH_SZ_FIRST_HALF, FIRST_HALF);

	if(numbers_recieved != BATCH_OF_INPUTS){
			uint8_t error_message[] = "\r\nERROR: RECIEVED DIFFERENT NUMBER OF INPUTS!\r\n";
			HAL_UART_Transmit(&huart2, error_message, sizeof(error_message), 100);
			Error_Handler();
		}
	// If transmission success, perform parsing
	write_idx = parse_input_float(UART_RECIEVE_BUFFER, UART_RECIEVE_BUFFER_SIZE/2, FIRST_HALF, PROCESSED_INPUTS, write_idx);
}

// copy upper half of the buffer and flip the inference flag
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
  {
	int numbers_recieved = numbers_count(UART_RECIEVE_BUFFER, RECIEVE_BATCH_SZ_SECOND_HALF, SECOND_HALF);

	if(numbers_recieved != BATCH_OF_INPUTS){
			uint8_t error_message[] = "\r\nERROR: RECIEVED DIFFERENT NUMBER OF INPUTS!\r\n";
			HAL_UART_Transmit(&huart2, error_message, sizeof(error_message), 100);
			Error_Handler();
		}

	// If transmission success, perform parsing
	write_idx = parse_input_float(UART_RECIEVE_BUFFER, UART_RECIEVE_BUFFER_SIZE/2, SECOND_HALF, PROCESSED_INPUTS, write_idx);

	// If entire window received
	// ready for inference
	// flip the flag
	if (write_idx == NUMBER_OF_INPUTS){
		write_idx = 0;
		input_ready = READY;
	}

  }

/********COMMUNICATIONS FUNCTIONS END********/

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
  MX_USART2_UART_Init();
  MX_CRC_Init();
  MX_TIM16_Init();
  /* USER CODE BEGIN 2 */
	/* The STM32 CRC IP clock should be enabled to use the network runtime library */
	__HAL_RCC_CRC_CLK_ENABLE();

	ai_init();
	HAL_UART_Receive_DMA(&huart2, UART_RECIEVE_BUFFER, UART_RECIEVE_BUFFER_SIZE);

	ai_network_report report;
	ai_ecg_get_info(ecg, &report);

	#if defined(AI_NETWORK_INPUTS_IN_ACTIVATIONS)
	  /* Set the descriptor of the first input tensor (index 0) */
	  const ai_buffer *input = &report.inputs[0];
	  /* Retrieve the @ of the input buffer */
	  in_data = (ai_float *)input->data;
	#endif

	for(int i = 0;i < 4;i++){
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		HAL_Delay(300);
	}

	HAL_TIM_Base_Start(&htim16);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  // check for inference flag
	if(input_ready == READY){
		if(time_the_execution){
			timer_val = __HAL_TIM_GET_COUNTER(&htim16); // start the timer
		}

		input_ready = NOT_READY;

		float y_val = perform_inference();

		// Detect rpeaks and calculate ecg parameters
		r_peak_detection(in_data, AI_ECG_IN_1_SIZE, SAMPLING_FREQUENCY, huart2);

		// Send inference result
		buf_len = sprintf(buf, "prediction: %.2f\n", y_val);
		HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_len, 100);

		if(time_the_execution){
			// Optionally time the execution speed
					timer_val = __HAL_TIM_GET_COUNTER(&htim16) - timer_val;
					buf_len = sprintf(buf, "exec_time: %u us\n", timer_val);
					HAL_UART_Transmit(&huart2, (uint8_t *)buf, buf_len, 100);
		}



	  }
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_TIM16;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Tim16ClockSelection = RCC_TIM16CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief TIM16 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM16_Init(void)
{

  /* USER CODE BEGIN TIM16_Init 0 */

  /* USER CODE END TIM16_Init 0 */

  /* USER CODE BEGIN TIM16_Init 1 */

  /* USER CODE END TIM16_Init 1 */
  htim16.Instance = TIM16;
  htim16.Init.Prescaler = 7999;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = 65535;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 0;
  htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM16_Init 2 */

  /* USER CODE END TIM16_Init 2 */

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
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
  /* DMA1_Channel7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);

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
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

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
  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
  HAL_Delay(100);

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
