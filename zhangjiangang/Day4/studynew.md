RTC设备使用

自己进行了RTC时钟设备的使用。

1.首先在软件包设置里打开软件模拟RTC，这样就不用配置硬件RTC了。

                rt_err_t ret = RT_EOK;
                rt_uint32_t now;
    
                /* 设置日期 */
                ret = set_date(2022, 7, 22);
                if (ret != RT_EOK)
                {
                    rt_kprintf("set RTC date failed\n");
                    return ret;
                }
    
                /* 设置时间 */
                ret = set_time(18, 11, 50);
                if (ret != RT_EOK)
                {
                    rt_kprintf("set RTC time failed\n");
                    return ret;
                }
    
                /* 延时3秒 */
                rt_thread_mdelay(3000);
    
                /* 获取时间 */
                now = time(RT_NULL);
                rt_kprintf("%s\n", ctime(&now));

大概流程为先行设置当前日期和时间，然后延时3秒后查看当前时间。

由于引用time_t的话会报错，所以设置成了下午老师设置的rt_uint32_t，问题成功解决。

打开串口查看执行情况：

![](/rtc.png)

设置时间为18:11:50秒。延时3秒后时间为18:11:53秒。

看门狗设备的使用

按照基本函数来写了一个测试程序：

/* generated main source file - do not edit */
#include "hal_data.h"
#include <rtthread.h>
#include <rtdevice.h>
//#include <time.h>

static rt_device_t wdg_dev;

static void idle_hook(void)
{
    /* 在空闲线程的回调函数里喂狗 */
    rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_KEEPALIVE, NULL);
    rt_kprintf("feed the dog!\n ");
}

int main(void) {
   rt_uint32_t timeout = 1;
   wdg_dev = rt_device_find("wdt");
   if (!wdg_dev)
     {
       rt_kprintf("find %s failed!\n", "wdt");
     }
   rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_SET_TIMEOUT, &timeout);//这里由于板子的设置原因，实际时间要按底层设置的来
   /* 启动看门狗 */
      rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_START, RT_NULL);
      rt_thread_idle_sethook(idle_hook);//设置回调函数

     // hal_entry();
      return 0;
    }

![](/wdt.png)

这里看到果然溢出时间不是一秒。看门狗设备验证成功。

串口驱动

由于瑞萨RA6M4的板级支持包默认开启了uart7，所以控制台也是串口设备uart7。但是很奇怪的是在rt-setting里面没有找到能修改控制台串口以及优先级的地方，因为之前在是stm32上是可以找到修改的地方的。不过后来在rtconfig.h文件中找到了这个修改的地方。

![](/console.png)

在文件里并未找见serial.c文件，只找见了serialv2.c。

在这个文件里看到了rt_serial_init,open,control等接口，这些又被注册到了rt_device中。

在rt_device_find中填入串口的名称，就得到了串口设备句柄，就可以配置串口。串口可以配置波特率，等重要参数，可以通过rt_device_control函数来完成。

![](/serial.png)

看到这里我就一目了然了。填入设备句柄表示要控制的串口，cmd命令填入RT_DEVICE_CTRL_CONFIG，表示控制方面的配置命令。最后修改串口配置结构体中的值将其填入，就完成了串口设备的控制更改。

串口又可以分为中断接收，轮训接收，dma接收等模式，可以通过rt_device_open函数来打开串口。
