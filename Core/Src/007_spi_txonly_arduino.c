/*
 * 007_spi_txonly_arduino.c
 *
 *  Created on: Jun 4, 2024
 *      Author: pragjha
 */


#include "stm32f072xb.h"
#include "stm32f07xx_spi_driver.h"
#include "stm32f07xx_gpio_driver.h"
#include <string.h>
/*
 * PB14--SPI2_MISO
 * PB15--SPI2_MOSI
 * PB13--SPI2_SCLK
 * PB12--SPI2_NSS
 * Alt function mode: 0
 */

void SPI2_GPIOInits()
{
	GPIO_Handle_t SPIPins;
	SPIPins.pGPIOx=GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_ALTFN;  //We are using the GPIO pins as SPI, so need to select the pin mode as alternate func
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode=5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType=GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl=GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_FAST;

	//SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	//MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

//	//MISO
//	SPIPins.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_14;
//	GPIO_Init(&SPIPins);

	//NSS
	SPIPins.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);

}

void SPI2_Inits()
{
	SPI_Handle_t SPI2handle;
	SPI2handle.pSPIx=SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig=SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode=SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed=SPI_SCLK_SPEED_DIV8; //generates sclk of 2MHZ
//	SPI2handle.SPIConfig.SPI_DFF=SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL=SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA=SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM=SPI_SSM_DI;

	SPI_Init(&SPI2handle);

}

void GPIOButtonInit(void)
{
	GPIO_Handle_t GPIOBtn;
	GPIOBtn.pGPIOx = GPIOC;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&GPIOBtn);

}

int main(void)
{
	char user_data[]="Hello world";
	//this function is used to initialize the GPIO pins to behave as SPI2 pins
	SPI2_GPIOInits();

	//this function is used to initialize SPI2 peripheral parameters
	SPI2_Inits();

    /*
     * making SSOE 1 does NSS output enable.
     * The NSS pin is automatically managed by hardware.
     * i.e, when SPE=1,NSS will be pulled to low and
     * when SPE=0, NSS pin will be high.
     */
	SPI_SSOEConfig(SPI2,ENABLE);
	while(1)
	{
		//wait till button is pressed
		while(! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0));

		//enable the SPI2 peripheral
		SPI_PeripheralControl(SPI2,ENABLE);

		//to send data
		SPI_sendData(SPI2,(uint8_t*)user_data ,strlen(user_data));

		//lets confirm SPI is not busy.
		while(SPI_GetFlagStatus(SPI2,SPI_SR_BSY_Pos));

		//Disable the SPI2 peripheral
		SPI_PeripheralControl(SPI2,DISABLE);

	}



	return 0;
}
