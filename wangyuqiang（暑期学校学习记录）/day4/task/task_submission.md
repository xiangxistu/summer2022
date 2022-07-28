## day3  Task_Submission

---

#### 任务：

1、学习RT-Thread IO设备

2、注册一个i2c设备，并使用ATHT10温湿度传感器读取温湿度数值

---

#### Task1：

#### 1、IO设备介绍

> 在RT-Thread中，IO设备框架设计主要分为IO设备管理层，设备驱动框架层以及设备驱动层，根据官网文档解说，该框架位于硬件和应用程序中。
>
> IO设备管理层实现对设备驱动程序的封装；设备驱动框架层是对同类硬件设备驱动的抽象；设备驱动层是一组驱使硬件设备工作的程序，实现访问硬件设备的功能。

![I/O 设备模型框架](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207212042974.png)

![I/O 设备模型框架补充图](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207212127078.png)

![简单 I/O 设备使用序列图](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207212054032.png)

这里补充一点知识点：耦合度&如何降低耦合度

```
耦合度就是某模块（类）与其它模块（类）之间的关联、感知和依赖的程度，是衡量代码独立性的一个指标，也是软件工程设计及编码质量评价的一个标准。

耦合的强度依赖于以下几个因素：
（1）一个模块对另一个模块的调用；
（2）一个模块向另一个模块传递的数据量；
（3）一个模块施加到另一个模块的控制的多少；
（4）模块之间接口的复杂程度。

降低耦合度的方法:
（1）少使用类的继承，多用接口隐藏实现的细节。 java面向对象编程引入接口除了支持多态外， 隐藏实现细节也是其中一个目的。
（2）模块的功能化分尽可能的单一，道理也很简单，功能单一的模块供其它模块调用的机会就少。（其实这是高内聚的一种说法，高内聚低耦合一般同时出现，为了限制篇幅，我们将在以后的版期中讨论）。
（3）遵循一个定义只在一个地方出现。
（4）少使用全局变量。
（5）类属性和方法的声明少用public，多用private关键字，
（6）多用设计模式，比如采用MVC的设计模式就可以降低界面与业务逻辑的耦合度。
（7）尽量不用“硬编码”的方式写程序，同时也尽量避免直接用SQL语句操作数据库。
（8）最后当然就是避免直接操作或调用其它模块或类（内容耦合）；如果模块间必须存在耦合，原则上尽量使用数据耦合，少用控制耦合，限制公共耦合的范围，避免使用内容耦合。
```

在RT-Thread的模型框架中，应用程序通过 I/O 设备层提供的标准接口访问底层设备，设备驱动程序的升级、更替不会对上层应用产生影响。这种方式使得设备的硬件操作相关的代码能够独立于应用程序而存在，双方只需关注各自的功能实现，从而降低了代码的耦合性、复杂性，提高了系统的可靠性。

#### 2、IO设备模型

RT-Thread 的设备模型是建立在内核对象模型基础之上的，设备被认为是一类对象，被纳入对象管理器的范畴。每个设备对象都是由基对象派生而来，每个具体设备都可以继承其父类对象的属性，并派生出其私有属性，下图是设备对象的继承和派生关系示意图。

![设备继承关系图](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207212113149.png)

#### 3、IO设备种类

```c
RT_Device_Class_Char             /* 字符设备       */
RT_Device_Class_Block            /* 块设备         */
RT_Device_Class_NetIf            /* 网络接口设备    */
RT_Device_Class_MTD              /* 内存设备       */
RT_Device_Class_RTC              /* RTC 设备        */
RT_Device_Class_Sound            /* 声音设备        */
RT_Device_Class_Graphic          /* 图形设备        */
RT_Device_Class_I2CBUS           /* I2C 总线设备     */
RT_Device_Class_USBDevice        /* USB device 设备  */
RT_Device_Class_USBHost          /* USB host 设备   */
RT_Device_Class_SPIBUS           /* SPI 总线设备     */
RT_Device_Class_SPIDevice        /* SPI 设备        */
RT_Device_Class_SDIO             /* SDIO 设备       */
RT_Device_Class_Miscellaneous    /* 杂类设备        */
```

#### 4、RT-Thread IO设备API接口

<1>动态创建：`rt_device_t rt_device_create(int type, int attach_size);`

调用该接口时，系统会从动态堆内存中分配一个设备控制块，大小为 struct rt_device 和 attach_size 的和，设备的类型由参数 type 设定。设备被创建后，需要实现它访问硬件的操作方法。

```c
struct rt_device_ops
{
    /* common device interface */
    rt_err_t  (*init)   (rt_device_t dev);
    rt_err_t  (*open)   (rt_device_t dev, rt_uint16_t oflag);
    rt_err_t  (*close)  (rt_device_t dev);
    rt_size_t (*read)   (rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size);
    rt_size_t (*write)  (rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size);
    rt_err_t  (*control)(rt_device_t dev, int cmd, void *args);
};
```

![image-20220721211710759](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207212117834.png)

<2>动态创建的设备的销毁：`void rt_device_destroy(rt_device_t device);`

<3>注册设备函数:`rt_err_t rt_device_register(rt_device_t dev, const char* name, rt_uint8_t flags);`

其中的flag标志参考如下：

```c
#define RT_DEVICE_FLAG_RDONLY       0x001 /* 只读 */
#define RT_DEVICE_FLAG_WRONLY       0x002 /* 只写  */
#define RT_DEVICE_FLAG_RDWR         0x003 /* 读写  */
#define RT_DEVICE_FLAG_REMOVABLE    0x004 /* 可移除  */
#define RT_DEVICE_FLAG_STANDALONE   0x008 /* 独立   */
#define RT_DEVICE_FLAG_SUSPENDED    0x020 /* 挂起  */
#define RT_DEVICE_FLAG_STREAM       0x040 /* 流模式  */
#define RT_DEVICE_FLAG_INT_RX       0x100 /* 中断接收 */
#define RT_DEVICE_FLAG_DMA_RX       0x200 /* DMA 接收 */
#define RT_DEVICE_FLAG_INT_TX       0x400 /* 中断发送 */
#define RT_DEVICE_FLAG_DMA_TX       0x800 /* DMA 发送 */
```

<4>注销设备：`rt_err_t rt_device_unregister(rt_device_t dev);`

当设备注销后的，设备将从设备管理器中移除，也就不能再通过设备查找搜索到该设备。注销设备不会释放设备控制块占用的内存。

<5>查找设备：`rt_device_t rt_device_find(const char* name);`

<6>初始化设备：`rt_err_t rt_device_init(rt_device_t dev);`

<7>打开设备：`rt_err_t rt_device_open(rt_device_t dev, rt_uint16_t oflags);`

其中oflag支持如下

```c
#define RT_DEVICE_OFLAG_CLOSE 0x000   /* 设备已经关闭（内部使用）*/
#define RT_DEVICE_OFLAG_RDONLY 0x001  /* 以只读方式打开设备 */
#define RT_DEVICE_OFLAG_WRONLY 0x002  /* 以只写方式打开设备 */
#define RT_DEVICE_OFLAG_RDWR 0x003    /* 以读写方式打开设备 */
#define RT_DEVICE_OFLAG_OPEN 0x008    /* 设备已经打开（内部使用）*/
#define RT_DEVICE_FLAG_STREAM 0x040   /* 设备以流模式打开 */
#define RT_DEVICE_FLAG_INT_RX 0x100   /* 设备以中断接收模式打开 */
#define RT_DEVICE_FLAG_DMA_RX 0x200   /* 设备以 DMA 接收模式打开 */
#define RT_DEVICE_FLAG_INT_TX 0x400   /* 设备以中断发送模式打开 */
#define RT_DEVICE_FLAG_DMA_TX 0x800   /* 设备以 DMA 发送模式打开 */
```

`注：如果上层应用程序需要设置设备的接收回调函数，则必须以 RT_DEVICE_FLAG_INT_RX 或者 RT_DEVICE_FLAG_DMA_RX 的方式打开设备，否则不会回调函数。`

<8>关闭设备：`rt_err_t rt_device_close(rt_device_t dev);`

<9>控制设备：`rt_err_t rt_device_control(rt_device_t dev, rt_uint8_t cmd, void* arg);`

cmd参数说明如下：

```c
#define RT_DEVICE_CTRL_RESUME           0x01   /* 恢复设备 */
#define RT_DEVICE_CTRL_SUSPEND          0x02   /* 挂起设备 */
#define RT_DEVICE_CTRL_CONFIG           0x03   /* 配置设备 */
#define RT_DEVICE_CTRL_SET_INT          0x10   /* 设置中断 */
#define RT_DEVICE_CTRL_CLR_INT          0x11   /* 清中断 */
#define RT_DEVICE_CTRL_GET_INT          0x12   /* 获取中断状态 */
```

<10>读写设备：`rt_size_t rt_device_read(rt_device_t dev, rt_off_t pos,void* buffer, rt_size_t size);`

调用这个函数，会从 dev 设备中读取数据，并存放在 buffer 缓冲区中，这个缓冲区的最大长度是 size，pos 根据不同的设备类别有不同的意义。

<11>向设备中写入数据：`rt_size_t rt_device_write(rt_device_t dev, rt_off_t pos,const void* buffer, rt_size_t size);`

调用这个函数，会把缓冲区 buffer 中的数据写入到设备 dev 中，写入数据的最大长度是 size，pos 根据不同的设备类别存在不同的意义。

#### 5、访问IO设备

应用程序通过 I/O 设备管理接口来访问硬件设备，当设备驱动实现后，应用程序就可以访问该硬件。

![I/O 设备管理接口与 I/O 设备的操作方法的映射关系](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207212121965.png)

---

#### Task2:注册一个i2c设备，并使用ATHT10温湿度传感器读取温湿度数值

使能i2c，并修改对应引脚数，根据提示要转换成十六进制

![image-20220721213950287](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207212139510.png)

软件包添加

![image-20220721214059035](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207212140116.png)

AHT10初始化函数：

```c
#include "sensor_asair_aht10.h"
#define AHT10_I2C_BUS  "i2c1"

int rt_hw_aht10_port(void)
{
    struct rt_sensor_config cfg;

    cfg.intf.dev_name  = AHT10_I2C_BUS;
    cfg.intf.user_data = (void *)AHT10_I2C_ADDR;

    rt_hw_aht10_init("aht10", &cfg);

    return RT_EOK;
}
INIT_ENV_EXPORT(rt_hw_aht10_port);
```

用户代码编写：

```c
extern struct aht10_device *temp_humi_dev;

/* 定义一个温湿度采集线程句柄结构体指针 */
static rt_thread_t aht10_thread = RT_NULL;

/* 温湿度采集线程入口函数*/
static void aht10_thread_entry(void *parameter)
{
    float humidity, temperature;

    int i = 0;
    for(i;i<5;i++)
    {
        rt_kprintf("*********************Testing number is %d**********************\n\n",i+1);
        /* read humidity 采集湿度 */
        humidity = aht10_read_humidity(temp_humi_dev);
        rt_kprintf("humidity   : %d.%d %%\n", (int)humidity, (int)(humidity * 10) % 10); /* former is integer and behind is decimal */

        /* read temperature 采集温度 */
        temperature = aht10_read_temperature(temp_humi_dev);
        rt_kprintf("temperature: %d.%d \n\n", (int)temperature, (int)(temperature * 10) % 10); /* former is integer and behind is decimal */

        rt_thread_mdelay(1000);
    }

}

static int app_aht10_init(void)
{
    rt_err_t rt_err;
    /* 创建温湿度采集线程*/
    aht10_thread = rt_thread_create("aht10 thread",aht10_thread_entry,RT_NULL,1024,25,10);
    /* 如果获得线程控制块，启动这个线程 */
    if (aht10_thread != RT_NULL)
        rt_err = rt_thread_startup(aht10_thread);
    else
        rt_kprintf("aht10 thread create failure !!! \n");

    /* 判断线程是否创建成功 */
    if( rt_err != RT_EOK)
        rt_kprintf("aht10 thread startup err. \n");

    return RT_EOK;
}

//INIT_APP_EXPORT(app_aht10_init);
MSH_CMD_EXPORT(app_aht10_init,app_aht10_init);
```

这里查看串口显示

![image-20220722092957169](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207220929396.png)
