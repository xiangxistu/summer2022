aht10软件包的使用

参考资料

[RT-Thread开发之路（6）— 通过I2C获取BH1750光传感器数据_Willliam_william的博客-CSDN博客](https://blog.csdn.net/qq_38113006/article/details/105950097)

RT_Thread的官方文档

导入软件包之后打开sensor_asair_aht10.c文件，找到aht10_port()函数将温度和湿度两个设备进行注册，如果没有的话自定义。

```c
//在线程中读取温度数据
//初始化ath10完成注册 Sensor 设备
int rt_hw_aht10_port(void)
{
    struct rt_sensor_config cfg;

    cfg.intf.dev_name  = "i2c1";  //
    cfg.intf.user_data = (void *)AHT10_I2C_ADDR;  //0x38

    rt_hw_aht10_init("aht10", &cfg);   //humi和temp设备的注册初始化函数

    return RT_EOK;
}

INIT_APP_EXPORT(rt_hw_aht10_port);   //自动初始化
```

随后编译烧录运行，在msh下输入list_device查看aht10的两个设备humi_aht10和temp_aht10两个sensor设备注册成功，随后使用sensor命令进行查看和测试读取数据。

```c
对于温度设备 sensor probe temp_aht10 启动设备。sensor read 读取设备的温度。

对于温度设备 sensor probe humi_aht10 启动设备。sensor read 读取设备的温度

```

<img title="" src=".\picture\sensor命令的使用.png" alt="" width="464" data-align="center">

编写应用代码，创建两个线程，一个是读取温度另外一个是读取湿度。

代码案例

```c
//在线程中读取温度数据
//初始化ath10完成注册 Sensor 设备
int rt_hw_aht10_port(void)
{
    struct rt_sensor_config cfg;

    cfg.intf.dev_name  = AHT10_I2C_BUS;
    cfg.intf.user_data = (void *)AHT10_I2C_ADDR;

    rt_hw_aht10_init("aht10", &cfg);
//    rt_kprintf("rt_hw_aht10_port ok\n");
    return RT_EOK;
}

INIT_ENV_EXPORT(rt_hw_aht10_port);


//在线程中读取温度数据
//通过一个线程，去实时获取aht10 的温度湿度数据。
//线程的基本操作有：创建/初始化( rt_thread_create/rt_thread_init)、启动(rt_thread_startup)、
//运行(rt_thread_delay/rt_thread_control)、删除/脱离(rt_thread_delete/rt_thread_detach)。
//之前我们已经将 ds18b20 对接到 ops 接口并成功注册成传感器设备了，
//接下来就可以利用 Sensor 框架的上层接口 open/close/read/write/control 对 ds18b20 进行操作了

static rt_thread_t ath10_thread_temp = RT_NULL, ath10_thread_humi = RT_NULL;
static void read_temp_entry(void *parameter);
static void read_humi_entry(void *parameter);

void ath10_test(void* parameter)
{
    //读取温度线程创建
    ath10_thread_temp = rt_thread_create("ath10 temp",
                            read_temp_entry,
                            "temp_aht10",//线程入口函数 参数
                            512,
                            RT_THREAD_PRIORITY_MAX / 2,
                            20);
    if (ath10_thread_temp != RT_NULL)
    {
        rt_thread_startup(ath10_thread_temp);
    }
    //读取湿度线程创建
    ath10_thread_humi = rt_thread_create("ath10 humi",
                            read_humi_entry,
                            "humi_aht10",
                            512,
                            RT_THREAD_PRIORITY_MAX / 2,
                            20);
    if (ath10_thread_humi != RT_NULL)
    {
        rt_thread_startup(ath10_thread_humi);
    }
}


//在线程入口函数 read_temp_entry 中，我们通过几个步骤，就可以读取 ds18b20 的温度数据了：
//创建一个 rt_sensor_data 的数据结构体
//查找传感器设备驱动
//打开对应的传感器设备
//配置对应设备-应用程序可以对传感器设备进行配置(可选)
//读取传感器设备数据
//当应用程序完成传感器操作后，可以关闭传感器设备(可选)
static void read_temp_entry(void *parameter)
{
    rt_device_t dev_aht10_temp = RT_NULL;
    struct rt_sensor_data sensor_data;

    rt_size_t res;
    rt_err_t result;
    dev_aht10_temp = rt_device_find(parameter);
    if (dev_aht10_temp == RT_NULL)
    {
        rt_kprintf("can't find device %s\n", parameter);

        return ;
    }

    result = rt_device_open(dev_aht10_temp, RT_DEVICE_FLAG_RDWR);
    if (result != RT_EOK)
    {
        rt_kprintf("open temp device failed\n");
        return ;

    }
    rt_device_control(dev_aht10_temp, RT_SENSOR_CTRL_SET_ODR, (void*)100);

    while(1)
    {
        res = rt_device_read(dev_aht10_temp, 0, &sensor_data, 1);
        if (res != 1)
        {
            rt_kprintf("read temp data filed,size is %d\n", res);
            rt_device_close(dev_aht10_temp);
            return ;
        }
        else
        {
            if (sensor_data.data.temp >= 0)
            {
                rt_kprintf("temp:%3d.%dC\n",sensor_data.data.temp/10, sensor_data.data.temp%10);
            }
            //else 负值的时候
        }
        rt_thread_mdelay(100);
    }

}
static void read_humi_entry(void *parameter)
{
    rt_device_t dev_aht10_humi = RT_NULL;
    struct rt_sensor_data sensor_data;

    rt_size_t res;
    rt_err_t result = RT_EOK;
    rt_uint16_t test_ID;

    dev_aht10_humi = rt_device_find(parameter);
    if (dev_aht10_humi == RT_NULL)
    {
        rt_kprintf("can't find humi device %s\n", parameter);

        return ;
    }

    result = rt_device_open(dev_aht10_humi, RT_DEVICE_FLAG_RDWR);

    if (result != RT_EOK)
    {
        rt_kprintf("open humi device failed\n");
        return ;

    }

    //RT_SENSOR_CTRL_SET_ODR设置传感器数据输出速率，unit is HZ
    rt_device_control(dev_aht10_humi, RT_SENSOR_CTRL_SET_ODR, (void*)100);

    while(1)
    {
        res = rt_device_read(dev_aht10_humi, 0, &sensor_data, 1);
        if (res != 1)
        {
            rt_kprintf("read humi data filed,size is %d\n", res);
            rt_device_close(dev_aht10_humi);
            return ;
        }
        else
        {
            if (sensor_data.data.humi >= 0)
            {
                rt_kprintf("humi:%3d.%d %%RH\n",sensor_data.data.humi/10, sensor_data.data.humi%10);
            }
            //else 负值的时候
        }
        rt_thread_mdelay(100);
    }

}

MSH_CMD_EXPORT(ath10_test, ath10_use);  
```

打印信息

```c
temp: 27.3C
humi: 74.9 %RH
temp: 27.3C
humi: 74.9 %RH
temp: 27.4C
humi: 74.9 %RH
temp: 27.4C
humi: 74.9 %RH
temp: 27.4C
humi: 74.9 %RH
temp: 27.4C
humi: 74.9 %RH
temp: 27.4C
humi: 74.8 %RH
temp: 27.4C
humi: 74.8 %RH
temp: 27.4C
```
