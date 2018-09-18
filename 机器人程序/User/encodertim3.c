#include "encodertim3.h"
volatile unsigned int CaptureNumberA1=0;
volatile unsigned int CaptureNumberA2=0;
volatile unsigned int CaptureNumberB1=0;
volatile unsigned int CaptureNumberB2=0;
static void ENCODER_TIM3_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* 使能定时器通道引脚GPIO时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 

  /* 配置定时器通道1引脚模式： */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}
static void TIM3_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 选择中断优先级配置组为2个抢占式优先级和2个子优先级，可以参考misc.h文件了解相关设置 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置TIM3_IRQ中断为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	/* 设置抢占式优先级为0 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	/* 设置子优先级为2 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	/* 使能外部中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
/**
  * 函数功能: 配置TIMx输出的PWM信号的模式，如周期、极性、占空比
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> TIMxCNT 重新计数
 *                    TIMx_CCR(电平发生变化)
 * 信号周期=(TIMx_ARR +1 ) * 时钟周期
 * 占空比=TIMx_CCR/(TIMx_ARR +1)
 *    _______    ______     _____      ____       ___        __         _
 * |_|       |__|      |___|     |____|    |_____|   |______|  |_______| |________|
 *
 */
static void ENCODER_TIM3_Configuration(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef        TIM_ICInitStructure;

  /* 使能定时器时钟 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
  /* 定时器基本参数始终 */		 
  /* 定时周期: ENCODER_TIM_Period+1 */
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF; 
  /* 设置预分频：ENCODER_TIM_Prescaler,输出脉冲频率：72MHz/(ENCODER_TIM_Prescaler+1)/(ENCODER_TIM_Period+1) */
  TIM_TimeBaseStructure.TIM_Prescaler = 71;
  /* 设置时钟分频系数：不分频(这里用不到) */
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
  /* 向上计数模式 */
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* 初始化TIM5输入捕获参数 */
  /* CC1S=01 	选择输入端 IC1映射到TI1上 */
	TIM_ICInitStructure.TIM_Channel     = TIM_Channel_1;
  /* 上升沿捕获 */
	TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;	
  /* 映射到TI1上 */
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
  /* 配置输入分频,不分频  */
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	           
  /* IC1F=0000 配置输入滤波器 不滤波 */
	TIM_ICInitStructure.TIM_ICFilter    = 0x00;                         
	TIM_ICInit(TIM3, &TIM_ICInitStructure);  
	TIM_ICInitStructure.TIM_Channel     = TIM_Channel_2;
  TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel     = TIM_Channel_3;
  TIM_ICInit(TIM3, &TIM_ICInitStructure);	
	TIM_ICInitStructure.TIM_Channel     = TIM_Channel_4;
  TIM_ICInit(TIM3, &TIM_ICInitStructure);		
  /* 配置NVIC */
  TIM3_NVIC_Configuration();
  
  /* 允许更新中断 ,允许CC1IE捕获中断	 */
  TIM_ITConfig(TIM3,TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM3,TIM_IT_CC2, ENABLE);
	TIM_ITConfig(TIM3,TIM_IT_CC3, ENABLE);
	TIM_ITConfig(TIM3,TIM_IT_CC4, ENABLE);
	
  /* 使能定时器 */
  TIM_Cmd(TIM3, ENABLE);							
}
/**
  * 函数功能: TIMx通道1输入捕获初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void TIM3_DCmotor_ENCODER_Init(void)
{
	ENCODER_TIM3_GPIO_Config();
	ENCODER_TIM3_Configuration();	
}
