/**
 ******************************************************************************
 * @file    Drivers/API/Src/API_uart.c
 * @author  Juan Manuel Hernández
 * @brief   Implementation of UART communication API layer.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "API_uart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DEFAULT_TIMEOUT 0xFFFF
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
#define CHECK_NULL(x)                   do { \
                                            if (x == NULL) { return UART_ERROR_NULL; } \
                                        } while(0U)

#define CHECK_HAL(x)                    do { \
                                            if (x != HAL_OK) { return UART_ERROR_HAL; } \
                                        } while(0U)

#define CHECK_INTERNAL(x)               do { \
                                            if (x != UART_OK) { return x; } \
                                        } while(0U)

/**
* @brief UART MSP Initialization
* This function configures the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(huart->Instance==USART2)
    {
        /* Peripheral clock enable */
        __HAL_RCC_USART2_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        /**USART2 GPIO Configuration
        PA3     ------> USART2_RX
        PD5     ------> USART2_TX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    }
}

/**
* @brief UART MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
    if(huart->Instance==USART2)
    {
        /* Peripheral clock disable */
        __HAL_RCC_USART2_CLK_DISABLE();

        /**USART2 GPIO Configuration
        PA3     ------> USART2_RX
        PD5     ------> USART2_TX
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_3);

        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_5);
    }

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static uart_Status_t myUART_Init(void)
{
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    CHECK_HAL(HAL_UART_Init(&huart2));

    return UART_OK;
}

/**
 * @brief  This function calculates the string length
 * @param  uint8_t* The string to calculate
 * @retval uint8_t The string length
 */
static uint8_t calculateStringLength(uint8_t * pString)
{
    if (pString == NULL)
    {
        return 0;
    }

    uint8_t i = 0;
    while (pString[i] != '\0')
    {
        i++;
    }
    return i;
}

/* Public functions ---------------------------------------------------------*/
uart_Status_t uart_Init()
{
    myUART_Init();
    return UART_OK;
}

uart_Status_t uart_SendString(uint8_t * pString)
{
    CHECK_NULL(pString);

    CHECK_HAL(HAL_UART_Transmit(&huart2, pString, calculateStringLength(pString), HAL_MAX_DELAY));
    return UART_OK;
}

uart_Status_t uart_SendStringSize(uint8_t * pString, uint16_t size)
{
    CHECK_NULL(pString);

    if (size > calculateStringLength(pString))
    {
        size = calculateStringLength(pString);
    }

    CHECK_HAL(HAL_UART_Transmit(&huart2, pString, size, HAL_MAX_DELAY));
    return UART_OK;
}

uart_Status_t uart_ReceiveStringSize(uint8_t * pString, uint16_t size)
{
    CHECK_NULL(pString);
    if (size <= 0)
    {
        return UART_OK;
    }

    CHECK_HAL(HAL_UART_Receive(&huart2, pString, size, HAL_MAX_DELAY));
    return UART_OK;
}
