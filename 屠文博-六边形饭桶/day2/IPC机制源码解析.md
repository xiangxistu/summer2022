# IPC



## 1.信号量

### 1.1用法

```c
// 动态创建信号量, 参数1是名字, 
// 参数2是初始资源个数, 我用了个宏, 
// 参数3是当要唤醒阻塞在这个信号量上的线程时, 
// 是按获取信号量的顺序唤醒RT_IPC_FLAG_FIFO, 还是按线程的优先级唤醒RT_IPC_FLAG_PRIO.
rt_sem_t rt_sem_create("sem1", INIT_RESOURSE, RT_IPC_FLAG_FIFO);


// 释放信号量, 使资源计数值加1,
// 参数是信号量的句柄.
// 如果释放成功则返回RT_EOK, 否则返回其它值.
ret = rt_sem_release(sem1_handle);


// 获取信号量, 使资源计数值减1,
// 参数1是信号量句柄, 参数2是等待时长. 一般都是设为永远等待RT_WAITING_FOREVER.
// 当然第二个参数也可以指定一个值, 让它等不到的时候, 阻塞指定时长. 它是个 rt_int32_t 类型.
rt_sem_take(sem1_handle, RT_WAITING_FOREVER);
```



### 1.2实现分析

#### 1.2.1信号量控制块:

```c
// 信号量结构体
struct rt_semaphore
{
    struct rt_ipc_object parent;      // 继承基本的IPC对象
    rt_uint16_t          value;       // 资源计数值
    rt_uint16_t          reserved;    // 保留字段
};
typedef struct rt_semaphore *rt_sem_t;// 信号量句柄


// 进一步的 ----->>> 基本的IPC对象
struct rt_ipc_object
{
    struct rt_object parent;          // RTT中的基本内核对象
    rt_list_t suspend_thread;         // 记录挂在本IPC对象上的线程的链表.
};


// 进一步的 ----->>> 基本的内核对象
// 这就只是一些对象所需的基本信息
struct rt_object
{
    char name[RT_NAME_MAX];          // 对象名字, RT_NAME_MAX 默认为8
    rt_uint8_t type;                 // 存储它是啥对象的, 要区分不同类型的实例啊
    rt_uint8_t flag;                 // 标志位, 比如是按FIFO, 还是PROI, 决定了链表的排队方式
    rt_list_t  list;                 // 这应该是侵入式链表的挂载点
};
typedef struct rt_object *rt_object_t;


// 进一步的 ----->>> 侵入式双向链表节点
// 这就是RTT的双向链表基础结构, 全仰仗它了.
struct rt_list_node
{
    struct rt_list_node *next;
    struct rt_list_node *prev;
};
typedef struct rt_list_node rt_list_t; 

```



#### 1.2.2信号量动态创建函数:

```c
// value就是你要赋给信号量控制块里value成员的值, 用来指定初始时有多少个资源.
// flag指定唤醒任务的方式, 它赋给了控制块里的flag成员, 是从基础内核对象中继承过来的.
// flag可选RT_IPC_FLAG_PRIO和RT_IPC_FLAG_FIFO, 人家也提示了, 说按FIFO排队的话, 就不是精致的real-time了.
rt_sem_t rt_sem_create(const char *name, rt_uint32_t value, rt_uint8_t flag)
{
    rt_sem_t sem;

    RT_DEBUG_NOT_IN_INTERRUPT;
    RT_ASSERT(value < 0x10000U);

    sem = (rt_sem_t)rt_object_allocate(RT_Object_Class_Semaphore, name);// 分配内存
    if (sem == RT_NULL)
        return sem;// 分配失败就返回

    _ipc_object_init(&(sem->parent));// 初始化IPC对象, 这里其实啥也没干, 就只是把链表设置为空

    sem->value = value;// 设定资源初始值
    sem->parent.parent.flag = flag;// 设置了flag

    return sem;
}
```



#### 1.2.3信号量释放函数:

```c
rt_err_t rt_sem_release(rt_sem_t sem)
{
    register rt_base_t temp;
    register rt_bool_t need_schedule;// 决定是否需要调度, 因为可能会唤醒线程

    RT_ASSERT(sem != RT_NULL);
    RT_ASSERT(rt_object_get_type(&sem->parent.parent) == RT_Object_Class_Semaphore);

    RT_OBJECT_HOOK_CALL(rt_object_put_hook, (&(sem->parent.parent)));

    // 默认不用调度
    need_schedule = RT_FALSE;
    
    // 关硬件中断, 应该是防止被打断吧
    temp = rt_hw_interrupt_disable();

    RT_DEBUG_LOG(RT_DEBUG_IPC, ("thread %s releases sem:%s, which value is: %d\n",
                                rt_thread_self()->name,
                                ((struct rt_object *)sem)->name,
                                sem->value));

    // 判断挂任务的链表是否为空, 实现其实很简单, 就一条语句
    if (!rt_list_isempty(&sem->parent.suspend_thread))
    {
        // 有挂起的任务, 因为你释放了一个, 所以要解挂一个任务. 
        // 解挂一个确定位置的线程, 这个位置上是哪个还要去看挂起线程的函数那里, 它会按flag的方式排好序的.
        _ipc_list_resume(&(sem->parent.suspend_thread));
        
        // 标记一下需要调度, 因为这个新解挂的线程的优先级可能比现在正在执行的线程的优先级还高
        need_schedule = RT_TRUE;
    }
    else// 没有挂起的任务
    {
        // 现在资源数比允许的总数要小, RT_SEM_VALUE_MAX默认为0xffff
        if(sem->value < RT_SEM_VALUE_MAX)
        {
            sem->value ++;// 看到了吧, 这里就是把那个控制块里的资源计数值加1
        }
        else
        {
            // 现在的资源数已经超过允许值了, 返回错误
            rt_hw_interrupt_enable(temp);
            return -RT_EFULL;
        }
    }

    // 开硬件中断, 因为重要任务执行完毕了
    rt_hw_interrupt_enable(temp);

    // 如果需要调度的话就调度一下
    if (need_schedule == RT_TRUE)
        rt_schedule();

    return RT_EOK;
}
```



#### 1.2.4信号量获取函数

```c
// 参数2就是指定允许等待的时间, 如果等了那么长时间还每到那就不管了, 直接不等了.
// 但是人家也说了, 我们一般都是死等信号量, 而且约定使用RT_WAITING_FOREVER来表示我要死等.
rt_err_t rt_sem_take(rt_sem_t sem, rt_int32_t time)
{
    register rt_base_t temp;
    struct rt_thread *thread;

    RT_ASSERT(sem != RT_NULL);
    RT_ASSERT(rt_object_get_type(&sem->parent.parent) == RT_Object_Class_Semaphore);

    RT_OBJECT_HOOK_CALL(rt_object_trytake_hook, (&(sem->parent.parent)));

    // 关中断, 因为下面的事情它不希望被打断
    temp = rt_hw_interrupt_disable();

    RT_DEBUG_LOG(RT_DEBUG_IPC, ("thread %s take sem:%s, which value is: %d\n",
                                rt_thread_self()->name,
                                ((struct rt_object *)sem)->name,
                                sem->value));

    // 如果资源计数值大于0, 那就很简单, 直接资源计数值减1就得了, 就退出函数了.
    if (sem->value > 0)
    {
        // 控制块中的资源计数值减1
        sem->value --;
		
        // 退出前别忘了开中断, 小子!
        rt_hw_interrupt_enable(temp);
    }
    else // 如果资源计数值小于0, 那就事大了, 当前正在运行的这个线程就要阻塞了, 因为你肯定是在一个线程里调用的这个函数啊
    {
		// 如果你并不想等待, 那也好办, 返回错误就得了
        if (time == 0)
        {
            // 同样别忘了开中断
            rt_hw_interrupt_enable(temp);
            return -RT_ETIMEOUT;
        }
        else // 如果我还想等一会
        {
            /* current context checking */
            // 不知道这个有啥用, 它就是一个空的宏标识符
            RT_DEBUG_IN_THREAD_CONTEXT;

            // 系统中有一个变量专门用来指向当前运行的线程的, 
            // 这个函数就是返回的这个指针变量, 获得当前正在执行的任务, 把它挂起
            thread = rt_thread_self();

            /* reset thread error number */
            thread->error = RT_EOK;

            RT_DEBUG_LOG(RT_DEBUG_IPC, ("sem take: suspend thread - %s\n",
                                        thread->name));

            // 把当前正在执行的线程挂到这个IPC的控制块中的链表中
            _ipc_list_suspend(&(sem->parent.suspend_thread),
                                thread,
                                sem->parent.parent.flag);

            // 如果你还想等一会
            if (time > 0)
            {
                RT_DEBUG_LOG(RT_DEBUG_IPC, ("set thread:%s to timer list\n",
                                            thread->name));

                // 线程控制块中有一个计数值, 专门用来阻塞一定时长的, 这里就是设定一下阻塞时长, 定时唤醒
                rt_timer_control(&(thread->thread_timer),
                                 RT_TIMER_CTRL_SET_TIME,
                                 &time);
                rt_timer_start(&(thread->thread_timer));
            }

            // 开中断
            rt_hw_interrupt_enable(temp);

            // 因为你挂起了一个任务, 所以要调度一下
            rt_schedule();

            if (thread->error != RT_EOK)
            {
                return thread->error;
            }
        }
    }

    RT_OBJECT_HOOK_CALL(rt_object_take_hook, (&(sem->parent.parent)));

    return RT_EOK;
}
```





## 2.互斥量

#### 2.1互斥量控制块

```c
struct rt_mutex
{
    struct rt_ipc_object parent;             // 跟信号量一样也是继承自基础的IPC对象

    rt_uint16_t          value;              // 互斥量的值, 只能为0和1啊, 0表示可以以上锁, 1表示开锁

    rt_uint8_t           original_priority;  // 应该是用于优先级继承机制的
    rt_uint8_t           hold;                          /**< numbers of thread hold the mutex */

    struct rt_thread    *owner;              // 记录这个互斥量是属于那个线程的, 也就是当前被谁获取了
};
typedef struct rt_mutex *rt_mutex_t;
```



#### 2.2互斥量动态创建函数

```c
// 参数2跟信号量的时候不一样, 它只能取RT_IPC_FLAG_PRIO.
// 使用RT_IPC_FLAG_FIFO的话会严重影响到系统的实时性.因为那会导致高优先级的任务等待的时间过于长.
rt_mutex_t rt_mutex_create(const char *name, rt_uint8_t flag)
{
    struct rt_mutex *mutex;
    (void)flag; // 你看这里它就没用这个参数, 因为一定是RT_IPC_FLAG_PRIO. 前面写个void是防止编译器爆警告.

    RT_DEBUG_NOT_IN_INTERRUPT;

    mutex = (rt_mutex_t)rt_object_allocate(RT_Object_Class_Mutex, name);
    if (mutex == RT_NULL)
        return mutex;

    // 初始化链表, 也就是它里面的阻塞链表置空, 很简单, 就一条语句
    _ipc_object_init(&(mutex->parent));

    mutex->value              = 1;      // 初始为1, 表示开锁
    mutex->owner              = RT_NULL;// 现在它谁也不属于
    mutex->original_priority  = 0xFF;
    mutex->hold               = 0;

    // 强行指定唤醒方式, 它说使用FIFO无法解决优先级翻转的问题, 我不理解为啥...
    mutex->parent.parent.flag = RT_IPC_FLAG_PRIO;

    return mutex;
}
```



#### 2.3上锁函数

```c
rt_err_t rt_mutex_take(rt_mutex_t mutex, rt_int32_t time)
{
    register rt_base_t temp;
    struct rt_thread *thread;

    /* this function must not be used in interrupt even if time = 0 */
    RT_DEBUG_IN_THREAD_CONTEXT;

    /* parameter check */
    RT_ASSERT(mutex != RT_NULL);
    RT_ASSERT(rt_object_get_type(&mutex->parent.parent) == RT_Object_Class_Mutex);

    /* get current thread */
    thread = rt_thread_self();

    /* disable interrupt */
    temp = rt_hw_interrupt_disable();

    RT_OBJECT_HOOK_CALL(rt_object_trytake_hook, (&(mutex->parent.parent)));

    RT_DEBUG_LOG(RT_DEBUG_IPC,
                 ("mutex_take: current thread %s, mutex value: %d, hold: %d\n",
                  thread->name, mutex->value, mutex->hold));

    /* reset thread error */
    thread->error = RT_EOK;

    if (mutex->owner == thread)
    {
        if(mutex->hold < RT_MUTEX_HOLD_MAX)
        {
            /* it's the same thread */
            mutex->hold ++;
        }
        else
        {
            rt_hw_interrupt_enable(temp); /* enable interrupt */
            return -RT_EFULL; /* value overflowed */
        }
    }
    else
    {
        /* The value of mutex is 1 in initial status. Therefore, if the
         * value is great than 0, it indicates the mutex is avaible.
         */
        if (mutex->value > 0)
        {
            /* mutex is available */
            mutex->value --;

            /* set mutex owner and original priority */
            mutex->owner             = thread;
            mutex->original_priority = thread->current_priority;
            if(mutex->hold < RT_MUTEX_HOLD_MAX)
            {
                mutex->hold ++;
            }
            else
            {
                rt_hw_interrupt_enable(temp); /* enable interrupt */
                return -RT_EFULL; /* value overflowed */
            }
        }
        else
        {
            /* no waiting, return with timeout */
            if (time == 0)
            {
                /* set error as timeout */
                thread->error = -RT_ETIMEOUT;

                /* enable interrupt */
                rt_hw_interrupt_enable(temp);

                return -RT_ETIMEOUT;
            }
            else
            {
                /* mutex is unavailable, push to suspend list */
                RT_DEBUG_LOG(RT_DEBUG_IPC, ("mutex_take: suspend thread: %s\n",
                                            thread->name));

                /* change the owner thread priority of mutex */
                if (thread->current_priority < mutex->owner->current_priority)
                {
                    /* change the owner thread priority */
                    rt_thread_control(mutex->owner,
                                      RT_THREAD_CTRL_CHANGE_PRIORITY,
                                      &thread->current_priority);
                }

                /* suspend current thread */
                _ipc_list_suspend(&(mutex->parent.suspend_thread),
                                    thread,
                                    mutex->parent.parent.flag);

                /* has waiting time, start thread timer */
                if (time > 0)
                {
                    RT_DEBUG_LOG(RT_DEBUG_IPC,
                                 ("mutex_take: start the timer of thread:%s\n",
                                  thread->name));

                    /* reset the timeout of thread timer and start it */
                    rt_timer_control(&(thread->thread_timer),
                                     RT_TIMER_CTRL_SET_TIME,
                                     &time);
                    rt_timer_start(&(thread->thread_timer));
                }

                /* enable interrupt */
                rt_hw_interrupt_enable(temp);

                /* do schedule */
                rt_schedule();

                if (thread->error != RT_EOK)
                {
                    /* return error */
                    return thread->error;
                }
                else
                {
                    /* the mutex is taken successfully. */
                    /* disable interrupt */
                    temp = rt_hw_interrupt_disable();
                }
            }
        }
    }

    /* enable interrupt */
    rt_hw_interrupt_enable(temp);

    RT_OBJECT_HOOK_CALL(rt_object_take_hook, (&(mutex->parent.parent)));

    return RT_EOK;
}
```



#### 2.4解锁函数

```c
rt_err_t rt_mutex_release(rt_mutex_t mutex)
{
    register rt_base_t temp;
    struct rt_thread *thread;
    rt_bool_t need_schedule;

    /* parameter check */
    RT_ASSERT(mutex != RT_NULL);
    RT_ASSERT(rt_object_get_type(&mutex->parent.parent) == RT_Object_Class_Mutex);

    need_schedule = RT_FALSE;

    /* only thread could release mutex because we need test the ownership */
    RT_DEBUG_IN_THREAD_CONTEXT;

    /* get current thread */
    thread = rt_thread_self();

    /* disable interrupt */
    temp = rt_hw_interrupt_disable();

    RT_DEBUG_LOG(RT_DEBUG_IPC,
                 ("mutex_release:current thread %s, mutex value: %d, hold: %d\n",
                  thread->name, mutex->value, mutex->hold));

    RT_OBJECT_HOOK_CALL(rt_object_put_hook, (&(mutex->parent.parent)));

    /* mutex only can be released by owner */
    if (thread != mutex->owner)
    {
        thread->error = -RT_ERROR;

        /* enable interrupt */
        rt_hw_interrupt_enable(temp);

        return -RT_ERROR;
    }

    /* decrease hold */
    mutex->hold --;
    /* if no hold */
    if (mutex->hold == 0)
    {
        /* change the owner thread to original priority */
        if (mutex->original_priority != mutex->owner->current_priority)
        {
            rt_thread_control(mutex->owner,
                              RT_THREAD_CTRL_CHANGE_PRIORITY,
                              &(mutex->original_priority));
        }

        /* wakeup suspended thread */
        if (!rt_list_isempty(&mutex->parent.suspend_thread))
        {
            /* get suspended thread */
            thread = rt_list_entry(mutex->parent.suspend_thread.next,
                                   struct rt_thread,
                                   tlist);

            RT_DEBUG_LOG(RT_DEBUG_IPC, ("mutex_release: resume thread: %s\n",
                                        thread->name));

            /* set new owner and priority */
            mutex->owner             = thread;
            mutex->original_priority = thread->current_priority;

            if(mutex->hold < RT_MUTEX_HOLD_MAX)
            {
                mutex->hold ++;
            }
            else
            {
                rt_hw_interrupt_enable(temp); /* enable interrupt */
                return -RT_EFULL; /* value overflowed */
            }

            /* resume thread */
            _ipc_list_resume(&(mutex->parent.suspend_thread));

            need_schedule = RT_TRUE;
        }
        else
        {
            if(mutex->value < RT_MUTEX_VALUE_MAX)
            {
                /* increase value */
                mutex->value ++;
            }
            else
            {
                rt_hw_interrupt_enable(temp); /* enable interrupt */
                return -RT_EFULL; /* value overflowed */
            }

            /* clear owner */
            mutex->owner             = RT_NULL;
            mutex->original_priority = 0xff;
        }
    }

    /* enable interrupt */
    rt_hw_interrupt_enable(temp);

    /* perform a schedule */
    if (need_schedule == RT_TRUE)
        rt_schedule();

    return RT_EOK;
}
```





## 3.事件组

```c

```



## 4.邮箱



## 5.消息队列

