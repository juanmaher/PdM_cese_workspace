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
typedef enum {
    BUTTON_UP, // Button released
    BUTTON_FALLING, // Decreasing edge
    BUTTON_RISING, // Increasing edge
    BUTTON_DOWN // Button pushed
} debounceState_t;

/* Private define ------------------------------------------------------------*/
#define DELAY_DURATION_MS 40

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static delay_t delay;
static debounceState_t debounceState;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void debounceFSM_init();
static void debounceFSM_update();
static void buttonPressed();
static void buttonReleased();

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void)
{
	/* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
	 */
	HAL_Init();

	/* Configure the system clock to 180 MHz */
	SystemClock_Config();

	/* Initialize BSP LED1 */
	BSP_LED_Init(LED1);
	/* Initialize BSP PB for BUTTON_USER */
	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

	debounceFSM_init();

	/* Infinite loop */
	while (1)
	{
		debounceFSM_update();
	}
}

/**
 * @brief Turn on LED1 indicating that the button has been pressed.
 * @return void
*/
static void buttonPressed()
{
	BSP_LED_On(LED1);
}

/**
 * @brief Turn off LED1 indicating that the button has been released.
 * @return void
*/
static void buttonReleased()
{
	BSP_LED_Off(LED1);
}

/**
 * @brief Initialize the debounce FSM
 * @return void
*/
static void debounceFSM_init()
{
	debounceState = BUTTON_UP;
    delayInit(&delay, DELAY_DURATION_MS);
}

/**
 * @brief Return button state
 * @return bool_t TRUE o FALSE depending on the button state
*/
static void debounceFSM_update()
{
	switch (debounceState) {
		case BUTTON_UP:
			if (BSP_PB_GetState(BUTTON_USER)) {
				debounceState = BUTTON_RISING;
			}
			break;
		case BUTTON_FALLING:
			if (delayRead(&delay)) {
				if (BSP_PB_GetState(BUTTON_USER)) {
					debounceState = BUTTON_DOWN;
					buttonPressed();
				} else {
					debounceState = BUTTON_UP;
				}
			}
			break;
		case BUTTON_RISING:
			if (delayRead(&delay)) {
				if (BSP_PB_GetState(BUTTON_USER)) {
					debounceState = BUTTON_DOWN;
				} else {
					debounceState = BUTTON_UP;
					buttonReleased();
				}
			}
			break;
		case BUTTON_DOWN:
			if (!BSP_PB_GetState(BUTTON_USER)) {
				debounceState = BUTTON_FALLING;
			}
			break;
		default:
			break;
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

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
