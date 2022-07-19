## HOMEWORK:

//ststic
ALIGN(8)
struct rt_thread static_thread;
rt_uint8_t static_stack[2048];
void timeout1(void){
       rt_kprintf("hey hey hey\n");
}
void thread_static(void)
{
   static rt_timer_t timer1;
   timer1 = rt_timer_create("timer1", timeout1,RT_NULL , 1000, RT_TIMER_FLAG_PERIODIC);
   if(timer1 != RT_NULL){
           rt_timer_start(timer1);
       }else{
           rt_kprintf("failed！");
       }
   }

void thread_test(void)
{
    //init thread
    rt_thread_init(&static_thread, "static", thread_static, RT_NULL, &static_stack[0], 2048, 16, 500);
    //startup thread
    rt_thread_startup(&static_thread);
}
MSH_CMD_EXPORT(thread_test,ceshi);

![](./figure/daimaRUNNING.jpg)

![](./figure/zuoye2.jpg)

![](./figure/zuoye3.jpg)

![](./figure/zuoye4.jpg)