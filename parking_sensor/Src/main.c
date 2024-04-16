/**
 ******************************************************************************
 * @file    Src/main.c
 * @author  Juan Manuel Hernández
 * @brief   Main program body
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum {
    CONFIGURATION,
    WELCOME,
    IDLE,
    MEASURING,
    SHARING
} parkingSensor_State_t;

/* Private define ------------------------------------------------------------*/
#define WELCOME_DELAY_DURATION_MS               1000
#define DISPLAYING_DATA_DELAY_DURATION_MS       1000

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static parkingSensor_State_t parkingSensor_State = CONFIGURATION;
static delay_t welcome_delay;
static delay_t displaying_data_delay;

static const char welcome_msg[] = "Parking Sensor";
static bool welcome_msg_flag = false;

static char distance_dbg_msg[16];
static char distance_msg[16];
static bool distance_msg_flag = false;

static uint16_t last_distance = 0;
static uint16_t distance_processed = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void Error_Handler(void);
static void parkingSensor_UpdateFSM();
void hcsr04_IRQ_Callback(uint16_t distance);
uint8_t parking_ProcessData();
void parking_GenerateLevel(char * st, int nivel_resolucion);

/* Private user code ---------------------------------------------------------*/
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    HAL_Init();

    /* Configure the system clock to 180 MHz */
    SystemClock_Config();

    /* Infinite loop */
    while (1) {
        parkingSensor_UpdateFSM();
        HAL_Delay(100);
    }
}

static void parkingSensor_UpdateFSM()
{
    switch (parkingSensor_State) {
        case CONFIGURATION:
            if (display_Init() != DISPLAY_OK)
                Error_Handler();

            if (hcsr04_Init() != HCSR04_OK)
                Error_Handler();

            if (uart_Init() != UART_OK)
                Error_Handler();
            
            if (reverse_Init() != REVERSE_OK)
                Error_Handler();

            delayInit(&welcome_delay, WELCOME_DELAY_DURATION_MS);
            delayInit(&displaying_data_delay, DISPLAYING_DATA_DELAY_DURATION_MS);
            parkingSensor_State = WELCOME;
            break;

        case WELCOME:
            if (!welcome_msg_flag) {
                welcome_msg_flag = true;
                display_Clear();
                display_PrintStringInTopLine((uint8_t *) welcome_msg);
                display_TurnOn();
            }
            
            if (delayRead(&welcome_delay)) {
                display_TurnOff();
                parkingSensor_State = IDLE;
            }
            break;

        case IDLE:
            if (reverse_GetState())
                parkingSensor_State = MEASURING;
            break;

        case MEASURING:
            if (reverse_GetState()) {
                if (!hcsr04_GetStatusMeasuring())
                    hcsr04_StartMeasure();
            } else {
                display_TurnOff();
                parkingSensor_State = IDLE;
            }
            break;

        case SHARING:
            if (reverse_GetState()) {
                if (!distance_msg_flag) {
                    distance_msg_flag = true;
                    distance_processed = parking_ProcessData();
                    sprintf(distance_dbg_msg, "%u \r\n", distance_processed);
                    uart_SendStringSize((uint8_t *) distance_dbg_msg, strlen(distance_dbg_msg));
                    parking_GenerateLevel(distance_msg, distance_processed);
                    display_Clear();
                    display_PrintStringInTopLine((uint8_t *) distance_msg);
                    display_PrintStringInBottomLine((uint8_t *) distance_msg);
                    if (!display_GetState()) {
                        display_TurnOn();
                    }
                }
                if (delayRead(&displaying_data_delay))
                    parkingSensor_State = MEASURING;
            } else {
                display_TurnOff();
                parkingSensor_State = IDLE;
            }
            break;

        default:
            Error_Handler();
            break;
    }
}

void hcsr04_IRQ_Callback(uint16_t distance)
{
    last_distance = distance;
    distance_msg_flag = false;
    parkingSensor_State = SHARING;
}

uint8_t parking_ProcessData()
{
    if (last_distance > 50) {
        return 0;
    }

    for (int i = 1; i <= 8; i++) {
        if (last_distance >= (50 - (i * 6.25))) {
            return i - 1;
        }
    }

    return 7;
}

void parking_GenerateLevel(char * st, int nivel_resolucion)
{
    if (st == NULL || nivel_resolucion < 0 || nivel_resolucion > 8) {
        Error_Handler();
        return;
    }

    memset(st, 0, sizeof(distance_msg));

    if (nivel_resolucion == 0) {
        return;
    }

    for (int i = 0; i < nivel_resolucion*2+2; i++) {
        st[i] = 0xFF;
    }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 168;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
static void Error_Handler(void)
{
    /* Turn LED2 on */
    BSP_LED_On(LED2);
    __disable_irq();
    while (1);
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1);
}
#endif
