1、CC_Mode模式已经完全OK
2、增加了实时电流显示，DAC8571的输出，ADC1,ADC2电压，负载两端电压的监控，误差都取得了良好的效果。
3、还没加入CR模式
4、目前存在的风险是：谜之问题，电流设定值达到0.5A以后，OLED显示电流值会有0.02甚至是1.5A大电流的情况，将电路板冷却静置30min，再上电会恢复，惊了。
5、电流检测插座必须接入台式5位半数字万用表，短接为导线的话设定值达到0.5A会有3A大电流出现，谨记！！
2020/7/27 18：06 江瀚林