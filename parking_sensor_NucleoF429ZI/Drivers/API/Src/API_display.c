/**
 ******************************************************************************
 * @file    .c
 * @author  Juan Manuel Hernández
 * @brief   Implementation of
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "API_display.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum {
    DELAY_1MS = 1,
    DELAY_2MS = 2,
    DELAY_10MS = 10,
    DELAY_20MS = 20
} display_Delay_t;

typedef enum {
    RS_CONTROL = 0,
    RS_DATA = 1
} display_RsType_t;

/* Private define ------------------------------------------------------------*/
#define HIGH_NIBBLE_MASK 0xF0
#define LOW_NIBBLE_MASK 0x0F

#define HIGH_NIBBLE_SHIFT 4
#define LOW_NIBBLE_SHIFT 0

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static I2C_HandleTypeDef hi2c1;

/* Private function prototypes -----------------------------------------------*/
static display_Status_t myI2Cx_Init();
static display_Status_t display_Send4bitsCmd(uint8_t value, display_RsType_t rs_type);

/* Private function in macros */
#define display_TransmitData(value)             HAL_I2C_Master_Transmit(&hi2c1, DISPLAY_ADDR<<1, &value, sizeof(value), HAL_MAX_DELAY)
#define display_Send4bitsControlCmd(value)      display_Send4bitsCmd(value, RS_CONTROL)
#define display_Send4bitsDataCmd(value)         display_Send4bitsCmd(value, RS_DATA)
#define display_Send8bitsControlCmd(value)      display_Send8bitsCmd(value, RS_CONTROL)
#define display_Send8bitsDataCmd(value)         display_Send8bitsCmd(value, RS_DATA)

#define CHECK_NULL(x)                   do { \
                                            if (x == NULL) { return DISPLAY_ERROR_NULL; } \
                                        } while(0U)

#define CHECK_I2C(x)                    do { \
                                            if (x != HAL_OK) { return DISPLAY_ERROR_HAL; } \
                                        } while(0U)
                                    
#define CHECK_INTERNAL(x)               do { \
                                            if (x != DISPLAY_OK) { return x; } \
                                        } while(0U)

/* Private functions ---------------------------------------------------------*/
static display_Status_t display_Send4bitsCmd(uint8_t value, display_RsType_t rs_type)
{
    uint8_t cmd = 0;
    // D = bit de datos
    // E = enable bit
    // B = enable backlight bit
    // R (R/W) = read/write bit
    // T (RS) = data or control bit
    // DDDDEBRT
    HAL_Delay(DELAY_1MS);
    cmd = value | BIT_BACKLIGHT | BIT_ENABLE | rs_type;
    CHECK_I2C(display_TransmitData(cmd));
    HAL_Delay(DELAY_1MS);
    cmd = value | BIT_BACKLIGHT | rs_type;
    CHECK_I2C(display_TransmitData(cmd));
    return DISPLAY_OK;
}

static display_Status_t display_Send8bitsCmd(uint8_t value, display_RsType_t rs_type)
{
    uint8_t cmd = 0;
    cmd = value & HIGH_NIBBLE_MASK;
    CHECK_INTERNAL(display_Send4bitsCmd(cmd, rs_type));
    cmd = value << HIGH_NIBBLE_SHIFT;
    CHECK_INTERNAL(display_Send4bitsCmd(cmd, rs_type));
    return DISPLAY_OK;
}

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

static display_Status_t myI2Cx_Init()
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

    return DISPLAY_OK;
}

/* Public functions ----------------------------------------------------------*/
display_Status_t display_Init()
{
    CHECK_INTERNAL(myI2Cx_Init());

    // Initialization sequence recommended by the datasheet
    HAL_Delay(DELAY_20MS);
    // 0x3c - 0x38
    CHECK_INTERNAL(display_Send4bitsControlCmd(CMD_INIT_1));
    HAL_Delay(DELAY_10MS);
    // 0x3c - 0x38
    CHECK_INTERNAL(display_Send4bitsControlCmd(CMD_INIT_1));
    HAL_Delay(DELAY_1MS);
    // 0x3c - 0x38
    CHECK_INTERNAL(display_Send4bitsControlCmd(CMD_INIT_1));
    // 0x2c - 0x28
    CHECK_INTERNAL(display_Send4bitsControlCmd(CMD_INIT_2));

    // Custom settings
    CHECK_INTERNAL(display_Send8bitsControlCmd(CMD_FUNCTION_SET_4B_2L));
    CHECK_INTERNAL(display_Send8bitsControlCmd(INSTR_DISPLAY_CONTROL));
    CHECK_INTERNAL(display_Send8bitsControlCmd(INSTR_RETURN_HOME));
    CHECK_INTERNAL(display_Send8bitsControlCmd(CMD_ENTRY_MODE_INCREMENT));
    CHECK_INTERNAL(display_Send8bitsControlCmd(CMD_TURN_ON_DISPLAY));
    CHECK_INTERNAL(display_Send8bitsControlCmd(INSTR_CLR_DISPLAY));

    return DISPLAY_OK;
}

display_Status_t display_Clear()
{
    CHECK_INTERNAL(display_Send8bitsControlCmd(INSTR_CLR_DISPLAY));
    HAL_Delay(DELAY_2MS);
    return DISPLAY_OK;
}

display_Status_t display_PrintChar(uint8_t ch)
{
    CHECK_INTERNAL(display_Send8bitsDataCmd(ch));
    return DISPLAY_OK;
}

display_Status_t display_PrintStringInTopLine(uint8_t * buf)
{
    CHECK_NULL(buf);

    CHECK_INTERNAL(display_Clear());
    CHECK_INTERNAL(display_Send8bitsControlCmd(CMD_SET_CURSOR_LINE1));

    for (int i = 0; i < MAX_CHAR_PER_LINE && buf[i] != '\0'; i++) {
        CHECK_INTERNAL(display_PrintChar(buf[i]));
    }

    return DISPLAY_OK;
}

display_Status_t display_PrintStringInBottomLine(uint8_t * buf)
{
    CHECK_NULL(buf);

    CHECK_INTERNAL(display_Clear());
    CHECK_INTERNAL(display_Send8bitsControlCmd(CMD_SET_CURSOR_LINE2));

    for (int i = 0; i < MAX_CHAR_PER_LINE && buf[i] != '\0'; i++) {
        CHECK_INTERNAL(display_PrintChar(buf[i]));
    }

    return DISPLAY_OK;
}
