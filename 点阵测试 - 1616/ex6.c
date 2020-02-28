 # include"stc15f2k60s2.h"
#include"intrins.h"

unsigned char code  table0[] = {0x20,0x04,0x20,0x04,0x20,0x24,0x40,0x24,0x7E,0x3F,0x84,0x24,0x44,0x45,0x44,0x04,
0x44,0x07,0x28,0x7C,0x28,0x24,0x10,0x04,0x28,0x04,0x44,0x04,0x82,0x04,0x00,0x05};
unsigned char code  table1[] = {0x00,0x01,0x80,0x00,0xFC,0x1F,0x10,0x04,0x20,0x02,0xFE,0x7F,0x00,0x00,0xF8,0x0F,
0x88,0x08,0xF8,0x0F,0x88,0x08,0xF8,0x0F,0x80,0x00,0xFC,0x1F,0x80,0x00,0xFE,0x7F};

sbit	SHCP = P1^0;
sbit	STCP = P1^1;
sbit	DS   = P1^2;


void WriteByte(char dat)          //дһ���ֽڵ����� 
{
	char i;	
	for(i=0;i<8;i++)          //ѭ��8�ΰѱ��봫�������� 	
	{
		dat=dat>>1;       //����һλ��ȡ�����ֽڵ����λ 
		DS=CY;            //�����ֽڵ����λ����R
		SHCP=0;          //����������595�������� 
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
     P0=num;          //ѡ���Ӧ����
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


	}           
}

