/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-07-16     ASUS       the first version
 */
/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-06-21     Tom_onemore       the first version
 */
#include <rtthread.h>
#include <rtdevice.h>
#include "hal_data.h"

#define ADC_DEV_NAME        "adc0"                      /* ADC 设备名称 */
#define ADC_DEV_CHANNEL_1   0                           /* ADC 通道 HC-SR501 */
//#define ADC_DEV_CHANNEL_2   1                           /* ADC 通道 MQ-2 */
#define BUZZER              BSP_IO_PORT_06_PIN_11       /* 蜂鸣器I/O引脚 */

static int adc_vol_sample(int argc, char *argv[])
{
    rt_adc_device_t adc_dev;
    rt_uint32_t value1, vol1;
    rt_err_t ret1 = RT_EOK;
//    rt_err_t ret2 = RT_EOK;

    /* 查找设备 */
    adc_dev = (rt_adc_device_t)rt_device_find(ADC_DEV_NAME);
    if (adc_dev == RT_NULL)
    {
        rt_kprintf("adc sample run failed! can't find %s device!\n", ADC_DEV_NAME);
        return RT_ERROR;
    }

    /* 初始化蜂鸣器引脚为低电平 */
    rt_pin_write(BUZZER, PIN_LOW);

    while(1)
        {
            /* 使能设备 */
            ret1 = rt_adc_enable(adc_dev, ADC_DEV_CHANNEL_1);
//            ret2 = rt_adc_enable(adc_dev, ADC_DEV_CHANNEL_2);
            /* 读取采样值 */
            value1 = rt_adc_read(adc_dev, ADC_DEV_CHANNEL_1);
            rt_kprintf("the value1 is :%d \n", value1);
            rt_hw_us_delay(1000);
//            value2 = rt_adc_read(adc_dev, ADC_DEV_CHANNEL_2);
//            rt_kprintf("the value1 is :%d  the value2 is :%d \n", value1,value2);

            /* 转换为对应电压值，3.3V对应12位最大值4096，数据精度乘以100保留2位小数 */
            vol1 = value1 * 330 / 4096;
//            vol2 = value2 * 330 / 4096;
            rt_kprintf("the voltage1 is :%d.%02d\n", vol1 / 100, vol1 % 100);
            rt_hw_us_delay(1000);
            if (vol1 > 3.3 )
                {
                    rt_pin_write(BUZZER, PIN_HIGH); /*打开蜂鸣器*/

                }
            else
                {
                    rt_pin_write(BUZZER, PIN_LOW); /*关闭蜂鸣器*/
                }
        }

    /* 关闭通道 */
    ret1 = rt_adc_disable(adc_dev, ADC_DEV_CHANNEL_1);
//    ret2 = rt_adc_disable(adc_dev, ADC_DEV_CHANNEL_2);
    return ret1;
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(adc_vol_sample, adc voltage convert sample);
//INIT_BOARD_EXPORT(adc_vol_sample);

