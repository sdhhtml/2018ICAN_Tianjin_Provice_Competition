#ifndef __TIM8_H_
#define __TIM8_H_
#include <stm32f10x.h>
#define DUTY_MAX  4900
#define DUTY_MIN  2900      
#define DUTY_MID  3900
#define Coefficient_Trh    10               //定义二次偏差系数阀值 

typedef struct PD {
 
   //  int setpoint;       //设定值
     int point;
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


void TIM8_Init(void);
void Steeringtest(void);
void TIM8Comtrol(void);
#endif
