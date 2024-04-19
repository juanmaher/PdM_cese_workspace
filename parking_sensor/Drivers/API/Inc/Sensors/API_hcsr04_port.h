/**
 ******************************************************************************
 * @file    API_hcsr04_port.h
 * @author  Juan Manuel Hernández
 * @brief   Header for API_hcsr04_port.c module
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __API_HCSR04_PORT_H
#define __API_HCSR04_PORT_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "API_hcsr04.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {
    HCSR04_PORT_OK = 0,
    HCSR04_PORT_ERROR_HAL
} hcsr04Port_Status_t;

/* Exported constants --------------------------------------------------------*/
#define hcsr04_TIMER                                    TIM1
#define hcsr04_TIMER_CLK_ENABLE()                       __HAL_RCC_TIM1_CLK_ENABLE();

#define hcsr04_ECHO_PIN                                 GPIO_PIN_9
#define hcsr04_ECHO_GPIO_PORT                           GPIOE
#define hcsr04_ECHO_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOE_CLK_ENABLE()
#define hcsr04_ECHO_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOE_CLK_DISABLE()

#define hcsr04_TRIG_PIN                                 GPIO_PIN_2
#define hcsr04_TRIG_GPIO_PORT                           GPIOE
#define hcsr04_TRIG_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOE_CLK_ENABLE()
#define hcsr04_TRIG_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOE_CLK_DISABLE()

#define hcsr04_TRIG_SOURCE                              TIM_TS_TI1FP1

#define hcsr04_IRQn                                     TIM1_CC_IRQn

// SystemClock Freq 180 MHz
#define hcsr04_PREESCALER_180MHz                        180         // 1 us pulses
#define hcsr04_PERIOD_180MHz                            0xFFFF

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
hcsr04Port_Status_t myTIM_Init();
uint32_t myTIM_ReadCapturedValue();
void myTIM_SetCapturePolarityFalling();
void myTIM_SetCapturePolarityRising();
void myTIM_ResetCounter();
void myTIM_StartIT();
void myTIM_DisableIT();
void myTIM_EnableIT();
void hcsr04_Delay(uint32_t time);
hcsr04Port_Status_t myHcsr04GPIO_Init();
void myHcsr04GPIO_SetTrig();
void myHcsr04GPIO_ResetTrig();

#endif /* __API_HCSR04_PORT_H */
