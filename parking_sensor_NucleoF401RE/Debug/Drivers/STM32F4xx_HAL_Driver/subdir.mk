################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_msp.c 

OBJS += \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_msp.o 

C_DEPS += \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_msp.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/STM32F4xx_HAL_Driver/%.o Drivers/STM32F4xx_HAL_Driver/%.su Drivers/STM32F4xx_HAL_Driver/%.cyclo: ../Drivers/STM32F4xx_HAL_Driver/%.c Drivers/STM32F4xx_HAL_Driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/jmhernandez/Documents/fiuba/cese/22Co2024/PdM/PdM_cese_workspace/parking_sensor_NucleoF401RE/Inc" -I"/home/jmhernandez/Documents/fiuba/cese/22Co2024/PdM/PdM_cese_workspace/parking_sensor_NucleoF401RE/Drivers" -I"/home/jmhernandez/Documents/fiuba/cese/22Co2024/PdM/PdM_cese_workspace/parking_sensor_NucleoF401RE/Drivers/STM32F4xx_HAL_Driver" -I"/home/jmhernandez/Documents/fiuba/cese/22Co2024/PdM/PdM_cese_workspace/parking_sensor_NucleoF401RE/Core" -I"/home/jmhernandez/Documents/fiuba/cese/22Co2024/PdM/PdM_cese_workspace/parking_sensor_NucleoF401RE/Drivers/API/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-STM32F4xx_HAL_Driver

clean-Drivers-2f-STM32F4xx_HAL_Driver:
	-$(RM) ./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_msp.cyclo ./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_msp.d ./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_msp.o ./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_msp.su

.PHONY: clean-Drivers-2f-STM32F4xx_HAL_Driver

