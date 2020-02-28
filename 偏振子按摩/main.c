#include"stc15fxxxx.h"
#include "intrins.h" 


void Timer0Init(void)		//100微秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xAE;		//设置定时初值
	TH0 = 0xFB;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;
	EA = 1;     //允许全局中断
}
void Delay10ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 108;
	j = 145;
	do
	{
		while (--j);
	} while (--i);
}

unsigned char key_scan(void)
{
	unsigned char key_value=0xff,left_num=0xfe,for_num=0;
	for(for_num=0;for_num<4;for_num++)
	{
		P3=left_num;
		if((P3!=left_num)||P42==0||P44==0)
		{
			key_value=(P3&0x3f)|((P4&0x04)<<4)|((P4&0x10)<<3);
			break;
		}
		left_num=_crol_(left_num,1);
	}
	return key_value;
}
unsigned char time0_num;
unsigned int Open_Close=0;
unsigned int Open_Close_Hz=0;
bit Open_Close_flag=1;
unsigned char num=0;
void main()
{
	
	bit key_flag=0;
	Timer0Init();
	
	while(1)
	{
		if((key_scan()!=0xff)&&(key_flag==0))
		{
			key_flag=1;
			Delay10ms();
			Delay10ms();
			Delay10ms();
			Delay10ms();
			Delay10ms();
		   	if(key_scan()==0x7e)
		   		num++;
			if(key_scan()==0x7d)
				Open_Close_Hz+=1000;
			if(key_scan()==0x7b)
			{
				P11=0;
				EA=~EA;
			}

		}
		if(key_scan()==0xff)
			key_flag=0;

		if(Open_Close_Hz>5000)
			Open_Close_Hz=0;
		if(num>10)
			num=0;

	    
    }
}

void TIME0 (void) interrupt 1
{
	if(time0_num>=10)
	{
		time0_num=0;
		if(Open_Close_flag)
			P11=1;
	}
	else
		time0_num++;
	if(time0_num>=num)
	   	{
	   		P11=0;
	   	}
	if(Open_Close_Hz!=0)
	{
		if(Open_Close>Open_Close_Hz)
		{
			Open_Close_flag=~Open_Close_flag;
			Open_Close=0;
		}
		else
			Open_Close++;
	}
	else
		Open_Close_flag=1;
}
