#include <STC15F2K60S2.h>
//void delay(unsigned  char z);
void display(unsigned char *xianshi);
unsigned char code tablenum[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff};
unsigned char code tablebit[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
unsigned char  num[]={11,11,11,11,11,11,11,11};
unsigned char n,flag=0;
unsigned long fan=5,zheng=6,low,high,hebin,shijian=0; 
void Timer0Init(void)	;
void Timer1Init(void)	;
void main()
{
	
	Timer1Init();
	Timer0Init();
	EA=1;
	ET0= 1;
	while(1)
	{
		hebin=((zheng+fan)*5);	
		num[0]=hebin/10000;
		num[1]=hebin%10000/1000;
		num[2]=hebin%10000%1000/100;
		num[3]=hebin%10000%1000%100/10;
		num[4]=hebin%10;
		display(num);
	}	
}


void display(unsigned char *xianshi)
{
	P2=((P2&0x1f)|0xe0);
	P0=tablenum[xianshi[n]];
	P2=P2&0x1f;
	P2=((P2&0x1f)|0xc0);
	P0=tablebit[n];
	P2=P2&0x1f;
	n++;
	if(n==8)
		n=0;
}

void Timer0Init(void)		//2毫秒@12.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x40;		//设置定时初值
	TH0 = 0xA2;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
}
void Timer1Init(void)		//5微秒@12.000MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0xC4;		//设置定时初值
	TH1 = 0xFF;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 0;		//定时器1开始计时
}
void time1()interrupt 3
{
	//TL1 = 0xC4;		//设置定时初值
	//TH1 = 0xFF;		//设置定时初值
	//TF1 = 0;		//清除TF1标志
		if(P34==0)
		{
			low++;
			if(flag==1)
			{
				flag=0;
				zheng=high;
				high=0;
			}
		}
	else	if(P34==1)
		{
			high++;
			if(flag==0)
			{
				flag=1;
				fan=low;
				low=0;
			}
		}
}

void time0()interrupt 1
{
	shijian++;
	if(shijian==495)
	{
	TR1 = 1;
	ET1	= 1;
//	P2=((P2&0x1f)|0x80);
//	P0=~P0;
//	P2=P2&0x1f;
	}
	if(shijian==500)
	{
		shijian=0;
		TR1 = 0;
		ET1	= 0;
	}
}