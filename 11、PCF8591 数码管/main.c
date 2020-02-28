							   /*



*/
#include"stc15Fxxxx.h"
u8 dbuf[8]={18,18,18,18,18,18,1,1};
//unsigned char code dis[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x7f,0x6f};
unsigned char code dis[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
//u8 code segtab[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0x89,0x8c,0xff,0xbf};
//					//0		1	 2	  3	   4	5	 6	  7	   8	9	 A	 B	  C		D	 E	 F	   H   P	灭	 -
//	 				//0		1	 2	  3	   4	5	 6	  7	   8	9	 10	 11	  12   13	14	15	  16  17	18	 19

extern bit RcvStr(u8 sla,u8 suba,u8 *s,u8 n);		//向有子地址器件读取多个字节
extern bit SendStr(u8 sla,u8 suba,u8 *s,u8 n);
void delay(u8 p)      //延时，短暂视觉效果
{
	u16 i;
	while(p--)
		for(i=0;i<100;i++);
}
//void  disp(void)
//{
//	u8 i;
//	u8 bsel=0x01;
//	P2&=0x1f;
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
void main()
{
	u8 value=178,dat=1;
//	P0&=0xaf;
//	P2=P2&0x1f|0xa0;
while(1)
	{	
		RcvStr(0x90,0x01,&value,1);		//获取当前电压模拟量
		//value=(value*500)/256;			//求电压模拟量百分比

		dbuf[5]	= value/100;
		dbuf[6]=(value/10)%10;
		dbuf[7]=value%10;

		P2=dis[dbuf[5]];
		P00=0;
		P01=1;
		P02=0;
		delay(1);
		
		P2=dis[dbuf[6]];
		
		P00=1;
		P01=0;
		P02=0;
		delay(1);
	
 	    P2=dis[dbuf[7]];
		P00=0;
		P01=0;
		P02=0;
		delay(1);

//		RcvStr(0x90,0x01,&dat,1);		//获取当前光敏模拟量
//		dat=(dat*100)/256;				//求光敏模拟量的百分比
//		dbuf[3]=(dat/10)%10;
//		dbuf[2]=dat%10;
//		disp();	
	}

}
