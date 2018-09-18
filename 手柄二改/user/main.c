/***************************************************************************************
                                      声  明
    本项目代码仅供个人学习使用，可以自由移植修改，但必须保留此声明信息。移植过程中出现
其他不可估量的BUG，天际智联不负任何责任。请勿商用！

程序版本：2.1
更新日期：2018-1-23
程序作者：愤怒的小孩，东方萧雨
版权所有：西安天际智联信息技术有限公司
****************************************************************************************/
#include "stm32f10x.h"
#include "led.h"
#include "systick.h"
#include "usart.h"
//#include "spi.h"
//#include "nRF.h"
#include "nvic.h"
#include "adc_dma.h"
#include "tim_octigr.h"
#include "deal_datapacket.h"
#include "button.h"
#include "oled.h"
#include "oleddispaly.h"
#include "paramsave.h"
#include "adc.h"
#include "math.h"
//#include "nrf24l01.h"
#include "bsp_nrf.h"
#if 1
uint8_t NRF24L01_RX_Data[3];	//接收数据
uint8_t NRF24L01_TX_Data[3];//发送数据
extern u8 ADC_CALIBRATOR_OK; 
extern vu16 ADC_ConvertedValue[4];  //遥控通道ADC值
extern vu16 ADC_Calibrator[4];      //遥控通道ADC校准值
extern vu16 ADC_Value[4];
//int SignedADC_Value[4];
int main(void)
{
	NRF24L01_TX_Data[2]=0x42;
	USART_Config();
	printf("usart is ready\r\n");
	SysTick_Init();
	OLED_Init();
	OLED_Clear() ;
	NVIC_Config();
	BUTTON_Config();
	LED_Config();
	LED_On(LED1);
	SPI_GPIO_Init();
	NRF24L01_WT_Init();
	GPIO_SetBits(NRF_GPIO_MISO_PORT, NRF_GPIO_MISO_PIN);
	GPIO_SetBits(NRF_GPIO_IRQ_PORT, NRF_GPIO_IRQ_PIN);	
//	NRF_Config();
	//Init_NRF24L01_MB();	        //初始化
	//Set_TxMode_MB();	
	TIM_OCTigrConfig();
	ADC_DmaConfig();	//这个必须最后一个配置，因为一旦配置好这个，ADC就会开始工作了，则DMA会开始每个一段时间产生中断，或者先关闭总中断，最后所有都配置完毕后在打开总中断
	//WaitFlY_Connection();
	//Display_init();
	OLED_ShowString(byte(0),line8,"ADC3: ",6);
	OLED_ShowString(byte(8),line8,"ADC4: ",6);
	if (NRF24L01_Check() == 0)
	{
			NRF24L01_WT_Init();
			GPIO_SetBits(GPIOC, GPIO_Pin_13);
	}
	else
		while(1)
		{
			GPIO_SetBits(GPIOC, GPIO_Pin_13);
			delay_ms(100);
			GPIO_ResetBits(GPIOC, GPIO_Pin_13);
			delay_ms(100);
		}
	
	OLED_ShowString(byte(0),line6,"ADC1: ",6);
	OLED_ShowString(byte(8),line6,"ADC2: ",6);		
	while(1)
	{
	//	Display_Update();//OLED更新显示
		//ReconnectionFly();//断线重连
	ADC_Value[0] = 1500 + ((1000+ADC_ConvertedValue[0]*1000/4096) - ADC_Calibrator[0]);
	//ADC_Value[0] = ADC_ValueLimit(ADC_Value[0],1470,1530,1100,1900);
	//SignedADC_Value[0]=1500 + ((1000+ADC_ConvertedValue[0]*1000/4096) - ADC_Calibrator[0]);
	ADC_Value[1] = (ADC_ConvertedValue[1]*1000/4096); //油门0~1000;
	//SignedADC_Value[1]=(ADC_ConvertedValue[1]*1000/4096); 
	ADC_Value[2] = 1500 + ((1000+(ADC_ConvertedValue[2])*1000/4096) - ADC_Calibrator[2] );
	//ADC_Value[2] = ADC_ValueLimit(ADC_Value[2],1470,1530,1100,1900);
	
	ADC_Value[3] = 1500 + ((1000+(ADC_ConvertedValue[3])*1000/4096) - ADC_Calibrator[3] );
	//SignedADC_Value[2]=1500 + ((1000+(ADC_ConvertedValue[2])*1000/4096) - ADC_Calibrator[2] );
	//SignedADC_Value[3]=1500 + ((1000+(ADC_ConvertedValue[3])*1000/4096) - ADC_Calibrator[3] );
	//ADC_Value[3] = ADC_ValueLimit(ADC_Value[3],1470,1530,1100,1900);
		
	OLED_ShowNum(byte(4),line8,ADC_Value[2] ,4,6); //右手的左右2500-3500
	OLED_ShowNum(byte(12),line8,ADC_Value[3] ,4,6); //右手的上下2500-3500
	OLED_ShowNum(byte(4),line6,ADC_Value[0] ,4,6);  //左手的左右2500-3500
	OLED_ShowNum(byte(12),line6,ADC_Value[1] ,4,6); //左手的上下0-999

	if((ADC_Value[3]>3450)&&(abs(ADC_Value[2]-3000)<350)){NRF24L01_TX_Data[0]=0x31;}
	else if((ADC_Value[3]<2550)&&(abs(ADC_Value[2]-3000)<350)){NRF24L01_TX_Data[0]=0x32;}
	else if((ADC_Value[2]<2550)&&(abs(ADC_Value[3]-3000)<350)){NRF24L01_TX_Data[0]=0x33;}
	else if((ADC_Value[2]>3450)&&(abs(ADC_Value[3]-3000)<350)){NRF24L01_TX_Data[0]=0x34;}
	else if((abs(ADC_Value[3]-3000)<350)&&(abs(ADC_Value[2]-3000)<350)){NRF24L01_TX_Data[0]=0x30;}
	if((ADC_Value[1]>950)&&(abs(ADC_Value[0]-3000)<350)){NRF24L01_TX_Data[1]=0x37;}
	else if((ADC_Value[1]<50)&&(abs(ADC_Value[0]-3000)<350)){NRF24L01_TX_Data[1]=0x38;}
	else if((ADC_Value[0]<2550)&&(abs(ADC_Value[1]-500)<350)){NRF24L01_TX_Data[1]=0x35;}
	else if((ADC_Value[0]>3450)&&(abs(ADC_Value[1]-500)<350)){NRF24L01_TX_Data[1]=0x36;}
	else if((abs(ADC_Value[0]-3000)<350)&&(abs(ADC_Value[1]-500)<350)){NRF24L01_TX_Data[1]=0x41;}
	OLED_ShowChar(32,line1,NRF24L01_TX_Data[0],6);
	OLED_ShowChar(64,line1,NRF24L01_TX_Data[1],6);
	OLED_ShowChar(96,line1,NRF24L01_TX_Data[2],6);
	SEND_BUF(NRF24L01_TX_Data);
	NRF24L01_TX_Data[2]=0x42;//b
	delay_ms(100);
	//NRF24L01_TxPacket_MB(data);
//	NRF_SendPacket(data);

//	printf("ADC_Value:%d\t%d\t%d\t%d\n",ADC_Value[0],ADC_Value[1],ADC_Value[2],ADC_Value[3]);
	}

}
#else
static u8 cnt1=0;
static vs32 ttemp1=0,ttemp2=0,ttemp3=0,ttemp4=0;
extern vu16 ADC_ConvertedValue[4];  //遥控通道ADC值
extern vu16 ADC_Calibrator[4];      //遥控通道ADC校准值
int main(void)
{
	SysTick_Init();
	OLED_Init();
	OLED_Clear() ;
	NVIC_Config();
	BUTTON_Config();
	LED_Config();
	LED_On(LED1);
	NRF_Config();
	TIM_OCTigrConfig();
	Adc_Init();
	//ADC_DmaConfig();
	OLED_ShowString(byte(0),line8,"ADC3: ",6);
	OLED_ShowString(byte(8),line8,"ADC4: ",6);
	OLED_ShowString(byte(0),line6,"ADC1: ",6);
	OLED_ShowString(byte(8),line6,"ADC2: ",6);	
	while(1)
	{
		if(cnt1==0)
		{
			ttemp1 = 0;
			ttemp2 = 0;
			ttemp3 = 0;
			ttemp4 = 0;
			cnt1 = 0;
		}
		ADC_ConvertedValue[0]=Get_Adc(0);
		ADC_ConvertedValue[1]=Get_Adc(1);
		ADC_ConvertedValue[2]=Get_Adc(2);
		ADC_ConvertedValue[3]=Get_Adc(3);
		ttemp1 += (ADC_ConvertedValue[0]*1000/4096);
		ttemp2 += (ADC_ConvertedValue[1]*500/4096);
		ttemp3 += (ADC_ConvertedValue[2]*1000/4096);
		ttemp4 += (ADC_ConvertedValue[3]*1000/4096);
		cnt1++;	
		if(cnt1 >= 3)
		{
			ADC_Calibrator[0] = ttemp1/cnt1;
			ADC_Calibrator[1] = ttemp2/cnt1;
			ADC_Calibrator[2] = ttemp3/cnt1;
			ADC_Calibrator[3] = ttemp4/cnt1;
			OLED_ShowNum(byte(4),line8,ADC_Calibrator[2] ,2,6); 
			OLED_ShowNum(byte(12),line8,ADC_Calibrator[2] ,2,6); 
		  OLED_ShowNum(byte(4),line6,ADC_Calibrator[0] ,2,6);  
			OLED_ShowNum(byte(12),line6,ADC_Calibrator[1] ,2,6); 
			//ADC_CALIBRATOR_OK = 0;
			cnt1 = 0;
			
		}			
		//LED_Toggle(LED1);
		//delay_ms(500);
	}
}
#endif
