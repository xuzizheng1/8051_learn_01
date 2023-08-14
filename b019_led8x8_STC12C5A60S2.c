/*********************************************************************************************
程序名： 　　 LED屏基础程序
编写人： 　　 杜洋　
编写时间：　　2010年11月22日
硬件支持：　　STC12C5A60S2　外部12MHZ晶振
接口说明：　　  
修改日志：　　

说明：
使用PWM方式调整LED的亮灭周期，达到亮度变化。

/*********************************************************************************************/

#include <STC12C5A60S2.H> //STC头文件

unsigned char DY_PWM; //每个点的显示时间，与delay程序配合。


sbit	DY_LED1_H1	=P4 ^ 7; //设置LED点阵屏连接的I/O口
sbit	DY_LED1_H2	=P1 ^ 3; //设置LED点阵屏连接的I/O口
sbit	DY_LED1_H3	=P0 ^ 7; //设置LED点阵屏连接的I/O口
sbit	DY_LED1_H4	=P1 ^ 5; //设置LED点阵屏连接的I/O口
sbit	DY_LED1_H5	=P0 ^ 0; //设置LED点阵屏连接的I/O口
sbit	DY_LED1_H6	=P0 ^ 6; //设置LED点阵屏连接的I/O口
sbit	DY_LED1_H7	=P0 ^ 1; //设置LED点阵屏连接的I/O口
sbit	DY_LED1_H8	=P0 ^ 4; //设置LED点阵屏连接的I/O口

sbit	DY_LED1_L1	=P1 ^ 4; //设置LED点阵屏连接的I/O口
sbit	DY_LED1_L2	=P0 ^ 2; //设置LED点阵屏连接的I/O口
sbit	DY_LED1_L3	=P0 ^ 3; //设置LED点阵屏连接的I/O口
sbit	DY_LED1_L4	=P1 ^ 7; //设置LED点阵屏连接的I/O口
sbit	DY_LED1_L5	=P0 ^ 5; //设置LED点阵屏连接的I/O口
sbit	DY_LED1_L6	=P1 ^ 6; //设置LED点阵屏连接的I/O口
sbit	DY_LED1_L7	=P1 ^ 2; //设置LED点阵屏连接的I/O口
sbit	DY_LED1_L8	=P1 ^ 1; //设置LED点阵屏连接的I/O口


sbit	DY_LED2_H1	=P3 ^ 7; //设置LED点阵屏连接的I/O口
sbit	DY_LED2_H2	=P3 ^ 2; //设置LED点阵屏连接的I/O口
sbit	DY_LED2_H3	=P2 ^ 3; //设置LED点阵屏连接的I/O口
sbit	DY_LED2_H4	=P3 ^ 4; //设置LED点阵屏连接的I/O口
sbit	DY_LED2_H5	=P4 ^ 6; //设置LED点阵屏连接的I/O口
sbit	DY_LED2_H6	=P2 ^ 4; //设置LED点阵屏连接的I/O口
sbit	DY_LED2_H7	=P4 ^ 5; //设置LED点阵屏连接的I/O口
sbit	DY_LED2_H8	=P2 ^ 6; //设置LED点阵屏连接的I/O口

sbit	DY_LED2_L1	=P3 ^ 3; //设置LED点阵屏连接的I/O口
sbit	DY_LED2_L2	=P4 ^ 4; //设置LED点阵屏连接的I/O口
sbit	DY_LED2_L3	=P2 ^ 7; //设置LED点阵屏连接的I/O口
sbit	DY_LED2_L4	=P3 ^ 6; //设置LED点阵屏连接的I/O口
sbit	DY_LED2_L5	=P2 ^ 5; //设置LED点阵屏连接的I/O口
sbit	DY_LED2_L6	=P3 ^ 5; //设置LED点阵屏连接的I/O口
sbit	DY_LED2_L7	=P3 ^ 1; //设置LED点阵屏连接的I/O口
sbit	DY_LED2_L8	=P3 ^ 0; //设置LED点阵屏连接的I/O口

sbit	DY_BEEP		=P1 ^ 0; //扬声器
sbit	DY_18B20	=P2 ^ 1; //温度传感器
sbit	DY_KEY		=P2 ^ 2; //按键
sbit	DY_POWER	=P2 ^ 0; //电源监测


#define DY_P0M1SET		0x00 //设置I/O口工作方式//00000000
#define DY_P0M0SET		0xd3 //设置I/O口工作方式//11010011
#define DY_P1M1SET		0x00 //设置I/O口工作方式//00000000
#define DY_P1M0SET		0x28 //设置I/O口工作方式//00101000
#define DY_P2M1SET		0x00 //设置I/O口工作方式//00000000
#define DY_P2M0SET		0x58 //设置I/O口工作方式//01011000
#define DY_P3M1SET		0x00 //设置I/O口工作方式//00000000
#define DY_P3M0SET		0x94 //设置I/O口工作方式//10010100
#define DY_P4M1SET		0x00 //设置I/O口工作方式//00000000
#define DY_P4M0SET		0xe0 //设置I/O口工作方式//11100000

/**********************************************************************************************/
unsigned char code SPEED[]={//显示“SPEED”即速度设置字样

	0,0x26,0x49,0x49,0x49,0x32,0,0,//S
	0,0x7F,0x09,0x09,0x09,0x06,0,0,//P
	0,0x7F,0x49,0x49,0x49,0x41,0,0,//E
	0,0x7F,0x49,0x49,0x49,0x41,0,0,//E
	0,0x7F,0x41,0x41,0x41,0x3E,0,0,//D
	0,0x00,0x00,0x00,0x00,0x00,0,0
};

/*********************************************************************************************/
void delay (unsigned int a){ // 0.1ms延时程序（12MHz 10倍于51单片机速度时）
	unsigned int i;
	while( --a != 0){
		for(i = 0; i < 1; i++); 
	}   				   
}
/*********************************************************************************************/
void dis_off (void){
	P0 = ~0xd3; //关所有显示
	P1 = ~0x28;
	P2 = ~0x58;
	P3 = ~0x94;
	P4 = ~0xe0;
	delay(100-DY_PWM); 
}
/*********************************************************************************************/
void displayHH1 (unsigned char d){ //第1列横向显示程序
	unsigned char i;
	i = d & 0x01;
	if(i == 0x01){
		DY_LED1_H1 = 1;DY_LED1_L1 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x02;
	if(i == 0x02){
		DY_LED1_H2 = 1;DY_LED1_L1 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x04;
	if(i == 0x04){
		DY_LED1_H3 = 1;DY_LED1_L1 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x08;
	if(i == 0x08){
		DY_LED1_H4 = 1;DY_LED1_L1 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x10;
	if(i == 0x10){
		DY_LED1_H5 = 1;DY_LED1_L1 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x20;
	if(i == 0x20){
		DY_LED1_H6 = 1;DY_LED1_L1 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x40;
	if(i == 0x40){
		DY_LED1_H7 = 1;DY_LED1_L1 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x80;
	if(i == 0x80){
		DY_LED1_H8 = 1;DY_LED1_L1 = 0;delay(DY_PWM);dis_off();
	}
} 
/*********************************************************************************************/
void displayHH2 (unsigned char d){ //第2列横向显示程序
	unsigned char i;
	i = d & 0x01;
	if(i == 0x01){
		DY_LED1_H1 = 1;DY_LED1_L2 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x02;
	if(i == 0x02){
		DY_LED1_H2 = 1;DY_LED1_L2 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x04;
	if(i == 0x04){
		DY_LED1_H3 = 1;DY_LED1_L2 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x08;
	if(i == 0x08){
		DY_LED1_H4 = 1;DY_LED1_L2 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x10;
	if(i == 0x10){
		DY_LED1_H5 = 1;DY_LED1_L2 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x20;
	if(i == 0x20){
		DY_LED1_H6 = 1;DY_LED1_L2 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x40;
	if(i == 0x40){
		DY_LED1_H7 = 1;DY_LED1_L2 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x80;
	if(i == 0x80){
		DY_LED1_H8 = 1;DY_LED1_L2 = 0;delay(DY_PWM);dis_off();
	}

} 
/*********************************************************************************************/
void displayHH3 (unsigned char d){ //第3列横向显示程序
	unsigned char i;
	i = d & 0x01;
	if(i == 0x01){
		DY_LED1_H1 = 1;DY_LED1_L3 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x02;
	if(i == 0x02){
		DY_LED1_H2 = 1;DY_LED1_L3 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x04;
	if(i == 0x04){
		DY_LED1_H3 = 1;DY_LED1_L3 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x08;
	if(i == 0x08){
		DY_LED1_H4 = 1;DY_LED1_L3 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x10;
	if(i == 0x10){
		DY_LED1_H5 = 1;DY_LED1_L3 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x20;
	if(i == 0x20){
		DY_LED1_H6 = 1;DY_LED1_L3 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x40;
	if(i == 0x40){
		DY_LED1_H7 = 1;DY_LED1_L3 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x80;
	if(i == 0x80){
		DY_LED1_H8 = 1;DY_LED1_L3 = 0;delay(DY_PWM);dis_off();
	}

} 
/*********************************************************************************************/
void displayHH4 (unsigned char d){ //第4列横向显示程序
	unsigned char i;
	i = d & 0x01;
	if(i == 0x01){
		DY_LED1_H1 = 1;DY_LED1_L4 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x02;
	if(i == 0x02){
		DY_LED1_H2 = 1;DY_LED1_L4 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x04;
	if(i == 0x04){
		DY_LED1_H3 = 1;DY_LED1_L4 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x08;
	if(i == 0x08){
		DY_LED1_H4 = 1;DY_LED1_L4 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x10;
	if(i == 0x10){
		DY_LED1_H5 = 1;DY_LED1_L1 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x20;
	if(i == 0x20){
		DY_LED1_H6 = 1;DY_LED1_L4 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x40;
	if(i == 0x40){
		DY_LED1_H7 = 1;DY_LED1_L4 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x80;
	if(i == 0x80){
		DY_LED1_H8 = 1;DY_LED1_L4 = 0;delay(DY_PWM);dis_off();
	}
} 
/*********************************************************************************************/
void displayHH5 (unsigned char d){ //第5列横向显示程序
	unsigned char i;
	i = d & 0x01;
	if(i == 0x01){
		DY_LED1_H1 = 1;DY_LED1_L5 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x02;
	if(i == 0x02){
		DY_LED1_H2 = 1;DY_LED1_L5 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x04;
	if(i == 0x04){
		DY_LED1_H3 = 1;DY_LED1_L5 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x08;
	if(i == 0x08){
		DY_LED1_H4 = 1;DY_LED1_L5 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x10;
	if(i == 0x10){
		DY_LED1_H5 = 1;DY_LED1_L5 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x20;
	if(i == 0x20){
		DY_LED1_H6 = 1;DY_LED1_L5 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x40;
	if(i == 0x40){
		DY_LED1_H7 = 1;DY_LED1_L5 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x80;
	if(i == 0x80){
		DY_LED1_H8 = 1;DY_LED1_L5 = 0;delay(DY_PWM);dis_off();
	}
} 
/*********************************************************************************************/
void displayHH6 (unsigned char d){ //第6列横向显示程序
	unsigned char i;
	i = d & 0x01;
	if(i == 0x01){
		DY_LED1_H1 = 1;DY_LED1_L6 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x02;
	if(i == 0x02){
		DY_LED1_H2 = 1;DY_LED1_L6 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x04;
	if(i == 0x04){
		DY_LED1_H3 = 1;DY_LED1_L6 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x08;
	if(i == 0x08){
		DY_LED1_H4 = 1;DY_LED1_L6 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x10;
	if(i == 0x10){
		DY_LED1_H5 = 1;DY_LED1_L6 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x20;
	if(i == 0x20){
		DY_LED1_H6 = 1;DY_LED1_L6 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x40;
	if(i == 0x40){
		DY_LED1_H7 = 1;DY_LED1_L6 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x80;
	if(i == 0x80){
		DY_LED1_H8 = 1;DY_LED1_L6 = 0;delay(DY_PWM);dis_off();
	}
} 
/*********************************************************************************************/
void displayHH7 (unsigned char d){ //第7列横向显示程序
	unsigned char i;
	i = d & 0x01;
	if(i == 0x01){
		DY_LED1_H1 = 1;DY_LED1_L7 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x02;
	if(i == 0x02){
		DY_LED1_H2 = 1;DY_LED1_L7 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x04;
	if(i == 0x04){
		DY_LED1_H3 = 1;DY_LED1_L7 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x08;
	if(i == 0x08){
		DY_LED1_H4 = 1;DY_LED1_L7 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x10;
	if(i == 0x10){
		DY_LED1_H5 = 1;DY_LED1_L7 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x20;
	if(i == 0x20){
		DY_LED1_H6 = 1;DY_LED1_L7 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x40;
	if(i == 0x40){
		DY_LED1_H7 = 1;DY_LED1_L7 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x80;
	if(i == 0x80){
		DY_LED1_H8 = 1;DY_LED1_L7 = 0;delay(DY_PWM);dis_off();
	}
} 
/*********************************************************************************************/
void displayHH8 (unsigned char d){ //第8列横向显示程序
	unsigned char i;
	i = d & 0x01;
	if(i == 0x01){
		DY_LED1_H1 = 1;DY_LED1_L8 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x02;
	if(i == 0x02){
		DY_LED1_H2 = 1;DY_LED1_L8 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x04;
	if(i == 0x04){
		DY_LED1_H3 = 1;DY_LED1_L8 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x08;
	if(i == 0x08){
		DY_LED1_H4 = 1;DY_LED1_L8 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x10;
	if(i == 0x10){
		DY_LED1_H5 = 1;DY_LED1_L8 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x20;
	if(i == 0x20){
		DY_LED1_H6 = 1;DY_LED1_L8 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x40;
	if(i == 0x40){
		DY_LED1_H7 = 1;DY_LED1_L8 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x80;
	if(i == 0x80){
		DY_LED1_H8 = 1;DY_LED1_L8 = 0;delay(DY_PWM);dis_off();
	}
}
/*********************************************************************************************/
void displayHH9 (unsigned char d){ //第9列横向显示程序
	unsigned char i;
	i = d & 0x01;
	if(i == 0x01){
		DY_LED2_H1 = 1;DY_LED2_L1 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x02;
	if(i == 0x02){
		DY_LED2_H2 = 1;DY_LED2_L1 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x04;
	if(i == 0x04){
		DY_LED2_H3 = 1;DY_LED2_L1 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x08;
	if(i == 0x08){
		DY_LED2_H4 = 1;DY_LED2_L1 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x10;
	if(i == 0x10){
		DY_LED2_H5 = 1;DY_LED2_L1 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x20;
	if(i == 0x20){
		DY_LED2_H6 = 1;DY_LED2_L1 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x40;
	if(i == 0x40){
		DY_LED2_H7 = 1;DY_LED2_L1 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x80;
	if(i == 0x80){
		DY_LED2_H8 = 1;DY_LED2_L1 = 0;delay(DY_PWM);dis_off();
	}
}
/*********************************************************************************************/
void displayHH10 (unsigned char d){ //第10列横向显示程序
	unsigned char i;
	i = d & 0x01;
	if(i == 0x01){
		DY_LED2_H1 = 1;DY_LED2_L2 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x02;
	if(i == 0x02){
		DY_LED2_H2 = 1;DY_LED2_L2 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x04;
	if(i == 0x04){
		DY_LED2_H3 = 1;DY_LED2_L2 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x08;
	if(i == 0x08){
		DY_LED2_H4 = 1;DY_LED2_L2 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x10;
	if(i == 0x10){
		DY_LED2_H5 = 1;DY_LED2_L2 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x20;
	if(i == 0x20){
		DY_LED2_H6 = 1;DY_LED2_L2 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x40;
	if(i == 0x40){
		DY_LED2_H7 = 1;DY_LED2_L2 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x80;
	if(i == 0x80){
		DY_LED2_H8 = 1;DY_LED2_L2 = 0;delay(DY_PWM);dis_off();
	}
}
/*********************************************************************************************/
void displayHH11 (unsigned char d){ //第11列横向显示程序
	unsigned char i;
	i = d & 0x01;
	if(i == 0x01){
		DY_LED2_H1 = 1;DY_LED2_L3 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x02;
	if(i == 0x02){
		DY_LED2_H2 = 1;DY_LED2_L3 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x04;
	if(i == 0x04){
		DY_LED2_H3 = 1;DY_LED2_L3 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x08;
	if(i == 0x08){
		DY_LED2_H4 = 1;DY_LED2_L3 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x10;
	if(i == 0x10){
		DY_LED2_H5 = 1;DY_LED2_L3 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x20;
	if(i == 0x20){
		DY_LED2_H6 = 1;DY_LED2_L3 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x40;
	if(i == 0x40){
		DY_LED2_H7 = 1;DY_LED2_L3 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x80;
	if(i == 0x80){
		DY_LED2_H8 = 1;DY_LED2_L3 = 0;delay(DY_PWM);dis_off();
	}
}
/*********************************************************************************************/
void displayHH12 (unsigned char d){ //第12列横向显示程序
	unsigned char i;
	i = d & 0x01;
	if(i == 0x01){
		DY_LED2_H1 = 1;DY_LED2_L4 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x02;
	if(i == 0x02){
		DY_LED2_H2 = 1;DY_LED2_L4 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x04;
	if(i == 0x04){
		DY_LED2_H3 = 1;DY_LED2_L4 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x08;
	if(i == 0x08){
		DY_LED2_H4 = 1;DY_LED2_L4 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x10;
	if(i == 0x10){
		DY_LED2_H5 = 1;DY_LED2_L4 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x20;
	if(i == 0x20){
		DY_LED2_H6 = 1;DY_LED2_L4 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x40;
	if(i == 0x40){
		DY_LED2_H7 = 1;DY_LED2_L4 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x80;
	if(i == 0x80){
		DY_LED2_H8 = 1;DY_LED2_L4 = 0;delay(DY_PWM);dis_off();
	}
}
/*********************************************************************************************/
void displayHH13 (unsigned char d){ //第13列横向显示程序
	unsigned char i;
	i = d & 0x01;
	if(i == 0x01){
		DY_LED2_H1 = 1;DY_LED2_L5 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x02;
	if(i == 0x02){
		DY_LED2_H2 = 1;DY_LED2_L5 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x04;
	if(i == 0x04){
		DY_LED2_H3 = 1;DY_LED2_L5 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x08;
	if(i == 0x08){
		DY_LED2_H4 = 1;DY_LED2_L5 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x10;
	if(i == 0x10){
		DY_LED2_H5 = 1;DY_LED2_L5 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x20;
	if(i == 0x20){
		DY_LED2_H6 = 1;DY_LED2_L5 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x40;
	if(i == 0x40){
		DY_LED2_H7 = 1;DY_LED2_L5 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x80;
	if(i == 0x80){
		DY_LED2_H8 = 1;DY_LED2_L5 = 0;delay(DY_PWM);dis_off();
	}
}
/*********************************************************************************************/
void displayHH14 (unsigned char d){ //第14列横向显示程序
	unsigned char i;
	i = d & 0x01;
	if(i == 0x01){
		DY_LED2_H1 = 1;DY_LED2_L6 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x02;
	if(i == 0x02){
		DY_LED2_H2 = 1;DY_LED2_L6 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x04;
	if(i == 0x04){
		DY_LED2_H3 = 1;DY_LED2_L6 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x08;
	if(i == 0x08){
		DY_LED2_H4 = 1;DY_LED2_L6 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x10;
	if(i == 0x10){
		DY_LED2_H5 = 1;DY_LED2_L6 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x20;
	if(i == 0x20){
		DY_LED2_H6 = 1;DY_LED2_L6 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x40;
	if(i == 0x40){
		DY_LED2_H7 = 1;DY_LED2_L6 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x80;
	if(i == 0x80){
		DY_LED2_H8 = 1;DY_LED2_L6 = 0;delay(DY_PWM);dis_off();
	}
}
/*********************************************************************************************/
void displayHH15 (unsigned char d){ //第15列横向显示程序
	unsigned char i;
	i = d & 0x01;
	if(i == 0x01){
		DY_LED2_H1 = 1;DY_LED2_L7 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x02;
	if(i == 0x02){
		DY_LED2_H2 = 1;DY_LED2_L7 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x04;
	if(i == 0x04){
		DY_LED2_H3 = 1;DY_LED2_L7 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x08;
	if(i == 0x08){
		DY_LED2_H4 = 1;DY_LED2_L7 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x10;
	if(i == 0x10){
		DY_LED2_H5 = 1;DY_LED2_L7 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x20;
	if(i == 0x20){
		DY_LED2_H6 = 1;DY_LED2_L7 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x40;
	if(i == 0x40){
		DY_LED2_H7 = 1;DY_LED2_L7 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x80;
	if(i == 0x80){
		DY_LED2_H8 = 1;DY_LED2_L7 = 0;delay(DY_PWM);dis_off();
	}
}
/*********************************************************************************************/
void displayHH16 (unsigned char d){ //第16列横向显示程序
	unsigned char i;
	i = d & 0x01;
	if(i == 0x01){
		DY_LED2_H1 = 1;DY_LED2_L8 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x02;
	if(i == 0x02){
		DY_LED2_H2 = 1;DY_LED2_L8 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x04;
	if(i == 0x04){
		DY_LED2_H3 = 1;DY_LED2_L8 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x08;
	if(i == 0x08){
		DY_LED2_H4 = 1;DY_LED2_L8 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x10;
	if(i == 0x10){
		DY_LED2_H5 = 1;DY_LED2_L8 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x20;
	if(i == 0x20){
		DY_LED2_H6 = 1;DY_LED2_L8 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x40;
	if(i == 0x40){
		DY_LED2_H7 = 1;DY_LED2_L8 = 0;delay(DY_PWM);dis_off();
	}
	i = d & 0x80;
	if(i == 0x80){
		DY_LED2_H8 = 1;DY_LED2_L8 = 0;delay(DY_PWM);dis_off();
	}
}
/*********************************************************************************************/
void diplay_data (unsigned char l,unsigned char d){
		switch (l){//显示的列位置		
				case 1://			
					displayHH1(d); //将显示数据送入		
					break;//		
				case 2://			
					displayHH2(d); //将显示数据送入		
					break;//		
				case 3://			
					displayHH3(d); //将显示数据送入		
					break;//		
				case 4://			
					displayHH4(d); //将显示数据送入		
					break;//		
				case 5://			
					displayHH5(d); //将显示数据送入		
					break;//		
				case 6://		
					displayHH6(d); //将显示数据送入		
					break;//		
				case 7://			
					displayHH7(d); //将显示数据送入		
					break;//		
				case 8://		
					displayHH8(d); //将显示数据送入		
					break;//		
				case 9://			
					displayHH9(d); //将显示数据送入		
					break;//		
				case 10://			
					displayHH10(d); //将显示数据送入		
					break;//		
				case 11://			
					displayHH11(d); //将显示数据送入		
					break;//		
				case 12://			
					displayHH12(d); //将显示数据送入		
					break;//		
				case 13://			
					displayHH13(d); //将显示数据送入		
					break;//		
				case 14://		
					displayHH14(d); //将显示数据送入		
					break;//		
				case 15://			
					displayHH15(d); //将显示数据送入		
					break;//		
				case 16://		
					displayHH16(d); //将显示数据送入		
					break;//		
		}
} 
/*********************************************************************************************/
void init (void){ //主程序
	P0M0 = DY_P0M0SET; //设置I/O口工作方式（行为推挽，列为普通输入/输出）
	P0M1 = DY_P0M1SET;
	P1M0 = DY_P1M0SET;
	P1M1 = DY_P1M1SET;
	P2M0 = DY_P2M0SET;
	P2M1 = DY_P2M1SET;
	P3M0 = DY_P3M0SET;
	P3M1 = DY_P3M1SET;
	P4M0 = DY_P4M0SET;
	P4M1 = DY_P4M1SET;
	P4SW = 0xff; //启动P4接口
	dis_off(); 
	DY_PWM = 99;	
}
/*********************************************************************************************/

/*********************************************************************************************/
void main (void){ //主程序 
	init();
	while (1){
		unsigned char a,b,c;
		for(b=0;b<5;b++){
			for(c=0;c<100;c++){
				for(a=1;a<17;a++){
					diplay_data(a,SPEED[a-1+b*8]);
				}
			}
		}
    }
}
