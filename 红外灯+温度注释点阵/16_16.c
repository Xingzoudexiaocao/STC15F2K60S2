#include "16_16.h"

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
		//ѡ���Ӧ����
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





