################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rt-thread/components/libc/compilers/common/stdlib.c \
../rt-thread/components/libc/compilers/common/time.c 

OBJS += \
./rt-thread/components/libc/compilers/common/stdlib.o \
./rt-thread/components/libc/compilers/common/time.o 

C_DEPS += \
./rt-thread/components/libc/compilers/common/stdlib.d \
./rt-thread/components/libc/compilers/common/time.d 


# Each subdirectory must supply rules for building sources it contributes
rt-thread/components/libc/compilers/common/%.o: ../rt-thread/components/libc/compilers/common/%.c
	arm-none-eabi-gcc -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\applications" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\board" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\libraries\N32G45x_Firmware_Library\CMSIS\core" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\libraries\N32G45x_Firmware_Library\CMSIS\device" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\libraries\N32G45x_Firmware_Library\n32g45x_std_periph_driver\inc" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\libraries\n32_drivers" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\rt-thread\components\drivers\include" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\rt-thread\components\drivers\spi" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\rt-thread\components\finsh" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\rt-thread\components\libc\compilers\common" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\rt-thread\components\libc\compilers\newlib" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\rt-thread\components\libc\posix\io\poll" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\rt-thread\components\libc\posix\io\stdio" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\rt-thread\components\libc\posix\ipc" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\rt-thread\include" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\rt-thread\libcpu\arm\common" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\rt-thread\libcpu\arm\cortex-m4" -include"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\rtconfig_preinc.h" -std=gnu11 -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -Dgcc -O0 -gdwarf-2 -g -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

