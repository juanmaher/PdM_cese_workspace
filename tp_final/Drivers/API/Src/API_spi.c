/**
 ******************************************************************************
 * @file    .c
 * @author  Juan Manuel Hernández
 * @brief   Implementation of
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "API_spi.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
bool_t SPI_Init(SPI_HandleTypeDef *hspi)
{
    hspi->Init.Mode = SPI_MODE_MASTER;
    hspi->Init.Direction = SPI_DIRECTION_2LINES;
    hspi->Init.DataSize = SPI_DATASIZE_8BIT;
    hspi->Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi->Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi->Init.NSS = SPI_NSS_SOFT;
    hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    hspi->Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi->Init.TIMode = SPI_TIMODE_DISABLE;
    hspi->Init.CRCPolynomial = 7;
    HAL_SPI_Init(hspi);
    return true;
}

bool_t SPI_TransmitData(SPI_HandleTypeDef *hspi, uint8_t * pTxBuffer, uint16_t size)
{
    return true;
}

bool_t SPI_ReceiveData(SPI_HandleTypeDef *hspi, uint8_t * pRxBuffer, uint16_t size)
{
    return true;
}

bool_t SPI_DeInit(SPI_HandleTypeDef *hspi)
{
    return true;
}
