```c
/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Main program body.
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/
#include "ch32v30x.h"
#include <rtthread.h>
#include <rthw.h>
#include "drivers/pin.h"
#include <board.h>
#include "easyblink.h"
#include "aht10.h"
/* Global typedef */

/* Global define */
#define AHT10_I2C_BUS_NAME          "i2c1"  /* 传感器连接的I2C总线设备名称 */
#define AHT10_ADDR                  0x38    /* 从机地址 */
#define AHT10_CALIBRATION_CMD       0xE1    /* 校准命令 */
#define AHT10_NORMAL_CMD            0xA8    /* 一般命令 */
#define AHT10_GET_DATA              0xAC    /* 获取数据命令 */


#define DATDACOMMIT_UART__BUS_NAME          "uart1"  /* 传感器连接的I2C总线设备名称 */




/* Global Variable */
ebled_t LED1 = {0};
ebled_t LED2 = {0};


float DataFrame[4] = {0};
int temperature[2];
struct rt_i2c_bus_device *i2c_bus;
struct rt_semaphore *timeout;
aht10_device_t aht10;
rt_device_t datacommit;
struct rt_memheap *memheap;


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
void AHT10PeriodRead(void *args);
void datatrans(void *args);

int main(void)
{

    rt_err_t error = RT_NULL;

    rt_thread_t dynamic_thrad = RT_NULL;
    rt_timer_t  datatranstimer = RT_NULL;

    rt_kprintf("MCU: CH32V307\n");
	rt_kprintf("SysClk: %dHz\n",SystemCoreClock);
    rt_kprintf("www.wch.cn\n");

	LED_BLINK_INIT();

    LED1 = easyblink_init_led(0,  0);
    LED2 = easyblink_init_led(1,  0);


    easyblink( LED1, -1, 400, 500);
    easyblink( LED2, -1, 250, 500);


    aht10 = aht10_init(AHT10_I2C_BUS_NAME);




   // datacommit =  rt_device_find(DATDACOMMIT_UART__BUS_NAME);

    //error =  rt_device_open(datacommit, RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_INT_TX);

   // rt_kprintf("%d",error);

    timeout = rt_sem_create ("timeout", 0, RT_IPC_FLAG_FIFO);

    datatranstimer =  rt_timer_create("datatrans",
            rt_sem_release,
            timeout,
                               100,
                               RT_TIMER_FLAG_SOFT_TIMER | RT_TIMER_CTRL_SET_PERIODIC);
    rt_timer_start  (datatranstimer);




    dynamic_thrad = rt_thread_create("AHT10func",
                                     AHT10PeriodRead,
                                     RT_NULL,
                                     2048,
                                      16,
                                     500);

    rt_thread_startup(dynamic_thrad);


}


void AHT10PeriodRead(void *args)
{
    char i = 0;


    while(1)
    {


//        *(float*)DataFrame   = aht10_read_temperature(aht10);
//        *((float*)DataFrame+1) = aht10_read_humidity(aht10);
//        *((float*)DataFrame+2) = 0x0000;
//        *((float*)DataFrame+3) = 0x807f;

        DataFrame[0] = aht10_read_humidity(aht10);
        temperature[0] = DataFrame[0]/1;
        temperature[1] = 1000*(DataFrame[0] - temperature[0] );
        //rt_kprintf("send");
     //   for( i = 0; i<16; i++)
     //   putc(datacommit, *((char*)(DataFrame +i)));
        DataFrame[1] = DataFrame[1]/1;
        rt_kprintf("%d.%d\n",temperature[0],temperature[1]);

        rt_sem_take(timeout,1000);


    }
}







```

