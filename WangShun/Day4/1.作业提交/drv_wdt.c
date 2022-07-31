/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2021-09-09     WCH        the first version
 */
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "hc32_ll_wdt.h"
/*小华HC32F460的看门狗定时器*/
#ifdef RT_USING_WDT
#include "drv_wdt.h"

#define DRV_DEBUG
#define LOG_TAG             "drv.wdt"
#include <drv_log.h>

static struct hc32_wdt_obj hc32_wdt;
static struct rt_watchdog_ops ops;

/*HC32F460 */
void WDT_Config(void)
{
    LL_PERIPH_WE(EXAMPLE_PERIPH_WE);
    RMU_ClearStatus();
    stc_wdt_init_t stcWdtInit;

    /* WDT configuration */
    stcWdtInit.u32CountPeriod   = WDT_CNT_PERIOD16384;
    stcWdtInit.u32ClockDiv      = WDT_CLK_DIV512;
    stcWdtInit.u32RefreshRange  = WDT_RANGE_0TO100PCT;
    stcWdtInit.u32LPMCount      = WDT_LPM_CNT_STOP;
    stcWdtInit.u32ExceptionType = WDT_EXP_TYPE_RST;
    (void)WDT_Init(&stcWdtInit);
    WDT_FeedDog();
    LL_PERIPH_WP(EXAMPLE_PERIPH_WP);
}

static rt_err_t IWDG_Reload(IWDG_HandleTypeDef *hiwdg)
{
    hiwdg->Instance->RR = 0x0123UL;
    hiwdg->Instance->RR = 0x3210UL;
    return RT_EOK;
}

static rt_err_t IWDG_Init(IWDG_HandleTypeDef *hiwdg)
{
    WDT_Config();
    return RT_EOK;
}

static rt_err_t wdt_init(rt_watchdog_t *wdt)
{
    return RT_EOK;
}

static rt_err_t wdt_control(rt_watchdog_t *wdt, int cmd, void *arg)
{
    switch (cmd)
    {
        /* feed the watchdog */
    case RT_DEVICE_CTRL_WDT_KEEPALIVE:
        if(IWDG_Reload(&hc32_wdt.hiwdg) != RT_EOK)
        {
            LOG_E("watch dog keepalive fail.");
        }
        break;
        /* set watchdog timeout */
    case RT_DEVICE_CTRL_WDT_SET_TIMEOUT:
        LOG_E("NO set watchdog timeout.");
        break;
    case RT_DEVICE_CTRL_WDT_GET_TIMEOUT:
            (*((rt_uint32_t*)arg)) = WDT_GetCountValue();
        break;
    case RT_DEVICE_CTRL_WDT_START:
//        if (IWDG_Init(&hc32_wdt.hiwdg) != RT_EOK)
//        {
//            LOG_E("wdt start failed.");
//            return -RT_ERROR;
//        }
        WDT_Config();
        hc32_wdt.is_start = 1;
        break;
    default:
        LOG_W("This command is not supported.");
        return -RT_ERROR;
    }
    return RT_EOK;
}

int rt_wdt_init(void)
{
    hc32_wdt.hiwdg.Instance = CM_WDT;
//    ch32_wdt.hiwdg.Init.Prescaler = IWDG_Prescaler_256;
//    ch32_wdt.hiwdg.Init.Reload = 0x00000FFF;
    hc32_wdt.is_start = 0;
    ops.init = &wdt_init;
    ops.control = &wdt_control;
    hc32_wdt.watchdog.ops = &ops;
    /* register watchdog device */
    if (rt_hw_watchdog_register(&hc32_wdt.watchdog, "wdt", RT_DEVICE_FLAG_DEACTIVATE, RT_NULL) != RT_EOK)
    {
        LOG_E("wdt device register failed.");
        return -RT_ERROR;
    }
    LOG_D("wdt device register success.");
    return RT_EOK;
}
INIT_BOARD_EXPORT(rt_wdt_init);

#endif /* RT_USING_WDT */
