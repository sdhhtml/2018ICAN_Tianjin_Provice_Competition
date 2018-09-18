#include "stm32f10x.h"
#include "bsp_led.h"
#include "MicroStepDriver.h" 
#include "bsp_usart.h"
#include "bsp_usart3.h"
#include "MSD_test.h" 
#include "beep.h"
#include "openmv.h"
#include "bsp_SysTick.h"
#include "timer.h"

#include "ultrasonic.h"
#include "TIM6.h"
#include "encodertim3.h"
#include "TIM4.h"
#include "TIM1.h"
#include "gesture.h"
#include "simulate.h"
#include "lcd.h"
#include "key.h"
#include "exti.h"
#include "bjdjt.h"
#include "TIM8.h"
#include "bjdj.h"
#include "uart4.h"
extern volatile unsigned int CaptureNumberA1;//左前
extern volatile unsigned int CaptureNumberA2;//右前
extern volatile unsigned int CaptureNumberB1;//左后
extern volatile unsigned int CaptureNumberB2;//右后
extern PD Steer;
extern int tim1_step;
int main(void)
{	int i;

    //初始化LED
    LED_GPIO_Config();//
    //初始化BEEP
    //BEEP_GPIO_Config();	
    //初始化按键
  //  EXTI_Key_Config();
    //初始化串口
    USART_Config();
	  USART_Config3();
	  UART_Config4();
	//步进电机初始化
	 // MSD_Init();
		SysTick_Init();//硬件延时
		KEY_Init();	 //按键初始化
		EXTIX_Init();//外部中断初始化
	  TIM1_Init();//定时器1初始化步进电机
	  TIM8_Init();//定时器8初始化舵机无刷电机
	  
		BJDJT_GPIO_Config();//步进电机推的方向和使能的引脚初始化
		BJDJ_GPIO_Config();//旋转的步进电机的方向和使能初始化
	 // LCD_Init();	
	  //TIM2_Init();//超声波134
	  TIM6_Init();//超声波触发
		TIM3_DCmotor_ENCODER_Init();//编码器的的初始化
		IncPIDInit();//pid的初始化
	  TIM4_Init();//减速电机初始化
   	LED0=0;
		LED1=0;
		BJDJ_STOP();
	  PD_init(&Steer);

	 #if 0
 	while(!paj7620u2_init())//PAJ7620U2传感器初始化
	{
	    printf("PAJ7620U2 Error!!!\r\n");
		//LCD_ShowString(30,140,200,16,16,"PAJ7620U2 Error!!!");
	    Delay_ms(500);
		//LCD_ShowString(30,140,200,16,16,"                  ");
		Delay_ms(500);
		LED0=!LED0;//DS0闪烁
		
	}
	#endif
	#if 1
		for(i=900;i<2000;i+=100){//2300  1200

		TIM_SetCompare1(TIM8,i);
		TIM_SetCompare2(TIM8,i);
		Delay_ms(100);

		}	 
				for(i=1100;i<2600;i+=100){//2300  1200

		TIM_SetCompare1(TIM8,i);
		TIM_SetCompare2(TIM8,i);
		Delay_ms(500);

		}
				#endif
    while(1) 
    {
			TIM1Control();
			TIM8Comtrol();
			openmvData();
		//	Clockwise();//顺时针
		//	delay_msmoni(5000);
		//	Anticlockwise();//逆时针
		//	delay_msmoni(5000);
		//	BJDJ_STOP();
		//	delay_msmoni(5000);
		//	Steeringtest();
//TIM_SetCompare1(TIM8,10000);
//TIM_SetCompare2(TIM8,10000);
			

	}	 
        //处理串口数据
			
			//printf("freq1:%d\r\n",CaptureNumberA1);  // 打印编码器捕获值
    //  CaptureNumberA1=0;    // 清零，从零开始计数
		//	printf("freq2:%d\r\n",CaptureNumberA2);  // 打印编码器捕获值
    //  CaptureNumberA2=0;    // 清零，从零开始计数
		//	printf("freq3:%d\r\n",CaptureNumberB1);  // 打印编码器捕获值
    //  CaptureNumberB1=0;    // 清零，从零开始计数
		//	printf("freq4:%d\r\n",CaptureNumberB2);  // 打印编码器捕获值
    //  CaptureNumberB2=0;    // 清零，从零开始计数				
			//LED1( ON );	
	//		LED2(OFF);
		//	Delay_ms(100);
			//paj7620u2_sensor_test();//PAJ7620U2传感器测试
			//Gesrure_test(); 
		//	TIM_SetCEompare1(TIM4,100);

		//	CSB1();
			//CSB2();
			//CSB3();
		//	CSB4();
		//	PCout(8)=0;//顺
			//PCout(7)=1;//顺
	//		PCout(8)=0;//逆
	//		LED1( OFF );	
    //  openmvData();
		//	Steeringtest();
	//		Steeringtest();
	//		LED2(ON);
	//		Delay_ms(500);
    }
                          


/*********************************************END OF FILE**********************/
