/**
 ******************************************************************************
 * @file    API_hcsr04.h
 * @author  Juan Manuel Hernández
 * @brief   Header for API_hcsr04.c module
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
#include "API_hcsr04_port.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {
    HCSR04_OK,
    HCSR04_ERROR_NULL,
    HCSR04_ERROR_PORT
} hcsr04_Status_t;

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define hcsr04_CONVERSION_FACTOR                            0.034
// Data obtained from the datasheet
#define hcsr04_MIN_DISTANCE                                 0
#define hcsr04_MAX_DISTANCE                                 50

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/**
 * @brief Initialize the hcsr04
 * @retval hcsr04_Status_t status
*/
hcsr04_Status_t hcsr04_Init();

/**
 * @brief Start the hcsr04 measurement
 * @retval hcsr04_Status_t status
*/
hcsr04_Status_t hcsr04_StartMeasure();

/**
 * @brief Get the status of the hcsr04
 * @retval bool status
*/
bool hcsr04_GetStatusMeasuring();

/**
 * @brief IRQ callback
 * @param[in] distance Distance in centimeters
 * @retval void
*/
void hcsr04_IRQ_Callback(uint16_t distance);

/**
 * @brief IC capture callback
 * @retval void
*/
void hcsr04_ICCaptureCallback();

#endif /* __API_HCSR04_H */
