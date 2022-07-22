#学习心得

1. 学习了多线程，了解了轮询开发模式（传统开发模式）、前后台开发模式、抢占式多线程开发模式、时间片多线程开发模式、
   RTT线程相关概念、RTT抢占式调度机制、RTT时间片调度机制、RTT线程状态、RTT线程状态如何切换、RTT特殊的线程（空闲线程优先级最低，main线程，finish线程）

2. 线程使用流程：

   静态此线程

   1，ALIGN(RT_ALIGN_SIZE)//对齐

   2，定义线程堆，定义句柄

   3，定义入口函数

   4，定义创建线程，启动线程的函数

   5，调用输出命令函数MSH_CMD_EXPORT(semaphore_sample, semaphore sample)

3. 学习了线程小实验，通过线程输出“2”，“1”
   代码如下：

   ````c
   2. /* static */
      ALIGN(8)//静态线程必须人为定义8字节对齐 
      struct rt_thread static_thread;//句柄 
      rt_uint8_t static_stack[2048];
   
   void thread_static(void)//静态线程操作 
   {
       while(1)
       {
           rt_kprintf("1");
       }
   }
   
   void thread_dynamic(void)//动态线程操作 
   {
       while(1)
       {
           rt_kprintf("2");
       }
   }
   
   void thread_test(void)
   {
       /* init thread */
       rt_thread_init(&static_thread, //线程控制块 
   	"static",					//线程名称 
   	thread_static,				//入口函数 
   	RT_NULL,					//入口函数参数 
   	&static_stack[0],			//线程栈起始地址 
   	2048,						//线程栈大小 
   	16,							//线程优先级 
   	100							//线程时间片 
   	);
       /*  startup thread */
       rt_thread_startup(&static_thread);//启动线程，开启调度，线程进入就绪状态 
   
   
   ```C
   rt_thread_t dynamic_thread = RT_NULL;//创建线程指针，并赋值 
   
   /* create thread */
   dynamic_thread = rt_thread_create("dynamic", // //线程名称 
   								thread_dynamic, //入口函数 
   								RT_NULL, 		//入口函数参数 
   								2048, 			//线程栈大小 
   								16, 			//线程优先级
   								500				//线程时间片 
   								);
   rt_thread_startup(dynamic_thread);//启动线程，开启调度，线程进入就绪状态
   ```
   
   
   //    rt_thread_create(name, entry, parameter, stack_size, priority, tick);
   }
   //命令导出 
   MSH_CMD_EXPORT(thread_test, static thread / dynamic thread test sample);
   ````

   

问题：电脑不能正常安装USB串行设备驱动导致现在还没有看到代码效果.

A:目前问题已经解决。详情可以参考我写的CSDN文章：[https://blog.csdn.net/m0_58449549/article/details/125883703?spm=1001.2014.3001.5501]()

