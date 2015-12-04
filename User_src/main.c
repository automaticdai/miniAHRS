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
#define REIMU  0x01 //�ϴ��������̬����
#define REMOV  0x02	//�ϴ������������
#define REHMC  0x03	//�ϴ������Ƶı궨ֵ

#define UPLOAD_RATE  	(10)   						// upload rate (hz)
#define upload_time 	(1000000UL/UPLOAD_RATE)/2  	// upload interval (us)

int16_t ax, ay, az;	
int16_t gx, gy, gz;
int16_t hx, hy, hz;
int32_t Temperature = 0, Pressure = 0, Altitude = 0;
uint32_t system_micrsecond;
int16_t hmcvalue[3];
u8 state = REIMU;  //�����ض�֡ ��״̬��

void system_init(void) {
	/* System Clock = 72M, 8MHz external osc + PLL */      
    //SystemInit();
	delay_init(72);		// init delay function
	Initial_LED_GPIO();	// init led
	Initial_PWMLED();
	Initial_UART1(115200L);
	Initial_UART2(115200L);
	read_config();  	// read configs from Flash --> eeprom.c
	IIC_Init();	 		// ��ʼ��I2C�ӿ�
	delay_ms(300);		// �ȴ������ϵ�
	IMU_init(); 		// ��ʼ��IMU�ʹ�����
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		int main(void)
*��������:		������
*******************************************************************************/
int main(void)
{
	int16_t Math_hz = 0;
	unsigned char ucPC_cmd; 	// PC ����ؼ��ֽ�	 
	float ypr[3]; 			// yaw pitch roll
	
	system_init();
	system_micrsecond = micros();
	
	while(1) {
		
		IMU_getYawPitchRoll(ypr); //��̬����
		Math_hz++; //������� ++
		BMP180_Routing(); //����BMP018 ���� ����ת���Ͷ�ȡ�����������ӳ����н��� 

	//-------------��λ��------------------------------
		//�Ƿ��˸��� ��λ����ʱ���ˣ�
		if((micros() - system_micrsecond) > upload_time) {
			switch(state){ 
				case REIMU:
					BMP180_getTemperat(&Temperature); 	// ��ȡ������¶�ֵ
					BMP180_getPress(&Pressure);	   		// ��ȡ�������ѹ����ֵ
					BMP180_getAlt(&Altitude);	   		// ��ȡ��Ը߶�
					UART2_ReportIMU((int16_t)(ypr[0]*10.0),(int16_t)(ypr[1]*10.0),
									(int16_t)(ypr[2]*10.0),Altitude/10,Temperature,Pressure/10,Math_hz*UPLOAD_RATE);
					Math_hz=0;
					state = REMOV; //����״̬��
					break;
				case REMOV:
					MPU6050_getlastMotion6(&ax, &ay, &az, &gx, &gy, &gz);
					HMC58X3_getlastValues(&hx,&hy,&hz);
					UART2_ReportMotion(ax,ay,az,gx,gy,gz,hx,hy,hz);
					state = REIMU;
					if (HMC5883_calib) {
						state = REHMC; // ��Ҫ���͵�ǰ�����Ʊ궨ֵ
					} 						
					break;
				default: 
					UART2_ReportHMC(HMC5883_maxx,HMC5883_maxy,HMC5883_maxz,
									HMC5883_minx,HMC5883_miny,HMC5883_minz,0);// ���ͱ궨ֵ
					state = REIMU;
					break;
			}// end of switch(state) 			 
			system_micrsecond = micros();	 	// ȡϵͳʱ�� ��λ us 
			LED_Change();						// LED1�ı�����
		}
	//--------------------------------------------------
		//����PC������������
		if((ucPC_cmd = UART2_CommandRoute()) != 0xff) {
			switch(ucPC_cmd){ // ��������ʶ
				case Gyro_init:			// ��ȡ��������ƫ
					MPU6050_InitGyro_Offset(); 
					break; 	
				case High_init:			// ��ѹ�߶� ����	
					BMP180_ResetAlt(0); 	
					break;		
				case HMC_calib_begin:	// ��ʼ�����Ʊ궨
					HMC5883L_Start_Calib();	
					break; 		
				case HMC_calib:			// ��������Ʊ궨		
					HMC5883L_Save_Calib();	
					break;   	
			}
		}// end if

	}// end of while

}  // end of main	

//------------------End of File----------------------------
