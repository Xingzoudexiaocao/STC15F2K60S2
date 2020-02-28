#include "serial_port.h"


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
    TH1 = (unsigned char)(Timer1_Reload >> 8);		//定时器高八位赋初值
    TL1 = (unsigned char)Timer1_Reload;			//定时器低八位赋初值
    TR1 = 1;    //打开定时器
    PS = 1;     //高优先级中断
    REN = 1;    //允许接收
    ES  = 1;    //允许中断
    EA = 1;     //允许全局中断
}

//串口1发送一个字节
void serial_one_send_byte(unsigned char dat)
{
    SBUF = dat;   //发送数据
    while(!TI);   //等待发送完毕
    TI=0;         //清除发送标志
}

//串口1发送字符串
void serial_one_send_string(unsigned char *dat)
{
    while(*dat)
        serial_one_send_byte(*dat++);
}

//串口1发送指定指定长度数据
void serial_one_send_length_string(unsigned char *dat,unsigned char length)
{
    unsigned char i=0;
    for(;i < length;i++)
        serial_one_send_byte(dat[i]);
}