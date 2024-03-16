/**
 ******************************************************************************
 * @file    UART/UART_Printf/Src/main.c
 * @author  MCD Application Team
 * @brief   This example shows how to retarget the C library printf function
 *          to the UART.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "API_delay.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MAX_LED_SEQ 2
#define DELAY_TIME_MS 200

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
	if (delay == NULL)
	{
		Error_Handler();
	}

	delay->running = false;
	delay->startTime = 0;
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
	if (delay == NULL)
	{
		Error_Handler();
	}

	delay->duration = duration;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
