/**
 ******************************************************************************
 * @file    API_display_port.h
 * @author  Juan Manuel Hernández
 * @brief   Header for API_display_port.c module
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __API_DISPLAY_PORT_H
#define __API_DISPLAY_PORT_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "API_display.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {
    DISPLAY_PORT_OK = 0,
    DISPLAY_PORT_ERROR_HAL
} displayPort_Status_t;

/* Exported constants --------------------------------------------------------*/
/* I2C definition */
#define I2Cx                                     I2C1
#define I2Cx_CLK_ENABLE()                        __HAL_RCC_I2C1_CLK_ENABLE()

#define I2Cx_SCL_AF                              GPIO_AF4_I2C1
#define I2Cx_SCL_GPIO_PORT                       GPIOB
#define I2Cx_SCL_PIN                             GPIO_PIN_8
#define I2Cx_SCL_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Cx_SCL_GPIO_CLK_DISABLE()              __HAL_RCC_GPIOB_CLK_DISABLE()

#define I2Cx_SDA_AF                              GPIO_AF4_I2C1
#define I2Cx_SDA_GPIO_PORT                       GPIOB
#define I2Cx_SDA_PIN                             GPIO_PIN_9
#define I2Cx_SDA_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Cx_SDA_GPIO_CLK_DISABLE()              __HAL_RCC_GPIOB_CLK_DISABLE()

#define I2C_CLOCK_RATE                          100000

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
displayPort_Status_t myI2Cx_Init();
displayPort_Status_t myI2Cx_DeInit();
displayPort_Status_t myI2Cx_Master_Transmit(uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
void display_Delay(uint16_t nTime);

#endif /* __API_DISPLAY_PORT_H */
