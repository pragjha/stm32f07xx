/*
 * stm32f07xx_i2c_driver.h
 *
 *  Created on: Dec 11, 2023
 *      Author: pragjha
 */

#ifndef CMSIS_DEVICE_ST_STM32F0XX_INCLUDE_STM32F07XX_I2C_DRIVER_H_
#define CMSIS_DEVICE_ST_STM32F0XX_INCLUDE_STM32F07XX_I2C_DRIVER_H_

#include<stm32f072xb.h>
typedef struct
{
	uint32_t I2C_SCLSpeed;
	uint8_t  I2C_DeviceAddress;
	uint8_t  I2C_AckControl;
	uint8_t  I2C_FMDutyCycle;

}I2C_Config_t;

/*
 *Handle structure for I2Cx peripheral
 */
typedef struct
{
	I2C_TypeDef 	*pI2Cx;
	I2C_Config_t 	I2C_Config;
}I2C_Handle_t;

/*
 * I2C SCL Speed
 */
#define I2C_SCL_SPEED_SM 	100000 //standard mode
#define I2C_SCL_SPEED_FM4K 	400000 //fast mode
#define I2C_SCL_SPEED_FM2K  200000


/*
 * @I2C_AckControl
 */
#define I2C_ACK_ENABLE        1
#define I2C_ACK_DISABLE       0

/*
 * @I2C_FMDutyCycle
 */
#define I2C_FM_DUTY_2        0
#define I2C_FM_DUTY_16_9     1


//some generic macros

#define ENABLE 				1
#define DISABLE 			0
#define SET 				ENABLE
#define RESET 				DISABLE
#define GPIO_PIN_SET        SET
#define GPIO_PIN_RESET      RESET
#define FLAG_RESET         RESET
#define FLAG_SET 			SET

/*
 * Bit position definitions I2C_SR
 */

#define I2C_SR_TXE 					0
#define I2C_SR_TXIS 				1
#define I2C_SR_RXNE 				2
#define I2C_SR_ADDR 				3
#define I2C_SR_NACKF 				4
#define I2C_SR_STOPF 				5
#define I2C_SR_TC 					6
#define I2C_SR_TCR 					7
#define I2C_SR_BERR 				8
#define I2C_SR_ARLO					9
#define I2C_SR_OVR 				   10

/*
 * I2C related status flags definitions
 */
#define I2C_FLAG_TXE   		( 1 << I2C_SR_TXE)
#define I2C_FLAG_RXNE   	( 1 << I2C_SR_RXNE)
#define I2C_FLAG_TXIS 		( 1 << I2C_SR_TXIS)
#define I2C_FLAG_OVR  		( 1 << I2C_SR_OVR)
#define I2C_FLAG_ADDR   	( 1 << I2C_SR_ADDR)
#define I2C_FLAG_ARLO 		( 1 << I2C_SR_ARLO)
#define I2C_FLAG_BERR 		( 1 << I2C_SR_BERR)
#define I2C_FLAG_STOPF 		( 1 << I2C_SR_STOPF)
#define I2C_FLAG_NACKF 		( 1 << I2C_SR_NACKF)
#define I2C_FLAG_TC  		( 1 << I2C_SR_TC)
#define I2C_FLAG_TCR 		( 1 << I2C_SR_TCR)

#define I2C_DISABLE_SR  	RESET
#define I2C_ENABLE_SR   	SET



/*
 * Peripheral Clock setup
 */
void I2C_PeriClockControl(I2C_TypeDef *pI2Cx, uint8_t EnorDi);

/*
 * Init and De-init
 */
void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_TypeDef *pI2Cx);


/*
 * Data Send and Receive
 */
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle,uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr);
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr);
uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr);
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr);

void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle);
void I2C_CloseSendData(I2C_Handle_t *pI2CHandle);


void I2C_SlaveSendData(I2C_TypeDef *pI2C,uint8_t data);
uint8_t I2C_SlaveReceiveData(I2C_TypeDef *pI2C);

/*
 * IRQ Configuration and ISR handling
 */
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle);
void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle);


/*
 * Other Peripheral Control APIs
 */
void I2C_PeripheralControl(I2C_TypeDef *pI2Cx, uint8_t EnOrDi);
uint8_t I2C_GetFlagStatus(I2C_TypeDef *pI2Cx , uint32_t FlagName);
void I2C_ManageAcking(I2C_TypeDef *pI2Cx, uint8_t EnorDi);
void I2C_GenerateStopCondition(I2C_TypeDef *pI2Cx);

void I2C_SlaveEnableDisableCallbackEvents(I2C_TypeDef *pI2Cx,uint8_t EnorDi);

/*
 * Application callback
 */
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle,uint8_t AppEv);
#endif /* CMSIS_DEVICE_ST_STM32F0XX_INCLUDE_STM32F07XX_I2C_DRIVER_H_ */
