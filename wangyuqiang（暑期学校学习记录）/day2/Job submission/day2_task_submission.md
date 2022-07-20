## day2 task_submission

---

#### 任务一、学习五种 IPC 的使用(这里的学习依据RTT软件包的内核示例)

![image-20220720093657642](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201811423.png)

>通过老师上课了解到IPC也就是保证多线程间协助关系与数据传输的机制，也就是说出于资源的合理分配以及最大程度的利用，RTT提供了相应的五种IPC机制来协同资源的调度。

1、信号量sem

首先查看代码部分，可以看到静态创建了一个dynamic_sem对象，然后是两个线程的入口函数，然后我们在semaphore_sample函数中来学习信号量的运行机制

![image-20220720180954045](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201809104.png)

在历程中可以看到，首先动态创建了一个优先级等待方式的信号量dsem，然后是一个判断创建是否成功，此处的代码规范值得借鉴和习惯养成。

接下来可以看到使用rt_thread_init静态创建了两个线程，在线程1启动后，执行线程1的入口函数

```c
static void rt_thread1_entry(void *parameter)
{
    static rt_uint8_t count = 0;

    while (1)
    {
        if (count <= 100)
        {
            count++;
        }
        else
            return;

        /* count release semaphore every 10 counts */
        if (0 == (count % 10))
        {
            rt_kprintf("thread1 release a dynamic semaphore.\n");
            rt_sem_release(dynamic_sem);
        }
    }
}
```

在线程1的入口函数中，首先会执行信号量的第1次释放，但是由于信号量创建使用的是优先级等待方式，而此时线程2也被启动，由于线程2的优先级比线程1要高一级，因此此时线程2拿到已经被释放且在等待获取的信号量，并打印信息。

此时线程2的第一次调度结束，CPU资源转让线程1，并且执行信号量的释放...在完成10次记录后，线程1完成调度并出让资源，而线程2由于没有被及时释放且一直持有信号量，而陷入阻塞态。

```c
static void rt_thread2_entry(void *parameter)
{
    static rt_err_t result;
    static rt_uint8_t number = 0;
    while (1)
    {
        /* permanently wait for the semaphore; once obtain the semaphore, perform the number self-add operation */
        result = rt_sem_take(dynamic_sem, RT_WAITING_FOREVER);
        if (result != RT_EOK)
        {
            rt_kprintf("thread2 take a dynamic semaphore, failed.\n");
            rt_sem_delete(dynamic_sem);
            return;
        }
        else
        {
            number++;
            rt_kprintf("thread2 take a dynamic semaphore. number = %d\n", number);
        }
    }
}
```

![image-20220720201436178](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207202014244.png)

2、互斥锁（mutex）

![image-20220720100355551](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201936334.png)

> 首先需要知道的是：`互斥锁具有优先级继承的机制`，对比信号量机制，由于存在优先级反转风险，系统的调度效率可能会因此降低，甚至导致系统发生死锁。所以对比信号量机制，互斥锁能够更好执行临界资源的保护。

>优先级继承，是指在一段时间内提高某个占用共享资源的低优先级线程的优先级，使之与所有等待该共享资源的线程中最高优先级的那个线程的优先级相同，从而提高资源使用效率。当这个低优先级的线程释放该资源时，优先级再次恢复，所以，`在获得互斥量后，需要尽快释放互斥量，并且在持有互斥量的过程中，不得再更改持有互斥量现成的的优先级。`
>
>同时，互斥锁支持嵌套

下面进行内核例程的学习

在该例程中，动态创建了一个名为dmutex的互斥量，同时使用的是优先级等待方法（`在互斥量中,FIFO方法已被弃用`）。

由于线程1首先被启动，执行入口函数，此时线程1获得互斥量，再执行number1和number2的自加1后释放掉该互斥量；然后线程2被启动，进入到线程2的入口函数并获得互斥量，此时number1和number2等同，打印线程2处于互斥保护的信息，并输出此时number的数值，在执行number的自加操作后，释放掉该互斥量。

如此循环执行50次，线程2退出资源调度，线程1陷入阻塞态

![image-20220720202114507](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207202021581.png)

3、邮箱（mailbox）

![image-20220720110034445](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201100865.png)

> 从上文中可以看到，邮箱一条消息固定4个字节，也就是说邮箱并不能进行内容的传输，但是可以将数据的地址发送过去，因此资源使用较少，提高效率

下面学习邮箱API

例程中，首先静态创建了一个邮箱句柄并使用的是优先级等待方法，然后创建两个线程，在线程1的入口函数中，由于第一次邮箱初始化创建的时候传入的地址为mb_pool[0],此时线程1去尝试接收邮箱；

此时线程2获得资源调度，此时线程2尝试发送mb_str1的地址到邮箱中，此时由于时间片到，系统资源转交给线程1，线程1由于之前被线程2打断陷入阻塞态转而恢复就绪态，此时进入运行态打印相关信息；

此时线程2再次发送mb_str2的地址到邮箱，由此循环10次，线程2发送mb_str3到邮箱后退出系统调度，然后线程1执行接收脱离操作后也完成资源调度。

![image-20220720210603814](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207202106883.png)

4、消息队列（messagequeue）

![image-20220720111407881](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207202108539.png)

> 对比消息队列和邮箱：
>
> 消息队列会将发送的消息靠别一份给接收的线程；而邮箱只是将地址传递给接收的线程。
>
> 邮箱大多数用于大数据传输；而消息队列用于处理有序的数据。

此处可以注意到消息队列还支持普通消息和紧急消息的发送

![image-20220720111643993](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201116121.png)

5、事件组（event）

![image-20220720112611431](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207201126565.png)

> 事件只与线程相关，事件间相互独立:每个线程可拥有32个事件标志，采用一一个32 bit无符号整型数进行记录，每一个bit 代表一个事件;
> 
> 事件仅用于同步，不提供数据传输功能;
> 
> 事件无排队性，即多次向线程发送同一-事件(如果线程还未来得及读走)，其效果等同于只发送-一次。

---

#### 任务二、总结 IPC 能够唤醒，挂起线程的原因

---

#### 任务三、rt_malloc, rt_free, rt_mp_alloc, rt_mp_free 的使用

---

#### 扩展：

1. 线程切换的概念，libcpu 下的内容
2. 内存常见错误产生原因
