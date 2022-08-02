/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-08-01     10091       the first version
 */

#include "measure.h"
#include <board.h>
//#include "onenet.h"
/* cat_dhtxx sensor data by dynamic */

rt_thread_t measure_thread = RT_NULL ;
rt_device_t dev = RT_NULL;
struct rt_sensor_data sensor_data;
rt_size_t res;
rt_uint8_t get_data_freq = 1; /* 1Hz */
uint8_t temp,humi;
extern struct rt_completion measure_completion;

static void read_temp_entry(void *parameter)
{

    while (1)
    {
        rt_completion_wait(&measure_completion, RT_WAITING_FOREVER);
        dht11_read_Data(38,&temp,&humi);
        onenet_mqtt_upload_digit("dht_temp", temp);
        rt_kprintf("temp:%d\n" ,temp);
    }
}

int dht11_read_temp_sample(void)
{
    dev = rt_device_find("temp_dht11");
    if (dev == RT_NULL)
    {
        return RT_ERROR;
    }

    if (rt_device_open(dev, RT_DEVICE_FLAG_RDWR) != RT_EOK)
    {
        rt_kprintf("open device failed!\n");
        return RT_ERROR;
    }

    rt_device_control(dev, RT_SENSOR_CTRL_SET_ODR, (void *)(&get_data_freq));
    rt_thread_t dht11_thread;
    dht11_thread = rt_thread_create("dht_tem",
                                     read_temp_entry,
                                     RT_NULL,
                                     2048,
                                     0,
                                     200);
    if (dht11_thread != RT_NULL)
    {
        rt_thread_startup(dht11_thread);
    }

    return RT_EOK;
}

static int rt_hw_dht11_port(void)
{
    struct rt_sensor_config cfg;

    cfg.intf.user_data = (void *)DHT11_DATA_PIN;
    rt_hw_dht11_init("dht11", &cfg);

    return RT_EOK;
}
INIT_COMPONENT_EXPORT(rt_hw_dht11_port);


//INIT_APP_EXPORT(dht11_read_temp_sample);



