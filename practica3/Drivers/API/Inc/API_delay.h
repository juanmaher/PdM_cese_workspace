/**
  ******************************************************************************
  * @file    API/Inc/API_delay.h
  * @author  Juan Manuel Hernández
  * @brief   Header for API_delay.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __API_DELAY_H
#define __API_DELAY_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */

/* Exported types ------------------------------------------------------------*/
typedef uint32_t tick_t;
typedef bool bool_t;

typedef struct {
   tick_t startTime;
   tick_t duration;
   bool_t running;
} delay_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/**
 * @brief Initializes the delay
 */
void delayInit(delay_t * delay, tick_t duration);

/**
 * @brief Reads if the time has consumed and starts the timer if it wasn't running.
 * @return param bool_t TRUE if the time has consumed and FALSE if it hasn't.
 */
bool_t delayRead(delay_t * delay);

/**
 * @brief Sets the duration of the delay
 */
void delayWrite(delay_t * delay, tick_t duration);

#endif /* __API_DELAY_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
