/**
 ******************************************************************************
 * @file    .h
 * @author  Juan Manuel Hernández
 * @brief   Header for .c module
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __API_GPIO_H
#define __API_GPIO_H

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
 * @brief Initialize the GPIO
 * @param port GPIO port
 * @param pin GPIO pin
 * @return bool_t true if success, false if not
*/
bool gpioInit(GPIO_TypeDef * port, uint16_t pin);

/**
 * @brief Read the GPIO
 * @param port GPIO port
 * @param pin GPIO pin
 * @return bool_t true if success, false if not
*/
bool gpioRead(GPIO_TypeDef * port, uint16_t pin);

/**
 * @brief Write the GPIO
 * @param port GPIO port
 * @param pin GPIO pin
 * @param value GPIO value
 * @return bool_t true if success, false if not
*/
bool gpioWrite(GPIO_TypeDef * port, uint16_t pin, bool_t value);

#endif /* __API_GPIO_H */
