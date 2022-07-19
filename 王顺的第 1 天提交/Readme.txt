/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 * Copyright (c) 2022, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-04-28     CDT          first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>



/* defined the LED_GREEN pin: PD4 */
#define LED0_PIN GET_PIN(D, 3)
#define LED1_PIN GET_PIN(D, 4)
#define LED2_PIN GET_PIN(D, 5)
#define LED3_PIN GET_PIN(D, 6)

static rt_thread_t rt_1 = RT_NULL;
void rt_1_entry(void* parameter);
static rt_thread_t rt_2 = RT_NULL;
void rt_2_entry(void* parameter);


ALIGN(8)
struct rt_thread static_thread;
rt_uint8_t static_stack[2048];

static rt_thread_t dynamic = RT_NULL;
void thread_dynamic(void* parameter);

void thread_static(void* parameter)
{
   while(1)
   {
       rt_kprintf("1");
   }
}

void thread_dynamic(void* parameter)
{
   while(1)
   {
       rt_kprintf("2");
   }
}

void thread_test(void* parameter)
{
   rt_thread_init(&static_thread,"static", thread_static, NULL, &static_stack[0], 2048, RT_THREAD_PRIORITY_MAX-3, 100);

   rt_thread_startup(&static_thread);

   dynamic = rt_thread_create("dynamic",
                               thread_dynamic,
                               NULL,
                               512,
                               RT_THREAD_PRIORITY_MAX-3,
                               100);
   rt_thread_startup(dynamic);
}
MSH_CMD_EXPORT(thread_test,static thread/dynamic thread test sample);

int main()
{
    rt_thread_mdelay(5);
}
