#include <STC12C5620AD.H> //STC12C5620AD
//#include <REG51.h> //通用89C51头文件


sbit LED = P1 ^ 7; //定义1.7脚为 LED
sbit LED2 = P1 ^ 6;
sbit LED3 = P1 ^ 4;

unsigned char i;
unsigned char internal = 20;



/*********************************************************************************************
函数名：毫秒级CPU延时函数
调  用：DELAY_MS (?);
参  数：1~65535（参数不可为0）
返回值：无
结  果：占用CPU方式延时与参数数值相同的毫秒时间
备  注：应用于1T单片机时i<600，应用于12T单片机时i<125
/*********************************************************************************************/
void DELAY_MS (unsigned int a){
	unsigned int i;
	while( --a != 0){
		for(i = 0; i < 600; i++);
	}
}
/*********************************************************************************************/


void main (void){
	while(1){
		for(i=1; i<internal; i++){
			LED = 0; //led所在的LED引脚设置为低电平，将led负极接LED引脚，此时led点亮
			DELAY_MS(i);
			LED = 1;
			DELAY_MS(internal-i);
		}
		for(i=internal-1; i>0; i--){
			LED = 0; //led所在的LED引脚设置为低电平，将led负极接LED引脚，此时led点亮
			DELAY_MS(i);
			LED = 1;
			DELAY_MS(internal-i);
		}
	}
}

