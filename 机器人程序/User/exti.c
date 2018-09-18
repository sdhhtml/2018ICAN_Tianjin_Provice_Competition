#include "exti.h"
#include "bsp_led.h"  
#include "key.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"
extern int tim1_step;
extern unsigned char tim1_mode;
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	
  /* 配置NVIC为优先级组1 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置中断源：按键1 */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  /* 配置抢占优先级 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  /* 配置子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  /* 使能中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* 配置中断源：按键2，其他使用上面相关配置 */  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
  /* 配置抢占优先级 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  /* 配置子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  /* 使能中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
  NVIC_Init(&NVIC_InitStructure);	
  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
  /* 配置抢占优先级 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  /* 配置子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
  /* 配置抢占优先级 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  /* 配置子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  /* 使能中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
  NVIC_Init(&NVIC_InitStructure);

}
void EXTI_Key_Config(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;



												
	/* 配置 NVIC 中断*/

	/* 选择EXTI的信号源 */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); 
  EXTI_InitStructure.EXTI_Line =EXTI_Line0;
	
	/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 上升沿中断 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  /* 使能中断 */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	/* 选择EXTI的信号源 */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource2); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	
	/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 下降沿中断 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  /* 使能中断 */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  /*--------------------------KEY3配置-----------------------------*/

	/* 选择EXTI的信号源 */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3); 
  EXTI_InitStructure.EXTI_Line =EXTI_Line3;
	
	/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 下降沿中断 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  /* 使能中断 */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

	/* 选择EXTI的信号源 */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	
	/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 下降沿中断 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  /* 使能中断 */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);	


}
void EXTIX_Init(void)
{
	NVIC_Configuration();
	EXTI_Key_Config();
}
//外部中断0服务程序 
void EXTI0_IRQHandler(void)
{
	delay_msmoni(10);
	if(WK_UP==1)	 	 //WK_UP按键
	{		
		LED0=!LED0;	
		PCout(1)=0;//顺
		PCout(2)=1;//顺
		delay_msmoni(5000);	
		LED1=!LED1;
		PCout(1)=0;//顺
		PCout(2)=0;//顺		
		delay_msmoni(5000);	
		PCout(1)=1;//顺
		PCout(2)=1;//顺
		LED0=!LED0;
		LED1=!LED1; 		

	}
	EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位  
}
 
//外部中断2服务程序
void EXTI2_IRQHandler(void)
{
	delay_msmoni(10);
	if(KEY2==0)	  //按键KEY2
	{
		LED0=!LED0;
		PCout(1)=0;//顺
		PCout(2)=1;//顺
		tim1_mode=1;
	}		 
	EXTI_ClearITPendingBit(EXTI_Line2);  //清除LINE2上的中断标志位  
}
//外部中断3服务程序
void EXTI3_IRQHandler(void)
{
	delay_msmoni(10);
	if(KEY1==0)	 //按键KEY1
	{				 
		LED1=!LED1;
		PCout(1)=0;//顺
		PCout(2)=0;//顺
		tim1_mode=0;
	}		 
	EXTI_ClearITPendingBit(EXTI_Line3);  //清除LINE3上的中断标志位  
}

void EXTI4_IRQHandler(void)
{
	delay_msmoni(10);
	if(KEY0==0)	 //按键KEY0
	{
		LED0=!LED0;
		LED1=!LED1; 
		PCout(1)=1;//顺
		PCout(2)=1;//顺
		tim1_mode=2;
	}		 
	EXTI_ClearITPendingBit(EXTI_Line4);  //清除LINE4上的中断标志位  
}
 

