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
static display_State_t display_state = TURN_OFF;
/* Private function prototypes -----------------------------------------------*/
static display_Status_t display_Send4bitsCmd(uint8_t value, display_RsType_t rs_type, bool bck_enable);

/* Private function in macros */
#define display_TransmitData(value)                         myI2Cx_Master_Transmit(DISPLAY_ADDR<<1, &value, sizeof(value), MAX_DELAY)
#define display_Send4bitsControlCmd(value, bck_enable)      display_Send4bitsCmd(value, RS_CONTROL, bck_enable)
#define display_Send4bitsDataCmd(value)                     display_Send4bitsCmd(value, RS_DATA, true)
#define display_Send8bitsControlCmd(value, bck_enable)      display_Send8bitsCmd(value, RS_CONTROL, bck_enable)
#define display_Send8bitsDataCmd(value)                     display_Send8bitsCmd(value, RS_DATA, true)

#define CHECK_NULL(x)                   do { \
                                            if (x == NULL) { return DISPLAY_ERROR_NULL; } \
                                        } while(0U)

#define CHECK_PORT(x)                   do { \
                                            if (x != DISPLAY_PORT_OK) { return DISPLAY_ERROR_PORT; } \
                                        } while(0U)

#define CHECK_INTERNAL(x)               do { \
                                            if (x != DISPLAY_OK) { return x; } \
                                        } while(0U)

/* Private functions ---------------------------------------------------------*/
static display_Status_t display_Send4bitsCmd(uint8_t value, display_RsType_t rs_type, bool bck_enable)
{
    uint8_t cmd = 0;
    // D = bit de datos
    // E = enable bit
    // B = enable backlight bit
    // R (R/W) = read/write bit
    // T (RS) = data or control bit
    // DDDDEBRT
    display_Delay(DELAY_1MS);
    if (bck_enable) {
        cmd = value | BIT_BACKLIGHT | BIT_ENABLE | rs_type;
        CHECK_PORT(display_TransmitData(cmd));
        display_Delay(DELAY_1MS);
        cmd = value | BIT_BACKLIGHT | rs_type;
        CHECK_PORT(display_TransmitData(cmd));
    } else {
        cmd = value | BIT_ENABLE | rs_type;
        CHECK_PORT(display_TransmitData(cmd));
        display_Delay(DELAY_1MS);
        cmd = value | rs_type;
        CHECK_PORT(display_TransmitData(cmd));
    }

    return DISPLAY_OK;
}

static display_Status_t display_Send8bitsCmd(uint8_t value, display_RsType_t rs_type, bool bck_enable)
{
    uint8_t cmd = 0;
    cmd = value & HIGH_NIBBLE_MASK;
    CHECK_INTERNAL(display_Send4bitsCmd(cmd, rs_type, bck_enable));
    cmd = value << HIGH_NIBBLE_SHIFT;
    CHECK_INTERNAL(display_Send4bitsCmd(cmd, rs_type, bck_enable));
    return DISPLAY_OK;
}



/* Public functions ----------------------------------------------------------*/
display_Status_t display_Init()
{
    CHECK_PORT(myI2Cx_Init());

    // Initialization sequence recommended by the datasheet
    display_Delay(DELAY_20MS);
    // 0x3c - 0x38
    CHECK_INTERNAL(display_Send4bitsControlCmd(CMD_INIT_1, false));
    display_Delay(DELAY_10MS);
    // 0x3c - 0x38
    CHECK_INTERNAL(display_Send4bitsControlCmd(CMD_INIT_1, false));
    display_Delay(DELAY_1MS);
    // 0x3c - 0x38
    CHECK_INTERNAL(display_Send4bitsControlCmd(CMD_INIT_1, false));
    // 0x2c - 0x28
    CHECK_INTERNAL(display_Send4bitsControlCmd(CMD_INIT_2, false));

    // Custom settings
    CHECK_INTERNAL(display_Send8bitsControlCmd(CMD_FUNCTION_SET_4B_2L, false));
    CHECK_INTERNAL(display_Send8bitsControlCmd(INSTR_DISPLAY_CONTROL, false));
    CHECK_INTERNAL(display_Send8bitsControlCmd(INSTR_RETURN_HOME, false));
    CHECK_INTERNAL(display_Send8bitsControlCmd(CMD_ENTRY_MODE_INCREMENT, false));
    CHECK_INTERNAL(display_Send8bitsControlCmd(INSTR_CLR_DISPLAY, false));

    return DISPLAY_OK;
}

display_Status_t display_Clear()
{
    CHECK_INTERNAL(display_Send8bitsControlCmd(INSTR_CLR_DISPLAY, true));
    display_Delay(DELAY_2MS);
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

    CHECK_INTERNAL(display_Send8bitsControlCmd(CMD_SET_CURSOR_LINE1, true));

    for (int i = 0; i < MAX_CHAR_PER_LINE && buf[i] != '\0'; i++) {
        CHECK_INTERNAL(display_PrintChar(buf[i]));
    }

    return DISPLAY_OK;
}

display_Status_t display_PrintStringInBottomLine(uint8_t * buf)
{
    CHECK_NULL(buf);

    CHECK_INTERNAL(display_Send8bitsControlCmd(CMD_SET_CURSOR_LINE2, true));

    for (int i = 0; i < MAX_CHAR_PER_LINE && buf[i] != '\0'; i++) {
        CHECK_INTERNAL(display_PrintChar(buf[i]));
    }

    return DISPLAY_OK;
}

display_Status_t display_TurnOn()
{
    CHECK_INTERNAL(display_Send8bitsControlCmd(CMD_TURN_ON_DISPLAY, true));
    display_state = TURN_ON;
    return DISPLAY_OK;
}

display_Status_t display_TurnOff()
{
    CHECK_INTERNAL(display_Send8bitsControlCmd(CMD_TURN_OFF_DISPLAY, false));
    display_state = TURN_OFF;
    return DISPLAY_OK;
}

display_State_t display_GetState()
{
    return display_state;
}
