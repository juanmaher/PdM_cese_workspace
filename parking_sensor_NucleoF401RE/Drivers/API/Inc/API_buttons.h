/**
 ******************************************************************************
 * @file    .h
 * @author  Juan Manuel Hernández
 * @brief   Header for .c module
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __API_BUTTONS_H
#define __API_BUTTONS_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_hal_exti.h"

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  BUTTON_USER = 0U,
}Button_TypeDef;

/* Keep compatibility with CMSIS Pack already delivered */
#define BUTTON_KEY BUTTON_USER

typedef enum
{
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

/* Exported constants --------------------------------------------------------*/
/**
 * @}
 */

/** @defgroup STM32F4XX_NUCLEO_LOW_LEVEL_BUTTON STM32F4XX_NUCLEO LOW LEVEL BUTTON
 * @{
 */
/* Button state */
#define BUTTON_RELEASED                   0U
#define BUTTON_PRESSED                    1U
/** Define number of BUTTON            **/
#define BUTTONn                           1U

/**
 * @brief User push-button
 */
  /**  Definition for BSP USER BUTTON   **/

#define BUS_GPIO_INSTANCE GPIO
#define BUS_BSP_BUTTON_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
#define BUS_BSP_BUTTON_GPIO_PIN GPIO_PIN_13
#define BUS_BSP_BUTTON_GPIO_CLK_DISABLE() __HAL_RCC_GPIOC_CLK_DISABLE()
#define BUS_BSP_BUTTON_GPIO_PORT GPIOC

#define USER_BUTTON_PIN	                  GPIO_PIN_13
#define USER_BUTTON_GPIO_PORT              GPIOC
#define USER_BUTTON_EXTI_IRQn              EXTI15_10_IRQn
#define USER_BUTTON_EXTI_LINE              EXTI_LINE_13
#define H_EXTI_13			  hpb_exti[BUTTON_USER]

#ifndef BSP_BUTTON_USER_IT_PRIORITY
  #define BSP_BUTTON_USER_IT_PRIORITY            15U
#endif

/* BSP Common Error codes */
#define BSP_ERROR_NONE                    0
#define BSP_ERROR_NO_INIT                -1
#define BSP_ERROR_WRONG_PARAM            -2
#define BSP_ERROR_BUSY                   -3
#define BSP_ERROR_PERIPH_FAILURE         -4
#define BSP_ERROR_COMPONENT_FAILURE      -5
#define BSP_ERROR_UNKNOWN_FAILURE        -6
#define BSP_ERROR_UNKNOWN_COMPONENT      -7
#define BSP_ERROR_BUS_FAILURE            -8
#define BSP_ERROR_CLOCK_FAILURE          -9
#define BSP_ERROR_MSP_FAILURE            -10
#define BSP_ERROR_FEATURE_NOT_SUPPORTED      -11

/* BSP BUS error codes */

#define BSP_ERROR_BUS_TRANSACTION_FAILURE    -100
#define BSP_ERROR_BUS_ARBITRATION_LOSS       -101
#define BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE    -102
#define BSP_ERROR_BUS_PROTOCOL_FAILURE       -103

#define BSP_ERROR_BUS_MODE_FAULT             -104
#define BSP_ERROR_BUS_FRAME_ERROR            -105
#define BSP_ERROR_BUS_CRC_ERROR              -106
#define BSP_ERROR_BUS_DMA_FAILURE            -107

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int32_t BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);

/**
 * @brief  Push Button DeInit.
 * @param  Button Button to be configured
 *                This parameter can be one of the following values:
 *                @arg  BUTTON_USER: Wakeup Push Button
 * @note PB DeInit does not disable the GPIO clock
 * @retval BSP status
 */
int32_t BSP_PB_DeInit(Button_TypeDef Button);

/**
 * @brief  Returns the selected button state.
 * @param  Button Button to be addressed
 *                This parameter can be one of the following values:
 *                @arg  BUTTON_USER
 * @retval The Button GPIO pin value (GPIO_PIN_RESET = button pressed)
 */

int32_t BSP_PB_GetState(Button_TypeDef Button);
/**
 * @brief  User EXTI line detection callbacks.
 * @retval None
 */
void BSP_PB_IRQHandler (Button_TypeDef Button);

/**
 * @brief  BSP Push Button callback
 * @param  Button Specifies the pin connected EXTI line
 * @retval None.
 */
__weak void BSP_PB_Callback(Button_TypeDef Button);

#endif /* __API_BUTTONS_H */
