/**
 ******************************************************************************
 * @file    API_hcsr04_port.c
 * @author  Juan Manuel Hernández
 * @brief   Implementation of API_hcsr04_port
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "API_hcsr04_port.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
TIM_HandleTypeDef htim1;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
#define CHECK_TIM(x)                    do { \
                                            if (x != HAL_OK) { return HCSR04_PORT_ERROR_HAL; } \
                                        } while(0U)

/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim1.Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
        hcsr04_ICCaptureCallback();
    }
}

/**
  * @brief This function handles TIM1 capture compare interrupt.
  */
void TIM1_CC_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim1);
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

    if(htim1.Instance == hcsr04_TIMER) {
        /* Peripheral clock enable */
        hcsr04_TIMER_CLK_ENABLE();

        hcsr04_ECHO_GPIO_CLK_ENABLE();
        
        /** TIM1 GPIO Configuration **/
        GPIO_InitStruct.Pin = hcsr04_ECHO_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
        HAL_GPIO_Init(hcsr04_ECHO_GPIO_PORT, &GPIO_InitStruct);

        /* TIM1 interrupt Init */
        HAL_NVIC_SetPriority(hcsr04_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(hcsr04_IRQn);
    }
}

uint32_t myTIM_ReadCapturedValue()
{
    return HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_1);
}

void myTIM_SetCapturePolarityFalling()
{
    __HAL_TIM_SET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
}

void myTIM_SetCapturePolarityRising()
{
    __HAL_TIM_SET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
}

void myTIM_ResetCounter()
{
    __HAL_TIM_SET_COUNTER(&htim1, 0);
}

void myTIM_StartIT()
{
    HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);;
}

void myTIM_DisableIT()
{
    __HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC1);
}

void myTIM_EnableIT()
{
    __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC1);
}

void hcsr04_Delay(uint32_t time)
{
    __HAL_TIM_SET_COUNTER(&htim1, 0);
    while(__HAL_TIM_GET_COUNTER(&htim1) < time);
}

hcsr04Port_Status_t myHcsr04GPIO_Init()
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

    return HCSR04_PORT_OK;
}

void myHcsr04GPIO_SetTrig()
{
    HAL_GPIO_WritePin(hcsr04_TRIG_GPIO_PORT, hcsr04_TRIG_PIN, GPIO_PIN_SET);
}

void myHcsr04GPIO_ResetTrig()
{
    HAL_GPIO_WritePin(hcsr04_TRIG_GPIO_PORT, hcsr04_TRIG_PIN, GPIO_PIN_RESET);
}

hcsr04Port_Status_t myTIM_Init()
{
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_IC_InitTypeDef sConfigIC = {0};

    htim1.Instance = hcsr04_TIMER;
    htim1.Init.Prescaler = hcsr04_PREESCALER_180MHz;
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1.Init.Period = hcsr04_PERIOD_180MHz;
    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.RepetitionCounter = 0;
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    CHECK_TIM(HAL_TIM_IC_Init(&htim1));

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    CHECK_TIM(HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig));

    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0;
    CHECK_TIM(HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_1));

    return HCSR04_PORT_OK;
}