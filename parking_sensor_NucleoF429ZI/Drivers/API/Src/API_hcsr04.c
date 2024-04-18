/**
 ******************************************************************************
 * @file    .c
 * @author  Juan Manuel Hernández
 * @brief   Implementation of
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "API_hcsr04.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define CHECK_NULL(x)                   do { \
                                            if (x == NULL) { return HCSR04_ERROR_NULL; } \
                                        } while(0U)

#define CHECK_INTERNAL(x)               do { \
                                            if (x != HCSR04_OK) { return x; } \
                                        } while(0U)

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/* Public functions ----------------------------------------------------------*/
hcsr04_Status_t hcsr04_Init()
{
    return HCSR04_OK;
}

hcsr04_Status_t hcsr04_GetDistance(uint8_t * distance)
{
    return HCSR04_OK;
}