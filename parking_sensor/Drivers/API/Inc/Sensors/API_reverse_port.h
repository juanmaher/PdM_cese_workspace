/**
 ******************************************************************************
 * @file    API_reverse_port.h
 * @author  Juan Manuel Hernández
 * @brief   Header for API_reverse_port.c module
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __API_REVERSE_PORT_H
#define __API_REVERSE_PORT_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"  		/* <- HAL include */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define REVERSE_PIN                          GPIO_PIN_13
#define REVERSE_GPIO_PORT                    GPIOC
#define REVERSE_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOC_CLK_ENABLE()
#define REVERSE_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOC_CLK_DISABLE()

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void myReverseGPIO_Init();
void myReverseGPIO_DeInit();

#endif /* __API_REVERSE_PORT_H */
