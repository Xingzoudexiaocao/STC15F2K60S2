#include"stc15f2k60s2.h"
#include"intrins.h"
#include "DS1302.h"
unsigned char code dis[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00};
unsigned char dbuf[]={10,10,10,10,10,10,10,10};
void display();
extern u8 second,minute,hour,week,date,mouth,year;

void Delay5ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 9;
	j = 244;
	do
	{
		while (--j);
	} while (--i);
}




void main(void)
{  		
	DS1302_init();//³õÊ¼»¯DS1302
	Wrds1302_init();  

	while(1)  
		{
			Rdds1302_init();
			dbuf[6]=second/10;
			dbuf[7]=second%10;
			dbuf[4]=minute/10;
			dbuf[5]=minute%10;
			dbuf[2]=hour/10;
			dbuf[3]=hour%10;
			dbuf[0]=mouth/10;
			dbuf[1]=mouth%10;  
			Delay5ms();
			display();
	    }
}

sfr dat_ = 0xa0;

//sbit cmd = P1^4;
unsigned char a=0;

void display()
{
//  ch_=(ch_&0xf8)|a;
//  cmd=1;
  dat_=dis[dbuf[a]];
  P0=a;
//  cmd=0;
  a++;
  if(a>=8)
  a=0;

  
}


