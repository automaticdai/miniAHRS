/* IOI2C.c file
±àÐ´Õß£ºlisn3188
ÍøÖ·£ºwww.chiplab7.com
×÷ÕßE-mail£ºlisn3188@163.com
±àÒë»·¾³£ºMDK-Lite  Version: 4.23
³õ°æÊ±¼ä: 2012-04-25
²âÊÔ£º ±¾³ÌÐòÒÑÔÚµÚÆßÊµÑéÊÒµÄminiIMUÉÏÍê³É²âÊÔ
¹¦ÄÜ£º
Ê¹ÓÃIOÄ£Äâ·½Ê½Ìá¹©I2C½Ó¿Ú²Ù×÷API. 
SCL -> PB6
SDA -> PB7 
------------------------------------
 */
#include "IOI2C.h"
#include "delay.h"
  
/**************************ÊµÏÖº¯Êý********************************************
*º¯ÊýÔ­ÐÍ:		void IIC_Init(void)
*¹¦¡¡¡¡ÄÜ:		³õÊ¼»¯I2C¶ÔÓ¦µÄ½Ó¿ÚÒý½Å¡£
*******************************************************************************/
void IIC_Init(void)
{			
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// Ê¹ÄÜGPIOBÊ±ÖÓ
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	
 	// ÅäÖÃPB6 PB7 Îª¿ªÂ©Êä³ö  Ë¢ÐÂÆµÂÊÎª50MHz
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
  	// Ó¦ÓÃÅäÖÃµ½GPIOB 
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


/**************************ÊµÏÖº¯Êý********************************************
*º¯ÊýÔ­ÐÍ:		void IIC_Start(void)
*¹¦¡¡¡¡ÄÜ:		²úÉúIICÆðÊ¼ÐÅºÅ
*******************************************************************************/
void IIC_Start(void)
{
	SDA_OUT();    	// sdaÏßÊä³ö
	IIC_SDA = 1;	  	  
	IIC_SCL = 1;
	delay_us(4);
 	IIC_SDA = 0;	// START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL = 0;	// Ç¯×¡I2C×ÜÏß£¬×¼±¸·¢ËÍ»ò½ÓÊÕÊý¾Ý 
}


/**************************ÊµÏÖº¯Êý********************************************
*º¯ÊýÔ­ÐÍ:		void IIC_Stop(void)
*¹¦¡¡¡¡ÄÜ:	    // ²úÉúIICÍ£Ö¹ÐÅºÅ
*******************************************************************************/	  
void IIC_Stop(void)
{
	SDA_OUT();		// sdaÏßÊä³ö
	IIC_SCL = 0;
	IIC_SDA = 0;	// STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL = 1; 
	IIC_SDA = 1;	// ·¢ËÍI2C×ÜÏß½áÊøÐÅºÅ
	delay_us(4);							   	
}


/**************************ÊµÏÖº¯Êý********************************************
*º¯ÊýÔ­ÐÍ:		u8 IIC_Wait_Ack(void)
*¹¦¡¡¡¡ÄÜ:	    µÈ´ýÓ¦´ðÐÅºÅµ½À´ 
//·µ»ØÖµ£º1£¬½ÓÊÕÓ¦´ðÊ§°Ü
//        0£¬½ÓÊÕÓ¦´ð³É¹¦
*******************************************************************************/
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	
	SDA_IN();      // SDAÉèÖÃÎªÊäÈë  
	IIC_SDA = 1; delay_us(1);	   
	IIC_SCL = 1; delay_us(1);	 
	while (READ_SDA)
	{
		ucErrTime++;
		if (ucErrTime > 50)
		{
			IIC_Stop();
			return 1;
		}
	  delay_us(1);
	}
	IIC_SCL = 0;	// Ê±ÖÓÊä³ö0 	   
	return 0;  
} 


/**************************ÊµÏÖº¯Êý********************************************
*º¯ÊýÔ­ÐÍ:		void IIC_Ack(void)
*¹¦¡¡¡¡ÄÜ:	    ²úÉúACKÓ¦´ð
*******************************************************************************/
void IIC_Ack(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 0;
	delay_us(2);
	IIC_SCL = 1;
	delay_us(2);
	IIC_SCL = 0;
}
	

/**************************ÊµÏÖº¯Êý********************************************
*º¯ÊýÔ­ÐÍ:		void IIC_NAck(void)
*¹¦¡¡¡¡ÄÜ:	    ²úÉúNACKÓ¦´ð
*******************************************************************************/	    
void IIC_NAck(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 1;
	delay_us(2);
	IIC_SCL = 1;
	delay_us(2);
	IIC_SCL = 0;
}					 				     


/**************************ÊµÏÖº¯Êý********************************************
*º¯ÊýÔ­ÐÍ:		void IIC_Send_Byte(u8 txd)
*¹¦¡¡¡¡ÄÜ:	    IIC·¢ËÍÒ»¸ö×Ö½Ú
*******************************************************************************/		  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT();		// SDAÉèÖÃÎªÊä³öÄ£Ê½   
    IIC_SCL = 0; 	// À­µÍÊ±ÖÓ¿ªÊ¼Êý¾Ý´«Êä
    for (t = 0; t < 8; t++)
    {              
        IIC_SDA = (txd&0x80) >> 7;
        txd <<= 1; 	  
		delay_us(2);   
		IIC_SCL = 1;
		delay_us(2); 
		IIC_SCL = 0;	
		delay_us(2);
    }	 
} 	 


/**************************ÊµÏÖº¯Êý********************************************
*º¯ÊýÔ­ÐÍ:		u8 IIC_Read_Byte(unsigned char ack)
*¹¦¡¡¡¡ÄÜ:	    //¶Á1¸ö×Ö½Ú£¬ack=1Ê±£¬·¢ËÍACK£¬ack=0£¬·¢ËÍnACK 
*******************************************************************************/  
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
	SDA_IN();¡		// SDAÉèÖÃÎªÊäÈë
    for (i = 0; i < 8; i++)
	{
        IIC_SCL = 0; 
        delay_us(2);
		IIC_SCL = 1;
        receive <<= 1;
        if (READ_SDA) {receive++;}   
		delay_us(2); 
    }					 
    if (ack)
        IIC_Ack(); // ·¢ËÍACK 
    else
        IIC_NAck();// ·¢ËÍnACK  
    return receive;
}


/**************************ÊµÏÖº¯Êý********************************************
*º¯ÊýÔ­ÐÍ:		unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
*¹¦¡¡¡¡ÄÜ:	    ¶ÁÈ¡Ö¸¶¨Éè±¸ Ö¸¶¨¼Ä´æÆ÷µÄÒ»¸öÖµ
ÊäÈë	I2C_Addr  Ä¿±êÉè±¸µØÖ·
		addr	   ¼Ä´æÆ÷µØÖ·
·µ»Ø   ¶Á³öÀ´µÄÖµ
*******************************************************************************/ 
unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
{
	unsigned char res=0;
	
	IIC_Start();	
	IIC_Send_Byte(I2C_Addr);	   //·¢ËÍÐ´ÃüÁî
	res++;
	IIC_Wait_Ack();
	IIC_Send_Byte(addr); res++;  //·¢ËÍµØÖ·
	IIC_Wait_Ack();	  
	//IIC_Stop();//²úÉúÒ»¸öÍ£Ö¹Ìõ¼þ	
	IIC_Start();
	IIC_Send_Byte(I2C_Addr+1); res++;          //½øÈë½ÓÊÕÄ£Ê½			   
	IIC_Wait_Ack();
	res=IIC_Read_Byte(0);	   
    IIC_Stop();//²úÉúÒ»¸öÍ£Ö¹Ìõ¼þ

	return res;
}


/**************************ÊµÏÖº¯Êý********************************************
*º¯ÊýÔ­ÐÍ:		u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data)
*¹¦¡¡¡¡ÄÜ:	    ¶ÁÈ¡Ö¸¶¨Éè±¸ Ö¸¶¨¼Ä´æÆ÷µÄ length¸öÖµ
ÊäÈë	dev  Ä¿±êÉè±¸µØÖ·
		reg	  ¼Ä´æÆ÷µØÖ·
		length Òª¶ÁµÄ×Ö½ÚÊý
		*data  ¶Á³öµÄÊý¾Ý½«Òª´æ·ÅµÄÖ¸Õë
·µ»Ø   ¶Á³öÀ´µÄ×Ö½ÚÊýÁ¿
*******************************************************************************/ 
u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data){
    u8 count = 0;
	
	IIC_Start();
	IIC_Send_Byte(dev);	   //·¢ËÍÐ´ÃüÁî
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);   //·¢ËÍµØÖ·
    IIC_Wait_Ack();	  
	IIC_Start();
	IIC_Send_Byte(dev+1);  //½øÈë½ÓÊÕÄ£Ê½	
	IIC_Wait_Ack();
	
    for(count=0;count<length;count++){
		 
		 if(count!=length-1)data[count]=IIC_Read_Byte(1);  //´øACKµÄ¶ÁÊý¾Ý
		 	else  data[count]=IIC_Read_Byte(0);	 //×îºóÒ»¸ö×Ö½ÚNACK
	}
    IIC_Stop();//²úÉúÒ»¸öÍ£Ö¹Ìõ¼þ
    return count;
}


/**************************ÊµÏÖº¯Êý********************************************
*º¯ÊýÔ­ÐÍ:		u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data)
*¹¦¡¡¡¡ÄÜ:	    ½«¶à¸ö×Ö½ÚÐ´ÈëÖ¸¶¨Éè±¸ Ö¸¶¨¼Ä´æÆ÷
ÊäÈë	dev  Ä¿±êÉè±¸µØÖ·
		reg	  ¼Ä´æÆ÷µØÖ·
		length ÒªÐ´µÄ×Ö½ÚÊý
		*data  ½«ÒªÐ´µÄÊý¾ÝµÄÊ×µØÖ·
·µ»Ø   ·µ»ØÊÇ·ñ³É¹¦
*******************************************************************************/ 
u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data){
  
 	u8 count = 0;
	IIC_Start();
	IIC_Send_Byte(dev);	   //·¢ËÍÐ´ÃüÁî
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);   //·¢ËÍµØÖ·
    IIC_Wait_Ack();	  
	for(count=0;count<length;count++){
		IIC_Send_Byte(data[count]); 
		IIC_Wait_Ack(); 
	 }
	IIC_Stop();//²úÉúÒ»¸öÍ£Ö¹Ìõ¼þ

    return 1; //status == 0;
}


/**************************ÊµÏÖº¯Êý********************************************
*º¯ÊýÔ­ÐÍ:		u8 IICreadByte(u8 dev, u8 reg, u8 *data)
*¹¦¡¡¡¡ÄÜ:	    ¶ÁÈ¡Ö¸¶¨Éè±¸ Ö¸¶¨¼Ä´æÆ÷µÄÒ»¸öÖµ
ÊäÈë	dev  Ä¿±êÉè±¸µØÖ·
		reg	   ¼Ä´æÆ÷µØÖ·
		*data  ¶Á³öµÄÊý¾Ý½«Òª´æ·ÅµÄµØÖ·
·µ»Ø   1
*******************************************************************************/ 
u8 IICreadByte(u8 dev, u8 reg, u8 *data){
	*data=I2C_ReadOneByte(dev, reg);
    return 1;
}


/**************************ÊµÏÖº¯Êý********************************************
*º¯ÊýÔ­ÐÍ:		unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data)
*¹¦¡¡¡¡ÄÜ:	    Ð´ÈëÖ¸¶¨Éè±¸ Ö¸¶¨¼Ä´æÆ÷Ò»¸ö×Ö½Ú
ÊäÈë	dev  Ä¿±êÉè±¸µØÖ·
		reg	   ¼Ä´æÆ÷µØÖ·
		data  ½«ÒªÐ´ÈëµÄ×Ö½Ú
·µ»Ø   1
*******************************************************************************/ 
unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data){
    return IICwriteBytes(dev, reg, 1, &data);
}


/**************************ÊµÏÖº¯Êý********************************************
*º¯ÊýÔ­ÐÍ:		u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
*¹¦¡¡¡¡ÄÜ:	    ¶Á ÐÞ¸Ä Ð´ Ö¸¶¨Éè±¸ Ö¸¶¨¼Ä´æÆ÷Ò»¸ö×Ö½Ú ÖÐµÄ¶à¸öÎ»
ÊäÈë	dev  Ä¿±êÉè±¸µØÖ·
		reg	   ¼Ä´æÆ÷µØÖ·
		bitStart  Ä¿±ê×Ö½ÚµÄÆðÊ¼Î»
		length   Î»³¤¶È
		data    ´æ·Å¸Ä±äÄ¿±ê×Ö½ÚÎ»µÄÖµ
·µ»Ø   ³É¹¦ Îª1 
 		Ê§°ÜÎª0
*******************************************************************************/ 
u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
{

    u8 b;
    if (IICreadByte(dev, reg, &b) != 0) {
        u8 mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
        data <<= (8 - length);
        data >>= (7 - bitStart);
        b &= mask;
        b |= data;
        return IICwriteByte(dev, reg, b);
    } else {
        return 0;
    }
}


/**************************ÊµÏÖº¯Êý********************************************
*º¯ÊýÔ­ÐÍ:		u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data)
*¹¦¡¡¡¡ÄÜ:	    ¶Á ÐÞ¸Ä Ð´ Ö¸¶¨Éè±¸ Ö¸¶¨¼Ä´æÆ÷Ò»¸ö×Ö½Ú ÖÐµÄ1¸öÎ»
ÊäÈë	dev  Ä¿±êÉè±¸µØÖ·
		reg	   ¼Ä´æÆ÷µØÖ·
		bitNum  ÒªÐÞ¸ÄÄ¿±ê×Ö½ÚµÄbitNumÎ»
		data  Îª0 Ê±£¬Ä¿±êÎ»½«±»Çå0 ·ñÔò½«±»ÖÃÎ»
·µ»Ø   ³É¹¦ Îª1 
 		Ê§°ÜÎª0
*******************************************************************************/ 
u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data){
    u8 b;
    IICreadByte(dev, reg, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return IICwriteByte(dev, reg, b);
}

//------------------End of File----------------------------
