# RTT启动流程
![](2022-07-20-11-33-05.png)

# 线程管理
**线程控制块**  
线程控制块是操作系统用于管理线程的一个数据结构，它会存放线程的一些信息，例如优先级、线程名称、线程状态等，也包含线程与线程之间连接用的链表结构，线程等待事件集合等。

**线程栈**  
RT-Thread 线程具有独立的栈，当进行线程切换时，会将当前线程的上下文存在栈中，当线程要恢复运行时，再从栈中读取上下文信息，进行恢复；同时，线程栈还用来存放函数中的局部变量。

**线程优先级**  
RT-Thread 最大支持 256 个线程优先级 (0~255)，数值越小的优先级越高，0 为最高优先级。

**空闲线程**  
空闲线程是系统创建的最低优先级的线程，线程状态永远为就绪态，当系统中无其他就绪线程存在时，调度器将调度到空闲线程，它通常是一个死循环，且永远不能被挂起。  
**作用**有回收被删除线程的资源，也提供了接口来运行用户设置的钩子函数，在空闲线程运行时会调用该钩子函数，适合钩入功耗管理、看门狗喂狗等工作。  

**线程管理**  
![](2022-07-20-10-56-12.png)
*线程相关操作*

**动态线程与静态线程的区别**是：动态线程是系统自动从动态内存堆上分配栈空间与线程句柄（初始化 heap 之后才能使用 create 创建动态线程），静态线程是由用户分配栈空间与线程句柄。

**创建静态线程**  
```C
static struct rt_thread static_thread;//静态线程控制块
rt_uint8_t static_stack[2048];//静态线程栈

void thread_dynamic(void)
{
    while(1)
    {
        rt_kprintf("2");

    }
}

void thread_test(void)
{
    rt_thread_init(&static_thread,"static",thread_static,RT_NULL,&static_stack[0],2048,15,100);

    rt_thread_startup(&thread_static);

}
```

**创建动态线程**  
```c
rt_thread_t dynamic_thread = RT_NULL;

void thread_static(void)
{
    while(1)
    {
        rt_kprintf("1");

    }
}

void thread_test(void)
{
    rt_thread_t dynamic_thread = RT_NULL;

    dynamic_thread=rt_thread_create("dynamic", thread_dynamic, RT_NULL, 2048, 16, 500);

    rt_thread_startup(dynamic_thread);
}
```

**例程**  
创建一动态线程一个静态线程，并且设置优先级不一样，再通过msh命令控制打印输出1和2  
```c
ALIGN(8)//对齐
static struct rt_thread static_thread;//线程句柄指针
rt_uint8_t static_stack[2048];//空间
rt_thread_t dynamic_thread = RT_NULL;
void thread1(void)//线程1
{
    while(1)
    {

       rt_kprintf("1\r\n");
       rt_thread_mdelay(100);
    }
}
void thread2(void)//线程2
{
    while(1)
    {

       rt_kprintf("2\r\n");
       rt_thread_mdelay(400);
    }
}
void thread_test(void)
{
    /*静态线程，优先级16*/
  rt_thread_init(&static_thread,"thread1",thread1,RT_NULL,&static_stack[0],2048,16,100);
  rt_thread_startup(&static_thread);
    /*动态线程，优先级15*/
  dynamic_thread = rt_thread_create("thread2",thread2,RT_NULL,2048,15,500);
  rt_thread_startup(dynamic_thread);
}
MSH_CMD_EXPORT(thread_test,static thread / dynamic thread test sample);
```
实验现象如下图所示：
![](2022-07-20-11-30-16.png)