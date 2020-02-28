#include"stc15fxxxx.h"
#include "DS1302.h"

u8 second,minute,hour,week,date,mouth,year;

void DS1302_init(void)//��ʼ��ʱ��
{
	WrDat_1302(W_control,0x00);//��ֹд����
	second=50;
	minute=36; 
	hour=19;  
	week=7;   
	date=31;  
	mouth=12; 
	year=12;
}
void delay(u8 i)//��ʱ
{
	u8 n;
	while(i--)
		for(n=0;n<200;n++);
}
void Wr_1302(u8 dat)//��ָ����ַд��һ���ֽ����ݺ���
{
	u8 i;
	for(i=0;i<8;i++)
	{
		DIO=dat&0x01;
		SCLK=0;
		SCLK=1;//DS_1302����������д				
		dat>>=1;//��λ���ݻ���������һλ
	}
}
u8 Rd_1302(void)//��ָ����ַ��ȡһ���ֽ����ݺ���
{
	u8 i,p;
	DIO=1;
	for(i=8;i>0;i--)
	{
		SCLK=1;
		SCLK=0;//DS_1302���½����Ƕ�
		p>>=1;//�����µ�1λ����ǰ������һλ
		if(DIO==1)		   	
		p|=0x80;//p���λ�������ݣ�����λ���ֲ���
	}
	return p;
}

void WrDat_1302(u8 dat,u8 cmd)//д�ֽں���
{
	SCLK=0;//��RST�ɵͱ��֮ǰ��SCLK���뱣�ֵ͵�ƽ
	RST=0;
	delay(1);
	RST=1;//RST�ߵ�ƽ�����/д
	Wr_1302(dat);//д�����ֽ�
	Wr_1302(cmd);//д�����ֽ�
	RST=0;//������ϣ���ֹRST
}
u8 RdDat_1302(u8 dat)//���ֽں���
{
	u8 p;
	SCLK=0;//��RST�ɵͱ��֮ǰ��SCLK���뱣�ֵ͵�ƽ
	RST=0;//RST�ߵ�ƽ�����/д
	delay(1);
	RST=1;
	Wr_1302(dat);//д�����ֽ�
	p=Rd_1302();//�������ֽڣ��ݴ���a��
	RST=0;//������ϣ���ֹRST
	return p;//����1�ֽڵ�RTC��ʵʱʱ�ӣ�����
}
void Wrds1302_init(void)//дʱ��
{								   
	second=D2BCD(second);//ʮ����ת��ΪBCD��
	minute=D2BCD(minute);
	hour=D2BCD(hour);
	date=D2BCD(date);
	mouth=D2BCD(mouth);
	week=D2BCD(week);
	year=D2BCD(year);

	WrDat_1302(W_control,0x00);//��ֹд����
	WrDat_1302(W_second,second);
	WrDat_1302(W_minute,minute);
	WrDat_1302(W_hour,hour);
	WrDat_1302(W_date,date);
	WrDat_1302(W_mouth,mouth);
	WrDat_1302(W_week,week);
	WrDat_1302(W_year,year);
	WrDat_1302(W_control,0x80);//����д����
								 
}
void Rdds1302_init(void)//��ȡRTC����
{					
	second=RdDat_1302(R_second)&0x7f;
	second=BCD2D(second);

	minute=RdDat_1302(R_minute);
	minute=BCD2D(minute);

	hour=RdDat_1302(R_hour);
	hour=BCD2D(hour);

	date=RdDat_1302(R_date);
	date=BCD2D(date);

	mouth=RdDat_1302(R_mouth);
	mouth=BCD2D(mouth);
	
	week=RdDat_1302(R_week);
	week=BCD2D(week);

	year=RdDat_1302(R_year);
	year=BCD2D(year);
}