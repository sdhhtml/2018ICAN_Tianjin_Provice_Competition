#ifndef __STEERING_H
#define __STEERING_H
#include "stm32f10x.h"
#define DUTY_MAX  2400
#define DUTY_MIN  1100      
#define DUTY_MID  1750
#define Coefficient_Trh    10               //定义二次偏差系数阀值 

typedef struct PD {
 
     unsigned char setpoint;          //设定值
     
     float Proportion_1;        //比例系数
     float Derivative_1;        //微分系数
		 int last_output;
		
     
     int error;         //偏差
     int last_error;        //上一次偏差
     int Gyro_error;
     int duty;             //占空比的值
}PD;
//初始化PD参数
void PD_init(PD *pdvar);
//PD的计算
void PD_cal(PD *pdvar,int Mid);


void TIM2_Int_Init(u16 arr,u16 psc);
void TIM2_PWM_Init(u16 arr,u16 psc);
void Steeringtest(void);
#endif

