################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Peripheral/Src/GPIO.c \
../Peripheral/Src/INT.c \
../Peripheral/Src/LCD.c \
../Peripheral/Src/SysConfig.c \
../Peripheral/Src/Timer.c 

OBJS += \
./Peripheral/Src/GPIO.o \
./Peripheral/Src/INT.o \
./Peripheral/Src/LCD.o \
./Peripheral/Src/SysConfig.o \
./Peripheral/Src/Timer.o 

C_DEPS += \
./Peripheral/Src/GPIO.d \
./Peripheral/Src/INT.d \
./Peripheral/Src/LCD.d \
./Peripheral/Src/SysConfig.d \
./Peripheral/Src/Timer.d 


# Each subdirectory must supply rules for building sources it contributes
Peripheral/Src/%.o Peripheral/Src/%.su Peripheral/Src/%.cyclo: ../Peripheral/Src/%.c Peripheral/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/Proyectos/STM32_Codes/Hola_Mundo/Peripheral/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Peripheral-2f-Src

clean-Peripheral-2f-Src:
	-$(RM) ./Peripheral/Src/GPIO.cyclo ./Peripheral/Src/GPIO.d ./Peripheral/Src/GPIO.o ./Peripheral/Src/GPIO.su ./Peripheral/Src/INT.cyclo ./Peripheral/Src/INT.d ./Peripheral/Src/INT.o ./Peripheral/Src/INT.su ./Peripheral/Src/LCD.cyclo ./Peripheral/Src/LCD.d ./Peripheral/Src/LCD.o ./Peripheral/Src/LCD.su ./Peripheral/Src/SysConfig.cyclo ./Peripheral/Src/SysConfig.d ./Peripheral/Src/SysConfig.o ./Peripheral/Src/SysConfig.su ./Peripheral/Src/Timer.cyclo ./Peripheral/Src/Timer.d ./Peripheral/Src/Timer.o ./Peripheral/Src/Timer.su

.PHONY: clean-Peripheral-2f-Src

