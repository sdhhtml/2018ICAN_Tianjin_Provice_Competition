#include "ultrasonic.h"
#include "bsp_usart.h"
uint32_t TIM_PscCLK = 1000000;
//TIM2
// 定时器输入捕获用户自定义变量结构体定义
TIM_ICUserValueTypeDef ultrasonic1 = {0,0,0,0,0,0};
TIM_ICUserValueTypeDef ultrasonic2 = {0,0,0,0,0,0};
TIM_ICUserValueTypeDef ultrasonic3 = {0,0,0,0,0,0};
TIM_ICUserValueTypeDef ultrasonic4 = {0,0,0,0,0,0};
// 中断优先级配置
static void TIM2_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn ;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void TIM2_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // 输入捕获通道 GPIO 初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
 	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //使能PB,PE端口时钟
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_6;				 //LED0-->PB.5 端口配置
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOE, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	 GPIO_ResetBits(GPIOE,GPIO_Pin_0);						 //Pe0低	
	 GPIO_ResetBits(GPIOE,GPIO_Pin_1);						 //Pe0低	
	 GPIO_ResetBits(GPIOE,GPIO_Pin_6);						 //Pe0低		 
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
// PWM 信号的周期 T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)

static void TIM2_Mode_Config(void)
{
  // 开启定时器时钟,即内部时钟CK_INT=72M
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

/*--------------------时基结构体初始化-------------------------*/	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period=0XFFFF;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= 71;	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/*--------------------输入捕获结构体初始化-------------------*/	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	// 配置输入捕获的通道，需要根据具体的GPIO来配置
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	// 输入捕获信号的极性配置
	TIM_ICInitStructure.TIM_ICPolarity = GENERAL_TIM_STRAT_ICPolarity;
	// 输入通道和捕获通道的映射关系，有直连和非直连两种
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// 输入的需要被捕获的信号的分频系数
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// 输入的需要被捕获的信号的滤波系数
	TIM_ICInitStructure.TIM_ICFilter = 0;
	// 定时器输入捕获初始化
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	// 清除更新和捕获中断标志位
  TIM_ClearFlag(TIM2, TIM_FLAG_Update|TIM_IT_CC1);	
  // 开启更新和捕获中断  
	TIM_ITConfig (TIM2, TIM_IT_Update | TIM_IT_CC1, ENABLE );
	#if 0
	TIM_ClearFlag(TIM2, TIM_FLAG_Update|TIM_IT_CC2);	
  // 开启更新和捕获中断  
	TIM_ITConfig (TIM2, TIM_IT_Update | TIM_IT_CC2, ENABLE );
	TIM_ClearFlag(TIM2, TIM_FLAG_Update|TIM_IT_CC3);	
  // 开启更新和捕获中断  
	TIM_ITConfig (TIM2, TIM_IT_Update | TIM_IT_CC3, ENABLE );
	TIM_ClearFlag(TIM2, TIM_FLAG_Update|TIM_IT_CC4);	
  // 开启更新和捕获中断  
	TIM_ITConfig (TIM2, TIM_IT_Update | TIM_IT_CC4, ENABLE );
#endif

	TIM_ClearFlag(TIM2, TIM_FLAG_Update|TIM_IT_CC3);	
  // 开启更新和捕获中断  
	TIM_ITConfig (TIM2, TIM_IT_Update | TIM_IT_CC3, ENABLE );
	TIM_ClearFlag(TIM2, TIM_FLAG_Update|TIM_IT_CC4);	
  // 开启更新和捕获中断  
	TIM_ITConfig (TIM2, TIM_IT_Update | TIM_IT_CC4, ENABLE );
	// 使能计数器
	TIM_Cmd(TIM2, ENABLE);
}

void TIM2_Init(void)
{
	TIM2_GPIO_Config();
	TIM2_NVIC_Config();
	TIM2_Mode_Config();		
}
void CSB1(void)
{
						if(ultrasonic1.Capture_FinishFlag == 1)
							{
								// 计算高电平时间的计数器的值
								ultrasonic1.time = ultrasonic1.Capture_Period * (0XFFFF+1) + 
											 (ultrasonic1.Capture_CcrValue+1);
								ultrasonic1.distance=ultrasonic1.time%TIM_PscCLK*340/2/1000;
								// 打印高电平脉宽时间
								//printf ( "\r\n测得高电平脉宽时间：%d.%d s\r\n",time/TIM_PscCLK,time%TIM_PscCLK );
								printf ( "\r\n测量距离1：%d mm\r\n",ultrasonic1.distance );
								ultrasonic1.Capture_FinishFlag = 0;			
							}	
}
#if 1
void CSB2(void)
{
						if(ultrasonic2.Capture_FinishFlag == 1)
							{
								// 计算高电平时间的计数器的值
								ultrasonic2.time = ultrasonic2.Capture_Period * (0XFFFF+1) + 
											 (ultrasonic2.Capture_CcrValue+1);
								ultrasonic2.distance=ultrasonic2.time%TIM_PscCLK*340/2/1000;
								// 打印高电平脉宽时间
								//printf ( "\r\n测得高电平脉宽时间：%d.%d s\r\n",time/TIM_PscCLK,time%TIM_PscCLK );
								printf ( "\r\n测量距离2：%d mm\r\n",ultrasonic2.distance );
								ultrasonic2.Capture_FinishFlag = 0;			
							}	
}
void CSB3(void)
{
						if(ultrasonic3.Capture_FinishFlag == 1)
							{
								// 计算高电平时间的计数器的值
								ultrasonic3.time = ultrasonic3.Capture_Period * (0XFFFF+1) + 
											 (ultrasonic3.Capture_CcrValue+1);
								ultrasonic3.distance=ultrasonic3.time%TIM_PscCLK*340/2/1000;
								// 打印高电平脉宽时间
								//printf ( "\r\n测得高电平脉宽时间：%d.%d s\r\n",time/TIM_PscCLK,time%TIM_PscCLK );
								printf ( "\r\n测量距离3：%d mm\r\n",ultrasonic3.distance );
								ultrasonic3.Capture_FinishFlag = 0;			
							}	
}
void CSB4(void)
{
						if(ultrasonic4.Capture_FinishFlag == 1)
							{
								// 计算高电平时间的计数器的值
								ultrasonic4.time = ultrasonic4.Capture_Period * (0XFFFF+1) + 
											 (ultrasonic4.Capture_CcrValue+1);
								ultrasonic4.distance=ultrasonic4.time%TIM_PscCLK*340/2/1000;
								// 打印高电平脉宽时间
								//printf ( "\r\n测得高电平脉宽时间：%d.%d s\r\n",time/TIM_PscCLK,time%TIM_PscCLK );
								printf ( "\r\n测量距离4：%d mm\r\n",ultrasonic4.distance );
								ultrasonic4.Capture_FinishFlag = 0;			
							}	
}
#endif

