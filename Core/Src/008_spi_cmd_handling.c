/*
 * 008_spi_cmd_handling.c
 *
 *  Created on: Jun 7, 2024
 *      Author: pragjha
 */


#include "stm32f072xb.h"
#include "stm32f07xx_spi_driver.h"
#include "stm32f07xx_gpio_driver.h"
#include<string.h>
#include<stdio.h>

extern void initialise_monitor_handles();

/*
 * PB14--SPI2_MISO
 * PB15--SPI2_MOSI
 * PB13--SPI2_SCLK
 * PB12--SPI2_NSS
 * Alt function mode: 0
 */

//command codes
#define COMMAND_LED_CTRL    0x50
#define COMMAND_SENSOR_READ 0x51
#define COMMAND_LED_READ    0x52
#define COMMAND_PRINT       0x53
#define COMMAND_ID_READ     0x54

#define LED_ON 1
#define LED_OFF 0

//arduino analog pins
#define ANALOG_PIN0 0
#define ANALOG_PIN1 1
#define ANALOG_PIN2 2
#define ANALOG_PIN3 3
#define ANALOG_PIN4 4

//arduino led
#define LED_PIN 9


void delay(void)
{
	for(unsigned long i=0;i<500000;i++);
}

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

	//MISO
	SPIPins.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);

	//NSS
	SPIPins.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);

}

uint8_t SPI_VerifyResponse(uint8_t ackbyte)
{
	if(ackbyte==0xF5)
	{
	   //ack
	   return 1;
	}
	else
	{
	   //nack
	   return 0;
	}

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

void GPIO_ButtonInit(void)
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
	uint8_t dummy_write=0xff;
	uint8_t dummy_read;

	initialise_monitor_handles();

	printf("Application is running\n");

	GPIO_ButtonInit();
	//this function is used to initialize the GPIO pins to behave as SPI2 pins
	SPI2_GPIOInits();

	//this function is used to initialize SPI2 peripheral parameters
	SPI2_Inits();

	printf("SPI Init. done\n");

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

		//to avoid button de-bouncing related issues 200ms of delay
		delay();

		//enable the SPI2 peripheral
		SPI_PeripheralControl(SPI2,ENABLE);

		//1. CMD_LED_CTRL <pin no(1) > <value(1)>
	    uint8_t commndcode=COMMAND_LED_CTRL;
	    uint8_t ackbyte;
	    uint8_t args[2];

	    //send command
	    SPI_sendData(SPI2,&commndcode,1);

	    //do dummy read to clear off the RXNE
	    SPI_receiveData(SPI2,&dummy_read,1);

	    //send some dummy bits(1 byte) to fetch the response from the slave.
	    SPI_sendData(SPI2,&dummy_write,1);

	    //read the ack the byte received
	    SPI_receiveData(SPI2,&ackbyte,1);

	    if(SPI_VerifyResponse(ackbyte))
	    {
	    	//send arguments
	    	args[0]=LED_PIN; //args[0] is PIN number
	    	args[1]=LED_ON;  //args[1] will be value

	    	//send arguments
	    	SPI_sendData(SPI2,args,2);

	    	// dummy read
	    	SPI_receiveData(SPI2,args,2);

	    	printf("COMMAND_LED_CTRL Executed\n");
	    }
	    //end of CMD_LED_CTRL

	    //2. CMD_SENSOR_READ <analog pin number(1) >
	    //wait till button is pressed
	    while(! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0));

	    //to avoid button de-bouncing related issues 200ms of delay
	    delay();

	    commndcode=COMMAND_SENSOR_READ;

	    //send command
	    SPI_sendData(SPI2,&commndcode,1);

	    //do dummy read to clear off the RXNE
	    SPI_receiveData(SPI2,&dummy_read,1);

	    //send some dummy bits(1 byte) to fetch the response from the slave.
	    SPI_sendData(SPI2,&dummy_write,1);


	    //read the ack the byte received
	    SPI_receiveData(SPI2,&ackbyte,1);

	    if(SPI_VerifyResponse(ackbyte))
	    {
	    	//send arguments
	    	args[0]=ANALOG_PIN0; //args[0] is PIN number

	    	//send arguments
	    	SPI_sendData(SPI2,args,2);
	    	//do dummy read to clear off the RXNE
	    	SPI_receiveData(SPI2,&dummy_read,1);

	    	//send some dummy bits(1 byte) to fetch the response from the slave.
	    	SPI_sendData(SPI2,&dummy_write,1);

	    	uint8_t analog_read;

	    	//end of CMD_LED_CTRL
	        SPI_receiveData(SPI2,&analog_read,1);
	        printf("COMMAND_SENSOR_READ %d\n",analog_read);
	    }
	    //3.  CMD_LED_READ 	 <pin no(1) >

	    //wait till button is pressed
	    while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0) );

	    //to avoid button de-bouncing related issues 200ms of delay
	    delay();

	    commndcode = COMMAND_LED_READ;

	    //send command
	    SPI_sendData(SPI2,&commndcode,1);

	    //do dummy read to clear off the RXNE
	    SPI_receiveData(SPI2,&dummy_read,1);

	    //Send some dummy byte to fetch the response from the slave
	    SPI_sendData(SPI2,&dummy_write,1);

	    //read the ack byte received
	    SPI_receiveData(SPI2,&ackbyte,1);

	    if( SPI_VerifyResponse(ackbyte))
	    {
	    	args[0] = LED_PIN;

	    	//send arguments
	    	SPI_sendData(SPI2,args,1); //sending one byte of

	    	//do dummy read to clear off the RXNE
	    	SPI_receiveData(SPI2,&dummy_read,1);

	    	//insert some delay so that slave can ready with the data
	    	delay();

	    	//Send some dummy bits (1 byte) fetch the response from the slave
	    	SPI_sendData(SPI2,&dummy_write,1);

	    	uint8_t led_status;
	    	SPI_receiveData(SPI2,&led_status,1);
	    	printf("COMMAND_READ_LED %d\n",led_status);

	    }

	    //4. CMD_PRINT 		<len(2)>  <message(len) >

	    //wait till button is pressed
	    while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0) );

	    //to avoid button de-bouncing related issues 200ms of delay
	    delay();

	    commndcode = COMMAND_PRINT;

	    //send command
	    SPI_sendData(SPI2,&commndcode,1);

	    //do dummy read to clear off the RXNE
	    SPI_receiveData(SPI2,&dummy_read,1);

	    //Send some dummy byte to fetch the response from the slave
	    SPI_sendData(SPI2,&dummy_write,1);

	    //read the ack byte received
	    SPI_receiveData(SPI2,&ackbyte,1);

	    uint8_t message[] = "Hello ! How are you ??";
	    if( SPI_VerifyResponse(ackbyte))
	    {
	    	args[0] = strlen((char*)message);

	    	//send arguments
	    	SPI_sendData(SPI2,args,1); //sending length

	    	//do dummy read to clear off the RXNE
	    	SPI_receiveData(SPI2,&dummy_read,1);

	    	delay();

	    	//send message
	    	for(int i = 0 ; i < args[0] ; i++){
	    		SPI_sendData(SPI2,&message[i],1);
	    		SPI_receiveData(SPI2,&dummy_read,1);
	    	}

	    	printf("COMMAND_PRINT Executed \n");

	    }

	    //5. CMD_ID_READ
	    //wait till button is pressed
	    while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0) );

	    //to avoid button de-bouncing related issues 200ms of delay
	    delay();

	    commndcode = COMMAND_ID_READ;

	    //send command
	    SPI_sendData(SPI2,&commndcode,1);

	    //do dummy read to clear off the RXNE
	    SPI_receiveData(SPI2,&dummy_read,1);

	    //Send some dummy byte to fetch the response from the slave
	    SPI_sendData(SPI2,&dummy_write,1);

	    //read the ack byte received
	    SPI_receiveData(SPI2,&ackbyte,1);

	    uint8_t id[11];
	    uint32_t i=0;
	    if( SPI_VerifyResponse(ackbyte))
	    {
	    	//read 10 bytes id from the slave
	    	for(  i = 0 ; i < 10 ; i++)
	    	{
	    		//send dummy byte to fetch data from slave
	    		SPI_sendData(SPI2,&dummy_write,1);
	    		SPI_receiveData(SPI2,&id[i],1);
	    	}

	    	id[10] = '\0';

	    	printf("COMMAND_ID : %s \n",id);

	    }

		//lets confirm SPI is not busy.
		while(SPI_GetFlagStatus(SPI2,SPI_SR_BSY_Pos));

		//Disable the SPI2 peripheral
		SPI_PeripheralControl(SPI2,DISABLE);
	}

	return 0;
}
