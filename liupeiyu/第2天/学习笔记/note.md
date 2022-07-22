# 第二天课堂收获

1. 概念：

+ IPC:线程间通信，是一种编程接口，让程序员能够协调不同的进程
+ IPC方法有：信号量（sem）、互斥锁（mutex）、事件组（event）、邮箱（mailbox）、消息队列（messagequeue）
+ 内存管理：计算机系统中，变量、中间数据一般存放在 RAM 中，只有在实际使用时才将它们从 RAM 调入到 CPU 中进行运算。一些数据需要的内存大小需要在程序运行过程中根据实际情况确定，这就要求系统具有对内存空间进行动态管理的能力，在用户需要一段内存空间时，向系统申请，系统选择一段合适的内存空间分配给用户，用户使用完毕后，再释放回系统，以便系统将该段内存空间回收再利用。



2. 信号量（sem）:分为动态信号量和静态信号量，可能导致线程优先级翻转

+ 以生活中的停车场为例来理解信号量的概念：

  ①当停车场空的时候，停车场的管理员发现有很多空车位，此时会让外面的车陆续进入停车场获得停车位；

  ②当停车场的车位满的时候，管理员发现已经没有空车位，将禁止外面的车进入停车场，车辆在外排队等候；

  ③当停车场内有车离开时，管理员发现有空的车位让出，允许外面的车进入停车场；待空车位填满后，又禁止外部车辆进入。

  在此例子中，管理员就相当于信号量，管理员手中空车位的个数就是信号量的值（非负数，动态变化）；停车位相当于公共资源（临界区），车辆相当于线程。车辆通过获得管理员的允许取得停车位，就类似于线程通过获得信号量访问公共资源。

  信号量工作示意图如下图所示，每个信号量对象都有一个信号量值和一个线程等待队列，信号量的值对应了信号量对象的实例数目、资源数目，假如信号量值为 5，则表示共有 5 个信号量实例（资源）可以被使用，当信号量实例数目为零时，再申请该信号量的线程就会被挂起在该信号量的等待队列上，等待可用的信号量实例（资源）。

  ![image-20220720172359326](C:\Users\刘佩羽\AppData\Roaming\Typora\typora-user-images\image-20220720172359326.png)

  ### [信号量控制块](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc1/ipc1?id=信号量控制块)

  在 RT-Thread 中，信号量控制块是操作系统用于管理信号量的一个数据结构，由结构体 struct rt_semaphore 表示。另外一种 C 表达方式 rt_sem_t，表示的是信号量的句柄，在 C 语言中的实现是指向信号量控制块的指针。信号量控制块结构的详细定义如下：

  ``` c
  struct rt_semaphore
  {
     struct rt_ipc_object parent;  /* 继承自 ipc_object 类 */
     rt_uint16_t value;            /* 信号量的值 */
  };
  /* rt_sem_t 是指向 semaphore 结构体的指针类型 */
  typedef struct rt_semaphore* rt_sem_t;
  
  ```

  ### [信号量的管理方式](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc1/ipc1?id=信号量的管理方式)

  信号量相关接口如下图所示，对一个信号量的操作包含：创建 / 初始化信号量、获取信号量、释放信号量、删除 / 脱离信号量。

  ![image-20220720173727613](C:\Users\刘佩羽\AppData\Roaming\Typora\typora-user-images\image-20220720173727613.png)

  脱离信号量就是让信号量对象从内核对象管理器中脱离，适用于静态初始化的信号量。

  #### [获取信号量](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc1/ipc1?id=获取信号量)

  线程通过获取信号量来获得信号量资源实例，当信号量值大于零时，线程将获得信号量，并且相应的信号量值会减 1，获取信号量使用下面的函数接口：

  ```c
  rt_err_t rt_sem_take (rt_sem_t sem, rt_int32_t time);
  /**/
  ```

  在调用这个函数时，如果信号量的值等于零，那么说明当前信号量资源实例不可用，申请该信号量的线程将根据 time 参数的情况选择直接返回、或挂起等待一段时间、或永久等待，直到其他线程或中断释放该信号量。如果在参数 time 指定的时间内依然得不到信号量，线程将超时返回，返回值是 - RT_ETIMEOUT。

  #### [无等待获取信号量](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc1/ipc1?id=无等待获取信号量)

  当用户不想在申请的信号量上挂起线程进行等待时，可以使用无等待方式获取信号量,用下面的函数接口：

​				`rt_err_t rt_sem_trytake(rt_sem_t sem);`

​		

#### 			   	[释放信号量](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc1/ipc1?id=释放信号量)

​		 释放信号量可以唤醒挂起在该信号量上的线程。释放信号量使用下面的函数接口：

​				`rt_err_t rt_sem_release(rt_sem_t sem);`

​				------部分内容来自RT-Thread文档中心

3. ## [互斥量](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc1/ipc1?id=互斥量)

   互斥量又叫相互排斥的信号量，是一种特殊的二值信号量。互斥量类似于只有一个车位的停车场：当有一辆车进入的时候，将停车场大门锁住，其他车辆在外面等候。当里面的车出来时，将停车场大门打开，下一辆车才可以进入。

   互斥量和信号量不同的是：拥有互斥量的线程拥有互斥量的所有权，互斥量支持递归访问且能防止线程优先级翻转；并且互斥量只能由持有线程释放，而信号量则可以由任何线程释放。

   ![image-20220720205034795](C:\Users\刘佩羽\AppData\Roaming\Typora\typora-user-images\image-20220720205034795.png)

   互斥量可以解决优先级翻转问题，实现的是优先级继承协议 （如下图）

   优先级继承是通过在线程 A 尝试获取共享资源而被挂起的期间内，将线程 C 的优先级提升到线程 A 的优先级别，从而解决优先级翻转引起的问题。这样能够防止 C（间接地防止 A）被 B 抢占，如下图所示。优先级继承是指，提高某个占有某种资源的低优先级线程的优先级，使之与所有等待该资源的线程中优先级最高的那个线程的优先级相等，然后执行，而当这个低优先级线程释放该资源时，优先级重新回到初始设定。因此，继承优先级的线程避免了系统资源被任何中间优先级的线程抢占。

   ![image-20220720205720273](C:\Users\刘佩羽\AppData\Roaming\Typora\typora-user-images\image-20220720205720273.png)

   ### [互斥量控制块](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc1/ipc1?id=互斥量控制块)

   结构体 struct rt_mutex 表示。

   ### [互斥量的管理方式](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc1/ipc1?id=互斥量的管理方式)

   ![image-20220720205925972](C:\Users\刘佩羽\AppData\Roaming\Typora\typora-user-images\image-20220720205925972.png)

   #### [初始化和脱离互斥量](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc1/ipc1?id=初始化和脱离互斥量)

   对于静态互斥量而言，使用的函数与动态互斥量不同。

4. ## [事件集](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc1/ipc1?id=事件集)

   事件集也是线程间同步的机制之一，一个事件集可以包含多个事件，利用事件集可以完成一对多，多对多的线程间同步。下面以坐公交为例说明事件，在公交站等公交时可能有以下几种情况：

   ①P1 坐公交去某地，只有一种公交可以到达目的地，等到此公交即可出发。

   ②P1 坐公交去某地，有 3 种公交都可以到达目的地，等到其中任意一辆即可出发。

   ③P1 约另一人 P2 一起去某地，则 P1 必须要等到 “同伴 P2 到达公交站” 与“公交到达公交站”两个条件都满足后，才能出发。

   这里，可以将 P1 去某地视为线程，将 “公交到达公交站”、“同伴 P2 到达公交站” 视为事件的发生，情况①是特定事件唤醒线程；情况②是任意单个事件唤醒线程；情况③是多个事件同时发生才唤醒线程。

   ### [事件集工作机制](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc1/ipc1?id=事件集工作机制)

   RT-Thread 定义的事件集有以下特点：

   1）事件只与线程相关，事件间相互独立：每个线程可拥有 32 个事件标志，采用一个 32 bit 无符号整型数进行记录，每一个 bit 代表一个事件；

   2）事件仅用于同步，不提供数据传输功能；

   3）事件无排队性，即多次向线程发送同一事件 (如果线程还未来得及读走)，其效果等同于只发送一次。

   ![image-20220720212539360](C:\Users\刘佩羽\AppData\Roaming\Typora\typora-user-images\image-20220720212539360.png)

   如上图所示，线程 #1 的事件标志中第 1 位和第 30 位被置位，如果事件信息标记位设为逻辑与，则表示线程 #1 只有在事件 1 和事件 30 都发生以后才会被触发唤醒，如果事件信息标记位设为逻辑或，则事件 1 或事件 30 中的任意一个发生都会触发唤醒线程 #1。如果信息标记同时设置了清除标记位，则当线程 #1 唤醒后将主动把事件 1 和事件 30 清为零，否则事件标志将依然存在（即置 1）。

   ### [事件集控制块](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc1/ipc1?id=事件集控制块)

   事件集控制块是操作系统用于管理事件的一个数据结构，由结构体 struct rt_event 表示。rt_event 对象从 rt_ipc_object 中派生，由 IPC 容器所管理。

   ### [事件集的管理方式](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc1/ipc1?id=事件集的管理方式)

   ![image-20220720212914016](C:\Users\刘佩羽\AppData\Roaming\Typora\typora-user-images\image-20220720212914016.png)

   #### [初始化和脱离事件集](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc1/ipc1?id=初始化和脱离事件集)

   静态事件集对象的内存是在系统编译时由编译器分配的，一般放于读写数据段或未初始化数据段中。

5. ## [邮箱](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc2/ipc2?id=邮箱)

   邮箱服务是实时操作系统中一种典型的线程间通信方法。举一个简单的例子，有两个线程，线程 1 检测按键状态并发送，线程 2 读取按键状态并根据按键的状态相应地改变 LED 的亮灭。这里就可以使用邮箱的方式进行通信，线程 1 将按键的状态作为邮件发送到邮箱，线程 2 在邮箱中读取邮件获得按键状态并对 LED 执行亮灭操作。

   RT-Thread 操作系统的邮箱用于线程间通信，特点是开销比较低，效率较高。邮箱中的每一封邮件只能容纳固定的 4 字节内容（针对 32 位处理系统，指针的大小即为 4 个字节，所以一封邮件恰好能够容纳一个指针）。典型的邮箱也称作交换消息，如下图所示，线程或中断服务例程把一封 4 字节长度的邮件发送到邮箱中，而一个或多个线程可以从邮箱中接收这些邮件并进行处理。

   ![image-20220720220834833](C:\Users\刘佩羽\AppData\Roaming\Typora\typora-user-images\image-20220720220834833.png)

   非阻塞方式的邮件发送过程能够安全的应用于中断服务中，是线程、中断服务、定时器向线程发送消息的有效手段。当邮箱中不存在邮件且超时时间不为 0 时，邮件收取过程将变成阻塞方式。在这类情况下，只能由线程进行邮件的收取。

   当一个线程向邮箱发送邮件时，如果邮箱没满，将把邮件复制到邮箱中。如果邮箱已经满了，发送线程可以设置超时时间，选择等待挂起或直接返回 - RT_EFULL。如果发送线程选择挂起等待，那么当邮箱中的邮件被收取而空出空间来时，等待挂起的发送线程将被唤醒继续发送。

   当一个线程从邮箱中接收邮件时，如果邮箱是空的，接收线程可以选择是否等待挂起直到收到新的邮件而唤醒，或可以设置超时时间。当达到设置的超时时间，邮箱依然未收到邮件时，这个选择超时等待的线程将被唤醒并返回 - RT_ETIMEOUT。如果邮箱中存在邮件，那么接收线程将复制邮箱中的 4 个字节邮件到接收缓存中。

   ### [邮箱控制块](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc2/ipc2?id=邮箱控制块)

   在 RT-Thread 中，邮箱控制块是操作系统用于管理邮箱的一个数据结构，由结构体 struct rt_mailbox 表示。另外一种 C 表达方式 rt_mailbox_t，表示的是邮箱的句柄，在 C 语言中的实现是邮箱控制块的指针。

   ### [邮箱的管理方式](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc2/ipc2?id=邮箱的管理方式)

   ![image-20220720221313041](C:\Users\刘佩羽\AppData\Roaming\Typora\typora-user-images\image-20220720221313041.png)

   #### [初始化和脱离邮箱](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc2/ipc2?id=初始化和脱离邮箱)

   初始化邮箱跟创建邮箱类似，只是初始化邮箱用于静态邮箱对象的初始化。与创建邮箱不同的是，静态邮箱对象的内存是在系统编译时由编译器分配的，一般放于读写数据段或未初始化数据段中，其余的初始化工作与创建邮箱时相同

6. ## [消息队列](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc2/ipc2?id=消息队列)

   消息队列能够接收来自线程或中断服务例程中不固定长度的消息，并把消息缓存在自己的内存空间中。其他线程也能够从消息队列中读取相应的消息，而当消息队列是空的时候，可以挂起读取线程。当有新的消息到达时，挂起的线程将被唤醒以接收并处理消息。消息队列是一种异步的通信方式。

   如下图所示，线程或中断服务例程可以将一条或多条消息放入消息队列中。同样，一个或多个线程也可以从消息队列中获得消息。当有多个消息发送到消息队列时，通常将先进入消息队列的消息先传给线程，也就是说，线程先得到的是最先进入消息队列的消息，即先进先出原则 (FIFO)。

   ![image-20220720230435642](C:\Users\刘佩羽\AppData\Roaming\Typora\typora-user-images\image-20220720230435642.png)

   ### [消息队列控制块](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc2/ipc2?id=消息队列控制块)

   

   # [线程间通信](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc2/ipc2?id=线程间通信)
   
   前面一章讲了线程间同步，提到了信号量、互斥量、事件集等概念；本章接着上一章的内容，讲解线程间通信。在裸机编程中，经常会使用全局变量进行功能间的通信，如某些功能可能由于一些操作而改变全局变量的值，另一个功能对此全局变量进行读取，根据读取到的全局变量值执行相应的动作，达到通信协作的目的。RT-Thread 中则提供了更多的工具帮助在不同的线程中间传递信息，本章会详细介绍这些工具。学习完本章，大家将学会如何将邮箱、消息队列、信号用于线程间的通信。
   
   ## [邮箱](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc2/ipc2?id=邮箱)
   
   邮箱服务是实时操作系统中一种典型的线程间通信方法。举一个简单的例子，有两个线程，线程 1 检测按键状态并发送，线程 2 读取按键状态并根据按键的状态相应地改变 LED 的亮灭。这里就可以使用邮箱的方式进行通信，线程 1 将按键的状态作为邮件发送到邮箱，线程 2 在邮箱中读取邮件获得按键状态并对 LED 执行亮灭操作。
   
   这里的线程 1 也可以扩展为多个线程。例如，共有三个线程，线程 1 检测并发送按键状态，线程 2 检测并发送 ADC 采样信息，线程 3 则根据接收的信息类型不同，执行不同的操作。
   
   ### [邮箱的工作机制](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc2/ipc2?id=邮箱的工作机制)
   
   RT-Thread 操作系统的邮箱用于线程间通信，特点是开销比较低，效率较高。邮箱中的每一封邮件只能容纳固定的 4 字节内容（针对 32 位处理系统，指针的大小即为 4 个字节，所以一封邮件恰好能够容纳一个指针）。典型的邮箱也称作交换消息，如下图所示，线程或中断服务例程把一封 4 字节长度的邮件发送到邮箱中，而一个或多个线程可以从邮箱中接收这些邮件并进行处理。
   
   ![邮箱工作示意图](https://www.rt-thread.org/document/site/rt-thread-version/rt-thread-standard/programming-manual/ipc2/figures/07mb_work.png)
   
   非阻塞方式的邮件发送过程能够安全的应用于中断服务中，是线程、中断服务、定时器向线程发送消息的有效手段。通常来说，邮件收取过程可能是阻塞的，这取决于邮箱中是否有邮件，以及收取邮件时设置的超时时间。当邮箱中不存在邮件且超时时间不为 0 时，邮件收取过程将变成阻塞方式。在这类情况下，只能由线程进行邮件的收取。
   
   当一个线程向邮箱发送邮件时，如果邮箱没满，将把邮件复制到邮箱中。如果邮箱已经满了，发送线程可以设置超时时间，选择等待挂起或直接返回 - RT_EFULL。如果发送线程选择挂起等待，那么当邮箱中的邮件被收取而空出空间来时，等待挂起的发送线程将被唤醒继续发送。
   
   当一个线程从邮箱中接收邮件时，如果邮箱是空的，接收线程可以选择是否等待挂起直到收到新的邮件而唤醒，或可以设置超时时间。当达到设置的超时时间，邮箱依然未收到邮件时，这个选择超时等待的线程将被唤醒并返回 - RT_ETIMEOUT。如果邮箱中存在邮件，那么接收线程将复制邮箱中的 4 个字节邮件到接收缓存中。
   
   ### [邮箱控制块](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc2/ipc2?id=邮箱控制块)
   
   在 RT-Thread 中，邮箱控制块是操作系统用于管理邮箱的一个数据结构，由结构体 struct rt_mailbox 表示。另外一种 C 表达方式 rt_mailbox_t，表示的是邮箱的句柄，在 C 语言中的实现是邮箱控制块的指针。邮箱控制块结构的详细定义请见以下代码：
   
   ```c
   struct rt_mailbox
   {
       struct rt_ipc_object parent;
   
       rt_uint32_t* msg_pool;                /* 邮箱缓冲区的开始地址 */
       rt_uint16_t size;                     /* 邮箱缓冲区的大小     */
   
       rt_uint16_t entry;                    /* 邮箱中邮件的数目     */
       rt_uint16_t in_offset, out_offset;    /* 邮箱缓冲的进出指针   */
       rt_list_t suspend_sender_thread;      /* 发送线程的挂起等待队列 */
   };
   typedef struct rt_mailbox* rt_mailbox_t;复制错误复制成功
   ```
   
   rt_mailbox 对象从 rt_ipc_object 中派生，由 IPC 容器所管理。
   
   ### [邮箱的管理方式](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc2/ipc2?id=邮箱的管理方式)
   
   邮箱控制块是一个结构体，其中含有事件相关的重要参数，在邮箱的功能实现中起重要的作用。邮箱的相关接口如下图所示，对一个邮箱的操作包含：创建 / 初始化邮箱、发送邮件、接收邮件、删除 / 脱离邮箱。
   
   ![邮箱相关接口](https://www.rt-thread.org/document/site/rt-thread-version/rt-thread-standard/programming-manual/ipc2/figures/07mb_ops.png)
   
   #### [创建和删除邮箱](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc2/ipc2?id=创建和删除邮箱)
   
   动态创建一个邮箱对象可以调用如下的函数接口：
   
   ```c
   rt_mailbox_t rt_mb_create (const char* name, rt_size_t size, rt_uint8_t flag);复制错误复制成功
   ```
   
   创建邮箱对象时会先从对象管理器中分配一个邮箱对象，然后给邮箱动态分配一块内存空间用来存放邮件，这块内存的大小等于邮件大小（4 字节）与邮箱容量的乘积，接着初始化接收邮件数目和发送邮件在邮箱中的偏移量。下表描述了该函数的输入参数与返回值：
   
   | **参数**       | **描述**                                                     |
   | -------------- | ------------------------------------------------------------ |
   | name           | 邮箱名称                                                     |
   | size           | 邮箱容量                                                     |
   | flag           | 邮箱标志，它可以取如下数值： RT_IPC_FLAG_FIFO 或 RT_IPC_FLAG_PRIO |
   | **返回**       | ——                                                           |
   | RT_NULL        | 创建失败                                                     |
   | 邮箱对象的句柄 | 创建成功                                                     |
   
   Note
   
   注：RT_IPC_FLAG_FIFO 属于非实时调度方式，除非应用程序非常在意先来后到，并且你清楚地明白所有涉及到该邮箱的线程都将会变为非实时线程，方可使用 RT_IPC_FLAG_FIFO，否则建议采用 RT_IPC_FLAG_PRIO，即确保线程的实时性。
   
   当用 rt_mb_create() 创建的邮箱不再被使用时，应该删除它来释放相应的系统资源，一旦操作完成，邮箱将被永久性的删除。删除邮箱的函数接口如下：
   
   ```c
   rt_err_t rt_mb_delete (rt_mailbox_t mb);复制错误复制成功
   ```
   
   删除邮箱时，如果有线程被挂起在该邮箱对象上，内核先唤醒挂起在该邮箱上的所有线程（线程返回值是 -RT_ERROR），然后再释放邮箱使用的内存，最后删除邮箱对象。下表描述了该函数的输入参数与返回值：
   
   | **参数** | **描述**       |
   | -------- | -------------- |
   | mb       | 邮箱对象的句柄 |
   | **返回** | ——             |
   | RT_EOK   | 成功           |
   
   #### [初始化和脱离邮箱](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc2/ipc2?id=初始化和脱离邮箱)
   
   初始化邮箱跟创建邮箱类似，只是初始化邮箱用于静态邮箱对象的初始化。与创建邮箱不同的是，静态邮箱对象的内存是在系统编译时由编译器分配的，一般放于读写数据段或未初始化数据段中，其余的初始化工作与创建邮箱时相同。函数接口如下：
   
   ```c
     rt_err_t rt_mb_init(rt_mailbox_t mb,
                       const char* name,
                       void* msgpool,
                       rt_size_t size,
                       rt_uint8_t flag)复制错误复制成功
   ```

   初始化邮箱时，该函数接口需要获得用户已经申请获得的邮箱对象控制块，缓冲区的指针，以及邮箱名称和邮箱容量（能够存储的邮件数）。下表描述了该函数的输入参数与返回值：

   | **参数** | **描述**                                                     |
   | -------- | ------------------------------------------------------------ |
   | mb       | 邮箱对象的句柄                                               |
   | name     | 邮箱名称                                                     |
   | msgpool  | 缓冲区指针                                                   |
   | size     | 邮箱容量                                                     |
   | flag     | 邮箱标志，它可以取如下数值： RT_IPC_FLAG_FIFO 或 RT_IPC_FLAG_PRIO |
   | **返回** | ——                                                           |
   | RT_EOK   | 成功                                                         |

   这里的 size 参数指定的是邮箱的容量，即如果 msgpool 指向的缓冲区的字节数是 N，那么邮箱容量应该是 N/4。
   
   脱离邮箱将把静态初始化的邮箱对象从内核对象管理器中脱离。脱离邮箱使用下面的接口：
   
   ```c
   rt_err_t rt_mb_detach(rt_mailbox_t mb);复制错误复制成功
   ```

   使用该函数接口后，内核先唤醒所有挂在该邮箱上的线程（线程获得返回值是 - RT_ERROR），然后将该邮箱对象从内核对象管理器中脱离。下表描述了该函数的输入参数与返回值：

   | **参数** | **描述**       |
   | -------- | -------------- |
   | mb       | 邮箱对象的句柄 |
   | **返回** | ——             |
   | RT_EOK   | 成功           |
   
   #### [发送邮件](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc2/ipc2?id=发送邮件)

   线程或者中断服务程序可以通过邮箱给其他线程发送邮件，发送邮件函数接口如下：

   ```c
   rt_err_t rt_mb_send (rt_mailbox_t mb, rt_uint32_t value);复制错误复制成功
   ```
   
   发送的邮件可以是 32 位任意格式的数据，一个整型值或者一个指向缓冲区的指针。当邮箱中的邮件已经满时，发送邮件的线程或者中断程序会收到 -RT_EFULL 的返回值。下表描述了该函数的输入参数与返回值：
   
   | **参数**  | **描述**       |
   | --------- | -------------- |
   | mb        | 邮箱对象的句柄 |
   | value     | 邮件内容       |
   | **返回**  | ——             |
   | RT_EOK    | 发送成功       |
   | -RT_EFULL | 邮箱已经满了   |

   #### [等待方式发送邮件](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc2/ipc2?id=等待方式发送邮件)
   
   用户也可以通过如下的函数接口向指定邮箱发送邮件：

   ```c
   rt_err_t rt_mb_send_wait (rt_mailbox_t mb,
                         rt_uint32_t value,
                         rt_int32_t timeout);复制错误复制成功
   ```
   
   rt_mb_send_wait() 与 rt_mb_send() 的区别在于有等待时间，如果邮箱已经满了，那么发送线程将根据设定的 timeout 参数等待邮箱中因为收取邮件而空出空间。如果设置的超时时间到达依然没有空出空间，这时发送线程将被唤醒并返回错误码。下表描述了该函数的输入参数与返回值：

   | **参数**     | **描述**       |
   | ------------ | -------------- |
   | mb           | 邮箱对象的句柄 |
   | value        | 邮件内容       |
   | timeout      | 超时时间       |
   | **返回**     | ——             |
   | RT_EOK       | 发送成功       |
   | -RT_ETIMEOUT | 超时           |
   | -RT_ERROR    | 失败，返回错误 |

   #### [发送紧急邮件](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc2/ipc2?id=发送紧急邮件)
   
   发送紧急邮件的过程与发送邮件几乎一样，唯一的不同是，当发送紧急邮件时，邮件被直接插队放入了邮件队首，这样，接收者就能够优先接收到紧急邮件，从而及时进行处理。发送紧急邮件的函数接口如下：
   
   ```c
   rt_err_t rt_mb_urgent (rt_mailbox_t mb, rt_ubase_t value);复制错误复制成功
   ```

   下表描述了该函数的输入参数与返回值：

   | **参数**  | **描述**       |
   | --------- | -------------- |
   | mb        | 邮箱对象的句柄 |
   | value     | 邮件内容       |
   | **返回**  | ——             |
   | RT_EOK    | 发送成功       |
   | -RT_EFULL | 邮箱已满       |

   #### [接收邮件](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc2/ipc2?id=接收邮件)

   只有当接收者接收的邮箱中有邮件时，接收者才能立即取到邮件并返回 RT_EOK 的返回值，否则接收线程会根据超时时间设置，或挂起在邮箱的等待线程队列上，或直接返回。接收邮件函数接口如下：
   
   ```c
   rt_err_t rt_mb_recv (rt_mailbox_t mb, rt_uint32_t* value, rt_int32_t timeout);复制错误复制成功
   ```
   
   ### [邮箱使用示例](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc2/ipc2?id=邮箱使用示例)
   
   

   ### [邮箱的使用场合](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc2/ipc2?id=邮箱的使用场合)

   邮箱是一种简单的线程间消息传递方式，特点是开销比较低，效率较高。在 RT-Thread 操作系统的实现中能够一次传递一个 4 字节大小的邮件，并且邮箱具备一定的存储功能，能够缓存一定数量的邮件数 (邮件数由创建、初始化邮箱时指定的容量决定)。邮箱中一封邮件的最大长度是 4 字节，所以邮箱能够用于不超过 4 字节的消息传递。由于在 32 系统上 4 字节的内容恰好可以放置一个指针，因此当需要在线程间传递比较大的消息时，可以把指向一个缓冲区的指针作为邮件发送到邮箱中，即邮箱也可以传递指针，例如：

   ```c
   struct msg
   {
       rt_uint8_t *data_ptr;
       rt_uint32_t data_size;
   };复制错误复制成功
   ```
   
   对于这样一个消息结构体，其中包含了指向数据的指针 data_ptr 和数据块长度的变量 data_size。当一个线程需要把这个消息发送给另外一个线程时，可以采用如下的操作：
   
   ```c
   struct msg* msg_ptr;
   
   msg_ptr = (struct msg*)rt_malloc(sizeof(struct msg));
   msg_ptr->data_ptr = ...; /* 指向相应的数据块地址 */
   msg_ptr->data_size = len; /* 数据块的长度 */
   /* 发送这个消息指针给 mb 邮箱 */
   rt_mb_send(mb, (rt_uint32_t)msg_ptr);复制错误复制成功
   ```
   
   而在接收线程中，因为收取过来的是指针，而 msg_ptr 是一个新分配出来的内存块，所以在接收线程处理完毕后，需要释放相应的内存块：

   ```c
   struct msg* msg_ptr;
   if (rt_mb_recv(mb, (rt_uint32_t*)&msg_ptr) == RT_EOK)
   {
       /* 在接收线程处理完毕后，需要释放相应的内存块 */
       rt_free(msg_ptr);
   }复制错误复制成功
   ```
   
   ## [消息队列](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc2/ipc2?id=消息队列)
   
   消息队列是另一种常用的线程间通讯方式，是邮箱的扩展。可以应用在多种场合：线程间的消息交换、使用串口接收不定长数据等。
   
   ### [消息队列的工作机制](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc2/ipc2?id=消息队列的工作机制)
   
   消息队列能够接收来自线程或中断服务例程中不固定长度的消息，并把消息缓存在自己的内存空间中。其他线程也能够从消息队列中读取相应的消息，而当消息队列是空的时候，可以挂起读取线程。当有新的消息到达时，挂起的线程将被唤醒以接收并处理消息。消息队列是一种异步的通信方式。
   
   如下图所示，线程或中断服务例程可以将一条或多条消息放入消息队列中。同样，一个或多个线程也可以从消息队列中获得消息。当有多个消息发送到消息队列时，通常将先进入消息队列的消息先传给线程，也就是说，线程先得到的是最先进入消息队列的消息，即先进先出原则 (FIFO)。
   
   ![消息队列工作示意图](https://www.rt-thread.org/document/site/rt-thread-version/rt-thread-standard/programming-manual/ipc2/figures/07msg_work.png)
   
   ### [消息队列控制块](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc2/ipc2?id=消息队列控制块)
   
   ```C
   struct rt_messagequeue
   {
       struct rt_ipc_object parent;
   
       void* msg_pool;                     /* 指向存放消息的缓冲区的指针 */
   
       rt_uint16_t msg_size;               /* 每个消息的长度 */
       rt_uint16_t max_msgs;               /* 最大能够容纳的消息数 */
   
       rt_uint16_t entry;                  /* 队列中已有的消息数 */
   
       void* msg_queue_head;               /* 消息链表头 */
       void* msg_queue_tail;               /* 消息链表尾 */
       void* msg_queue_free;               /* 空闲消息链表 */
   
       rt_list_t suspend_sender_thread;    /* 发送线程的挂起等待队列 */
   };
   typedef struct rt_messagequeue* rt_mq_t;
   
   ```
   
   
   
   ### [消息队列的管理方式](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc2/ipc2?id=消息队列的管理方式)
   
   ![image-20220720231113540](C:\Users\刘佩羽\AppData\Roaming\Typora\typora-user-images\image-20220720231113540.png)
   
   #### [创建和删除消息队列](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc2/ipc2?id=创建和删除消息队列)
   
   消息队列在使用前，应该被创建出来，或对已有的静态消息队列对象进行初始化，创建消息队列的函数接口如下所示：
   
   ```C
   rt_mq_t rt_mq_create(const char* name, rt_size_t msg_size,
               rt_size_t max_msgs, rt_uint8_t flag);
   
   ```
   
   
   
   #### [发送紧急消息](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc2/ipc2?id=发送紧急消息)
   
   发送紧急消息的过程与发送消息几乎一样，唯一的不同是，当发送紧急消息时，从空闲消息链表上取下来的消息块不是挂到消息队列的队尾，而是挂到队首，这样，接收者就能够优先接收到紧急消息，从而及时进行消息处理。发送紧急消息的函数接口如下：
   
   ```c
   rt_err_t rt_mq_urgent(rt_mq_t mq, void* buffer, rt_size_t size);
   
   ```
   
   ### [消息队列应用示例](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/ipc2/ipc2?id=消息队列应用示例)
   
7. # [内存管理](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/memory/memory?id=内存管理)

   计算机系统中，变量、中间数据一般存放在 RAM 中，只有在实际使用时才将它们从 RAM 调入到 CPU 中进行运算。一些数据需要的内存大小需要在程序运行过程中根据实际情况确定，这就要求系统具有对内存空间进行动态管理的能力，在用户需要一段内存空间时，向系统申请，系统选择一段合适的内存空间分配给用户，用户使用完毕后，再释放回系统，以便系统将该段内存空间回收再利用。

   ![image-20220720233912811](C:\Users\刘佩羽\AppData\Roaming\Typora\typora-user-images\image-20220720233912811.png)


## [内存管理的功能特点](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/memory/memory?id=内存管理的功能特点)

1）分配内存的时间必须是确定的。

2）随着内存不断被分配和释放，整个内存区域会产生越来越多的碎片（因为在使用过程中，申请了一些内存，其中一些释放了，导致内存空间中存在一些小的内存块，它们地址不连续，不能够作为一整块的大内存分配出去），系统中还有足够的空闲内存，但因为它们地址并非连续，不能组成一块连续的完整内存块，会使得程序不能申请到大的内存。

![image-20220721201127352](C:\Users\刘佩羽\Desktop\GIT教学\summer2022\liupeiyu\第2天\学习笔记\image-20220721201127352-16584054935881.png)

3）嵌入式系统的资源环境也是不尽相同,如何为这些不同的系统，选择适合它们的高效率的内存分配算法，就将变得复杂化。

## [RT-Thread 程序内存分布](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/basic/basic?id=rt-thread-程序内存分布)

一般 MCU 包含的存储空间有：片内 Flash 与片内 RAM，RAM 相当于内存，Flash 相当于硬盘。编译器会将一个程序分类为好几个部分，分别存储在 MCU 不同的存储区。

``` c
linking...
Program Size: Code=48008 RO-data=5660 RW-data=604 ZI-data=2124
After Build - User command \#1: fromelf --bin.\\build\\rtthread-stm32.axf--output rtthread.bin
".\\build\\rtthread-stm32.axf" - 0 Error(s), 0 Warning(s).
Build Time Elapsed: 00:00:07

```

上面提到的 Program Size 包含以下几个部分：

1）Code：代码段，存放程序的代码部分；

2）RO-data：只读数据段，存放程序中定义的常量；

3）RW-data：读写数据段，存放初始化为非 0 值的全局变量；

4）ZI-data：0 数据段，存放未初始化的全局变量及初始化为 0 的变量；

编译完工程会生成一个. map 的文件，该文件说明了各个函数占用的尺寸和地址，在文件的最后几行也说明了上面几个字段的关系：

```c
Total RO Size (Code + RO Data) 53668 ( 52.41kB)
Total RW Size (RW Data + ZI Data) 2728 ( 2.66kB)
Total ROM Size (Code + RO Data + RW Data) 53780 ( 52.52kB)

```

1）RO Size 包含了 Code 及 RO-data，表示程序占用 Flash 空间的大小；

2）RW Size 包含了 RW-data 及 ZI-data，表示运行时占用的 RAM 的大小；

3）ROM Size 包含了 Code、RO-data 以及 RW-data，表示烧写程序所占用的 Flash 空间的大小；

程序运行之前，需要有文件实体被烧录到 STM32 的 Flash 中，一般是 bin 或者 hex 文件，该被烧录文件称为可执行映像文件。如下图左边部分所示，是可执行映像文件烧录到 STM32 后的内存分布，它包含 RO 段和 RW 段两个部分：其中 RO 段中保存了 Code、RO-data 的数据，RW 段保存了 RW-data 的数据，由于 ZI-data 都是 0，所以未包含在映像文件中。

STM32 在上电启动之后默认从 Flash 启动，启动之后会将 RW 段中的 RW-data（初始化的全局变量）搬运到 RAM 中，但不会搬运 RO 段，即 CPU 的执行代码从 Flash 中读取，另外根据编译器给出的 ZI 地址和大小分配出 ZI 段，并将这块 RAM 区域清零。

![image-20220721202154784](C:\Users\刘佩羽\Desktop\GIT教学\summer2022\liupeiyu\第2天\学习笔记\image-20220721202154784.png)

其中动态内存堆为未使用的 RAM 空间，应用程序申请和释放的内存块都来自该空间。

而一些全局变量则是存放于 RW 段和 ZI 段中，RW 段存放的是具有初始值的全局变量（而常量形式的全局变量则放置在 RO 段中，是只读属性的），ZI 段存放的系统未初始化的全局变量，如下面的例子：

```c
#include <rtthread.h>

const static rt_uint32_t sensor_enable = 0x000000FE;
rt_uint32_t sensor_value;
rt_bool_t sensor_inited = RT_FALSE;
```

上述代码中sensor_enable为RO段，sensor_value为在ZI段，sensor_inited为RW段。



## [内存堆管理](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/memory/memory?id=内存堆管理)

RT-Thread 将 “ZI 段结尾处” 到内存尾部的空间用作内存堆。

![](C:\Users\刘佩羽\Desktop\GIT教学\summer2022\liupeiyu\第2天\学习笔记\image-20220721200634962.png)



实时操作系统与裸机开发最大区别：实时操作系统任务切换之间一般都有上下文，能够实现多线程。