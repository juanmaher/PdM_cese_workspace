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
#include "API_hcsr04_port.h"

typedef bool bool_t;

/* Exported types ------------------------------------------------------------*/
typedef enum {
    HCSR04_OK,
    HCSR04_ERROR_NULL,
    HCSR04_ERROR_PORT
} hcsr04_Status_t;

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define hcsr04_CONVERSION_FACTOR                            0.034
#define hcsr04_MIN_DISTANCE                                 0
#define hcsr04_MAX_DISTANCE                                 50

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
hcsr04_Status_t hcsr04_Init();
hcsr04_Status_t hcsr04_StartMeasure();
bool hcsr04_GetStatusMeasuring();
void hcsr04_IRQ_Callback(uint16_t distance);
void hcsr04_ICCaptureCallback();

#endif /* __API_HCSR04_H */
