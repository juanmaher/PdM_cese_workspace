/**
 ******************************************************************************
 * @file    Drivers/API/Src/API_debounce.c
 * @author  Juan Manuel Hernández
 * @brief   Implementation of button debounce.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "API_debounce.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum {
    BUTTON_UP, // Button released
    BUTTON_FALLING, // Decreasing edge
    BUTTON_RISING, // Increasing edge
    BUTTON_DOWN // Button pushed
} debounceState_t;

/* Private define ------------------------------------------------------------*/
#define DELAY_DURATION_MS 40

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static debounceState_t debounceState;
static bool_t buttonState;
static delay_t delay;

static char button_pressed_msg[] = "Button pressed\r\n";
static char button_released_msg[] = "Button released\r\n";

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
void debounceFSM_init()
{
	debounceState = BUTTON_UP;
	buttonState = false;
	delayInit(&delay, DELAY_DURATION_MS);

}

void debounceFSM_update()
{
	switch (debounceState) {
		case BUTTON_UP:
			if (BSP_PB_GetState(BUTTON_USER)) {
				debounceState = BUTTON_FALLING;
			}
			break;
		case BUTTON_FALLING:
			if (delayRead(&delay)) {
				if (BSP_PB_GetState(BUTTON_USER)) {
					debounceState = BUTTON_DOWN;
					buttonState = true;
					uartSendStringSize((uint8_t *)button_pressed_msg, strlen(button_pressed_msg));
				} else {
					debounceState = BUTTON_UP;
				}
			}
			break;
		case BUTTON_RISING:
			if (delayRead(&delay)) {
				if (BSP_PB_GetState(BUTTON_USER)) {
					debounceState = BUTTON_DOWN;
				} else {
					debounceState = BUTTON_UP;
					buttonState = false;
					uartSendStringSize((uint8_t *)button_released_msg, strlen(button_released_msg));
				}
			}
			break;
		case BUTTON_DOWN:
			if (!BSP_PB_GetState(BUTTON_USER)) {
				debounceState = BUTTON_RISING;
			}
			break;
		default:
			break;
	}
}

bool_t readKey()
{
    return buttonState;
}
