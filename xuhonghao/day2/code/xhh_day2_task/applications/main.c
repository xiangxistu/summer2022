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
/* defined the LED3 pin: PE5 */
#define LED3_PIN    GET_PIN(E, 5)

/* 任务1句柄 */
static rt_thread_t task1=RT_NULL;
/* 任务3句柄 */
static rt_thread_t t_task3=RT_NULL;
/* 定时器控制块 */
static rt_timer_t rt_time;


/**
 * @brief   任务1入口函数
 */
static void rtt_task1(void * par)
{
    rt_uint32_t count =0;
    while(1)
    {
        rt_kprintf("rtt_task1 count=%d\r\n",count++);
        rt_thread_mdelay(1000);
    }
}

ALIGN(8)//以下代码使用8字节对其
static char task2_stack[1024];//任务2栈
static struct rt_thread task2;//这里注意是结构体，而不是指针，和动态创建有点不同

/**
 * @brief   任务2入口函数
 */
static void rtt_task2(void * par)
{
    rt_uint32_t count =0;
    rt_kprintf("enter task2\r\n");
    while(1)
    {
        rt_kprintf("rtt_task2 count=%d\r\n",count++);
        rt_thread_mdelay(1000);
    }
}

/**
 * @brief   定时器超时回调函数
 */
static void rt_time_back(void *par)
{
    static rt_uint32_t time_count=0;
    static char led_state=0;
    if(led_state!=0)
    {
        rt_pin_write(LED3_PIN, PIN_HIGH);
    }else {
        rt_pin_write(LED3_PIN, PIN_LOW);
    }
    led_state=~led_state;
    rt_kprintf("time_back count=%d\r\n",time_count++);
}

/**
 * @brief   任务3入口函数
 */
static void time_task3(void * par)
{
    rt_kprintf("enter_time\r\n");
    rt_time= rt_timer_create("time1",
                             rt_time_back,
                             RT_NULL,
                             1000,RT_TIMER_FLAG_PERIODIC);
    if(rt_time!=RT_NULL)
    {
           rt_timer_start(rt_time);
    }else {
        rt_kprintf("time_err\r\n");
    }

}

/**
 * @brief   主函数
 */
int main(void)
{
    /* set LED2 pin mode to output */
    rt_pin_mode(LED2_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED3_PIN, PIN_MODE_OUTPUT);

    while (1)
    {
        rt_pin_write(LED2_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED2_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }
}

static void xhh_task(void)
{

    /* 动态创建创建任务1 */
    task1=rt_thread_create("rtt_task1", /*任务名字*/
                            rtt_task1, /*任务名字*/
                            RT_NULL,
                            1024,
                            25, 30);
    if(task1 != RT_NULL)
    {
        rt_thread_startup(task1);
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
                   25,30);
    rt_thread_startup(&task2);

    /* 动态创建创建任务3 */
    t_task3=rt_thread_create("time_task3", /*任务名字*/
                            time_task3, /*任务名字*/
                            RT_NULL,
                            1024,
                            25, 10);
    if(t_task3 != RT_NULL)
    {
        rt_thread_startup(t_task3);
    }
    else {
        rt_kprintf("time_task3_err");
    }
}

//MSH_CMD_EXPORT(xhh_task,val);
INIT_APP_EXPORT(xhh_task);









