/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

static rt_timer_t rt_softimer=RT_NULL;
static void softtimer_callback(void* parameter);

uint8_t RT_data[2]={0,0};
int main(void)
{
    rt_softimer = rt_timer_create("rt_timer",
                                   softtimer_callback,
                                   0,
                                   2000,
                                   RT_TIMER_FLAG_PERIODIC|RT_TIMER_FLAG_SOFT_TIMER);
    if(rt_softimer!=RT_NULL)
    {
        rt_kprintf("rt_timer create successful\r\n");
        rt_timer_start(rt_softimer);
    }
    return RT_EOK;
}


static void softtimer_callback(void* parameter)
{
   rt_kprintf("Hello RTT -First Day\r\n");
}




