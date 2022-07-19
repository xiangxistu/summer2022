################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rt-thread/libcpu/arm/common/backtrace.c \
../rt-thread/libcpu/arm/common/div0.c \
../rt-thread/libcpu/arm/common/showmem.c 

OBJS += \
./rt-thread/libcpu/arm/common/backtrace.o \
./rt-thread/libcpu/arm/common/div0.o \
./rt-thread/libcpu/arm/common/showmem.o 

C_DEPS += \
./rt-thread/libcpu/arm/common/backtrace.d \
./rt-thread/libcpu/arm/common/div0.d \
./rt-thread/libcpu/arm/common/showmem.d 


# Each subdirectory must supply rules for building sources it contributes
rt-thread/libcpu/arm/common/%.o: ../rt-thread/libcpu/arm/common/%.c
	arm-none-eabi-gcc -I"D:\RT-Thread\RT-ThreadStudio\workspace\CPK-RA6M4" -I"D:\RT-Thread\RT-ThreadStudio\workspace\CPK-RA6M4\board\ports" -I"D:\RT-Thread\RT-ThreadStudio\workspace\CPK-RA6M4\board" -I"D:\RT-Thread\RT-ThreadStudio\workspace\CPK-RA6M4\libraries\HAL_Drivers\config" -I"D:\RT-Thread\RT-ThreadStudio\workspace\CPK-RA6M4\libraries\HAL_Drivers" -I"D:\RT-Thread\RT-ThreadStudio\workspace\CPK-RA6M4\ra\arm\CMSIS_5\CMSIS\Core\Include" -I"D:\RT-Thread\RT-ThreadStudio\workspace\CPK-RA6M4\ra\fsp\inc\api" -I"D:\RT-Thread\RT-ThreadStudio\workspace\CPK-RA6M4\ra\fsp\inc\instances" -I"D:\RT-Thread\RT-ThreadStudio\workspace\CPK-RA6M4\ra\fsp\inc" -I"D:\RT-Thread\RT-ThreadStudio\workspace\CPK-RA6M4\ra_cfg\fsp_cfg\bsp" -I"D:\RT-Thread\RT-ThreadStudio\workspace\CPK-RA6M4\ra_cfg\fsp_cfg" -I"D:\RT-Thread\RT-ThreadStudio\workspace\CPK-RA6M4\ra_gen" -I"D:\RT-Thread\RT-ThreadStudio\workspace\CPK-RA6M4\rt-thread\components\drivers\include" -I"D:\RT-Thread\RT-ThreadStudio\workspace\CPK-RA6M4\rt-thread\components\finsh" -I"D:\RT-Thread\RT-ThreadStudio\workspace\CPK-RA6M4\rt-thread\components\libc\compilers\common" -I"D:\RT-Thread\RT-ThreadStudio\workspace\CPK-RA6M4\rt-thread\components\libc\compilers\newlib" -I"D:\RT-Thread\RT-ThreadStudio\workspace\CPK-RA6M4\rt-thread\components\libc\posix\io\poll" -I"D:\RT-Thread\RT-ThreadStudio\workspace\CPK-RA6M4\rt-thread\components\libc\posix\io\stdio" -I"D:\RT-Thread\RT-ThreadStudio\workspace\CPK-RA6M4\rt-thread\components\libc\posix\ipc" -I"D:\RT-Thread\RT-ThreadStudio\workspace\CPK-RA6M4\rt-thread\include" -I"D:\RT-Thread\RT-ThreadStudio\workspace\CPK-RA6M4\rt-thread\libcpu\arm\common" -I"D:\RT-Thread\RT-ThreadStudio\workspace\CPK-RA6M4\rt-thread\libcpu\arm\cortex-m4" -include"D:\RT-Thread\RT-ThreadStudio\workspace\CPK-RA6M4\rtconfig_preinc.h" -std=gnu11 -mcpu=cortex-m33 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -Dgcc -O0 -gdwarf-2 -g -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

