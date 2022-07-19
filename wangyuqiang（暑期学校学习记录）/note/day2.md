## 多线程

---

#### 1、多线程的概念



#### 2、多线程开发模式与传统开发模式的对比

![image-20220719094141658](G:/Typora/user_potograph/image-20220719094141658.png)

#### 3、轮询开发模式

![image-20220719094317960](G:/Typora/user_potograph/image-20220719094317960.png)

#### 4、前后台开发模式

![image-20220719094439104](G:/Typora/user_potograph/image-20220719094439104.png)

#### 5、抢占式多线程开发模式

![image-20220719094548511](G:/Typora/user_potograph/image-20220719094548511.png)

#### 6、时间片多线程开发模式

![image-20220719094745176](G:/Typora/user_potograph/image-20220719094745176.png)

#### 7、RT-Thread中的多线程相关概念1

![image-20220719094914881](G:/Typora/user_potograph/image-20220719094914881.png)

#### 8、RT-Thread调度机制

![image-20220719095029322](G:/Typora/user_potograph/image-20220719095029322.png)

#### 9、RT-Thread抢占式调度机制

![image-20220719095100189](G:/Typora/user_potograph/image-20220719095100189.png)

#### 10、RT-Thread时间片调度机制

![image-20220719095342017](G:/Typora/user_potograph/image-20220719095342017.png)

#### 11、RT-Thread中的多线程相关概念2

![image-20220719095434983](G:/Typora/user_potograph/image-20220719095434983.png)

#### 12、RT-Thread线程的状态

![image-20220719095652043](G:/Typora/user_potograph/image-20220719095652043.png)

#### 13、线程状态的相关切换

![image-20220719095925846](G:/Typora/user_potograph/image-20220719095925846.png)

#### 14、RT-Thread特殊的线程

![image-20220719100337172](G:/Typora/user_potograph/image-20220719100337172.png)

`空闲线程（idle线程 ）`

#### 15、RTOS调度器对比

![image-20220719100823907](G:/Typora/user_potograph/image-20220719100823907.png)

#### 16、RT-Thread调度算法

![image-20220719101117722](G:/Typora/user_potograph/image-20220719101117722.png)











---

#### 2、RT-Thread静态与动态线程创建的区别

![image-20220719101354295](G:/Typora/user_potograph/image-20220719101354295.png)

#### 3、静态创建线程

![image-20220719101928014](G:/Typora/user_potograph/image-20220719101928014.png)

#### 4、动态创建线程

![image-20220719102048224](G:/Typora/user_potograph/image-20220719102048224.png)

#### 5、启动线程

![image-20220719102113405](G:/Typora/user_potograph/image-20220719102113405.png)

`在RT-Thread中以'_t'结尾的一般是指针类型`

#### 7、线程睡眠

![image-20220719102232522](G:/Typora/user_potograph/image-20220719102232522.png)

#### 8、线程退出

![image-20220719102302048](G:/Typora/user_potograph/image-20220719102302048.png)

`在RT-Thread中是由idle线程来完成回收删除的`

#### 9、RT-Thread错误码分析

![image-20220719102520913](G:/Typora/user_potograph/image-20220719102520913.png)

#### 10、实验：多线程调度（sample_1_2.c）

![image-20220719102712238](G:/Typora/user_potograph/image-20220719102712238.png)

#### 19、IPC机制

![image-20220719141134622](G:/Typora/user_potograph/image-20220719141134622.png)

#### 21、设置和删除空闲钩子

![image-20220719141328306](G:/Typora/user_potograph/image-20220719141328306.png)

#### 22、设置调度器钩子

![image-20220719141412035](G:/Typora/user_potograph/image-20220719141412035.png)

#### 23、RT-Thread三个特殊线程

**<1>main线程**

**<2>finish线程**

**<3>RT-Thread idle线程**

![image-20220719141448167](G:/Typora/user_potograph/image-20220719141448167.png)

![image-20220719141512247](G:/Typora/user_potograph/image-20220719141512247.png)

![image-20220719145141948](G:/Typora/user_potograph/image-20220719145141948.png)



---

## RT-Thread定时器

#### 1、RT-Thread定时器

![image-20220719145916165](G:/Typora/user_potograph/image-20220719145916165.png)

#### 2、创建/初始化定时器

![image-20220719150000769](G:/Typora/user_potograph/image-20220719150000769.png)

#### 3、定时器参数

![image-20220719150059733](G:/Typora/user_potograph/image-20220719150059733.png)

#### 4、定时器使用

![image-20220719150113318](G:/Typora/user_potograph/image-20220719150113318.png)

