/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Main program body.
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/
#include "ch32v30x.h"
#include <rtthread.h>
#include <rthw.h>
#include "drivers/pin.h"
#include <board.h>

/* Global typedef */

/* Global define */

#define LED0_PIN  35   //PC3

/* Global Variable */

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    rt_kprintf("MCU: CH32V307\n");
	rt_kprintf("SysClk: %dHz\n",SystemCoreClock);
    rt_kprintf("www.wch.cn\n");

    LED1_BLINK_INIT();

    GPIO_ResetBits(GPIOA,GPIO_Pin_0);
    while(1)
    {
        GPIO_SetBits(GPIOA,GPIO_Pin_0);
        rt_thread_mdelay(500);
        GPIO_ResetBits(GPIOA,GPIO_Pin_0);
        rt_thread_mdelay(500);
    }
}

int led(void)
{
    rt_uint8_t count;

    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
    rt_kprintf("led_SP:%08x\r\n",__get_SP());
    for(count = 0 ; count < 10 ;count++)
    {
        rt_pin_write(LED0_PIN, PIN_LOW);
        rt_kprintf("led on, count : %d\r\n", count);
        rt_thread_mdelay(500);

        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_kprintf("led off\r\n");
        rt_thread_mdelay(500);
    }
    return 0;
}

MSH_CMD_EXPORT(led,  led sample by using I/O drivers);

static int cnt = 0;

/* 定时器超时函数 */
static void timeout(void *parameter)
{
    rt_kprintf("periodic timer is timeout %d\n", cnt++);
}

static rt_timer_t timer;

void thread_static(void)
{
    /* 创建定时器  周期定时器 */
    timer = rt_timer_create("timer", timeout, RT_NULL, 10, RT_TIMER_FLAG_PERIODIC);
    /* 启动定时器 */
    if (timer != RT_NULL)
        rt_timer_start(timer);
}

void Timer_thread(void)
{
    rt_thread_t thread_timer = RT_NULL;
    thread_timer = rt_thread_create("Timer_thread", thread_static, RT_NULL, 512, 10, 100);
    rt_thread_startup(thread_timer);
}

MSH_CMD_EXPORT(Timer_thread, timer_thread);


