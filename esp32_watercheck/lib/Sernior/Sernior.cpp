#include <Arduino.h>
#include <Sernior.h>
/*
    XJF DC12V/Z-5 5W/J2Z-H62-N2
*/

volatile int flow_frequency1; // 脉冲频率
volatile int flow_frequency2; // 脉冲频率
float l_minute1,l_minute2;//中间变量
unsigned char flowsensor_1 = 16; //脉冲输入引脚
unsigned char flowsensor_2 = 15; //脉冲输入引脚
unsigned char ControlPin_1 = 10; //输出控制引脚
unsigned char ControlPin_2 = 11; //输出控制引脚


/*中断触发回调函数*/
void flew1()
{  
    flow_frequency1++;  
}
void flew2()
{  
    flow_frequency2++;  
}

/*功能函数*/
void  Data::Sernior_init()//初始化硬件
{
    pinMode(flowsensor_1,INPUT);//初始化输入信号引脚
    pinMode(flowsensor_2,INPUT);

    pinMode(ControlPin_1,OUTPUT);//初始化输出控制引脚
    pinMode(ControlPin_2,OUTPUT);

    digitalWrite(flowsensor_1,HIGH);//信号引脚初始化为高，不对就修改
    digitalWrite(flowsensor_2,HIGH);

    digitalWrite(ControlPin_1,LOW);//控制引脚初始化为低，不对就修改
    digitalWrite(ControlPin_2,LOW);

    attachInterrupt(digitalPinToInterrupt(flowsensor_1), flew1, RISING); // Setup Interrupt
    attachInterrupt(digitalPinToInterrupt(flowsensor_2), flew2, RISING); // Setup Interrupt
}

float Data::GET_flow1()//获取传感器1流量
{

    if(flow_frequency1 != 0)
    {
        // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
        l_minute1 = (flow_frequency1 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
        l_minute1 = l_minute1/60;
        opo1_t.flow_1 = opo1_t.flow_1 +l_minute1;
        flow_frequency1 = 0; // Reset Counter
        return opo1_t.flow_1;
    }
    else
    {
        printf("flow1 rate = 0\n");

    }

    return 0;

}
float Data::GET_flow2()//获取传感器2流量
{
    if(flow_frequency2 != 0)
    {
        // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
        l_minute2 = (flow_frequency2 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
        l_minute2 = l_minute2/60;
        opo1_t.flow_2 = opo1_t.flow_2 +l_minute2;
        flow_frequency2 = 0; // Reset Counter
        return opo1_t.flow_2;
    }
    else
    {
        printf("flow2 rate = 0\n");

    }
    return 0;
}
float Data::GET_water_diffenert()//h获取传感器差值
{
    opo1_t.Water_Diffenert = opo1_t.flow_1 - opo1_t.flow_2;
    return opo1_t.Water_Diffenert;
}

void Data::close_fa()//手动关闭阀
{
    digitalWrite(ControlPin_1,LOW);
    digitalWrite(ControlPin_2,LOW);


}
void Data::open_fa()//手动开启阀
{
    digitalWrite(ControlPin_1,HIGH);
    digitalWrite(ControlPin_2,HIGH);



}
void Data::set_water_diffenert_target(float value)//设置目标流量差
{
    this->water_diffenert_target = value;
    
}
float Data::GET_water_diffenert_target()//获取目标流量差
{

    return this->water_diffenert_target;
}

void Data::Auto_fa(float water_diffenert_target,fun cb)//自动关闭阀，触发条件：水的流量差达到目标值. 参数：目标值
{
    if(opo1_t.Water_Diffenert >= water_diffenert_target)
    {
        digitalWrite(ControlPin_1,HIGH);
        digitalWrite(ControlPin_2,HIGH);
        cb();//其他功能函数
    }  
    else
    {
        printf("一切安好\n");

    }
}
float Data::GET_flow_1()//获取传感器1的流量
{
    return this->flow_1;
    
}
float Data::GET_flow_2()//获取传感器2的流量
{
    return this->flow_2;

}
float Data::GET_Water_Diffenertex()//获取实际流量差
{

    return this->Water_Diffenert;
}



