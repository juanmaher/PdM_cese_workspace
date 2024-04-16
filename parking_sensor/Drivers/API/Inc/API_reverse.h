/**
 ******************************************************************************
 * @file    API_reverse.h
 * @author  Juan Manuel Hernández
 * @brief   Header for API_reverse.c module
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __API_REVERSE_H
#define __API_REVERSE_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "API_debounce.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {
    REVERSE_OK,
    REVERSE_ERROR_NULL,
    REVERSE_ERROR_HAL,
} reverse_Status_t;

/* Exported constants --------------------------------------------------------*/
#define REVERSE_PIN                          GPIO_PIN_13
#define REVERSE_GPIO_PORT                    GPIOC
#define REVERSE_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOC_CLK_ENABLE()
#define REVERSE_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOC_CLK_DISABLE()
#define REVERSE_EXTI_LINE                    GPIO_PIN_13
#define REVERSE_EXTI_IRQn                    EXTI15_10_IRQn

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
reverse_Status_t reverse_Init();
reverse_Status_t reverse_DeInit();
uint32_t reverse_GetState();

#endif /* __API_REVERSE_H */
