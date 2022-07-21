## day2 Note

---

#### 1、什么是IPC

![image-20220720093657642](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207200936825.png)

#### 2、目前RT-Thread支持的基础IPC

![image-20220720093913990](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207200939065.png)

#### 3、RT-Thread基础IPC函数

![image-20220720094001960](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207200940022.png)

![image-20220720094034359](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207200940511.png)





---

## 信号量

#### 1、信号量相关API

![image-20220720094438887](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207200944008.png)

#### 2、动态创建信号量

![image-20220720094703910](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207200947033.png)

#### 3、信号量策略

![image-20220720094725327](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207200947554.png)

![image-20220720094858212](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207200948352.png)

![image-20220720094913198](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207200949465.png)

#### 4、静态初始化信号量

![image-20220720095001914](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207200950097.png)

#### 5、获取信号量

![image-20220720095037317](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207200950458.png)

#### 6、释放信号量

![image-20220720095255338](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207200952501.png)

#### 7、中断与线程间的同步应用

![](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207200955524.png)

#### 8、信号量优先级反转

![image-20220720095633828](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207200956024.png)

#### 9、信号量其他API（脱离、删除、控制）

![image-20220720095821609](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207200958721.png)

#### 10、总结：信号量编程注意事项

![image-20220720095931416](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207200959516.png)



---

## 互斥锁

#### 1、概念

![image-20220720100355551](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201003702.png)

#### 2、互斥锁使用场景总结

![image-20220720100502853](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201005973.png)

#### 3、互斥锁优先级继承机制

![image-20220720100942163](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201009316.png)

#### 4、互斥锁嵌套

![image-20220720101038919](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201010035.png)

#### 5、互斥锁相关API

![image-20220720101137507](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201011573.png)

![image-20220720101223462](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201012566.png)

![image-20220720101242980](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201012149.png)

![image-20220720101339609](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201013723.png)

#### 6、互斥锁与信号量的对比

![image-20220720101453640](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201014710.png)



---

## 邮箱

#### 1、RT-Thread邮箱

![image-20220720110034445](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201100865.png)

> 邮箱不拷贝数据，但会拷贝数据的地址

	#### 2、邮箱的工作示意图

![image-20220720110318656](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201103785.png)

#### 3、创建/初始化邮箱

![image-20220720110415072](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201104222.png)

#### 4、邮箱接收邮件

![image-20220720110532349](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201105489.png)

#### 5、邮箱发送邮件

![image-20220720110615231](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201106345.png)

#### 6、等待邮箱发送邮件

![image-20220720110713069](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201107203.png)

#### 7、典型使用场景

![image-20220720110800875](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201108980.png)

---

## 消息队列

#### 1、消息队列介绍

![image-20220720111407881](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201114032.png)

#### 2、消息队列和邮箱有什么不同

![image-20220720111442112](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201114253.png)

#### 3、消息队列状态图

![image-20220720111518610](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201115725.png)

#### 4、消息队列发送消息

![image-20220720111632577](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201116779.png)

![image-20220720111643993](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201116121.png)



---

## 事件

#### 1、事件简介

![image-20220720112611431](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201126565.png)

#### 2、事件工作示意图

![image-20220720112713306](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201127515.png)

#### 3、事件属性

![image-20220720112736498](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201127670.png)

#### 4、事件接收关系示意图

![image-20220720112954926](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201129042.png)

> 分别代表一对一和一对多的事件发送



实时操作系统：在规定时间内响应请求（安全性伴随实时性）





---

## 下午课程



---

## 内存

#### 1、内存介绍

![image-20220720140517150](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201405385.png)

#### 2、静态内存介绍

![image-20220720140542930](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201405059.png)

#### 3、动态内存介绍

![image-20220720141355345](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201413463.png)

#### 4、静态与动态内存对比

![image-20220720141504058](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201415214.png)

#### 5、如何选择静态与动态内存

![image-20220720141657838](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201416990.png)

#### 6、RT-Threead支持的内存管理算法

![image-20220720142007294](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201420451.png)

#### 7、小内存分配算法

![image-20220720142032812](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201420011.png)

![image-20220720142114803](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201421994.png)

![image-20220720142142686](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201421812.png)

#### 8、slap管理算法

![](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201423683.png)

![image-20220720142350348](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201423503.png)

![image-20220720142520924](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201425097.png)

![image-20220720142455479](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201424621.png)

#### 9、memheap管理算法

![image-20220720142554303](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201425431.png)

#### 10、内存堆配置和初始化

![image-20220720142624247](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201426402.png)

![image-20220720142633940](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201426122.png)

#### 11、内存管理算法对齐

![image-20220720142705070](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201427214.png)

#### 12、内存管理算法对比

![image-20220720142801996](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201428109.png)

---

#### 1、接口|分配和释放内存块

![image-20220720142945657](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201429772.png)

#### 2、接口|重分配和分配多内存块

![image-20220720143109271](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201431385.png)

#### 3、接口|设置内存钩子函数

![image-20220720143141906](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201431015.png)

#### 4、libc内存接口

![image-20220720143215517](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201432649.png)

#### 5、对比两套接口

![image-20220720143244515](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201432617.png)

#### 6、代码实现

![image-20220720143307385](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201433511.png)



---

## 内存池接口

#### 1、静态内存池（mempool）

![image-20220720143339473](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201433640.png)

#### 2、静态内存池使用模型

![image-20220720143506303](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201435492.png)

#### 3、静态内存池工作机制

![image-20220720143542602](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201435785.png)

#### 4、静态内存池与内存管理接口的对比

![image-20220720143633471](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201436662.png)

#### 5、使用思路

![image-20220720144001971](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201440097.png)

#### 6、API使用

![](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201440373.png)

![image-20220720144055024](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201440203.png)

![image-20220720144154211](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201441426.png)

![image-20220720144213837](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201442965.png)

#### 7、常见的内存使用问题

![image-20220720144534872](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201445968.png)

![image-20220720144617250](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201446415.png)

![image-20220720144755904](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201447023.png)

![image-20220720144948272](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201449532.png)

![image-20220720145138529](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201451706.png)

> 内存申请而不去free释放，会产生内存碎片，造成内存泄漏

![image-20220720150302113](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201503509.png)

#### 8、判断内存泄漏

![image-20220720150413506](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201504665.png)

![image-20220720150452989](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201504122.png)

![image-20220720150527695](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201505830.png)

![image-20220720150716406](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201507588.png)

#### 9、减少内存泄漏与碎片的建议

![image-20220720150907573](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201509730.png)

