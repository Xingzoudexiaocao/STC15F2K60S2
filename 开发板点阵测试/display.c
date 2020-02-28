#include"stc15f2k60s2.h"
extern unsigned int num;
extern unsigned char display_num[];
code unsigned char shuzi[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
void display_1(unsigned char num,unsigned char x)//数码管显示函数
{
	       		P2=((P2&0x1f)|0xc0);
			    P0=0x80>>num;//数码管位选
		    	P2&=0x1f;
		    	P2=((P2&0x1f)|0xe0);
			    P0=shuzi[x];
			    P2&=0x1f; 
	
}
 void num_BUFF8(void)//数码管解析
{	 unsigned char ge,shi,bai,qian;
 	if(num<10)//各位
	{  ge=num;
	  display_num[0]=ge;
	}
   if(num>=10&&num<100)//十位
	{	ge=num%10;
	    display_num[0]=ge;
	    shi=num/10;
		display_num[1]=shi;		
	}
   if(num>=100&&num<1000)//百位
	{	ge=num%10;
	    display_num[0]=ge;
	    shi=(num/10)%10;
		display_num[1]=shi;
		bai=num/100;
		display_num[2]=bai;		
	}
   if(num>=1000&&num<10000)//千位
	{	ge=num%10;
	    display_num[0]=ge;
	    shi=(num%100)/10;
		display_num[1]=shi;
		bai=(num/100)%10;
		display_num[2]=bai;
		qian=num/1000;
		display_num[3]=qian;		
	}
}
void   display_all(unsigned char *p)
{
	int i;
	 for(i=0;i<8;i++)
	{	
		display_1(i,p[i]);
		delay();
	}
}
void flush()//清除缓冲区
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		display_num[i]=0;
	}
}