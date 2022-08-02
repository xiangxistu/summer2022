/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-07-27     10091       the first version
 */
#ifndef APPLICATIONS_MY_PWM_H_
#define APPLICATIONS_MY_PWM_H_

#include <rtthread.h>
#include <rtdevice.h>
#include "string.h"


#define PWM_PERIOD 1000

#define PWM1_DEV_NAME ("tim3pwm1")
#define PWM1_CH           (1)


#define PWM2_DEV_NAME ("tim3pwm2")
#define PWM2_CH           (2)





int my_pwm_init(void);
rt_err_t my_pwm_set_pulse(struct rt_device_pwm * pwm_dev,rt_uint32_t pulse_val);
rt_err_t my_pwm_enable(struct rt_device_pwm * pwm_dev);
rt_err_t my_pwm_disable(struct rt_device_pwm * pwm_dev);

#endif /* APPLICATIONS_MY_PWM_H_ */
