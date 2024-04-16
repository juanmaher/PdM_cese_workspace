/**
 ******************************************************************************
 * @file    API_reverse.c
 * @author  Juan Manuel Hernández
 * @brief   Implementation of API_reverse
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "API_reverse.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
reverse_Status_t reverse_Init()
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

    debounceFSM_init();

    return REVERSE_OK;
}

reverse_Status_t reverse_DeInit()
{
    GPIO_InitTypeDef gpio_init_structure;

    gpio_init_structure.Pin = REVERSE_PIN;
    HAL_GPIO_DeInit(REVERSE_GPIO_PORT, gpio_init_structure.Pin);

    return REVERSE_OK;
}

uint32_t reverse_GetState()
{
    debounceFSM_update();
    return readKey();
}
