#include "bjdj.h"
void BJDJ_GPIO_Config(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启LED相关的GPIO外设时钟*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3;	

		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIO*/
		GPIO_Init(GPIOC, &GPIO_InitStructure);	
		
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;

		/*调用库函数，初始化GPIO*/
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		


		/* 关闭所有led灯	*/
		GPIO_SetBits(GPIOC,GPIO_Pin_3);
		
		/* 关闭所有led灯	*/
		GPIO_SetBits(GPIOC,GPIO_Pin_4);	 
    

}
void Clockwise(void)//顺时针
{
	PCout(3) =0;
	PCout(4)=1;
}
void Anticlockwise(void)//逆时针
{
	PCout(3) =0;
	PCout(4)=0;
}
void BJDJ_STOP(void)
{
	PCout(3) =1;
	PCout(4)=1;
}
