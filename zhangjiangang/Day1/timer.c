/* generated main source file - do not edit */
#include "hal_data.h"
#include <rtdbg.h>
#include <rtthread.h>
#include <rtdevice.h>

/*开启一个周期定时器，每500个tick后打印当前cnt的值并且cnt自加1


#define LED3_PIN    BSP_IO_PORT_01_PIN_06
/* 定时器的控制块 */
static rt_timer_t timer1;
static int cnt = 0;

/* 定时器 1 超时函数 */
static void timeout1(void *parameter)
{
    rt_kprintf("periodic timer is timeout %d\n", cnt);
    cnt++;
}

  int main(void) {

      /* 创建定时器 1  周期定时器 */
      timer1 = rt_timer_create("timer1", timeout1,
                               RT_NULL, 500,
                               RT_TIMER_FLAG_PERIODIC);

      /* 启动定时器 1 */
      if (timer1 != RT_NULL) rt_timer_start(timer1);
    //   hal_entry();
       return 0;
            }