#include "rtthread.h"
#include "rtdevice.h"

static void m1(void *p)
{
    rt_uint8_t *x = rt_malloc(10);
    rt_kprintf("x = 0x%.8x thread = %s\n", (int)x, rt_thread_self()->name);
    rt_free(x);
}

int memtest1(void)
{
    rt_timer_t t1 = rt_timer_create("m1", m1, RT_NULL, RT_TICK_PER_SECOND,
        RT_TIMER_FLAG_HARD_TIMER|RT_TIMER_FLAG_PERIODIC);
    rt_timer_start(t1);

    return 0;
}
MSH_CMD_EXPORT(memtest1, memtest1);

int memtest2(void)
{
    rt_uint8_t *p;
    rt_free(p);

    return 0;
}
MSH_CMD_EXPORT(memtest2, memtest2);

rt_uint8_t *pp;
int memtest3(void)
{
    rt_free(pp);

    return 0;
}
MSH_CMD_EXPORT(memtest3, memtest3);

int memtest4(void)
{
    rt_uint8_t *p = rt_malloc(1);
    rt_free(p);
    rt_free(p);

    return 0;
}
MSH_CMD_EXPORT(memtest4, memtest4);

int memtest5(void)
{
    rt_uint8_t *p = rt_malloc(1);
    rt_free(p); p = RT_NULL;
    rt_free(p);

    return 0;
}
MSH_CMD_EXPORT(memtest5, memtest5);

void rt_freep(void *p)
{
    void **ptr = p;

    if(ptr && *ptr)
    {
        rt_free(*ptr);
        *ptr = RT_NULL;
    }
}

int memtest6(void)
{
    rt_uint8_t *p = rt_malloc(1);
    rt_freep(&p);
    rt_freep(&p);

    return 0;
}
MSH_CMD_EXPORT(memtest6, memtest6);

void m7(void *p)
{
    while(1)
    {
        rt_kprintf("m77777777\n");
        rt_thread_mdelay(500);
    }
}

int memtest7(void)
{
    rt_uint8_t *p = rt_malloc(128);
    rt_freep(&p);

    rt_thread_mdelay(1000);
    rt_memcpy(p, "xxxxxxxxxxxxxxxxxxxxxxxx", sizeof("xxxxxxxxxxxxxxxxxxxxxxxx"));

    return 0;
}
MSH_CMD_EXPORT(memtest7, memtest7);

int memtest8(void)
{
    rt_uint8_t *p = rt_malloc(128);
    rt_freep(&p);

    rt_thread_t t7 = rt_thread_create("m7", m7, RT_NULL, 128, 1, 10);
    rt_thread_startup(t7);

    rt_thread_mdelay(1000);
    rt_memcpy(p, "xxxxxxxxxxxxxxxxxxxxxxxx", sizeof("xxxxxxxxxxxxxxxxxxxxxxxx"));

    return 0;
}
MSH_CMD_EXPORT(memtest8, memtest8);

// 空指针异常访问
typedef struct memtest9
{
    rt_uint8_t a[1024*1024*32];
    rt_uint8_t b;
    rt_uint8_t c;
} memtest9_t;

int memtest9(void)
{
    struct memtest9 *p = rt_malloc(sizeof(struct memtest9));
    rt_kprintf("addr: 0x%.8x\n", (int)p);
    rt_kprintf("p->c = %d\n", p->c);

    return 0;
}
MSH_CMD_EXPORT(memtest9, memtest9);

int memtest10(void)
{
    struct memtest9 *p = rt_malloc(sizeof(struct memtest9));
    rt_kprintf("addr: 0x%.8x\n", (int)p);

    if(p)
    {
        rt_kprintf("p->c = %d\n", p->c);
    }

    return 0;
}
MSH_CMD_EXPORT(memtest10, memtest10);

/* 内存写穿 */
void m11(void *p)
{
    while(1)
    {
        rt_kprintf("m11 11 11 11\n");
        rt_thread_mdelay(500);
    }
}

int memtest11(void)
{
    rt_uint8_t buffer[1024];

    rt_uint8_t *p = rt_malloc(128);
    rt_kprintf("addr: 0x%.8x\n", (int)p);

    rt_thread_t t11 = rt_thread_create("t11", t11, RT_NULL, 128, 1, 10);
    rt_thread_startup(t11);

    rt_thread_mdelay(2000);
    rt_memcpy(p, buffer, 1024);

    return 0;
}
MSH_CMD_EXPORT(memtest11, memtest11);

int memtest12(void)
{
    rt_uint8_t buffer[128*2]; // 换成1024又会如何

    rt_uint8_t *p1 = rt_malloc(128);
    rt_kprintf("p1: 0x%.8x\n", (int)p1);

    rt_uint8_t *p2 = rt_malloc(128);
    rt_kprintf("p2: 0x%.8x\n", (int)p2);

    rt_thread_mdelay(2000);
    rt_memcpy(p1, buffer, 1024);

    return 0;
}
MSH_CMD_EXPORT(memtest12, memtest12);

/* 内存泄漏1 */
void m13(void *p)
{
    for(int i = 0; i < 1000; i++)
    {
        rt_kprintf("malloc 1MB\n");
        rt_uint8_t *p = rt_malloc(1024*1024);
        if(!p)
        {
            break;
        }
        rt_thread_mdelay(500);
    }
}

int memtest13(void)
{
    rt_thread_t t13 = rt_thread_create("t13", m13, RT_NULL, 1024, 1, 10);
    rt_thread_startup(t13);

    return 0;
}
MSH_CMD_EXPORT(memtest13, memtest13);

/* 内存泄漏2 */
void m14_1(void *p)
{
    for(int i = 0; i < 1000; i++)
    {
        rt_uint8_t *p1 = rt_malloc(1024*1024);
        rt_uint8_t *p2 = rt_malloc(1024*7);
        rt_thread_mdelay(200);
        rt_freep(&p1);
        rt_freep(&p2);
        rt_thread_mdelay(200);
    }
}

void m14_2(void *p)
{
    for(int i = 0; i < 100; i++)
    {
        rt_uint8_t *p = rt_malloc(10*1024);
        rt_thread_mdelay(1000);
    }
}

int memtest14(void)
{
    rt_thread_t t14_1 = rt_thread_create("t14_1", m14_1, RT_NULL, 1024, 1, 10);
    rt_thread_startup(t14_1);

    rt_thread_t t14_2 = rt_thread_create("t14_2", m14_2, RT_NULL, 1024, 1, 10);
    rt_thread_startup(t14_2);

    return 0;
}
MSH_CMD_EXPORT(memtest14, memtest14);

typedef struct memtest15
{
    rt_uint8_t *buffer;
    rt_uint32_t len;
} memtest15_t;

int memtest15(void)
{
    struct memtest15 *m15 = rt_malloc(sizeof(struct memtest15));
    m15->buffer = rt_malloc(1024);
    m15->len = 1024;

    return 0;
}
MSH_CMD_EXPORT(memtest15, memtest15);

int memtest16(void)
{
    struct memtest15 *m15 = rt_malloc(sizeof(struct memtest15) + 1024);
    m15->buffer = ((rt_uint8_t *)m15) + 1024;
    m15->len = 1024;

    return 0;
}
MSH_CMD_EXPORT(memtest16, memtest16);

int memtest17(void)
{
    rt_malloc(777);

    struct rt_memheap *mh17 = (struct rt_memheap *)rt_object_find("sdram", RT_Object_Class_MemHeap);
    rt_memheap_alloc(mh17, 888);
    rt_memheap_alloc(mh17, 888);
    rt_memheap_alloc(mh17, 888);

    return 0;
}
MSH_CMD_EXPORT(memtest17, memtest17);

/*
msh />memtrace
total memory: 16352
used memory : 5720
maximum allocated memory: 5720

memory heap address:
heap_ptr: 0x20004818
lfree   : 0x200052e4
heap_end: 0x20008808

--memory item information --
[0x20004818 -   128] NONE
[0x200048a8 -    2K] NONE
[0x200050b8 -    12] main
[0x200050d4 -   428] main
[0x20005290 -    16] main
[0x200052b0 -    36] main
[0x200052e4 -    12]
[0x20005300 -    12] main
[0x2000531c -    76] main
[0x20005378 -    12] main
[0x20005394 -    12] main
[0x200053b0 -    12] main
[0x200053cc -   528] main
[0x200055ec -   128] main
[0x2000567c -    2K] main
[0x20005e8c -   10K]
msh />

*/