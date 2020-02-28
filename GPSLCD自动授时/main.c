/************************************************************
������������Ƭ��12864Һ����ʾ 
************************************************************/
#include"stc15fxxxx.h"
#include<stdlib.h>
unsigned char code dis[]={0x3f,0x06,0x5b,0x4f,0x6d,0x7d,0x07,0x7f,0x6f};

#define MAIN_Fosc                   11059200L       //��ʱ��,����Ƶ�ʲ�ͬ��ֱ���������޸�Ƶ��
#define serial_one_read_max         40              //���建�����ĳ���
#define serial_one_baud_rate        9600L           //������,ֻ���������޸Ĳ�����
#define Timer1_Reload               (65536UL -(MAIN_Fosc / 4 / serial_one_baud_rate))    //�ö�ʱ��1�������ʷ����������㶨ʱ��1����װֵ    
#define u8 unsigned char

/*************��������****************/
u8  serial_one_read_data[serial_one_read_max];  //���崮�ڽ������ݻ�����
u8  serial_one_read_count=0;                    //�������ݵĸ���

unsigned char LCD_cache[9];
bit  delay_1ms(u16 x);
void delay(void);
bit lcd_busy(void);
bit  lcd_pos(u8 X,u8 Y);
bit lcd_init();
bit write_str(u8 *c);
unsigned char RX_dat_flag=0;
unsigned char time0_num=0;
/************************************************************
������������LCD12864��ָ��λ����ʾָ������
��ڲ�������
���ڲ�������
************************************************************/
void Timer0Init(void)		//50����@11.0592MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0x4C;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1;
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

void Delay10ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 108;
	j = 145;
	do
	{
		while (--j);
	} while (--i);
}
void main()
{
    lcd_init();       			//LCD��ʼ��
	delay_1ms(3);	  			//��ʱ
	lcd_pos(0,1);				//ָ����ʾλ��
	write_str("GPS");	//��ʾ�ַ���
	lcd_pos(0,3);				//ָ����ʾλ��
	write_str("����ʱ��");	//��ʾ�ַ���
	lcd_pos(1,2);				//ָ����ʾλ��
	write_str("�Զ���ʱ");	//��ʾ�ַ���
	lcd_pos(3,1);				//ָ����ʾλ��
	write_str("������ͯ������");

	P0&=0xaf;
	P2=P2&0x1f|0xa0;
    serial_one_init();//���ڳ�ʼ��
	Timer0Init();
    while(1)
    {
		if(time0_num == 0)
		{
			P10=0;
			LCD_cache[0]=serial_one_read_data[0];
			LCD_cache[1]=serial_one_read_data[1];
			LCD_cache[2]=':';
			LCD_cache[3]=serial_one_read_data[2];
			LCD_cache[4]=serial_one_read_data[3];
			LCD_cache[5]=':';
			LCD_cache[6]=serial_one_read_data[4];
			LCD_cache[7]=serial_one_read_data[5];
			LCD_cache[8]='\0';

			LCD_cache[0]=(LCD_cache[0]-48)*10+LCD_cache[1]-48;	  //Сʱʮλ
			LCD_cache[0]=LCD_cache[0]+8;

			if(LCD_cache[0]==24)
				 LCD_cache[0]=0;
			else if(LCD_cache[0]==25)
				 LCD_cache[0]=1;
			 else if(LCD_cache[0]==26)
				 LCD_cache[0]=2;
			 else if(LCD_cache[0]==27)
				 LCD_cache[0]=3;
			 else if(LCD_cache[0]==28)
				 LCD_cache[0]=4;
			 else if(LCD_cache[0]==29)
				 LCD_cache[0]=5;
			 else if(LCD_cache[0]==30)
				 LCD_cache[0]=6;
			 else if(LCD_cache[0]==31)
				 LCD_cache[0]=7;
			 else if(LCD_cache[0]==32)
				 LCD_cache[0]=8;

			LCD_cache[1]=LCD_cache[0]%10+48;
			LCD_cache[0]=LCD_cache[0]/10+48;	
		}
		if(time0_num==5)
		{
			P10=1;
			lcd_pos(2,2);				//ָ����ʾλ��
			write_str(LCD_cache);	//��ʾ�ַ���
		}
        if(RX_dat_flag == 0)//������ܵ�����
        {
            serial_one_send_length_string(serial_one_read_data,serial_one_read_count);//���ͽ��յ�������
            serial_one_read_count = 0;//���ռ�����0
			Delay10ms();
        }
    }
}
unsigned char num=0;
/********************* ����1�жϺ���************************/
void UART1_int (void) interrupt 4
{
	unsigned char cache=0x00;
    if(RI)
    {
        RI = 0;//������ܱ�־
		cache = SBUF;
		if(cache=='L')
		{
			RX_dat_flag=1;
		}
		if((RX_dat_flag==1)&&(cache==','))
		{
		 	RX_dat_flag=2;
			cache=0;
		}
		if((RX_dat_flag==2)&&(cache==','))
		{
		 	RX_dat_flag=3;
			cache=0;
		}
		if((RX_dat_flag==3)&&(cache==','))
		{
		 	RX_dat_flag=4;
			cache=0;
		}
		if((RX_dat_flag==4)&&(cache==','))
		{
		 	RX_dat_flag=5;
			cache=0;
		}
		if((RX_dat_flag==5)&&(cache==','))
		{
		 	RX_dat_flag=6;
		}

		if(RX_dat_flag==7)	
		{
			num++;
			serial_one_read_data[serial_one_read_count]=cache;
			serial_one_read_count++;
		}
		if(RX_dat_flag==6)
			RX_dat_flag=7;


		if(num>5)
		{
			num=0;
			RX_dat_flag=0;
		}
    }
}
void TIME0 (void) interrupt 1
{
	if(time0_num>18)
		time0_num=0;
	else
		time0_num++;
}
