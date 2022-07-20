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

static rt_timer_t swtmr = RT_NULL;
static rt_thread_t timer_thread = RT_NULL;

static uint32_t TimerCount = 0;

void thread_entry(void *arg);
void timer_thread_entry(void *arg);
void swtmr_callback(void* arg);

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


void timer_thread_entry(void *arg)
{
    swtmr = rt_timer_create("swtmr1_callback",
                            swtmr_callback,
                            0,
                            3000,
                            RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);
    if (swtmr != RT_NULL)
        rt_timer_start(swtmr);
    while(1)
    {
        rt_thread_mdelay(500);
    }
}


void swtmr_callback(void* arg)
{
    TimerCount++;
    rt_kprintf("定时器超时！！！    Count:%d  Name:王建松\n",TimerCount);
}

void rt_thread_test(void)
{

    timer_thread = rt_thread_create("timer",
                                     timer_thread_entry,
                                     RT_NULL,
                                     2048,
                                     5,
                                     100);
    if (timer_thread != RT_NULL)
        rt_thread_startup(timer_thread);
}

//INIT_APP_EXPORT(rt_thread_test);
MSH_CMD_EXPORT(rt_thread_test,test sample);
