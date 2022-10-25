#include <Arduino.h>
#include <Sernior.h>
Data opo1_t;//实例化对象


Data_up data[256];//数据上传数组

void func()//函数名可自定义
{
    //功能
}

void Task1(void* arg)//函数名可自定义
{
    int i = 0;
    const TickType_t fre = 1000;//1s执行一次，使得测量更准确
    TickType_t count = xTaskGetTickCount();
    for(;;)
    {
        opo1_t.GET_flow1();
        opo1_t.GET_flow2();
        opo1_t.GET_water_diffenert();
        data[i].flow_1 = opo1_t.GET_flow_1();
        data[i].flow_2 = opo1_t.GET_flow_2();
        data[i].Water_Diffenert = opo1_t.GET_water_diffenert();
        i++;
        i = i % 256;
        vTaskDelayUntil(&count,fre);
    }
}

void Task2(void* arg)//函数名可自定义
{
    for(;;)
    {
        opo1_t.Auto_fa(opo1_t.GET_water_diffenert_target(),func);
        vTaskDelay(100);
    }
}

void setup()
{
    opo1_t.Sernior_init();//硬件初始化

    xTaskCreatePinnedToCore(Task1,"task1",1024*4,NULL,1,NULL,1);//用于检测水流量，计算水流量差值,每1s计算一次
    xTaskCreatePinnedToCore(Task2,"task2",1024*4,NULL,1,NULL,1);//当水流量达到目标值，执行控制


    vTaskDelete(NULL);
}

void loop()
{

}