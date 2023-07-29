/*********************************************************************************************
程序名： 　　 STC12C5616AD 连接1602液晶屏测试
编写人： 　　 　
编写时间：　　
硬件支持：　　STC12C5616AD  12MHz晶振
接口说明：　　
	- 将 液晶屏时钟.hex 文件下载到 STC12C5616AD
	- 1602液晶屏共16个引脚，从左到右标注为：
			1. GND
			2. VDD
			3. VO
			4. RS
			5. RW
			6. E
			7. D0
			8. D1
			9. D2
			10.D3
			11.D4
			12.D5
			13.D6
			14.D7
			15.BLA
			16.BLK
	- 将 1602液晶屏 的引脚连接如下：
			先断开usb 5v 电源
				1. GND 接地
				2. VDD 接正极
				3. VO 接一个10K电位器，电位器接地，此时可通过电位器调节 1602液晶屏 对比度
			接通usb 5v 电源
			观察 1602液晶屏 是否有16个全黑的方框，如果有表示 1602液晶屏 灰度正常
			
			先断开usb 5v 电源
				15.BLA 通过8055三极管、1k电阻接到 P3.7，设置 P3.7=0 开启背光最大亮度
				16.BLK 接地
			接通usb 5v 电源
			观察 1602液晶屏 是否有黄绿色背光，如果有表示 1602液晶屏 背光正常
			
			先断开usb 5v 电源
				4. RS 接 P32
				5. RW 接 P33
				6. E  接 P34
				7. D0 接 P10
				8. D1 接 P11
				9. D2 接 P12
				10.D3 接 P13
				11.D4 接 P14
				12.D5 接 P15
				13.D6 接 P16
				14.D7 接 P17
			接通usb 5v 电源
			观察 1602液晶屏 是否有黄绿色背光，且显示正在增加的日期时间，如果有，则程序正常
			


/*********************************************************************************************/


//#include <REG51.h> //通用89C51头文件
#include <STC12C5620AD.H> // STC12C5616AD 凑合用
//#include <STC12C5A60S2.H> // STC12C5A60S2 系列单片机头文件




/********************************************************************************************/
typedef unsigned char      uint8;          // 无符号8位整型变量 //

/********************************************************************************************
// 引脚定义 // （使用者根据实际更改）
/********************************************************************************************/
#define		LCM2402_DB0_DB7		P1			// 定义LCM2402的数据总线
sbit LCM2402_RS   = P3 ^ 2;					// 定义LCM2402的RS控制线
sbit LCM2402_RW   = P3 ^ 3;					// 定义LCM2402的RW控制线
sbit LCM2402_E    = P3 ^ 4;					// 定义LCM2402的E控制线
sbit LCM2402_Busy = P1 ^ 7;					// 定义LCM2402的测忙线（与LCM2402_DB0_DB7关联）
data unsigned char TIME_DD,TIME_MO,TIME_YY,TIME_WW,TIME_HH,TIME_MM,TIME_SS;//设置日、月、年、周、时、分、秒和温度存放区
data bit DAY_BIT = 0;//天数增加标志位（用于日期进位的启动）
data unsigned char DIS_BIT = 0; //多种信息的切换显示
data unsigned char cou  = 0;     // 软计数器,对10ms时基信号累加到1s 
/********************************************************************************************
// 定义LCM2402指令集 // （详细请见技术手册）
/********************************************************************************************/
#define			CMD_clear		0x01             // 清除屏幕
#define			CMD_back		0x02             // DDRAM回零位
#define			CMD_dec1		0x04             // 读入后AC（指针）减1，向左写
#define			CMD_add1		0x06             // 读入后AC（指针）加1，向右写
#define			CMD_dis_gb1		0x0f             // 开显示_开光标_开光标闪烁
#define			CMD_dis_gb2		0x0e             // 开显示_开光标_关光标闪烁
#define			CMD_dis_gb3		0x0c             // 开显示_关光标_关光标闪烁
#define			CMD_OFF_dis		0x08             // 关显示_关光标_关光标闪烁
#define			CMD_set82		0x38             // 8位总线_2行显示
#define			CMD_set81		0x30             // 8位总线_1行显示（上边行）
#define			CMD_set42		0x28             // 4位总线_2行显示
#define			CMD_set41		0x20             // 4位总线_1行显示（上边行）
#define			lin_1			0x80             // 4位总线_1行显示（上边行）
#define			lin_2			0xc0             // 4位总线_1行显示（上边行）

/********************************************************************************************
// 读LCM忙程序 [底层协议] // （所有底层协议都无需关注）
// LCM2402测忙，若LCM2402处于忙状态，本函数将等待至非忙状态 //
/********************************************************************************************/
void LCM2402_TestBusy(void){
   	LCM2402_DB0_DB7 = 0xff;		//设备读状态
   	LCM2402_RS = 0;
   	LCM2402_RW = 1;
   	LCM2402_E = 1;
   	while(LCM2402_Busy);		//等待LCM不忙
   	LCM2402_E = 0;				//
}
/********************************************************************************************
// 写指令程序 //
// 向LCM2402写命令 本函数需要1个指令集的入口参数 //
/********************************************************************************************/
void LCM2402_WriteCMD(uint8 LCM2402_command) { 
  	LCM2402_TestBusy();
  	LCM2402_DB0_DB7 = LCM2402_command;
  	LCM2402_RS = 0;
  	LCM2402_RW = 0;
  	LCM2402_E = 1;
  	LCM2402_E = 0;
}
/********************************************************************************************
// 写数据程序 //
// 向LCM2402写数据 //
/********************************************************************************************/
void LCM2402_WriteData(uint8 LCM2402_data){ 
    LCM2402_TestBusy();
	LCM2402_DB0_DB7 = LCM2402_data;
  	LCM2402_RS = 1;
  	LCM2402_RW = 0;
  	LCM2402_E = 1;
  	LCM2402_E = 0;
}
/********************************************************************************************
// 打印字符串程序 // （本函数调用指针函数）
// 向LCM发送一个字符串,长度48字符之内 
// 第一行位置 0x00~0x17  第二行位置 0x40~0x57 
// 应用举例：print(0x80,"doyoung.net"); //在第一行第一位处从左向右打印doyoung.net字符串
/********************************************************************************************/
void print(uint8 a,uint8 *str){
	LCM2402_WriteCMD(a | 0x80);
	while(*str != '\0'){
		LCM2402_WriteData(*str++);
	}
	*str = 0;
}
/********************************************************************************************
// 打印单字符程序 // 
// 第一行位置 0x00~0x17  第二行位置 0x40~0x57 
// 向LCM发送一个字符,以十六进制（0x00）表示 
// 应用举例：print(0xc0,0x30); //在第二行第一位处打印字符“0”
/********************************************************************************************/
void print2(uint8 a,uint8 t){
		LCM2402_WriteCMD(a | 0x80);
		LCM2402_WriteData(t);
}
/********************************************************************************************
// 定义小汉字 //
// 可写入8个自字义字符，写入后可用其CGRAM代码直接提取显示。
// 字符定义方法请参考技术手册 
/********************************************************************************************/
uint8 code Xword[]={
    0x18,0x18,0x07,0x08,0x08,0x08,0x07,0x00,        //℃，代码 0x00
    0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,        //一，代码 0x01
    0x00,0x00,0x00,0x0e,0x00,0xff,0x00,0x00,        //二，代码 0x02
    0x00,0x00,0xff,0x00,0x0e,0x00,0xff,0x00,        //三，代码 0x03
    0x00,0x00,0xff,0xf5,0xfb,0xf1,0xff,0x00,        //四，代码 0x04
    0x00,0xfe,0x08,0xfe,0x0a,0x0a,0xff,0x00,        //五，代码 0x05
    0x00,0x04,0x00,0xff,0x00,0x0a,0x11,0x00,        //六，代码 0x06
    0x00,0x1f,0x11,0x1f,0x11,0x11,0x1f,0x00,        //日，代码 0x07
};
void CgramWrite(void) {	// 装入CGRAM //
    uint8 i;
	LCM2402_WriteCMD(0x06);			// CGRAM地址自动加1
	LCM2402_WriteCMD(0x40);			// CGRAM地址设为00处
    for(i=0;i<64;i++) {
    	LCM2402_WriteData(Xword[i]);// 按数组写入数据
    }
}
/********************************************************************************************
// LCM2402初始化 //（使用者可自定义，加 * 号程序行必须保留但可修改）
/********************************************************************************************/
void LCM2402_Init(void){
  	LCM2402_WriteCMD(CMD_set82);	//* 显示模式设置：显示2行，每个字符为5*7个像素
  	LCM2402_WriteCMD(CMD_clear);	//  显示清屏
  	LCM2402_WriteCMD(CMD_back);		//* 数据指针指向第1行第1个字符位置
  	LCM2402_WriteCMD(CMD_add1);		//  显示光标移动设置：文字不动，光标右移
  	LCM2402_WriteCMD(CMD_dis_gb3); 	//  显示开及光标设置：显示开，光标开，闪烁开
	CgramWrite();					//  向CGRAM写入自定义字符
}
/********************************************************************************************/
//			以上是LCM2402驱动程序			//
/*********************************************************************************************/

/*********************************************************************************************/
bit IsLeapYear(void){    //计算本年是否是润年 
	unsigned int a;
	a = 2000+TIME_YY;//加2000表示成完整的年
	if((a%4==0 && a%100!=0)||(a%400==0)){ //润年的计算公式
			return 1;//是润年返回1 
		}else{ 
 			return 0;//不是润年返回0 
		}
} 
/**********************************************************************************************/	
void month_day(void){  
	unsigned char mon_day[]={31,28,31,30,31,30,31,31,30,31,30,31};
	TIME_DD++;//天加1
	TIME_WW++;//星期值加1
	if(TIME_WW > 7){
		TIME_WW = 1;//时期值限定
	}
	if(TIME_DD > mon_day[TIME_MO-1]){//检查天是否大于当月的最大值
		if(IsLeapYear()&&TIME_MO==2){ //计算本月是否是润年的2月份 
			TIME_DD = 29;//如果是润年又是2月，则天为29
		}else{
			TIME_DD = 1; //否则就等于1
			TIME_MO++;//月加1
			if(TIME_MO > 12){
				TIME_MO = 1; //如果月大于12则月等于1
				TIME_YY++;//年加1 （公历年无限积加）
			}
		}
	}
} 
/**********************************************************************************************/	
void init (void){ //上电初始化
	TMOD = 0x11;         // 定时/计数器0,1工作于方式1   
    TH0 = 0x3c;          // 预置产生50ms时基信号   
    TL0 = 0xb0;   
    EA = 1;              // 开总中断   
    ET0 = 1;             // 定时/计数器0允许中断   
    TR0 = 1;             // 开闭定时/计数器0   
////
	TIME_DD = 6; //时间在首次使用的值，之后会在EEPROM自动记录上一天的值
	TIME_MO	= 7; //初始时间：2009年5月18日周一，20时13分40秒
	TIME_YY = 9;
	TIME_WW = 1;
	TIME_HH	= 0;
	TIME_MM = 0;
	TIME_SS = 0;
}
/********************************************************************************************
//显示项目 时间部分 在第一行全行显示时间
*********************************************************************************************/    
void RealTime_Display(void){
	    print(0x80,"20");
	    print2(0x82,TIME_YY/10+0x30);
	    print2(0x83,TIME_YY%10+0x30);
		print(0x84,"/");            // 显示年
		//
	    print2(0x85,TIME_MO/10+0x30);
	    print2(0x86,TIME_MO%10+0x30);
		print(0x87,"/");            // 显示月
		//
	    print2(0x88,TIME_DD/10+0x30);
	    print2(0x89,TIME_DD%10+0x30);

		print(0x8b,"[");            // 显示[
		print2(0x8c,TIME_WW%10); //星期
		print(0x8d,"]");            // 显示]

	    print2(0x40,TIME_HH/10+0x30);//小时
	    print2(0x41,TIME_HH%10+0x30);
		print(0x42,":");            // 显示cgram第一个字模"："
		//
	    print2(0x43,TIME_MM/10+0x30);//分钟
	    print2(0x44,TIME_MM%10+0x30);
		print(0x45,".");            // 显示cgram第一个字模"."
		//
	    print2(0x46,TIME_SS/10+0x30);//秒
	    print2(0x47,TIME_SS%10+0x30);
		//
}




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





/********************************************************************************************/
// 测试用函数 //
void main (void){
	init();//初始化
	PWM_init();  // PWM调光设置初始化
	PWM0_set(22);  // 0 - 255 共256级调节背光亮度
	LCM2402_Init();//LCM2402初始化                           
	while(1){ //主线程// 
		RealTime_Display();    	
//		print(0x80,"DoYoung Studio"); //在第一行第一位处从左向右打印doyoung.net字符串
//		print(0x40,"www.DoYoung.net"); //在第一行第一位处从左向右打印doyoung.net字符串

		if(DAY_BIT == 1){ //检查天数是否更新，是则计算公历
			month_day();//计算公历日期	
			DAY_BIT = 0;//计算完成后将日期变更标志位置0
		}
	}
}
/********************************************************************************************/    
void tiem0(void) interrupt 1{   // T/C0中断服务程序(产生50ms时基信号)   
    cou++;                      // 软计数器加1   
    if(cou > 19){                 // 计数值到100(1s)   
        cou = 0;               // 软计数器清零   
        TIME_SS++;                 // 秒计数器加1(进位10ms*100=1s)   
        if(TIME_SS > 59){          // 秒计数值到60   
           
            TIME_SS = 0;           // 秒计数器清零   
            TIME_MM++;             // 分计数器加1(进位60s=1m)  
            if(TIME_MM > 59){      // 分计数到60   
                TIME_MM = 0;       // 分计数器清零   
                TIME_HH++;         // 时计数器加1(进位60m=1h)   
                if(TIME_HH > 23){  // 时计数到23   
                    TIME_HH = 0;   // 时计数器清零
					DAY_BIT = 1;	//天增加标志位 
				}  
            }   
        }   
    }   
    TH0 = 0x3c;                // 重置定时常数   
    TL0 = 0xb0;   
}

