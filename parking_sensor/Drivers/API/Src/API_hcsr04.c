/**
 ******************************************************************************
 * @file    .c
 * @author  Juan Manuel Hernández
 * @brief   Implementation of
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "API_hcsr04.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define CHECK_NULL(x)                   do { \
                                            if (x == NULL) { return HCSR04_ERROR_NULL; } \
                                        } while(0U)

#define CHECK_HAL(x)                    do { \
                                            if (x != HAL_OK) { return HCSR04_ERROR_HAL; } \
                                        } while(0U)

#define CHECK_INTERNAL(x)               do { \
                                            if (x != HCSR04_OK) { return x; } \
                                        } while(0U)

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;

/* Private function prototypes -----------------------------------------------*/
hcsr04_Status_t myTIM_Init();
hcsr04_Status_t myGPIO_Init();

/* Private functions ---------------------------------------------------------*/
uint32_t IC_Val1 = 0;
uint32_t IC_Val2 = 0;
uint32_t Difference = 0;
uint8_t Is_First_Captured = 0;  // is the first value captured ?
uint8_t Distance  = 0;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	uint8_t partial = 0;
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)  // if the interrupt source is channel1
	{
		if (Is_First_Captured==0) // if the first value is not captured
		{
			IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // read the first value
			Is_First_Captured = 1;  // set the first captured as true
			// Now change the polarity to falling edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
		}

		else if (Is_First_Captured==1)   // if the first is already captured
		{
			IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // read second value
			__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

			if (IC_Val2 > IC_Val1)
			{
				Difference = IC_Val2-IC_Val1;
			}

			else if (IC_Val1 > IC_Val2)
			{
				Difference = (0xffff - IC_Val1) + IC_Val2;
			}

			partial = Difference * .034/2;
			Distance = partial;

			Is_First_Captured = 0; // set it back to false

			// set polarity to rising edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC1);
		}
	}
}

/**
* @brief TIM_IC MSP Initialization
* This function configures the hardware resources used in this example
* @param htim_ic: TIM_IC handle pointer
* @retval None
*/
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef* htim_ic)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(htim_ic->Instance==hcsr04_TIMER)
    {
    /* USER CODE BEGIN TIM1_MspInit 0 */

    /* USER CODE END TIM1_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_TIM1_CLK_ENABLE();

        hcsr04_ECHO_GPIO_CLK_ENABLE();
        /**TIM1 GPIO Configuration
        PE9     ------> TIM1_CH1
        */
        GPIO_InitStruct.Pin = hcsr04_ECHO_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
        HAL_GPIO_Init(hcsr04_ECHO_GPIO_PORT, &GPIO_InitStruct);

        /* TIM1 interrupt Init */
        HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
    }
}

hcsr04_Status_t myTIM_Init()
{
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_IC_InitTypeDef sConfigIC = {0};

    htim1.Instance = hcsr04_TIMER;
    htim1.Init.Prescaler = 168-1;
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1.Init.Period = 0xffff-1;
    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.RepetitionCounter = 0;
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    CHECK_HAL(HAL_TIM_IC_Init(&htim1));

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    CHECK_HAL(HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig));

    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0;
    CHECK_HAL(HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_1));

    return HCSR04_OK;
}

hcsr04_Status_t myGPIO_Init()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    hcsr04_TRIG_GPIO_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(hcsr04_TRIG_GPIO_PORT, hcsr04_TRIG_PIN, GPIO_PIN_RESET);

    /*Configure TRIG GPIO pin : PE2 */
    GPIO_InitStruct.Pin = hcsr04_TRIG_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(hcsr04_TRIG_GPIO_PORT, &GPIO_InitStruct);

    return HCSR04_OK;
}

/**
  * @brief This function handles TIM1 capture compare interrupt.
  */
void TIM1_CC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_CC_IRQn 0 */

  /* USER CODE END TIM1_CC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_CC_IRQn 1 */

  /* USER CODE END TIM1_CC_IRQn 1 */
}

void delay(uint16_t time)
{
    __HAL_TIM_SET_COUNTER(&htim1, 0);
    while(__HAL_TIM_GET_COUNTER(&htim1) < time);
}

/* Public functions ----------------------------------------------------------*/
hcsr04_Status_t hcsr04_Init()
{
    myGPIO_Init();
    myTIM_Init();
    HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);
    return HCSR04_OK;
}

hcsr04_Status_t hcsr04_StartMeasure()
{
    HAL_GPIO_WritePin(hcsr04_TRIG_GPIO_PORT, hcsr04_TRIG_PIN, GPIO_PIN_SET);  // pull the TRIG pin HIGH
    delay(10);  // wait for 10 us
    HAL_GPIO_WritePin(hcsr04_TRIG_GPIO_PORT, hcsr04_TRIG_PIN, GPIO_PIN_RESET);  // pull the TRIG pin low

    __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC1);
    return HCSR04_OK;
}

uint8_t hcsr04_GetDistance()
{
    return Distance;
}