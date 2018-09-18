/***************************************************************************************
                                      声  明
    本项目代码仅供个人学习使用，可以自由移植修改，但必须保留此声明信息。移植过程中出现
其他不可估量的BUG，天际智联不负任何责任。请勿商用！

程序版本：2.1
更新日期：2018-1-23
程序作者：愤怒的小孩，东方萧雨
版权所有：西安天际智联信息技术有限公司
****************************************************************************************/
/********************************************************************************
点亮遥控器上的LED灯

注意：
1.当要使用LED_CyclieOn(...)函数循环点亮LED1和LED2灯时，请先初始化SysTick_init()

主函数初始化代码：
	LED_Config();
	
测试代码为：
	SysTick_init();
	LED_Config();
	LED_CyclieOn(1000);
	
	while(1){}
*******************************************************************************/
#include "led.h"
#include "systick.h"

void LED_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructus;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructus.GPIO_Pin = LED1;
	GPIO_InitStructus.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructus.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC,&GPIO_InitStructus);
	
	LED_Off(LED1);
}

//点亮LED灯
void LED_On(u16 LEDx)
{
	GPIO_ResetBits(GPIOA,LEDx);
}

void LED_Off(u16 LEDx)
{
	GPIO_SetBits(GPIOA,LEDx);
}


//开关LED
void LED_Toggle(u16 LEDx)
{
	GPIOA->ODR ^= LEDx;		//异或，开关模式输出高低电平
}


////循环点亮LED
//void LED_CyclieOn(u32 delayms)
//{	
//	while(1){
//		LED_On(LED1);
//		delay_ms(delayms);
//		LED_Off(LED1);
//		
//		LED_On(LED2);
//		delay_ms(delayms);
//		LED_Off(LED2);
//	}
//}


