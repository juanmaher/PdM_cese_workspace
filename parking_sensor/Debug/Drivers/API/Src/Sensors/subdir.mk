################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/API/Src/Sensors/API_hcsr04.c \
../Drivers/API/Src/Sensors/API_hcsr04_port.c \
../Drivers/API/Src/Sensors/API_reverse.c \
../Drivers/API/Src/Sensors/API_reverse_port.c 

OBJS += \
./Drivers/API/Src/Sensors/API_hcsr04.o \
./Drivers/API/Src/Sensors/API_hcsr04_port.o \
./Drivers/API/Src/Sensors/API_reverse.o \
./Drivers/API/Src/Sensors/API_reverse_port.o 

C_DEPS += \
./Drivers/API/Src/Sensors/API_hcsr04.d \
./Drivers/API/Src/Sensors/API_hcsr04_port.d \
./Drivers/API/Src/Sensors/API_reverse.d \
./Drivers/API/Src/Sensors/API_reverse_port.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/API/Src/Sensors/%.o Drivers/API/Src/Sensors/%.su Drivers/API/Src/Sensors/%.cyclo: ../Drivers/API/Src/Sensors/%.c Drivers/API/Src/Sensors/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F429ZITx -DSTM32F4 -DNUCLEO_F429ZI -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Inc -I"/home/jmhernandez/Documents/fiuba/cese/22Co2024/PdM/PdM_cese_workspace/parking_sensor/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/jmhernandez/Documents/fiuba/cese/22Co2024/PdM/PdM_cese_workspace/parking_sensor/Drivers/CMSIS/Include" -I"/home/jmhernandez/Documents/fiuba/cese/22Co2024/PdM/PdM_cese_workspace/parking_sensor/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/jmhernandez/Documents/fiuba/cese/22Co2024/PdM/PdM_cese_workspace/parking_sensor/Drivers/BSP/STM32F4xx_Nucleo_144" -I"/home/jmhernandez/Documents/fiuba/cese/22Co2024/PdM/PdM_cese_workspace/parking_sensor/Drivers/Core/Inc" -I"/home/jmhernandez/Documents/fiuba/cese/22Co2024/PdM/PdM_cese_workspace/parking_sensor/Drivers/API/Inc" -I"/home/jmhernandez/Documents/fiuba/cese/22Co2024/PdM/PdM_cese_workspace/parking_sensor/Drivers/API/Inc/Communication" -I"/home/jmhernandez/Documents/fiuba/cese/22Co2024/PdM/PdM_cese_workspace/parking_sensor/Drivers/API/Inc/HMI" -I"/home/jmhernandez/Documents/fiuba/cese/22Co2024/PdM/PdM_cese_workspace/parking_sensor/Drivers/API/Inc/Misc" -I"/home/jmhernandez/Documents/fiuba/cese/22Co2024/PdM/PdM_cese_workspace/parking_sensor/Drivers/API/Inc/Sensors" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-API-2f-Src-2f-Sensors

clean-Drivers-2f-API-2f-Src-2f-Sensors:
	-$(RM) ./Drivers/API/Src/Sensors/API_hcsr04.cyclo ./Drivers/API/Src/Sensors/API_hcsr04.d ./Drivers/API/Src/Sensors/API_hcsr04.o ./Drivers/API/Src/Sensors/API_hcsr04.su ./Drivers/API/Src/Sensors/API_hcsr04_port.cyclo ./Drivers/API/Src/Sensors/API_hcsr04_port.d ./Drivers/API/Src/Sensors/API_hcsr04_port.o ./Drivers/API/Src/Sensors/API_hcsr04_port.su ./Drivers/API/Src/Sensors/API_reverse.cyclo ./Drivers/API/Src/Sensors/API_reverse.d ./Drivers/API/Src/Sensors/API_reverse.o ./Drivers/API/Src/Sensors/API_reverse.su ./Drivers/API/Src/Sensors/API_reverse_port.cyclo ./Drivers/API/Src/Sensors/API_reverse_port.d ./Drivers/API/Src/Sensors/API_reverse_port.o ./Drivers/API/Src/Sensors/API_reverse_port.su

.PHONY: clean-Drivers-2f-API-2f-Src-2f-Sensors

