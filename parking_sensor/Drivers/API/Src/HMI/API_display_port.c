/**
 ******************************************************************************
 * @file    API_display_port.c
 * @author  Juan Manuel Hernández
 * @brief   Implementation of API_display_port
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "API_display_port.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static I2C_HandleTypeDef hi2c1;

/* Private function prototypes -----------------------------------------------*/
#define CHECK_I2C(x)                    do { \
                                            if (x != HAL_OK) { return DISPLAY_PORT_ERROR_HAL; } \
                                        } while(0U)

/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    /*** Configure the GPIOs ***/
    /* Enable GPIO clock */
    I2Cx_SCL_GPIO_CLK_ENABLE();
    I2Cx_SDA_GPIO_CLK_ENABLE();

    /*** Configure the I2C peripheral ***/
    GPIO_InitStruct.Pin = I2Cx_SCL_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = I2Cx_SCL_AF;
    HAL_GPIO_Init(I2Cx_SCL_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = I2Cx_SDA_PIN;
    HAL_GPIO_Init(I2Cx_SDA_GPIO_PORT, &GPIO_InitStruct);

    /* Enable I2C clock */
    I2Cx_CLK_ENABLE();
}

displayPort_Status_t myI2Cx_Init()
{
    hi2c1.Instance = I2Cx;
    hi2c1.Init.ClockSpeed = I2C_CLOCK_RATE;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    CHECK_I2C(HAL_I2C_Init(&hi2c1));
    return DISPLAY_PORT_OK;
}

displayPort_Status_t myI2Cx_DeInit()
{
    CHECK_I2C(HAL_I2C_DeInit(&hi2c1));
    return DISPLAY_PORT_OK;
}

displayPort_Status_t myI2Cx_Master_Transmit(uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    CHECK_I2C(HAL_I2C_Master_Transmit(&hi2c1, DevAddress, pData, Size, Timeout));
    return DISPLAY_PORT_OK;
}

void display_Delay(uint16_t nTime)
{
    HAL_Delay(nTime);
}
