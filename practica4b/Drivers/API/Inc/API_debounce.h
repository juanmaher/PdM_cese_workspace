/**
 ******************************************************************************
 * @file    Drivers/API/Inc/API_debounce.h
 * @author  Juan Manuel Hernández
 * @brief   Header for API_debounce.c module
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __API_DEBOUNCE_H
#define __API_DEBOUNCE_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */
#include "API_delay.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/**
 * @brief Initialize the debounce FSM
 * @return void
*/
void debounceFSM_init();		

/**
 * @brief Update the debounce FSM
 * @return void
*/
void debounceFSM_update();

/**
 * @brief Return button state
 * @return bool_t TRUE o FALSE depending on the button state
*/
bool_t readKey();

#endif /* __API_DEBOUNCE_H */
