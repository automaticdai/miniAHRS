/* ------------------------------------------------------------
main.c, miniAHRS Project

Author: Yunfei
Website: www.yfworld.com
Version: V1.0

Declaration: 
This code is modified from lisn3188 (www.chiplab7.com).
-------------------------------------------------------------*/

#include "stm32f10x.h"
#include "drivers.h" 

// PC communication state machine
#define REIMU  0x01 //上传解算的姿态数据
#define REMOV  0x02	//上传传感器的输出
#define REHMC  0x03	//上传磁力计的标定值

#define UPLOAD_RATE  	(10)   						// upload rate (hz)
#define upload_time 	(1000000UL/UPLOAD_RATE)/2  	// upload interval (us)

int16_t ax, ay, az;	
int16_t gx, gy, gz;
int16_t hx, hy, hz;
int32_t Temperature = 0, Pressure = 0, Altitude = 0;
uint32_t system_micrsecond;
int16_t hmcvalue[3];
u8 state = REIMU;  //发送特定帧 的状态机

void system_init(void) {
	/* System Clock = 72M, 8MHz external osc + PLL */      
    //SystemInit();
	delay_init(72);		// init delay function
	Initial_LED_GPIO();	// init led
	Initial_PWMLED();
	Initial_UART1(115200L);
	Initial_UART2(115200L);
	read_config();  	// read configs from Flash --> eeprom.c
	IIC_Init();	 		// 初始化I2C接口
	delay_ms(300);		// 等待器件上电
	IMU_init(); 		// 初始化IMU和传感器
}

/**************************实现函数********************************************
*函数原型:		int main(void)
*功　　能:		主程序
*******************************************************************************/
int main(void)
{
	int16_t Math_hz = 0;
	unsigned char ucPC_cmd; 	// PC 命令关键字节	 
	float ypr[3]; 			// yaw pitch roll
	
	system_init();
	system_micrsecond = micros();
	
	while(1) {
		
		IMU_getYawPitchRoll(ypr); //姿态更新
		Math_hz++; //解算次数 ++
		BMP180_Routing(); //处理BMP018 事务 开启转换和读取结果将在这个子程序中进行 

	//-------------上位机------------------------------
		//是否到了更新 上位机的时间了？
		if((micros() - system_micrsecond) > upload_time) {
			switch(state){ 
				case REIMU:
					BMP180_getTemperat(&Temperature); 	// 读取最近的温度值
					BMP180_getPress(&Pressure);	   		// 读取最近的气压测量值
					BMP180_getAlt(&Altitude);	   		// 读取相对高度
					UART2_ReportIMU((int16_t)(ypr[0]*10.0),(int16_t)(ypr[1]*10.0),
									(int16_t)(ypr[2]*10.0),Altitude/10,Temperature,Pressure/10,Math_hz*UPLOAD_RATE);
					Math_hz=0;
					state = REMOV; //更改状态。
					break;
				case REMOV:
					MPU6050_getlastMotion6(&ax, &ay, &az, &gx, &gy, &gz);
					HMC58X3_getlastValues(&hx,&hy,&hz);
					UART2_ReportMotion(ax,ay,az,gx,gy,gz,hx,hy,hz);
					state = REIMU;
					if (HMC5883_calib) {
						state = REHMC; // 需要发送当前磁力计标定值
					} 						
					break;
				default: 
					UART2_ReportHMC(HMC5883_maxx,HMC5883_maxy,HMC5883_maxz,
									HMC5883_minx,HMC5883_miny,HMC5883_minz,0);// 发送标定值
					state = REIMU;
					break;
			}// end of switch(state) 			 
			system_micrsecond = micros();	 	// 取系统时间 单位 us 
			LED_Change();						// LED1改变亮度
		}
	//--------------------------------------------------
		//处理PC发送来的命令
		if((ucPC_cmd = UART2_CommandRoute()) != 0xff) {
			switch(ucPC_cmd){ // 检查命令标识
				case Gyro_init:			// 读取陀螺仪零偏
					MPU6050_InitGyro_Offset(); 
					break; 	
				case High_init:			// 气压高度 清零	
					BMP180_ResetAlt(0); 	
					break;		
				case HMC_calib_begin:	// 开始磁力计标定
					HMC5883L_Start_Calib();	
					break; 		
				case HMC_calib:			// 保存磁力计标定		
					HMC5883L_Save_Calib();	
					break;   	
			}
		}// end if

	}// end of while

}  // end of main	

//------------------End of File----------------------------
