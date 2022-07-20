

/* 此例使用的开发板为CPK_RA6M4
 *
 * 此例在hal_entry()即main()中创建了一个timer_thread线程
 * 在线程中创建了一个软件的周期定时器,500tick为一个周期.
 * 在回调函数中控制灯翻转,并且输出当前灯的状态
 * 创建完成定时器后,timer_thread线程即被自动回收
 *
 * 关键词:PIN 软件定时器 thread创建及其使用
 */

#include <rtthread.h>
#include "hal_data.h"
#include <rtdevice.h>

#define LED3_PIN    BSP_IO_PORT_01_PIN_06

void timer_thread(void *parameter);

void hal_entry(void)
{
    rt_thread_t timerthead=RT_NULL;
    timerthead = rt_thread_create("timer_thread", timer_thread, 0, 2048, 9, 500);
    if(timerthead!=RT_NULL)
    {
        rt_thread_startup(timerthead);
    }
    else {
        rt_kprintf("creat timerthread error\r\n");
    }

    while (1)
    {
        rt_thread_mdelay(500);
    }
}
void timer_sample_overtime(void *parameter)
{
    static rt_uint8_t led_flag=0;
    rt_pin_write(LED3_PIN, led_flag);
    if(led_flag == 0)
    {
        rt_kprintf("led is off\r\n");
        led_flag = 1 ;
    }
    else {
        rt_kprintf("led is on\r\n");
        led_flag = 0 ;
    }
}
void timer_thread(void *parameter)
{
    rt_timer_t timer = RT_NULL;
    timer=rt_timer_create("time_smp", timer_sample_overtime, 0, 500, RT_TIMER_FLAG_SOFT_TIMER|RT_TIMER_FLAG_PERIODIC);
    rt_timer_start(timer);
    return;
}


