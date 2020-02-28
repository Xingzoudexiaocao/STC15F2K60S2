#include <STC15F2K60S2.H>
#include <intrins.h>
#include<stdlib.h>
/*************************端口定义***************************/
#define LCD_data P0    //数据端口
sbit LCD_RS=P2^0; //液晶寄存器选择输入
sbit LCD_RW=P2^1; //液晶读写控制
sbit LCD_EN=P2^2; //液晶使能控制端



/**************函数声明**************/
void delay_1ms(unsigned int x);
void lcd_pos(unsigned char X,unsigned char Y);
void write_str(unsigned char *c);
void lcd_init();