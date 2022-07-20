# memtest

### test1

```
msh >memtest1
msh >
msh >Function[rt_malloc] shall not be used in ISR
(0) assertion failed at function:rt_malloc, line number:279 
```

原因：malloc不能使用在中断中，此处用在硬件定时器的超时函数中

### test2

```
msh >memtest2
((((rt_ubase_t)rmem) & (RT_ALIGN_SIZE - 1)) == 0) assertion failed at function:rt_free, line number:562
```

原因：释放了野指针

```
    rt_uint8_t *p;
    rt_free(p);
```

### test3

```
msh >memtest3
msh >
msh >
```

此处编译器自动赋初值RT_NULL，不会导致断言

### test4

```
msh >memtest4
to free a bad data block:
mem: 0x200030a4, used flag: 0, magic code: 0x1ea0
(mem->used) assertion failed at function:rt_free, line number:594 
```

原因：连续两次释放，第二次释放相当于释放野指针，导致断言

### test5

```
msh >memtest5
msh >
msh >
```

原因：虽然是两次释放，但是第一次释放后将指针指向空，不会导致断言

### test6

```
msh >memtest6
msh >
msh >
```

原因：rt_freep（）释放后将指针指向空，后续同test5

```
void rt_freep(void *p)
{
    void **ptr = p;

    if(ptr && *ptr)
    {
        rt_free(*ptr);
        *ptr = RT_NULL;
    }
}
```

### test7

```
msh >memtest7
psr: 0x81000000
r00: 0x00000000
r01: 0x0800f611
r02: 0xffffffff
r03: 0x00000000
r04: 0xdeadbeef
r05: 0xdeadbeef
r06: 0xdeadbeef
r07: 0x20002ca0
r08: 0xdeadbeef
r09: 0xdeadbeef
r10: 0xdeadbeef
r11: 0xdeadbeef
r12: 0x20001aee
 lr: 0x08009fdd
 pc: 0x08001f72
hard fault on thread: tshell



thread   pri  status      sp     stack size max used left tick  error
-------- ---  ------- ---------- ----------  ------  ---------- ---
test       0  suspend 0x0000009c 0x00000200    39%   0x00000063 000
tshell    20  running 0x00000144 0x00001000    07%   0x00000009 000
tidle0    31  ready   0x0000005c 0x00000100    45%   0x00000018 000
timer      4  suspend 0x0000007c 0x00000200    24%   0x00000009 000
main      10  suspend 0x000000bc 0x00000800    14%   0x00000014 000
bus fault:
SCB_CFSR_BFSR:0x04 IMPRECISERR 
```

原因：将指针释放后又使用了该指针，此时指针为野指针

```
  rt_freep(&p);

    rt_thread_mdelay(1000);
    rt_memcpy(p, "xxxxxxxxxxxxxxxxxxxxxxxx", sizeof("xxxxxxxxxxxxxxxxxxxxxxxx"));
```

### test8

```
msh >memtest8
m77777777
psr: 0x01000000
r00: 0x20003114
r01: 0x00000000
r02: 0x20003114
r03: 0x20003114
r04: 0x00000000
r05: 0x00000001
r06: 0xdeadbeef
r07: 0x20003120
r08: 0xdeadbeef
r09: 0xdeadbeef
r10: 0xdeadbeef
r11: 0xdeadbeef
r12: 0x00000000
 lr: 0x08004c09
 pc: 0x08004b04
hard fault on thread: m7



thread   pri  status      sp     stack size max used left tick  error
-------- ---  ------- ---------- ----------  ------  ---------- ---
m7       190 
```

原因：堆栈被破坏，动态分配的内存被释放后被动态线程创建所使用（同一段内存），但是后续使用了野指针，导致动态线程运行时堆栈破坏。

```
 rt_thread_t t7 = rt_thread_create("m7", m7, RT_NULL, 128, 1, 10);
    rt_thread_startup(t7);

    rt_thread_mdelay(1000);
    rt_memcpy(p, "xxxxxxxxxxxxxxxxxxxxxxxx", sizeof("xxxxxxxxxxxxxxxxxxxxxxxx"));
```

### test9

```
msh >memtest9
addr: 0x00000000
psr: 0x01000000
r00: 0x00000011
r01: 0x0000000a
r02: 0x00000011
r03: 0x02000001
r04: 0xdeadbeef
r05: 0xdeadbeef
r06: 0xdeadbeef
r07: 0x20002cd0
r08: 0xdeadbeef
r09: 0xdeadbeef
r10: 0xdeadbeef
r11: 0xdeadbeef
r12: 0x20001aee
 lr: 0x0800a065
 pc: 0x0800a06c
hard fault on thread: tshell



thread   pri  status      sp     stack size max used left tick  error
-------- ---  ------- ---------- ----------  ------  ---------- ---
test       0  suspend 0x0000009c 0x00000200    39%   0x00000063 000
tshell    20  running 0x000000cc 0x00001000    08%   0x00000009 000
tidle0    31  ready   0x00000074 0x00000100    45%   0x00000009 000
timer      4  suspend 0x0000007c 0x00000200    24%   0x00000009 000
main      10  suspend 0x000000bc 0x00000800    14%   0x00000014 000
bus fault:
SCB_CFSR_BFSR:0x82 PRECISERR SCB->BFAR:02000001
```

原因：创建时所需内存太大，内存分配失败，指针为空，但是未判断是否创建成功直接使用导致出错

```
 rt_kprintf("p->c = %d\n", p->c);
```

### test10

```
msh >memtest10
addr: 0x00000000
msh >
msh >
```

原因：此处进行了判断内存是否分配成功，所以不会出现9中错误

```
if(p)
    {
        rt_kprintf("p->c = %d\n", p->c);
    }
```

### test11

```
msh >memtest11
addr: 0x200030b4
psr: 0x01000000
r00: 0x00000000
r01: 0x00000000
r02: 0x00000000
r03: 0x00000000
r04: 0xdeadbeef
r05: 0xdeadbeef
r06: 0xdeadbeef
r07: 0xdeadbeef
r08: 0xdeadbeef
r09: 0xdeadbeef
r10: 0xdeadbeef
r11: 0xdeadbeef
r12: 0x00000000
 lr: 0x080044f5
 pc: 0x00000008
hard fault on thread: t11



thread   pri  status      sp     stack size max used left tick  error
-------- ---  ------- ---------- ----------  ------  ---------- ---
t11        1  running 0x00000048 0x00000080    59%   0x0000000a 000
test       0  suspend 0x0000009c 0x00000200    39%   0x00000063 000
tshell    20  ready   0x00000524 0x00001000    35%   0x0000000a 000
tidle0    31  ready   0x00000074 0x00000100    45%   0x00000019 000
timer      4  suspend 0x0000007c 0x00000200    24%   0x00000009 000
main      10  suspend 0x000000bc 0x00000800    14%   0x00000014 000
usage fault:
SCB_CFSR_UFSR:0x100 UNALIGNED 
```

原因：内存写穿，p只有128字节内存，buffer有1024字节，调用rt_memcpy(p, buffer, 1024)导致内存写穿

### test12

修改rt_uint8_t buffer[128**2];为rt_uint8_t buffer[1024]则正常

```
msh >memtest12
p1: 0x200030b4
p2: 0x20003144
msh >
msh >
```

#### test13

```
msh >memtest13
malloc 1MB
msh >
msh >
```

原因：

```
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
```

后来不够申请1M内存空间

### test14

```
msh >memtest14
(thread != RT_NULL) assertion failed at function:rt_thread_startup, line number:326
```

原因：内存泄漏，线程m14_2不够创建导致内存泄漏

### test15

正常

缺点：可能内存碎片化

### test16

优点：申请较大内存，减少内存碎片化

