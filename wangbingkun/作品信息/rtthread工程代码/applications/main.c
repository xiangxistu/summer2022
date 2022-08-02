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
#include "controller.h"
#include "uart3.h"
#include "measure.h"
/* defined the LED3 pin: PB5 */
#define LED3_PIN GET_PIN(B, 5)

extern rt_uint8_t path_num;
extern int path[8][4];
struct rt_completion measure_completion;


int main(void)
{
    /* set LED3 pin mode to output */
    rt_pin_mode(LED3_PIN, PIN_MODE_OUTPUT);
      rt_completion_init(&measure_completion);
      onenet_mqtt_init();
      dht11_read_temp_sample();
      uart3_init();
      car_init();
    while (1)
    {
        rt_pin_write(LED3_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED3_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }
}


rt_uint8_t a=0;
extern int turn_flag;
void irq_callback_test(void *args)
{
    turn_flag = 0;
    if(a==0)
    {
        car_stop();
        rt_completion_done(&measure_completion);
        rt_kprintf("upload event send\r\n");
        rt_kprintf("stop\r\n");
            int temp[4]={0};
            for(int i=0;i<4;i++)
            {
                if(path[path_num][i]==1)
                {
                    path[path_num][i]=2;
                }
                else if(path[path_num][i]==2)
                {
                    path[path_num][i]=1;
                }
                temp[i] = path[path_num][i];
            }
            for(int i=0;i<4;i++)
            {
                path[path_num][i] = temp[3-i];
                rt_kprintf("%d, ",path[path_num][i]);
            }
            rt_kprintf("\r\n");
    }

    a=1;
}

int icu_sample(void)
{
    /* init */
    rt_uint32_t pin = 40;
    //rt_kprintf("\n pin number : 0x%04X \n", pin);
    rt_pin_mode(40, PIN_MODE_INPUT);
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
    return err;
}
INIT_APP_EXPORT(icu_sample);
