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
    myReverseGPIO_Init();
    debounceFSM_init();

    return REVERSE_OK;
}

reverse_Status_t reverse_DeInit()
{
    myReverseGPIO_DeInit();
    return REVERSE_OK;
}

uint32_t reverse_GetState()
{
    debounceFSM_update();
    return readKey();
}
