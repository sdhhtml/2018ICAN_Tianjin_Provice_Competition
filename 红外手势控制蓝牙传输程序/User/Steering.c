#include "Steering.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"
PD Steer;
void PD_init(PD *pdvar)
{
    pdvar->setpoint = 120;  
    pdvar->error = 0;
    pdvar->last_error = 0;
    pdvar->duty = 120;
    pdvar->last_output=0;
		
    pdvar->Proportion_1 = 0.5;
    pdvar->Derivative_1 = 0.1;    
}

void PD_cal(PD *pdvar,int Mid)
{

    pdvar->error =   Mid-pdvar->setpoint;   
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
		TIM_SetCompare2(TIM2,pdvar->duty);
	 pdvar->last_output=pdvar->duty;
}

void TIM2_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设
							 
}





//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM2_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
//	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5    
 
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	// 互补输出禁能
//	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; 	
	// 设置占空比大小
//	TIM_OCInitStructure.TIM_Pulse = 30;


  TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	// 输出通道空闲电平极性配置
//	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;	

  TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC1

	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
//	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
    //使能TIM1预装载寄存器
  TIM_ARRPreloadConfig(TIM2, ENABLE);  
    //设置中断源，只有溢出时才中断
  TIM_UpdateRequestConfig(TIM2,TIM_UpdateSource_Regular);
	// 清除中断标志位
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    // 使能中断
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	TIM_Cmd(TIM2, ENABLE);  //使能TIM3
	

}
void Steeringtest(void)//2500下
{
	int i;
		for(i=1100;i<2500;i+=100){//2300  1200
		TIM_SetCompare2(TIM2,i);Delay_ms(500);printf("%d\r\n",i);	 }
		i=2400;
		for(;i>1000;i-=100)
		{
			TIM_SetCompare2(TIM2,i);Delay_ms(500);	 printf("%d\r\n",i);
		}
}

