/*********************************************************************************************
程序名： 　　 10位ADC转换实验程序
编写人： 　　 杜洋　
编写时间：　　2010年3月24日
硬件支持：　　STC12C5A60S2 使用10位ADC功能 外部晶体12MHz
接口说明：　　P1.0接口接电位器  
修改日志：　　
　　1-								
/*********************************************************************************************
说明：
PC串口端设置 [ 4800，8，无，1，无 ]
将ADC读出的数值通过串口以十六进制方式显示（第一个数据是ADC高8位，第二个数据是ADC低2位）。
/*********************************************************************************************/

#include <STC12C5A60S2.H> //单片机头文件
#include <intrins.h>	//51基本运算（包括_nop_空函数）

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
	while( a-- != 0){
		for(i = 0; i < 600; i++);
	}
}
/*********************************************************************************************/

/*********************************************************************************************
函数名：UART串口初始化函数
调  用：UART_init();
参  数：无
返回值：无
结  果：启动UART串口接收中断，允许串口接收，启动T/C1产生波特率（占用）
备  注：振荡晶体为12MHz，PC串口端设置 [ 4800，8，无，1，无 ]
/**********************************************************************************************/
void UART_init (void){
	//EA = 1; //允许总中断（如不使用中断，可用//屏蔽）
	//ES = 1; //允许UART串口的中断

	TMOD = 0x20;	//定时器T/C1工作方式2
	SCON = 0x50;	//串口工作方式1，允许串口接收（SCON = 0x40 时禁止串口接收）
	TH1 = 0xF3;	//定时器初值高8位设置
	TL1 = 0xF3;	//定时器初值低8位设置
	PCON = 0x80;	//波特率倍频（屏蔽本句波特率为2400）
	TR1 = 1;	//定时器启动    
}
/**********************************************************************************************/

/*********************************************************************************************
函数名：UART串口发送函数
调  用：UART_T (?);
参  数：需要UART串口发送的数据（8位/1字节）
返回值：无 
结  果：将参数中的数据发送给UART串口，确认发送完成后退出
备  注：
/**********************************************************************************************/
void UART_T (unsigned char UART_data){ //定义串口发送数据变量
	SBUF = UART_data;	//将接收的数据发送回去
	while(TI == 0);		//检查发送中断标志位
	TI = 0;			//令发送中断标志位为0（软件清零）
}
/**********************************************************************************************/
/*********************************************************************************************
函数名：10位A/D转换初始化函数
调  用：Read_init (?);
参  数：输入的端口（0000 0XXX 其中XXX是设置输入端口号，可用十进制0~7表示，0表示P1.0，7表示P1.7）
返回值：无
结  果：开启ADC功能并设置ADC的输入端口
备  注：适用于STC12C5A60S2系列单片机（必须使用STC12C5A60S2.h头文件）
/**********************************************************************************************/
void Read_init (unsigned char CHA){
	unsigned char AD_FIN=0; //存储A/D转换标志
    CHA &= 0x07;            //选择ADC的8个接口中的一个（0000 0111 清0高5位）
    ADC_CONTR = 0x40;		//ADC转换的速度（0XX0 0000 其中XX控制速度，请根据数据手册设置）
    _nop_();
    ADC_CONTR |= CHA;       //选择A/D当前通道
    _nop_();
    ADC_CONTR |= 0x80;      //启动A/D电源
    DELAY_MS(1);            //使输入电压达到稳定（1ms即可）
}
/**********************************************************************************************/
/*********************************************************************************************
函数名：10位A/D转换函数
调  用：ADC_Read ();
参  数：无
返回值：无（10位ADC数据高8位存放在ADC_RES中，低2位存放在ADC_RESL中）
结  果：读出指定ADC接口的A/D转换值，并返回数值
备  注：适用于STC12C5A60S2系列单片机（必须使用STC12C5A60S2.h头文件）
/**********************************************************************************************/
void ADC_Read (void){
	unsigned char AD_FIN=0; //存储A/D转换标志
    ADC_CONTR |= 0x08;      //启动A/D转换（0000 1000 令ADCS = 1）
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    while (AD_FIN ==0){     //等待A/D转换结束
        AD_FIN = (ADC_CONTR & 0x10); //0001 0000测试A/D转换结束否
    }
    ADC_CONTR &= 0xE7;      //1111 0111 清ADC_FLAG位, 关闭A/D转换, 
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
	UART_init();//串口初始程序
	Read_init(0);//ADC初始化
	P1M1 = 0x01; //P1.7~P1.0：0000 0001（高阻）//注意：更改ADC通道时须同时将对应的IO接口修改为高阻输入。
	P1M0 = 0x00; //P1.7~P1.0：0000 0000（强推）
	while(1){
	    ADC_Read ();//调用ADC转换程序
	    UART_T (ADC_RES); //串口小秘书，将ADC读出值高8位送入串口  0000 0000
	    UART_T (ADC_RESL); //串口小秘书，将ADC读出值低2位送入串口  XXXX XX00
	}
}/**********************************************************************************************/
/*************************************************************
* 杜洋工作室 www.DoYoung.net
/*************************************************************/