# RTT 夏令营第五天总结

## WDT 看门狗

硬件看门狗（watchdog timer）是一个**定时器**，其定时输出**连接到电路的复位端**。在产品化的嵌入式系统中，为了使系统在**异常情况下能自动复位**，一般都需要引入看门狗。

当看门狗启动后，计数器开始自动计数，在计数器溢出前如果没有被复位，计数器溢出就会对 CPU 产生一个复位信号使系统重启（俗称 “被狗咬”）。系统正常运行时，需要在看门狗允许的时间间隔内对看门狗计数器清零（俗称“喂狗“），不让复位信号产生。如果系统不出问题，程序能够按时“喂狗”。一旦程序跑飞，没有“喂狗”，系统“被咬” 复位。

### 相关 API

* 查找看门狗

  ```c
  /* 查找看门狗 */
  rt_device_t rt_device_find(const char* name);				//看门狗设备名称
  ```

  应用程序可根据该函数接口获取设备句柄，进而可以操作看门狗设备。

* 初始化看门狗

  ```c
  /* 初始化看门狗 */
  rt_err_t rt_device_init(rt_device_t dev);					//看门狗设备句柄
  ```

  使用看门狗设备前需要使用该函数接口进行看门狗设备的初始化。

* 控制看门狗

  ```c
  /* 控制看门狗 */
  rt_err_t rt_device_control(rt_device_t dev, 				//看门狗设备句柄
                             rt_uint8_t cmd, 					//命令控制字
                             void* arg);						//控制的参数
  ```

  通过命令控制字，应用程序可以通过该函数接口对看门狗设备进行配置。

  命令控制字可取如下宏定义值：

  ```c
  #define RT_DEVICE_CTRL_WDT_GET_TIMEOUT    (1) /* 获取溢出时间 */
  #define RT_DEVICE_CTRL_WDT_SET_TIMEOUT    (2) /* 设置溢出时间 */
  #define RT_DEVICE_CTRL_WDT_GET_TIMELEFT   (3) /* 获取剩余时间 */
  #define RT_DEVICE_CTRL_WDT_KEEPALIVE      (4) /* 喂狗 */
  #define RT_DEVICE_CTRL_WDT_START          (5) /* 启动看门狗 */
  #define RT_DEVICE_CTRL_WDT_STOP           (6) /* 停止看门狗 */
  ```

* 关闭看门狗

  ```c
  /* 关闭看门狗 */
  rt_err_t rt_device_close(rt_device_t dev);					//看门狗设备句柄
  ```

  当应用程序完成看门狗操作后，可以关闭看门狗设备。

  值得注意的是：关闭设备接口和打开设备接口需配对使用，打开一次设备对应要关闭一次设备，这样设备才会被完全关闭，否则设备仍处于未关闭状态。

### 代码实现：

```c
#define WDT_DEVICE_NAME    "wdt"    /* 看门狗设备名称 */
static rt_device_t wdg_dev;         /* 看门狗设备句柄 */

void LED_Toggle(void *args)
{
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
    if (rt_pin_read(LED0_PIN) == 0)
    {
        rt_pin_write(LED0_PIN, 1);
        rt_kprintf("LED OFF\r\n");
    }
    else
    {
        rt_pin_write(LED0_PIN, 0);
        rt_kprintf("LED ON\r\n");
    }
}

MSH_CMD_EXPORT(LED_Toggle,  use to toggle led_pin);

void LED_Blink(void *args)
{
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
    while(1)
    {
        rt_pin_write(LED0_PIN, 0);
        rt_thread_mdelay(500);
        rt_pin_write(LED0_PIN, 1);
        rt_thread_mdelay(500);
    }
}

void Key_scan(void *args)
{
    rt_err_t ret = RT_EOK;
    rt_uint32_t timeout = 1;        /* 溢出时间，单位：秒 */

    rt_pin_mode(13,PIN_MODE_INPUT_PULLUP);

    wdg_dev = rt_device_find(WDT_DEVICE_NAME);
    if (!wdg_dev)
    {
            rt_kprintf("find %s failed!\n", WDT_DEVICE_NAME);
            return RT_ERROR;
     }
    /* 设置看门狗溢出时间 */
    ret = rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_SET_TIMEOUT, &timeout);
    if (ret != RT_EOK)
    {
        rt_kprintf("set %s timeout failed!\n", WDT_DEVICE_NAME);
        return RT_ERROR;
    }
    /* 启动看门狗 */
    ret = rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_START, RT_NULL);
    if (ret != RT_EOK)
    {
        rt_kprintf("start %s failed!\n", WDT_DEVICE_NAME);
        return -RT_ERROR;
    }

    while(1)
    {
        if(rt_pin_read(13) == 0)
        {
            rt_thread_mdelay(10);
            if(rt_pin_read(13) == 0)
            {
                rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_KEEPALIVE, NULL);//喂狗
                rt_kprintf("feed the dog!\n ");
            }
        }
        rt_thread_mdelay(10);
    }
}

void test_thread_init(void)
{
    rt_thread_t err;
    err = rt_thread_create("blink", LED_Blink, RT_NULL, 1024, 16, 100);
    if(err != RT_NULL)
    {
        rt_thread_startup(err);
    }
    else
    {
        rt_kprintf("blink thread create fault\r\n");
    }

    rt_thread_t tid;
    tid = rt_thread_create("key", Key_scan, RT_NULL, 1024, 15, 100);
    if(tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }
    else
    {
        rt_kprintf("key thread create fault\r\n");
    }
}
MSH_CMD_EXPORT(test_thread_init,test thread init);
```

程序思路及效果：

```mermaid
graph LR
A[按下按键]
B[LED闪烁线程运行]
C[系统重启]
D[超时未按下按键]
E[将按键设置为'喂狗'入口]
E --> A --> B
E --> D --> C
```

![WDT](.\assets\image-20220723141521310.png)

## RTC 使用

RTC （Real-Time Clock）实时时钟可以提供精确的实时时间，它可以用于产生年、月、日、时、分、秒等信息。目前实时时钟芯片大多采用精度较高的晶体振荡器作为时钟源。

### 相关 API

* 设置日期

  ```c
  /* 设置日期 */
  rt_err_t set_date(rt_uint32_t year,						//待设置生效的年份 
                    rt_uint32_t month, 					//待设置生效的月份
                    rt_uint32_t day)						//待设置生效的日
  ```

* 设置时间

  ```c
  /* 设置时间 */
  rt_err_t set_time(rt_uint32_t hour,						//待设置生效的时 
                    rt_uint32_t minute, 					//待设置生效的分
                    rt_uint32_t second)					//待设置生效的秒
  ```

* 获取当前时间

  ```c
  /* 获取当前时间 */
  time_t time(time_t *t)									//时间数据指针
  ```

### 代码实现

```c
static int rtc_test(void *args)
{
    rt_err_t ret = RT_EOK;
    time_t now;

    /* 设置日期 */
    ret = set_date(2022, 7, 23);
    if (ret != RT_EOK)
    {
        rt_kprintf("set RTC date failed\n");
        return ret;
    }

    /* 设置时间 */
    ret = set_time(00, 00, 00);
    if (ret != RT_EOK)
    {
        rt_kprintf("set RTC time failed\n");
        return ret;
    }
    while(1)
    {
        /* 延时3秒 */
        rt_thread_mdelay(3000);

        /* 获取时间 */
        now = time(RT_NULL);
        rt_kprintf("%s\n", ctime(&now));
    }

    return ret;
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(rtc_test, rtc sample);
```

![rtc](.\assets\image-20220723153256640.png)

## 串口驱动对接

串口设备驱动框架可浅析为三个层：设备管理层、设备驱动框架层、设备驱动层。

以 CH32V307 为例：

* 设备管理层（**device.c**）：**rt_device_find、rt_device_open、rt_device_read等**。

* 设备驱动框架层(**serial.c**)：**rt_serial_init、rt_serial_control、rt_serial_open等**。

* 设备驱动层(**drv_usart.c**)：**ch32_configure、ch32_control、ch32_putc、ch32_getc等**。

  

设备管理层的作用为：

* **通过设备管理接口获得正确的设备驱动，然后通过这个设备驱动与底层硬件设备进行交互**。

* **为用户提供标准操作接口**。

  

设备驱动框架层的作用为：

* **向下为设备驱动层提供：需要被实现的操作硬件的基本函数、设备注册函数、中断处理调用函数和需要被实现的结构体等**。

* **在内部调用操作底层硬件的基本函数实现更为复杂的驱动功能**。

* **向上提供标准的驱动接口**。

  

设备驱动层的作用为：

* **提供硬件层的驱动，如引脚初始化等**。



**由此可见：RT-Thread 利用设备驱动框架层将驱动层的底层代码封装成一个又一个标准的驱动接口，再由设备管理层进行进一步的封装，形成应用所用到的标准设备接口函数，从而实现应用层和底层驱动的解耦合，再利用设备驱动框架层提供的接口实现硬件层的控制，实现驱动对接**。

## 更换rt_kprintf的输出终端

![uart](.\assets\image-20220723165709477.png)

可在 RT-Thread Setting 中更改 FinSH 控制台 和 rt_kprintf 的默认输出串口。