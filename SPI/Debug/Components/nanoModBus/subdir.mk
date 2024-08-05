################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Components/nanoModBus/nanomodbus.c 

OBJS += \
./Components/nanoModBus/nanomodbus.o 

C_DEPS += \
./Components/nanoModBus/nanomodbus.d 


# Each subdirectory must supply rules for building sources it contributes
Components/nanoModBus/%.o Components/nanoModBus/%.su Components/nanoModBus/%.cyclo: ../Components/nanoModBus/%.c Components/nanoModBus/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F411xE -DSTM32 -DSTM32F4 -DSTM32F411RETx -c -I"D:/Proyectos/STM32_Codes/SPI/Components/Wiznet_ioLibrary/Internet" -I"D:/Proyectos/STM32_Codes/SPI/Components/Wiznet_ioLibrary/Internet/DHCP" -I"D:/Proyectos/STM32_Codes/SPI/Components/Wiznet_ioLibrary/Internet/DNS" -I"D:/Proyectos/STM32_Codes/SPI/Components/Wiznet_ioLibrary/Internet/FTPClient" -I"D:/Proyectos/STM32_Codes/SPI/Components/Wiznet_ioLibrary/Internet/FTPServer" -I"D:/Proyectos/STM32_Codes/SPI/Components/Wiznet_ioLibrary/Internet/httpServer" -I"D:/Proyectos/STM32_Codes/SPI/Components/Wiznet_ioLibrary/Internet/MQTT" -I"D:/Proyectos/STM32_Codes/SPI/Components/Wiznet_ioLibrary/Internet/SNMP" -I"D:/Proyectos/STM32_Codes/SPI/Components/Wiznet_ioLibrary/Internet/SNTP" -I"D:/Proyectos/STM32_Codes/SPI/Components/Wiznet_ioLibrary/Internet/TFTP" -I"D:/Proyectos/STM32_Codes/SPI/Core/Inc" -I"D:/Proyectos/STM32_Codes/SPI/Components/Wiznet_ioLibrary/Ethernet" -I"D:/Proyectos/STM32_Codes/SPI/Components/Wiznet_ioLibrary/Ethernet/W5100" -I"D:/Proyectos/STM32_Codes/SPI/Components/Wiznet_ioLibrary/Ethernet/W5100S" -I"D:/Proyectos/STM32_Codes/SPI/Components/Wiznet_ioLibrary/Ethernet/W5200" -I"D:/Proyectos/STM32_Codes/SPI/Components/Wiznet_ioLibrary/Ethernet/W5300" -I"D:/Proyectos/STM32_Codes/SPI/Components/Wiznet_ioLibrary/Ethernet/W5500" -I"D:/Proyectos/STM32_Codes/SPI/Components/nanoModBus" -I"D:/Proyectos/STM32_Codes/SPI/Drivers/CMSIS/Include" -I"D:/Proyectos/STM32_Codes/SPI/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Components-2f-nanoModBus

clean-Components-2f-nanoModBus:
	-$(RM) ./Components/nanoModBus/nanomodbus.cyclo ./Components/nanoModBus/nanomodbus.d ./Components/nanoModBus/nanomodbus.o ./Components/nanoModBus/nanomodbus.su

.PHONY: clean-Components-2f-nanoModBus

