```c
#include "sensor_asair_aht10.h"
#define AHT10_I2C_BUS  "i2c1"
int rt_hw_aht10_port(void)
{
    struct rt_sensor_config cfg;
    cfg.intf.dev_name  = AHT10_I2C_BUS;
    cfg.intf.user_data = (void *)AHT10_I2C_ADDR;
    rt_hw_aht10_init("aht10", &cfg);
    return RT_EOK;
}
INIT_APP_EXPORT(rt_hw_aht10_port);
void read_entry(void *parameter)
{
    rt_device_t dev1;
    rt_device_t dev2;
    struct rt_sensor_data data1;
    struct rt_sensor_data data2;
    /* 分成两个设备来打开以及读取,因为在注册时对温度和湿度设备分别注册了*/
    dev1 = rt_device_find("temp_aht10");
    dev2 = rt_device_find("humi_aht10");
    rt_device_open(dev1, RT_DEVICE_FLAG_RDONLY);
    rt_device_open(dev2, RT_DEVICE_FLAG_RDONLY);

    while(1)
    {
        if (rt_device_read(dev1, 0, &data1, 1) == 1)
       {
            rt_kprintf("temp:%d.%d    ",data1.data.temp/10,data1.data.temp%10);
       }
        if (rt_device_read(dev2, 0, &data2, 1) == 1)
       {
            rt_kprintf("huim:%d.%d\r\n",data2.data.humi/10,data2.data.humi%10);
       }
        rt_thread_mdelay(1000);
    }
}

void test_aht10(void *parameter)
{
    rt_thread_t read_threada=RT_NULL;

    read_threada = rt_thread_create("read_thread", read_entry, RT_NULL, 1024, 30, 100);

    if(read_threada!=RT_NULL)
    {
        rt_thread_startup(read_threada);
    }
    else {
        rt_kprintf("read_thread create error!\r\n");
    }
}

MSH_CMD_EXPORT(test_aht10,a sample to test aht10);
```

