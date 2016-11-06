#ifndef __IOI2C_H
#define __IOI2C_H
#include "stm32f10x.h"

// IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))  

// IO�ڵ�ַӳ��
#define GPIOA_ODR_ADDR    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_ADDR    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_ADDR    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_ADDR    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_ADDR    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_ADDR    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_ADDR    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_ADDR    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_ADDR    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_ADDR    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_ADDR    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_ADDR    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_ADDR    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_ADDR    (GPIOG_BASE+8) //0x40011E08 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_ADDR,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_ADDR,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_ADDR,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_ADDR,n)  //���� 
   	   		   
// IO��������
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0x80000000;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0x30000000;}


// IO��������	 
#define IIC_SCL    PBout(6) // SCL
#define IIC_SDA    PBout(7) // SDA	 
#define READ_SDA   PBin(7)  // ����SDA 

// I2C Public Function
void IIC_Init(void);                // ��ʼ��IIC��IO��				 
void IIC_Start(void);				// ����IIC��ʼ�ź�
void IIC_Stop(void);	  			// ����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			// IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);// IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				// IIC�ȴ�ACK�ź�
void IIC_Ack(void);					// IIC����ACK�ź�
void IIC_NAck(void);				// IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	 
unsigned char I2C_Readkey(unsigned char I2C_Addr);

unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr);
unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data);
u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data);
u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data);
u8 IICwriteBit(u8 dev,u8 reg,u8 bitNum,u8 data);
u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data);

#endif

//------------------End of File----------------------------
