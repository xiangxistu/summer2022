## 使用AT联网

---

首先需要打开FSP，然后为串口设备新建stack

![image-20220723130346297](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207231303548.png)

在settings->interrupts->Callback,设置为user_uart0_callback

![image-20220723130520036](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207231305133.png)

1、打开pins
2、选择scl0
3、在Pin Configuration中配置Operation Mode为`Asynchronous UATRT`
4、根据信息对应接线

![image-20220723130617373](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207231306479.png)

点击生成配置

![image-20220723130905587](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207231309686.png)

打开RT-Thread Setting使能串口0

![image-20220723130949970](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207231309007.png)

添加软件包并右键打开配置

![image-20220723130924990](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207231309022.png)

![image-20220723131040414](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207231310455.png)

组件中使能AT命令

![image-20220723131130033](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207231311078.png)

保存编译下载即可

![image-20220723131210752](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207231312799.png)





---



![image-20220723163843041](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207231638218.png)

![image-20220723163855987](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207231638035.png)