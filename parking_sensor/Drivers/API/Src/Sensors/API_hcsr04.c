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

#define CHECK_PORT(x)                    do { \
                                            if (x != HCSR04_PORT_OK) { return HCSR04_ERROR_PORT; } \
                                        } while(0U)

#define CHECK_INTERNAL(x)               do { \
                                            if (x != HCSR04_OK) { return x; } \
                                        } while(0U)

/* Private variables ---------------------------------------------------------*/
static bool measuring = false;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
uint32_t first_value_captured = 0;
uint32_t second_value_captured = 0;
bool first_capture_flag = false;

void hcsr04_ICCaptureCallback()
{
    uint8_t distance = 0;
    uint32_t difference = 0;

    if (!first_capture_flag) {
        first_value_captured = myTIM_ReadCapturedValue();
        first_capture_flag = true;
        myTIM_SetCapturePolarityFalling();
    } else if (first_capture_flag) {
        second_value_captured = myTIM_ReadCapturedValue();
        myTIM_ResetCounter();

        if (second_value_captured > first_value_captured) {
            difference = second_value_captured-first_value_captured;
        } else if (first_value_captured > second_value_captured) {
            difference = (hcsr04_PERIOD_180MHz - first_value_captured) + second_value_captured;
        }

        distance = difference * hcsr04_CONVERSION_FACTOR/2;

        first_capture_flag = false;

        myTIM_SetCapturePolarityRising();
        myTIM_DisableIT();

        measuring = false;
        hcsr04_IRQ_Callback(distance);
    }
}

/* Public functions ----------------------------------------------------------*/
hcsr04_Status_t hcsr04_Init()
{
    CHECK_PORT(myHcsr04GPIO_Init());
    CHECK_PORT(myTIM_Init());
    myTIM_StartIT();
    myTIM_DisableIT();
    return HCSR04_OK;
}

hcsr04_Status_t hcsr04_StartMeasure()
{
    if (!measuring) {
        myHcsr04GPIO_SetTrig();
        hcsr04_Delay(10);
        myHcsr04GPIO_ResetTrig();
        measuring = true;

        myTIM_EnableIT();
    }

    return HCSR04_OK;
}

bool hcsr04_GetStatusMeasuring()
{
    return measuring;
}

__weak void hcsr04_IRQ_Callback(uint16_t distance_value)
{
    UNUSED(distance_value);
}
