#include "TIM7.h" 

// 中断优先级配置
static void TIM7_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn ;	
		// 设置主优先级为 1
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{ TIM_Prescaler            都有
 *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
 *  TIM_Period               都有
 *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
 *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
 *}TIM_TimeBaseInitTypeDef; 
 *-----------------------------------------------------------------------------
 */


static void TIM7_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// 开启定时器时钟,即内部时钟CK_INT=36M
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	
		// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period =999;	

	  // 时钟预分频数为
    TIM_TimeBaseStructure.TIM_Prescaler= 71;//36M 36 00 0 000
		//1/100000*20*36= 72 0 00000
	
		// 时钟分频因子 ，基本定时器没有，不用管
    //TIM_TimeBaseStructure.TIM_ClockDivision=0;
		
		// 计数器计数模式，基本定时器只能向上计数，没有计数模式的设置
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// 重复计数器的值，基本定时器没有，不用管
		//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // 初始化定时器
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
		
		// 清除计数器中断标志位
    //TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	  
		// 开启计数器中断
    TIM_ITConfig(TIM6,TIM_IT_Update|TIM_IT_Trigger,ENABLE);
		
		// 使能计数器
    TIM_Cmd(TIM6, ENABLE);	
}

void TIM6_Init(void)
{
	TIM6_NVIC_Config();
	TIM6_Mode_Config();
}