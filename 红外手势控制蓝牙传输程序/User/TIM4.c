#include "TIM4.h"
#include "bsp_usart.h"
PID sPID1;
static PID *sptr1 = &sPID1;
PID sPID2;
static PID *sptr2 = &sPID2;
PID sPID3;
static PID *sptr3 = &sPID3;
PID sPID4;
static PID *sptr4 = &sPID4;
extern volatile unsigned int CaptureNumberA1;
extern volatile unsigned int CaptureNumberA2;
extern volatile unsigned int CaptureNumberB1;
extern volatile unsigned int CaptureNumberB2;
void IncPIDInit(void) 
{
    sptr1->LastError=0;            //Error[-1]
    sptr1->PrevError=0;            //Error[-2]
		sptr1->Error=0; 
    sptr1->Proportion=P_DATA;      //比例常数 Proportional Const
    sptr1->Integral=I_DATA;        //积分常数  Integral Const
    sptr1->Derivative=D_DATA;      //微分常数 Derivative Const
    sptr1->SetPoint=setpoint;           //设定目标Desired Value
		sptr1->lastoutput=0;
		sptr1->output=0;
		sptr1->increment=0;
		
    sptr2->LastError=0;            //Error[-1]
    sptr2->PrevError=0;            //Error[-2]
		sptr2->Error=0; 
    sptr2->Proportion=P_DATA;      //比例常数 Proportional Const
    sptr2->Integral=I_DATA;        //积分常数  Integral Const
    sptr2->Derivative=D_DATA;      //微分常数 Derivative Const
    sptr2->SetPoint=setpoint;           //设定目标Desired Value
		sptr2->lastoutput=0;
		sptr2->output=0;
		sptr2->increment=0;

    sptr3->LastError=0;            //Error[-1]
    sptr3->PrevError=0;            //Error[-2]
		sptr3->Error=0; 
    sptr3->Proportion=P_DATA;      //比例常数 Proportional Const
    sptr3->Integral=I_DATA;        //积分常数  Integral Const
    sptr3->Derivative=D_DATA;      //微分常数 Derivative Const
    sptr3->SetPoint=setpoint;           //设定目标Desired Value
		sptr3->lastoutput=0;
		sptr3->output=0;
		sptr3->increment=0;

    sptr4->LastError=0;            //Error[-1]
    sptr4->PrevError=0;            //Error[-2]
		sptr4->Error=0; 
    sptr4->Proportion=P_DATA;      //比例常数 Proportional Const
    sptr4->Integral=I_DATA;        //积分常数  Integral Const
    sptr4->Derivative=D_DATA;      //微分常数 Derivative Const
    sptr4->SetPoint=setpoint;           //设定目标Desired Value
		sptr4->lastoutput=0;
		sptr4->output=0;
		sptr4->increment=0;
}
void pid_calculation(PID *spid,int now,int ch)
{
	//printf("\r\nsss=%d",now);
	spid->Error=spid->SetPoint-now;
	//printf("\r\nerror=%d",spid->Error);
	spid->increment=spid->Proportion*spid->Error-spid->Integral*spid->LastError+spid->Derivative*spid->PrevError;
	
	//printf("\r\nincrement=%d",spid->increment);
	spid->PrevError=spid->LastError;
	//printf("\r\nPrevError=%d",spid->PrevError);
	spid->LastError=spid->Error;
	//printf("\r\nLastError=%d",spid->LastError);
	spid->output=spid->lastoutput+spid->increment;
	
	spid->lastoutput=spid->output;
	if(spid->output>900)spid->output=900;
	if(spid->output<0)spid->output=0;
	//printf("\r\nss=%d",spid->output);
	//printf("11");
	TIM4_SetPWM_Num(spid->output,ch);
	//if(ch==1){printf("\r\n1111");printf("\r\n1s=%d",spid->output);}
	//if(ch==2){printf("\r\n2222");printf("\r\n2s=%d",spid->output);}
	//if(ch==3){printf("\r\n3333");printf("\r\n3s=%d",spid->output);}
	//if(ch==4){printf("\r\n4444");printf("\r\n4s=%d",spid->output);}
		
	
}
int IncPIDCalc1(int NextPoint) 
{
  int iError,iIncpid;                                 //当前误差
  iError=sptr1->SetPoint-NextPoint;                    //增量计算
  iIncpid=(sptr1->Proportion * iError)                 //E[k]项
              -(sptr1->Integral * sptr1->LastError)     //E[k-1]项
              +(sptr1->Derivative * sptr1->PrevError);  //E[k-2]项
              
  sptr1->PrevError=sptr1->LastError;                    //存储误差，用于下次计算
  sptr1->LastError=iError;
  return(iIncpid);                                    //返回增量值
	
}
void TIM4_NVIC_Config(void)
{

	NVIC_InitTypeDef NVIC_InitStructure;


 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器


							 
}

void TIM4_Mode_Config(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
//	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5    
 
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIO
#if 0
	PDout(4)=1;
	PDout(5)=0;
	PDout(6)=1;
	PDout(7)=0;
#endif


TIM_TimeBaseStructure.TIM_Period = 999; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =3; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位 

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

  TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC1

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
//	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
    //使能TIM1预装载寄存器
  TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC1

	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
//	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
    //使能TIM1预装载寄存器
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC1

	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
//	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
    //使能TIM1预装载寄存器		
  TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC1

	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
//	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
    //使能TIM1预装载寄存器


  TIM_ARRPreloadConfig(TIM4, ENABLE);  
    //设置中断源，只有溢出时才中断
  TIM_UpdateRequestConfig(TIM4,TIM_UpdateSource_Regular);
	// 清除中断标志位
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    // 使能中断
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	
	TIM_Cmd(TIM4, ENABLE);  //使能TIM3
	//100//211 1s
	//500//1190
	TIM_SetCompare1(TIM4,500);
	TIM_SetCompare2(TIM4,500);
	TIM_SetCompare3(TIM4,500);
	TIM_SetCompare4(TIM4,500);
	TIM_SetCompare1(TIM4,0);
	TIM_SetCompare2(TIM4,0);
	TIM_SetCompare3(TIM4,0);
	TIM_SetCompare4(TIM4,0);
}
void TIM4_Init(void)
{
	TIM4_NVIC_Config();
	TIM4_Mode_Config();
}
void TIM4_SetPWM_Num(u16 value,u8 ch)
{
	if (ch==1)
	{
		TIM_SetCompare1(TIM4,value);
	}
	if (ch==2)
	{
		TIM_SetCompare2(TIM4,value);
	}
	if (ch==3)
	{
		TIM_SetCompare3(TIM4,value);
	}
	if (ch==4)
	{
		TIM_SetCompare4(TIM4,value);
	}
}
void Stop_it(void)
{	
TIM4_SetPWM_Num(0,1);
	TIM4_SetPWM_Num(0,2);
	TIM4_SetPWM_Num(0,3);
	TIM4_SetPWM_Num(0,4);
}
void forward(void)
{	
			PDout(4)=1;
			PDout(5)=0;
			PDout(6)=1;
			PDout(7)=0;
			pid_calculation(&sPID1,CaptureNumberA1,1);

			CaptureNumberA1=0;    // 清零，从零开始计数
			pid_calculation(&sPID2,CaptureNumberA2,2);
			CaptureNumberA2=0;    // 清零，从零开始计数
			pid_calculation(&sPID3,CaptureNumberB1,3);
			CaptureNumberB1=0;    // 清零，从零开始计数
			pid_calculation(&sPID4,CaptureNumberB2,4);			
			CaptureNumberB2=0;    // 清零，从零开始计数
}
void backward(void)
{	
			PDout(4)=0;
			PDout(5)=1;
			PDout(6)=0;
			PDout(7)=1;
			pid_calculation(&sPID1,CaptureNumberA1,1);

			CaptureNumberA1=0;    // 清零，从零开始计数
			pid_calculation(&sPID2,CaptureNumberA2,2);
			CaptureNumberA2=0;    // 清零，从零开始计数
			pid_calculation(&sPID3,CaptureNumberB1,3);
			CaptureNumberB1=0;    // 清零，从零开始计数
			pid_calculation(&sPID4,CaptureNumberB2,4);			
			CaptureNumberB2=0;    // 清零，从零开始计数
}
void leftward(void)
{	
			PDout(4)=0;
			PDout(5)=0;
			PDout(6)=1;
			PDout(7)=1;
			pid_calculation(&sPID1,CaptureNumberA1,1);

			CaptureNumberA1=0;    // 清零，从零开始计数
			pid_calculation(&sPID2,CaptureNumberA2,2);
			CaptureNumberA2=0;    // 清零，从零开始计数
			pid_calculation(&sPID3,CaptureNumberB1,3);
			CaptureNumberB1=0;    // 清零，从零开始计数
			pid_calculation(&sPID4,CaptureNumberB2,4);			
			CaptureNumberB2=0;    // 清零，从零开始计数
}
void rightward(void)
{	
			PDout(4)=1;
			PDout(5)=1;
			PDout(6)=0;
			PDout(7)=0;
			pid_calculation(&sPID1,CaptureNumberA1,1);

			CaptureNumberA1=0;    // 清零，从零开始计数
			pid_calculation(&sPID2,CaptureNumberA2,2);
			CaptureNumberA2=0;    // 清零，从零开始计数
			pid_calculation(&sPID3,CaptureNumberB1,3);
			CaptureNumberB1=0;    // 清零，从零开始计数
			pid_calculation(&sPID4,CaptureNumberB2,4);			
			CaptureNumberB2=0;    // 清零，从零开始计数
}
