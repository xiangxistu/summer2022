# HC32F460驱动AHT10
1.Shell下查找AHT10
![image](https://github.com/Yaochenger/summer2022/blob/main/WangShun/Day3/3.picture/1.aht10%E4%BC%A0%E6%84%9F%E5%99%A8%E6%9F%A5%E6%89%BE.jpg)
#
2.sensor read测试
![image](https://github.com/Yaochenger/summer2022/blob/main/WangShun/Day3/3.picture/2.aht10%E6%95%B0%E6%8D%AE%E6%B5%8B%E8%AF%95.jpg)
#
3.串口打印的方式输出温湿度
![image](https://github.com/Yaochenger/summer2022/blob/main/WangShun/Day3/3.picture/3.%E4%B8%B2%E5%8F%A3%E6%89%93%E5%8D%B0.jpg)
#
经验总结！！！
`非常感慨RTT的软件框架，在使用IIC总线的时候只需要将设备挂载在IIC总线就可以使用IIC驱动设备，大大的增强了RTT的可移植性，这是其他仅内核型的RTOS不可比拟的，其次由于对RTT的设备框架还不太熟悉，导致使用的时候不太熟练，在使用的AHT11的过程中甚至跳过传感器设备管理层直接使用设备驱动层来获取传感器值，就是上面所列出来的第三张图片，总之学习RTT，使用RTT，发展RTT就行了`



