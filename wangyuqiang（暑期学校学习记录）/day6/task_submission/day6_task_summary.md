## day5 summary

---

## AT组件

#### 1、介绍

​	AT 命令集是一种应用于 AT 服务器（AT Server）与 AT 客户端（AT Client）间的设备连接与数据通信的方式。

![AT 命令框架](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207231816353.jpeg)

<1> 一般 AT 命令由三个部分组成，分别是：前缀、主体和结束符。其中前缀由字符 AT 构成；主体由命令、参数和可能用到的数据组成；结束符一般为 `<CR><LF>` (`"\r\n"`)。
<2> AT 功能的实现需要 AT Server 和 AT Client 两个部分共同完成。
<3> AT Server 主要用于接收 AT Client 发送的命令，判断接收的命令及参数格式，并下发对应的响应数据，或者主动下发数据。
<4> AT Client 主要用于发送命令、等待 AT Server 响应，并对 AT Server 响应数据或主动发送的数据进行解析处理，获取相关信息。
<5> AT Server 和 AT Client 之间支持多种数据通讯的方式（UART、SPI 等），目前最常用的是串口 UART 通讯方式。
<6> AT Server 向 AT Client 发送的数据分成两种：响应数据和 URC 数据。

#### 2、AT Seriver

AT Server 目前默认支持的基础命令如下：

- AT：AT 测试命令；
- ATZ：设备恢复出厂设置；
- AT+RST：设备重启；
- ATE：ATE1 开启回显，ATE0 关闭回显；
- AT&L：列出全部命令列表；
- AT+UART：设置串口设置信息。

AT Server API接口

* 发送数据至客户端（不换行）：void at_server_printf(const char *format, ...);
* 发送数据至客户端（换行）：void at_server_printfln(const char *format, ...);
* 发送命令执行至客户端：void at_server_print_result(at_result_t result);
* 解析输入命令参数：int at_req_parse_args(const char *req_args, const char *req_expr, ...);

#### 3、AT Client

AT Client API接口

* 初始化：int at_client_init(const char *dev_name,  rt_size_t recv_bufsz);
* 发送命令并接收响应：rt_err_t at_exec_cmd(at_response_t resp, const char *cmd_expr, ...);
* 发送指定长度数据：rt_size_t at_client_send(const char *buf, rt_size_t size);
* 接收指定长度数据：rt_size_t at_client_recv(char *buf, rt_size_t size,rt_int32_t timeout);
* 设置接收数据的行结束符：void at_set_end_sign(char ch); // 用于判断客户端接收一行数据的结束。
* 等待模块初始化完成：int at_client_wait_connect(rt_uint32_t timeout);

---

## SAL套接字抽象层

#### 1、SAL简介

​	为了适配更多的网络协议栈类型，避免系统对单一网络协议栈的依赖，RT-Thread 系统提供了一套 SAL（套接字抽象层）组件，该组件完成对不同网络协议栈或网络实现接口的抽象并对上层提供一组标准的 BSD Socket API，这样开发者只需要关心和使用网络应用层提供的网络接口，而无需关心底层具体网络协议栈类型和实现，极大的提高了系统的兼容性，方便开发者完成协议栈的适配和网络相关的开发。

特点：
- 抽象、统一多种网络协议栈接口；
- 提供 Socket 层面的 TLS 加密传输特性;
- 支持标准 BSD Socket API；
- 统一的 FD 管理，便于使用 read/write poll/select 来操作网络功能；

#### 2、SAL网络框架

![网络框架图](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207231840950.jpeg)

最底层：网络应用层，提供一套标准 BSD Socket API ，如 socket、connect 等函数，用于系统中大部分网络开发应用。

倒数第二层：SAL 套接字抽象层，通过它 RT-Thread 系统能够适配下层不同的网络协议栈，并提供给上层统一的网络编程接口，方便不同协议栈的接入。套接字抽象层为上层应用层提供接口有：accept、connect、send、recv 等。

netdev 网卡层：主要作用是解决多网卡情况设备网络连接和网络管理相关问题，通过 netdev 网卡层用户可以统一管理各个网卡信息和网络连接状态，并且可以使用统一的网卡调试命令接口。

协议栈层：该层包括几种常用的 TCP/IP 协议栈，例如嵌入式开发中常用的轻型 TCP/IP 协议栈 lwIP 以及 RT-Thread 自主研发的 AT Socket 网络功能实现等。这些协议栈或网络功能实现直接和硬件接触，完成数据从网络层到传输层的转化。

网络应用层：提供的接口主要以标准 BSD Socket API 为主，这样能确保程序可以在 PC 上编写、调试，然后再移植到 RT-Thread 操作系统上。

#### 3、SAL工作原理

SAL 组件工作原理的介绍主要分为如下三部分：

- 多协议栈接入与接口函数统一抽象功能；
- SAL TLS 加密传输功能；

#### 4、多协议栈接入与接口函数同意抽象功能

​	SAL 组件提供对不同协议栈或网络实现接口的抽象和统一，组件在 socket 创建时通过**判断传入的协议簇（domain）类型来判断使用的协议栈或网络功能**，完成 RT-Thread 系统中多协议的接入与使用。

目前 SAL 组件支持的协议栈或网络实现类型有：**lwIP 协议栈**、**AT Socket 协议栈**、**WIZnet 硬件 TCP/IP 协议栈**。

```c
int socket(int domain, int type, int protocol);
```

上述为标准 BSD Socket API 中 socket 创建函数的定义，`domain` 表示协议域又称为协议簇（family），用于判断使用哪种协议栈或网络实现，AT Socket 协议栈使用的簇类型为 **AF_AT**，lwIP 协议栈使用协议簇类型有 **AF_INET**等，WIZnet 协议栈使用的协议簇类型为 **AF_WIZ**。

对于不同的软件包，socket 传入的协议簇类型可能是固定的，不会随着 SAL 组件接入方式的不同而改变。**为了动态适配不同协议栈或网络实现的接入**，SAL 组件中对于每个协议栈或者网络实现提供两种协议簇类型匹配方式：**主协议簇类型和次协议簇类型**。socket 创建时先判断传入协议簇类型是否存在已经支持的主协议类型，如果是则使用对应协议栈或网络实现，如果不是判断次协议簇类型是否支持。目前系统支持协议簇类型如下：

```c
lwIP 协议栈： family = AF_INET、sec_family = AF_INET
AT Socket 协议栈： family = AF_AT、sec_family = AF_INET
WIZnet 硬件 TCP/IP 协议栈： family = AF_WIZ、sec_family = AF_INET
```

SAL 组件主要作用是`统一抽象底层 BSD Socket API 接口`

#### 5、SAL TLS加密传输功能

​	TLS（Transport Layer Security，传输层安全协议) 是建立在传输层 TCP 协议之上的协议，其前身是 SSL（Secure Socket Layer，安全套接字层 ），主要作用是将应用层的报文进行非对称加密后再由 TCP 协议进行传输，实现了数据的加密安全交互。

​	目前常用的 TLS 方式：**MbedTLS、OpenSSL、s2n** 等，但是对于不同的加密方式，需要使用其指定的加密接口和流程进行加密，对于部分应用层协议的移植较为复杂。因此 SAL TLS 功能产生，主要作用是**提供 Socket 层面的 TLS 加密传输特性，抽象多种 TLS 处理方式，提供统一的接口用于完成 TLS 数据交互**。	

**SAL TLS 功能使用方式**

使用流程如下：

- 配置开启任意网络协议栈支持（如 lwIP 协议栈）；
- 配置开启 MbedTLS 软件包（目前只支持 MbedTLS 类型加密方式）；
- 配置开启 SAL_TLS 功能支持（如下配置选项章节所示）；

配置完成之后，只要在 socket 创建时传入的 `protocol` 类型使用 **PROTOCOL_TLS** 或 **PROTOCOL_DTLS **，即可使用标准 BSD Socket API 接口，完成 TLS 连接的建立和数据的收发。

`初始化：int sal_init(void);`

该初始化函数主要是对 SAL 组件进行初始，支持组件重复初始化判断，完成对组件中使用的互斥锁等资源的初始化。 SAL 组件中没有创建新的线程，这也意味着 SAL 组件资源占用极小，目前**SAL 组件资源占用为 ROM 2.8K 和 RAM 0.6K**。

#### 6、BSD Socket API接口

SAL 组件抽象出标准 BSD Socket API 接口，如下是对常用网络接口的介绍：

* `创建套接字（socket）：int socket(int domain, int type, int protocol);`

![image-20220723185242614](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207231852656.png)

该函数用于根据指定的地址族、数据类型和协议来分配一个套接字描述符及其所用的资源。

* `绑定套接字（bind）：int bind(int s, const struct sockaddr *name, socklen_t namelen);`

该函数用于将端口号和 IP 地址绑定带指定套接字上。

SAL 组件依赖 netdev 组件，当使用 `bind()` 函数时，可以通过 netdev 网卡名称获取网卡对象中 IP 地址信息，用于将创建的 Socket 套接字绑定到指定的网卡对象。

* `监听套接字（listen）：int listen(int s, int backlog);`

该函数用于 TCP 服务器监听指定套接字连接。

* `接收连接（accept）：int accept(int s, struct sockaddr *addr, socklen_t *addrlen);`

当应用程序监听来自其他主机的连接时，使用 `accept()` 函数初始化连接，`accept()` 为每个连接创立新的套接字并从监听队列中移除这个连接。

* `建立连接（connect）：int connect(int s, const struct sockaddr *name, socklen_t namelen);`

该函数用于建立与指定 socket 的连接。

* `TCP数据发送（send）：int send(int s, const void *dataptr, size_t size, int flags);`

该函数常用于 TCP 连接发送数据。

* `TCP数据接收（recv）：int recv(int s, void *mem, size_t len, int flags);`

该函数用于 TCP 连接接收数据。

* `UDP数据发送（sendto）：int sendto(int s, const void *dataptr, size_t size, int flags, const struct sockaddr *to, socklen_t tolen);`

该函数用于 UDP 连接发送数据。

* `UDP数据接收（recvfrom）：int recvfrom(int s, void *mem, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen);`

该函数用于 UDP 连接接收数据。

* `关闭套接字（closesocket）：int closesocket(int s);`

该函数用于关闭连接，释放资源。

---

## LTS

#### 1、简介

​	TLS（Transport Layer Security，安全传输层)，TLS 是建立在传输层 TCP 协议之上的协议，服务于应用层，它的前身是 SSL（Secure Socket Layer，安全套接字层），它实现了将应用层的报文进行加密后再交由 TCP 进行传输的功能。

#### 2、TLS作用

TLS 协议主要解决如下三个网络安全问题。

保密(message privacy)，保密通过加密 encryption 实现，所有信息都加密传输，第三方无法嗅探； 完整性(message integrity)，通过MAC校验机制，一旦被篡改，通信双方会立刻发现； 认证(mutual authentication)，双方认证,双方都可以配备证书，防止身份被冒充；

#### 3、TLS报文结构

![tls_http_layer.png](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207232009523.png)

---

## RT-Thread的lwip协议栈

#### 1、TCP/IP的网络分层模型

![img](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207232013148.png)

![img](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207232013062.png)

#### 2、说明

​	lwip 协议栈是实现了 TCP/IP 协议栈一部分功能的，可以用在嵌入式中的网络协议栈；有些函数并不能完全兼容，比如 SO_REUSEADDR 参数，在大型的网络协议栈中，很有可能是 `setsockopt`的一个入参，而对于 lwip 来说，是一个一旦编译好就固定的策略，不是一个灵活的配置项。在进行对应的 APP 功能移植时，应当慎重处理编译产生的异常。通过 sal 的方式使用 lwip，at socket，wiznet 来进行编程，一定要注意编译产生一些异常事件和函数异常事件。lwip 是一个可以运行在嵌入式中的网络协议栈，它的高效，精简的编程方式和策略都很值得我们学习。

---

## lwip ARP

#### 1、简介

在实际使用中，**ARP （Address Resolution Protocol）地址解析协议** 就起到了`沟通 IP 地址与 MAC 地址`的作用。

ARP 的报文比较简单，就是两个功能：ARP request，ARP response；即一个 ARP 查询报文，一个 ARP 回复报文。

#### 2、ARP报文格式

ARP 报文与 IP 报文都是附着在 ETH 帧之上，可以看到 ARP 报文长度共有 28 字节；包含的内容包括【发送端】与【接收端】的【以太网地址】与【IP 地址】。

![img](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207232018056.png)

1. ARP 只有两个报文类型：request 与 response ( reply ) 。
2. 是在知道 IP 地址的基础上，使用 request 查询该 IP 地址可以使用的 MAC 地址，而不是反过来。
3. 一旦收到 ARP 请求，发现该 IP 与本机 IP 地址相符就回复 response ，如果不是就忽略。
4. 为了能维护一个 MAC 与 IP 的映射表，会有一个 ARP 缓存表的存储结构；ETH 帧的收发都会尝试更新这个 ARP 缓存表。
5. ARP 报文的收发解析的主要功能就是维护这个 ARP 缓存表，以便在发送 IP 报文时可以直接填充 【以太网目的地址】与【以太网源地址】。

#### 3、ARP功能描述

1. 发送数据时，根据 ARP 表填充 ETH 帧的地址。
2. 如果 ARP 表没有对应的 IP 与 MAC 地址的条目，应当发送 ARP request 来查询。
3. 在收到 ARP response 时，应当把报文中的 IP 与 MAC 地址添加到 ARP表中。
4. 应当提供一个查询 ARP 表的功能，并能更新 ARP 映射关系。

---

## Lwip ICMP

#### 1、简介

为了提高数据交付的准确性，ICMP ( Internet Control Message Protocol ，因特网控制报文协议 ) 随之出现。在交付数据时，如果由于网络状况不佳、链路不通等情况导致数据报无法到达目标主机，ICMP 就会返回一个差错报文，让源主机知道数据没能正常到达目标主机，接着进行重发或者放弃发送都可以。

ICMP 通常被认为是 IP 层协议的一部分，但从体系结构上讲它是位于 IP 层之上的，因为 ICMP 报文是承载在 IP 数据报中的。这就是说：**ICMP 报文是作为 IP 数据报数据区域的，就像 TCP 与 UDP 报文作为 IP 数据报区域那样。**类似的，**当一台主机收到一个指明上层协议为 ICMP 的 IP 数据报时**，它将分解出该数据报的内容给 ICMP，就像分解出一个数据报的内容给 TCP 或 UDP 一样，但与 TCP 或 UDP 又有所不同，**ICMP 不是为上层应用程序提供服务，而只是在 IP 层传递差错信息的报文，依赖于 IP 进行传输。**

#### 2、ICMP报文结构

![protocol.png](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207232024233.png)

ICMP 首部剩下的四个字节会因为不同类型的报文而有不一样的定义，并且数据部分的长度也存在差异。ICMP 报文格式如下图所示：

![protocol.png](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207232024072.png)

#### 3、ICMP报文类型

ICMP 报文可以划分为 **差错报告** 和 **报文查询** 报文两类。

- **差错报告报文**主要用来向 IP 数据报源主机返回一个差错报告信息，而产生这个差错报告信息的原因是路由器或者主机不能对当前数据进行正常的处理，简单来说，就是源主机发送的数据报无法到达目标主机，或者到达了目标主机而无法递交给上层协议。
- **查询报文**适用于一台主机向另一台主机发送一个请求情况，如果目标主机收到这个查询的请求后，就会按照查询报文的格式向源主机做出应答，比如我们使用的 ping 命令，它的本质就是一个 ICMP 查询报文。

![icmp_protocol_detail.png](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207232025592.png)

#### 4、ICMP差错报告报文

- **目的不可达：**当路由器或主机不能交付数据报时就向源主机发送终点(目的)不可达的报文。
- **超时：**IP 数据报首部有一个 TTL 字段，当 IP 数据报每经过一个路由器转发，TTL 的值就会减 1，如果 TTL 的值被减到 0，那么路由或者主机就会丢弃该数据报，并返回一个 ICMP 超时报文到源主机中。此外，在数据报分片重装的时候也使用了 ICMP 报文，当所有的 IP 分片数据报无法在规定的时间内完成重装时，主机也会认为它超时了，那么这些数据报就会被删除，同时返回一个 ICMP 超时报文到源主机中。
- **参数问题：**IP 数据报在网络中传输时，都是根据其首部进行识别的，如果首部出现错误，那么就会产生严重的问题，因此**如果 IP 数据报首部出现错误就会丢弃数据报，并且向源主机返回一个 ICMP 参数错误表。**不过，**对于携带 ICMP 差错报文的数据报、非第一个分片的分片数据报、具有特殊目的地的数据报 ( 如环回、多播、广播 ) 等，即使出现了差错，也不会返回对应的差错报文。**
- **重定向：**一般来说，某个主机在启动的时候只有一个路由表 ( 即：默认路由 )，所以它发送的数据都发给了默认路由，让其帮忙转发，而路由器发现数据应该是发送给另一个路由的，那么它会返回一个 ICMP 重定向报文给源主机，告诉源主机应该直接发给另一个路由器。**重定向一般用来让刚启动的主机逐渐建立更完善的路由表**，**重定向报文只能有路由器生成而不能有主机生成，但是使用重定向报文的只能是主机而非路由器**。在主机刚开始工作时，一般都在路由表中设置一个默认路由器的 IP 地址，不管数据报要发送到哪个目的地，都一律先把数据报传送给这个默认路由器，而这个默认路由器知道到每一个目的网络的最佳路由 ( 通过和其他路由器交换路由信息 )。如果路由器发现主机发往某个目的地址的数据报的最佳路由应该经过网络上的另一个路由器 R 时，就会发送重定向的 ICMP 报文将此情况告诉主机。于是主机就会在其路由表中增加一个项目：到某某目的地址应该经过路由器 R ( 而不是默认路由器 )。

**所有 ICMP 差错报文中的数据字段都具有同样的格式：把收到的需要进行差错报告的 IP 数据报的首部和数据字段的前 8 个字节提取出来，作为 ICMP 报文的数据字段，再加上相应的 ICMP 差错报文的前 8 个字节，就构成了 ICMP 差错报告报文。**如下图所示：

![icmp_protocol_01.png](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207232026914.png)

提取收到的数据报的数据字段前 8 个字节是为了得到运输层的端口号 ( 对于 TCP 和 UDP ) 以及运输层报文的发送序号 ( 对于 TCP )，这些信息对源主机通知高层协议是有用的，然后整个 ICMP 报文作为 IP 数据报的数据字段发送给源主机。

#### ICMP 查询报文

ICMP 的查询报文常见的只有两种：

- **回送请求和回答：**ICMP 回送请求报文是由主机或路由器向一个特定的目的主机发出的询问。收到此报文的主机必须给源主机或路由器发送 ICMP 回送回答报文。这种询问报文用来测试目的站是否可达以及了解其有关状态。
- **时间戳请求和应答：**ICMP 时间戳请求报文是请某台主机或路由器回答当前的日期和时间。在 ICMP 时间戳回答报文中有一个 32 位的字段，其中写入的整数代表从 1900 年 1 月 1 日起到当前时刻一共多少秒。**时间戳请求和回答可用于时钟同步和时间测量。**

ICMP 查询报文最通用的一个事例是 **ping 命令**，用来测试两台主机之间的连通性。ping 是应用层直接使用**网络层ICMP**的一个例子，**它没有通过传输层的 TCP 或 UDP**。Windows 操作系统的用户可以在接入互联网后，在 CMD 中输入 **ping hostname ( hostname 是要测试连通性的主机名或它的 IP 地址 ) **，按回车键后就可看到结果。

---

## Lwip IP

#### 1、IP背景

IP 协议是 TCP/IP 协议中最为核心的协议，所有的 TCP、UDP、ICMP 及 IGMP 数据都已 IP 数据报格式传输。IP协议在 TCP/IP 协议族的分层中属于网络层，不难理解，IP 的主要作用有两个：其一是对上层协议的数据进行封装（ 增加 IP 首部 ），然后交给链路层协议进行发送；其二是对链路层接收到的数据进行解析，并根据解析结果将数据交给对应的上层协议进行处理。

#### 2、IP数据报的格式

IP数据报的格式主要包含 IP 首部和数据，通常情况下，IP 首部的长度为 20 字节（含有选项字段的除外），具体如下图所示：

![IP数据报格式](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207232032094.png)

#### 3、IP地址基础知识

IP 地址的是一个 32 位的二进制数，为了方便记忆和读写，常用点分十进制的方法来表示 IP 地址。IP 地址中包含了网络号和主机号，可能还包含有子网号，为了能正确的从 IP 地址中获取网络号、主机号、子网号，还需要有子网掩码的辅助。

---

## Lwip UDP

#### 1、简介

UDP (User Datagram Protocol)：用户数据报协议，是一种简单、无连接、不可靠的传输协议。

无需建立连接、没有提供任何流量控制、拥塞控制机制，收到的报文也没有确认，因此 UDP 的传输速度快，但不能保证数据到达目的地。

与我们熟知的 TCP 协议一样，都属于 OSI 模型中的传输层协议。

#### 2、UDP特点

1. 无连接性 UDP 可以提供无连接的数据传输服务，无需在通讯前建立连接，也无需在通讯结束后断开连接，节省了维护连接的开销。
2. 不可靠性 受自身协议的限制，UDP 的传输是一种不靠传输方式，无法保证数据一定能完整有效的传输到目标。
3. 以报文为边界 因为没有 TCP 协议的数据编号和接收确认机制，UDP 对于应用层交付的数据直接进行封装传输，不会对报文进行拆分合并，在多包数据传输时可能出现乱序的现象。
4. 无流量和拥塞控制功能 UDP 协议没有流量控制和拥塞控制的机制，因此更适用于对数据连续性比完整性要求更高、对轻微的数据差错不敏感的场景，如语音、视频通话等。
5. 支持广播、组播 不同于 TCP 协议只能实现一对一的单播通讯，UDP 协议支持单播、广播、组播通讯，实现一对一、一对多、多对多的数据传输。因此所有以广播、组播方式通信的协议都是在 UDP 协议上实现的，如我们常见的 DHCP、SNMP 协议。

#### 3、报文格式

![udp_protocol.jpg](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207232034033.jpeg)

---

#### Lwip TCP

#### 1、TCP协议简介

TCP 与 UDP 一样，都是传输层的协议，但是提供的服务却不相同，UDP 为上层应用提供的是一种不可靠的，无连接的服务，而 TCP 提供一种面向连接，可靠的字节传输服务，TCP 让两个主机建立连接的关系，应用数据以数据流的形式进行传输，先后发出的数据在网络中虽然也是互不干扰的传输，但是这些数据本身携带的信息确实紧密联系的，TCP 协议会给每个传输的字号进行编号，当然了，两个主机方向上的数据编号是彼此独立的，在传输的过程中，发送方把数据的起始编号与长度放在 TCP 报文中，接收方将所有数据按照编号组装起来，然后返回一个确认，当所有的数据接收完成后才将数据递交到应用层。

#### 2、TCP报文段结构

TCP 报文段依赖 IP 协议进行发送，因此 TCP 报文段与 ICMP 报文 一样，都是封装在 IP 数据报文中，IP 数据报封装在以太网帧中，因此 TCP 报文段也是经过 了两次的封装，然后发送出去,报文结构如下：

![tcp_protocol](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207232036645.jpeg)

---

## Lwip DHCP

#### 1、简介

动态主机配置协议 DHCP（Dynamic Host Configuration Protocol）是一种对基于 TCP/IP 协议主机的网络参数进行动态配置和集中管理的协议，可以实现：

- 为网络主机分配 IP 地址

DHCP 可以提供两种地址分配机制（动态分配机制和静态分配机制），网络管理员可以根据网络需求为不同的主机选择不同的分配策略。

#### 2、DHCP报文格式

![lwip_dhcp_protocol.png](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207232038894.png)

#### 3、DHCP实现原理

![dhcp_work_01.png](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207232039267.png)

![dhcp_work_02.png](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207232039160.png)

- 发现阶段

DHCP 客户端在网络中广播发送 DHCP DISCOVER 请求报文，发现 DHCP 服务器，请求 IP 地址租约

- 提供阶段

DHCP 服务器通过 DHCP OFFER 报文向 DHCP 客户端提供 IP 地址预分配

- 选择阶段

DHCP 客户端通过 DHCP REQUEST 报文确认选择第一个 DHCP 服务器为它提供 IP 地址自动分配服务

- 确认阶段

被选择的 DHCP 服务器通过 DHCP ACK 报文把在 DHCP OFFER 报文中准备的 IP 地址租约给对应 DHCP 客户端

---

## netdev网卡

#### 1、介绍

netdev（network interface device），即网络接口设备，又称网卡。每一个用于网络连接的设备都可以注册成网卡，为了适配更多的种类的网卡，避免系统中对单一网卡的依赖，RT-Thread 系统提供了 netdev 组件用于网卡管理和控制。

netdev 组件主要作用是**解决设备多网卡连接时网络连接问题，用于统一管理各个网卡信息与网络连接状态，并且提供统一的网卡调试命令接口**。 其主要功能特点如下所示：

- 抽象网卡概念，每个网络连接设备可注册唯一网卡。
- 提供多种网络连接信息查询，方便用户实时获取当前网卡网络状态；
- 建立网卡列表和默认网卡，可用于网络连接的切换；
- 提供多种网卡操作接口（设置 IP、DNS 服务器地址，设置网卡状态等）；
- 统一管理网卡调试命令（ping、ifconfig、netstat、dns 等命令）；

#### 2、网卡概念

网卡概念介绍之前先了解协议栈相关概念，协议栈是指网络中各层协议的总和，每种协议栈反映了不同的网络数据交互方式，RT-Thread 系统中目前支持三种协议栈类型： lwIP 协议栈、AT Socket 协议栈、WIZnet TCP/IP硬件协议栈。每种协议栈对应一种协议簇类型（family），上述协议栈分别对应的协议簇类型为：AF_INET、AF_AT、AF_WIZ。

网卡的初始化和注册建立在协议簇类型上，所以每种网卡对应唯一的协议簇类型。 Socket 套接字描述符的创建建立在 netdev 网卡基础上，所以每个创建的 Socket 对应唯一的网卡。协议簇、网卡和 socket 之间关系如下图所示：

![netdev 网络关系图](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202207232040894.jpeg)