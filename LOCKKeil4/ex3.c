
#include "stc15f2k60s2.h"
#include <intrins.h>
/*************常量定义****************/

#define MAIN_Fosc                   11059200L       //主时钟,晶振频率不同的直接在这里修改频率
#define serial_one_read_max         4              //定义缓存区的长度
#define serial_one_baud_rate        9600L           //波特率,只需在这里修改波特率
#define Timer1_Reload               (65536UL -(MAIN_Fosc / 4 / serial_one_baud_rate))    //用定时器1做波特率发生器，计算定时器1的重装值    
#define u8 unsigned char



/*************变量定义****************/
u8  serial_one_read_data[serial_one_read_max]={0,0,0,0};  //定义串口接受数据缓存区
u8  serial_one_read_count=0;                    //接受数据的个数


void serial_one_init();
void serial_one_send_byte(u8 dat);
void serial_one_send_string(u8 *dat);
void serial_one_send_length_string(u8 *dat,u8 length);

void Delay100ms();
void Delay2000ms();

void Delay2000ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 85;
	j = 12;
	k = 155;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay100ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 5;
	j = 52;
	k = 195;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
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

void Timer0Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xCD;		//设置定时初值
	TH0 = 0xD4;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
}


void main()
{
	char i=0;
    serial_one_init();//串口初始化
	Timer0Init();
	P10=1;
    while(1)
    {
	//	serial_one_send_string(serial_one_read_data);
      if((serial_one_read_data[0]=='Z'))
	  {	

	  		P10=0;
			ET0 = 1;
			Delay100ms();
			ET0 = 0;
			Delay2000ms();

			P10=1;
			ET0 = 1;
			Delay100ms();
			ET0 = 0;

			for(i=0;i<10;i++)
			serial_one_read_data[i]=0;
	  }
	  if(serial_one_read_data[3]!=0)
	  		for(i=0;i<4;i++)
					serial_one_read_data[i]=0;
	  }

}

/********************* 串口1中断函数************************/
void UART1_int (void) interrupt 4
{
    if(RI)
    {
        RI = 0;//清除接受标志
        serial_one_read_data[serial_one_read_count] = SBUF;//将数据存入数组

        if(++serial_one_read_count > serial_one_read_max)    //避免溢出
            serial_one_read_count = 0;

    }
}
void tm0_isr() interrupt 1
{
    P11 = ! P11;                    //将测试口取反
}