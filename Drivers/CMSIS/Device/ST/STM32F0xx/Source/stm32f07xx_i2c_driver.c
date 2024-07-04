/*
 * stm32f07xx_i2c_driver.c
 *
 *  Created on: Dec 11, 2023
 *      Author: pragjha
 */


#include<stm32f072xb.h>
#include<stm32f07xx_i2c_driver.h>

#define ENABLE 				1
#define DISABLE 			0
#define SET 				ENABLE
#define RESET 				DISABLE
#define I2C_CR1_NACK 4


static void I2C_GenerateStartCondition(I2C_TypeDef *pI2Cx);
static void I2C_ExecuteAddressPhaseWrite(I2C_TypeDef *pI2Cx, uint8_t SlaveAddr);
static void I2C_ExecuteAddressPhaseRead(I2C_TypeDef *pI2Cx, uint8_t SlaveAddr);
static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle );
void I2C_PeriClockControl(I2C_TypeDef *pI2Cx, uint8_t EnorDi);
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr);

uint16_t AHB_Prescaler[9]={2,4,8,16,32,64,128,256,512};
uint16_t APB_Prescaler[4]={2,4,8,16};


void I2C_ManageAcking(I2C_TypeDef *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == DISABLE)
	{
		//enable the nack
		pI2Cx->CR1 |= ( 1 << I2C_CR1_NACK);
	}else
	{
		//disable the nack
		pI2Cx->CR1 &= ~( 1 << I2C_CR1_NACK);


	}
}

void I2C_PeriClockControl(I2C_TypeDef *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pI2Cx == I2C1)
		{
			I2C1_PCLK_EN();
		}else if (pI2Cx == I2C2)
		{
			I2C2_PCLK_EN();
		}
	}
	else
	{
         if(pI2Cx == I2C1)
		 {
        	 I2C1_PCLK_DI();
		 }else if (pI2Cx == I2C2)
		 {
			 I2C2_PCLK_DI();
		 }
	}
}

void I2C_DeInit(I2C_TypeDef *pI2Cx)
{
	if(pI2Cx == I2C1)
	{
		I2C1_REG_RESET();
	}else if (pI2Cx == I2C2)
	{
		I2C2_REG_RESET();
	}
}

void I2C_PeripheralControl(I2C_TypeDef *pI2Cx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pI2Cx->CR1 |= (1 << I2C_CR1_PE_Pos);

	}else
	{
		pI2Cx->CR1 &= ~(1 << I2C_CR1_PE_Pos);
	}

}

void I2C_Init(I2C_Handle_t *pI2CHandle)
{
	//enable the clock for I2C
	I2C_PeriClockControl(pI2CHandle->pI2Cx,ENABLE);

	uint32_t tempreg = 0 ;
    //ack control bit
	tempreg &= ~(pI2CHandle->I2C_Config.I2C_AckControl << 15);
	pI2CHandle->pI2Cx->CR2 = tempreg;


	//program the device own address in I2C_OAR1 register
	 tempreg = 0;
	 tempreg |= pI2CHandle->I2C_Config.I2C_DeviceAddress << 0;
	 tempreg |= ( 1 << 10); //configuring the OA1MODE bit in I2C_OAR1 register
	 pI2CHandle->pI2Cx->OAR1 = tempreg;

}

static void I2C_GenerateStartCondition(I2C_TypeDef *pI2Cx)
{
	pI2Cx->CR2 |= ( 1 << 13); //left shifting to START bit of I2C_CR2 register
}


static void I2C_ExecuteAddressPhaseRead(I2C_TypeDef *pI2Cx, uint8_t SlaveAddr)
{
	SlaveAddr = SlaveAddr << 1;
	SlaveAddr |= 1; //SlaveAddr is Slave address + r/nw bit=1
	pI2Cx->RXDR = SlaveAddr;
}

static void I2C_ExecuteAddressPhaseWrite(I2C_TypeDef *pI2Cx, uint8_t SlaveAddr)
{
	SlaveAddr = SlaveAddr << 1; //making space for r/w bit
	SlaveAddr &= ~(1); //SlaveAddr is Slave address + r/nw bit=0
	pI2Cx->TXDR = SlaveAddr; //writing in TXDR register
}

static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle )
{
	uint32_t dummy_read;
	dummy_read = pI2CHandle->pI2Cx->ISR;
	(void)dummy_read;

}
void I2C_GenerateStopCondition(I2C_TypeDef *pI2Cx)
{
	pI2Cx->CR2 |= ( 1 << 14);
}

uint8_t I2C_GetFlagStatus(I2C_TypeDef *pI2Cx , uint32_t FlagName)
{
	if(pI2Cx->ISR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

void I2C_MasterSendData(I2C_Handle_t *pI2CHandle,uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr)
{

	    // 1. Generate the START condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		//2. confirm that start generation is completed by checking the SB flag in the SR1
	    //Note: Until SB is cleared SCL will be stretched (pulled to LOW)
        //while( !  I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SB)   );

		//3. Send the address of the slave with r/nw bit set to w(0) (total 8 bits )
		I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx,SlaveAddr);

		/*
		 *4. Confirm that address phase is completed by checking the ADDR flag in the SR
		     ADDR: Address matched (slave mode).
		     This bit is set by hardware as soon as the received slave address matched with
		     one of the enabled slave addresses.
		 */
		 while( !  I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_ADDR)   );

		//5. clear the ADDR flag according to its hardware sequence
		//   Note: Until ADDR is cleared SCL will be stretched (pulled to LOW)
		I2C_ClearADDRFlag(pI2CHandle);

		//6. send the data until len becomes 0

		while(Len > 0)
		{
			while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_TXE) ); //Wait till TXE is set
			pI2CHandle->pI2Cx->TXDR = *pTxbuffer;
			pTxbuffer++;
			Len--;
		}

		//7. when Len becomes zero wait for TXE=1 and BTF=1 before generating the STOP condition
		//   Note: TXE=1 , BTF=1 , means that both SR and DR are empty and next transmission should begin
		//   when BTF=1 SCL will be stretched (pulled to LOW)

		while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_TXE) );

       //while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_BTF) );


		//8. Generate STOP condition and master need not to wait for the completion of stop condition.
		//   Note: generating STOP, automatically clears the BTF
		if(Sr == I2C_DISABLE_SR )
			I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
}

void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr)
{

	//1. Generate the START condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. confirm that start generation is completed by checking the SB flag in the SR1
	//   Note: Until SB is cleared SCL will be stretched (pulled to LOW)
//	while( !  I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SB)   );

	//3. Send the address of the slave with r/w bit set to R(1) (total 8 bits )
	I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx,SlaveAddr);

	//4. wait until address phase is completed by checking the ADDR flag in SR1
	while( !  I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_ADDR)   );


	//procedure to read only 1 byte from slave
	if(Len == 1)
	{
		//Enable NAcking
		I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_DISABLE);

		//clear the ADDR flag
		I2C_ClearADDRFlag(pI2CHandle);

		//wait until  RXNE becomes 1
		while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_RXNE) );

		//generate STOP condition
		if(Sr == I2C_DISABLE_SR )
			I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

		//read data into buffer
		*pRxBuffer = pI2CHandle->pI2Cx->RXDR;

	}


    //procedure to read data from slave when Len > 1
	if(Len > 1)
	{
		//clear the ADDR flag
		I2C_ClearADDRFlag(pI2CHandle);

		//read the data until Len becomes zero
		for ( uint32_t i = Len ; i > 0 ; i--)
		{
			//wait until RXNE becomes 1
			while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_RXNE) );

			if(i == 2) //if last 2 bytes are remaining
			{
				//Disable Acking
				I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_DISABLE);

				//generate STOP condition
				if(Sr == I2C_DISABLE_SR )
					I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

			}

			//read the data from data register in to buffer
			*pRxBuffer = pI2CHandle->pI2Cx->RXDR;

			//increment the buffer address
			pRxBuffer++;

		}

	}

	//re-enable ACKing
	if(pI2CHandle->I2C_Config.I2C_AckControl == I2C_ACK_ENABLE)
	{
		I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_ENABLE);
	}

}
