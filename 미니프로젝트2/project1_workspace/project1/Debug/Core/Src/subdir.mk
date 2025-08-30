################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Aiot_Task.c \
../Core/Src/Command_Task.c \
../Core/Src/LCD_Task.c \
../Core/Src/NTP_Task.c \
../Core/Src/PIR_Task.c \
../Core/Src/RFID_Task.c \
../Core/Src/Schedule_Task.c \
../Core/Src/Servo.c \
../Core/Src/TH_Task.c \
../Core/Src/Weather_Task.c \
../Core/Src/dht11.c \
../Core/Src/esp.c \
../Core/Src/freertos.c \
../Core/Src/lcd.c \
../Core/Src/main.c \
../Core/Src/mfrc522.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c 

OBJS += \
./Core/Src/Aiot_Task.o \
./Core/Src/Command_Task.o \
./Core/Src/LCD_Task.o \
./Core/Src/NTP_Task.o \
./Core/Src/PIR_Task.o \
./Core/Src/RFID_Task.o \
./Core/Src/Schedule_Task.o \
./Core/Src/Servo.o \
./Core/Src/TH_Task.o \
./Core/Src/Weather_Task.o \
./Core/Src/dht11.o \
./Core/Src/esp.o \
./Core/Src/freertos.o \
./Core/Src/lcd.o \
./Core/Src/main.o \
./Core/Src/mfrc522.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o 

C_DEPS += \
./Core/Src/Aiot_Task.d \
./Core/Src/Command_Task.d \
./Core/Src/LCD_Task.d \
./Core/Src/NTP_Task.d \
./Core/Src/PIR_Task.d \
./Core/Src/RFID_Task.d \
./Core/Src/Schedule_Task.d \
./Core/Src/Servo.d \
./Core/Src/TH_Task.d \
./Core/Src/Weather_Task.d \
./Core/Src/dht11.d \
./Core/Src/esp.d \
./Core/Src/freertos.d \
./Core/Src/lcd.d \
./Core/Src/main.d \
./Core/Src/mfrc522.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/Aiot_Task.cyclo ./Core/Src/Aiot_Task.d ./Core/Src/Aiot_Task.o ./Core/Src/Aiot_Task.su ./Core/Src/Command_Task.cyclo ./Core/Src/Command_Task.d ./Core/Src/Command_Task.o ./Core/Src/Command_Task.su ./Core/Src/LCD_Task.cyclo ./Core/Src/LCD_Task.d ./Core/Src/LCD_Task.o ./Core/Src/LCD_Task.su ./Core/Src/NTP_Task.cyclo ./Core/Src/NTP_Task.d ./Core/Src/NTP_Task.o ./Core/Src/NTP_Task.su ./Core/Src/PIR_Task.cyclo ./Core/Src/PIR_Task.d ./Core/Src/PIR_Task.o ./Core/Src/PIR_Task.su ./Core/Src/RFID_Task.cyclo ./Core/Src/RFID_Task.d ./Core/Src/RFID_Task.o ./Core/Src/RFID_Task.su ./Core/Src/Schedule_Task.cyclo ./Core/Src/Schedule_Task.d ./Core/Src/Schedule_Task.o ./Core/Src/Schedule_Task.su ./Core/Src/Servo.cyclo ./Core/Src/Servo.d ./Core/Src/Servo.o ./Core/Src/Servo.su ./Core/Src/TH_Task.cyclo ./Core/Src/TH_Task.d ./Core/Src/TH_Task.o ./Core/Src/TH_Task.su ./Core/Src/Weather_Task.cyclo ./Core/Src/Weather_Task.d ./Core/Src/Weather_Task.o ./Core/Src/Weather_Task.su ./Core/Src/dht11.cyclo ./Core/Src/dht11.d ./Core/Src/dht11.o ./Core/Src/dht11.su ./Core/Src/esp.cyclo ./Core/Src/esp.d ./Core/Src/esp.o ./Core/Src/esp.su ./Core/Src/freertos.cyclo ./Core/Src/freertos.d ./Core/Src/freertos.o ./Core/Src/freertos.su ./Core/Src/lcd.cyclo ./Core/Src/lcd.d ./Core/Src/lcd.o ./Core/Src/lcd.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/mfrc522.cyclo ./Core/Src/mfrc522.d ./Core/Src/mfrc522.o ./Core/Src/mfrc522.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su

.PHONY: clean-Core-2f-Src

