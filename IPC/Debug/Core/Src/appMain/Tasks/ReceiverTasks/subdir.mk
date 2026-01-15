################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/appMain/Tasks/ReceiverTasks/receiverTasks.c 

OBJS += \
./Core/Src/appMain/Tasks/ReceiverTasks/receiverTasks.o 

C_DEPS += \
./Core/Src/appMain/Tasks/ReceiverTasks/receiverTasks.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/appMain/Tasks/ReceiverTasks/%.o Core/Src/appMain/Tasks/ReceiverTasks/%.su Core/Src/appMain/Tasks/ReceiverTasks/%.cyclo: ../Core/Src/appMain/Tasks/ReceiverTasks/%.c Core/Src/appMain/Tasks/ReceiverTasks/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F413xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/1786/STM32CubeIDE/workspace_1.18.1/IPC/Core/Src/appMain" -I"C:/Users/1786/STM32CubeIDE/workspace_1.18.1/IPC/Core/Src/appMain/OS" -I"C:/Users/1786/STM32CubeIDE/workspace_1.18.1/IPC/Core/Src/appMain/Tasks" -I"C:/Users/1786/STM32CubeIDE/workspace_1.18.1/IPC/Core/Src/appMain/GPIO" -I"C:/Users/1786/STM32CubeIDE/workspace_1.18.1/IPC/Core/Src/appMain/Tasks/PollerTaks" -I"C:/Users/1786/STM32CubeIDE/workspace_1.18.1/IPC/Core/Src/appMain/Tasks/ReceiverTasks" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-appMain-2f-Tasks-2f-ReceiverTasks

clean-Core-2f-Src-2f-appMain-2f-Tasks-2f-ReceiverTasks:
	-$(RM) ./Core/Src/appMain/Tasks/ReceiverTasks/receiverTasks.cyclo ./Core/Src/appMain/Tasks/ReceiverTasks/receiverTasks.d ./Core/Src/appMain/Tasks/ReceiverTasks/receiverTasks.o ./Core/Src/appMain/Tasks/ReceiverTasks/receiverTasks.su

.PHONY: clean-Core-2f-Src-2f-appMain-2f-Tasks-2f-ReceiverTasks

