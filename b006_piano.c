/*********************************************************************************************
程序名： 　　 8键电子琴C程序
编写人： 　　 杜洋　
编写时间：　　2009年5月18日
硬件支持：　　STC系列单片机 12MHz
接口说明：　　  
修改日志：　　
　　NO.1-								
/*********************************************************************************************
说明：

/*********************************************************************************************/

#include <AT89X51.H> 

sbit SPEAKER  = P3^7;
#define KEY  P1 
unsigned char MUSIC; 
unsigned char STH0;
unsigned char STL0;
unsigned int code tab[]={
64021,64103,64260,64400,//低音3开始 
64524,64580,64684,64777, 
64820,64898,64968,65030, 
65058,65110,65157,65178
}; 

void main(void){ 
	TMOD=0x01; 
	ET0=1; 
	EA=1; 
	KEY = 0xff;
	while(1){ 	
		if(KEY != 0xff){ 
		switch (~KEY){//显示的列位置		
				case 0x01://			
					MUSIC = 7;	
					break;//		
				case 0x02://			
					MUSIC = 6;	
					break;//		
				case 0x04://			
					MUSIC = 5;	
					break;//		
				case 0x08://			
					MUSIC = 4;	
					break;//		
				case 0x10://			
					MUSIC = 3;	
					break;//		
				case 0x20://			
					MUSIC = 2;	
					break;//		
				case 0x40://			
					MUSIC = 1;	
					break;//		
				case 0x80://			
					MUSIC = 0;	
					break;//		
		}	

    		STH0=tab[MUSIC]/256; 
    		STL0=tab[MUSIC]%256; 
    		TR0=1;
		}else{ 
    		SPEAKER = 1;
			TR0=0;
		} 
    } 
} 
void t0(void) interrupt 1 using 0{ 
  TH0=STH0; 
  TL0=STL0; 
  SPEAKER=~SPEAKER; 
}
/*************************************************************
* 杜洋工作室 DoYoung Studio
* 与电子爱好者同行 www.DoYoung.net
/*************************************************************/