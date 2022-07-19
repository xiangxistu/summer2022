# N32G45XVL-STB BSP 说明

## 简介

N32G45XVL-STB 是国民技术推出的一款N32G457系列的评估板，其搭载的MCU主要资源参数如下：

| 硬件      | 描述          |
| --------- | ------------- |
| 芯片型号  | N32G457VEL7   |
| CPU       | ARM Cortex M4 |
| 主频      | 144M          |
| 片内SRAM  | 144K          |
| 片内FLASH | 512K          |

## 编译说明

N32G45XVL-STB 板级包支持MDK5开发环境和GCC编译器，以下是具体版本信息：

| IDE/编译器 | 已测试版本                    |
| ---------- | ---------------------------- |
| MDK5       | V5.34                        |
| IAR        | V8.50                        |
| GCC        | GCC 6.2.1 20161205 (release) |

## 板载资源

- MCU：N32G457VEL7，主频 144MHz，512KB FLASH
- 常用外设
  - LED ：3个，LED1 (PA8)，LED2（PB4），LED3（PB5）
  - 按键：5个，RESET(NRST)，WAKEUP（PA0），KEY1（PA4），KEY2（PA5），KEY3（PA6）

- 常用接口：USB 接口(J3)
- 调试接口: 板载的 CMSIS-DAP SWD 下载(J4)，JLINK

## 外设支持

本 BSP 目前对外设驱动的支持情况如下：

| 驱动      | 支持情况   |            备注            |
| --------- | --------  | ------------------------   |
| UART      | 支持      | USART1/2/3, UART4/5/6/7    |
| GPIO      | 支持      | PA0, PA1... PK15 ---> PIN: 0, 1...79 |
| I2C       | 支持      | I2C1/2/3/4                 |
| SPI       | 支持      | SPI1/2/3                   |
| ADC       | 支持      | ADC1/2/3/4                 |
| CAN       | 支持      | CAN1/2                     |
| DAC       | 支持      | DAC1/2                     |
| HWTIMER   | 支持      | TIMER1/2/3/4/5/6/7/8       |
| WDT       | 支持      | IWDG                       |

### IO在板级支持包中的映射情况

| IO号 | 板级包中的定义 |
| ---- | -------------- |
| PA9  | USART1_TX      |
| PA10 | USART1_RX      |
| PA2  | USART2_TX      |
| PA3  | USART2_RX      |
| PB10 | USART3_TX      |
| PB11 | USART3_RX      |
| PC10 | USART4_TX      |
| PC11 | USART4_RX      |
| PC10 | USART5_TX      |
| PD2  | USART5_RX      |
| PE2  | USART6_TX      |
| PE3  | USART6_RX      |
| PC4  | USART7_TX      |
| PC5  | USART7_RX      |
| PA4  | SPI1_NSS       |
| PA5  | SPI1_SCK       |
| PA6  | SPI1_MISO      |
| PA7  | SPI1_MOSI      |
| PB12 | SPI2_NSS       |
| PB13 | SPI2_SCK       |
| PB14 | SPI2_MISO      |
| PB15 | SPI2_MOSI      |
| PB6  | I2C1_SCL       |
| PB7  | I2C1_SDA       |

| PC0  | ADC1/2_IN10    |
| PC1  | ADC1/2_IN11    |
| PC2  | ADC1/2_IN12    |
| PC3  | ADC1/2_IN13    |

## 使用说明

    本章节是为刚接触 RT-Thread 的新手准备的使用说明，遵循简单的步骤即可将 RT-Thread 操作系统运行在该开发板上，看到实验效果 。

### 快速上手

本 BSP 为开发者提供 MDK5工程，IAR工程，并且支持 GCC 开发环境。下面以 MDK5 开发环境为例，介绍如何将系统运行起来。

#### 硬件连接

使用数据线连接开发板到 PC，打开电源开关。

#### 编译下载

双击 project.uvprojx 文件，打开 MDK5 工程，编译并下载程序到开发板。

#### 运行结果

下载程序成功之后，系统会自动运行，观察开发板上 LED 的运行效果，LED3 会周期性闪烁。

连接开发板对应串口到 PC , 在终端工具里打开相应的串口（115200-8-1-N），复位设备后，在串口上可以看到 RT-Thread 的输出信息:

```bash
 \ | /
- RT -     Thread Operating System
 / | \     4.1.1 build Apr 24 2022 17:24:22
 2006 - 2022 Copyright by RT-Thread team
msh >
```

## 注意事项

暂无

## 联系人信息

维护人:

- [LinYuanbo](https://github.com/Lim-LinYuanbo)
- [breo.com](https://github.com/breo-shenzhen)
