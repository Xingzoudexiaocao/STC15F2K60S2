#include <STC15F2K60S2.H>
#include "iic.h"
#include "display.h"
#include "key.h"
#include "ds18b20.h"
unsigned char display_cache [] = {10,10,10,10,10,10,10,10};
unsigned char key_value = 0xff;
unsigned char data_set[2];
unsigned int temp;
unsigned int count_save_temp;
unsigned char save_temp;
char set_temp[2];
unsigned char Old_Temp_dat_[10];

unsigned char key_State_flag = 0;
unsigned char Set_State_flag = 0;
bit dis_flag = 0;
bit flag_key = 1;
void Timer1Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x10;		//设置定时器模式
	TL1 = 0xCD;			//设置定时初值
	TH1 = 0xD4;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	ET1 = 1;
}
void Delay5ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 54;
	j = 199;
	do
	{
		while (--j);
	} while (--i);
}
void Delay100us()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 2;
	j = 15;
	do
	{
		while (--j);
	} while (--i);
}
void main()
{
	Timer1Init();
	Old_Temp_dat_[0] = IIC_read_dat(0xa0 ,0x00);
	Delay5ms();
	Old_Temp_dat_[1] = IIC_read_dat(0xa0 ,0x01);
	Delay5ms();
	Old_Temp_dat_[2] = IIC_read_dat(0xa0 ,0x02);
	Delay5ms();
	Old_Temp_dat_[3] = IIC_read_dat(0xa0 ,0x03);
	Delay5ms();
	Old_Temp_dat_[4] = IIC_read_dat(0xa0 ,0x04);
	Delay5ms();
	Old_Temp_dat_[5] = IIC_read_dat(0xa0 ,0x05);
	Delay5ms();
	Old_Temp_dat_[6] = IIC_read_dat(0xa0 ,0x06);
	Delay5ms();
	Old_Temp_dat_[7] = IIC_read_dat(0xa0 ,0x07);
	Delay5ms();
	Old_Temp_dat_[8] = IIC_read_dat(0xa0 ,0x08);
	Delay5ms();
	Old_Temp_dat_[9] = IIC_read_dat(0xa0 ,0x09);
	Delay5ms();
	set_temp[0] = IIC_read_dat(0xa0 ,0x0a);
	Delay5ms();
	set_temp[1] = IIC_read_dat(0xa0 ,0x0b);
	Delay5ms();

	EA = 1;
	while(1)
	{
		if((key_State_flag == 0)&&(Set_State_flag == 0))
		{
			display_cache[0]=10;
			display_cache[1]=10;
			display_cache[2]=10;
			display_cache[3]=10;
			display_cache[4]= temp /1000;
			display_cache[5]= temp /100 %10;
			display_cache[6]= temp /10 %10;
			display_cache[7]= temp %10;
		}
		else if((key_State_flag == 1)&&(Set_State_flag == 0))
		{
			display_cache[0]=11;
			display_cache[1]=1;
			display_cache[2]=11;
			display_cache[3]=10;
			display_cache[4]=10;
			display_cache[5]=10;
			display_cache[6]= Old_Temp_dat_[0] /10 %10;
			display_cache[7]= Old_Temp_dat_[0] %10;
		}
		else if((key_State_flag == 2)&&(Set_State_flag == 0))
		{
			display_cache[0]=11;
			display_cache[1]=2;
			display_cache[2]=11;
			display_cache[3]=10;
			display_cache[4]=10;
			display_cache[5]=10;
			display_cache[6]= Old_Temp_dat_[1] /10 %10;
			display_cache[7]= Old_Temp_dat_[1] %10;
		}
		else if((key_State_flag == 3)&&(Set_State_flag == 0))
		{
			display_cache[0]=11;
			display_cache[1]=3;
			display_cache[2]=11;
			display_cache[3]=10;
			display_cache[4]=10;
			display_cache[5]=10;
			display_cache[6]= Old_Temp_dat_[2] /10 %10;
			display_cache[7]= Old_Temp_dat_[2] %10;
		}
		else if((key_State_flag == 4)&&(Set_State_flag == 0))
		{
			display_cache[0]=11;
			display_cache[1]=4;
			display_cache[2]=11;
			display_cache[3]=10;
			display_cache[4]=10;
			display_cache[5]=10;
			display_cache[6]= Old_Temp_dat_[3] /10 %10;
			display_cache[7]= Old_Temp_dat_[3] %10;
		}
		else if((key_State_flag == 5)&&(Set_State_flag == 0))
		{
			display_cache[0]=11;
			display_cache[1]=5;
			display_cache[2]=11;
			display_cache[3]=10;
			display_cache[4]=10;
			display_cache[5]=10;
			display_cache[6]= Old_Temp_dat_[4] /10 %10;
			display_cache[7]= Old_Temp_dat_[4] %10;
		}
		else if((key_State_flag == 6)&&(Set_State_flag == 0))
		{
			display_cache[0]=11;
			display_cache[1]=6;
			display_cache[2]=11;
			display_cache[3]=10;
			display_cache[4]=10;
			display_cache[5]=10;
			display_cache[6]= Old_Temp_dat_[5] /10 %10;
			display_cache[7]= Old_Temp_dat_[5] %10;
		}
		else if((key_State_flag == 7)&&(Set_State_flag == 0))
		{
			display_cache[0]=11;
			display_cache[1]=7;
			display_cache[2]=11;
			display_cache[3]=10;
			display_cache[4]=10;
			display_cache[5]=10;
			display_cache[6]= Old_Temp_dat_[6] /10 %10;
			display_cache[7]= Old_Temp_dat_[6] %10;
		}
		else if((key_State_flag == 8)&&(Set_State_flag == 0))
		{
			display_cache[0]=11;
			display_cache[1]=8;
			display_cache[2]=11;
			display_cache[3]=10;
			display_cache[4]=10;
			display_cache[5]=10;
			display_cache[6]= Old_Temp_dat_[7] /10 %10;
			display_cache[7]= Old_Temp_dat_[7] %10;
		}
		else if((key_State_flag == 9)&&(Set_State_flag == 0))
		{
			display_cache[0]=11;
			display_cache[1]=9;
			display_cache[2]=11;
			display_cache[3]=10;
			display_cache[4]=10;
			display_cache[5]=10;
			display_cache[6]= Old_Temp_dat_[8] /10 %10;
			display_cache[7]= Old_Temp_dat_[8] %10;
		}
		else if((key_State_flag == 10)&&(Set_State_flag == 0))
		{
			display_cache[0]=11;
			display_cache[1]=1;
			display_cache[2]=0;
			display_cache[3]=11;
			display_cache[4]=10;
			display_cache[5]=10;
			display_cache[6]= Old_Temp_dat_[9] /10 %10;
			display_cache[7]= Old_Temp_dat_[9] %10;
		}
		else if((key_State_flag == 0)&&(Set_State_flag == 1)&&(dis_flag == 0))
		{
			display_cache[0]=set_temp[0] /10 %10;
			display_cache[1]=set_temp[0] %10;
			display_cache[2]=10;
			display_cache[3]=10;
			display_cache[4]=10;
			display_cache[5]=10;
			display_cache[6]= set_temp[1] /10 %10;
			display_cache[7]= set_temp[1] %10;
		}
		else if((key_State_flag == 0)&&(Set_State_flag == 1)&&dis_flag)
		{
			display_cache[0]=10;
			display_cache[1]=10;
			display_cache[2]=10;
			display_cache[3]=10;
			display_cache[4]=10;
			display_cache[5]=10;
			display_cache[6]= set_temp[1] /10 %10;
			display_cache[7]= set_temp[1] %10;
		}
		else if((key_State_flag == 0)&&(Set_State_flag == 2)&&(dis_flag == 0))
		{
			display_cache[0]=set_temp[0] /10 %10;
			display_cache[1]=set_temp[0] %10;
			display_cache[2]=10;
			display_cache[3]=10;
			display_cache[4]=10;
			display_cache[5]=10;
			display_cache[6]=10;
			display_cache[7]=10;
		}
		else if((key_State_flag == 0)&&(Set_State_flag == 2)&&dis_flag)
		{
			display_cache[0]=set_temp[0] /10 %10;
			display_cache[1]=set_temp[0] %10;
			display_cache[2]=10;
			display_cache[3]=10;
			display_cache[4]=10;
			display_cache[5]=10;
			display_cache[6]= set_temp[1] /10 %10;
			display_cache[7]= set_temp[1] %10;
		}
		if(set_temp[0] < save_temp)
		{
			
			P2=P2&0x1f|0xa0;	//开蜂鸣器
			P0=0xbf;
			P2=P2&0x1f;
			Delay100us();
			P2=P2&0x1f|0xa0;	//开蜂鸣器
			P0=0xff;
			P2=P2&0x1f;
			Delay100us();
		}
		else if(set_temp[1] > save_temp)
		{
			
			P2=P2&0x1f|0xa0;	//开蜂鸣器
			P0=0xbf;
			P2=P2&0x1f;
			Delay100us();
			P2=P2&0x1f|0xa0;	//开蜂鸣器
			P0=0xff;
			P2=P2&0x1f;
			Delay100us();
		}
		else
		{
			P2=P2&0x1f|0xa0;	//开蜂鸣器
			P0=0xff;
			P2=P2&0x1f;
		}
	}
}


void time1() interrupt 3
{
	TL1 = 0xCD;		//设置定时初值
	TH1 = 0xD4;		//设置定时初值
	display( display_cache );
	key_value = key();
	if((key_value != 0xff)&&flag_key)
	{
		flag_key = 0;
		Delay100us();
		if(key_value == S4)
		{
			key_State_flag = ++key_State_flag % 11;
			Set_State_flag = 0;
		}
		else if(key_value == S5)	
		{
			key_State_flag = 0;
			Set_State_flag = ++Set_State_flag % 3;
			
		}	
		else if(key_value == S6)	
		{
			if(Set_State_flag == 1)
			{
				  set_temp[0]++;  if( set_temp[0] >99)  set_temp[0] =0;
				  IIC_send_dat(0xa0 ,0x0a ,set_temp[0]);
			}
			else if(Set_State_flag == 2)
			{
				  set_temp[1]++;  if( set_temp[1] >99)  set_temp[1] =0;
				  IIC_send_dat(0xa0 ,0x0b ,set_temp[1]);
			}
	
			 
		}	
		else if(key_value == S7)	
		{
			if(Set_State_flag == 1)
			{
				  set_temp[0]--;   
				  if( set_temp[0] <0)  
				  		set_temp[0] =99;   
				  IIC_send_dat(0xa0 ,0x0a ,set_temp[0]);
			}
			else if(Set_State_flag == 2)
			{
				  set_temp[1]--;   
				  if( set_temp[1] <0)  
				  		set_temp[1] =99;
			      IIC_send_dat(0xa0 ,0x0b ,set_temp[1]);
			}
	
		}
	}
	else if(key_value == 0xff)
	{
		flag_key = 1;
	}
	if((count_save_temp %500) == 0)
	{
		temp = rd_temperature()*6.25;
		dis_flag = ~dis_flag;
	}

	if(count_save_temp<2000)
	{
		count_save_temp++;

	}else
	{
		count_save_temp = 0;
		
		save_temp = temp /1000 *10 + temp /100 %10;;
		Old_Temp_dat_[9] = Old_Temp_dat_[8];
		IIC_send_dat(0xa0 ,0x09 ,Old_Temp_dat_[9]);
		Delay5ms();
		Old_Temp_dat_[8] = Old_Temp_dat_[7];
		IIC_send_dat(0xa0 ,0x08 ,Old_Temp_dat_[8]);
		Delay5ms();
		Old_Temp_dat_[7] = Old_Temp_dat_[6];
		IIC_send_dat(0xa0 ,0x07 ,Old_Temp_dat_[7]);
		Delay5ms();
		Old_Temp_dat_[6] = Old_Temp_dat_[5];
		IIC_send_dat(0xa0 ,0x06 ,Old_Temp_dat_[6]);
		Delay5ms();
		Old_Temp_dat_[5] = Old_Temp_dat_[4];
		IIC_send_dat(0xa0 ,0x05 ,Old_Temp_dat_[5]);
		Delay5ms();
		Old_Temp_dat_[4] = Old_Temp_dat_[3];
		IIC_send_dat(0xa0 ,0x04 ,Old_Temp_dat_[4]);
		Delay5ms();
		Old_Temp_dat_[3] = Old_Temp_dat_[2];
		IIC_send_dat(0xa0 ,0x03 ,Old_Temp_dat_[3]);
		Delay5ms();
		Old_Temp_dat_[2] = Old_Temp_dat_[1]; 
		IIC_send_dat(0xa0 ,0x02 ,Old_Temp_dat_[2]);
		Delay5ms();
		Old_Temp_dat_[1] = Old_Temp_dat_[0];
		IIC_send_dat(0xa0 ,0x01 ,Old_Temp_dat_[1]);
		Delay5ms();
		Old_Temp_dat_[0] = save_temp;
		IIC_send_dat(0xa0 ,0x00 ,Old_Temp_dat_[0]);
		Delay5ms();
	}

}