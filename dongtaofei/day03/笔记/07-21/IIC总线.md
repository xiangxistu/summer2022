# IIC总线

I2C 总线传输数据时只需两根信号线：

☐ 双向数据线 SDA

☐ 双向时钟线 SCL

![image-20220721151249452](D:\RT-Thread_summer_holiday_happiness\笔记\07-21\image-20220721151249452.png)



## 1.代码

注册IIC总线设备

```c
rt_err_t  rt_i2c_bus_device_register (struct rt_i2c_bus_device *bus, 
                                      const char *bus_name) 
```

注册I2C 总线设备 

本函数用于向系统中注册I2C 总线设备。

- 参数

  busI2C 总线设备句柄 bus_nameI2C 总线设备名称，一般与硬件控制器名称一致，如：“i2c0”

- 返回

  RT_EOK 成功；-RT_ERROR 注册失败，已有其他驱动使用该bus_name注册。 

IIC数据传输

```c
rt_size_t  rt_i2c_transfer (struct rt_i2c_bus_device *bus,
                            struct rt_i2c_msg msgs[], 
                            rt_uint32_t num) 
```

I2C 数据传输 

该函数传递的消息是链式结构，可以通过消息链，实现调用一次完成多次数据的收发，主机调用此函数可以完成读或者写从设备的操作。

- 参数

  busI2C 总线设备句柄 msgs[]I2C 消息数组 numI2C 消息数组的数量

- 返回

  传输成功则返回成功传输的消息数组的个数；失败则返回其他错误码。 



## 2.查找GPIO号的两种方式

第一种：gpio.c

```C
#define PIN_NUM(port, no) (((((port) & 0xFu) << 4) | ((no) & 0xFu)))
```

A0->0

B0->16

第二种：

直接 在命令行发送  pin num PB6 类似这样

















