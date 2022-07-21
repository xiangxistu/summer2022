# DAY3 总结-刘道鑫

## 7.21 上午

### 上午主要内容：

#### 		1.纠正昨天问题；

#### 		2.对象的讲解：

​			RT-Thread 采用内核对象管理系统来访问 / 管理所有内核对象，内核对象包含了内核中绝大部分设施，这些内核对象可以是静态分配的静态对象，也可以是从系统内存堆中分配的动态对象。

​			RT-Thread 内核对象包括：线程，信号量，互斥量，事件，邮箱，消息队列和定时器，内存池，设备驱动等。对象容器中包含了每类内核对象的信息，包括对象类型，大小等。对象容器给每类内核对象分配了一个链表，所有的内核对象都被链接到该链表上，RT-Thread 的内核对象容器及链表如下图所示：

![](./pictures/03kernel_object.png)

内核对象控制块的数据结构：

struct rt_object
{
     /* 内核对象名称     */
     char      name[RT_NAME_MAX];
     /* 内核对象类型     */
     rt_uint8_t  type;
     /* 内核对象的参数   */
     rt_uint8_t  flag;
     /* 内核对象管理链表 */
     rt_list_t   list;
};

管理方式：

内核对象容器的数据结构：

struct rt_object_information
{
     /* 对象类型 */
     enum rt_object_class_type type;
     /* 对象链表 */
     rt_list_t object_list;
     /* 对象大小 */
     rt_size_t object_size;
};

一类对象由一个 rt_object_information 结构体来管理，每一个这类对象的具体实例都通过链表的形式挂接在 object_list 上。而这一类对象的内存块尺寸由 object_size 标识出来（每一类对象的具体实例，他们占有的内存块大小都是相同的）。

#### 		3.IO设备

I/O 设备模型框架

RT-Thread 提供了一套简单的 I/O 设备模型框架，如下图所示，它位于硬件和应用程序之间，共分成三层，从上到下分别是 I/O 设备管理层、设备驱动框架层、设备驱动层。

![](./pictures/io-dev.png)

应用程序通过 I/O 设备管理接口获得正确的设备驱动，然后通过这个设备驱动与底层 I/O 硬件设备进行数据（或控制）交互。

I/O 设备管理层实现了对设备驱动程序的封装。应用程序通过 I/O 设备层提供的标准接口访问底层设备，设备驱动程序的升级、更替不会对上层应用产生影响。这种方式使得设备的硬件操作相关的代码能够独立于应用程序而存在，双方只需关注各自的功能实现，从而降低了代码的耦合性、复杂性，提高了系统的可靠性。

相关API：

![](./pictures/API.jpg)

#### 4.GPIO点灯

int led(void)
{
    rt_uint8_t count;

    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
    rt_kprintf("led_SP:%08x\r\n",__get_SP());
    for(count = 0 ; count < 10 ;count++)
    {
        rt_pin_write(LED0_PIN, PIN_LOW);
        rt_kprintf("led on, count : %d\r\n", count);
        rt_thread_mdelay(500);
    
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_kprintf("led off\r\n");
        rt_thread_mdelay(500);
    }
    return 0;
}

MSH_CMD_EXPORT(led,  led sample by using I/O drivers);

**点灯成功！**

## 7.21下午

### 一. 讲座

### 二.I2C总线简介

I2C（Inter Integrated Circuit）总线是 PHILIPS 公司开发的一种半双工、双向二线制同步串行总线。I2C 总线传输数据时只需两根信号线，一根是双向数据线 SDA（serial data），另一根是双向时钟线 SCL（serial clock）。SPI 总线有两根线分别用于主从设备之间接收数据和发送数据，而 I2C 总线只使用一根线进行数据收发。

### 三.SENSOR设备

Sensor（传感器）是物联网重要的一部分，“Sensor 之于物联网”就相当于“眼睛之于人类”。人类如果没有了眼睛就看不到这大千的花花世界，对于物联网来说也是一样。

如今随着物联网的发展，已经有大量的 Sensor 被开发出来供开发者选择了，如：加速度计(Accelerometer)、磁力计(Magnetometer)、陀螺仪(Gyroscope)、气压计(Barometer/pressure)、湿度计(Humidometer)等。这些传感器，世界上的各大半导体厂商都有生产，虽然增加了市场的可选择性，同时也加大了应用程序开发的难度。因为不同的传感器厂商、不同的传感器都需要配套自己独有的驱动才能运转起来，这样在开发应用程序的时候就需要针对不同的传感器做适配，自然加大了开发难度。为了降低应用开发的难度，增加传感器驱动的可复用性，我们设计了 Sensor 设备。

Sensor 设备的作用是：为上层提供统一的操作接口，提高上层代码的可重用性。

### 四.实验——AHT10传感器的使用

![](./pictures/jieguo.jpg)

