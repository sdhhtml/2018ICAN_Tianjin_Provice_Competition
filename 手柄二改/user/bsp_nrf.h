
#ifndef __BSP_NRF_H
#define __BSP_NRF_H

#include "stm32f10x.h"

#define uchar unsigned char
#define uint unsigned int


#define NRF_SPI           SPI1
#define NRF_SPI_CLK       RCC_APB2Periph_SPI1
#define NRF_SPI_CLK_EN_FUN RCC_APB2PeriphClockCmd

#define NRF_GPIO_CE_PORT  GPIOB
#define NRF_GPIO_CE_PIN   GPIO_Pin_0

#define NRF_GPIO_CSN_PORT GPIOA
#define NRF_GPIO_CSN_PIN  GPIO_Pin_4

#define NRF_GPIO_MOSI_PORT GPIOA
#define NRF_GPIO_MOSI_PIN  GPIO_Pin_7

#define NRF_GPIO_MISO_PORT GPIOA
#define NRF_GPIO_MISO_PIN GPIO_Pin_6

#define NRF_GPIO_SCK_PORT GPIOA
#define NRF_GPIO_SCK_PIN GPIO_Pin_5

#define NRF_GPIO_IRQ_PORT GPIOB
#define NRF_GPIO_IRQ_PIN GPIO_Pin_1

#define NRF_GPIO_CLK   RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB

//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入


/**********  NRF24L01寄存器操作命令  ***********/
#define nRF_READ_REG        0x00  //读配置寄存器,低5位为寄存器地址
#define nRF_WRITE_REG       0x20  //写配置寄存器,低5位为寄存器地址
#define RD_RX_PLOAD     0x61  //读RX有效数据,1~32字节
#define WR_TX_PLOAD     0xA0  //写TX有效数据,1~32字节
#define FLUSH_TX        0xE1  //清除TX FIFO寄存器.发射模式下用
#define FLUSH_RX        0xE2  //清除RX FIFO寄存器.接收模式下用
#define REUSE_TX_PL     0xE3  //重新使用上一包数据,CE为高,数据包被不断发送.
#define NOP             0xFF  //空操作,可以用来读状态寄存器	 
/**********  NRF24L01寄存器地址   *************/
#define CONFIG          0x00  //配置寄存器地址                             
#define EN_AA           0x01  //使能自动应答功能 
#define EN_RXADDR       0x02  //接收地址允许
#define SETUP_AW        0x03  //设置地址宽度(所有数据通道)
#define SETUP_RETR      0x04  //建立自动重发
#define RF_CH           0x05  //RF通道
#define RF_SETUP        0x06  //RF寄存器
#define STATUS          0x07  //状态寄存器
#define OBSERVE_TX      0x08  // 发送检测寄存器
#define CD              0x09  // 载波检测寄存器
#define RX_ADDR_P0      0x0A  // 数据通道0接收地址
#define RX_ADDR_P1      0x0B  // 数据通道1接收地址
#define RX_ADDR_P2      0x0C  // 数据通道2接收地址
#define RX_ADDR_P3      0x0D  // 数据通道3接收地址
#define RX_ADDR_P4      0x0E  // 数据通道4接收地址
#define RX_ADDR_P5      0x0F  // 数据通道5接收地址
#define TX_ADDR         0x10  // 发送地址寄存器
#define RX_PW_P0        0x11  // 接收数据通道0有效数据宽度(1~32字节) 
#define RX_PW_P1        0x12  // 接收数据通道1有效数据宽度(1~32字节) 
#define RX_PW_P2        0x13  // 接收数据通道2有效数据宽度(1~32字节) 
#define RX_PW_P3        0x14  // 接收数据通道3有效数据宽度(1~32字节) 
#define RX_PW_P4        0x15  // 接收数据通道4有效数据宽度(1~32字节)
#define RX_PW_P5        0x16  // 接收数据通道5有效数据宽度(1~32字节)
#define FIFO_STATUS     0x17  // FIFO状态寄存器
/*————————————————————————————————————————————————————————————————————*/

/******   STATUS寄存器bit位定义      *******/
#define MAX_TX  	0x10  	  //达到最大发送次数中断
#define TX_OK   	0x20  	  //TX发送完成中断
#define RX_OK   	0x40  	  //接收到数据中断
/*——————————————————————————————————————————————————*/
/*********     24L01发送接收数据宽度定义	  ***********/
#define TX_ADR_WIDTH    5     //5字节地址宽度
#define RX_ADR_WIDTH    5     //5字节地址宽度
#define TX_PLOAD_WIDTH  3    //32字节有效数据宽度
#define RX_PLOAD_WIDTH  3    //32字节有效数据宽度

#define NRF_CSN   PAout(4)  //片选信号，推挽输出  
#define NRF_IRQ   PBin(1)  //IRQ主机数据输入,上拉输入
#define NRF_SCK   PAout(5)  //发送接收模式选择推挽输出
#define NRF_CE   	PBout(0)  //PB0 推完输出
#define NRF_MOSI  PAout(7)  //PC6 MOSI 主机推挽输出 （根据速率初始化PC_CR2寄存器）
#define NRF_MISO  PAin(6)  //PC7 MISO


void delay_150us(void);
uchar SPI_RW(uchar byte);
uchar NRF24L01_Write_Reg(uchar reg,uchar value);
uchar NRF24L01_Read_Reg(uchar reg);
uchar NRF24L01_Read_Buf(uchar reg,uchar *pBuf,uchar len);
uchar NRF24L01_Write_Buf(uchar reg, uchar *pBuf, uchar len);
uchar NRF24L01_RxPacket(uchar *rxbuf);
uchar NRF24L01_TxPacket(uchar *txbuf);
uchar NRF24L01_Check(void);
void NRF24L01_WT_Init(void);
uint8_t SEND_BUF(uchar *buf);
void SPI_GPIO_Init(void);



#endif /* __BSP_NRF_H */

