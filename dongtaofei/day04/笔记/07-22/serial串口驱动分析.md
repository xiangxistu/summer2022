初始化第一步，把config传到rt_hw_uarta-init中进行板级初始化配置

```c
static rt_err_t rt_serial_init(struct rt_device *dev)
```

![image-20220723082754646](D:\RT-Thread_summer_holiday_happiness\笔记\07-22\image-20220723082754646.png)



config的信息处理

![image-20220723082727868](D:\RT-Thread_summer_holiday_happiness\笔记\07-22\image-20220723082727868.png)

把config的命令复制下来

![image-20220723083043873](D:\RT-Thread_summer_holiday_happiness\笔记\07-22\image-20220723083043873.png)

 

然后跳转到初始化等等的配置

![image-20220723083126377](D:\RT-Thread_summer_holiday_happiness\笔记\07-22\image-20220723083126377.png)



