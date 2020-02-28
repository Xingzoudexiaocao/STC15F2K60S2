#include "iic.h"
#include "main.h"

//�������Ŷ���
sbit SDA = P2^1;  /* ������ */
sbit SCL = P2^0;  /* ʱ���� */

void Delay5us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 11;
	while (--i);
}
//������������
void IIC_Start(void)
{
    SDA = 1;
    SCL = 1;
    Delay5us();
    SDA = 0;
    Delay5us();
    SCL = 0;	
}

//����ֹͣ����
void IIC_Stop(void)
{
    SDA = 0;
    SCL = 1;
    Delay5us();
    SDA = 1;
    Delay5us();
}

//����Ӧ��
void IIC_SendAck(bit ackbit)
{
    SCL = 0;
    SDA = ackbit;  					// 0��Ӧ��1����Ӧ��
    Delay5us();
    SCL = 1;
    Delay5us();
    SCL = 0; 
    SDA = 1;
    Delay5us();
}

//�ȴ�Ӧ��
bit IIC_WaitAck(void)
{
    bit ackbit;
	
    SCL  = 1;
    Delay5us();
    ackbit = SDA;
    SCL = 0;
    Delay5us();
    return ackbit;
}

//ͨ��I2C���߷�������
void IIC_SendByte(unsigned char byt)
{
    unsigned char i;

    for(i=0; i<8; i++)
    {
        SCL  = 0;
        Delay5us();
        if(byt & 0x80) SDA  = 1;
        else SDA  = 0;
       	Delay5us();
        SCL = 1;
        byt <<= 1;
        Delay5us();
    }
    SCL  = 0;  
}

//��I2C�����Ͻ�������
unsigned char IIC_RecByte(void)
{
    unsigned char i, da;
    for(i=0; i<8; i++)
    {   
    	SCL = 1;
	Delay5us();
	da <<= 1;
	if(SDA) da |= 1;
	SCL = 0;
	Delay5us();
    }
    return da;    
}
void IIC_send_dat(unsigned char dev ,unsigned char dev_dat ,unsigned char dat)
{
	IIC_Start();
	IIC_SendByte(dev);
	IIC_WaitAck();

	IIC_SendByte(dev_dat);
	IIC_WaitAck();

	IIC_SendByte(dat);
	IIC_WaitAck();

	IIC_Stop();

}
unsigned char IIC_read_dat(unsigned char dev ,unsigned char dev_dat)
{
	unsigned char dat;
	IIC_Start();

	IIC_SendByte(dev);
	IIC_WaitAck();

	IIC_SendByte(dev_dat);
	IIC_WaitAck();

	IIC_Stop();

	IIC_Start();
	IIC_SendByte(dev + 1);
	IIC_WaitAck();

	dat = IIC_RecByte();

	IIC_Stop();
	return dat;
}

