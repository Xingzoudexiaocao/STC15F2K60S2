#ifndef _DS1302_H_
#define _DS1302_H_

#include "stc15f2k60s2.h"
#define u8 unsigned char 
#define u16 unsigned int

/*********************端口定义***************************/
sbit SCLK=P1^7;
sbit DIO=P2^3;
sbit RST=P1^3;
/**********************全局变量定义**********************/
extern char second,minute,hour,week,date,mouth,year;
extern char set_second,set_minute,set_hour;
/*******************通用宏定义***************************

此通用宏定义 给定的时间初值 是十进制，因为DS1302存放的数据 
是以BCD码形式存放的，因为写数据写入DS1302时要以BCD码存放， 
所以必须进行转换，将十进制转换为BCD码。反之，读取DS1302时， 
1602接收的是十进制数据，所以要将BCD码转换为十进制写入1602 

*********************************************************/
#define D2BCD(X)   ((X/10<<4)|(X%10)) //十进制转化为BCD码
#define BCD2D(X)   (((X&0x70)>>4)*10+(X&0x0f))//BCD码转换为十进制

/*******************DS1302命令***************************/
//写命令
#define W_second  0x80	  //写秒
#define W_minute  0x82	  //写分
#define W_hour     0x84	  //写小时
#define W_date     0x86	  //写日
#define W_mouth    0x88	  //写月
#define W_week     0x8A	  //写星期
#define W_year     0x8C	  //写年
#define W_control  0x8E	  //写保护

//读命令
#define R_second  0x81	  //写秒
#define R_minute  0x83	  //写分
#define R_hour     0x85	  //写小时
#define R_date     0x87	  //写日
#define R_mouth    0x89	  //写月
#define R_week     0x8B	  //写星期
#define R_year     0x8D	  //写年					  	
#define R_control  0x8E	  //写保护

/********************函数定义*****************************/
extern void delay(u8 i);    //延时
extern void Wr_1302(u8 dat);//向指定地址写入一个字节数据函数
extern u8 Rd_1302(void);    //从指定地址读取一个字节数据函数
extern void WrDat_1302(u8 dat,u8 cmd);//写字节函数
extern u8 RdDat_1302(u8 dat);//读字节函数
extern void Wrds1302_init();//写时间
extern void Rdds1302_init();//读取RTC数据
extern void DS1302_init(void);//初始化时间

extern void Delay200us();		//@11.0592MHz


#endif