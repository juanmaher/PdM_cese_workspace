/**
 ******************************************************************************
 * @file    .h
 * @author  Juan Manuel Hernández
 * @brief   Header for .c module
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __API_SPI_H
#define __API_SPI_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */

/* Exported types ------------------------------------------------------------*/
typedef bool bool_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/**
 * @brief Initialize the SPI
 * @return void
*/
bool_t SPI_Init(SPI_HandleTypeDef *hspi);

/**
 * @brief Transmit data
 * @return void
*/
bool_t SPI_TransmitData(SPI_HandleTypeDef *hspi, uint8_t * pTxBuffer, uint16_t size);

/**
 * @brief Receive data
 * @return void
*/
bool_t SPI_ReceiveData(SPI_HandleTypeDef *hspi, uint8_t * pRxBuffer, uint16_t size);

/**
 * @brief Deinitialize the SPI
 * @return void
*/
bool_t SPI_DeInit(SPI_HandleTypeDef *hspi);


#endif /* __API_SPI_H */
