/*
 * stm32f07xx_spi_driver.h
 *
 *  Created on: Feb 29, 2024
 *      Author: pragjha
 */

#ifndef CMSIS_DEVICE_ST_STM32F0XX_INCLUDE_STM32F07XX_SPI_DRIVER_H_
#define CMSIS_DEVICE_ST_STM32F0XX_INCLUDE_STM32F07XX_SPI_DRIVER_H_

#include "stm32f072xb.h"


typedef struct
{
	uint8_t SPI_DeviceMode;
	uint8_t SPI_BusConfig;
	uint8_t SPI_SclkSpeed;
	uint8_t SPI_DFF;
	uint8_t SPI_CPOL;
	uint8_t SPI_CPHA;
	uint8_t SPI_SSM;
}SPI_Config_t;

typedef struct
{
	SPI_TypeDef *pSPIx;       		/*!< This holds the base address of the SPI port to which the pin belongs >*/
    SPI_Config_t SPIConfig;   /*!< This holds SPI pin configuration settings >*/
}SPI_Handle_t;

/*
 * SPI_Device mode
 */

#define SPI_DEVICE_MODE_MASTER 1
#define SPI_DEVICE_MODE_SLAVE 0

/*
 * SPI_BusConfig
 */
#define SPI_BUS_CONFIG_FD 1
#define SPI_BUS_CONFIG_HD 2
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY 4

/*
 * 	SPI_SclkSpeed
 */
#define SPI_SCLK_SPEED_DIV2   0
#define SPI_SCLK_SPEED_DIV4   1
#define SPI_SCLK_SPEED_DIV8   2
#define SPI_SCLK_SPEED_DIV16  3
#define SPI_SCLK_SPEED_DIV32  4
#define SPI_SCLK_SPEED_DIV64  5
#define SPI_SCLK_SPEED_DIV128 6
#define SPI_SCLK_SPEED_DIV256 7

/*
 * SPI_DFF
 */
//#define SPI_DFF_8bits 0
//#define SPI_DFF_8bits 1


/*
 * CPOL
 */
#define SPI_CPOL_HIGH 1
#define SPI_CPOL_LOW 0

/*
 * CPHA
 *
 */
#define SPI_CPHA_HIGH 1
#define SPI_CPHA_LOW 0

/*
 * SPI_SSM
 */

#define SPI_SSM_EN 1
#define SPI_SSM_DI 0

/*
 * SPI related status flags definition
 */

#define SPI_TXE_FLAG (1<< SPI_SR_TXE_Pos)

//some generic macros

#define ENABLE 				1
#define DISABLE 			0
#define SET 				ENABLE
#define RESET 				DISABLE
#define GPIO_PIN_SET        SET
#define GPIO_PIN_RESET      RESET
#define FLAG_RESET         RESET
#define FLAG_SET 			SET


/******************************************************************************************
 *								APIs
 ******************************************************************************************/

/*
 * Peripheral Clock setup
 */
void SPI_PeriClockControl(SPI_TypeDef *pSPIx, uint8_t EnorDi);

/*
 * Init and De-init
 */
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_TypeDef *pSPIx);

/*
 * Init and De-init
 */

void SPI_sendData(SPI_TypeDef *pSPIx, uint8_t *pTxBuffer, uint32_t Len);
void SPI_receiveData(SPI_TypeDef *pSPIx, uint8_t *pTxBuffer, uint32_t Len);

void SPI_PeripheralControl(SPI_TypeDef *pSPIx,uint8_t EnOrDi);
void SPI_SSIConfig(SPI_TypeDef *pSPIx,uint8_t EnOrDi);
void SPI_SSOEConfig(SPI_TypeDef *pSPIx,uint8_t EnOrDi);
/*
 * IRQ Configuration and ISR handling
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);

#endif /* CMSIS_DEVICE_ST_STM32F0XX_INCLUDE_STM32F07XX_SPI_DRIVER_H_ */
