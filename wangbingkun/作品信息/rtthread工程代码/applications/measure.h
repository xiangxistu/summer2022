  /*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-08-01     10091       the first version
 */
#ifndef APPLICATIONS_MEASURE_H_
#define APPLICATIONS_MEASURE_H_

#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"
#include "sensor.h"
#include "sensor_dallas_dht11.h"
#include "drv_gpio.h"
#include "onenet.h"


#define DHT11_DATA_PIN   38

int dht11_read_temp_sample(void);

#endif /* APPLICATIONS_MEASURE_H_ */
