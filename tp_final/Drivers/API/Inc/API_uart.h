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
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */

/* Exported types ------------------------------------------------------------*/
typedef bool bool_t;

/* Exported constants --------------------------------------------------------*/
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
bool_t uartInit();

/**
 * @brief  Send a string to UART.
 * @param  pString String to send
 * @retval bool_t true if success, false if not
*/
bool_t uartSendString(uint8_t * pString);

/**
 * @brief  Send a string to UART.
 * @param  pString String to send
 * @param  size Size of the string
 * @retval bool_t true if success, false if not
*/
bool_t uartSendStringSize(uint8_t * pString, uint16_t size);

/**
 * @brief  Receive a string via UART.
 * @param  pString String to receive
 * @param  size Size of the string expected
 * @retval bool_t true if success, false if not
*/
bool_t uartReceiveStringSize(uint8_t * pString, uint16_t size);

#endif /* __API_UART_H */
