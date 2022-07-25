# 800C连接物联网云服务器:

1. 查询注册上网络：AT+CREG?-->+CREG: 0,1

2. CSQ指令，查询信号：AT+CSQ-->信号强度范围0~31,99表示无信号

3. CGATT指令，查询是否附着GPRS:AT+CGATT?-->+CGATT: 1

4. 设置单链接:AT+CIPMUX=0-->OK

5. 设置透传:AT+CIPMODE=1-->OK

6. 激活数据网络:
    激活GPRS：AT+SAPBR=3,1,"CONTYPE","GPRS"-->OK
    设置接入点:AT+SAPBR=3,1,"APN","CMIOT"-->OK
    打开GPRS网络:AT+SAPBR=1,1-->OK

7. 同服务器建立TCP连接:
    AT+CIPSTART="TCP",
    "3E76RFZM92.iotcloud.tencentdevices.com",
    "1883"-->CONNECT

8. 若⑦失败>同服务器断开TCP连接AT+CIPCLOSE-->CLOSE OK