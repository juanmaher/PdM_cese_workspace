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

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/
void nRF24_IRQ_Callback(uint8_t event_type, uint16_t data_src, uint8_t* data, uint8_t width)
{
	switch(event_type) {
		case EVENT_RX_DR:
			BSP_LED_Toggle(LED2);
			break;
		case EVENT_TX_DS:
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
	BSP_LED_Init(LED1);
	BSP_LED_Init(LED2);

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
	hnrf24.DeviceMode = RECEIVER_MODE;

	if (nRF24_Init(&hnrf24) != NRF24_OK) {
		Error_Handler();
	}

	/* Infinite loop */
	while (1)
	{
		BSP_LED_Toggle(LED1);
		HAL_Delay(1000);
	}
}

static void SystemClock_Config(void)
{
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
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}

	if(HAL_PWREx_EnableOverDrive() != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
static void Error_Handler(void)
{
	/* Turn LED2 on */
	BSP_LED_On(LED2);
	while (1)
	{
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
	}
}
#endif
