#include <STC15F2K60S2.H>
#include <intrins.H>
#include "key.h"

unsigned int time0_num=0;
unsigned int Key_Value=0xff00;
void Timer0Init(void);
void Delay10ms();
void main(void)
{
	Timer0Init();
	while(1)
	{
		Key_Value = Key_Scan();
		if(Key_Value != 0xff00)
		{
			if((Key_Value&0x0001)==1)
			{
			}
			else
			{
			}
			if((Key_Value&0x0002)==1)
			{
			}
			else
			{
			}
		}
		if(P10==1)
		{
			Delay10ms();
			if(P10==1)
			{
			time0_num=0;
			TR0 = 0;
			P11 = 0;
			}
		}
		else
		{	TR0 = 1;
			if((time0_num>=500)&&(P10==0))
			{
				TR0 = 0;
				P11=1;	
			}
		}		
	}
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
void Timer0Init(void)		//50����@11.0592MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0x4C;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1;
	EA = 1; 
}
void time0() interrupt 1
{
	if(time0_num>=4000)
		time0_num=0;
	else
		time0_num++;
}