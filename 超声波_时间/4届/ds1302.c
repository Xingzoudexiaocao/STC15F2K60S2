#include "stc15f2k60s2.h"
#include "ds1302.h"

char second,minute,hour,week,date,mouth,year;
char set_second,set_minute,set_hour,set_week,set_date,set_mouth,set_year;
char set_second_DS,set_minute_DS,set_hour_DS,set_week_DS,set_date_DS,set_mouth_DS,set_year_DS;
void DS1302_init(void)//��ʼ��ʱ��
{
	WrDat_1302(W_control,0x00);//��ֹд����
	set_second=59;
	set_minute=20; 
	set_hour=11;  
	set_week=1;   
	set_date=1;  
	set_mouth=1; 
	set_year=10;
}
void Delay200us()		//@11.0592MHz
{
	unsigned char i, j;

	i = 3;
	j = 35;
	do
	{
		while (--j);
	} while (--i);
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
	Delay200us();
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
	Delay200us();
	RST=1;
	Wr_1302(dat);//д�����ֽ�
	p=Rd_1302();//�������ֽڣ��ݴ���a��
	RST=0;//������ϣ���ֹRST
	return p;//����1�ֽڵ�RTC��ʵʱʱ�ӣ�����
}
void Wrds1302_init(void)//дʱ��
{								   
	set_second_DS = set_second;
	set_minute_DS = set_minute;
	set_hour_DS = set_hour;
	set_week_DS = set_week;
	set_date_DS = set_date;
	set_mouth_DS = set_mouth;
	set_year_DS = set_year;
	
	set_second_DS=D2BCD(set_second_DS);//ʮ����ת��ΪBCD��
	set_minute_DS=D2BCD(set_minute_DS);
	set_hour_DS=D2BCD(set_hour_DS);
	set_date_DS=D2BCD(set_date_DS);
	set_mouth_DS=D2BCD(set_mouth_DS);
	set_week_DS=D2BCD(set_week_DS);
	set_year_DS=D2BCD(set_year_DS);

	WrDat_1302(W_control,0x00);//��ֹд����
	WrDat_1302(W_second,set_second_DS);
	WrDat_1302(W_minute,set_minute_DS);
	WrDat_1302(W_hour,set_hour_DS);
	WrDat_1302(W_date,set_date_DS);
	WrDat_1302(W_mouth,set_mouth_DS);
	WrDat_1302(W_week,set_week_DS);
	WrDat_1302(W_year,set_year_DS);
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