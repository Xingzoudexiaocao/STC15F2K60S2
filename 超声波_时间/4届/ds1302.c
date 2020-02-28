#include "stc15f2k60s2.h"
#include "ds1302.h"

char second,minute,hour,week,date,mouth,year;
char set_second,set_minute,set_hour,set_week,set_date,set_mouth,set_year;
char set_second_DS,set_minute_DS,set_hour_DS,set_week_DS,set_date_DS,set_mouth_DS,set_year_DS;
void DS1302_init(void)//初始化时间
{
	WrDat_1302(W_control,0x00);//禁止写保护
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




void delay(u8 i)//延时
{
	u8 n;
	while(i--)
		for(n=0;n<200;n++);
}



void Wr_1302(u8 dat)//向指定地址写入一个字节数据函数
{
	u8 i;
	for(i=0;i<8;i++)
	{
		DIO=dat&0x01;
		SCLK=0;
		SCLK=1;//DS_1302在上升沿是写				
		dat>>=1;//八位数据或命令右移一位
	}
}
u8 Rd_1302(void)//从指定地址读取一个字节数据函数
{
	u8 i,p;
	DIO=1;
	for(i=8;i>0;i--)
	{
		SCLK=1;
		SCLK=0;//DS_1302在下降沿是读
		p>>=1;//接受新的1位数据前先右移一位
		if(DIO==1)		   	
		p|=0x80;//p最高位接受数据，其他位保持不变
	}
	return p;
}

void WrDat_1302(u8 dat,u8 cmd)//写字节函数
{
	SCLK=0;//在RST由低变高之前，SCLK必须保持低电平
	RST=0;
	Delay200us();
	RST=1;//RST高电平允许读/写
	Wr_1302(dat);//写命令字节
	Wr_1302(cmd);//写数据字节
	RST=0;//操作完毕，禁止RST
}


u8 RdDat_1302(u8 dat)//读字节函数
{
	u8 p;
	SCLK=0;//在RST由低变高之前，SCLK必须保持低电平
	RST=0;//RST高电平允许读/写
	Delay200us();
	RST=1;
	Wr_1302(dat);//写命令字节
	p=Rd_1302();//读数据字节，暂存在a中
	RST=0;//操作完毕，禁止RST
	return p;//返回1字节的RTC（实时时钟）数据
}
void Wrds1302_init(void)//写时间
{								   
	set_second_DS = set_second;
	set_minute_DS = set_minute;
	set_hour_DS = set_hour;
	set_week_DS = set_week;
	set_date_DS = set_date;
	set_mouth_DS = set_mouth;
	set_year_DS = set_year;
	
	set_second_DS=D2BCD(set_second_DS);//十进制转换为BCD码
	set_minute_DS=D2BCD(set_minute_DS);
	set_hour_DS=D2BCD(set_hour_DS);
	set_date_DS=D2BCD(set_date_DS);
	set_mouth_DS=D2BCD(set_mouth_DS);
	set_week_DS=D2BCD(set_week_DS);
	set_year_DS=D2BCD(set_year_DS);

	WrDat_1302(W_control,0x00);//禁止写保护
	WrDat_1302(W_second,set_second_DS);
	WrDat_1302(W_minute,set_minute_DS);
	WrDat_1302(W_hour,set_hour_DS);
	WrDat_1302(W_date,set_date_DS);
	WrDat_1302(W_mouth,set_mouth_DS);
	WrDat_1302(W_week,set_week_DS);
	WrDat_1302(W_year,set_year_DS);
	WrDat_1302(W_control,0x80);//允许写保护
								 
}
void Rdds1302_init(void)//读取RTC数据
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