#include <STC15F2K60S2.H>
#include <intrins.h>
#include<stdlib.h>
/*************************�˿ڶ���***************************/
#define LCD_data P0    //���ݶ˿�
sbit LCD_RS=P2^0; //Һ���Ĵ���ѡ������
sbit LCD_RW=P2^1; //Һ����д����
sbit LCD_EN=P2^2; //Һ��ʹ�ܿ��ƶ�



/**************��������**************/
void delay_1ms(unsigned int x);
void lcd_pos(unsigned char X,unsigned char Y);
void write_str(unsigned char *c);
void lcd_init();