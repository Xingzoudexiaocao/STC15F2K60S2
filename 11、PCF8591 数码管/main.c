							   /*



*/
#include"stc15Fxxxx.h"
u8 dbuf[8]={18,18,18,18,18,18,1,1};
//unsigned char code dis[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x7f,0x6f};
unsigned char code dis[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
//u8 code segtab[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0x89,0x8c,0xff,0xbf};
//					//0		1	 2	  3	   4	5	 6	  7	   8	9	 A	 B	  C		D	 E	 F	   H   P	��	 -
//	 				//0		1	 2	  3	   4	5	 6	  7	   8	9	 10	 11	  12   13	14	15	  16  17	18	 19

extern bit RcvStr(u8 sla,u8 suba,u8 *s,u8 n);		//�����ӵ�ַ������ȡ����ֽ�
extern bit SendStr(u8 sla,u8 suba,u8 *s,u8 n);
void delay(u8 p)      //��ʱ�������Ӿ�Ч��
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
//		P0=bsel;//����׼����λѡ����
//		P2=P2&0x1f|0xc0;//��������
//		P2&=0x1f;//�ٴ��ͷ�Ƭѡ
//		P0=segtab[dbuf[i]];//����׼������ʾ����
//		P2=P2&0x1f|0xe0;//��������
//		delay(1);//��ʱ���γ��Ӿ�����
//		bsel=_crol_(bsel,1);	
//		P0|=0xff;//��P0�ڳ�ʼ��Ϊ��1111 1111  Ϩ�����������
//		P2&=0x1f;//�ٴ��ͷ�Ƭѡ	
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
		RcvStr(0x90,0x01,&value,1);		//��ȡ��ǰ��ѹģ����
		//value=(value*500)/256;			//���ѹģ�����ٷֱ�

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

//		RcvStr(0x90,0x01,&dat,1);		//��ȡ��ǰ����ģ����
//		dat=(dat*100)/256;				//�����ģ�����İٷֱ�
//		dbuf[3]=(dat/10)%10;
//		dbuf[2]=dat%10;
//		disp();	
	}

}
