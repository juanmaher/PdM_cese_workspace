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
#include "API_debounce.h"
#include "API_reverse_port.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {
    REVERSE_OK,
    REVERSE_ERROR_NULL,
    REVERSE_ERROR_HAL,
} reverse_Status_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/**
 * @brief Initialize the reverse module
 * @return reverse_Status_t status
*/
reverse_Status_t reverse_Init();

/**
 * @brief DeInitialize the reverse module
 * @return reverse_Status_t status
*/
reverse_Status_t reverse_DeInit();

/**
 * @brief Get the state of the reverse
 * @return uint32_t state
*/
uint32_t reverse_GetState();

#endif /* __API_REVERSE_H */
