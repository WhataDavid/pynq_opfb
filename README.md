# pynq_opfb

包含了使用snap_test发送测试数据，并正确展示内容

包含了分别使用int 和uint读取dac replay并正确绘图

结论：
即便上一个dac replay ip 全使用uint存，下一个ip全使用int读，dma也使用int读，也是正确的

int读adc 右移4 正确
uint读adc 转int 正确
int读adc 正确
其他都错误
