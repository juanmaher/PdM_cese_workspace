/**
 ******************************************************************************
 * @file    API_nrf24.h
 * @author  Juan Manuel Hernández
 * @brief   Header for API_nrf24.c module
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __API_NRF24_H
#define __API_NRF24_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_hal_exti.h"

typedef bool bool_t;
/* Exported types ------------------------------------------------------------*/
typedef enum {
    NRF24_OK,
    ERROR_NULL,
    ERROR_IO,
    ERROR_HAL,
    ERROR_INVALID_OPTION,
    ERROR_INVALID_MODE,
    ERROR_TX_FULL,
    ERROR_RX_EMPTY,
    ERROR_TIMEOUT
} nRF24_Status_t;

typedef enum {
    RF_POWER_18DBM = 0x0,
    RF_POWER_12DBM = 0x2,
    RF_POWER_6DBM = 0x4,
    RF_POWER_0DBM = 0x6
} nRF24_RfPower_t;

typedef enum {
    RF_DR_1MBPS = 0x0,
    RF_DR_2MBPS = 0x8,
    RF_DR_250KBPS = 0x20
} nRF24_RfDataRate_t;

typedef enum {
    CRC_ENCODING_1_BYTE,
    CRC_ENCODING_2_BYTES
} nRF24_CrcEncoding_t;

typedef enum {
    AW_3_BYTES = 0x1,
    AW_4_BYTES,
    AW_5_BYTES
} nRF24_Aw_t;

typedef enum {
    TRANSMITTION_INIT,
    TRANSMITTION_DONE,
    TRANSMITTION_IN_PROGRESS,
    TRANSMITTION_FAILED
} nRF24_TxStatus_t;

typedef enum {
    STANDBYI_MODE,
    TRANSMITTER_MODE,
    RECEIVER_MODE
} nRF24_Mode_t;

typedef enum {
    EVENT_MAX_RT=4,
    EVENT_TX_DS,
    EVENT_RX_DR,
	EVENT_GPIO_IRQ,
} nRF24_IRQ_EVENT;

typedef struct __nRF24_InitTypeDef {
    /* CONFIG */
    uint8_t CrcEnable;
    nRF24_CrcEncoding_t CrcEncodingScheme;
    //uint8_t TxRxControl; no va porque es lo que controla si es PRX o PTX

    /* EN_AA */
    uint8_t AutoAckEnable;
    uint8_t AutoAckDataPipes;

    /* EN_RXADDR */
    uint8_t RxDataPipes;

    /* SETUP_AW */
    nRF24_Aw_t Aw;

    /* RF_CH */
    uint8_t RfChannel;

    /* RF_SETUP */
    uint8_t ContWave;
    nRF24_RfPower_t RfPower;
    nRF24_RfDataRate_t RfDataRate;

    /* DPL */
    uint8_t DplEnable;
    uint8_t DplEnableDataPipes;

    /* EN_DYN_ACK */
    uint8_t DynAckEnable;
} nRF24_InitTypeDef;

typedef struct __nRF24_HandleTypeDef {
    nRF24_InitTypeDef Init;
    uint8_t StatusRegister;
    nRF24_Mode_t DeviceMode;
} nRF24_HandleTypeDef;

/* Exported constants --------------------------------------------------------*/

/* Transtions Delay */
#define TX_DELAY_MS             1
#define RX_DELAY_MS             1
#define START_UP_DELAY_MS       2
#define POWER_ON_DELAY_MS       100

/* Register MAP */
#define CONFIG                  0x00
#define MASK_RX_DR              (1 << 6)
#define MASK_TX_DS              (1 << 5)
#define MASK_MAX_RT             (1 << 4)
#define EN_CRC                  (1 << 3)
#define CRCO                    (1 << 2)
#define PWR_UP                  (1 << 1)
#define PRIM_RX                 (1 << 0)
// #define CONFIG_DFT_VALUE        ( EN_CRC )

#define EN_AA                   0x01
#define ENAA_P5                 (1 << 5)
#define ENAA_P4                 (1 << 4)
#define ENAA_P3                 (1 << 3)
#define ENAA_P2                 (1 << 2)
#define ENAA_P1                 (1 << 1)
#define ENAA_P0                 (1 << 0)
// #define EN_AA_DFT_VALUE         ( ENAA_P5 | ENAA_P4 | ENAA_P3 | ENAA_P2 | ENAA_P1 | ENAA_P0 )

#define EN_RXADDR               0x02
#define ERX_P5                  (1 << 5)
#define ERX_P4                  (1 << 4)
#define ERX_P3                  (1 << 3)
#define ERX_P2                  (1 << 2)
#define ERX_P1                  (1 << 1)
#define ERX_P0                  (1 << 0)
// #define EN_RXADDR_DFT_VALUE     ( ERX_P1 | ERX_P0 )

#define SETUP_AW                0x03
#define AW_1                    (1 << 1)
#define AW_0                    (1 << 0)
// #define SETUP_AW_DFT_VALUE      ( AW_1 | AW_0 )

#define SETUP_RETR              0x04
#define ARD_7                   (1 << 7)
#define ARD_6                   (1 << 6)
#define ARD_5                   (1 << 5)
#define ARD_4                   (1 << 4)
#define ARC_3                   (1 << 3)
#define ARC_2                   (1 << 2)
#define ARC_1                   (1 << 1)
#define ARC_0                   (1 << 0)
// #define SETUP_RETR_DFT_VALUE    ( ARC_1 | ARC_0 )

#define RF_CH                   0x05
#define RF_CH_6                 (1 << 6)
#define RF_CH_5                 (1 << 5)
#define RF_CH_4                 (1 << 4)
#define RF_CH_3                 (1 << 3)
#define RF_CH_2                 (1 << 2)
#define RF_CH_1                 (1 << 1)
#define RF_CH_0                 (1 << 0)
// #define RF_CH_DFT_VALUE         ( RF_CH_1 )

#define RF_SETUP                0x06
#define CONT_WAVE               (1 << 7)
#define RF_DR_LOW               (1 << 5)
#define PLL_LOCK                (1 << 4)
#define RF_DR_HIGH              (1 << 3)
#define RF_PWR_2                (1 << 2)
#define RF_PWR_1                (1 << 1)
// #define RF_SETUP_DFT_VALUE      ( RF_DR_HIGH | RF_PWR_2 | RF_PWR_1 )

#define STATUS                  0x07
#define RX_DR                   (1 << 6)
#define TX_DS                   (1 << 5)
#define MAX_RT                  (1 << 4)
#define RX_P_NO_3               (1 << 3)
#define RX_P_NO_2               (1 << 2)
#define RX_P_NO_1               (1 << 1)
#define TX_FIFO_FULL            (1 << 0)
// #define STATUS_DFT_VALUE        ( RX_P_NO_3 | RX_P_NO_2 | RX_P_NO_1 )

#define OBSERVE_TX              0x08
#define PLOS_CNT_7              (1 << 7)
#define PLOS_CNT_6              (1 << 6)
#define PLOS_CNT_5              (1 << 5)
#define PLOS_CNT_4              (1 << 4)
#define ARC_CNT_3               (1 << 3)
#define ARC_CNT_2               (1 << 2)
#define ARC_CNT_1               (1 << 1)
#define ARC_CNT_0               (1 << 0)
// #define OBSERVE_TX_DFT_VALUE    ( 0x00U )

#define RPD                     0x09
#define RPD_0                   (1 << 0)
// #define RPD_DFT_VALUE           ( 0x00U )

#define RX_ADDR_P0              0x0A
// #define RX_ADDR_PO_DFT_VALUE    ( 0xE7E7E7E7E7 )
#define RX_ADDR_P1              0x0B
// #define RX_ADDR_P1_DFT_VALUE    ( 0xC2C2C2C2C2 )
#define RX_ADDR_P2              0x0C
// #define RX_ADDR_P2_DFT_VALUE    ( 0xC3 )
#define RX_ADDR_P3              0x0D
// #define RX_ADDR_P3_DFT_VALUE    ( 0xC4 )
#define RX_ADDR_P4              0x0E
// #define RX_ADDR_P4_DFT_VALUE    ( 0xC5 )
#define RX_ADDR_P5              0x0F
// #define RX_ADDR_P5_DFT_VALUE    ( 0xC6 )

#define TX_ADDR                 0x10
// #define TX_ADDR_DFT_VALUE       ( 0xE7E7E7E7E7 )

#define RX_PW_P0                0x11
#define RX_PW_P0_5              (1 << 5)
#define RX_PW_P0_4              (1 << 4)
#define RX_PW_P0_3              (1 << 3)
#define RX_PW_P0_2              (1 << 2)
#define RX_PW_P0_1              (1 << 1)
#define RX_PW_P0_0              (1 << 0)
// #define RX_PW_P0_DFT_VALUE      ( 0x00U )

#define RX_PW_P1                0x12
#define RX_PW_P1_5              (1 << 5)
#define RX_PW_P1_4              (1 << 4)
#define RX_PW_P1_3              (1 << 3)
#define RX_PW_P1_2              (1 << 2)
#define RX_PW_P1_1              (1 << 1)
#define RX_PW_P1_0              (1 << 0)
// #define RX_PW_P1_DFT_VALUE      ( 0x00U )

#define RX_PW_P2                0x13
#define RX_PW_P2_5              (1 << 5)
#define RX_PW_P2_4              (1 << 4)
#define RX_PW_P2_3              (1 << 3)
#define RX_PW_P2_2              (1 << 2)
#define RX_PW_P2_1              (1 << 1)
#define RX_PW_P2_0              (1 << 0)
// #define RX_PW_P2_DFT_VALUE      ( 0x00U )

#define RX_PW_P3                0x14
#define RX_PW_P3_5              (1 << 5)
#define RX_PW_P3_4              (1 << 4)
#define RX_PW_P3_3              (1 << 3)
#define RX_PW_P3_2              (1 << 2)
#define RX_PW_P3_1              (1 << 1)
#define RX_PW_P3_0              (1 << 0)
// #define RX_PW_P3_DFT_VALUE      ( 0x00U )

#define RX_PW_P4                0x15
#define RX_PW_P4_5              (1 << 5)
#define RX_PW_P4_4              (1 << 4)
#define RX_PW_P4_3              (1 << 3)
#define RX_PW_P4_2              (1 << 2)
#define RX_PW_P4_1              (1 << 1)
#define RX_PW_P4_0              (1 << 0)
// #define RX_PW_P4_DFT_VALUE      ( 0x00U )

#define RX_PW_P5                0x16
#define RX_PW_P5_5              (1 << 5)
#define RX_PW_P5_4              (1 << 4)
#define RX_PW_P5_3              (1 << 3)
#define RX_PW_P5_2              (1 << 2)
#define RX_PW_P5_1              (1 << 1)
#define RX_PW_P5_0              (1 << 0)
// #define RX_PW_P5_DFT_VALUE      ( 0x00U )

#define FIFO_STATUS             0x17
#define TX_REUSE                (1 << 6)
#define TX_FULL                 (1 << 5)
#define TX_EMPTY                (1 << 4)
#define RX_FULL                 (1 << 1)
#define RX_EMPTY                (1 << 0)
// #define FIFO_STATUS_DFT_VALUE   ( TX_EMPTY | RX_EMPTY )

#define DYNPD                   0x1C
#define DPL_P5                  (1 << 5)
#define DPL_P4                  (1 << 4)
#define DPL_P3                  (1 << 3)
#define DPL_P2                  (1 << 2)
#define DPL_P1                  (1 << 1)
#define DPL_P0                  (1 << 0)
// #define DYNPD_DFT_VALUE         ( 0x00U )

#define FEATURE                 0x1D
#define EN_DPL                  (1 << 2)
#define EN_ACK_PAY              (1 << 1)
#define EN_DYN_ACK              (1 << 0)
// #define FEATURE_DFT_VALUE       ( 0x00U )

/* Commands definition */
#define R_REGISTER              0b00000000
#define W_REGISTER              0b00100000
#define R_RX_PAYLOAD            0b01100001
#define W_TX_PAYLOAD            0b10100000
#define FLUSH_TX                0b11100001
#define FLUSH_RX                0b11100010
#define REUSE_TX_PL             0b11100011
#define R_RX_PL_WID             0b01100000
#define W_ACK_PAYLOAD           0b10101000
#define W_TX_PAYLOAD_NOACK      0b10110000
#define NOP                     0b11111111

/* Commands masks */
#define R_REGISTER_MASK         0b11100000
#define W_REGISTER_MASK         0b11100000

/* SPI definition */
#define SPIx                                     SPI1
#define SPIx_CLK_ENABLE()                        __HAL_RCC_SPI1_CLK_ENABLE()

#define SPIx_SCK_AF                              GPIO_AF5_SPI1
#define SPIx_SCK_GPIO_PORT                       GPIOA
#define SPIx_SCK_PIN                             GPIO_PIN_5
#define SPIx_SCK_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_SCK_GPIO_CLK_DISABLE()              __HAL_RCC_GPIOA_CLK_DISABLE()

#define SPIx_MISO_MOSI_AF                        GPIO_AF5_SPI1
#define SPIx_MISO_MOSI_GPIO_PORT                 GPIOA
#define SPIx_MISO_MOSI_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_MISO_MOSI_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOA_CLK_DISABLE()
#define SPIx_MISO_PIN                            GPIO_PIN_6
#define SPIx_MOSI_PIN                            GPIO_PIN_7
/* Maximum Timeout values for flags waiting loops. These timeout are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */
#define SPIx_TIMEOUT_MAX                   10000

#define SPIx_CS_GPIO_PORT                        GPIOA
#define SPIx_CS_PIN                              GPIO_PIN_4
#define SPIx_CS_GPIO_CLK_ENABLE()                __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_CS_GPIO_CLK_DISABLE()               __HAL_RCC_GPIOA_CLK_DISABLE()

#define SPIx_CS_LOW()          HAL_GPIO_WritePin(SPIx_CS_GPIO_PORT, SPIx_CS_PIN, GPIO_PIN_RESET)
#define SPIx_CS_HIGH()         HAL_GPIO_WritePin(SPIx_CS_GPIO_PORT, SPIx_CS_PIN, GPIO_PIN_SET)

/**
  * @brief  nRF24 Control Interface pins (shield D4)
  */
#define nRF24_CE_PIN                                GPIO_PIN_8
#define nRF24_CE_GPIO_PORT                          GPIOA
#define nRF24_CE_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOA_CLK_ENABLE()
#define nRF24_CE_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOA_CLK_DISABLE()
#define nRF24_CE_LOW()          HAL_GPIO_WritePin(nRF24_CE_GPIO_PORT, nRF24_CE_PIN, GPIO_PIN_RESET)
#define nRF24_CE_HIGH()         HAL_GPIO_WritePin(nRF24_CE_GPIO_PORT, nRF24_CE_PIN, GPIO_PIN_SET)

#define nRF24_IRQ_PIN                                GPIO_PIN_10
#define nRF24_IRQ_GPIO_PORT                          GPIOA
#define nRF24_IRQ_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOA_CLK_ENABLE()
#define nRF24_IRQ_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOA_CLK_DISABLE()

/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */
nRF24_Status_t nRF24_Init(nRF24_HandleTypeDef * pHnrf24);
nRF24_Status_t nRF24_Transmit(uint8_t * pTxBuffer, uint8_t length);
nRF24_Status_t nRF24_Receive(uint8_t * pRxBuffer, uint8_t length);
nRF24_Status_t nRF24_SetAutoAck(const uint8_t bit, const bool_t enable);
nRF24_Status_t nRF24_SetFeature(const uint8_t bit, const bool_t enable);
nRF24_Status_t nRF24_SetDynamicPayload(const uint8_t bit, bool_t enable);
bool_t nRF24_IsTxFull();
bool_t nRF24_IsTxEmpty();
bool_t nRF24_IsRxEmpty();
bool_t nRF24_IsRxFull();
nRF24_Status_t nRF24_GetStatus(uint8_t *status);
nRF24_Status_t nRF24_GetTransmitionStatus(nRF24_TxStatus_t *pTxStatus);

/*!
 * \brief HAL_GPIO_EXTI_Callback used by nRF24L01 driver. User must define nRF24_irq_callback
 *        to get interrupt event
 * \param event_type EVENT_RX_DR: Receiver Data Ready,
 * 					 EVENT_TX_DS: Transmitter Data Sent,
 *                   EVENT_MAX_RT:Transmitter retransmit
 *                   EVENT_GPIO_IRQ: gpio interrupt handler except nRF24L01 IRQ GPIO_pin
 *  \param data_src  nRF24L01 datapipe or GPIO_pin
 *  \param data		 nRF24L01 received data
 *  \param width	 nRF24L01 received data length
 */
void nRF24_IRQ_Callback(uint8_t event_type, uint16_t data_src, uint8_t* data, uint8_t width);


#endif /* __API_NRF24_H */
