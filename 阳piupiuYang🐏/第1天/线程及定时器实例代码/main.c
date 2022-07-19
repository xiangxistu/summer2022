/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-18     BruceOu      first implementation
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <rtdbg.h>
#include <rthw.h>
#include <board.h>
#include "thread_demo.h"
#include<errno.h>


/* defined the LED3 pin: PB5 */
#define LED3_PIN GET_PIN(B, 5)
#define LED1_PIN GET_PIN(A, 8)



int main(void)
{

   thread_test();

   return 0;
}


