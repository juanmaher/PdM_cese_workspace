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
UART_HandleTypeDef UartHandle;

static const char welcome_msg[] = "Serial Port Communication (UART3)\r\n";
static const char info_msg[] = 
    "Serial Port = USART3\r\n"
    "Baudrate = 115200\r\n"
    "Word Length = 8\r\n"
    "Stop Bits = 1\r\n"
    "Parity = None\r\n\r\n";

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
static void Error_Handler(void)
{
	/* Turn LED2 on */
    BSP_LED_Init(LED2);
	BSP_LED_On(LED2);
	while (1)
	{
	}
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
bool_t uartInit()
{

    UartHandle.Instance        = USART3;
    UartHandle.Init.BaudRate   = 115200;
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits   = UART_STOPBITS_1;
    UartHandle.Init.Parity     = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode       = UART_MODE_TX_RX;
    UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&UartHandle) != HAL_OK)
    {
        return false;
    }
    uartSendStringSize((uint8_t *)welcome_msg, strlen(welcome_msg));
    uartSendStringSize((uint8_t *)info_msg, strlen(info_msg));
    return true;
}

bool_t uartSendString(uint8_t * pString)
{
    if (pString == NULL)
    {
        Error_Handler();
    }

    return HAL_UART_Transmit(&UartHandle, pString, calculateStringLength(pString), HAL_MAX_DELAY);
}

bool_t uartSendStringSize(uint8_t * pString, uint16_t size)
{
    if (pString == NULL || size <= 0)
    {
        Error_Handler();
    }

    if (size > calculateStringLength(pString))
    {
        size = calculateStringLength(pString);
    }

    return HAL_UART_Transmit(&UartHandle, pString, size, HAL_MAX_DELAY) != HAL_OK;
}

bool_t uartReceiveStringSize(uint8_t * pString, uint16_t size)
{
    if (pString == NULL || size <= 0)
    {
        Error_Handler();
    }

    return HAL_UART_Receive(&UartHandle, pString, size, HAL_MAX_DELAY) != HAL_OK;
}
