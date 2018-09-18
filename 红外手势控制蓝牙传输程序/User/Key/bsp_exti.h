#ifndef __EXTI_H
#define	__EXTI_H


#include "stm32f10x.h"


//Òý½Å¶¨Òå
#define KEY1_INT_GPIO_PORT         GPIOA
#define KEY1_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
#define KEY1_INT_GPIO_PIN          GPIO_Pin_0
#define KEY1_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
#define KEY1_INT_EXTI_PINSOURCE    GPIO_PinSource0
#define KEY1_INT_EXTI_LINE         EXTI_Line0
#define KEY1_INT_EXTI_IRQ          EXTI0_IRQn

#define KEY1_IRQHandler            EXTI0_IRQHandler


#define KEY2_INT_GPIO_PORT         GPIOE
#define KEY2_INT_GPIO_CLK          (RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO)
#define KEY2_INT_GPIO_PIN          GPIO_Pin_2
#define KEY2_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOE
#define KEY2_INT_EXTI_PINSOURCE    GPIO_PinSource2
#define KEY2_INT_EXTI_LINE         EXTI_Line2
#define KEY2_INT_EXTI_IRQ          EXTI2_IRQn

#define KEY2_IRQHandler            EXTI2_IRQHandler

void EXTI_Key_Config(void);


#endif /* __EXTI_H */
