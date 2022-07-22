/*
 * Copyright (c) 2021 hpmicro
 *
 * Change Logs:
 * Date         Author          Notes
 * 2021-08-13   Fan YANG        first version
 *
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "rtt_board.h"


/*
 * 逻辑就是: 两个定时器, 定时器1在超时函数里给线程1发送一个事件, 线程1接收到事件就释放一个信号量.
 *                  .....2.............2..............2........获取........
 */

// 当多个任务都是用rt_kprintf时, 要使用互斥量保护起来.


// led thread
#define LED_TH_STACK_SIZE 1024
#define LED_TH_PROI       5
#define LED_TH_TIME_SLICE 10
rt_thread_t led_thread = RT_NULL;
void thread_entry(void *arg);


// thread 1
#define TH1_STACK_SIZE 1024
#define TH1_PROI       5
#define TH1_TIME_SLICE 10
rt_thread_t thread1_handle = RT_NULL;
void thread1_entry(void* args);


// thread 2
#define TH2_STACK_SIZE 1024
#define TH2_PROI       1
#define TH2_TIME_SLICE 20
rt_thread_t thread2_handle = RT_NULL;
void thread2_entry(void* args);


// sem1
#define INIT_RESOURSE 0
rt_sem_t sem1_handle = RT_NULL;


// eventgroup
#define EVENT1 (1<<3)
#define EVENT2 (2<<3)
rt_event_t event_handle = RT_NULL;


// timer1
#define TIMER1_PERIOD 3000
rt_timer_t timer1_handle = RT_NULL;
void timer1_entry(void* args);


// timer2
#define TIMER2_PERIOD 6000
rt_timer_t timer2_handle = RT_NULL;
void timer2_entry(void* args);


// mutex
rt_mutex_t mutex_handle = RT_NULL;


int main(void)
{
    app_init_led_pins();

    // create sem 1
    sem1_handle = rt_sem_create("sem1", INIT_RESOURSE, RT_IPC_FLAG_FIFO); // 初始信号量个数为0; 先进先唤醒
    if(sem1_handle != RT_NULL)
    {
        rt_kprintf("sem 1 create successful!\n");
    }


    // create eventgroup
    event_handle = rt_event_create("eventgroup", RT_IPC_FLAG_FIFO);//当多个线程阻塞在同一个线程上时, 按先后顺序唤醒, 不按优先级
    if(event_handle != RT_NULL)
    {
        rt_kprintf("eventgroup create successful!\n");
    }


    // create timer 1
    timer1_handle = rt_timer_create("timer1", timer1_entry, RT_NULL, TIMER1_PERIOD, RT_TIMER_FLAG_PERIODIC);
    if(timer1_handle != RT_NULL)
    {
        rt_kprintf("timer 1 create successful!\n");
        rt_timer_start(timer1_handle);
    }


    // create mutex
    mutex_handle = rt_mutex_create("mutex", RT_IPC_FLAG_PRIO);
    if(mutex_handle != RT_NULL)
    {
        rt_kprintf("mutex create successful!\n");
    }


    // create timer 2
    timer2_handle = rt_timer_create("timer2", timer2_entry, RT_NULL, TIMER2_PERIOD, RT_TIMER_FLAG_PERIODIC);
    if(timer2_handle != RT_NULL)
    {
        rt_kprintf("timer 2 create successful!\n");
        rt_timer_start(timer2_handle);
    }


    // create led thread
    led_thread = rt_thread_create("led_th", thread_entry, RT_NULL, LED_TH_STACK_SIZE, LED_TH_PROI, LED_TH_TIME_SLICE);
    rt_thread_startup(led_thread);


    // create thread 1
    thread1_handle = rt_thread_create("thread1", thread1_entry, RT_NULL, 1024, 1, 20);
    if(thread1_handle != RT_NULL)
    {
        rt_kprintf("thread1 create successful!\n");
        rt_thread_startup(thread1_handle);
    }


    // create thread 2
    thread2_handle = rt_thread_create("thread2", thread2_entry, RT_NULL, 1024, 2, 10);
    if(thread2_handle != RT_NULL)
    {
        rt_kprintf("thread2 create successful!\n\n");
        rt_thread_startup(thread2_handle);
    }

    return 0;
}


void timer1_entry(void* args)
{
    //发送一个事件给线程1, 让它释放一个信号量
    rt_event_send(event_handle, EVENT1);

    rt_mutex_take(mutex_handle, RT_WAITING_FOREVER);
    rt_kprintf("event -> thread 1!\n");
    rt_mutex_release(mutex_handle);
}


void timer2_entry(void* args)
{
    //发送一个事件给线程2, 让它获取一个信号量
    rt_event_send(event_handle, EVENT2);

    rt_mutex_take(mutex_handle, RT_WAITING_FOREVER);
    rt_kprintf("event -> thread 2!\n");
    rt_mutex_release(mutex_handle);
}


void thread_entry(void *arg)
{
    while(1){
        app_led_write(0, APP_LED_ON);
        rt_thread_mdelay(500);
        app_led_write(0, APP_LED_OFF);
        rt_thread_mdelay(500);
        app_led_write(1, APP_LED_ON);
        rt_thread_mdelay(500);
        app_led_write(1, APP_LED_OFF);
        rt_thread_mdelay(500);
        app_led_write(2, APP_LED_ON);
        rt_thread_mdelay(500);
        app_led_write(2, APP_LED_OFF);
        rt_thread_mdelay(500);
    }
}


void thread1_entry(void* args)
{
    rt_uint32_t received_event = 0;
    rt_err_t ret = 0;

    for(;;)
    {
        rt_event_recv(event_handle, EVENT1, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &received_event);
        ret = rt_sem_release(sem1_handle);
        if(ret == RT_EOK)
        {
            rt_mutex_take(mutex_handle, RT_WAITING_FOREVER);
            rt_kprintf("release sem successful...\n\n");
            rt_mutex_release(mutex_handle);
        }
        else
        {
            rt_mutex_take(mutex_handle, RT_WAITING_FOREVER);
            rt_kprintf("release sem failed...\n\n");
            rt_mutex_release(mutex_handle);
        }
    }
}


void thread2_entry(void* args)
{
    rt_uint32_t received_event = 0;

    for(;;)
    {
        rt_event_recv(event_handle, EVENT2, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &received_event);
        rt_sem_take(sem1_handle, RT_WAITING_FOREVER);

        rt_mutex_take(mutex_handle, RT_WAITING_FOREVER);
        rt_kprintf("take sem successful...\n\n");
        rt_mutex_release(mutex_handle);
    }
}


