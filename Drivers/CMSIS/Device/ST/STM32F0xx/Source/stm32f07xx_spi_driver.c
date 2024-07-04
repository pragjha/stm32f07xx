/*
 * stm32f07xx_spi_driver.c
 *
 *  Created on: Feb 29, 2024
 *      Author: pragjha
 */

#include <stm32f07xx_spi_driver.h>

uint8_t SPI_GetFlagStatus(SPI_TypeDef *pSPIx, uint32_t FlagName)
{
	if(pSPIx->SR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;

}
/*
 * Peripheral Clock setup
 */
void SPI_PeriClockControl(SPI_TypeDef *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_EN();
		}
		else if(pSPIx == SPI2)
		{
			SPI2_PCLK_EN();
		}
	}

	else
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_DI();
		}else if (pSPIx == SPI2)
		{
			SPI2_PCLK_DI();
		}
	}
}

/*
 * Init and De-init
 */
void SPI_Init(SPI_Handle_t *pSPIHandle){
	//first lets configure the SPI_CR1 register
    uint32_t tempreg=0;

	//1. configure the device mode
	tempreg|= pSPIHandle-> SPIConfig.SPI_DeviceMode<< SPI_CR1_MSTR_Pos;

	//2. configure the bus config
	if(pSPIHandle->SPIConfig.SPI_BusConfig==SPI_BUS_CONFIG_FD)
	{
		//bidi mode should be cleared
		tempreg&= ~(1<<SPI_CR1_BIDIMODE_Pos);
	}
	else if(pSPIHandle->SPIConfig.SPI_BusConfig==SPI_BUS_CONFIG_HD)
	{
		//bidi mode should be set
		tempreg|=(1<<SPI_CR1_BIDIMODE_Pos);
	}
	else if(pSPIHandle->SPIConfig.SPI_BusConfig==SPI_BUS_CONFIG_SIMPLEX_RXONLY)
	{
		//bidi mode should be cleared
		tempreg&= ~(1<<SPI_CR1_BIDIMODE_Pos);
		//RX bit must be set
		tempreg|=(1<<SPI_CR1_RXONLY_Pos);
	}

	//3. configure the spi serial clock speed (baud rate)
	tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed<<SPI_CR1_BR_Pos;

	//4. configure the DFF
	//5. configure the CPOL
	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL<<SPI_CR1_CPOL_Pos;

	//6. configure the CPHA
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA<<SPI_CR1_CPHA_Pos;

	//save the value of tempreg in SPI_CR1
	pSPIHandle-> pSPIx-> CR1=tempreg;


}


void SPI_DeInit(SPI_TypeDef *pSPIx)
{

}



void SPI_sendData(SPI_TypeDef *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
	while(Len>0)
	{
		//1. wait till TXE is set
		while(SPI_GetFlagStatus(pSPIx,SPI_TXE_FLAG)== FLAG_RESET);

		//2. Check the DFF bit
		//3. load the data into DR
		pSPIx->DR= *pTxBuffer;
		Len--;
		pTxBuffer++;

	}
}


void SPI_receiveData(SPI_TypeDef *pSPIx, uint8_t *pRxBuffer, uint32_t Len)
{
	while(Len>0)
		{
			//1. wait till RXNE is set
			while(SPI_GetFlagStatus(pSPIx,SPI_SR_RXNE_Pos)== FLAG_RESET);

			//2. Check the DFF bit
			//3. load the data from DR to Rx buffer
			*pRxBuffer =pSPIx->DR;
			Len--;
			pRxBuffer++;

		}
}

void SPI_PeripheralControl(SPI_TypeDef *pSPIx,uint8_t EnOrDi)
{
	if(EnOrDi==ENABLE)
	{
		pSPIx->CR1|= (1 << SPI_CR1_SPE_Pos);
	}
	else
	{
		pSPIx->CR1&= ~(1 << SPI_CR1_SPE_Pos);
	}
}
void SPI_SSIConfig(SPI_TypeDef *pSPIx,uint8_t EnOrDi)
{
	if(EnOrDi==ENABLE)
	{
		pSPIx->CR1|= (1 << SPI_CR1_SSI_Pos);
	}
	else
	{
		pSPIx->CR1&= ~(1 << SPI_CR1_SSI_Pos);
	}

}

void SPI_SSOEConfig(SPI_TypeDef *pSPIx,uint8_t EnOrDi)
{
	if(EnOrDi==ENABLE)
	{
		pSPIx->CR2|= (1 << SPI_CR2_SSOE_Pos);
	}
	else
	{
		pSPIx->CR2&= ~(1 << SPI_CR2_SSOE_Pos);
	}

}

/*
 * IRQ Configuration and ISR handling
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi){

}
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority){

}
void SPI_IRQHandling(SPI_Handle_t *pHandle){

}
