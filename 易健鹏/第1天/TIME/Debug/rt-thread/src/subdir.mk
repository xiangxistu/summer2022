################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rt-thread/src/clock.c \
../rt-thread/src/components.c \
../rt-thread/src/device.c \
../rt-thread/src/idle.c \
../rt-thread/src/ipc.c \
../rt-thread/src/irq.c \
../rt-thread/src/kservice.c \
../rt-thread/src/mem.c \
../rt-thread/src/memheap.c \
../rt-thread/src/mempool.c \
../rt-thread/src/object.c \
../rt-thread/src/scheduler.c \
../rt-thread/src/thread.c \
../rt-thread/src/timer.c 

OBJS += \
./rt-thread/src/clock.o \
./rt-thread/src/components.o \
./rt-thread/src/device.o \
./rt-thread/src/idle.o \
./rt-thread/src/ipc.o \
./rt-thread/src/irq.o \
./rt-thread/src/kservice.o \
./rt-thread/src/mem.o \
./rt-thread/src/memheap.o \
./rt-thread/src/mempool.o \
./rt-thread/src/object.o \
./rt-thread/src/scheduler.o \
./rt-thread/src/thread.o \
./rt-thread/src/timer.o 

C_DEPS += \
./rt-thread/src/clock.d \
./rt-thread/src/components.d \
./rt-thread/src/device.d \
./rt-thread/src/idle.d \
./rt-thread/src/ipc.d \
./rt-thread/src/irq.d \
./rt-thread/src/kservice.d \
./rt-thread/src/mem.d \
./rt-thread/src/memheap.d \
./rt-thread/src/mempool.d \
./rt-thread/src/object.d \
./rt-thread/src/scheduler.d \
./rt-thread/src/thread.d \
./rt-thread/src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
rt-thread/src/%.o: ../rt-thread/src/%.c
	arm-none-eabi-gcc -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\applications" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\board" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\libraries\N32G45x_Firmware_Library\CMSIS\core" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\libraries\N32G45x_Firmware_Library\CMSIS\device" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\libraries\N32G45x_Firmware_Library\n32g45x_std_periph_driver\inc" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\libraries\n32_drivers" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\rt-thread\components\drivers\include" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\rt-thread\components\drivers\spi" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\rt-thread\components\finsh" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\rt-thread\components\libc\compilers\common" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\rt-thread\components\libc\compilers\newlib" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\rt-thread\components\libc\posix\io\poll" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\rt-thread\components\libc\posix\io\stdio" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\rt-thread\components\libc\posix\ipc" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\rt-thread\include" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\rt-thread\libcpu\arm\common" -I"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\rt-thread\libcpu\arm\cortex-m4" -include"D:\software\RT-ThreadStudio\RT-ThreadStudio\workspace\TIME\rtconfig_preinc.h" -std=gnu11 -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -Dgcc -O0 -gdwarf-2 -g -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

