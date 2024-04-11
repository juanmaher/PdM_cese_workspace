/**
 ******************************************************************************
 * @file    Drivers/API/Src/API_delay.c
 * @author  Juan Manuel Hernández
 * @brief   Implementation of non-blocking delay.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "API_delay.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DEFAULT_START_TIME 0

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/
/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
static void Error_Handler(void)
{
	/* Turn LED2 on */
	BSP_LED_On(LED2);
	while (1)
	{
	}
}

/* Public functions ---------------------------------------------------------*/
void delayInit(delay_t * delay, tick_t duration)
{
	if (delay == NULL || duration == 0)
	{
		Error_Handler();
	}

	delay->running = false;
	delay->startTime = DEFAULT_START_TIME;
	delay->duration = duration;
}

bool_t delayRead(delay_t * delay)
{
	if (delay == NULL)
	{
		Error_Handler();
		return false;
	}

	if (delay->running == false)
	{
		delay->running = true;
		delay->startTime = (tick_t) HAL_GetTick();
		return false;
	}

	if (HAL_GetTick() - delay->startTime >= delay->duration)
	{
		delay->running = false;
		return true;
	}
		
	return false;
}

void delayWrite(delay_t * delay, tick_t duration)
{
	if (delay == NULL || duration == 0)
	{
		Error_Handler();
	}

	delay->duration = duration;
}
