#include "main.h"
#include "iic.h"
#include "display.h"
#include "key.h"
#include "ds1302.h"
sbit CSB_Tx = P1^0;
sbit CSB_Rx = P1^1;
u16 CSB_count =0; 
u16 CSB_dat =0; 
u16 CSB_dat_old = 0;
int CSB_dat_Warning = 30; 
u8 display_cache [] = {10,10,10,10,10,10,10,10};
u8 key_value = 0xff;
extern char second,minute,hour;
extern char set_second,set_minute,set_hour;
u16 time2_count = 0;

u8 set_flag =0;
bit test_start = 0;

bit CSB_Tx_flag = 0;

bit test_start_state = 0;


void Delay1ms(unsigned int b)		//@11.0592MHz
{
	
	unsigned char i, j;
	unsigned int c;
	for(c = 0;c<b;c++){
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
}


void Delay13us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 33;
	while (--i);
}

void main ( void )
{
	init();
	Timer1Init();
	Timer2Init();
	DS1302_init();
	Wrds1302_init();
	Rdds1302_init();
	CSB_dat_Warning = IIC_read_dat(0xa0 ,0x00); 
	IE2 = 0x04;              
    EA = 1;
	while(1)
	{
		if(test_start&&test_start_state&&(set_flag == 0))
		{
			test_start = 0;
			CSB_count = 0;
			CSB_Tx_40khz();
			TR1 = 1;  //定时器开始计时
			while((CSB_Rx)&&(CSB_count<5));
			
			TR1 = 0;		//定时器1停止计时 
			TF1 = 0;		//清除TF1标志
			if(CSB_Rx == 0)
			{
				CSB_dat = TH1;
				CSB_dat = CSB_dat <<8;
				CSB_dat |= TL1;
				CSB_dat = (CSB_dat + CSB_count*65536)/650;
			}
			else
				CSB_dat = CSB_dat_old;
			TH1 = TL1 = 0;	  //清空计数器	
			CSB_dat_old = CSB_dat;
			Delay1ms(10);
			display_cache[0]=10;
			display_cache[1]=10;
			display_cache[2]=10;
			display_cache[3]=10;
			display_cache[4]=10;
			display_cache [5]= CSB_dat /100;
			display_cache [6]= CSB_dat /10 %10;
			display_cache [7]=  CSB_dat %10;
		}



			if(CSB_dat < (int)CSB_dat_Warning*1.2)		   //报警
			{
			P2=P2&0x1f|0x80;
			P0=0xfe;
			P2=P2&0x1f;			//开LD1	
			Delay1ms(10);
			}
			if(CSB_dat < CSB_dat_Warning)
			{
				
				P2=P2&0x1f|0xa0;	//开蜂鸣器
				P0=0xbf;
				P2=P2&0x1f;
				Delay1ms(1);
				P2=P2&0x1f|0xa0;	//开蜂鸣器
				P0=0xff;
				P2=P2&0x1f;
				Delay1ms(1);
			}
			if(CSB_dat > CSB_dat_Warning)
			{
				P2=P2&0x1f|0xa0;	//开蜂鸣器
				P0=0xff;
				P2=P2&0x1f;
				P2=P2&0x1f|0x80;
				P0=0xff;
				P2=P2&0x1f;
				Delay1ms(20);	
			}
			
	}		
}

void init( void )
{
	unsigned int buzzer = 0;
	P2=P2&0x1f|0x80;
	P0=0xfe;
	P2=P2&0x1f;			//开LD1	

	P2=P2&0x1f;
	P2=P2&0x1f|0xc0;
	P0=0xff;
	P2=P2&0x1f;

	P2=P2|0xe0;
	P0=0x00;
	P2=P2&0x1f;

	for(;buzzer<500;buzzer++)
	{
		Delay1ms(1);
		P2=P2&0x1f|0xa0;	//开蜂鸣器
		P0=0xbf;
		P2=P2&0x1f;
		Delay1ms(1);
		P2=P2&0x1f|0xa0;	//开蜂鸣器
		P0=0xff;
		P2=P2&0x1f;
	}
	P2=P2&0x1f|0x80;
	P0=0xFF;
	P2=P2&0x1f;	
}
void Timer1Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x10;		//设置定时器模式
	TL1 = 0x00;		//设置定时初值
	TH1 = 0x00;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 0;		//定时器1开始计时
	ET1 = 1;
}

void CSB_Tx_40khz(void)
{
	unsigned char for_count;
	for(for_count = 0 ;for_count < 8 ; for_count++)
	{
		CSB_Tx = ~CSB_Tx;
		Delay13us();
	}
}
void Timer2Init( void )		//1毫秒@11.0592MHz
{
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0xcd;		//设置定时初值
	T2H = 0xd4;		//设置定时初值
	AUXR |= 0x10;		//定时器2开始计时
}
void time1() interrupt 3
{
	TF1 = 0;
	TH1 = TL1 = 0;
	CSB_count++;
}
void time2() interrupt 12           //中断入口
{
	display( display_cache );
	key_value = key_state();
	if(key_value == S7)	
	{
		if(set_flag != 0){
		}
		else{
			test_start_state = ~test_start_state;
		}
		Delay1ms(20);
	}
	else if(key_value == S6)	
	{
		if(test_start_state)
		{
			set_flag = ++set_flag%2;
		}
		else
		{
			set_flag = ++set_flag%4;
		}
		Delay1ms(20);
	}	
	else if(key_value == S5)	
	{
		if(test_start_state == 0)
		{
			if(set_flag == 1)
			{
				set_hour++;	 if(set_hour > 23) set_hour = 0;Wrds1302_init();
			}
			else if(set_flag == 2)
			{
				set_minute++; if(set_minute > 59) set_minute = 0; Wrds1302_init();
			}
			else if(set_flag == 3)
			{
				set_second++; if(set_second > 59) set_second = 0;Wrds1302_init();
			}
			
		}
		else
		{
			if(set_flag == 1)
			{
				CSB_dat_Warning++;
				if(CSB_dat_Warning > 200) 
					CSB_dat_Warning = 0; 
				IIC_send_dat(0xa0 ,0x00 ,CSB_dat_Warning); 
				Delay1ms(20);
			}
		}
		Delay1ms(20); 
	}	
	else if(key_value == S4)	
	{
		if(test_start_state == 0)
		{
			if(set_flag == 1)
			{
				set_hour--;	 if(set_hour < 0) set_hour = 23;   Wrds1302_init();
			}
			else if(set_flag == 2)
			{
				set_minute--; if(set_minute < 0) set_minute = 59;	Wrds1302_init();
			}
			else if(set_flag == 3)
			{
				set_second--; if(set_second < 0) set_second = 59;	Wrds1302_init();
			}
		}
		else
		{
			if(set_flag == 1)
			{
				 CSB_dat_Warning--;	
				 if(CSB_dat_Warning < 0) 
				 	CSB_dat_Warning = 200; 
				 IIC_send_dat(0xa0 ,0x00 ,CSB_dat_Warning); 
				 Delay1ms(20);
			}
		}
		Delay1ms(20);
	}
	if(key_value == (Key_down_long | S4))	
	{

	}
	if(time2_count <400)
	{
		time2_count++;	

	}
	else
	{
		time2_count=0;
		test_start = ~test_start;
		
	}
	if((test_start_state == 0)&&(set_flag == 0))
	{
		Rdds1302_init();
		display_cache[0]=hour/10;
		display_cache[1]=hour%10;
		display_cache[2]=13;
		display_cache[3]=minute/10;
		display_cache[4]=minute%10;
		display_cache[5]=13;
		display_cache[6]=second/10;
		display_cache[7]=second%10;
		set_hour	 = hour;
		set_minute	 = minute;
		set_second	 = second;
	}
	if(test_start&&(test_start_state == 0)&&(set_flag != 0))
	{
		display_cache[0]=set_hour/10;
		display_cache[1]=set_hour%10;
		display_cache[2]=13;
		display_cache[3]=set_minute/10;
		display_cache[4]=set_minute%10;
		display_cache[5]=13;
		display_cache[6]=set_second/10;
		display_cache[7]=set_second%10;
	}
	else if((test_start == 0)&&(test_start_state == 0)&&(set_flag == 1))
	{
		display_cache[0]=10;
		display_cache[1]=10;
		display_cache[2]=13;
		display_cache[3]=set_minute/10;
		display_cache[4]=set_minute%10;
		display_cache[5]=13;
		display_cache[6]=set_second/10;
		display_cache[7]=set_second%10;
	}
	else if((test_start == 0)&&(test_start_state == 0)&&(set_flag == 2))
	{
		display_cache[0]=set_hour/10;
		display_cache[1]=set_hour%10;
		display_cache[2]=13;
		display_cache[3]=10;
		display_cache[4]=10;
		display_cache[5]=13;
		display_cache[6]=set_second/10;
		display_cache[7]=set_second%10;
	}
	else if((test_start == 0)&&(test_start_state == 0)&&(set_flag == 3))
	{
		display_cache[0]=set_hour/10;
		display_cache[1]=set_hour%10;
		display_cache[2]=13;
		display_cache[3]=set_minute/10;
		display_cache[4]=set_minute%10;
		display_cache[5]=13;
		display_cache[6]=10;
		display_cache[7]=10;
	}
	if(test_start&&test_start_state&&(set_flag == 1))
	{
		display_cache[0]=10;
		display_cache[1]=10;
		display_cache[2]=10;
		display_cache[3]=10;
		display_cache[4]=10;
		display_cache[5]=10;
		display_cache[6]=10;
		display_cache[7]=10;
	}
	if((test_start == 0)&&test_start_state&&(set_flag == 1))
	{
		display_cache[0]=10;
		display_cache[1]=10;
		display_cache[2]=10;
		display_cache[3]=10;
		display_cache[4]=10;
		display_cache[5]= CSB_dat_Warning /100;
		display_cache[6]= CSB_dat_Warning /10 %10;
		display_cache[7]= CSB_dat_Warning %10;
	}
			
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