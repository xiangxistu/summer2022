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
#define LED3_PIN    GET_PIN(E, 5)
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


ALIGN(8)
struct rt_thread thread1;
static rt_timer_t rt_time;
rt_uint8_t static_stack[2048];

/*任务1函数入口*/
void thread_static(void)
{
    while(1)
    {
        rt_thread_mdelay(1000);
        rt_kprintf("1\n");
    }
}
/*任务2函数入口*/
void thread_dynamic(void)
{
    while(1)
    {
        rt_thread_mdelay(1000);
        rt_kprintf("2\n");
    }
}
/*定时器超时回调函数*/
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
/*任务3函数入口*/
static void thread_time(void * par)
{
    rt_kprintf("enter_time\r\n");
    rt_time= rt_timer_create("time",rt_time_back,RT_NULL,1000,RT_TIMER_FLAG_PERIODIC);
    if(rt_time!=RT_NULL)
    {
           rt_timer_start(rt_time);
    }else {
        rt_kprintf("time_err\r\n");
    }

}

void thread_test_lhq(void)
{
    /*静态创建任务1 */
    rt_thread_init(&thread1,"test1",thread_static,RT_NULL,static_stack,2048,16,50);
    rt_thread_startup(&thread1);

    /*动态创建任务2*/
    rt_thread_t thread2=RT_NULL;
    thread2=rt_thread_create("test2",thread_dynamic,RT_NULL,2048,16,50);

    if(thread2 != RT_NULL)
        {
            rt_thread_startup(thread2);
        }
        else {
            rt_kprintf("thread2 err");
        }

    /*动态创建任务3*/
    rt_thread_t thread3=RT_NULL;
        thread3=rt_thread_create("test3",thread_time,RT_NULL,2048,16,50);

        if(thread3 != RT_NULL)
            {
                rt_thread_startup(thread3);
            }
            else {
                rt_kprintf("thread3 err");
            }
}
INIT_APP_EXPORT(thread_test_lhq);
//MSH_CMD_EXPORT(thread_test_lhq,static thread / dynamic thread test sample);



