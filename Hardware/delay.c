/* delay.c file
��д�ߣ�lisn3188
��ַ��www.chiplab7.com
����E-mail��lisn3188@163.com
���뻷����MDK-Lite Version: 4.23
����ʱ��: 2012-04-25
���ܣ��ṩ��ȷ����ʱAPI  ��΢�뼶 �ͺ��뼶��ʱ
------------------------------------
 */
#include "delay.h"
	 
static u8  fac_us = 0; // us��ʱ������
static u16 fac_ms = 0; // ms��ʱ������


/**************************ʵ�ֺ���********************************************
*����ԭ��:		void delay_init(u8 SYSCLK)
*��������:		��ʼ���ӳ�ϵͳ��ʹ��ʱ����������״̬
*ע������:      ��ʼ���ӳٺ���
				SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
				SYSCLK:ϵͳʱ�� (MHz)
*******************************************************************************/
void delay_init(u8 SYSCLK)
{
	SysTick->CTRL &= 0xfffffffb; 	// bit2���,ѡ���ⲿʱ��  HCLK/8
	fac_us = SYSCLK/8;		    
	fac_ms = (u16)fac_us*1000;
}				
			    

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void delay_ms(u16 nms)
*��������:		���뼶��ʱ  ��ʱnms  nms<=1864
*ע������:      SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
				nms <= 0xffffff*8*1000/SYSCLK
				SYSCLK��λΪHz,nms��λΪms
*******************************************************************************/
void delay_ms(u16 nms)
{	 		  	  
	u32 status;		   
	SysTick->LOAD = (u32)nms*fac_ms;// ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL = 0x00;           	// ��ռ�����
	SysTick->CTRL = 0x01 ;          // ��ʼ����  
	do {
		status = SysTick->CTRL;
	} while ( (status & 0x01) && !(status & (1<<16)) );// �ȴ�ʱ�䵽��   
	SysTick->CTRL = 0x00;       	// �رռ�����
	SysTick->VAL = 0X00;       		// ��ռ�����	  	    
}   


/**************************ʵ�ֺ���********************************************
*����ԭ��:		void delay_us(u32 nus)
*��������:		΢�뼶��ʱ  ��ʱnus  nus<=1864 135
*******************************************************************************/		    								   
void delay_us(u32 nus)
{		
	u32 status;	    	 
	SysTick->LOAD = nus*fac_us; 	// ʱ�����	  		 
	SysTick->VAL = 0x00;        	// ��ռ�����
	SysTick->CTRL = 0x01 ;      	// ��ʼ���� 	 
	do {
		status = SysTick->CTRL;
	} while( (status & 0x01) && !(status & (1<<16)));// �ȴ�ʱ�䵽��   
	SysTick->CTRL=0x00;       		// �رռ�����
	SysTick->VAL =0X00;       		// ��ռ�����	 
}
	
//------------------End of File----------------------------
