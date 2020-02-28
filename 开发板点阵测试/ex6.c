#include"stc15f2k60s2.h"
#include"intrins.h"
#define A0 P03
#define A1 P04
#define A2 P05
#define A3 P06
#define	SHCP P07
#define	STCP P15
#define	DS   P16
//--  宋体12;  此字体下对应的点阵为：宽x高=16x16   -- 
unsigned char code  table0[] = {0x00,0x04,0x02,0x04,0x42,0x04,0x22,0x3F,0x14,0x04,0x14,0x04,0x08,0x7F,0x08,0x04,
0x14,0x14,0x14,0x14,0x22,0x17,0x42,0x14,0x00,0x14,0x00,0x2C,0xFE,0x27,0x00,0x40};
unsigned char code  table1[] = {0x00,0x00,0x7E,0x00,0x42,0x3E,0x42,0x22,0x42,0x22,0x7E,0x22,0x42,0x3E,0x42,0x22,
0x42,0x22,0x7E,0x22,0x42,0x3E,0x42,0x22,0x82,0x00,0x82,0x00,0x0A,0x01,0x04,0x02};




void WriteByte(char dat)          //写一个字节的数据 
{
	char i;	
	for(i=0;i<8;i++)          //循环8次把编码传给锁存器 	
	{
		dat=dat>>1;       //右移一位，取出该字节的最低位 
		DS=CY;            //将该字节的最低位传给R
		SHCP=0;          //将数据移入595，上升沿 
		SHCP=1;                    
	} 
	    
}
void display(unsigned char *p)
{
	int x,num;
	for(num=0;num<16;num++)
	{ 
		WriteByte(0); 
		WriteByte(0);   	 	
		STCP=1;         					          						
		STCP=0; 
		//选择对应的行
		switch(num)
		{
			case 0 : A0 = 0; A1 = 0; A2 = 0; A3 = 0; break;
			case 1 : A0 = 1; A1 = 0; A2 = 0; A3 = 0; break;
			case 2 : A0 = 0; A1 = 1; A2 = 0; A3 = 0; break;
			case 3 : A0 = 1; A1 = 1; A2 = 0; A3 = 0; break;
			case 4 : A0 = 0; A1 = 0; A2 = 1; A3 = 0; break;
			case 5 : A0 = 1; A1 = 0; A2 = 1; A3 = 0; break;
			case 6 : A0 = 0; A1 = 1; A2 = 1; A3 = 0; break;
			case 7 : A0 = 1; A1 = 1; A2 = 1; A3 = 0; break;
			case 8 : A0 = 0; A1 = 0; A2 = 0; A3 = 1; break;
			case 9 : A0 = 1; A1 = 0; A2 = 0; A3 = 1; break;
			case 10 : A0 = 0; A1 = 1; A2 = 0; A3 = 1; break;
			case 11 : A0 = 1; A1 = 1; A2 = 0; A3 = 1; break;
			case 12 : A0 = 0; A1 = 0; A2 = 1; A3 = 1; break;
			case 13 : A0 = 1; A1 = 0; A2 = 1; A3 = 1; break;
			case 14 : A0 = 0; A1 = 1; A2 = 1; A3 = 1; break;
			case 15 : A0 = 1; A1 = 1; A2 = 1; A3 = 1; break;
		}
		
		WriteByte(*(p+2*num)); 
		WriteByte(*(p+2*num+1));   	 	
		STCP=1;         					          						
		STCP=0; 
		for(x=0;x<100;x++);
	}
}

int main(void)
{	
    unsigned int j;

	while(1)
	{	
		for(j=0;j<500;j++)
		display(table0);

		for(j=0;j<500;j++)
		display(table1); 

		for(j=0;j<500;j++)
		display(table1); 

	}           
}

