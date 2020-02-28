#include "stc15f2k60s2.h"
#include "18b20.h"
sbit DQ = P1^7;
void Delay_OneWire(unsigned int t) //��ʱ���� 
{
	unsigned char i;
	while(t--)
		{for(i=0;i<12;i++);}
		}
void Write_DS18B20(unsigned char dat)	 //����һ���ֽ�
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(5);
		DQ = 1;
		dat >>= 1;
	}
	Delay_OneWire(5);
}
unsigned char Read_DS18B20(void)		   //����һ���ֽ�
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(5);
	}
	return dat;
}
bit init_ds18b20(void)	//��ʼ������
{
  	bit initflag = 0;
  	DQ = 1;
  	Delay_OneWire(12);
  	DQ = 0;
  	Delay_OneWire(80); // ��ʱ����480us
  	DQ = 1;
  	Delay_OneWire(10);  // 14
  	initflag = DQ;     // initflag����1��ʼ��ʧ��
  	Delay_OneWire(5);
  
  	return initflag;
}
unsigned int rd_temperature(void)
{
    unsigned char low,high;
  	unsigned int temp;
  
  	init_ds18b20();
  	Write_DS18B20(0xCC);
  	Write_DS18B20(0x44); //�����¶�ת��
  	Delay_OneWire(20);

  	init_ds18b20();
  	Write_DS18B20(0xCC);
  	Write_DS18B20(0xBE); //��ȡ�Ĵ���

  	low = Read_DS18B20(); //���ֽ�
  	high = Read_DS18B20(); //���ֽ�
  
  	temp = high<<8;
  	temp |= low;
  
  	return temp;
}