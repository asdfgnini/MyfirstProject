#include <Arduino.h>
#include "Sernior.h"
/*
    XJF DC12V/Z-5 5W/J2Z-H62-N2
*/
char buf[1024];
SemaphoreHandle_t sem  =  xSemaphoreCreateBinary();
void Printf(State message)
{
    sprintf(buf,"fas_working:%d,dloes_working%d," ,message.faS_workIng,message.flowS_workIng);
    //{faS_workIng:1,flowS_workIng:1}


}

/*中断触发回调函数*/
void State::flew1()
{  
   flow1_frequency++;  
}
void State::flew2()
{  
    flow2_frequency++;  
}

void State::delete_Task()
{
    vTaskDelete(Task_con_Hander);
}
void State::change_Fas_State(bool num)
{
    this->faS_workIng = num;
    if(num == true)
    {
        digitalWrite(this->plusein_pin_1,HIGH);//信号引脚初始化为高，不对就修改
        digitalWrite(this->plusein_pin_2,HIGH);
        attachInterrupt(digitalPinToInterrupt(this->plusein_pin_1), this->flew1, RISING); // Setup Interrupt
        attachInterrupt(digitalPinToInterrupt(this->plusein_pin_2), this->flew2, RISING); // Setup Interrupt
        this->flowS_workIng = true;
    }
    else
    {
        digitalWrite(this->controlPin_1,LOW);//信号引脚初始化为高，不对就修改
        digitalWrite(this->controlPin_2,LOW);
        detachInterrupt(this->plusein_pin_1);
        detachInterrupt(this->plusein_pin_1); 
        this->flowS_workIng = false;
    }
}

void Task_message(void* arg)
{
    StatePtr* state = (StatePtr*)arg;
    
    for(;;)
    {
        if(xSemaphoreTake(sem,3000) ==pdPASS)
        {
            state->Con_state.cb_message(state);

            vTaskDelay(2000);

        }
    }
}

void Task_control(void* arg)
{
    float l_minute1,l_minute2;//中间变量

    StatePtr* state = (StatePtr*)arg;
    TickType_t ticks = xTaskGetTickCount();
    
    for(;;)
    {            
        bzero(state->error_message,20);
        state->xTaskGetTickCount_v =  xTaskGetTickCount();
        state->Pluse_Different = state->flow1_frequency-state->flow2_frequency;
        state->Pluse_Different = abs(state->Pluse_Different);
        if(state->Con_state.aotoFas_type == 0)//脉冲差
        {
            if(state->Pluse_Different != 0)
            {
                if(state->Pluse_Different >= state->Con_state.aotoFas_value)
                {
                    state->change_Fas_State(false);
                    state->Con_state.cb_stop(state);
                }
                else
                {
                    state->Con_state.cb_work(state);
                }
            }
            else
            {
                strcpy(state->error_message,"警报,脉冲差为0\n");
                state->Con_state.cb_error(state);
            }
            
        }
        else if(state->Con_state.aotoFas_type == 1)//水流量
        {
            if(state->flow1_frequency != 0 && state->flow2_frequency != 0)
            {
                // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
                l_minute1 = (state->flow1_frequency / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
                l_minute1 = l_minute1/60;
                state->flow_1 = state->flow_1 +l_minute1;
                state->flow1_frequency = 0; // Reset Counter                

                // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
                l_minute2 = (state->flow2_frequency / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
                l_minute2 = l_minute2/60;
                state->flow_2 = state->flow_2 +l_minute2;
                state->flow2_frequency = 0; // Reset Counter

                state->Water_Diffenert = state->flow_1 - state->flow_2;
                if(state->Water_Diffenert != 0)
                {
                    if(state->Water_Diffenert >= state->Con_state.aotoFas_value)
                    {
                        state->change_Fas_State(false);
                        state->Con_state.cb_stop(state);
                    }
                    else
                    {
                        state->Con_state.cb_work(state);

                    }
                }
                else
                {
                    strcpy(state->error_message,"警报,流量差为0\n");
                    state->Con_state.cb_error(state);
                }
            }
            else
            {
                    strcpy(state->error_message,"警报,至少有一边脉冲为0\n");
                    state->change_Fas_State(false);
                    state->Con_state.cb_stop(state);
                    state->Con_state.cb_error(state);
            }
        }
        xSemaphoreGive(sem);
        vTaskDelayUntil(&ticks,state->Con_state.vTaskDelayUntil_param2);

    }
}




/*功能函数*/
void  Data::Sernior_init(int plusein_pin_1,int plusein_pin_2,int controlPin_1,int controlPin_2)//初始化硬件
{
    pinMode(plusein_pin_1,INPUT);//初始化输入信号引脚
    pinMode(plusein_pin_2,INPUT);

    pinMode(controlPin_1,OUTPUT);//初始化输出控制引脚
    pinMode(controlPin_2,OUTPUT);

    digitalWrite(plusein_pin_1,HIGH);//信号引脚初始化为高，不对就修改
    digitalWrite(plusein_pin_2,HIGH);

    digitalWrite(controlPin_1,LOW);//控制引脚初始化为低，不对就修改
    digitalWrite(controlPin_2,LOW);

    attachInterrupt(digitalPinToInterrupt(plusein_pin_1), this->stat_1.flew1, RISING); // Setup Interrupt
    attachInterrupt(digitalPinToInterrupt(plusein_pin_2), this->stat_1.flew2, RISING); // Setup Interrupt
    this->stat_1.flowS_workIng = true;
    this->stat_1.faS_workIng = true;
}


float State::frequencyToWater_h(float flow_frequency)
{
    return (flow_frequency * 60) / 7.5;
}
float State::frequencyToWater_m(float flow_frequency)
{
    return flow_frequency / 7.5;
}
float State::frequencyToWater_s(float flow_frequency)
{
    return flow_frequency / 7.5 / 60;
}

void DATA::init(CONFIG Conf_in)//重建任务
{
    this->Sernior_init(Conf_in.plusein_pin_1,Conf_in.plusein_pin_2,Conf_in.controlPin_1,Conf_in.controlPin_2);
    this->stat_1.Con_state = Conf_in;
    this->stat_1.plusein_pin_1 = Conf_in.plusein_pin_1;
    this->stat_1.plusein_pin_2 = Conf_in.plusein_pin_2;
    this->stat_1.controlPin_1 = Conf_in.controlPin_1;
    this->stat_1.controlPin_2 = Conf_in.controlPin_2;
    xTaskCreatePinnedToCore(Task_control,"Task_control",1024*5,(void*)&this->stat_1,9,&Task_con_Hander,Conf_in.cpu);
    xTaskCreatePinnedToCore(Task_message,"Task_message",1024*5,(void*)&this->stat_1,1,NULL,1);
}
DATA::DATA(CONFIG Conf_in)
{
    init(Conf_in);
    
}
void DATA::re_struct_task(CONFIG Conf_in)//重建任务
{
    vTaskDelete(Task_con_Hander);
    vTaskDelay(100);   
    init(Conf_in);
}

