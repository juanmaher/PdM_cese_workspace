/**
 ******************************************************************************
 * @file    .h
 * @author  Juan Manuel Hernández
 * @brief   Header for .c module
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __API_DISPLAY_H
#define __API_DISPLAY_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "API_display_port.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {
    DISPLAY_OK = 0,
    DISPLAY_ERROR_NULL,
    DISPLAY_ERROR_PORT
} display_Status_t;

typedef enum {
    TURN_OFF = 0,
    TURN_ON = 1
} display_State_t;

/* Exported constants --------------------------------------------------------*/

/* Display control */
#define BIT_ENABLE                          (1 << 2)
#define BIT_BACKLIGHT                       (1 << 3)
#define DISPLAY_ADDR                        0x27
#define LINE1_POSITION                      0x80
#define LINE2_POSITION                      0xc0
#define MAX_CHAR_PER_LINE                   16
#define MAX_DELAY                           0xFFFFFFFFU
#define CHARACTER_FULL                      0xFF
#define CHARACTER_EMPTY                     0x0

/* Instruction list */
#define INSTR_CLR_DISPLAY                   (1 << 0) // Clear display

#define INSTR_RETURN_HOME                   (1 << 1) // Return home

#define INSTR_ENTRY_MODE                    (1 << 2) // Entry mode set
#define INSTR_ENTRY_MODE_ID                 (1 << 1) // Increment/decrement
#define INSTR_ENTRY_MODE_S                  (1 << 0) // Shift

#define INSTR_DISPLAY_CONTROL               (1 << 3) // Display on/off control
#define INSTR_DISPLAY_CONTROL_D             (1 << 2) // Display on/off
#define INSTR_DISPLAY_CONTROL_C             (1 << 1) // Cursor on/off
#define INSTR_DISPLAY_CONTROL_B             (1 << 0) // Cursor blink on/off

#define INSTR_CURSOR_DISPLAY_SHIFT          (1 << 4) // Cursor or display shift
#define INSTR_CURSOR_DISPLAY_SHIFT_SC       (1 << 3) // Shift or cursor move
#define INSTR_CURSOR_DISPLAY_SHIFT_RL       (1 << 2) // Shift right or left

#define INSTR_FUNTION_SET                   (1 << 5) // Function set
#define INSTR_FUNTION_SET_DL                (1 << 4) // Interface data length
#define INSTR_FUNTION_SET_N                 (1 << 3) // Number of display lines
#define INSTR_FUNTION_SET_F                 (1 << 2) // Font

#define INSTR_SET_CGRAM                     (1 << 6) // Set CGRAM address
#define INSTR_SET_DDRAM                     (1 << 7) // Set DDRAM address

#define INSTR_BUSY                          (1 << 7) // Busy flag

/* Common commands */
// Set cursor
#define CMD_SET_CURSOR_LINE1                LINE1_POSITION
#define CMD_SET_CURSOR_LINE2                LINE2_POSITION

// Entry mode set
#define CMD_ENTRY_MODE_INCREMENT            (INSTR_ENTRY_MODE | INSTR_ENTRY_MODE_ID)                                // 0x06 Increment
#define CMD_ENTRY_MODE_DECREMENT            INSTR_ENTRY_MODE                                                        // 0x04 Decrement

// Display on/off control
#define CMD_TURN_ON_DISPLAY                 (INSTR_DISPLAY_CONTROL | INSTR_DISPLAY_CONTROL_D)                       // 0x0E Turn on display
#define CMD_TURN_OFF_DISPLAY                INSTR_DISPLAY_CONTROL                                                   // 0x08 Turn off display
#define CMD_CURSOR_ON                       (CMD_TURN_ON_DISPLAY | INSTR_DISPLAY_CONTROL_C)                         // 0x0E Cursor on
#define CMD_CURSOR_OFF                      CMD_TURN_ON_DISPLAY                                                     // 0x0C Cursor off
#define CMD_BLINK_CURSOR_ON                 (CMD_CURSOR_ON | INSTR_DISPLAY_CONTROL_B)                               // 0x0F Blink cursor on
#define CMD_BLINK_CURSOR_OFF                CMD_TURN_ON_DISPLAY                                                     // 0x0E Blink cursor off

// Function set
#define CMD_FUNCTION_SET_4B_1L              INSTR_FUNTION_SET                                                       // 0x20 4-bit interface, 1 line
#define CMD_FUNCTION_SET_4B_2L              (INSTR_FUNTION_SET | INSTR_FUNTION_SET_N)                               // 0x28 4-bit interface, 2 lines
#define CMD_FUNCTION_SET_8B_2L              (INSTR_FUNTION_SET | INSTR_FUNTION_SET_DL | INSTR_FUNTION_SET_N)        // 0x38 8-bit interface, 2 lines

// Initialization
#define CMD_INIT_1                          0x30
#define CMD_INIT_2                          0x20

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
display_Status_t display_Init();
display_Status_t display_Clear();
display_Status_t display_PrintChar(uint8_t ch);
display_Status_t display_PrintStringInTopLine(uint8_t * buf);
display_Status_t display_PrintStringInBottomLine(uint8_t * buf);
display_Status_t display_TurnOn();
display_Status_t display_TurnOff();
display_State_t display_GetState();

#endif /* __API_DISPLAY_H */
