1了解RTT多线程相关概念
2线程的创建与使用
	2.1静态创建：需要提前创建好结构体和数组，即提前申请好所需空间
	rt_err_t rt_thread_init(struct rt_thread *thread,
                        const char       *name,
                        void (*entry)(void *parameter),
                        void             *parameter,
                        void             *stack_start,
                        rt_uint32_t       stack_size,
                        rt_uint8_t        priority,
                        rt_uint32_t       tick)
    2.2动态创建：需要创建句柄用来接收函数返回的句柄，即系统根据空间大小自动分配空间
    rt_thread_t rt_thread_create(const char *name,
                             void (*entry)(void *parameter),
                             void       *parameter,
                             rt_uint32_t stack_size,
                             rt_uint8_t  priority,
                             rt_uint32_t tick)
3软硬定时器的使用
    硬件定时器： 需要开启硬件定时器如systick、timx、等，需要在定时器中断中管理
    软件定时器： 超时函数只需要在线程中管理即可。
4RTT的启动流程
    4,1Resrt_Handler进入startup_enter后
    4.2先进入SystemInit函数，调用SystemClockCinflg函数执行系统时钟
    4.3再进入entry函数,调用rttthread_startup函数执行初始化操作（关闭中断，板载外设初始化，打印信息，定时器初始化，用户代码初始化，空闲任务，开始调度）
