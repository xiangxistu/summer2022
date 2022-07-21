RT-Thread 文档中心：https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/device/i2c/i2c

## [RT-Thread 内核对象模型](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/basic/basic?id=rt-thread-内核对象模型)

### [静态对象和动态对象](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/basic/basic?id=静态对象和动态对象)

RT-Thread 内核采用面向对象的设计思想进行设计，系统级的基础设施都是一种内核对象，例如线程，信号量，互斥量，定时器等。内核对象分为两类：静态内核对象和动态内核对象，静态内核对象通常放在 RW 段和 ZI 段中，在系统启动后在程序中初始化；动态内核对象则是从内存堆中创建的，而后手工做初始化。

以下代码是一个关于静态线程和动态线程的例子：

```c
/* 线程 1 的对象和运行时用到的栈 */
static struct rt_thread thread1;
static rt_uint8_t thread1_stack[512];

/* 线程 1 入口 */
void thread1_entry(void* parameter)
{
     int i;

    while (1)
    {
        for (i = 0; i < 10; i ++)
        {
            rt_kprintf("%d\n", i);

            /* 延时 100ms */
            rt_thread_mdelay(100);
        }
    }
}

/* 线程 2 入口 */
void thread2_entry(void* parameter)
{
     int count = 0;
     while (1)
     {
         rt_kprintf("Thread2 count:%d\n", ++count);

        /* 延时 50ms */
        rt_thread_mdelay(50);
    }
}

/* 线程例程初始化 */
int thread_sample_init()
{
     rt_thread_t thread2_ptr;
     rt_err_t result;

    /* 初始化线程 1 */
    /* 线程的入口是 thread1_entry，参数是 RT_NULL
     * 线程栈是 thread1_stack
     * 优先级是 200，时间片是 10 个 OS Tick
     */
    result = rt_thread_init(&thread1,
                            "thread1",
                            thread1_entry, RT_NULL,
                            &thread1_stack[0], sizeof(thread1_stack),
                            200, 10);

    /* 启动线程 */
    if (result == RT_EOK) rt_thread_startup(&thread1);

    /* 创建线程 2 */
    /* 线程的入口是 thread2_entry, 参数是 RT_NULL
     * 栈空间是 512，优先级是 250，时间片是 25 个 OS Tick
     */
    thread2_ptr = rt_thread_create("thread2",
                                thread2_entry, RT_NULL,
                                512, 250, 25);

    /* 启动线程 */
    if (thread2_ptr != RT_NULL) rt_thread_startup(thread2_ptr);

    return 0;
}
```

在这个例子中，thread1 是一个静态线程对象，而 thread2 是一个动态线程对象。thread1 对象的内存空间，包括线程控制块 thread1 与栈空间 thread1_stack 都是编译时决定的，因为代码中都不存在初始值，都统一放在未初始化数据段中。thread2 运行中用到的空间都是动态分配的，包括线程控制块（thread2_ptr 指向的内容）和栈空间。

静态对象会占用 RAM 空间，不依赖于内存堆管理器，内存分配时间确定。动态对象则依赖于内存堆管理器，运行时申请 RAM 空间，当对象被删除后，占用的 RAM 空间被释放。这两种方式各有利弊，可以根据实际环境需求选择具体使用方式。

### [内核对象管理架构](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/basic/basic?id=内核对象管理架构)

RT-Thread 采用内核对象管理系统来访问 / 管理所有内核对象，内核对象包含了内核中绝大部分设施，这些内核对象可以是静态分配的静态对象，也可以是从系统内存堆中分配的动态对象。

通过这种内核对象的设计方式，RT-Thread 做到了不依赖于具体的内存分配方式，系统的灵活性得到极大的提高。

RT-Thread 内核对象包括：线程，信号量，互斥量，事件，邮箱，消息队列和定时器，内存池，设备驱动等。对象容器中包含了每类内核对象的信息，包括对象类型，大小等。对象容器给每类内核对象分配了一个链表，所有的内核对象都被链接到该链表上，RT-Thread 的内核对象容器及链表如下图所示：

![RT-Thread 的内核对象容器及链表](C:\Users\刘佩羽\Desktop\GIT教学\summer2022\liupeiyu\第3天\学习笔记\03kernel_object.png)

下图则显示了 RT-Thread 中各类内核对象的派生和继承关系。对于每一种具体内核对象和对象控制块，除了基本结构外，还有自己的扩展属性（私有属性），例如，对于线程控制块，在基类对象基础上进行扩展，增加了线程状态、优先级等属性。这些属性在基类对象的操作中不会用到，只有在与具体线程相关的操作中才会使用。因此从面向对象的观点，可以认为每一种具体对象是抽象对象的派生，继承了基本对象的属性并在此基础上扩展了与自己相关的属性。

![RT-Thread 内核对象继承关系](C:\Users\刘佩羽\Desktop\GIT教学\summer2022\liupeiyu\第3天\学习笔记\03kernel_object2.png)

在对象管理模块中，定义了通用的数据结构，用来保存各种对象的共同属性，各种具体对象只需要在此基础上加上自己的某些特别的属性，就可以清楚的表示自己的特征。

这种设计方法的优点有：

（1）提高了系统的可重用性和扩展性，增加新的对象类别很容易，只需要继承通用对象的属性再加少量扩展即可。

（2）提供统一的对象操作方式，简化了各种具体对象的操作，提高了系统的可靠性。

上图中由对象控制块 rt_object 派生出来的有：线程对象、内存池对象、定时器对象、设备对象和 IPC 对象（IPC：Inter-Process Communication，进程间通信。在 RT-Thread 实时操作系统中，IPC 对象的作用是进行线程间同步与通信）；由 IPC 对象派生出信号量、互斥量、事件、邮箱与消息队列、信号等对象。





上海先楫半导体有限公司概况：

* 2020年8月成立 ，亿股
* 产品：微控制器（MCU），微处理器和周边芯片，代表作HPM6750
* 团队：研发50+，80%为研发，90%为硕士学历以上
* ![image-20220721141243890](C:\Users\刘佩羽\AppData\Roaming\Typora\typora-user-images\image-20220721141243890.png)

* 微控制器(MCU):又称为单片机，将微型计算机的主要部分集成在一个芯片上的单芯片微型计算机
  * ![image-20220721142147453](C:\Users\刘佩羽\AppData\Roaming\Typora\typora-user-images\image-20220721142147453.png)
* RISC-V 一种开源指令集架构，其他的比如X86架构，ARM架构（可以了解一下）

I2C简介：

* I2C（Inter Integrated Circuit）总线是 PHILIPS 公司开发的一种半双工、双向二线制同步串行总线。I2C 总线传输数据时只需两根信号线，一根是双向数据线 SDA（serial data），另一根是双向时钟线 SCL（serial clock）。I2C（Inter Integrated Circuit）总线是 PHILIPS 公司开发的一种半双工、双向二线制同步串行总线。I2C 总线传输数据时只需两根信号线，一根是双向数据线 SDA（serial data），另一根是双向时钟线 SCL（serial clock）。

* ![image-20220721160513911](C:\Users\刘佩羽\AppData\Roaming\Typora\typora-user-images\image-20220721160513911.png)

* I2C设备使用示例：

  ```c
  /*
   * 程序清单：这是一个 I2C 设备使用例程
   * 例程导出了 i2c_aht10_sample 命令到控制终端
   * 命令调用格式：i2c_aht10_sample i2c1
   * 命令解释：命令第二个参数是要使用的I2C总线设备名称，为空则使用默认的I2C总线设备
   * 程序功能：通过 I2C 设备读取温湿度传感器 aht10 的温湿度数据并打印
  */
  
  #include <rtthread.h>
  #include <rtdevice.h>
  
  #define AHT10_I2C_BUS_NAME          "i2c1"  /* 传感器连接的I2C总线设备名称 */
  #define AHT10_ADDR                  0x38    /* 从机地址 */
  #define AHT10_CALIBRATION_CMD       0xE1    /* 校准命令 */
  #define AHT10_NORMAL_CMD            0xA8    /* 一般命令 */
  #define AHT10_GET_DATA              0xAC    /* 获取数据命令 */
  
  static struct rt_i2c_bus_device *i2c_bus = RT_NULL;     /* I2C总线设备句柄 */
  static rt_bool_t initialized = RT_FALSE;                /* 传感器初始化状态 */
  
  /* 写传感器寄存器 */
  static rt_err_t write_reg(struct rt_i2c_bus_device *bus, rt_uint8_t reg, rt_uint8_t *data)
  {
      rt_uint8_t buf[3];
      struct rt_i2c_msg msgs;
      rt_uint32_t buf_size = 1;
  
      buf[0] = reg; //cmd
      if (data != RT_NULL)
      {
          buf[1] = data[0];
          buf[2] = data[1];
          buf_size = 3;
      }
  
      msgs.addr = AHT10_ADDR;
      msgs.flags = RT_I2C_WR;
      msgs.buf = buf;
      msgs.len = buf_size;
  
      /* 调用I2C设备接口传输数据 */
      if (rt_i2c_transfer(bus, &msgs, 1) == 1)
      {
          return RT_EOK;
      }
      else
      {
          return -RT_ERROR;
      }
  }
  
  /* 读传感器寄存器数据 */
  static rt_err_t read_regs(struct rt_i2c_bus_device *bus, rt_uint8_t len, rt_uint8_t *buf)
  {
      struct rt_i2c_msg msgs;
  
      msgs.addr = AHT10_ADDR;
      msgs.flags = RT_I2C_RD;
      msgs.buf = buf;
      msgs.len = len;
  
      /* 调用I2C设备接口传输数据 */
      if (rt_i2c_transfer(bus, &msgs, 1) == 1)
      {
          return RT_EOK;
      }
      else
      {
          return -RT_ERROR;
      }
  }
  
  static void read_temp_humi(float *cur_temp, float *cur_humi)
  {
      rt_uint8_t temp[6];
  
      write_reg(i2c_bus, AHT10_GET_DATA, RT_NULL);      /* 发送命令 */
      rt_thread_mdelay(400);
      read_regs(i2c_bus, 6, temp);                /* 获取传感器数据 */
  
      /* 湿度数据转换 */
      *cur_humi = (temp[1] << 12 | temp[2] << 4 | (temp[3] & 0xf0) >> 4) * 100.0 / (1 << 20);
      /* 温度数据转换 */
      *cur_temp = ((temp[3] & 0xf) << 16 | temp[4] << 8 | temp[5]) * 200.0 / (1 << 20) - 50;
  }
  
  static void aht10_init(const char *name)
  {
      rt_uint8_t temp[2] = {0, 0};
  
      /* 查找I2C总线设备，获取I2C总线设备句柄 */
      i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(name);
  
      if (i2c_bus == RT_NULL)
      {
          rt_kprintf("can't find %s device!\n", name);
      }
      else
      {
          write_reg(i2c_bus, AHT10_NORMAL_CMD, temp);
          rt_thread_mdelay(400);
  
          temp[0] = 0x08;
          temp[1] = 0x00;
          write_reg(i2c_bus, AHT10_CALIBRATION_CMD, temp);
          rt_thread_mdelay(400);
          initialized = RT_TRUE;
      }
  }
  
  static void i2c_aht10_sample(int argc, char *argv[])
  {
      float humidity, temperature;
      char name[RT_NAME_MAX];
  
      humidity = 0.0;
      temperature = 0.0;
  
      if (argc == 2)
      {
          rt_strncpy(name, argv[1], RT_NAME_MAX);
      }
      else
      {
          rt_strncpy(name, AHT10_I2C_BUS_NAME, RT_NAME_MAX);
      }
  
      if (!initialized)
      {
          /* 传感器初始化 */
          aht10_init(name);
      }
      if (initialized)
      {
          /* 读取温湿度数据 */
          read_temp_humi(&temperature, &humidity);
  
          rt_kprintf("read aht10 sensor humidity   : %d.%d %%\n", (int)humidity, (int)(humidity * 10) % 10);
          if( temperature >= 0 )
          {
              rt_kprintf("read aht10 sensor temperature: %d.%d°C\n", (int)temperature, (int)(temperature * 10) % 10);
          }
          else
          {
              rt_kprintf("read aht10 sensor temperature: %d.%d°C\n", (int)temperature, (int)(-temperature * 10) % 10);
          }
      }
      else
      {
          rt_kprintf("initialize sensor failed!\n");
      }
  }
  /* 导出到 msh 命令列表中 */
  MSH_CMD_EXPORT(i2c_aht10_sample, i2c aht10 sample);
  
  ```




## 

## [常见宏定义说明](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/basic/basic?id=常见宏定义说明)

RT-Thread 中经常使用一些宏定义，举例 Keil 编译环境下一些常见的宏定义：

1）rt_inline，定义如下，static 关键字的作用是令函数只能在当前的文件中使用；inline 表示内联，用 static 修饰后在调用函数时会建议编译器进行内联展开。

```c
#define rt_inline                   static __inline复制错误复制成功
```

2）RT_USED，定义如下，该宏的作用是向编译器说明这段代码有用，即使函数中没有调用也要保留编译。例如 RT-Thread 自动初始化功能使用了自定义的段，使用 RT_USED 会将自定义的代码段保留。

```c
#define RT_USED                     __attribute__((used))复制错误复制成功
```

3）RT_UNUSED，定义如下，表示函数或变量可能不使用，这个属性可以避免编译器产生警告信息。

```c
#define RT_UNUSED                   __attribute__((unused))复制错误复制成功
```

4）RT_WEAK，定义如下，常用于定义函数，编译器在链接函数时会优先链接没有该关键字前缀的函数，如果找不到则再链接由 weak 修饰的函数。

```c
#define RT_WEAK                     __weak复制错误复制成功
```

5）ALIGN(n)，定义如下，作用是在给某对象分配地址空间时，将其存放的地址按照 n 字节对齐，这里 n 可取 2 的幂次方。字节对齐的作用不仅是便于 CPU 快速访问，同时合理的利用字节对齐可以有效地节省存储空间。

```c
#define ALIGN(n)                    __attribute__((aligned(n)))复制错误复制成功
```

6）RT_ALIGN(size,align)，定义如下，作用是将 size 提升为 align 定义的整数的倍数，例如，RT_ALIGN(13,4) 将返回 16。

```c
#define RT_ALIGN(size, align)      (((size) + (align) - 1) & ~((align) - 1))
```



## [RT-Thread 自动初始化机制](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/basic/basic?id=rt-thread-自动初始化机制)

自动初始化机制是指初始化函数不需要被显式调用，只需要在函数定义处通过宏定义的方式进行申明，就会在系统启动过程中被执行。