/*
 * stm32f072xb_rcc_driver.h
 *
 *  Created on: Jun 29, 2024
 *      Author: pragjha
 */

#ifndef CMSIS_DEVICE_ST_STM32F0XX_INCLUDE_STM32F072XB_RCC_DRIVER_H_
#define CMSIS_DEVICE_ST_STM32F0XX_INCLUDE_STM32F072XB_RCC_DRIVER_H_

#include "stm32f072xb.h"

//This returns the APB1 clock value
uint32_t RCC_GetPCLK1Value(void);

//This returns the APB2 clock value
uint32_t RCC_GetPCLK2Value(void);


uint32_t  RCC_GetPLLOutputClock(void);

#endif /* CMSIS_DEVICE_ST_STM32F0XX_INCLUDE_STM32F072XB_RCC_DRIVER_H_ */
