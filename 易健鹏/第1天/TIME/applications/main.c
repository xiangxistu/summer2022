/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-18     BruceOu      first implementation
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

/* defined the LED3 pin: PB5 */
#define LED3_PIN GET_PIN(B, 5)
/* 定时器的控制块 */
static rt_timer_t timer1;

ALIGN(8)
struct rt_thread static_thread;
rt_uint8_t static_stack[2048];

/* 定时器 1 超时函数 */
static void timeout1(void *parameter)
{
    rt_kprintf("periodic timer is timeout \n");
}
//定时器线程的创建
int timer_sample(void)
{
    /* 创建定时器 1  周期定时器 */
    timer1 = rt_timer_create("timer1", timeout1,
                             RT_NULL, 100,
                             RT_TIMER_FLAG_PERIODIC);

    /* 启动定时器 1 */
    if (timer1 != RT_NULL) rt_timer_start(timer1);
    return 0;
}

//线程1的函数
void thread1_static(void)
{
    timer_sample();//创建定时器线程
}

//动态线程的创建
void thread_test(void)
{
    /* init thread */
    rt_thread_t thread1 = RT_NULL;
    /*  startup thread */
    thread1 =rt_thread_create("thread1",
                               thread1_static,
                                RT_NULL,
                                256,
                                16,
                                50);
    rt_thread_startup(thread1);
//    rt_thread_create(name, entry, parameter, stack_size, priority, tick);
}





int main(void)
{
    /* set LED3 pin mode to output */
    rt_pin_mode(LED3_PIN, PIN_MODE_OUTPUT);
    thread_test();
    while (1)
    {
        rt_pin_write(LED3_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED3_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }
}
