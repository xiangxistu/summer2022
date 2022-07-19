/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-18     BruceOu      first implementation
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

/* defined the LED3 pin: PB5 */
#define LED3_PIN GET_PIN(B, 5)
/* ��ʱ���Ŀ��ƿ� */
static rt_timer_t timer1;

ALIGN(8)
struct rt_thread static_thread;
rt_uint8_t static_stack[2048];

/* ��ʱ�� 1 ��ʱ���� */
static void timeout1(void *parameter)
{
    rt_kprintf("periodic timer is timeout \n");
}
//��ʱ���̵߳Ĵ���
int timer_sample(void)
{
    /* ������ʱ�� 1  ���ڶ�ʱ�� */
    timer1 = rt_timer_create("timer1", timeout1,
                             RT_NULL, 100,
                             RT_TIMER_FLAG_PERIODIC);

    /* ������ʱ�� 1 */
    if (timer1 != RT_NULL) rt_timer_start(timer1);
    return 0;
}

//�߳�1�ĺ���
void thread1_static(void)
{
    timer_sample();//������ʱ���߳�
}

//��̬�̵߳Ĵ���
void thread_test(void)
{
    /* init thread */
    rt_thread_t thread1 = RT_NULL;
    /*  startup thread */
    thread1 =rt_thread_create("thread1",
                               thread1_static,
                                RT_NULL,
                                256,
                                16,
                                50);
    rt_thread_startup(thread1);
//    rt_thread_create(name, entry, parameter, stack_size, priority, tick);
}





int main(void)
{
    /* set LED3 pin mode to output */
    rt_pin_mode(LED3_PIN, PIN_MODE_OUTPUT);
    thread_test();
    while (1)
    {
        rt_pin_write(LED3_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED3_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }
}
