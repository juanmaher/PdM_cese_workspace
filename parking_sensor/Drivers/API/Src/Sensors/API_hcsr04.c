/**
 ******************************************************************************
 * @file    API_hcsr04.c
 * @author  Juan Manuel Hernández
 * @brief   Implementation of API_hcsr04
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
// Flag to know if it is measuring or not
static bool measuring_flag = false;
static uint32_t first_value_captured = 0;
static uint32_t second_value_captured = 0;
// Flag to know if the first capture has been done
static bool first_capture_flag = false;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
hcsr04_Status_t hcsr04_Init()
{
    CHECK_PORT(myHcsr04GPIO_Init());
    CHECK_PORT(myTIM_Init());

    // Start IT module for the ECHO pin, but disabled 
    myTIM_StartIT();
    myTIM_DisableIT();
    return HCSR04_OK;
}

hcsr04_Status_t hcsr04_StartMeasure()
{
    if (!measuring_flag) {
        // Write a HIGH value in the TRIG pin
        myHcsr04GPIO_SetTrig();
        // Wait 10 us by datasheet recommendation
        hcsr04_Delay(10);
        // Write a LOW value in the TRIG pin
        myHcsr04GPIO_ResetTrig();
        measuring_flag = true;

        // Enable to IT to wait to rising edge in the ECHO pin
        myTIM_EnableIT();
    }

    return HCSR04_OK;
}

bool hcsr04_GetStatusMeasuring()
{
    return measuring_flag;
}

void hcsr04_ICCaptureCallback()
{
    uint8_t distance = 0;
    uint32_t difference = 0;

    if (!first_capture_flag) {
        /* A rising edge has been detected in the ECHO pin.
        Take a timestamp and change the polarity of the IT to detect the falling edge */
        first_value_captured = myTIM_ReadCapturedValue();
        first_capture_flag = true;
        myTIM_SetCapturePolarityFalling();
    } else if (first_capture_flag) {
        /* A falling edge has been detected in the ECHO pin.
        Take a timestamp and prepare to a new measure: 
            - Reset the counter.
            - Set the IT to the rising edge.
            - Disable the IT.
        With the data captured, calculate the difference of the read data and transform it to centimeters.
        */
        second_value_captured = myTIM_ReadCapturedValue();
        myTIM_ResetCounter();

        if (second_value_captured > first_value_captured) {
            difference = second_value_captured - first_value_captured;
        } else if (first_value_captured > second_value_captured) {
            difference = (hcsr04_PERIOD_180MHz - first_value_captured) + second_value_captured;
        }

        distance = difference * hcsr04_CONVERSION_FACTOR/2;

        first_capture_flag = false;

        myTIM_SetCapturePolarityRising();
        myTIM_DisableIT();

        measuring_flag = false;

        // Notify that the measurement is ready
        hcsr04_IRQ_Callback(distance);
    }
}

__weak void hcsr04_IRQ_Callback(uint16_t distance_value)
{
    UNUSED(distance_value);
}
