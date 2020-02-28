/************************************************************
功能描述：单片机12864液晶显示 
************************************************************/
#include"stc15fxxxx.h"
#include<stdlib.h>
unsigned char code dis[]={0x3f,0x06,0x5b,0x4f,0x6d,0x7d,0x07,0x7f,0x6f};

#define MAIN_Fosc                   11059200L       //主时钟,晶振频率不同的直接在这里修改频率
#define serial_one_read_max         40              //定义缓存区的长度
#define serial_one_baud_rate        9600L           //波特率,只需在这里修改波特率
#define Timer1_Reload               (65536UL -(MAIN_Fosc / 4 / serial_one_baud_rate))    //用定时器1做波特率发生器，计算定时器1的重装值    
#define u8 unsigned char

/*************变量定义****************/
u8  serial_one_read_data[serial_one_read_max];  //定义串口接受数据缓存区
u8  serial_one_read_count=0;                    //接受数据的个数

unsigned char LCD_cache[9];
bit  delay_1ms(u16 x);
void delay(void);
bit lcd_busy(void);
bit  lcd_pos(u8 X,u8 Y);
bit lcd_init();
bit write_str(u8 *c);
unsigned char RX_dat_flag=0;
unsigned char time0_num=0;
/************************************************************
功能描述：在LCD12864上指定位置显示指定内容
入口参数：无
出口参数：无
************************************************************/
void Timer0Init(void)		//50毫秒@11.0592MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x00;		//设置定时初值
	TH0 = 0x4C;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;
}

//初始化串口1
void serial_one_init()
{
    SCON |= 0x40;          //8位数据
    P_SW1 &=  ~0xc0;       //UART1 使用P30 P31口  默认
    TR1 = 0;               //关闭定时器
    AUXR &= ~0x01;      //S1 BRT Use Timer1;
    TMOD &= ~(1<<6);    //Timer1 set As Timer
    TMOD &= ~0x30;      //Timer1_16bitAutoReload;2
    AUXR |=  (1<<6);    //Timer1 set as 1T mode
    TH1 = (u8)(Timer1_Reload >> 8);		//定时器高八位赋初值
    TL1 = (u8)Timer1_Reload;			//定时器低八位赋初值
    TR1 = 1;    //打开定时器
    PS = 1;     //高优先级中断
    REN = 1;    //允许接收
    ES  = 1;    //允许中断
    EA = 1;     //允许全局中断
}

//串口1发送一个字节
void serial_one_send_byte(u8 dat)
{
    SBUF = dat;   //发送数据
    while(!TI);   //等待发送完毕
    TI=0;         //清除发送标志
}

//串口1发送字符串
void serial_one_send_string(u8 *dat)
{
    while(*dat)
        serial_one_send_byte(*dat++);
}

//串口1发送指定指定长度数据
void serial_one_send_length_string(u8 *dat,u8 length)
{
    u8 i=0;
    for(;i < length;i++)
        serial_one_send_byte(dat[i]);
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
void main()
{
    lcd_init();       			//LCD初始化
	delay_1ms(3);	  			//延时
	lcd_pos(0,1);				//指定显示位置
	write_str("GPS");	//显示字符串
	lcd_pos(0,3);				//指定显示位置
	write_str("北京时间");	//显示字符串
	lcd_pos(1,2);				//指定显示位置
	write_str("自动授时");	//显示字符串
	lcd_pos(3,1);				//指定显示位置
	write_str("——牧童工作室");

	P0&=0xaf;
	P2=P2&0x1f|0xa0;
    serial_one_init();//串口初始化
	Timer0Init();
    while(1)
    {
		if(time0_num == 0)
		{
			P10=0;
			LCD_cache[0]=serial_one_read_data[0];
			LCD_cache[1]=serial_one_read_data[1];
			LCD_cache[2]=':';
			LCD_cache[3]=serial_one_read_data[2];
			LCD_cache[4]=serial_one_read_data[3];
			LCD_cache[5]=':';
			LCD_cache[6]=serial_one_read_data[4];
			LCD_cache[7]=serial_one_read_data[5];
			LCD_cache[8]='\0';

			LCD_cache[0]=(LCD_cache[0]-48)*10+LCD_cache[1]-48;	  //小时十位
			LCD_cache[0]=LCD_cache[0]+8;

			if(LCD_cache[0]==24)
				 LCD_cache[0]=0;
			else if(LCD_cache[0]==25)
				 LCD_cache[0]=1;
			 else if(LCD_cache[0]==26)
				 LCD_cache[0]=2;
			 else if(LCD_cache[0]==27)
				 LCD_cache[0]=3;
			 else if(LCD_cache[0]==28)
				 LCD_cache[0]=4;
			 else if(LCD_cache[0]==29)
				 LCD_cache[0]=5;
			 else if(LCD_cache[0]==30)
				 LCD_cache[0]=6;
			 else if(LCD_cache[0]==31)
				 LCD_cache[0]=7;
			 else if(LCD_cache[0]==32)
				 LCD_cache[0]=8;

			LCD_cache[1]=LCD_cache[0]%10+48;
			LCD_cache[0]=LCD_cache[0]/10+48;	
		}
		if(time0_num==5)
		{
			P10=1;
			lcd_pos(2,2);				//指定显示位置
			write_str(LCD_cache);	//显示字符串
		}
        if(RX_dat_flag == 0)//如果接受到数据
        {
            serial_one_send_length_string(serial_one_read_data,serial_one_read_count);//发送接收到的数据
            serial_one_read_count = 0;//接收计数清0
			Delay10ms();
        }
    }
}
unsigned char num=0;
/********************* 串口1中断函数************************/
void UART1_int (void) interrupt 4
{
	unsigned char cache=0x00;
    if(RI)
    {
        RI = 0;//清除接受标志
		cache = SBUF;
		if(cache=='L')
		{
			RX_dat_flag=1;
		}
		if((RX_dat_flag==1)&&(cache==','))
		{
		 	RX_dat_flag=2;
			cache=0;
		}
		if((RX_dat_flag==2)&&(cache==','))
		{
		 	RX_dat_flag=3;
			cache=0;
		}
		if((RX_dat_flag==3)&&(cache==','))
		{
		 	RX_dat_flag=4;
			cache=0;
		}
		if((RX_dat_flag==4)&&(cache==','))
		{
		 	RX_dat_flag=5;
			cache=0;
		}
		if((RX_dat_flag==5)&&(cache==','))
		{
		 	RX_dat_flag=6;
		}

		if(RX_dat_flag==7)	
		{
			num++;
			serial_one_read_data[serial_one_read_count]=cache;
			serial_one_read_count++;
		}
		if(RX_dat_flag==6)
			RX_dat_flag=7;


		if(num>5)
		{
			num=0;
			RX_dat_flag=0;
		}
    }
}
void TIME0 (void) interrupt 1
{
	if(time0_num>18)
		time0_num=0;
	else
		time0_num++;
}
