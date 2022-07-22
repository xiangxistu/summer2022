## day4 task_submission

---

#### Task1

#### WATCHDOG 设备

硬件看门狗（watchdog timer）是一个定时器，其定时输出连接到电路的复位端。在产品化的嵌入式系统中，为了使系统在异常情况下能自动复位，一般都需要引入看门狗。

当看门狗启动后，计数器开始自动计数，在计数器溢出前如果没有被复位，计数器溢出就会对 CPU 产生一个复位信号使系统重启（俗称 “被狗咬”）。系统正常运行时，需要在看门狗允许的时间间隔内对看门狗计数器清零（俗称“喂狗“），不让复位信号产生。如果系统不出问题，程序能够按时“喂狗”。一旦程序跑飞，没有“喂狗”，系统“被咬” 复位。

一般情况下可以在 RT-Thread 的 idle 回调函数和关键任务里喂狗。

#### 1、访问看门狗设备

应用程序通过 RT-Thread 提供的 I/O 设备管理接口来访问看门狗硬件，相关接口如下所示：

| **函数**            | **描述**                                   |
| ------------------- | ------------------------------------------ |
| rt_device_find()    | 根据看门狗设备设备名称查找设备获取设备句柄 |
| rt_device_init()    | 初始化看门狗设备                           |
| rt_device_control() | 控制看门狗设备                             |
| rt_device_close()   | 关闭看门狗设备                             |

#### 2、查找看门狗

rt_device_t rt_device_find(const char* name);

#### 3、初始化看门狗

rt_err_t rt_device_init(rt_device_t dev);

#### 4、控制看门狗

rt_err_t rt_device_control(rt_device_t dev, rt_uint8_t cmd, void* arg);

控制命令取下宏定义：

```c
#define RT_DEVICE_CTRL_WDT_GET_TIMEOUT    (1) /* 获取溢出时间 */
#define RT_DEVICE_CTRL_WDT_SET_TIMEOUT    (2) /* 设置溢出时间 */
#define RT_DEVICE_CTRL_WDT_GET_TIMELEFT   (3) /* 获取剩余时间 */
#define RT_DEVICE_CTRL_WDT_KEEPALIVE      (4) /* 喂狗 */
#define RT_DEVICE_CTRL_WDT_START          (5) /* 启动看门狗 */
#define RT_DEVICE_CTRL_WDT_STOP           (6) /* 停止看门狗 */
```

#### 5、关闭看门狗

rt_err_t rt_device_close(rt_device_t dev);

#### 6、测试

```c
rt_device_t wdg_dev = RT_NULL;
rt_err_t result = RT_EOK;

static void wdt_entry(void *parameter)
{
    result = rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_KEEPALIVE,RT_NULL); // 看门狗
    if(result != RT_EOK)
    {
        rt_kprintf("\n\0start %s failed...\n",wdg_dev);
    }

    rt_kprintf("wdg_dev is feed the dog...\n");
    rt_kprintf("/*********************************************/");
    rt_thread_mdelay(500);
}

static void wdt_sample(void *parameter)
{
    rt_thread_t wdt_thread = RT_NULL;

    rt_kprintf("/*********************************************/\n");


    wdt_thread = rt_thread_create("wdt_test", wdt_entry, RT_NULL, 1024, 20, 500);
    if(wdt_thread != RT_NULL)
    {
        rt_kprintf("wdt_thread create successed...\n");
    }
    else
    {
        rt_kprintf("wdt_thread create failed...\n");
    }

    wdg_dev = rt_device_find("wdt"); // 查找名为wdg_dev的看门狗设备
    if(! wdg_dev)
    {
        rt_kprintf("rt_device_find is failed...\n");
    }
    else
    {
        rt_kprintf("rt_device_find is ok...\n");
        rt_thread_mdelay(500);
    }

    result = rt_device_init(wdg_dev); // 初始化看门狗设备
    if(result != RT_EOK )
    {
        rt_kprintf("rt_device_init is failed...\n");
    }
    else
    {
        rt_kprintf("rt_device_init is ok...\n");
        rt_thread_mdelay(500);
    }

    result = rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_GET_TIMEOUT, RT_NULL); // 获取溢出时间
    if( result != RT_EOK)
    {
        rt_kprintf("RT_DEVICE_CTRL_WDT_GET_TIMEOUT is failed...\n");
    }
    else
    {
        rt_kprintf("RT_DEVICE_CTRL_WDT_GET_TIMEOUT is ok...\n");
        rt_thread_mdelay(500);
    }


    while(1)
    {
        int i = 0;
        for( i ; i < 5; i++)
        {
            rt_kprintf("LED%d is running...\n",i+1);
            rt_pin_write(LED3_PIN, PIN_HIGH);
            rt_thread_mdelay(500);
            rt_pin_write(LED3_PIN, PIN_LOW);
            rt_thread_mdelay(500);
        }

        result = rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_START, RT_NULL); // 启动看门狗
        if (result != RT_EOK)
        {
            rt_kprintf("RT_DEVICE_CTRL_WDT_START is failed!\n");
        }
        rt_kprintf("RT_DEVICE_CTRL_WDT_START is ok...\n");

        rt_thread_startup(wdt_thread);
        rt_kprintf("wdt_thread is startup...\n");

        break;
    }
}
MSH_CMD_EXPORT(wdt_sample,wdt_sample);
```

这里首先动态创建了一个线程用于喂狗，然后在线程函数中完成看门狗的初始化及启动工作，然后在该线程中设置一个五次一循环的LED点亮的死循环，在启动喂狗后退出该循环。

下面查看串口打印情况：

![image-20220723084648604](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207230846874.png)

亮点设计：其中由于tshell线程及创建的线程发生对资源的抢占，导致在串口打印时多打印了一串命令行，因此在线程的回调函数中加入了一个延时函数，用于对tshell线程的挂起。



## Task2

#### RTC 使用

#### 1、RTC简介

RTC （Real-Time Clock）实时时钟可以提供精确的实时时间，它可以用于产生年、月、日、时、分、秒等信息。目前实时时钟芯片大多采用精度较高的晶体振荡器作为时钟源。有些时钟芯片为了在主电源掉电时还可以工作，会外加电池供电，使时间信息一直保持有效。

RT-Thread 的 RTC设备为操作系统的时间系统提供了基础服务。面对越来越多的 IoT 场景，RTC 已经成为产品的标配，甚至在诸如 SSL 的安全传输过程中，RTC 已经成为不可或缺的部分。

#### 2、访问RTC设备

应用程序通过 RTC 设备管理接口来访问 RTC 硬件，相关接口如下所示：

| **函数**   | **描述**                         |
| ---------- | -------------------------------- |
| set_date() | 设置日期，年、月、日（当地时区） |
| set_time() | 设置时间，时、分、秒（当地时区） |
| time()     | 获取时间戳（格林威治时间）       |
| stime()    | 设置时间戳（格林威治时间）       |

#### 3、设置日期

rt_err_t set_date(rt_uint32_t year, rt_uint32_t month, rt_uint32_t day)

#### 4、设置时间

rt_err_t set_time(rt_uint32_t hour, rt_uint32_t minute, rt_uint32_t second)

#### 5、获取当前时间

time_t time(time_t *t)

`注意：在目前系统中只允许存在一个RTC设备，且名称为“rtc”`

#### 6、实例演示（这里使用官方例程）

![image-20220723092959167](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207230929619.png)
