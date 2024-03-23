/**
 ******************************************************************************
 * @file    Drivers/API/Inc/API_delay.h
 * @author  Juan Manuel Hernández
 * @brief   Header for API_delay.c module
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
 * @brief Initializes the delay structure.
 * @param delay Pointer to the delay structure to be initialized.
 * @param duration Duration of the delay in ticks.
 */
void delayInit(delay_t *delay, tick_t duration);

/**
 * @brief Checks if the delay time has elapsed and starts the timer if it wasn't running.
 * @param delay Pointer to the delay structure.
 * @retval TRUE if the time has elapsed, FALSE if it hasn't.
 */
bool_t delayRead(delay_t *delay);

/**
 * @brief Sets the duration of the delay.
 * @param delay Pointer to the delay structure.
 * @param duration Duration of the delay in ticks.
 */
void delayWrite(delay_t *delay, tick_t duration);

#endif /* __API_DELAY_H */
