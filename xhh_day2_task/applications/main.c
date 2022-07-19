/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-20     Abbcc        first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

/* defined the LED2 pin: PE6 */
#define LED2_PIN    GET_PIN(E, 6)


static rt_thread_t tid1=RT_NULL;//任务1句柄

/**
 * @brief   任务1入口函数
 */
static void rtt_task1(void * par)
{
    rt_uint32_t count =0;
    while(1)
    {
        rt_kprintf("rtt_task1 count=%d\r\n",count++);
        rt_thread_mdelay(100);
    }
}

ALIGN(8)//以下代码使用8字节对其
static char task2_stack[1024];
static struct rt_thread task2;//这里注意是结构体，而不是指针，和动态创建有点不同

/**
 * @brief   任务2入口函数
 */
static void rtt_task2(void * par)
{
    rt_uint32_t count =0;
    while(1)
    {
        rt_kprintf("rtt_task2 count=%d\r\n",count++);
        rt_thread_mdelay(500);
    }
}

int main(void)
{
    /* set LED2 pin mode to output */
    rt_pin_mode(LED2_PIN, PIN_MODE_OUTPUT);


    /* 动态创建创建任务1 */
    tid1=rt_thread_create("rtt_task1", /*任务名字*/
                            rtt_task1, /*任务名字*/
                            RT_NULL,
                            1024,
                            25, 5);
    if(tid1 != RT_NULL)
    {
        rt_thread_startup(tid1);
    }
    else {
        rt_kprintf("task1_err");
    }
    /* 静态创建创建任务2 */
    rt_thread_init(&task2,
                   "thread2",
                   rtt_task2,
                   RT_NULL,
                   &task2_stack[0],
                   1024,
                   25,10);
    rt_thread_startup(&task2);


    while (1)
    {
        rt_pin_write(LED2_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED2_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }
}
