## 多线程

---

#### 1、多线程的概念



#### 2、多线程开发模式与传统开发模式的对比

![image-20220719164058967](https://github.com/kurisaW/picbed.git/img/202207191640122.png)

#### 3、轮询开发模式

![image-20220719164138635](https://github.com/kurisaW/picbed.git/img/202207191641746.png)

#### 4、前后台开发模式

![image-20220719164338470](https://github.com/kurisaW/picbed.git/img/202207191643545.png)

#### 5、抢占式多线程开发模式

![image-20220719164359151](https://github.com/kurisaW/picbed.git/img/202207191643236.png)

#### 6、时间片多线程开发模式

![image-20220719164413015](https://github.com/kurisaW/picbed.git/img/202207191644080.png)

#### 7、RT-Thread中的多线程相关概念1

![](https://github.com/kurisaW/picbed.git/img/202207191644080.png)

#### 8、RT-Thread调度机制

![image-20220719164439414](https://github.com/kurisaW/picbed.git/img/202207191644510.png)

#### 9、RT-Thread抢占式调度机制

![image-20220719164453495](https://github.com/kurisaW/picbed.git/img/202207191644571.png)

#### 10、RT-Thread时间片调度机制

![image-20220719164510125](https://github.com/kurisaW/picbed.git/img/202207191645196.png)

#### 11、RT-Thread中的多线程相关概念2

![image-20220719164523970](https://github.com/kurisaW/picbed.git/img/202207191645053.png)

#### 12、RT-Thread线程的状态

![image-20220719164536926](https://github.com/kurisaW/picbed.git/img/202207191645002.png)

#### 13、线程状态的相关切换

![image-20220719164549796](https://github.com/kurisaW/picbed.git/img/202207191645859.png)

#### 14、RT-Thread特殊的线程

![image-20220719164607814](https://github.com/kurisaW/picbed.git/img/202207191646901.png)

`空闲线程（idle线程 ）`

#### 15、RTOS调度器对比

![image-20220719164630967](https://github.com/kurisaW/picbed.git/img/202207191646040.png)

#### 16、RT-Thread调度算法

![image-20220719164645204](https://github.com/kurisaW/picbed.git/img/202207191646265.png)











---

#### 2、RT-Thread静态与动态线程创建的区别

![image-20220719164657078](https://github.com/kurisaW/picbed.git/img/202207191646151.png)

#### 3、静态创建线程

![image-20220719164709624](https://github.com/kurisaW/picbed.git/img/202207191647744.png)

#### 4、动态创建线程

![image-20220719164722387](https://github.com/kurisaW/picbed.git/img/202207191647552.png)

#### 5、启动线程

![image-20220719164732764](https://github.com/kurisaW/picbed.git/img/202207191647858.png)

`在RT-Thread中以'_t'结尾的一般是指针类型`

#### 7、线程睡眠

![image-20220719164744018](https://github.com/kurisaW/picbed.git/img/202207191647119.png)

#### 8、线程退出

![image-20220719164756126](https://github.com/kurisaW/picbed.git/img/202207191647212.png)

`在RT-Thread中是由idle线程来完成回收删除的`

#### 9、RT-Thread错误码分析

![image-20220719164809648](https://github.com/kurisaW/picbed.git/img/202207191648714.png)

#### 10、实验：多线程调度（sample_1_2.c）

![image-20220719164820531](https://github.com/kurisaW/picbed.git/img/202207191648610.png)

#### 19、IPC机制

![image-20220719164831811](https://github.com/kurisaW/picbed.git/img/202207191648890.png)

#### 21、设置和删除空闲钩子

![image-20220719164846151](https://github.com/kurisaW/picbed.git/img/202207191648257.png)

#### 22、设置调度器钩子

![image-20220719164854658](https://github.com/kurisaW/picbed.git/img/202207191648747.png)

#### 23、RT-Thread三个特殊线程

**<1>main线程**

**<2>finish线程**

**<3>RT-Thread idle线程**

![image-20220719164908070](https://github.com/kurisaW/picbed.git/img/202207191649162.png)

![image-20220719164935253](https://github.com/kurisaW/picbed.git/img/202207191649342.png)

![image-20220719164920291](https://github.com/kurisaW/picbed.git/img/202207191649390.png)



---

## RT-Thread定时器

#### 1、RT-Thread定时器

![image-20220719164947185](https://github.com/kurisaW/picbed.git/img/202207191649256.png)

#### 2、创建/初始化定时器

![image-20220719164957609](https://github.com/kurisaW/picbed.git/img/202207191649677.png)

#### 3、定时器参数

![image-20220719165015895](https://github.com/kurisaW/picbed.git/img/202207191650959.png)

#### 4、定时器使用

![image-20220719165006435](https://github.com/kurisaW/picbed.git/img/202207191650529.png)

