/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2015-07-29     Arda.Fu      first implementation
 */
#include <stdint.h>
#include <rtthread.h>
#include <rtdevice.h>

/* defined the LED1 pin: PB5 */
#define LED1_PIN    57


rt_timer_t test_timer = RT_NULL;
rt_thread_t create = RT_NULL;

void create_time_thread(void *parameter);
void timeout_thread(void *parameter);
void sample_create(void *parameter);

int main(void)
{
    sample_create(RT_NULL);
      while(1)
          rt_thread_delay(1000);
}

void create_time_thread(void *parameter)
{
    test_timer = rt_timer_create("timer", timeout_thread, RT_NULL,100, RT_TIMER_FLAG_ONE_SHOT|RT_TIMER_FLAG_SOFT_TIMER);
    rt_timer_start(test_timer);

}

void sample_create(void *parameter)
{
    create = rt_thread_create("test", create_time_thread, RT_NULL, 512, 0, 200);
    if(create != RT_NULL)
        rt_kprintf("create successfully\r\n");
    rt_thread_startup(create);
}

void timeout_thread(void *parameter)
{

    rt_kprintf("wangbingkun\r\n");
}



