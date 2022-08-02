/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-07-27     10091       the first version
 */
#ifndef APPLICATIONS_CONTROLLER_H_
#define APPLICATIONS_CONTROLLER_H_

#include <rtthread.h>
#include <rtdevice.h>
#include "my_pwm.h"

#define ENABLE_CAR_MSH 1

#define AIN1_PIN "PE8"
#define AIN2_PIN "PE9"
#define BIN1_PIN "PE10"
#define BIN2_PIN "PE11"


#define PATH_INF_MAX 10
#define PWM_PERCENT_INIT   60


enum CAR_S{
  CAR_RUN,
  CAR_STOP,
  CAR_ARRIVE,
  CAR_ERR
};

struct Controller{
    enum CAR_S car_status;       //车当前状态

    rt_uint8_t arrive_flag;      //到达FLAG
    int path_inf[PATH_INF_MAX];  //路径信息

    rt_uint8_t left_pulse;       //左占空比
    rt_uint8_t rignt_pulse;      //右占空比

    rt_err_t (*start)(int argc,char **argv);
    rt_err_t (*stop)(void);
    rt_err_t (*left)(void);
    rt_err_t (*right)(void);
    rt_err_t (*turn)(int argc,char **argv);

};

int car_init(void);
rt_err_t car_start(int argc,char **argv);
rt_err_t car_stop(void);
rt_err_t car_left(void);
rt_err_t car_right(void);
rt_err_t car_turn(int argc,char **argv);

#endif /* APPLICATIONS_CONTROLLER_H_ */
