作业

![image-20220722154828356](C:\Users\刘佩羽\Desktop\GIT教学\summer2022\liupeiyu\第4天\学习笔记\note.assets\image-20220722154828356.png)



![image-20220722154900137](C:\Users\刘佩羽\Desktop\GIT教学\summer2022\liupeiyu\第5天\学习笔记\note.assets\image-20220722154900137.png)

串口驱动既是作业，也是明天AT组件所依赖的
今天看明白一点，明天就会轻松些

# [SPI 设备](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/spi/spi?id=spi-设备)

## [SPI 简介](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/spi/spi?id=spi-简介)

SPI（Serial Peripheral Interface，串行外设接口）是一种高速、全双工、同步通信总线，常用于短距离通讯，主要应用于 EEPROM、FLASH、实时时钟、AD 转换器、还有数字信号处理器和数字信号解码器之间。SPI 一般使用 4 根线通信，如下图所示：

![image-20220722142447524](C:\Users\刘佩羽\Desktop\GIT教学\summer2022\liupeiyu\第4天\学习笔记\note.assets\image-20220722142447524-16585018764982.png)

- MOSI –主机输出 / 从机输入数据线（SPI Bus Master Output/Slave Input）。
- MISO –主机输入 / 从机输出数据线（SPI Bus Master Input/Slave Output)。
- SCLK –串行时钟线（Serial Clock），主设备输出时钟信号至从设备。
- CS –从设备选择线 (Chip select)。也叫 SS、CSB、CSN、EN 等，主设备输出片选信号至从设备。

SPI 以主从方式工作，通常有一个主设备和一个或多个从设备。通信由主设备发起，主设备通过 CS 选择要通信的从设备，然后通过 SCLK 给从设备提供时钟信号，数据通过 MOSI 输出给从设备，同时通过 MISO 接收从设备发送的数据。

如下图所示芯片有 2 个 SPI 控制器，SPI 控制器对应 SPI 主设备，每个 SPI 控制器可以连接多个 SPI 从设备。挂载在同一个 SPI 控制器上的从设备共享 3 个信号引脚：SCK、MISO、MOSI，但每个从设备的 CS 引脚是独立的。

![image-20220723224737295](C:\Users\刘佩羽\Desktop\GIT教学\summer2022\liupeiyu\第4天\学习笔记\note.assets\image-20220723224737295.png)



从设备的时钟由主设备通过 SCLK 提供，MOSI、MISO 则基于此脉冲完成数据传输。SPI 的工作时序模式由 CPOL（Clock Polarity，时钟极性）和 CPHA（Clock Phase，时钟相位）之间的相位关系决定，CPOL 表示时钟信号的初始电平的状态，CPOL 为 0 表示时钟信号初始状态为低电平，为 1 表示时钟信号的初始电平是高电平。CPHA 表示在哪个时钟沿采样数据，CPHA 为 0 表示在首个时钟变化沿采样数据，而 CPHA 为 1 则表示在第二个时钟变化沿采样数据。根据 CPOL 和 CPHA 的不同组合共有 4 种工作时序模式：①CPOL=0，CPHA=0、②CPOL=0，CPHA=1、③CPOL=1，CPHA=0、④CPOL=1，CPHA=1。如下图所示：	

![image-20220723224936197](C:\Users\刘佩羽\Desktop\GIT教学\summer2022\liupeiyu\第4天\学习笔记\note.assets\image-20220723224936197.png)

## [挂载 SPI 设备](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/spi/spi?id=挂载-spi-设备)

SPI 驱动会注册 SPI 总线，SPI 设备需要挂载到已经注册好的 SPI 总线上。

```c
rt_err_t rt_spi_bus_attach_device(struct rt_spi_device *device,
                                  const char           *name,
                                  const char           *bus_name,
                                  void                 *user_data)
```

| **参数**   | **描述**     |
| ---------- | ------------ |
| device     | SPI 设备句柄 |
| name       | SPI 设备名称 |
| bus_name   | SPI 总线名称 |
| user_data  | 用户数据指针 |
| **返回**   | ——           |
| RT_EOK     | 成功         |
| 其他错误码 | 失败         |

此函数用于挂载一个 SPI 设备到指定的 SPI 总线，并向内核注册 SPI 设备，并将 user_data 保存到 SPI 设备的控制块里。

一般 SPI 总线命名原则为 spix， SPI 设备命名原则为 spixy ，如 spi10 表示挂载在 spi1 总线上的 0 号设备。user_data 一般为 SPI 设备的 CS 引脚指针，进行数据传输时 SPI 控制器会操作此引脚进行片选。

未完待续……

# [RTC 设备](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/rtc/rtc?id=rtc-设备)

## [RTC 简介](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/rtc/rtc?id=rtc-简介)

RTC （Real-Time Clock）实时时钟可以提供精确的实时时间，它可以用于产生年、月、日、时、分、秒等信息。目前实时时钟芯片大多采用精度较高的晶体振荡器作为时钟源。有些时钟芯片为了在主电源掉电时还可以工作，会外加电池供电，使时间信息一直保持有效。

RT-Thread 的 RTC设备为操作系统的时间系统提供了基础服务。面对越来越多的 IoT 场景，RTC 已经成为产品的标配，甚至在诸如 SSL 的安全传输过程中，RTC 已经成为不可或缺的部分。

使用方法：

1，#include <rtthread.h>
	  #include <rtdevice.h>

2，set_date(年，月，日)【之后可以加一个判断语句，看是否建立成功】--->set_time(时，分，秒)---

【之后可以加一个判断语句，看是否建立成功】---->rt_thread_mdelay(若干时间);------>读取now = time(RT_NULL);----->显示rt_kprintf("%s\n", ctime(&now));

![image-20220722215108018](C:\Users\刘佩羽\Desktop\GIT教学\summer2022\liupeiyu\第4天\学习笔记\note.assets\image-20220722215108018.png)

## [Kconfig语法及示例](https://www.rt-thread.org/document/site/#/development-tools/build-config-system/Kconfig?id=kconfig语法及示例)

Kconfig源于[Linux内核的配置构建系统](https://www.kernel.org/doc/html/latest/kbuild/kconfig-language.html#)，当前只介绍RT-Thread中的常用语法。

Kconfig 采用 `#` 作为注释标记符，例如：

```C
# This is a comment
```

### [config语句](https://www.rt-thread.org/document/site/#/development-tools/build-config-system/Kconfig?id=config语句)

config 定义了一组新的配置选项

以下为 RT-Thread 系统中 config 语句的示例

```C
config BSP_USING_GPIO
    bool "Enable GPIO"
    select RT_USING_PIN
    default y
    help
    config gpio
```

语句分析：

- config 表示一个配置选项的开始，紧跟着的 BSP_USING_GPIO 是配置选项的名称，config 下面几行定义了该配置选项的属性。属性可以是该配置选项的

  - 类型
  - 输入提示
  - 依赖关系
  - 默认值
  - 帮助信息

- bool 表示配置选项的类型，每个 config 菜单项都要有类型定义，变量有5种类型

  - bool 布尔类型
  - tristate 三态类型
  - string 字符串
  - hex 十六进制
  - int 整型

- select 是反向依赖关系的意思，即当前配置选项被选中，则 RT_USING_PIN 就会被选中。

- default 表示配置选项的默认值，bool 类型的默认值可以是 y/n。

- help 帮助信息。

  通过 env 选中以上配置界面的选项后，最终可在 rtconfig.h 文件中生成如下两个宏

  ```C
  #define RT_USING_PIN
  #define BSP_USING_GPIO
  ```

#### [bool类型](https://www.rt-thread.org/document/site/#/development-tools/build-config-system/Kconfig?id=bool类型)

布尔类型变量的取值范围是 y/n 

### [menu/endmenu语句](https://www.rt-thread.org/document/site/#/development-tools/build-config-system/Kconfig?id=menuendmenu语句)

menu 语句用于生成菜单。

以下为 RT-Thread 系统中 menu/endmenu 语句的示例

```C
menu "Hardware Drivers Config"
    config BSP_USING_COM2
        bool "Enable COM2 (uart2 pin conflict with Ethernet and PWM)"
        select BSP_USING_UART
        select BSP_USING_UART2
        default n
    config BSP_USING_COM3
        bool "Enable COM3 (uart3 pin conflict with Ethernet)"
        select BSP_USING_UART3
        default n
endmenu
```

通过 env 选中以上配置界面的所有选项后，最终可在 rtconfig.h 文件中生成如下五个宏

```C
#define BSP_USING_UART
#define BSP_USING_UART2
#define BSP_USING_UART3
#define BSP_USING_COM2
#define BSP_USING_COM3
```

### [if/endif语句](https://www.rt-thread.org/document/site/#/development-tools/build-config-system/Kconfig?id=ifendif语句)

if/endif 语句是一个条件判断，定义了一个 if 结构，示例代码如下

```C
menu "Hardware Drivers Config"
    menuconfig BSP_USING_CAN
        bool "Enable CAN"
        default n
        select RT_USING_CAN
        if BSP_USING_CAN
            config BSP_USING_CAN1
                bool "Enable CAN1"
                default n
        endif
endmenu复制错误复制成功
```

当没有选中 "Enable CAN" 选项时，下面通过 if 判断的 Enable CAN1 选项并不会显示出来

### [menuconfig语句](https://www.rt-thread.org/document/site/#/development-tools/build-config-system/Kconfig?id=menuconfig语句)

menuconfig 语句表示带菜单的配置项

### [comment语句](https://www.rt-thread.org/document/site/#/development-tools/build-config-system/Kconfig?id=comment语句)

comment 语句出现在界面的第一行，用于定义一些提示信息。
