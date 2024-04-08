/**
 ******************************************************************************
 * @file    API_nrf24.c
 * @author  Juan Manuel Hernández
 * @brief   Implementation of RF communication.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "API_nrf24.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum {
    POWER_DOWN,
    STANDBYI,
    STANDBYII,
    PRX,
    PTX
} operationModes_t;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static nRF24_Status_t nRF24_Reset();
static nRF24_Status_t nRF24_UpdateMode(const operationModes_t mode);
static nRF24_Status_t nRF24_SendCmd(uint8_t cmd, uint8_t * value, const uint8_t length);
static nRF24_Status_t nRF24_SetRegister(const uint8_t registerName, const uint8_t bit);
static nRF24_Status_t nRF24_ResetRegister(const uint8_t registerName, const uint8_t bit);
static void mySPIx_Init();
static void myGPIO_Init();

/* Private function in macros */
#define nRF24_SendReadCmd(addr, value)                      nRF24_SendCmd(R_REGISTER | addr, value, 1)
#define nRF24_SendWriteCmd(addr, value)                     nRF24_SendCmd(W_REGISTER | addr, value, 1)
#define nRF24_SendReadRxPlCmd(value, len)                   nRF24_SendCmd(R_RX_PAYLOAD, value, len)
#define nRF24_SendReadRxPlWidthCmd(value)                   nRF24_SendCmd(R_RX_PL_WID, value, 1)
#define nRF24_SendWriteTxPlCmd(value, len)                  nRF24_SendCmd(W_TX_PAYLOAD, value, len)
#define nRF24_SendFlushTx()                                 nRF24_SendCmd(FLUSH_TX, 0, 1)
#define nRF24_SendFlushRx()                                 nRF24_SendCmd(FLUSH_RX, 0, 1)
#define nRF24_SendReuseTxPlCmd()                            nRF24_SendCmd(REUSE_TX_PL, 0, 1)
#define nRF24_SendReadRxPlWidCmd()                          nRF24_SendCmd(W_TX_PAYLOAD_NOACK, 0, 1)
#define nRF24_SendWriteTxPlNoAckCmd(pipe, value, len)       nRF24_SendCmd(W_TX_PAYLOAD_NOACK | pipe, value, len)
#define nRF24_SendWriteAckPlCmd(value, len)                 nRF24_SendCmd(W_ACK_PAYLOAD, value, len)

// #define nRF24_SetIRQ(port, pin)                     HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET)
// #define nRF24_ResetIRQ(port, pin)                   HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET)

#define CHECK_NULL(x)                   do { \
                                            if (x == NULL) { return ERROR_NULL; } \
                                        } while(0U)

#define CHECK_SPI(x)                    do { \
                                            if (x != HAL_OK) { return ERROR_HAL; } \
                                        } while(0U)
                                    
#define CHECK_INTERNAL(x)               do { \
                                            if (x != NRF24_OK) { return x; } \
                                        } while(0U)

/* Private variables ---------------------------------------------------------*/
static operationModes_t operationCurrentMode = POWER_DOWN;
static SPI_HandleTypeDef hspi1;
static nRF24_HandleTypeDef * hnrf24;

static uint8_t CONFIG_DFT_VALUE = EN_CRC; // 0x08
static uint8_t EN_AA_DFT_VALUE = ENAA_P5 | ENAA_P4 | ENAA_P3 | ENAA_P2 | ENAA_P1 | ENAA_P0; // 0x3F
static uint8_t EN_RXADDR_DFT_VALUE = ERX_P1 | ERX_P0; // 0x03
static uint8_t SETUP_AW_DFT_VALUE = AW_1 | AW_0; // 0x03
static uint8_t SETUP_RETR_DFT_VALUE = ARC_1 | ARC_0; // 0x03
static uint8_t RF_CH_DFT_VALUE = RF_CH_1; // 0x02
static uint8_t RF_SETUP_DFT_VALUE = RF_DR_HIGH | RF_PWR_2 | RF_PWR_1; // 0x2E
static uint8_t STATUS_DFT_VALUE = RX_P_NO_3 | RX_P_NO_2 | RX_P_NO_1; // 0x0E
// static uint8_t RPD_DFT_VALUE = 0x00;
// static uint8_t RX_ADDR_PO_DFT_VALUE = 0xE7E7E7E7E7;
// static uint8_t RX_ADDR_P1_DFT_VALUE = 0xC2C2C2C2C2;
// static uint8_t RX_ADDR_P2_DFT_VALUE = 0xC3;
// static uint8_t RX_ADDR_P3_DFT_VALUE = 0xC4;
// static uint8_t RX_ADDR_P4_DFT_VALUE = 0xC5;
// static uint8_t RX_ADDR_P5_DFT_VALUE = 0xC6;
// static uint8_t TX_ADDR_DFT_VALUE = 0xE7E7E7E7E7;
static uint8_t RX_PW_P0_DFT_VALUE = 0x00; // 0x00
static uint8_t RX_PW_P1_DFT_VALUE = 0x00; // 0x00
static uint8_t RX_PW_P2_DFT_VALUE = 0x00; // 0x00
static uint8_t RX_PW_P3_DFT_VALUE = 0x00; // 0x00
static uint8_t RX_PW_P4_DFT_VALUE = 0x00; // 0x00
static uint8_t RX_PW_P5_DFT_VALUE = 0x00; // 0x00
static uint8_t FIFO_STATUS_DFT_VALUE = TX_EMPTY | RX_EMPTY; // 0x11
static uint8_t DYNPD_DFT_VALUE = 0x00; // 0x00
static uint8_t FEATURE_DFT_VALUE = 0x00; // 0x00

/* Private functions ---------------------------------------------------------*/
static nRF24_Status_t nRF24_Reset()
{
    CHECK_INTERNAL(nRF24_SendWriteCmd(CONFIG, &CONFIG_DFT_VALUE));
    CHECK_INTERNAL(nRF24_SendWriteCmd(EN_AA, &EN_AA_DFT_VALUE));
    CHECK_INTERNAL(nRF24_SendWriteCmd(EN_RXADDR, &EN_RXADDR_DFT_VALUE));
    CHECK_INTERNAL(nRF24_SendWriteCmd(SETUP_AW, &SETUP_AW_DFT_VALUE));
    CHECK_INTERNAL(nRF24_SendWriteCmd(SETUP_RETR, &SETUP_RETR_DFT_VALUE));
    CHECK_INTERNAL(nRF24_SendWriteCmd(RF_CH, &RF_CH_DFT_VALUE));
    CHECK_INTERNAL(nRF24_SendWriteCmd(RF_SETUP, &RF_SETUP_DFT_VALUE));
    CHECK_INTERNAL(nRF24_SendWriteCmd(STATUS, &STATUS_DFT_VALUE));
    // CHECK_INTERNAL(nRF24_SendWriteCmd(RX_ADDR_P0, &RX_ADDR_PO_DFT_VALUE));
    // CHECK_INTERNAL(nRF24_SendWriteCmd(RX_ADDR_P1, &RX_ADDR_P1_DFT_VALUE));
    // CHECK_INTERNAL(nRF24_SendWriteCmd(RX_ADDR_P2, &RX_ADDR_P2_DFT_VALUE));
    // CHECK_INTERNAL(nRF24_SendWriteCmd(RX_ADDR_P3, &RX_ADDR_P3_DFT_VALUE));
    // CHECK_INTERNAL(nRF24_SendWriteCmd(RX_ADDR_P4, &RX_ADDR_P4_DFT_VALUE));
    // CHECK_INTERNAL(nRF24_SendWriteCmd(RX_ADDR_P5, &RX_ADDR_P5_DFT_VALUE));
    // CHECK_INTERNAL(nRF24_SendWriteCmd(TX_ADDR, &TX_ADDR_DFT_VALUE));
    CHECK_INTERNAL(nRF24_SendWriteCmd(RX_PW_P0, &RX_PW_P0_DFT_VALUE));
    CHECK_INTERNAL(nRF24_SendWriteCmd(RX_PW_P1, &RX_PW_P1_DFT_VALUE));
    CHECK_INTERNAL(nRF24_SendWriteCmd(RX_PW_P2, &RX_PW_P2_DFT_VALUE));
    CHECK_INTERNAL(nRF24_SendWriteCmd(RX_PW_P3, &RX_PW_P3_DFT_VALUE));
    CHECK_INTERNAL(nRF24_SendWriteCmd(RX_PW_P4, &RX_PW_P4_DFT_VALUE));
    CHECK_INTERNAL(nRF24_SendWriteCmd(RX_PW_P5, &RX_PW_P5_DFT_VALUE));
    CHECK_INTERNAL(nRF24_SendWriteCmd(FIFO_STATUS, &FIFO_STATUS_DFT_VALUE));
    CHECK_INTERNAL(nRF24_SendWriteCmd(DYNPD, &DYNPD_DFT_VALUE));
    CHECK_INTERNAL(nRF24_SendWriteCmd(FEATURE, &FEATURE_DFT_VALUE));

    return NRF24_OK;
}

static nRF24_Status_t nRF24_UpdateMode(const operationModes_t mode)
{
    uint8_t register_value = 0;

    if (operationCurrentMode == mode) {
        return NRF24_OK;
    }

    CHECK_INTERNAL(nRF24_SendReadCmd(CONFIG, &register_value));

    switch (operationCurrentMode) {
        case POWER_DOWN:
            if (mode == STANDBYI) {
                register_value |= PWR_UP;
                CHECK_INTERNAL(nRF24_SendWriteCmd(CONFIG, &(register_value)));
                HAL_Delay(START_UP_DELAY_MS);
            }
            break;
        case STANDBYI:
            switch (mode) {
                case POWER_DOWN:
                    register_value &= ~PWR_UP;
                    CHECK_INTERNAL(nRF24_SendWriteCmd(CONFIG, &(register_value)));
                    break;
                case PRX:
                    register_value |= PRIM_RX;
                    CHECK_INTERNAL(nRF24_SendWriteCmd(CONFIG, &(register_value)));
                    nRF24_CE_HIGH();
                    HAL_Delay(RX_DELAY_MS);
                    break;
                case PTX:
                    // if (nRF24_IsTxEmpty()) {
                    //     return ERROR_INVALID_MODE;
                    // }
                    register_value |= PRIM_RX;
                    CHECK_INTERNAL(nRF24_SendWriteCmd(CONFIG, &(register_value)));
                    nRF24_CE_HIGH();
                    HAL_Delay(TX_DELAY_MS);
                    break;
                default:
                    register_value &= ~PWR_UP;
                    CHECK_INTERNAL(nRF24_SendWriteCmd(CONFIG, &(register_value)));
                    break;
            }
            break;

        case PRX:
        case PTX:
            if (mode == STANDBYI) {
                nRF24_CE_LOW();
            }
            break;
        default:
            break;
    }

    operationCurrentMode = mode;

    return NRF24_OK;
}

static nRF24_Status_t nRF24_SendCmd(uint8_t cmd, uint8_t * value, const uint8_t length)
{
    /* Start SPI communication */
    SPIx_CS_LOW();

    HAL_Delay(1);

    /* Send command */
    CHECK_SPI(HAL_SPI_Transmit(&hspi1, &cmd, 1, SPIx_TIMEOUT_MAX));

    if (cmd & W_REGISTER || cmd == W_TX_PAYLOAD || cmd == W_TX_PAYLOAD_NOACK || cmd == W_ACK_PAYLOAD) {
        CHECK_SPI(HAL_SPI_Transmit(&hspi1, value, length, SPIx_TIMEOUT_MAX));
    } else if (cmd & R_REGISTER || cmd == R_RX_PAYLOAD || cmd == R_RX_PL_WID) {
        CHECK_SPI(HAL_SPI_Receive(&hspi1, value, length, SPIx_TIMEOUT_MAX));
    }

    HAL_Delay(1);

    /* Stop SPI communication */
    SPIx_CS_HIGH();

    HAL_Delay(1);

    return NRF24_OK;
}

static nRF24_Status_t nRF24_SetRegister(const uint8_t registerName, const uint8_t bit)
{
    uint8_t registerValue = 0;

    CHECK_INTERNAL(nRF24_SendReadCmd(registerName, &registerValue));
    registerValue |= bit;
    CHECK_INTERNAL(nRF24_SendWriteCmd(registerName, &(registerValue)));

    return NRF24_OK;
}

static nRF24_Status_t nRF24_ResetRegister(const uint8_t registerName, const uint8_t bit)
{
    uint8_t registerValue = 0;

    CHECK_INTERNAL(nRF24_SendReadCmd(registerName, &registerValue));
    registerValue &= ~(bit);
    CHECK_INTERNAL(nRF24_SendWriteCmd(registerName, &(registerValue)));

    return NRF24_OK;
}

/**
  * @brief  Initializes SPI MSP.
  */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    /*** Configure the GPIOs ***/
    /* Enable GPIO clock */
    SPIx_SCK_GPIO_CLK_ENABLE();
    SPIx_MISO_MOSI_GPIO_CLK_ENABLE();
    SPIx_CS_GPIO_CLK_ENABLE();

    /* Configure SPI SCK */
    GPIO_InitStruct.Pin = SPIx_SCK_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = SPIx_SCK_AF;
    HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);

    /* Configure SPI MISO and MOSI */
    GPIO_InitStruct.Pin = SPIx_MOSI_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = SPIx_MISO_MOSI_AF;
    //GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
    HAL_GPIO_Init(SPIx_MISO_MOSI_GPIO_PORT, &GPIO_InitStruct);

    //GPIO_InitStruct.Pin = SPIx_MISO_PIN;
    //GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
    //HAL_GPIO_Init(SPIx_MISO_MOSI_GPIO_PORT, &GPIO_InitStruct);

    /* Configure SPI CS */
    GPIO_InitStruct.Pin = SPIx_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(SPIx_CS_GPIO_PORT, &GPIO_InitStruct);

    /*** Configure the SPI peripheral ***/
    /* Enable SPI clock */
    SPIx_CLK_ENABLE();
}

/**
  * @brief  Initializes SPI.
  */
static void mySPIx_Init()
{
    if(HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_RESET) {
        /* SPI Config */
        hspi1.Instance = SPIx;
        /* SPI configuration contraints
            - ST7735 LCD SPI interface max baudrate is 15MHz for write and 6.66MHz for read
                Since the provided driver doesn't use read capability from LCD, only constraint
                on write baudrate is considered.
            - SD card SPI interface max baudrate is 25MHz for write/read
        to feat these constraints SPI baudrate is set to:
            - For STM32F412ZG devices: 12,5 MHz maximum (PCLK2/SPI_BAUDRATEPRESCALER_8 = 100 MHz/8 = 12,5 MHz)
            - For STM32F446ZE/STM32F429ZI devices: 11,25 MHz maximum (PCLK2/SPI_BAUDRATEPRESCALER_8 = 90 MHz/8 = 11,25 MHz)
        */
        hspi1.Init.Mode = SPI_MODE_MASTER;
        hspi1.Init.Direction = SPI_DIRECTION_1LINE;
        hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
        hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
        hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
        hspi1.Init.NSS = SPI_NSS_SOFT;
        hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
        hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
        hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
        hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
        hspi1.Init.CRCPolynomial = 10;

        //HAL_SPI_MspInit(&hspi1);
        HAL_SPI_Init(&hspi1);
    }
}

static void myGPIO_Init()
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    /* GPIO Ports Clock Enable */
    nRF24_CE_GPIO_CLK_ENABLE();
    nRF24_IRQ_GPIO_CLK_ENABLE();

    HAL_GPIO_WritePin(nRF24_CE_GPIO_PORT, nRF24_CE_PIN, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = nRF24_CE_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(nRF24_CE_GPIO_PORT, &GPIO_InitStruct);

    //HAL_GPIO_WritePin(nRF24_IRQ_GPIO_PORT, nRF24_IRQ_PIN, GPIO_PIN_RESET);

    /* Configure IRQ GPIO pin */
    GPIO_InitStruct.Pin = nRF24_IRQ_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(nRF24_IRQ_GPIO_PORT, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    /* PROBABLEMENTE ESTO ESTE MAL */
    HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);
}

/* Public functions ----------------------------------------------------------*/
nRF24_Status_t nRF24_Init(nRF24_HandleTypeDef * pHnrf24)
{
    uint8_t register_value = 0;

    hnrf24 = pHnrf24;

    myGPIO_Init();
    mySPIx_Init();

    CHECK_INTERNAL(nRF24_Reset());

    return NRF24_OK;

    CHECK_INTERNAL(nRF24_UpdateMode(POWER_DOWN));

    CHECK_INTERNAL(nRF24_SendReadCmd(CONFIG, &register_value));
    register_value |= hnrf24->Init->CrcEnable ? EN_CRC : ~EN_CRC;
    register_value |= hnrf24->Init->CrcEncodingScheme ? CRCO : ~CRCO;
    //register_value |= hnrf24->Init->PrimRx ? PRIM_RX : ~PRIM_RX;
    CHECK_INTERNAL(nRF24_SendWriteCmd(CONFIG, &register_value));

    nRF24_SetFeature(EN_ACK_PAY, hnrf24->Init->AutoAckEnable);
    CHECK_INTERNAL(nRF24_SendReadCmd(EN_AA, &register_value));
    // Protection for AutoAckDataPipes
    hnrf24->Init->AutoAckDataPipes &= ~(0b11000000);
    register_value &= hnrf24->Init->AutoAckDataPipes;
    CHECK_INTERNAL(nRF24_SendWriteCmd(EN_AA, &register_value));

    CHECK_INTERNAL(nRF24_SendReadCmd(EN_RXADDR, &register_value));
    // Protection for RxDataPipes
    hnrf24->Init->RxDataPipes &= ~(0b11000000);
    register_value &= hnrf24->Init->RxDataPipes;
    CHECK_INTERNAL(nRF24_SendWriteCmd(EN_RXADDR, &register_value));

    CHECK_INTERNAL(nRF24_SendReadCmd(SETUP_AW, &register_value));
    register_value &= hnrf24->Init->Aw;
    CHECK_INTERNAL(nRF24_SendWriteCmd(SETUP_AW, &register_value));

    CHECK_INTERNAL(nRF24_SendReadCmd(RF_CH, &register_value));
    hnrf24->Init->RfChannel &= ~(0b10000000);
    register_value &= hnrf24->Init->RfChannel;
    CHECK_INTERNAL(nRF24_SendWriteCmd(RF_CH, &register_value));

    CHECK_INTERNAL(nRF24_SendReadCmd(RF_SETUP, &register_value));
    register_value |= hnrf24->Init->ContWave ? CONT_WAVE : ~CONT_WAVE;
    register_value &= ~(RF_DR_LOW | RF_DR_HIGH);
    register_value |= hnrf24->Init->RfDataRate;
    register_value &= ~(RF_PWR_2 | RF_PWR_1);
    register_value |= hnrf24->Init->RfPower;
    CHECK_INTERNAL(nRF24_SendWriteCmd(RF_SETUP, &register_value));

    nRF24_SetFeature(EN_DPL, hnrf24->Init->DplEnable);
    CHECK_INTERNAL(nRF24_SendReadCmd(DYNPD, &register_value));
    //Protection for DplEnableDataPipes
    hnrf24->Init->DplEnableDataPipes &= ~(0b11000000);
    register_value &= hnrf24->Init->DplEnableDataPipes;
    CHECK_INTERNAL(nRF24_SendWriteCmd(DYNPD, &register_value));

    nRF24_SetFeature(EN_DYN_ACK, hnrf24->Init->DynAckEnable);

    nRF24_UpdateMode(STANDBYI);

    if (hnrf24->DeviceMode == TRANSMITTER_MODE) {
        nRF24_UpdateMode(PTX);
    } else if (hnrf24->DeviceMode == RECEIVER_MODE) {
        nRF24_UpdateMode(PRX);
    }

    return NRF24_OK;
}

nRF24_Status_t nRF24_Transmit(uint8_t * pTxBuffer, uint8_t length)
{
    CHECK_NULL(pTxBuffer);

    // TODO: Agregar chequeo del largo
    // Maximo aceptable
    // Que no sea mas grande que el buffer

    if (operationCurrentMode != PTX) {
        return ERROR_INVALID_MODE;
    }

    if (nRF24_IsTxFull()) {
        nRF24_SendFlushTx();
    }

    // TODO: Puedo transmitir con ACK y sin ACK
    // W_TX_PAYLOAD o W_TX_PAYLOAD_NOACK
    CHECK_INTERNAL(nRF24_SendWriteTxPlCmd(pTxBuffer, length));

    return NRF24_OK;
}

nRF24_Status_t nRF24_Receive(uint8_t * pRxBuffer, uint8_t length)
{
    CHECK_NULL(pRxBuffer);

    // TODO: Agregar chequeo del largo
    // Maximo aceptable
    // Que no sea mas grande que el buffer

    if (operationCurrentMode != PRX) {
        return ERROR_INVALID_MODE;
    }

    if (nRF24_IsRxEmpty()) {
        return ERROR_RX_EMPTY;
    }

    CHECK_INTERNAL(nRF24_SendReadRxPlCmd(pRxBuffer, length));

    if (nRF24_IsRxEmpty()) {
        // Reset RX_DR flag
        CHECK_INTERNAL(nRF24_SetRegister(STATUS, RX_DR));
    }

    return NRF24_OK;
}

nRF24_Status_t nRF24_SetAutoAck(const uint8_t bit, const bool_t enable)
{
    uint8_t registerValue = 0;

    CHECK_INTERNAL(nRF24_SendReadCmd(CONFIG, &registerValue));
    if (!(registerValue & EN_CRC)) {
        registerValue |= EN_CRC;
        CHECK_INTERNAL(nRF24_SendWriteCmd(CONFIG, &(registerValue)));
    }

    if (enable) {
        CHECK_INTERNAL(nRF24_SetRegister(EN_AA, bit));
    } else {
        CHECK_INTERNAL(nRF24_ResetRegister(EN_AA, bit));
    }

    return NRF24_OK;
}

nRF24_Status_t nRF24_SetFeature(const uint8_t bit, const bool_t enable)
{
    if (enable) {
        CHECK_INTERNAL(nRF24_SetRegister(FEATURE, bit));
    } else {
        CHECK_INTERNAL(nRF24_ResetRegister(FEATURE, bit));
    }

    return NRF24_OK;
}

nRF24_Status_t nRF24_SetDynamicPayload(const uint8_t bit, bool_t enable)
{
    uint8_t registerValue = 0;

    CHECK_INTERNAL(nRF24_SendReadCmd(FEATURE, &registerValue));
    if (!(registerValue & EN_DPL)) {
        return ERROR_INVALID_OPTION;
    }

    CHECK_INTERNAL(nRF24_SendReadCmd(EN_AA, &registerValue));
    if (!(registerValue & bit)) {
        return ERROR_INVALID_OPTION;
    }

    if (enable) {
        CHECK_INTERNAL(nRF24_SetRegister(DYNPD, bit));
    } else {
        CHECK_INTERNAL(nRF24_ResetRegister(DYNPD, bit));
    }

    return NRF24_OK;
}

bool_t nRF24_IsTxFull()
{
    uint8_t register_fifo_status = 0;

    if (nRF24_SendReadCmd(FIFO_STATUS, &register_fifo_status) != NRF24_OK) {
        return false;
    }

    if (register_fifo_status & TX_FULL) {
        return true;
    }

    return false;
}

bool_t nRF24_IsTxEmpty()
{
    uint8_t register_fifo_status = 0;

    if (nRF24_SendReadCmd(FIFO_STATUS, &register_fifo_status) != NRF24_OK) {
        return false;
    }

    if (register_fifo_status & TX_EMPTY) {
        return true;
    }

    return false;
}

bool_t nRF24_IsRxEmpty()
{
    uint8_t register_fifo_status = 0;


    if (nRF24_SendReadCmd(FIFO_STATUS, &register_fifo_status) != NRF24_OK) {
        return false;
    }

    if (register_fifo_status & RX_EMPTY) {
        return true;
    }

    return false;
}

bool_t nRF24_IsRxFull()
{
    uint8_t register_fifo_status = 0;

    if (nRF24_SendReadCmd(FIFO_STATUS, &register_fifo_status) != NRF24_OK) {
        return false;
    }

    if (register_fifo_status & RX_FULL) {
        return true;
    }

    return false;
}

nRF24_Status_t nRF24_GetStatus(uint8_t *status)
{
    CHECK_NULL(status);

    CHECK_INTERNAL(nRF24_SendReadCmd(STATUS, status));
    return NRF24_OK;
}

nRF24_Status_t nRF24_GetTransmitionStatus(nRF24_TxStatus_t *pTxStatus)
{
    uint8_t register_status;

    CHECK_NULL(pTxStatus);

    CHECK_INTERNAL(nRF24_GetStatus(&register_status));

    if (register_status & TX_DS) {
        // Reset TX_DS flag
        CHECK_INTERNAL(nRF24_SetRegister(STATUS, TX_DS));
        *pTxStatus = TRANSMITTION_DONE;
    } else if (register_status & MAX_RT) {
        // Reset MAX_RT flag
        CHECK_INTERNAL(nRF24_SetRegister(STATUS, MAX_RT));
        *pTxStatus = TRANSMITTION_FAILED;
    } else {
        *pTxStatus = TRANSMITTION_IN_PROGRESS;
    }
    return NRF24_OK;
}

void HAL_GPIO_EXTI_Callback(uint16_t gpio)
{
    uint8_t status = 0, event_type = 0, width = 0;
    uint16_t data_src = 0;
    uint8_t buf[32];

    memset(buf, 0, sizeof(buf));
    if (gpio == nRF24_IRQ_PIN) {
        nRF24_GetStatus(&status);
        data_src = (status & 0x0E) >> 1;
        if ((status & 0x40) >> EVENT_RX_DR) {  // RX_DR
            event_type = EVENT_RX_DR;

            if (nRF24_SendReadRxPlWidthCmd(&width) == NRF24_OK) {
                if (width > 32) {
                    nRF24_UpdateMode(STANDBYI);
                    nRF24_SendFlushRx();
                    if (hnrf24->DeviceMode == TRANSMITTER_MODE) {
                        nRF24_UpdateMode(PTX);
                    } else if (hnrf24->DeviceMode == RECEIVER_MODE) {
                        nRF24_UpdateMode(PRX);
                    }
                }
                else {
                    nRF24_SendReadRxPlCmd(buf, width);
                    nRF24_IRQ_Callback(event_type, data_src, buf, width);
                }
                nRF24_ResetRegister(STATUS, MASK_RX_DR);
            }
        }

        if ((status & 0x20) >> EVENT_TX_DS) {  // TX_DS
            event_type = EVENT_TX_DS;
            nRF24_IRQ_Callback(event_type, data_src, buf, 0);
            nRF24_ResetRegister(STATUS, MASK_TX_DS);
        }

        if ((status & 0x10) >> EVENT_MAX_RT) {  // MAX_RT
            event_type = EVENT_MAX_RT;
            nRF24_IRQ_Callback(event_type, data_src, buf, 0);
            nRF24_ResetRegister(STATUS, MASK_MAX_RT);
        }

    } else {
        event_type = EVENT_GPIO_IRQ;  // STM32 gpio irq except nRF24L01 IRQ
        nRF24_IRQ_Callback(event_type, gpio, buf, width);
    }
}

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
__weak void nRF24_IRQ_Callback(uint8_t event_type, uint16_t data_src, uint8_t* data, uint8_t width) {

      switch(event_type) {
        case EVENT_RX_DR:
            break;
        case EVENT_TX_DS:
            break;
        case EVENT_MAX_RT:
            break;
        case EVENT_GPIO_IRQ:
            break;
        default:
            break;
    }
}
