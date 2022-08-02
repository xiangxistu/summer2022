################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../libraries/N32G45x_Firmware_Library/CMSIS/device/startup/startup_n32g45x_gcc.s 

O_SRCS += \
../libraries/N32G45x_Firmware_Library/CMSIS/device/startup/startup_n32g45x_gcc.o 

OBJS += \
./libraries/N32G45x_Firmware_Library/CMSIS/device/startup/startup_n32g45x_gcc.o 

S_DEPS += \
./libraries/N32G45x_Firmware_Library/CMSIS/device/startup/startup_n32g45x_gcc.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/N32G45x_Firmware_Library/CMSIS/device/startup/%.o: ../libraries/N32G45x_Firmware_Library/CMSIS/device/startup/%.s
	arm-none-eabi-gcc -x assembler-with-cpp -Xassembler -mimplicit-it=thumb -c -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -x assembler-with-cpp -Wa,-mimplicit-it=thumb  -gdwarf-2 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

