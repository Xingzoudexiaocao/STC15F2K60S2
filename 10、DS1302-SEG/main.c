#include"stc15fxxxx.h"
#include "ds1302.h"
unsigned char code dis[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40};
u8 code segtab[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0x89,0x8c,0xff,0xbf};
					//0		1	 2	  3	   4	5	 6	  7	   8	9	 A	 B	  C		D	 E	 F	   H   P	灭	 -
	 				//0		1	 2	  3	   4	5	 6	  7	   8	9	 10	 11	  12   13	14	15	  16  17	18	 19
					//共阳极数码管字段码
u8 dbuf[8]={0,0,19,0,0,19,0,0};
/************延时函数***************/
void delay1(void)//延时函数
{
    unsigned char i,j,k;
    for(i=0; i<200; i++)
    {
        for(j=0; j<1; j++)
        {						
            for(k=0; k<2; k++);
        }
    } 
}
/*******数码管显示函数***************/
//	void  disp(void)
//{
//	u8 i;
//	u8 bsel=0x01;
//	
//	for(i=0;i<8;i++)
//	{
//		P0=bsel;//数据准备：位选数据
//		P2=P2&0x1f|0xc0;//送入数据
//		P2&=0x1f;//再次释放片选
//		P0=segtab[dbuf[i]];//数据准备：显示数据
//		P2=P2&0x1f|0xe0;//送入数据
//		delay(1);//延时，形成视觉暂留
//		bsel=_crol_(bsel,1);	
//		P0|=0xff;//将P0口初始化为：1111 1111  熄灭所有数码管
//		P2&=0x1f;//再次释放片选	
//	}
//	
//}
int main()
{
//	P0&=0xaf;
//	P2=P2&0x1f|0xa0;
	DS1302_init();//初始化DS1302
	Wrds1302_init();
	while(1)
	{
		Rdds1302_init();
		dbuf[6]=second/10;
		dbuf[7]=second%10;
		dbuf[3]=minute/10;
		dbuf[4]=minute%10;
		dbuf[0]=hour/10;
		dbuf[1]=hour%10;
		dbuf[2]=mouth%10;

		 P00=1;
		 P01=1;
		 P02=1;
		 P2=dis[dbuf[7]];
		 delay1();
//		 P10=1;
//		 P11=0;
//		 P12=0;
//		 P0=dis[dbuf[0]];
		 delay1();
//		 P10=0;
//		 P11=1;
//		 P12=0;
//		 P0=dis[dbuf[1]];
		 delay1();
//		 P10=1;
//		 P11=1;
//		 P12=0;
//		 P0=dis[dbuf[3]];
		 delay1();
//		 P10=0;
//		 P11=0;
//		 P12=1;
//		 P0=dis[dbuf[4]];
		 delay1();
//		 P10=1;
//		 P11=0;
//		 P12=1;
//		 P0=dis[dbuf[6]];
//		 delay1();
//		 P10=0;
//		 P11=1;
//		 P12=1;
//		 P0=dis[dbuf[7]];
//		 delay1();
//		 P10=1;
//		 P11=1;
//		 P12=1;
//		 P0=dis[11];
//		 delay1();


//		disp();	
	}		 
	return 0;
}