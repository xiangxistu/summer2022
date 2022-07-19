/*
 * Copyright (c) 2021 hpmicro
 *
 * Change Logs:
 * Date         Author          Notes
 * 2021-08-13   Fan YANG        first version
 *
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "rtt_board.h"

void thread_entry(void *arg);
void timeout1(void* args);
void timeout2(void* args);


int main(void)
{
    app_init_led_pins();

    static uint32_t led_thread_arg = 0;
    rt_thread_t led_thread = rt_thread_create("led_th", thread_entry, &led_thread_arg, 1024, 1, 10);
    rt_thread_startup(led_thread);


    rt_timer_t timer1_handle = RT_NULL;
    timer1_handle = rt_timer_create("timer1", timeout1, RT_NULL, 1000, RT_TIMER_FLAG_PERIODIC);// 循环的, 1000ms进一次回调

    rt_timer_t timer2_handle = RT_NULL;
    timer2_handle = rt_timer_create("timer", timeout2, RT_NULL, 2000, RT_TIMER_FLAG_PERIODIC);// 循环的, 2000ms进一次回调

    if(timer1_handle != RT_NULL)
        rt_timer_start(timer1_handle);
    else
        rt_kprintf("error: cannot allocate memory for timer1...\n");

    if(timer2_handle != RT_NULL)
        rt_timer_start(timer2_handle);
    else
        rt_kprintf("error: cannot allocate memory for timer2...\n");


    return 0;
}


void thread_entry(void *arg)
{
    while(1){
        app_led_write(0, APP_LED_ON);
        rt_thread_mdelay(500);
        app_led_write(0, APP_LED_OFF);
        rt_thread_mdelay(500);
        app_led_write(1, APP_LED_ON);
        rt_thread_mdelay(500);
        app_led_write(1, APP_LED_OFF);
        rt_thread_mdelay(500);
        app_led_write(2, APP_LED_ON);
        rt_thread_mdelay(500);
        app_led_write(2, APP_LED_OFF);
        rt_thread_mdelay(500);
    }
}


void timeout1(void* args)
{
    static uint32_t time = 1;
    rt_kprintf("this is timer1 = %d\n", time++);
}


void timeout2(void* args)
{
    static uint32_t time = 1;
    rt_kprintf("this is timer2 = %d\n", time++);
}

