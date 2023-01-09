#include <Arduino.h>
#include "A7129.h"//引入头文件
RF_A7129 RF1;//创建对象

void setup() 
{
 Serial.begin(115200);
}

void loop() 
{
 
    if(RF_A7129_Master)//判断是否是主机
    {
       RF1.RF_A7129_Send();//发送数据
    }
    else
    {
        RF1.RF_A7129_Receive();//接收数据
    }
}