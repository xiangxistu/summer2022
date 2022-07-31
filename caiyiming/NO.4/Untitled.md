1.在rtt操作系统中使用了Kconfig文件和SConstruct脚本两种方式来组件工程。

2.因为我使用的是HPM6750这块板子，我们打开两个I2C，并且使能ulog

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=YmU0NjMwMmEyYjMzNGU3ZTdkOWQ4ODY5Mjg5MzdjMmZfTXMyQlFOcTU0bjBabm1GeUFjSHRJbDZEQVgwMnlzMTJfVG9rZW46Ym94Y25ma0Z5cGY1cVJvMDUyU0tIYjdnYnFjXzE2NTg1Mzk0ODY6MTY1ODU0MzA4Nl9WNA)

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=MGQ3OTVlNWMzZWNhZjBiNWU1ZTcxOWE2ZjFjYmRiZjJfSEdtQXcyaE41Y0d5cmZiTkNQYkV1RGZmNUdzTUVGbkpfVG9rZW46Ym94Y25yMWNWeG5yb0QwcHdsUXVCb3hLMnBCXzE2NTg1Mzk0ODY6MTY1ODU0MzA4Nl9WNA)

  

只是这样配置的话我们出来显示的是这种情况

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=MmY5M2RhODI0ODJiNzA4ODY3NjdhNmY4ZWE4YTYwODdfbTFwNHBCaGZBUGNLb25adDZzdDVwbXh5SHlRbWdEblZfVG9rZW46Ym94Y25pSzlQcnBiYTFVMGxVaHBnSFRRWGloXzE2NTg1Mzk0ODY6MTY1ODU0MzA4Nl9WNA)

原因在于我们静态输出日志级别的设定

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=ODM0OWU4NDM5MmJhYWNkNzEwOWI5NDA0MWYwZGU1YjVfdGIwcDFBVEJlRFlsT0lXQ1hHOWtDM0U5SGZkSGlyMHZfVG9rZW46Ym94Y25EbmRISDF1SzBHcHNvNmlDTkdKMlJkXzE2NTg1Mzk0ODY6MTY1ODU0MzA4Nl9WNA)

当我们把等级改到information后就好啦

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=MjQ5Y2FlOWEwZmJkN2M1NTVlZGYzNDUzZGNjOGM0MDlfMnV1RkowbUFERmVyUHc4b3V1ZjRMVHJyQjhCMDNYQVhfVG9rZW46Ym94Y25wOXB3aVVUT3R0Z1g3dkdvMFJTOUhmXzE2NTg1Mzk0ODY6MTY1ODU0MzA4Nl9WNA)

Crtl+b是编译的快捷键

改log等级输出的颜色不一样

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=NGY1YjU4MTJlYWRjOTA0ZDZiZDUxNGM5M2NmOGU3NDNfb2VGS1p5U1ZNOWNFa1dkeFlvaUkwTHg5Z1o2dHZKM1VfVG9rZW46Ym94Y24yNkFTU3Ezdk9VMHFMMlhRN2xWckRlXzE2NTg1Mzk0ODY6MTY1ODU0MzA4Nl9WNA)

异步通信是通过创建一个线程来输出，对时间敏感的可以选择异步。

Konfig来管理我们整个工程，打开RTTSetting,第一步就是去读Kconfig这个文件

下午老师先是讲解了一个关于RTT Setting里面的一些硬件和软件包

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=MWVjOTg0ODZhNGZiMTlhNzBlM2ViY2Q5ZmQ0YzMzZjdfS1pNQ1I3amYyRE9sVGFRN0dFZkthcmZFa1hWMGtrTEtfVG9rZW46Ym94Y25DdUhnTTJPMFhzc0VTamFqUVdGSU5TXzE2NTg1Mzk0ODY6MTY1ODU0MzA4Nl9WNA)

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=NjVjOTk1ODc4MDc2YzE1MjA1YjJmNmVkNzYzYmE1ZGNfWXJXcHk4UHdGTnNmMFlocDZhMWNpOURNdFpmQldOQjBfVG9rZW46Ym94Y25kdHR0Z2sxNnluOVZHU1NxQ2x4SEJjXzE2NTg1Mzk0ODY6MTY1ODU0MzA4Nl9WNA)

在这个过程中我遇到了这个问题：总是不等我输入完确定日期 怎么就进行温湿度的显示了呢

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=YTliNTZkZGVlMDY2ZDI5MmRjY2M2NDBlODcxZDUyZjBfVzE2eWtOd2hOMEpUam1xbHJKWHpjOWJqMTllczRkWjhfVG9rZW46Ym94Y25CT0FKcG9WQTNlVnJtd0hpUGh3VDNmXzE2NTg1Mzk0ODY6MTY1ODU0MzA4Nl9WNA)

SPI外设

SPI（串行外设接口）是一种高速、全双工、同步通信总线，主要应用于 EEPROM、FLASH、实时时钟、AD 转换器、还有数字信号处理器和数字信号解码器之间。SPI 一般使用 4 根线通信，如下图所示：

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=NmU0ZDJkMTZhYmU4OGJjNmIxNDdjMWQzZGE2MDI4MDVfQ3NJSFgwNU9NbVduUHF4NlY5SmxjeVpRQzBoTHBTamFfVG9rZW46Ym94Y25MYkg0SHUxVGJlZDU4YXVDWGs4Q2JjXzE2NTg1Mzk0ODY6MTY1ODU0MzA4Nl9WNA)

像我手中的HPM6750背面的RW007就是使用的SPI。和WIFI有关

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=MTQ3YWJhNzUwNzBkZmJhZmRjYmYwZTljZWYzZWIzZWFfa2ZSRDFjV0JabGo0U3pXek1hdnNUZWl2S2V5R1BaWjRfVG9rZW46Ym94Y252N0dqWHdzd1pkajY4VTNtNXg1QkFnXzE2NTg1Mzk0ODY6MTY1ODU0MzA4Nl9WNA)

SPI 以主从方式工作，通常有一个主设备和一个或多个从设备。通信由主设备发起，主设备通过 CS 选择要通信的从设备，然后通过 SCLK 给从设备提供时钟信号，数据通过 MOSI 输出给从设备，同时通过 MISO 接收从设备发送的数据。

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=MmRkNWU5NjFlNDg0NmUwYjNhMjM4YTFiZmM1ZjkyMGRfSUlTSUpzZFFXZUg1UkZpRTFLV0xDRXVZcnJ3TVh1VmpfVG9rZW46Ym94Y256Z2dVbVNJMXN6UWFtU29jNFBndU1lXzE2NTg1Mzk0ODY6MTY1ODU0MzA4Nl9WNA)SPI开发模式

1.注册SPI Device设备 2.打开注册SPI Device设备

3.使用SPI框架提供API编程发送接收数据

4.关闭SPI Device设备

1.注册SPI Device设备

[挂载 SPI 设备](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/spi/spi?id=%e6%8c%82%e8%bd%bd-spi-%e8%ae%be%e5%a4%87)

SPI 驱动会注册 SPI 总线，SPI 设备需要挂载到已经注册好的 SPI 总线上

```Plaintext
rt_err_t rt_spi_bus_attach_device(struct rt_spi_device *device,const char           *name,const char           *bus_name,void                 *user_data)
```

此函数用于挂载一个 SPI 设备到指定的 SPI 总线，并向内核注册 SPI 设备，并将 user_data 保存到 SPI 设备的控制块里。

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=ZDcyZTRkNDBmNzRkMTJkYmM3NTg3YjI2NTc0MWYwYWZfN2JleDdQQmwxdXlXUWNsTTk0eGZiRUd3RUoySWNtc0VfVG9rZW46Ym94Y25WOUswcG9LUmdKQ0cyM0dsWHpDYVRkXzE2NTg1Mzk1Mzc6MTY1ODU0MzEzN19WNA)

若使用 rt-thread/bsp/stm32 目录下的 BSP 则可以使用下面的函数挂载 SPI 设备到总线：

GPIO特殊

```Plaintext
rt_err_t rt_hw_spi_device_attach(const char *bus_name, const char *device_name, GPIO_TypeDef* cs_gpiox, uint16_t cs_gpio_pin);
```

一般 SPI 总线命名原则为 spix， SPI 设备命名原则为 spixy ，如 spi10 表示挂载在 spi1 总线上的 0 号设备。user_data 一般为 SPI 设备的 CS 引脚指针，进行数据传输时 SPI 控制器会操作此引脚进行片选。

### [查找 SPI 设备](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/spi/spi?id=%e6%9f%a5%e6%89%be-spi-%e8%ae%be%e5%a4%87)

在使用 SPI 设备前需要根据 SPI 设备名称获取设备句柄，进而才可以操作 SPI 设备，查找设备函数如下所示，

```Plaintext
rt_device_t rt_device_find(const char* name);
```

参数

描述

name

设备名称

返回

——

设备句柄

查找到对应设备将返回相应的设备句柄

RT_NULL

没有找到相应的设备对象

  

## [配置 SPI 设备](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/spi/spi?id=%e9%85%8d%e7%bd%ae-spi-%e8%ae%be%e5%a4%87)

挂载 SPI 设备到 SPI 总线后需要配置 SPI 设备的传输参数。

```Plaintext
rt_err_t rt_spi_configure(struct rt_spi_device *device,struct rt_spi_configuration *cfg)
```



```Plaintext
struct rt_spi_configuration
{
    rt_uint8_t mode;        /* 模式 */
    rt_uint8_t data_width;  /* 数据宽度，可取8位、16位、32位 */
    rt_uint16_t reserved;   /* 保留 */
    rt_uint32_t max_hz;     /* 最大频率 */
};
```

### [传输一次数据](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/spi/spi?id=%e4%bc%a0%e8%be%93%e4%b8%80%e6%ac%a1%e6%95%b0%e6%8d%ae)

如果只传输一次数据可以通过如下函数：

```Plaintext
rt_size_t rt_spi_transfer(struct rt_spi_device *device,const void           *send_buf,void                  *recv_buf,
                          rt_size_t             length);
```

### [发送一次数据](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/spi/spi?id=%e5%8f%91%e9%80%81%e4%b8%80%e6%ac%a1%e6%95%b0%e6%8d%ae)

如果只发送一次数据，而忽略接收到的数据可以通过如下函数：

```Plaintext
rt_size_t rt_spi_send(struct rt_spi_device *device,const void           *send_buf,
                      rt_size_t             length)
```


### [接收一次数据](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/spi/spi?id=%e6%8e%a5%e6%94%b6%e4%b8%80%e6%ac%a1%e6%95%b0%e6%8d%ae)

如果只接收一次数据可以通过如下函数：

```Plaintext
rt_size_t rt_spi_recv(struct rt_spi_device *device,void                 *recv_buf,
                      rt_size_t             length);
```

### [连续两次发送数据](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/spi/spi?id=%e8%bf%9e%e7%bb%ad%e4%b8%a4%e6%ac%a1%e5%8f%91%e9%80%81%e6%95%b0%e6%8d%ae)

如果需要先后连续发送 2 个缓冲区的数据，并且中间片选不释放，可以调用如下函数:

```Plaintext
rt_err_t rt_spi_send_then_send(struct rt_spi_device *device,const void           *send_buf1,
                               rt_size_t             send_length1,const void           *send_buf2,
                               rt_size_t             send_length2);
```

### [自定义传输数据](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/spi/spi?id=%e8%87%aa%e5%ae%9a%e4%b9%89%e4%bc%a0%e8%be%93%e6%95%b0%e6%8d%ae)

获取到 SPI 设备句柄就可以使用 SPI 设备管理接口访问 SPI 设备器件，进行数据收发。可以通过如下函数传输消息：

```Plaintext
struct rt_spi_message *rt_spi_transfer_message(struct rt_spi_device  *device,struct rt_spi_message *message)；
```


发送失败，返回指向剩余未发送的 message 的指针

endbuf 为发送缓冲区指针，其值为 RT_NULL 时，表示本次传输为只接收状态，不需要发送数据。

recvbuf 为接收缓冲区指针，其值为 RT_NULL 时，表示本次传输为只发送状态，不需要保存接收到的数据，所以收到的数据直接丢弃。

length 的单位为 word，即数据长度为 8 位时，每个 length 占用 1 个字节；当数据长度为 16 位时，每个 length 占用 2 个字节。

参数 next 是指向继续发送的下一条消息的指针，若只发送一条消息，则此指针值为 RT_NULL。多个待传输的消息通过 next 指针以单向链表的形式连接在一起。

cs_take 值为 1 时，表示在传输数据前，设置对应的 CS 为有效状态。cs_release 值为 1 时，表示在数据传输结束后，释放对应的 CS。

  

跟着老师一起学习RW007链接wifi

我遇到了这个问题

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=NTE2NDQyZjk1ZjBlMDRmN2NmMDNhY2ZhMjNlNDllOTVfUVEyenBzYm5jVDdXY2hvVVlPS3B0Tkg5V3VxWHMyTkVfVG9rZW46Ym94Y25JWnpIM291WnpYajl3cEhaNkpoWkJiXzE2NTg1Mzk1Mzc6MTY1ODU0MzEzN19WNA)

  

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=MjAwYTMwZWIzOGI2MzVjN2MzMmQ2ZWM5YzY4MjU0YjlfeGVwbHhsSzVScTlpNUdYYVl5SmFwSlFUa1V3T3JUa3RfVG9rZW46Ym94Y25NeXYwcFlVUEQzQURneFdnV2lYcTJiXzE2NTg1Mzk1Mzc6MTY1ODU0MzEzN19WNA)

经过调整，可以显示IP地址 需要注意：输入完wifi join 还要输入 ifconfig 并且WIFI名称密码要正确

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=NjU4MmE2MWEyOTg2M2I3MTVmOTNmNTE2ZmU1OTU5MGVfa3JjOFRjS0NXZVVNVkpNSmQybVZSdWZtdTRIWWoyeGxfVG9rZW46Ym94Y25BZkhiRlE2bVRUQ1lRcmtQdEtRQnJnXzE2NTg1Mzk1Mzc6MTY1ODU0MzEzN19WNA)

联网之后可以显示实时时间

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=ZGYyMzcyMDMzNTRiYzBiNzM2ODhkYTBiZWZmN2ZkNTNfYUs0ZnVMNEh1dmtwVnh6NlpiZ29rRzc0YWRTTlZsdkpfVG9rZW46Ym94Y25IOTR0SGFKSlZ1a2hqQ2RmcXNtREViXzE2NTg1Mzk1Mzc6MTY1ODU0MzEzN19WNA)

RTC外设

RTC （Real-Time Clock）实时时钟可以提供精确的实时时间，它可以用于产生年、月、日、时、分、秒等信息。目前实时时钟芯片大多采用精度较高的晶体振荡器作为时钟源。有些时钟芯片为了在主电源掉电时还可以工作，会外加电池供电，使时间信息一直保持有效。

### [获取当前时间](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/rtc/rtc?id=%e8%8e%b7%e5%8f%96%e5%bd%93%e5%89%8d%e6%97%b6%e9%97%b4)

使用到 C 标准库中的时间 API 获取时间戳（格林威治时间）：

```Plaintext
time_t time(time_t *t)复制错误复制成功
```


```Plaintext
time_t now;     /* 保存获取的当前时间值 */
/* 获取时间 */
now = time(RT_NULL);
/* 打印输出时间信息 */
rt_kprintf("%s\n", ctime(&now))
```

### [设置时间](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/rtc/rtc?id=%e8%ae%be%e7%bd%ae%e6%97%b6%e9%97%b4)

通过如下函数设置 RTC 设备当前当地时区时间值：

```Plaintext
rt_err_t set_time(rt_uint32_t hour, rt_uint32_t minute, rt_uint32_t second)复制错误复制成功
```

使用示例如下所示：

```Plaintext
/* 设置时间为11点15分50秒 */
set_time(11, 15, 50);
```

MSH命令配置事件

输入date查看当前时间，使用date命令，在后面依次输入年月日时分秒

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=YjBiNmMxZDIyY2ZjNDA4NjU3ZThmYTVjYzczZDA1NzZfRml4TzVmMUFxSE41UklDSDJOS012ak96Z1dVTjBjYzdfVG9rZW46Ym94Y25raVBHMGJxbllxRDZ3ZjA1c2FUQ21mXzE2NTg1Mzk1Mzc6MTY1ODU0MzEzN19WNA)

Argc 6个

1.Function2. sdad 3.fD 4.FEDS 5.FEDEWDE 6. FSFDF 7.BHYG。

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=MWE1NDlmNmY0YzdiODhkNjI5NmEwNmYzMTc4OGYxOTZfYk1LN2dybEVRM3F6OUNlc0xLMlFSUDJBc0I1dE9IZ2RfVG9rZW46Ym94Y25HeUgyZ21qc3kyZzJvdlo3ODVtT1RaXzE2NTg1Mzk1Mzc6MTY1ODU0MzEzN19WNA)

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=NDk1MTJlNmRjMzczMTllZDIyZjE1MDgyOTJlNjFhYmZfR3FsQkpwMjRZeXlDVEtQend1U0pnMlRHeWxibFZRYURfVG9rZW46Ym94Y25zT2FmY1ZnbTA5bVlKUVVwQWYxdkFLXzE2NTg1Mzk1Mzc6MTY1ODU0MzEzN19WNA)

Set time设置时分秒 Set data获取年月日。

WDG看门狗外设

（1）硬件看门狗（watchdog timer）是一个定时器，其定时输出连接到电路的复位端。在产品化的嵌入式系统中，为了使系统在异常情况下能自动复位，一般都需要引入看门狗。

（2）当看门狗启动后，计数器开始自动计数，在计数器溢出前如果没有被复位，计数器溢出就会对 CPU 产生一个复位信号使系统重启（俗称 “被狗咬”）。系统正常运行时，需要在看门狗允许的时间间隔内对看门狗计数器清零（俗称“喂狗“），不让复位信号产生。如果系统不出问题，程序能够按时“喂狗”。一旦程序跑飞，没有“喂狗”，系统“被咬” 复位。

（3）复位信号是一个特别强的信号，是不可屏蔽的。

（4）一般情况下可以在 RT-Thread 的 **idle 回调函数（优先级最低的线程）（再也没有处于就绪状态的函数，有价值的线程都处于挂起的状态或者等待某些资源的状态，idle工作，主动喂狗）和关键任务里喂狗。**

看门狗是如何工作的
SPI开发模式

1.注册SPI Device设备 2.打开注册SPI Device设备

3.使用SPI框架提供API编程发送接收数据

4.关闭SPI Device设备

1.注册SPI Device设备

[挂载 SPI 设备](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/spi/spi?id=%e6%8c%82%e8%bd%bd-spi-%e8%ae%be%e5%a4%87)

SPI 驱动会注册 SPI 总线，SPI 设备需要挂载到已经注册好的 SPI 总线上

```Plaintext
rt_err_t rt_spi_bus_attach_device(struct rt_spi_device *device,const char           *name,const char           *bus_name,void                 *user_data)
```

此函数用于挂载一个 SPI 设备到指定的 SPI 总线，并向内核注册 SPI 设备，并将 user_data 保存到 SPI 设备的控制块里。

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=ZDcyZTRkNDBmNzRkMTJkYmM3NTg3YjI2NTc0MWYwYWZfN2JleDdQQmwxdXlXUWNsTTk0eGZiRUd3RUoySWNtc0VfVG9rZW46Ym94Y25WOUswcG9LUmdKQ0cyM0dsWHpDYVRkXzE2NTg1Mzk1Mzc6MTY1ODU0MzEzN19WNA)

若使用 rt-thread/bsp/stm32 目录下的 BSP 则可以使用下面的函数挂载 SPI 设备到总线：

GPIO特殊

```Plaintext
rt_err_t rt_hw_spi_device_attach(const char *bus_name, const char *device_name, GPIO_TypeDef* cs_gpiox, uint16_t cs_gpio_pin);
```

一般 SPI 总线命名原则为 spix， SPI 设备命名原则为 spixy ，如 spi10 表示挂载在 spi1 总线上的 0 号设备。user_data 一般为 SPI 设备的 CS 引脚指针，进行数据传输时 SPI 控制器会操作此引脚进行片选。

### [查找 SPI 设备](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/spi/spi?id=%e6%9f%a5%e6%89%be-spi-%e8%ae%be%e5%a4%87)

在使用 SPI 设备前需要根据 SPI 设备名称获取设备句柄，进而才可以操作 SPI 设备，查找设备函数如下所示，

```Plaintext
rt_device_t rt_device_find(const char* name);
```

  

## [配置 SPI 设备](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/spi/spi?id=%e9%85%8d%e7%bd%ae-spi-%e8%ae%be%e5%a4%87)

挂载 SPI 设备到 SPI 总线后需要配置 SPI 设备的传输参数。

```Plaintext
rt_err_t rt_spi_configure(struct rt_spi_device *device,struct rt_spi_configuration *cfg)
```


```Plaintext
struct rt_spi_configuration
{
    rt_uint8_t mode;        /* 模式 */
    rt_uint8_t data_width;  /* 数据宽度，可取8位、16位、32位 */
    rt_uint16_t reserved;   /* 保留 */
    rt_uint32_t max_hz;     /* 最大频率 */
};
```

### [传输一次数据](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/spi/spi?id=%e4%bc%a0%e8%be%93%e4%b8%80%e6%ac%a1%e6%95%b0%e6%8d%ae)

如果只传输一次数据可以通过如下函数：

```Plaintext
rt_size_t rt_spi_transfer(struct rt_spi_device *device,const void           *send_buf,void                  *recv_buf,
                          rt_size_t             length);
```

### [发送一次数据](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/spi/spi?id=%e5%8f%91%e9%80%81%e4%b8%80%e6%ac%a1%e6%95%b0%e6%8d%ae)

如果只发送一次数据，而忽略接收到的数据可以通过如下函数：

```Plaintext
rt_size_t rt_spi_send(struct rt_spi_device *device,const void           *send_buf,
                      rt_size_t             length)
```

](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/spi/spi?id=%e6%8e%a5%e6%94%b6%e4%b8%80%e6%ac%a1%e6%95%b0%e6%8d%ae)

如果只接收一次数据可以通过如下函数：

```Plaintext
rt_size_t rt_spi_recv(struct rt_spi_device *device,void                 *recv_buf,
                      rt_size_t             length);
```

### [连续两次发送数据](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/spi/spi?id=%e8%bf%9e%e7%bb%ad%e4%b8%a4%e6%ac%a1%e5%8f%91%e9%80%81%e6%95%b0%e6%8d%ae)

如果需要先后连续发送 2 个缓冲区的数据，并且中间片选不释放，可以调用如下函数:

```Plaintext
rt_err_t rt_spi_send_then_send(struct rt_spi_device *device,const void           *send_buf1,
                               rt_size_t             send_length1,const void           *send_buf2,
                               rt_size_t             send_length2);
```

### [自定义传输数据](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/spi/spi?id=%e8%87%aa%e5%ae%9a%e4%b9%89%e4%bc%a0%e8%be%93%e6%95%b0%e6%8d%ae)

获取到 SPI 设备句柄就可以使用 SPI 设备管理接口访问 SPI 设备器件，进行数据收发。可以通过如下函数传输消息：

```Plaintext
struct rt_spi_message *rt_spi_transfer_message(struct rt_spi_device  *device,struct rt_spi_message *message)；
```


发送失败，返回指向剩余未发送的 message 的指针

endbuf 为发送缓冲区指针，其值为 RT_NULL 时，表示本次传输为只接收状态，不需要发送数据。

recvbuf 为接收缓冲区指针，其值为 RT_NULL 时，表示本次传输为只发送状态，不需要保存接收到的数据，所以收到的数据直接丢弃。

length 的单位为 word，即数据长度为 8 位时，每个 length 占用 1 个字节；当数据长度为 16 位时，每个 length 占用 2 个字节。

参数 next 是指向继续发送的下一条消息的指针，若只发送一条消息，则此指针值为 RT_NULL。多个待传输的消息通过 next 指针以单向链表的形式连接在一起。

cs_take 值为 1 时，表示在传输数据前，设置对应的 CS 为有效状态。cs_release 值为 1 时，表示在数据传输结束后，释放对应的 CS。

  

跟着老师一起学习RW007链接wifi

我遇到了这个问题

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=NTE2NDQyZjk1ZjBlMDRmN2NmMDNhY2ZhMjNlNDllOTVfUVEyenBzYm5jVDdXY2hvVVlPS3B0Tkg5V3VxWHMyTkVfVG9rZW46Ym94Y25JWnpIM291WnpYajl3cEhaNkpoWkJiXzE2NTg1Mzk1Mzc6MTY1ODU0MzEzN19WNA)

  

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=MjAwYTMwZWIzOGI2MzVjN2MzMmQ2ZWM5YzY4MjU0YjlfeGVwbHhsSzVScTlpNUdYYVl5SmFwSlFUa1V3T3JUa3RfVG9rZW46Ym94Y25NeXYwcFlVUEQzQURneFdnV2lYcTJiXzE2NTg1Mzk1Mzc6MTY1ODU0MzEzN19WNA)

经过调整，可以显示IP地址 需要注意：输入完wifi join 还要输入 ifconfig 并且WIFI名称密码要正确

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=NjU4MmE2MWEyOTg2M2I3MTVmOTNmNTE2ZmU1OTU5MGVfa3JjOFRjS0NXZVVNVkpNSmQybVZSdWZtdTRIWWoyeGxfVG9rZW46Ym94Y25BZkhiRlE2bVRUQ1lRcmtQdEtRQnJnXzE2NTg1Mzk1Mzc6MTY1ODU0MzEzN19WNA)

联网之后可以显示实时时间

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=ZGYyMzcyMDMzNTRiYzBiNzM2ODhkYTBiZWZmN2ZkNTNfYUs0ZnVMNEh1dmtwVnh6NlpiZ29rRzc0YWRTTlZsdkpfVG9rZW46Ym94Y25IOTR0SGFKSlZ1a2hqQ2RmcXNtREViXzE2NTg1Mzk1Mzc6MTY1ODU0MzEzN19WNA)

RTC外设

RTC （Real-Time Clock）实时时钟可以提供精确的实时时间，它可以用于产生年、月、日、时、分、秒等信息。目前实时时钟芯片大多采用精度较高的晶体振荡器作为时钟源。有些时钟芯片为了在主电源掉电时还可以工作，会外加电池供电，使时间信息一直保持有效。

### [获取当前时间](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/rtc/rtc?id=%e8%8e%b7%e5%8f%96%e5%bd%93%e5%89%8d%e6%97%b6%e9%97%b4)

使用到 C 标准库中的时间 API 获取时间戳（格林威治时间）：

```Plaintext
time_t time(time_t *t)复制错误复制成功
```


```Plaintext
time_t now;     /* 保存获取的当前时间值 */
/* 获取时间 */
now = time(RT_NULL);
/* 打印输出时间信息 */
rt_kprintf("%s\n", ctime(&now))
```

### [设置时间](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/rtc/rtc?id=%e8%ae%be%e7%bd%ae%e6%97%b6%e9%97%b4)

通过如下函数设置 RTC 设备当前当地时区时间值：

```Plaintext
rt_err_t set_time(rt_uint32_t hour, rt_uint32_t minute, rt_uint32_t second)复制错误复制成功
```



```Plaintext
/* 设置时间为11点15分50秒 */
set_time(11, 15, 50);
```

MSH命令配置事件

输入date查看当前时间，使用date命令，在后面依次输入年月日时分秒

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=YjBiNmMxZDIyY2ZjNDA4NjU3ZThmYTVjYzczZDA1NzZfRml4TzVmMUFxSE41UklDSDJOS012ak96Z1dVTjBjYzdfVG9rZW46Ym94Y25raVBHMGJxbllxRDZ3ZjA1c2FUQ21mXzE2NTg1Mzk1Mzc6MTY1ODU0MzEzN19WNA)

Argc 6个

1.Function2. sdad 3.fD 4.FEDS 5.FEDEWDE 6. FSFDF 7.BHYG。

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=MWE1NDlmNmY0YzdiODhkNjI5NmEwNmYzMTc4OGYxOTZfYk1LN2dybEVRM3F6OUNlc0xLMlFSUDJBc0I1dE9IZ2RfVG9rZW46Ym94Y25HeUgyZ21qc3kyZzJvdlo3ODVtT1RaXzE2NTg1Mzk1Mzc6MTY1ODU0MzEzN19WNA)

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=NDk1MTJlNmRjMzczMTllZDIyZjE1MDgyOTJlNjFhYmZfR3FsQkpwMjRZeXlDVEtQend1U0pnMlRHeWxibFZRYURfVG9rZW46Ym94Y25zT2FmY1ZnbTA5bVlKUVVwQWYxdkFLXzE2NTg1Mzk1Mzc6MTY1ODU0MzEzN19WNA)

Set time设置时分秒 Set data获取年月日。

WDG看门狗外设

（1）硬件看门狗（watchdog timer）是一个定时器，其定时输出连接到电路的复位端。在产品化的嵌入式系统中，为了使系统在异常情况下能自动复位，一般都需要引入看门狗。

（2）当看门狗启动后，计数器开始自动计数，在计数器溢出前如果没有被复位，计数器溢出就会对 CPU 产生一个复位信号使系统重启（俗称 “被狗咬”）。系统正常运行时，需要在看门狗允许的时间间隔内对看门狗计数器清零（俗称“喂狗“），不让复位信号产生。如果系统不出问题，程序能够按时“喂狗”。一旦程序跑飞，没有“喂狗”，系统“被咬” 复位。

（3）复位信号是一个特别强的信号，是不可屏蔽的。

（4）一般情况下可以在 RT-Thread 的 **idle 回调函数（优先级最低的线程）（再也没有处于就绪状态的函数，有价值的线程都处于挂起的状态或者等待某些资源的状态，idle工作，主动喂狗）和关键任务里喂狗。**

看门狗是如何工作的
注册完成，由谁打开呢？

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=MTE2NGI0YjMwOGVlMGJlZDhmNDZiZDZlZWJlMTNhZTJfY2hwbllSNzl4WDBmdlRwZlVjWTZqN3VKbW5PZWdWMU1fVG9rZW46Ym94Y25RQkxVQlNPUDdRUjRkbENvVmpQQzJkXzE2NTg1Mzk2NjE6MTY1ODU0MzI2MV9WNA)

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=OTRjYzMxYjhlOGMwOGJlMDcyY2U4ZWZmNzU2ZjRhOWNfeDhJYTNiZjlsQ0l6anRJQXBEN2piaDZDcjJjSVFGdENfVG9rZW46Ym94Y240SjBsYVVUNFpNdGxoaUN2dDlPcTJkXzE2NTg1Mzk2NjE6MTY1ODU0MzI2MV9WNA)

这个是在rtt_board.c中。

这样串口的基本配置就被打开了

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=MzBmMmYxNjk4NTVlNTdhNzE2MDY5ZWFkN2I3YzQ0ZWRfT05pY3VYUE5PNjFWbGxlUkV1YXFqVEdSZnVib0E1WWVfVG9rZW46Ym94Y25EMldONE9qd2dNcTlUSEh3T3lJejhXXzE2NTg1Mzk2NjE6MTY1ODU0MzI2MV9WNA)

所以说我们在终端 list_device就可以看到uart0，串口设备了。

线程的时候 rt_thread_creat

rt_sem_creat

rt_sem_release

rt_event_recv

rt_mv_recv

//rt_device_find(name)//通过名称来找到一个设备，获取设备句柄

rt_devide_init;/*有些设备只注册了Init接口

rt_device_open(0)

rt_device_open(0)//AHT10 sensor

rt_device_write()

rt_device_read()

rt_device_set_rx_indicate(serial,serial_rx_ind)//对于串口而言的回调函数

官方文档中有关串口

UART 串口的特点是将数据一位一位地顺序传送，只要 2 根传输线就可以实现双向通信，一根线发送数据的同时用另一根线接收数据。UART 串口通信有几个重要的参数，分别是波特率、起始位、数据位、停止位和奇偶检验位，对于两个使用 UART 串口通信的端口，这些参数必须匹配，否则通信将无法正常完成。UART 串口传输的数据格式如下图所示：

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=NTdlZDBlNDY2NjZlN2E4YjExYTJmNGNjZTFmMmRjYzJfRnl6NmpZNVJSTzBwYlNZTDJWSGlSbkYwN0VDc0FqNkRfVG9rZW46Ym94Y25IbXk4OHFUeTBOdkdKQ1B4ekVCZmZmXzE2NTg1Mzk2NjE6MTY1ODU0MzI2MV9WNA)

![](https://i8cezjo4qg.feishu.cn/space/api/box/stream/download/asynccode/?code=NTc5ZWMxMzMyMThiM2QwYjNiMmJlZmIzMzRlYWMyNzhfRVpIWmdxRzJwN1l2RWJ6cWNaNzdSOThQSDZDNWlna01fVG9rZW46Ym94Y25OaFhEaE1DQ1NMVDlQNGhLbFB2ZEpoXzE2NTg1Mzk2NjE6MTY1ODU0MzI2MV9WNA)

### [查找串口设备](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/uart/uart_v1/uart?id=%e6%9f%a5%e6%89%be%e4%b8%b2%e5%8f%a3%e8%ae%be%e5%a4%87)

应用程序根据串口设备名称获取设备句柄，进而可以操作串口设备，查找设备函数如下所示，

```Plaintext
rt_device_t rt_device_find(const char* name);
```

```Plaintext
#define SAMPLE_UART_NAME       "uart2"    /* 串口设备名称 */
static rt_device_t serial;                /* 串口设备句柄 */
/* 查找串口设备 */
serial = rt_device_find(SAMPLE_UART_NAME);
```

### [打开串口设备](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/uart/uart_v1/uart?id=%e6%89%93%e5%bc%80%e4%b8%b2%e5%8f%a3%e8%ae%be%e5%a4%87)

通过设备句柄，应用程序可以打开和关闭设备，打开设备时，会检测设备是否已经初始化，没有初始化则会默认调用初始化接口初始化设备。通过如下函数打开设备:

```Plaintext
rt_err_t rt_device_open(rt_device_t dev, rt_uint16_t oflags);
```

```Plaintext
#define RT_DEVICE_FLAG_STREAM       0x040     /* 流模式      */
/* 接收模式参数 */
#define RT_DEVICE_FLAG_INT_RX       0x100     /* 中断接收模式 */
#define RT_DEVICE_FLAG_DMA_RX       0x200     /* DMA 接收模式 */
/* 发送模式参数 */
#define RT_DEVICE_FLAG_INT_TX       0x400     /* 中断发送模式 */
#define RT_DEVICE_FLAG_DMA_TX       0x800     /* DMA 发送模式 */
```

串口数据接收和发送数据的模式分为 3 种：中**断模式、轮询模式、DMA 模式**。在使用的时候，这 3 种模式只能选其一，若串口的打开参数 oflags 没有指定使用中断模式或者 DMA 模式，则默认使用轮询模式。

DMA（Direct Memory Access）即直接存储器访问。 DMA 传输方式无需 CPU 直接控制传输，也没有中断处理方式那样保留现场和恢复现场的过程，通过 DMA 控制器为 RAM 与 I/O 设备开辟一条直接传送数据的通路，这就**节省了 CPU 的资源来做其他操作**。**使用 DMA 传输可以连续获取或发送一段信息而不占用中断或延时，在通信频繁或有大段信息要传输时非常有用。**

以中断接收及轮询发送模式使用串口设备的示例如下所示：

```Plaintext
#define SAMPLE_UART_NAME       "uart2"    /* 串口设备名称 */
static rt_device_t serial;                /* 串口设备句柄 */
/* 查找串口设备 */
serial = rt_device_find(SAMPLE_UART_NAME);/* 以中断接收及轮询发送模式打开串口设备 */
rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);
```

若串口要使用 DMA 接收模式，oflags 取值 RT_DEVICE_FLAG_DMA_RX。以DMA 接收及轮询发送模式使用串口设备的示例如下所示：

```Plaintext
#define SAMPLE_UART_NAME       "uart2"  /* 串口设备名称 */
static rt_device_t serial;              /* 串口设备句柄 */
/* 查找串口设备 */
serial = rt_device_find(SAMPLE_UART_NAME);/* 以 DMA 接收及轮询发送模式打开串口设备 */
rt_device_open(serial, RT_DEVICE_FLAG_DMA_RX);
```

### [控制串口设备](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/uart/uart_v1/uart?id=%e6%8e%a7%e5%88%b6%e4%b8%b2%e5%8f%a3%e8%ae%be%e5%a4%87)

通过控制接口，应用程序可以对串口设备进行配置，如波特率、数据位、校验位、接收缓冲区大小、停止位等参数的修改。控制函数如下所示：

```Plaintext
rt_err_t rt_device_control(rt_device_t dev, rt_uint8_t cmd, void* arg);
```

```Plaintext
struct serial_configure
{
    rt_uint32_t baud_rate;            /* 波特率 */
    rt_uint32_t data_bits    :4;      /* 数据位 */
    rt_uint32_t stop_bits    :2;      /* 停止位 */
    rt_uint32_t parity       :2;      /* 奇偶校验位 */
    rt_uint32_t bit_order    :1;      /* 高位在前或者低位在前 */
    rt_uint32_t invert       :1;      /* 模式 */
    rt_uint32_t bufsz        :16;     /* 接收数据缓冲区大小 */
    rt_uint32_t reserved     :4;      /* 保留位 */
};
```

### [发送数据](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/uart/uart_v1/uart?id=%e5%8f%91%e9%80%81%e6%95%b0%e6%8d%ae)

向串口中写入数据，可以通过如下函数完成：

```Plaintext
rt_size_t rt_device_write(rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size);
```

```Plaintext
#define SAMPLE_UART_NAME       "uart2"    /* 串口设备名称 */
static rt_device_t serial;                /* 串口设备句柄 */
char str[] = "hello RT-Thread!\r\n";
struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT; /* 配置参数 */
/* 查找串口设备 */
serial = rt_device_find(SAMPLE_UART_NAME);/* 以中断接收及轮询发送模式打开串口设备 */
rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);
/* 发送字符串 */
rt_device_write(serial, 0, str, (sizeof(str) - 1));
```

### [设置发送完成回调函数](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/uart/uart_v1/uart?id=%e8%ae%be%e7%bd%ae%e5%8f%91%e9%80%81%e5%ae%8c%e6%88%90%e5%9b%9e%e8%b0%83%e5%87%bd%e6%95%b0)

在应用程序调用 `rt_device_write()` 写入数据时，如果底层硬件能够支持自动发送，那么上层应用可以设置一个回调函数。这个回调函数会在底层硬件数据发送完成后 (例如 DMA 传送完成或 FIFO 已经写入完毕产生完成中断时) 调用。可以通过如下函数设置设备发送完成指示 ：

```Plaintext
rt_err_t rt_device_set_tx_complete(rt_device_t dev, rt_err_t (*tx_done)(rt_device_t dev,void *buffer));
```

调用这个函数时，回调函数由调用者提供，当硬件设备发送完数据时，由设备驱动程序回调这个函数并把发送完成的数据块地址 buffer 作为参数传递给上层应用。上层应用（线程）在收到指示时会根据发送 buffer 的情况，释放 buffer 内存块或将其作为下一个写数据的缓存。

### [设置接收回调函数](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/uart/uart_v1/uart?id=%e8%ae%be%e7%bd%ae%e6%8e%a5%e6%94%b6%e5%9b%9e%e8%b0%83%e5%87%bd%e6%95%b0)

可以通过如下函数来设置数据接收指示，当串口收到数据时，通知上层应用线程有数据到达 ：

```Plaintext
rt_err_t rt_device_set_rx_indicate(rt_device_t dev, rt_err_t (*rx_ind)(rt_device_t dev,rt_size_t size));
```

### [接收数据](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/uart/uart_v1/uart?id=%e6%8e%a5%e6%94%b6%e6%95%b0%e6%8d%ae)

可调用如下函数读取串口接收到的数据：

```Plaintext
rt_size_t rt_device_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size);
```

### [关闭串口设备](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/uart/uart_v1/uart?id=%e5%85%b3%e9%97%ad%e4%b8%b2%e5%8f%a3%e8%ae%be%e5%a4%87)

当应用程序完成串口操作后，可以关闭串口设备，通过如下函数完成:

```Plaintext
rt_err_t rt_device_close(rt_device_t dev);
```