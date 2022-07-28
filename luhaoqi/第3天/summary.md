1 驱动开发
	rt_pin_get()	获取引脚编号
	rt_pin_mode()	设置引脚模式//通过使用rt_pin_mode()设定LED连接的引脚为输出模式，然后再通过rt_pin_write()对引脚写1和置0的方式就可以使LED灯闪烁
	rt_pin_write()	设置引脚电平
	rt_pin_read()	读取引脚电平
	rt_pin_attach_irq()	绑定引脚中断回调函数
	rt_pin_irq_enable()	使能引脚中断
	rt_pin_detach_irq()	脱离引脚中断回调函数

2 I2C通讯协议
	首先进入设置在硬件中开启使能I2C再在组建中勾选I2C设备驱动
	设置完成后下载到板子上通过list_device指令查看系统设备情况，检查I2C设备是否已启动

3 AHT10传感器驱动
	3.1添加以下代码进行初始化
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
	INIT_ENV_EXPORT(rt_hw_aht10_port);
	3.2使用sensor probe temp_aht10或是sensor probe humi_aht10命令来选择温度或是湿度
	3.3使用sensor read来读取数值