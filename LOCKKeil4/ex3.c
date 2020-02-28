
#include "stc15f2k60s2.h"
#include <intrins.h>
/*************��������****************/

#define MAIN_Fosc                   11059200L       //��ʱ��,����Ƶ�ʲ�ͬ��ֱ���������޸�Ƶ��
#define serial_one_read_max         4              //���建�����ĳ���
#define serial_one_baud_rate        9600L           //������,ֻ���������޸Ĳ�����
#define Timer1_Reload               (65536UL -(MAIN_Fosc / 4 / serial_one_baud_rate))    //�ö�ʱ��1�������ʷ����������㶨ʱ��1����װֵ    
#define u8 unsigned char



/*************��������****************/
u8  serial_one_read_data[serial_one_read_max]={0,0,0,0};  //���崮�ڽ������ݻ�����
u8  serial_one_read_count=0;                    //�������ݵĸ���


void serial_one_init();
void serial_one_send_byte(u8 dat);
void serial_one_send_string(u8 *dat);
void serial_one_send_length_string(u8 *dat,u8 length);

void Delay100ms();
void Delay2000ms();

void Delay2000ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 85;
	j = 12;
	k = 155;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay100ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 5;
	j = 52;
	k = 195;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


//��ʼ������1
void serial_one_init()
{
    SCON |= 0x40;          //8λ����
    P_SW1 &=  ~0xc0;       //UART1 ʹ��P30 P31��  Ĭ��
    TR1 = 0;               //�رն�ʱ��
    AUXR &= ~0x01;      //S1 BRT Use Timer1;
    TMOD &= ~(1<<6);    //Timer1 set As Timer
    TMOD &= ~0x30;      //Timer1_16bitAutoReload;2
    AUXR |=  (1<<6);    //Timer1 set as 1T mode
    TH1 = (u8)(Timer1_Reload >> 8);		//��ʱ���߰�λ����ֵ
    TL1 = (u8)Timer1_Reload;			//��ʱ���Ͱ�λ����ֵ
    TR1 = 1;    //�򿪶�ʱ��
    PS = 1;     //�����ȼ��ж�
    REN = 1;    //�������
    ES  = 1;    //�����ж�
	EA = 1;     //����ȫ���ж�
}

//����1����һ���ֽ�
void serial_one_send_byte(u8 dat)
{
    SBUF = dat;   //��������
    while(!TI);   //�ȴ��������
    TI=0;         //������ͱ�־
}

//����1�����ַ���
void serial_one_send_string(u8 *dat)
{
    while(*dat)
        serial_one_send_byte(*dat++);
}

//����1����ָ��ָ����������
void serial_one_send_length_string(u8 *dat,u8 length)
{
    u8 i=0;
    for(;i < length;i++)
        serial_one_send_byte(dat[i]);
}

void Timer0Init(void)		//1����@11.0592MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0xCD;		//���ö�ʱ��ֵ
	TH0 = 0xD4;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
}


void main()
{
	char i=0;
    serial_one_init();//���ڳ�ʼ��
	Timer0Init();
	P10=1;
    while(1)
    {
	//	serial_one_send_string(serial_one_read_data);
      if((serial_one_read_data[0]=='Z'))
	  {	

	  		P10=0;
			ET0 = 1;
			Delay100ms();
			ET0 = 0;
			Delay2000ms();

			P10=1;
			ET0 = 1;
			Delay100ms();
			ET0 = 0;

			for(i=0;i<10;i++)
			serial_one_read_data[i]=0;
	  }
	  if(serial_one_read_data[3]!=0)
	  		for(i=0;i<4;i++)
					serial_one_read_data[i]=0;
	  }

}

/********************* ����1�жϺ���************************/
void UART1_int (void) interrupt 4
{
    if(RI)
    {
        RI = 0;//������ܱ�־
        serial_one_read_data[serial_one_read_count] = SBUF;//�����ݴ�������

        if(++serial_one_read_count > serial_one_read_max)    //�������
            serial_one_read_count = 0;

    }
}
void tm0_isr() interrupt 1
{
    P11 = ! P11;                    //�����Կ�ȡ��
}