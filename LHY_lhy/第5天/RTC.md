### RTC的使用

1.在硬件中使能RTC

2.在组件中 打开 *使用RTC设备驱动程序*

3.可以选择*使用软件模拟RTC设备*,如果使能,==第1条必须关闭,不然会造成冲突!!!==,跳到第5条

4.不选择使用软件模拟RTC,需要打开RA Smart Configurator,新建一个stack,然后在timers中找到r_rtc,修改下面的名字为g_rtc,保存并生成工程.

5.编译整个工程,此时应该0错误0警告.

6.下载到板子里,输入date命令发现回复如下

![image-20220722175248081](https://cdn.jsdelivr.net/gh/lhylhylhy6/Picgo/202207221752126.png)

7.输入 date 2022 7 22 17 46 22 设定当前时间,其格式为

date [year month day hour min sec] 

输入后不会有回显

8.再次输入date测试

发现时间已经更改为我们设定的时间.