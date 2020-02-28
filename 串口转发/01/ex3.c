//**********************************************
//STC15F2K60S2串口通信程序,将串口接受到的数据从串口发出去
//采用11.0592M晶振,波特率9600
//*********************************************

#include <STC15F2K60S2.H>

/*************常量定义****************/

#define MAIN_Fosc                   11059200L       //主时钟,晶振频率不同的直接在这里修改频率
#define serial_one_read_max         32              //定义缓存区的长度
#define serial_one_baud_rate        9600L           //波特率,只需在这里修改波特率
#define Timer1_Reload               (65536UL -(MAIN_Fosc / 4 / serial_one_baud_rate))    //用定时器1做波特率发生器，计算定时器1的重装值    
#define u8 unsigned char

/*************变量定义****************/
u8  serial_one_read_data[serial_one_read_max];  //定义串口接受数据缓存区
u8  serial_one_read_count=0;                    //接受数据的个数

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
    
    serial_one_init();//串口初始化
    while(1)
    {
        if(serial_one_read_count > 0)//如果接受到数据
        {

            serial_one_send_length_string(serial_one_read_data,serial_one_read_count);//发送接收到的数据
            serial_one_read_count = 0;//接收计数清0
			Delay10ms();
        }
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