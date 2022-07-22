# 1.HC32F460 WDT看门狗
`HC32F460的看门狗`在HC32的板子上并未完成设备框架与设备管理层的对接，所以对于HC32这部分就不能直接在Studio中配置，只能自己完成匹配，沁恒的板子有看门狗所以仿照沁恒的代码来写HC32的，虽然最后成功注册了看门狗设备，但是设备管理层的函数不知道什么原因并未运行起来，所以最后还是调用了看门狗驱动框架层的函数来实现看门狗的初始化，头有点大，先解决下面的问题再来解决这个问题。
#
1.注册看门狗
![image](https://github.com/Yaochenger/summer2022/blob/main/WangShun/Day4/3.picture/1.HC32F460%E6%B3%A8%E5%86%8C%E7%9C%8B%E9%97%A8%E7%8B%97.jpg)
#移植的CH32V307的看门狗代码

 1.1未及时喂狗
![image](https://github.com/Yaochenger/summer2022/blob/main/WangShun/Day4/3.picture/2.%E6%9C%AA%E5%8F%8A%E6%97%B6%E5%96%82%E7%8B%97%E7%B3%BB%E7%BB%9F%E5%A4%8D%E4%BD%8D.jpg)

1.2
`drv_wdt.c文件`
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

1.3
`drv_wdt.h文件`
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
# 2.使用RTC
![image](https://github.com/Yaochenger/summer2022/blob/main/WangShun/Day4/3.picture/3.%E9%A9%B1%E5%8A%A8%E8%BD%AF%E4%BB%B6%E6%A8%A1%E6%8B%9FRTC.jpg)
# 3.将HC32F460k_printf默认的终端串口4改为串口1，外接USB转串口查看
![image](https://github.com/Yaochenger/summer2022/blob/main/WangShun/Day4/3.picture/4.%E5%B0%86HC32%20shell%E7%9A%84%E9%BB%98%E8%AE%A4%E4%B8%B2%E5%8F%A34%E6%8D%A2%E5%88%B0%E4%B8%B2%E5%8F%A31.jpg)
# 4.梳理串口驱动
  此部分未整理为文字，整理后上传，未完待续。。。





