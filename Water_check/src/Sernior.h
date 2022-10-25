#pragma once
#include <Arduino.h>

/*
    水流量传感器：
    XJF DC12V/Z-5 5W/J2Z-H62-N2

*/

typedef void(*fun)();
typedef class DATA
{
public:
//获取各种传感器数据
    float GET_flow1();//获取传感器1流量
    float GET_flow2();//获取传感器2流量
    float GET_water_diffenert();//h获取传感器差值
    void  Sernior_init();//初始化硬件
//控制
    void close_fa();//手动关闭阀
    void open_fa();//手动开启阀
    void Auto_fa(float water_diffenert_target,fun cb);//自动关闭阀，触发：水的流量差达到目标值
//数据接口
    void set_water_diffenert_target(float value);//设置目标流量差
    float GET_water_diffenert_target();//获取目标流量差
    float GET_flow_1();//传感器1的流量
    float GET_flow_2();//传感器2的流量
    float GET_Water_Diffenertex(); //实际流量差

private:
    float flow_1;//传感器1的流量
    float flow_2;//传感器2的流量
    float Water_Diffenert; //实际流量差

    float water_diffenert_target;//目标流量差

    int fa1_state;//阀门1状态 0:关 1：开
    int fa2_state;//阀门2状态 0:关 1：开

}Data;


//上报数据结构体
typedef struct DATA_UP
{
    float flow_1;//传感器1的流量
    float flow_2;//传感器2的流量
    float Water_Diffenert; //实际流量差
    
}Data_up;

