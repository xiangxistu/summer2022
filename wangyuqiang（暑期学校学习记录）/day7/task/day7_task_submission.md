## day7 task_submission

---

## 瑞萨RA6M4使用Onenet进行设备上云

#### 1、首先先使用esp8266进行联网，并使用AT上网测试（[参考](https://github.com/kurisaW/summer2022/blob/main/wangyuqiang%EF%BC%88%E6%9A%91%E6%9C%9F%E5%AD%A6%E6%A0%A1%E5%AD%A6%E4%B9%A0%E8%AE%B0%E5%BD%95%EF%BC%89/day6/task_submission/task_submission.md)）

#### 2、添加onenet软件包



在onenet官网注册，选择多协议接入，添加一个产品，设置信息自行设定

![image-20220724203508398](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207242035464.png)

![image-20220724203547266](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207242035317.png)

并且添加设备，这里记住设备的相关信息

![image-20220724203756593](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207242037654.png)

![image-20220724203822419](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207242038482.png)

打开RT-Thread Setting，设置相关信息。

![image-20220724203336127](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207242043294.png)保存设置编译报错：

参考此处[[CPK-RA6M4] onenet上云报错<RT-Thread 的版本为 4.1.0 及以上>](https://github.com/RT-Thread/rt-thread/issues/6188)

如果还会存在数据类型报错，跳转到相关类型声明并复制到报错文件中即可。

#### 3、上云测试

![image-20220724204423445](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207242044504.png)

![image-20220724204417333](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207242044403.png)