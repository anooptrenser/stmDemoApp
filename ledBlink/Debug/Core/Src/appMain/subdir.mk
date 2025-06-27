################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/appMain/appMain.c 

OBJS += \
./Core/Src/appMain/appMain.o 

C_DEPS += \
./Core/Src/appMain/appMain.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/appMain/%.o Core/Src/appMain/%.su Core/Src/appMain/%.cyclo: ../Core/Src/appMain/%.c Core/Src/appMain/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F413xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/1786/STM32CubeIDE/workspace_1.18.1/ledBlink/Core/Src/appMain" -I"C:/Users/1786/STM32CubeIDE/workspace_1.18.1/ledBlink/Core/Src/appMain/GPIO" -I"C:/Users/1786/STM32CubeIDE/workspace_1.18.1/ledBlink/Core/Src/appMain/ledBlink" -I"C:/Users/1786/STM32CubeIDE/workspace_1.18.1/ledBlink/Core/Src/appMain/OS" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-appMain

clean-Core-2f-Src-2f-appMain:
	-$(RM) ./Core/Src/appMain/appMain.cyclo ./Core/Src/appMain/appMain.d ./Core/Src/appMain/appMain.o ./Core/Src/appMain/appMain.su

.PHONY: clean-Core-2f-Src-2f-appMain

