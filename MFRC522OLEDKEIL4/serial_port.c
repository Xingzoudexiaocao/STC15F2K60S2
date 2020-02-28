#include "serial_port.h"


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
    TH1 = (unsigned char)(Timer1_Reload >> 8);		//��ʱ���߰�λ����ֵ
    TL1 = (unsigned char)Timer1_Reload;			//��ʱ���Ͱ�λ����ֵ
    TR1 = 1;    //�򿪶�ʱ��
    PS = 1;     //�����ȼ��ж�
    REN = 1;    //�������
    ES  = 1;    //�����ж�
    EA = 1;     //����ȫ���ж�
}

//����1����һ���ֽ�
void serial_one_send_byte(unsigned char dat)
{
    SBUF = dat;   //��������
    while(!TI);   //�ȴ��������
    TI=0;         //������ͱ�־
}

//����1�����ַ���
void serial_one_send_string(unsigned char *dat)
{
    while(*dat)
        serial_one_send_byte(*dat++);
}

//����1����ָ��ָ����������
void serial_one_send_length_string(unsigned char *dat,unsigned char length)
{
    unsigned char i=0;
    for(;i < length;i++)
        serial_one_send_byte(dat[i]);
}