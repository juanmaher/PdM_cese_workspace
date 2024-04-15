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
UART_HandleTypeDef huart3;

static const char welcome_msg[] = "Serial Port Communication (UART3)\r\n";
static const char info_msg[] = 
    "Serial Port = USART3\r\n"
    "Baudrate = 115200\r\n"
    "Word Length = 8\r\n"
    "Stop Bits = 1\r\n"
    "Parity = None\r\n\r\n";

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
    GPIO_InitTypeDef  GPIO_InitStruct;

    if(huart->Instance == USARTx) {
        /* Enable GPIO TX/RX clock */
        USARTx_TX_GPIO_CLK_ENABLE();
        USARTx_RX_GPIO_CLK_ENABLE();

        /* Enable USARTx clock */
        USARTx_CLK_ENABLE();

        /* UART TX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = USARTx_TX_PIN;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = USARTx_TX_AF;

        HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

        /* UART RX GPIO pin configuration  */
        GPIO_InitStruct.Pin = USARTx_RX_PIN;
        GPIO_InitStruct.Alternate = USARTx_RX_AF;

        HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
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
    if(huart->Instance == USARTx) {
        USARTx_FORCE_RESET();
        USARTx_RELEASE_RESET();

        /* Configure UART Tx as alternate function  */
        HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
        /* Configure UART Rx as alternate function  */
        HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);
    }

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static uart_Status_t myUART_Init(void)
{
    huart3.Instance        = USART3;
    huart3.Init.BaudRate   = 115200;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits   = UART_STOPBITS_1;
    huart3.Init.Parity     = UART_PARITY_NONE;
    huart3.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    huart3.Init.Mode       = UART_MODE_TX_RX;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    CHECK_HAL(HAL_UART_Init(&huart3));

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

    uart_SendStringSize((uint8_t *)welcome_msg, strlen(welcome_msg));
    uart_SendStringSize((uint8_t *)info_msg, strlen(info_msg));

    return UART_OK;
}

uart_Status_t uart_SendString(uint8_t * pString)
{
    CHECK_NULL(pString);

    CHECK_HAL(HAL_UART_Transmit(&huart3, pString, calculateStringLength(pString), HAL_MAX_DELAY));
    return UART_OK;
}

uart_Status_t uart_SendStringSize(uint8_t * pString, uint16_t size)
{
    CHECK_NULL(pString);

    if (size > calculateStringLength(pString))
    {
        size = calculateStringLength(pString);
    }

    CHECK_HAL(HAL_UART_Transmit(&huart3, pString, size, HAL_MAX_DELAY));
    return UART_OK;
}

uart_Status_t uart_ReceiveStringSize(uint8_t * pString, uint16_t size)
{
    CHECK_NULL(pString);
    if (size <= 0)
    {
        return UART_OK;
    }

    CHECK_HAL(HAL_UART_Receive(&huart3, pString, size, HAL_MAX_DELAY));
    return UART_OK;
}
