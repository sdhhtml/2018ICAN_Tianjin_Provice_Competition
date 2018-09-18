#include "TIM8.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"
#include "MicroStepDriver.h" 
PD Steer;
int steerduty=3900;
void PD_init(PD *pdvar)
{
    pdvar->point = 120;  
    pdvar->error = 0;
    pdvar->last_error = 0;
    pdvar->duty = 120;
    pdvar->last_output=0;
		
    pdvar->Proportion_1 = 0.5;
    pdvar->Derivative_1 = 0.1;    
}

void PD_cal(PD *pdvar,int Mid)
{

    pdvar->error =   Mid-pdvar->point;   
 //   pdvar->Gyro_error=(GetData(GYRO_XOUT_H)>>3)+5;
    
    //if(Speed_UP == 1 || Speed_Mid == 1)
    //{
        pdvar->duty = pdvar->Proportion_1 * pdvar->error
              //      - pdvar->Derivative_1*pdvar->Gyro_error
                      + pdvar->last_output;    
   if(pdvar->duty >= DUTY_MAX)                                //舵机PID防止最高和最低溢出。
     pdvar->duty = DUTY_MAX;
   else if(pdvar->duty <= DUTY_MIN)
      pdvar->duty = DUTY_MIN; 
		TIM_SetCompare3(TIM8,pdvar->duty);
	 pdvar->last_output=pdvar->duty;
}
static void ADVANCE_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // 输出比较通道 GPIO 初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}


///*
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM信号 周期和占空比的计算--------------- */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = (ARR+1) * (1/CLK_cnt) = (ARR+1)*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)

static void ADVANCE_TIM_Mode_Config(void)
{
  // 开启定时器时钟,即内部时钟CK_INT=72M
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);

/*--------------------时基结构体初始化-------------------------*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period=19999;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= 35;	//144mhz
	//1000 000
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
  TIM_ClearITPendingBit(TIM8,TIM_IT_Update);
	TIM_UpdateRequestConfig(TIM8,TIM_UpdateSource_Regular); //设置只有计数溢出作为更新中断
	/*--------------------输出比较结构体初始化-------------------*/		
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	// 配置为PWM模式1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// 输出使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// 互补输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Disable; 
	// 设置占空比大小
	TIM_OCInitStructure.TIM_Pulse = 1000;
	// 输出通道电平极性配置
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	// 输出通道空闲电平极性配置
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	// 互补输出通道空闲电平极性配置
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

	TIM_OC1Init(TIM8, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM8, ENABLE); //使能TIMx在ARR上的预装载寄存器
	TIM_CtrlPWMOutputs(TIM8, ENABLE);
	
	// 使能计数器
	TIM_Cmd(TIM8, ENABLE);	
	// 主输出使能，当使用的是通用定时器时，这句不需要

}
void TIM8_NVIC_Config(void)
{

	NVIC_InitTypeDef NVIC_InitStructure;


 
	TIM_ITConfig(TIM8,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器


							 
}
void TIM8_Init(void)
{
ADVANCE_TIM_GPIO_Config();	
	ADVANCE_TIM_Mode_Config();
	//TIM8_NVIC_Config();
}
void Steeringtest(void)//2500下
{
	int iii;
		for(iii=2900;iii<4900;iii+=100){//2300  1200
		TIM_SetCompare3(TIM8,iii);delay_msmoni(300);printf("%d\r\n",iii);	 }
		iii=4900;
		for(;iii>2900;iii-=100)
		{
			TIM_SetCompare3(TIM8,iii);delay_msmoni(300);	 printf("%d\r\n",iii);
		}
}
void TIM8Comtrol(void)
{
		if(status.up==1)
	{
		steerduty+=100;
		if(steerduty>4900)steerduty=4900;
		TIM_SetCompare3(TIM8,steerduty);
		

		status.up=0;
	}
	if(status.down==1)
	{
		steerduty-=100;
		if(steerduty<2900)steerduty=2900;
		TIM_SetCompare3(TIM8,steerduty);
		status.down=0;
	}
}
