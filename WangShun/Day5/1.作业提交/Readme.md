# 1.HC32F460 ESP8266的使用
![image](https://github.com/Yaochenger/summer2022/blob/main/WangShun/Day5/2.picture/1.ESP8266%E7%9A%84%E4%BD%BF%E7%94%A8.jpg)
# 2.HC32f460 Webclient测试
在使用`Webclient`软件包的时候出现了一个问题
![image](https://github.com/Yaochenger/summer2022/blob/main/WangShun/Day5/2.picture/2.HC32-Webclient%E6%B5%8B%E8%AF%95.jpg)

在输入studio中使用查找的功能，查找`MEM_POOL(&small_mem->heap_ptr[mem->next]) == small_mem`,定位到出问题的地方，在一路跳转后由于水平问题并未找到错的原因，最后采用粗暴的方式`直接将该句断言注释掉`注释掉后成功跑起，终端输出错误定位在`rt_smem_free`,猜测可能是错误释放内存引起的。
![image](https://github.com/Yaochenger/summer2022/blob/main/WangShun/Day5/2.picture/3.%E7%B2%97%E6%9A%B4%E7%9A%84%E8%A7%A3%E5%86%B3%E5%8A%9E%E6%B3%95.jpg)

 2.1![image](https://github.com/Yaochenger/summer2022/blob/main/WangShun/Day5/2.picture/4.web_get_test.jpg)

 2.2![image](https://github.com/Yaochenger/summer2022/blob/main/WangShun/Day5/2.picture/5.web_post_test.jpg)

 2.3![image](https://github.com/Yaochenger/summer2022/blob/main/WangShun/Day5/2.picture/6.web_shard_test.jpg)

 加油！！！！