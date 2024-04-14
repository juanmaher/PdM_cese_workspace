/**
 ******************************************************************************
 * @file    .h
 * @author  Juan Manuel Hernández
 * @brief   Header for .c module
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __API_HCSR04_H
#define __API_HCSR04_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "stm32f4xx_hal.h"

typedef bool bool_t;

/* Exported types ------------------------------------------------------------*/
typedef enum {
    HCSR04_OK,
    HCSR04_ERROR_NULL,
    HCSR04_ERROR_HAL,
} hcsr04_Status_t;

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define hcsr04_TIMER                                    TIM1

#define hcsr04_ECHO_PIN                                 GPIO_PIN_9
#define hcsr04_ECHO_GPIO_PORT                           GPIOE
#define hcsr04_ECHO_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOE_CLK_ENABLE()
#define hcsr04_ECHO_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOE_CLK_DISABLE()

#define hcsr04_TRIG_PIN                                 GPIO_PIN_2
#define hcsr04_TRIG_GPIO_PORT                           GPIOE
#define hcsr04_TRIG_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOE_CLK_ENABLE()
#define hcsr04_TRIG_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOE_CLK_DISABLE()

#define hcsr04_TRIG_SOURCE                              TIM_TS_TI1FP1
/* Exported functions ------------------------------------------------------- */
hcsr04_Status_t hcsr04_Init();
hcsr04_Status_t hcsr04_StartMeasure();
uint8_t hcsr04_GetDistance();

#endif /* __API_HCSR04_H */
