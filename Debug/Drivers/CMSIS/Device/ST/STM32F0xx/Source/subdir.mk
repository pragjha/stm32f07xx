################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f072xb_rcc_driver.c \
../Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f07xx_gpio_driver.c \
../Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f07xx_i2c_driver.c \
../Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f07xx_spi_driver.c \
../Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f07xx_usart_driver.c 

OBJS += \
./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f072xb_rcc_driver.o \
./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f07xx_gpio_driver.o \
./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f07xx_i2c_driver.o \
./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f07xx_spi_driver.o \
./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f07xx_usart_driver.o 

C_DEPS += \
./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f072xb_rcc_driver.d \
./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f07xx_gpio_driver.d \
./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f07xx_i2c_driver.d \
./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f07xx_spi_driver.d \
./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f07xx_usart_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/Device/ST/STM32F0xx/Source/%.o Drivers/CMSIS/Device/ST/STM32F0xx/Source/%.su Drivers/CMSIS/Device/ST/STM32F0xx/Source/%.cyclo: ../Drivers/CMSIS/Device/ST/STM32F0xx/Source/%.c Drivers/CMSIS/Device/ST/STM32F0xx/Source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F072xB -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-Device-2f-ST-2f-STM32F0xx-2f-Source

clean-Drivers-2f-CMSIS-2f-Device-2f-ST-2f-STM32F0xx-2f-Source:
	-$(RM) ./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f072xb_rcc_driver.cyclo ./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f072xb_rcc_driver.d ./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f072xb_rcc_driver.o ./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f072xb_rcc_driver.su ./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f07xx_gpio_driver.cyclo ./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f07xx_gpio_driver.d ./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f07xx_gpio_driver.o ./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f07xx_gpio_driver.su ./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f07xx_i2c_driver.cyclo ./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f07xx_i2c_driver.d ./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f07xx_i2c_driver.o ./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f07xx_i2c_driver.su ./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f07xx_spi_driver.cyclo ./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f07xx_spi_driver.d ./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f07xx_spi_driver.o ./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f07xx_spi_driver.su ./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f07xx_usart_driver.cyclo ./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f07xx_usart_driver.d ./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f07xx_usart_driver.o ./Drivers/CMSIS/Device/ST/STM32F0xx/Source/stm32f07xx_usart_driver.su

.PHONY: clean-Drivers-2f-CMSIS-2f-Device-2f-ST-2f-STM32F0xx-2f-Source

