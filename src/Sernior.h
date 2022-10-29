#pragma once
#include <Arduino.h>

/*
    水流量传感器：
    XJF DC12V/Z-5 5W/J2Z-H62-N2

*/


struct State
{

    bool faS_workIng;               //阀门不通水false||通水true；
    bool flowS_workIng;             //流量传感器使用中true
    TickType_t xTaskGetTickCount_v; //时间戳
    static int flow1_frequency;            // 水流传感器脉冲频率
    static int flow2_frequency;            // 水流传感器脉冲频率
    float Pluse_Different;                  //脉冲差值
    float flow_1;//传感器1的流量
    float flow_2;//传感器2的流量
    float Water_Diffenert; //实际流量差
    int plusein_pin_1;//脉冲引脚1
    int plusein_pin_2;//脉冲引脚2
    int controlPin_1;//控制引脚1
    int controlPin_2;//控制引脚2
    char error_message[20];//错误日志

    // Config Con_state;
    void delete_Task();//挂起任务
    void change_Fas_State(bool);//阀门不通水false||通水true；
    static void flew1();
    static void flew2();
    float frequencyToWater_h(float flow_frequency);
    float frequencyToWater_m(float flow_frequency);
    float frequencyToWater_s(float flow_frequency);
};
typedef void(*CB)(State* abc_1);

struct CONFIG
{
    int aotoFas_type;                  //设定差类型 0=脉冲差frequency||1=流量差water||2=压力差
    int aotoFas_value;                 //设定差值(绝对值)
    TickType_t vTaskDelayUntil_param2; //周期
    int cpu;                           // 0||1

    CB cb_stop;
    CB cb_work;
    CB cb_error;
    CB cb_message;

    int plusein_pin_1;
    int plusein_pin_2;
    int controlPin_1;
    int controlPin_2;

};

struct StatePtr:State{
    CONFIG Con_state;
};

TaskHandle_t Task_con_Hander;


typedef class DATA
{
public:
    DATA(CONFIG Conf_in);
    void re_struct_task(CONFIG Conf_in);//重建任务

private:
    void init(CONFIG Conf_in);
    void  Sernior_init(int i,int j,int k,int m);//初始化硬件
    StatePtr stat_1;
}Data;


