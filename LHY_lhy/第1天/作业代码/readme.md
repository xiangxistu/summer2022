此例使用的开发板为  **CPK_RA6M4**



此例在hal_entry()即main()中创建了一个timer_thread线程

在线程中创建了一个软件的周期定时器,500tick为一个周期.

在回调函数中控制led灯翻转,并且输出当前灯的状态

创建完成定时器后,timer_thread线程即被自动回收


关键词 : **PIN ** **软件定时器** **thread创建及其使用**







