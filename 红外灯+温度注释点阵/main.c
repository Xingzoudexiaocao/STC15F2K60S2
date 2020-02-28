#include "main.h"

//--  宋体12;  此字体下对应的点阵为：宽x高=16x16   -- 
unsigned char code  table0[] = {0x00,0x04,0x02,0x04,0x42,0x04,0x22,0x3F,0x14,0x04,0x14,0x04,0x08,0x7F,0x08,0x04,
0x14,0x14,0x14,0x14,0x22,0x17,0x42,0x14,0x00,0x14,0x00,0x2C,0xFE,0x27,0x00,0x40};

unsigned char code  table1[] = {0x00,0x00,0x7E,0x00,0x42,0x3E,0x42,0x22,0x42,0x22,0x7E,0x22,0x42,0x3E,0x42,0x22,
0x42,0x22,0x7E,0x22,0x42,0x3E,0x42,0x22,0x82,0x00,0x82,0x00,0x0A,0x01,0x04,0x02};

unsigned char code dis[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,
						  0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xef,0x39};
unsigned char  display_num[]={0xff,0xff,0xff,0xff};

unsigned int temperature = 0;
unsigned int _16_16_cache = 0;
unsigned char _16_16_flag = 0;
unsigned char display_switch = 0;
unsigned int time0_num=0;
bit time_bit = 0;
void Delay10ms();
void main(void)
{
	Timer0Init();
	EA = 1;
	while(1)
	{
		temperature = rd_temperature();  //读温度      
		temperature = temperature *6.25;

		display_num[0]=temperature/1000;	
		display_num[1]=temperature/100%10;
		display_num[2]=temperature/10%10;
		display_num[3]=temperature%10;

		if(P10==1)
		{
			Delay10ms();
			if(P10==1)
			{
				time0_num = 0;
				time_bit = 0;
				P11 = 0;
			}
		}
		else
		{	
			time_bit = 1;
			if((time0_num>=10000)&&(P10==0))
			{
				time_bit = 0;
				P11=1;	
			}
		}

	}
}

void Timer0Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xCD;		//设置定时初值
	TH0 = 0xD4;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;
}


void time0_int(void) interrupt 1
{
//	if(_16_16_cache ==500)
//	{
//		_16_16_cache = 0;
//
//		_16_16_flag ++;
//
//		if(_16_16_flag ==3)
//			_16_16_flag = 0;
//	}
//	else
//		_16_16_cache++;
//	switch(_16_16_flag)
//	{
//		case 0 : display(table0); break;
//		case 1 : display(table1); break;
//		case 2 : display(table1); break;
//	}

	if(time0_num>=12000)
		time0_num=0;
	else
//		if(time_bit == 1)
			time0_num++;
//		else 
//		   	time0_num = 0;


	switch(display_switch)
	{
		case 0 : 	P00=0;
					P01=0;
					P02=0;
					P2=dis[20];
					break;
		case 1 : 
					P00=0;
					P01=0;
					P02=1;
					P2=dis[display_num[0]];
					break;
		case 2 : 
					P00=1;
					P01=0;
					P02=1;
					P2=dis[display_num[1]+ 9];
					break;
		case 3 : 
					P00=0;
					P01=1;
					P02=1;
					P2=dis[display_num[2]];
					break;
		case 4 :	P00=1;
					P01=1;
					P02=1;
					P2=dis[display_num[3]];
					break;
	}
	if(display_switch > 4)
		display_switch = 0;
	else
		display_switch++;		
}
void Delay10ms()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	_nop_();
	i = 11;
	j = 190;
	do
	{
		while (--j);
	} while (--i);
}