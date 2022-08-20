# <center>基于IOT的智能语音拐杖

## 作品简介

*本产品基于stm32与K210开发板实现语音提示、盲人用户的实时定位、超声波避障与安全监测，将数据实时上传小程序。*

## 功能概述

|          具体功能         	|  元器件、模块及其他要求	|  任务情况 	|
| --------------------------| ----------------------|-----------|
| 加速度传感器、跌倒报警 | <center>ADXL345模块	| 已调试完成 |
| 超声波模块避障            	| <center>HCSR04超声波模块      	| 已调试完成	|
| Sim卡模块拨电话紧急联系    	| <center>SIM800/SIM800A、Sim卡	| 已调试完成	|
| 语音提示        	| <center>LD3320A语音识别模块   	| 已调试完成	|
| 实现软硬件对接以及数据传输 	| <center>腾讯云mqtt           	| 已调试完成	|
| 定位、路线导航             | <center>ATGM336H    | 已调试完成	|
| 设备充放电                  	| <center>锂电池              |			|

## 串口使用情况

串口号   | 对应模块
| --- | ---|
usart1  |  <center>后期调试
usart2  |  <center>k210,语音模块
usart3  |  <center>sim800
usart4  |  <center>gps

## 模块接线

超声波模块：

PE2--->Trig

PE3<---Echo

语音识别模块：

PA2--->RXD

TTS（语音合成模块）<---TXD

V831模块：

PA3<---TXD

SIM800模块：

B10--->RXD

B11<---TXD

GPS模块：

PC11<---TXD

加速度检测模块：

PA4--->SCL

PA5<---SDA

## GPS定位数据格式

无效地址：$GNRMC,hhmmss.sss,V,,,,,,,xxxxxx,,,X*xx

有效地址：$GPRMC,hhmmss.sss,A,ddmm.mmmm,N,dddmm.mmmm,E,x.x,xxx.x,xxxxxx,x.x,X,X*xx





