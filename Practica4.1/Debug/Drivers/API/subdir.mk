################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/API/API_delay.c 

OBJS += \
./Drivers/API/API_delay.o 

C_DEPS += \
./Drivers/API/API_delay.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/API/%.o Drivers/API/%.su: ../Drivers/API/%.c Drivers/API/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F429ZITx -DSTM32F4 -DNUCLEO_F429ZI -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Inc -I"C:/Users/Gabriel/Documents/GitHub/PdM/Practica3/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Gabriel/Documents/GitHub/PdM/Practica3/Drivers/CMSIS/Include" -I"C:/Users/Gabriel/Documents/GitHub/PdM/Practica3/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Gabriel/Documents/GitHub/PdM/Practica3/Drivers/BSP/STM32F4xx_Nucleo_144" -I"C:/Users/Gabriel/Documents/GitHub/PdM/Practica3/Drivers/Core/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-API

clean-Drivers-2f-API:
	-$(RM) ./Drivers/API/API_delay.d ./Drivers/API/API_delay.o ./Drivers/API/API_delay.su

.PHONY: clean-Drivers-2f-API

