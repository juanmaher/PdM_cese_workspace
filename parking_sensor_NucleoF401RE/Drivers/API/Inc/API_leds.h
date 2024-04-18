/**
 ******************************************************************************
 * @file    .h
 * @author  Juan Manuel Hernández
 * @brief   Header for .c module
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __API_LEDS_H
#define __API_LEDS_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Exported types ------------------------------------------------------------*/
typedef enum 
{
  LED2 = 0
}Led_TypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define LEDn                                    1

#define LED2_PIN                                GPIO_PIN_5
#define LED2_GPIO_PORT                          GPIOA
#define LED2_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOA_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOA_CLK_DISABLE()

#define LEDx_GPIO_CLK_ENABLE(__INDEX__)   do { if((__INDEX__) == 0) {__HAL_RCC_GPIOA_CLK_ENABLE();} else\
                                                                    {__HAL_RCC_GPIOA_CLK_ENABLE();   }} while(0)	
#define LEDx_GPIO_CLK_DISABLE(__INDEX__)  do { if((__INDEX__) == 0) {__HAL_RCC_GPIOA_CLK_DISABLE();} else\
                                                                    {__HAL_RCC_GPIOA_CLK_DISABLE();   }} while(0)	

/* Exported functions ------------------------------------------------------- */
void             BSP_LED_Init(Led_TypeDef Led);
void             BSP_LED_DeInit(Led_TypeDef Led);
void             BSP_LED_On(Led_TypeDef Led);
void             BSP_LED_Off(Led_TypeDef Led);
void             BSP_LED_Toggle(Led_TypeDef Led);

#endif /* __API_LEDS_H */
