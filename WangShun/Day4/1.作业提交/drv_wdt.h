/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-09-09     WCH        the first version
 */
#ifndef DRV_WDT_H__
#define DRV_WDT_H__
#include "hc32_ll.h"

#define EXAMPLE_PERIPH_WE               (LL_PERIPH_GPIO | LL_PERIPH_EFM | LL_PERIPH_FCG | \
                                         LL_PERIPH_PWC_CLK_RMU | LL_PERIPH_SRAM)
#define EXAMPLE_PERIPH_WP               (LL_PERIPH_EFM | LL_PERIPH_FCG | LL_PERIPH_SRAM)

/* WDT count period definition */
#define WDT_COUNT_PERIOD                (16384U)
void WDT_Config(void);
typedef struct
{
  uint32_t Prescaler;
  uint32_t Reload;
} IWDG_InitTypeDef;

typedef struct
{
  CM_WDT_TypeDef        *Instance;
  IWDG_InitTypeDef      Init;
} IWDG_HandleTypeDef;

struct hc32_wdt_obj
{
    rt_watchdog_t      watchdog;
    IWDG_HandleTypeDef hiwdg ;
    rt_uint16_t        is_start;
};


#endif /* RT_USING_WDT */
