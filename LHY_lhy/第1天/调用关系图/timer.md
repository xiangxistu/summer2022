# rt_timer_create

### rt_object_allocate();

### _timer_init

rt_list_init();

# rt_timer_start

### _timer_remove

rt_list_remove();

### RT_OBJECT_HOOK_CALL

```
#ifdef RT_USING_TIMER_SOFT
    if (timer->parent.flag & RT_TIMER_FLAG_SOFT_TIMER)
    {
        /* insert timer to soft timer list */
        timer_list = _soft_timer_list;
    }
    else
#endif
```

### rt_list_entry

### rt_list_insert_after

```
#ifdef RT_USING_TIMER_SOFT
    if (timer->parent.flag & RT_TIMER_FLAG_SOFT_TIMER)
    {
        /* check whether timer thread is ready */
        if ((_soft_timer_status == RT_SOFT_TIMER_IDLE) &&
           ((_timer_thread.stat & RT_THREAD_STAT_MASK) == RT_THREAD_SUSPEND))
        {
            /* resume timer thread to check soft timer */
            rt_thread_resume(&_timer_thread);
            need_schedule = RT_TRUE;
        }
    }
#endif /* RT_USING_TIMER_SOFT */
```

### rt_schedule

