/*********************************************************************************************
程序名： 　　 PWM模块调试程序（仅实验用）
编写人： 　　 杜洋　
编写时间：　　2009年7月22日
硬件支持：　　STC12C2052  12MHz
接口说明：　　LED灌电流接P3.7接口（PWM0）  
修改日志：　　
　　NO.1-								
/*********************************************************************************************
说明：

/*********************************************************************************************/

#include <STC12C5620AD.H> //STC12C5620AD

/*********************************************************************************************
函数名：PWM初始化函数
调  用：PWM_init();
参  数：无
返回值：无
结  果：将PCA初始化为PWM模式，初始占空比为0
备  注：需要更多路PWM输出直接插入CCAPnH和CCAPnL即可
/**********************************************************************************************/
void PWM_init (void){
	CMOD=0x02; //设置PCA定时器
 	CL=0x00; 
 	CH=0x00;
	CCAPM0=0x42; //PWM0设置PCA工作方式为PWM方式（0100 0010）
 	CCAP0L=0x00; //设置PWM0初始值与CCAP0H相同
 	CCAP0H=0x00; // PWM0初始时为0

	//CCAPM1=0x42; //PWM1设置PCA工作方式为PWM方式（使用时删除//）
	//CCAP1L=0x00; //设置PWM1初始值与CCAP0H相同
 	//CCAP1H=0x00; // PWM1初始时为0

	//CCAPM2=0x42; //PWM2设置PCA工作方式为PWM方式
	//CCAP2L=0x00; //设置PWM2初始值与CCAP0H相同
 	//CCAP2H=0x00; // PWM2初始时为0

	//CCAPM3=0x42; //PWM3设置PCA工作方式为PWM方式
	//CCAP3L=0x00; //设置PWM3初始值与CCAP0H相同
 	//CCAP3H=0x00; // PWM3初始时为0

 	CR=1; //启动PCA定时器
}
/**********************************************************************************************/

/*********************************************************************************************
函数名：PWM0占空比设置函数
调  用：PWM0_set();
参  数：0x00~0xFF（亦可用0~255）
返回值：无
结  果：设置PWM模式占空比，为0时全部高电平，为1时全部低电平
备  注：如果需要PWM1的设置函数，只要把CCAP0L和CCAP0H中的0改为1即可
/**********************************************************************************************/
void PWM0_set (unsigned char a){
	CCAP0L= a; //设置值直接写入CCAP0L
 	CCAP0H= a; //设置值直接写入CCAP0H
}
/**********************************************************************************************/



 /***************************************************************************
******************
-函数名：毫秒级CPU延时函数
-调  用：DELAY_MS (?);
-参  数：1~65535（参数不可为0）
 返回值：无
-结  果：占用CPU方式延时与参数数值相同的毫秒时间
-备  注：应用于1T单片机时i<600，应用于12T单片机时i<125
-/***************************************************************************
******************/
void DELAY_MS (unsigned int a){
       unsigned int i;
       while( --a != 0){
               for(i = 0; i < 600; i++);
       }
		 }
/**********************************************************************************************/



/*********************************************************************************************
函数名：主函数
调  用：无
参  数：无
返回值：无
结  果：程序开始处，无限循环
备  注：
/**********************************************************************************************/
void main (void){

	PWM_init(); //PWM初始化
	PWM0_set(0x90); //设置PWM占空比，从而控制led亮度，0x00到0xFF，最暗到最亮

	while(1){
		
	}
}
/**********************************************************************************************/
/*************************************************************
* 杜洋工作室 www.DoYoung.net
/*************************************************************/


