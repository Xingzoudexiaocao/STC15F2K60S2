#ifndef _DS1302_H_
#define _DS1302_H_

#include "stc15f2k60s2.h"
#define u8 unsigned char 
#define u16 unsigned int

/*********************�˿ڶ���***************************/
sbit SCLK=P1^7;
sbit DIO=P2^3;
sbit RST=P1^3;
/**********************ȫ�ֱ�������**********************/
extern char second,minute,hour,week,date,mouth,year;
extern char set_second,set_minute,set_hour;
/*******************ͨ�ú궨��***************************

��ͨ�ú궨�� ������ʱ���ֵ ��ʮ���ƣ���ΪDS1302��ŵ����� 
����BCD����ʽ��ŵģ���Ϊд����д��DS1302ʱҪ��BCD���ţ� 
���Ա������ת������ʮ����ת��ΪBCD�롣��֮����ȡDS1302ʱ�� 
1602���յ���ʮ�������ݣ�����Ҫ��BCD��ת��Ϊʮ����д��1602 

*********************************************************/
#define D2BCD(X)   ((X/10<<4)|(X%10)) //ʮ����ת��ΪBCD��
#define BCD2D(X)   (((X&0x70)>>4)*10+(X&0x0f))//BCD��ת��Ϊʮ����

/*******************DS1302����***************************/
//д����
#define W_second  0x80	  //д��
#define W_minute  0x82	  //д��
#define W_hour     0x84	  //дСʱ
#define W_date     0x86	  //д��
#define W_mouth    0x88	  //д��
#define W_week     0x8A	  //д����
#define W_year     0x8C	  //д��
#define W_control  0x8E	  //д����

//������
#define R_second  0x81	  //д��
#define R_minute  0x83	  //д��
#define R_hour     0x85	  //дСʱ
#define R_date     0x87	  //д��
#define R_mouth    0x89	  //д��
#define R_week     0x8B	  //д����
#define R_year     0x8D	  //д��					  	
#define R_control  0x8E	  //д����

/********************��������*****************************/
extern void delay(u8 i);    //��ʱ
extern void Wr_1302(u8 dat);//��ָ����ַд��һ���ֽ����ݺ���
extern u8 Rd_1302(void);    //��ָ����ַ��ȡһ���ֽ����ݺ���
extern void WrDat_1302(u8 dat,u8 cmd);//д�ֽں���
extern u8 RdDat_1302(u8 dat);//���ֽں���
extern void Wrds1302_init();//дʱ��
extern void Rdds1302_init();//��ȡRTC����
extern void DS1302_init(void);//��ʼ��ʱ��

extern void Delay200us();		//@11.0592MHz


#endif