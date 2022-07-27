# Serial.c的分析

#### 串口注册

```c
rt_err_t rt_hw_serial_register(struct rt_serial_device *serial,
                               const char              *name,
                               rt_uint32_t              flag,
                               void                    *data)
```

1.

```C
device = &(serial->parent);
```

这句是找到rt_serial_device 类型的 serial 的parent对象,也就是device对象地址赋值给刚刚新建好的rt_device *device,以便进行后续的操作;

2.

```c
device->type        = RT_Device_Class_Char;
    device->rx_indicate = RT_NULL;
    device->tx_complete = RT_NULL;

#ifdef RT_USING_DEVICE_OPS
    device->ops         = &serial_ops;
#else
    device->init        = rt_serial_init;
    device->open        = rt_serial_open;
    device->close       = rt_serial_close;
    device->read        = rt_serial_read;
    device->write       = rt_serial_write;
    device->control     = rt_serial_control;
#endif
    device->user_data   = data;
```

这些是绑定自己实现的函数到设备对象中,之后调用rt_device_open等指令时就会调用相应的自己对接的api

3.

```C
ret = rt_device_register(device, name, flag);
```

这句是把这个设备对象注册在设备管理器中,完成整个串口的注册.

-----

##### rt_err_t rt_serial_init(struct rt_device *dev)

1.把传入的参数 dev强制转化为rt_serial_device* *类型,赋值给rt_serial_device* *类型的serial

2.初始化serial的serial_rx/tx成员,然后调用serial->ops>configure初始化参数配置

3.返回配置结果

---

##### rt_err_t rt_serial_open(struct rt_device *dev, rt_uint16_t oflag)

1.首先把设备对象强制转换为串口设备对象

2.检查设备的flag和open flag,如果有问题就返回-RT_EIO

3.得到流模式flag,open flag

4.根据openflag来初始化,设置对应的fifo的buffer 和index,修改full标志,修改device的openflag

5.调用control() 设置相应的模式.

----



