/**
 ******************************************************************************
 * @file    Src/main.c
 * @author  Juan Manuel Hernández
 * @brief   Main program body
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

nRF24_HandleTypeDef hnrf24;
bool_t can_transmit = true;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

/**
 * @brief  BSP Push Button callback
 * @param  Button Specifies the pin connected EXTI line
 * @retval None.
 */
void BSP_PB_Callback(Button_TypeDef Button)
{
  static int count = 0;
  count++;
}

/* Private functions ---------------------------------------------------------*/
void nRF24_IRQ_Callback(uint8_t event_type, uint16_t data_src, uint8_t* data, uint8_t width)
{
	switch(event_type) {
		case EVENT_RX_DR:
			//BSP_LED_On(LED2);
			break;
		case EVENT_TX_DS:
			can_transmit = true;
			break;
		case EVENT_MAX_RT:
			break;
		case EVENT_GPIO_IRQ:
			break;
	}
}

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void)
{
	/* STM32F4xx HAL library initialization */
	HAL_Init();

	/* Configure the system clock to 180 MHz */
	SystemClock_Config();

  /* Initialize BSP LED1 */
	//BSP_LED_Init(LED2);
  //BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

	/* CONFIG */
	hnrf24.Init.CrcEnable = 1;
	hnrf24.Init.CrcEncodingScheme = CRC_ENCODING_1_BYTE;
	/* EN_AA */
	hnrf24.Init.AutoAckEnable = 1;
	hnrf24.Init.AutoAckDataPipes = 0b00000011;
	/* EN_RXADDR */
	hnrf24.Init.RxDataPipes = 0b00000001;
	/* SETUP_AW */
	hnrf24.Init.Aw = AW_3_BYTES;
	/* RF_CH */
	hnrf24.Init.RfChannel = 0b00000010;
	/* RF_SETUP */
	hnrf24.Init.ContWave = 0;
	hnrf24.Init.RfPower = RF_POWER_0DBM;
	hnrf24.Init.RfDataRate = RF_DR_2MBPS;
	/* DPL */
	hnrf24.Init.DplEnable = 1;
	hnrf24.Init.DplEnableDataPipes = 0b00000011;
	/* EN_DYN_ACK */
	hnrf24.Init.DynAckEnable = 0;

	hnrf24.StatusRegister = 0;
	hnrf24.DeviceMode = TRANSMITTER_MODE;

	if (nRF24_Init(&hnrf24) != NRF24_OK) {
		Error_Handler();
	}
uint8_t send_buffer[33];
  int count=0;

  //nRF24_TxStatus_t tx_status = TRANSMITTION_INIT;

	/* Infinite loop */
	while (1)
	{
		if (can_transmit) {
			can_transmit = false;
			sprintf((char*)send_buffer, "0:abcdefgh%d", count++);
			count %=10000;
			nRF24_Transmit(send_buffer, strlen((char*)send_buffer));
		}

		HAL_Delay(100);

//		switch (tx_status) {
//		  case TRANSMITTION_INIT:
//		  // 30 3a 61 62 63 64 65 66 67 68 30
//			sprintf((char*)send_buffer, "0:abcdefgh%d", count++);
//			count %=10000;
//			nRF24_Transmit(send_buffer, strlen((char*)send_buffer));
//			nRF24_GetTransmitionStatus(&tx_status);
//			break;
//		  case TRANSMITTION_DONE:
//			HAL_Delay(10000);
//			tx_status = TRANSMITTION_INIT;
//			break;
//		  case TRANSMITTION_IN_PROGRESS:
//			nRF24_GetTransmitionStatus(&tx_status);
//			break;
//		  case TRANSMITTION_FAILED:
//			  HAL_Delay(10000);
//			  tx_status = TRANSMITTION_INIT;
//			break;
//		  default:
//			break;
//		}
//		HAL_Delay(1000);
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

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
  RCC_OscInitStruct.PLL.PLLQ = 7;
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

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
static void Error_Handler(void)
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
