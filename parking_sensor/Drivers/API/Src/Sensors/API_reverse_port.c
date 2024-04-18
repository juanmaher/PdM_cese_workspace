/**
 ******************************************************************************
 * @file    API_reverse_port.c
 * @author  Juan Manuel Hernández
 * @brief   Implementation of API_reverse_port
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "API_reverse_port.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
void myReverseGPIO_Init()
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* Enable the BUTTON Clock */
    REVERSE_GPIO_CLK_ENABLE();

    /* Configure Button pin as input */
    GPIO_InitStruct.Pin = REVERSE_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(REVERSE_GPIO_PORT, &GPIO_InitStruct);
}

void myReverseGPIO_DeInit()
{
    GPIO_InitTypeDef gpio_init_structure;

    gpio_init_structure.Pin = REVERSE_PIN;
    HAL_GPIO_DeInit(REVERSE_GPIO_PORT, gpio_init_structure.Pin);
}