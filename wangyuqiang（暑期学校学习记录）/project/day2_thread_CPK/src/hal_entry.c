/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2021-10-10     Sherman       first version
 * 2021-11-03     Sherman       Add icu_sample
 */

#include <rtthread.h>
#include "hal_data.h"
#include <rtdevice.h>

#define LED3_PIN    BSP_IO_PORT_01_PIN_06
#define USER_INPUT  "P105"

void hal_entry(void)
{
    rt_kprintf("\nHello RT-Thread!\n");


    while (1)
    {
        rt_pin_write(LED3_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED3_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }
}

void irq_callback_test(void *args)
{
    rt_kprintf("\n IRQ00 triggered \n");
}

void icu_sample(void)
{
    /* init */
    rt_uint32_t pin = rt_pin_get(USER_INPUT);
    rt_kprintf("\n pin number : 0x%04X \n", pin);
    rt_err_t err = rt_pin_attach_irq(pin, PIN_IRQ_MODE_RISING, irq_callback_test, RT_NULL);
    if(RT_EOK != err)
    {
        rt_kprintf("\n attach irq failed. \n");
    }
    err = rt_pin_irq_enable(pin, PIN_IRQ_ENABLE);
    if(RT_EOK != err)
    {
        rt_kprintf("\n enable irq failed. \n");
    }
}
MSH_CMD_EXPORT(icu_sample, icu sample);





/* static */
ALIGN(8) // 往下都按8字节对齐
struct rt_thread static_thread;
rt_uint8_t static_stack[2048];

void thread_static(void *parameter)
{
    while(1)
    {
        rt_kprintf("1");
//        rt_thread_mdelay(100);
    }
}

void thread_dynamic(void *parameter)
{
    while(1)
    {
        rt_kprintf("2");
//        rt_thread_mdelay(500);
    }
}

void thread_test(void)
{
    /* init thread */
    rt_thread_init(&static_thread,"static",thread_static,RT_NULL,&static_stack[0],sizeof(static_stack),15,100);

    /* startup thread */
    rt_thread_startup(&static_thread);
    rt_thread_mdelay(100);


    rt_thread_t dynamic_thread = RT_NULL;// 线程结构体指针：设置句柄

    /* create thread */
    dynamic_thread = rt_thread_create("dynamic", thread_dynamic, RT_NULL, 2048, 16, 500);

    rt_thread_startup(dynamic_thread);
    rt_thread_mdelay(500);
}
MSH_CMD_EXPORT(thread_test,static thread / dynamic thread test sample);

