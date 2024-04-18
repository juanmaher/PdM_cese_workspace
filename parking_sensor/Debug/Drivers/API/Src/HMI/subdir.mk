################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/API/Src/HMI/API_display.c \
../Drivers/API/Src/HMI/API_display_port.c 

OBJS += \
./Drivers/API/Src/HMI/API_display.o \
./Drivers/API/Src/HMI/API_display_port.o 

C_DEPS += \
./Drivers/API/Src/HMI/API_display.d \
./Drivers/API/Src/HMI/API_display_port.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/API/Src/HMI/%.o Drivers/API/Src/HMI/%.su Drivers/API/Src/HMI/%.cyclo: ../Drivers/API/Src/HMI/%.c Drivers/API/Src/HMI/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F429ZITx -DSTM32F4 -DNUCLEO_F429ZI -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Inc -I"/home/jmhernandez/Documents/fiuba/cese/22Co2024/PdM/PdM_cese_workspace/parking_sensor/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/jmhernandez/Documents/fiuba/cese/22Co2024/PdM/PdM_cese_workspace/parking_sensor/Drivers/CMSIS/Include" -I"/home/jmhernandez/Documents/fiuba/cese/22Co2024/PdM/PdM_cese_workspace/parking_sensor/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/jmhernandez/Documents/fiuba/cese/22Co2024/PdM/PdM_cese_workspace/parking_sensor/Drivers/BSP/STM32F4xx_Nucleo_144" -I"/home/jmhernandez/Documents/fiuba/cese/22Co2024/PdM/PdM_cese_workspace/parking_sensor/Drivers/Core/Inc" -I"/home/jmhernandez/Documents/fiuba/cese/22Co2024/PdM/PdM_cese_workspace/parking_sensor/Drivers/API/Inc" -I"/home/jmhernandez/Documents/fiuba/cese/22Co2024/PdM/PdM_cese_workspace/parking_sensor/Drivers/API/Inc/Communication" -I"/home/jmhernandez/Documents/fiuba/cese/22Co2024/PdM/PdM_cese_workspace/parking_sensor/Drivers/API/Inc/HMI" -I"/home/jmhernandez/Documents/fiuba/cese/22Co2024/PdM/PdM_cese_workspace/parking_sensor/Drivers/API/Inc/Misc" -I"/home/jmhernandez/Documents/fiuba/cese/22Co2024/PdM/PdM_cese_workspace/parking_sensor/Drivers/API/Inc/Sensors" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-API-2f-Src-2f-HMI

clean-Drivers-2f-API-2f-Src-2f-HMI:
	-$(RM) ./Drivers/API/Src/HMI/API_display.cyclo ./Drivers/API/Src/HMI/API_display.d ./Drivers/API/Src/HMI/API_display.o ./Drivers/API/Src/HMI/API_display.su ./Drivers/API/Src/HMI/API_display_port.cyclo ./Drivers/API/Src/HMI/API_display_port.d ./Drivers/API/Src/HMI/API_display_port.o ./Drivers/API/Src/HMI/API_display_port.su

.PHONY: clean-Drivers-2f-API-2f-Src-2f-HMI

