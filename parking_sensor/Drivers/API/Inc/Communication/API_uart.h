/**
 ******************************************************************************
 * @file    Drivers/API/Inc/API_delay.H
 * @author  Juan Manuel Hernández
 * @brief   Header for API_delay.c module
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __API_UART_H
#define __API_UART_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "stm32f4xx_hal.h"  		/* <- HAL include */

/* Exported types ------------------------------------------------------------*/
typedef enum {
    UART_OK,
    UART_ERROR_NULL,
    UART_ERROR_HAL,
} uart_Status_t;

/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor USARTx/UARTx instance used and associated
   resources */
/* Definition for USARTx clock resources */
#define USARTx                           USART3
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART3_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART3_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART3_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_8
#define USARTx_TX_GPIO_PORT              GPIOD
#define USARTx_TX_AF                     GPIO_AF7_USART3
#define USARTx_RX_PIN                    GPIO_PIN_9
#define USARTx_RX_GPIO_PORT              GPIOD
#define USARTx_RX_AF                     GPIO_AF7_USART3

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/**
 * @brief  Init UART.
 * @note Initialization parameters:
 * - UartHandle.Instance        = USART3;
 * - UartHandle.Init.BaudRate   = 115200;
 * - UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
 * - UartHandle.Init.StopBits   = UART_STOPBITS_1;
 * - UartHandle.Init.Parity     = UART_PARITY_NONE;
 * - UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
 * - UartHandle.Init.Mode       = UART_MODE_TX_RX;
 * - UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
 * @param  None
 * @retval None
*/
uart_Status_t uart_Init();

/**
 * @brief  Send a string to UART.
 * @param  pString String to send
 * @retval uart_Status_t true if success, false if not
*/
uart_Status_t uart_SendString(uint8_t * pString);

/**
 * @brief  Send a string to UART.
 * @param  pString String to send
 * @param  size Size of the string
 * @retval uart_Status_t true if success, false if not
*/
uart_Status_t uart_SendStringSize(uint8_t * pString, uint16_t size);

/**
 * @brief  Receive a string via UART.
 * @param  pString String to receive
 * @param  size Size of the string expected
 * @retval uart_Status_t true if success, false if not
*/
uart_Status_t uart_ReceiveStringSize(uint8_t * pString, uint16_t size);

#endif /* __API_UART_H */
