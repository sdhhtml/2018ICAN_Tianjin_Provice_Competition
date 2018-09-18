#ifndef __BEEP_H
#define	__BEEP_H


#include "stm32f10x.h"


/* 定义LED连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
// R-红色
#define BEEP_GPIO_PORT    	GPIOB			              /* GPIO端口 */
#define BEEP_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define BEEP_GPIO_PIN		GPIO_Pin_8			        /* 连接到SCL时钟线的GPIO */


// B-蓝色
//#define LED3_GPIO_PORT    	GPIOB			              /* GPIO端口 */
//#define LED3_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
//#define LED3_GPIO_PIN		GPIO_Pin_1			        /* 连接到SCL时钟线的GPIO */


/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */



//#define LED3(a)	if (a)	\
					GPIO_SetBits(LED3_GPIO_PORT,LED3_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED3_GPIO_PORT,LED3_GPIO_PIN)


/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //输出为高电平		
#define digitalLo(p,i)		 {p->BRR=i;}	 //输出低电平
#define digitalToggle(p,i) {p->ODR ^=i;} //输出反转状态


/* 定义控制IO的宏 */

#define BEEP_OFF		   digitalHi(BEEP_GPIO_PORT,BEEP_GPIO_PIN)
#define BEEP_ON			   digitalLo(BEEP_GPIO_PORT,BEEP_GPIO_PIN)


//#define LED3_TOGGLE		 digitalToggle(LED3_GPIO_PORT,LED3_GPIO_PIN)
//#define LED3_OFF		   digitalHi(LED3_GPIO_PORT,LED3_GPIO_PIN)
//#define LED3_ON			   digitalLo(LED3_GPIO_PORT,LED3_GPIO_PIN)

/* 基本混色，后面高级用法使用PWM可混出全彩颜色,且效果更好 */


void BEEP_GPIO_Config(void);

#endif /* __LED_H */
