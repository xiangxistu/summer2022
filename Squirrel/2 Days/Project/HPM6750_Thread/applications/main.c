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

ALIGN(RT_ALIGN_SIZE)
static rt_int8_t static_thread_stack[2048];
static struct rt_thread static_thread;

static rt_thread_t dynamic_thread = RT_NULL;

void thread_entry(void *arg);
void static_thread_entry(void *arg);
void dynamic_thread_entry(void *arg);

int main(void)
{

    app_init_led_pins();

    static uint32_t led_thread_arg = 0;
    rt_thread_t led_thread = rt_thread_create("led_th", thread_entry, &led_thread_arg, 1024, 1, 10);
    rt_thread_startup(led_thread);
    return 0;
}


void thread_entry(void *arg)
{
    while(1){
//        app_led_write(0, APP_LED_ON);
//        rt_thread_mdelay(500);
//        app_led_write(0, APP_LED_OFF);
//        rt_thread_mdelay(500);
//        app_led_write(1, APP_LED_ON);
//        rt_thread_mdelay(500);
//        app_led_write(1, APP_LED_OFF);
//        rt_thread_mdelay(500);
//        app_led_write(2, APP_LED_ON);
//        rt_thread_mdelay(500);
//        app_led_write(2, APP_LED_OFF);
        rt_thread_mdelay(500);
    }
}


void static_thread_entry(void *arg)
{
    while(1)
    {
        rt_thread_mdelay(1000);
        rt_kprintf("1\n");
    }
}

void dynamic_thread_entry(void *arg)
{
    while(1)
    {
        rt_thread_mdelay(1000);
        rt_kprintf("2\n");
    }
}

void rt_thread_test(void)
{
    rt_thread_init(&static_thread,
                   "static",
                   static_thread_entry,
                   RT_NULL,
                   &static_thread_stack[0],
                   sizeof(static_thread_stack),
                   15,
                   100);
    rt_thread_startup(&static_thread);

    dynamic_thread = rt_thread_create("dynamic",
                                      dynamic_thread_entry,
                                      RT_NULL,
                                      2048,
                                      16,
                                      500);
    rt_thread_startup(dynamic_thread);
}

//INIT_APP_EXPORT(rt_thread_test);
MSH_CMD_EXPORT(rt_thread_test,static thread / dynamic thread test sample);
