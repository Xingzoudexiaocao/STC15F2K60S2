#include <intrins.h>
#include <STC15F2K60S2.H>
#include "IIC.h"

sbit lcd_sclk=P1^0;  /*�ӿڶ���:lcd_sclk����D0*/
sbit lcd_sid=P1^1;   /*�ӿڶ���:lcd_sid����D1*/
sbit lcd_rs=P1^2;    /*�ӿڶ���:lcd_rs����DC*/
sbit lcd_cs1=P1^3;   /*�ӿڶ���:lcd_cs1����OLED��ƬѡCS1*/
sbit Rom_OUT=P1^4;   /*�ֿ�IC�ӿڶ���:Rom_OUT����OUT*/
sbit Rom_CS=P1^5;    /*�ֿ�IC�ӿڶ���Rom_CS�����ֿ�IC��CS2*/

#define uchar unsigned char 
#define uint unsigned int
#define ulong unsigned long
#define MAIN_Fosc                   11059200L       //��ʱ��,����Ƶ�ʲ�ͬ��ֱ���������޸�Ƶ��
#define serial_one_read_max         32              //���建�����ĳ���
#define serial_one_baud_rate        9600L           //������,ֻ���������޸Ĳ�����
#define Timer1_Reload               (65536UL -(MAIN_Fosc / 4 / serial_one_baud_rate))    //�ö�ʱ��1�������ʷ����������㶨ʱ��1����װֵ    
uchar UART_Dat;
void serial_one_init()
{
    SCON |= 0x40;          //8λ����
    P_SW1 &=  ~0xc0;       //UART1 ʹ��P30 P31��  Ĭ��
    TR1 = 0;               //�رն�ʱ��
    AUXR &= ~0x01;      //S1 BRT Use Timer1;
    TMOD &= ~(1<<6);    //Timer1 set As Timer
    TMOD &= ~0x30;      //Timer1_16bitAutoReload;2
    AUXR |=  (1<<6);    //Timer1 set as 1T mode
    TH1 = (uchar)(Timer1_Reload >> 8);		//��ʱ���߰�λ����ֵ
    TL1 = (uchar)Timer1_Reload;			//��ʱ���Ͱ�λ����ֵ
    TR1 = 1;    //�򿪶�ʱ��
    PS = 1;     //�����ȼ��ж�
    REN = 1;    //�������
    ES  = 1;    //�����ж�
    EA = 1;     //����ȫ���ж�
}
//����1����һ���ֽ�
void serial_one_send_byte(uchar dat)
{
    SBUF = dat;   //��������
    while(!TI);   //�ȴ��������
    TI=0;         //������ͱ�־
}
/********************* ����1�жϺ���************************/
void UART1_int (void) interrupt 4
{
    if(RI)
    {
        RI = 0;//������ܱ�־
        UART_Dat = SBUF;//�����ݴ�������
    }
}


/*дָ�LCDģ��*/
void transfer_command_lcd(int data1)   
{
	char i;
	lcd_rs=0;
	lcd_sclk=0;
	for(i=0;i<8;i++)
  	{
		if(data1&0x80) lcd_sid=1;
		else lcd_sid=0;
		lcd_sclk=1;
		_nop_();
		lcd_sclk=0;
	 	data1<<=1;
 	 }
}

/*д���ݵ�LCDģ��*/
void transfer_data_lcd(int data1)
{
	char i;
	lcd_rs=1;
	lcd_sclk=0;
	for(i=0;i<8;i++)
   	{
		if(data1&0x80) lcd_sid=1;
		else lcd_sid=0;
		lcd_sclk=1;
		_nop_();
		lcd_sclk=0;
	 	data1<<=1;
   	}
}

/*��ʱ*/
void delay(int n_ms)               
{
 int j,k;
 for(j=0;j<n_ms;j++)
 for(k=0;k<110;k++);
}


/*LCDģ���ʼ��*/
void initial_lcd()
{
	lcd_cs1=0;
	Rom_CS = 1;
//	lcd_reset=0;        /*�͵�ƽ��λ*/
	delay(20);
	//lcd_reset=1;		    /*��λ���*/
	delay(20);        
	transfer_command_lcd(0xAE);   //display off
	transfer_command_lcd(0x20);	//Set Memory Addressing Mode	
	transfer_command_lcd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	transfer_command_lcd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	transfer_command_lcd(0xc8);	//Set COM Output Scan Direction
	transfer_command_lcd(0x02);//---set low column address
	transfer_command_lcd(0x10);//---set high column address
	transfer_command_lcd(0x40);//--set start line address
	transfer_command_lcd(0x81);//--set contrast control register
	transfer_command_lcd(0x7f);
	transfer_command_lcd(0xa1);//--set segment re-map 0 to 127
	transfer_command_lcd(0xa6);//--set normal display
	transfer_command_lcd(0xa8);//--set multiplex ratio(1 to 64)
	transfer_command_lcd(0x3F);//
	transfer_command_lcd(0xa4);//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	transfer_command_lcd(0xd3);//-set display offset
	transfer_command_lcd(0x00);//-not offset
	transfer_command_lcd(0xd5);//--set display clock divide ratio/oscillator frequency
	transfer_command_lcd(0xf0);//--set divide ratio
	transfer_command_lcd(0xd9);//--set pre-charge period
	transfer_command_lcd(0x22); //
	transfer_command_lcd(0xda);//--set com pins hardware configuration
	transfer_command_lcd(0x12);
	transfer_command_lcd(0xdb);//--set vcomh
	transfer_command_lcd(0x20);//0x20,0.77xVcc
	transfer_command_lcd(0x8d);//--set DC-DC enable
	transfer_command_lcd(0x14);//
	transfer_command_lcd(0xaf);//--turn on oled panel 
	lcd_cs1=1;
}

void lcd_address(uchar page,uchar column)
{
	transfer_command_lcd(0xb0 + column);   /*����ҳ��ַ*/
	transfer_command_lcd((((page+2) & 0xf0) >> 4) | 0x10);	/*�����е�ַ�ĸ�4λ*/
	transfer_command_lcd(((page+2)& 0x0f) | 0x00);	/*�����е�ַ�ĵ�4λ*/	
}

/*ȫ������*/
void clear_screen()
{
	unsigned char i,j;
	lcd_cs1=0;
	Rom_CS = 1;	
	for(i=0;i<8;i++)
	{
		transfer_command_lcd(0xb0+i);
		transfer_command_lcd(0x02);
		transfer_command_lcd(0x10);
		for(j=0;j<132;j++)
		{
		  	transfer_data_lcd(0x00);
		}
	}
 	lcd_cs1=1;
}

/*��ʾ16x16����ͼ�񡢺��֡���Ƨ�ֻ�16x16���������ͼ��*/
void display_graphic_16x16(uint page,uint column,uchar *dp)
{
	uint i,j;
 	lcd_cs1=0;
	Rom_CS = 1; 	
	for(j=2;j>0;j--)
	{
		lcd_address(column,page);
		for (i=0;i<16;i++)
		{	
			transfer_data_lcd(*dp);					/*д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1*/
			dp++;
		}
		page++;
	}
	lcd_cs1=1;
}

/*��ʾ8x16����ͼ��ASCII, ��8x16����������ַ�������ͼ��*/
void display_graphic_8x16(uint page,uchar column,uchar *dp)
{
	uint i,j;
	lcd_cs1=0;	
	for(j=2;j>0;j--)
	{
		lcd_address(column,page);
		for (i=0;i<8;i++)
		{	
			transfer_data_lcd(*dp);					/*д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1*/
			dp++;
		}
		page++;
	}
	lcd_cs1=1;
}

/****��ָ�����Ѷ�ֿ�IC***/
void send_command_to_ROM( uchar datu )
{
	uchar i;
	for(i=0;i<8;i++ )
	{
		if(datu&0x80)
			lcd_sid = 1;
		else
			lcd_sid = 0;
			datu = datu<<1;
			lcd_sclk=0;
			lcd_sclk=1;
	}
}

/****�Ӿ���Ѷ�ֿ�IC��ȡ���ֻ��ַ����ݣ�1���ֽڣ�***/
static uchar get_data_from_ROM( )
{
	uchar i;
	uchar ret_data=0;
	lcd_sclk=1;
	for(i=0;i<8;i++)
	{
		Rom_OUT=1;
		lcd_sclk=0;
		ret_data=ret_data<<1;
		if( Rom_OUT )
			ret_data=ret_data+1;
		else
			ret_data=ret_data+0;
		lcd_sclk=1;
	}
	return(ret_data);
}

/*����ص�ַ��addrHigh����ַ���ֽ�,addrMid����ַ���ֽ�,addrLow����ַ���ֽڣ�����������DataLen���ֽڵ����ݵ� pBuff�ĵ�ַ*/
/*������ȡ*/
void get_n_bytes_data_from_ROM(uchar addrHigh,uchar addrMid,uchar addrLow,uchar *pBuff,uchar DataLen )
{
	uchar i;
	Rom_CS = 0;
	lcd_cs1=1;	
	lcd_sclk=0;
	send_command_to_ROM(0x03);
	send_command_to_ROM(addrHigh);
	send_command_to_ROM(addrMid);
	send_command_to_ROM(addrLow);
	for(i = 0; i < DataLen; i++ )
	     *(pBuff+i) =get_data_from_ROM();
	Rom_CS = 1;
}

/******************************************************************/

ulong  fontaddr=0;
void display_GB2312_string(uchar y,uchar x,uchar *text)
{
	uchar i= 0;
	uchar addrHigh,addrMid,addrLow ;
	uchar fontbuf[32];			
	while((text[i]>0x00))
	{
		if(((text[i]>=0xb0) &&(text[i]<=0xf7))&&(text[i+1]>=0xa1))
		{						
			/*������壨GB2312�������ھ���Ѷ�ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
			/*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0*/
			/*���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ*/
			fontaddr = (text[i]- 0xb0)*94; 
			fontaddr += (text[i+1]-0xa1)+846;
			fontaddr = (ulong)(fontaddr*32);
			
			addrHigh = (fontaddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
			addrMid = (fontaddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
			addrLow = fontaddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
			get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,32 );/*ȡ32���ֽڵ����ݣ��浽"fontbuf[32]"*/
			display_graphic_16x16(y,x,fontbuf);/*��ʾ���ֵ�LCD�ϣ�yΪҳ��ַ��xΪ�е�ַ��fontbuf[]Ϊ����*/
			i+=2;
			x+=16;
		}
		else if(((text[i]>=0xa1) &&(text[i]<=0xa3))&&(text[i+1]>=0xa1))
		{						
			/*������壨GB2312��15x16����ַ��ھ���Ѷ�ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
			/*Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0*/
			/*���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ*/
			fontaddr = (text[i]- 0xa1)*94; 
			fontaddr += (text[i+1]-0xa1);
			fontaddr = (ulong)(fontaddr*32);
			
			addrHigh = (fontaddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
			addrMid = (fontaddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
			addrLow = fontaddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
			get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,32 );/*ȡ32���ֽڵ����ݣ��浽"fontbuf[32]"*/
			display_graphic_16x16(y,x,fontbuf);/*��ʾ���ֵ�LCD�ϣ�yΪҳ��ַ��xΪ�е�ַ��fontbuf[]Ϊ����*/
			i+=2;
			x+=16;
		}
		else if((text[i]>=0x20) &&(text[i]<=0x7e))	
		{						
			unsigned char fontbuf[16];			
			fontaddr = (text[i]- 0x20);
			fontaddr = (unsigned long)(fontaddr*16);
			fontaddr = (unsigned long)(fontaddr+0x3cf80);			
			addrHigh = (fontaddr&0xff0000)>>16;
			addrMid = (fontaddr&0xff00)>>8;
			addrLow = fontaddr&0xff;

			get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,16 );/*ȡ16���ֽڵ����ݣ��浽"fontbuf[32]"*/
			
			display_graphic_8x16(y,x,fontbuf);/*��ʾ8x16��ASCII�ֵ�LCD�ϣ�yΪҳ��ַ��xΪ�е�ַ��fontbuf[]Ϊ����*/
			i+=1;
			x+=8;
		}
		else
			i++;	
	}
}


void num_4(unsigned char y, unsigned char x,unsigned int num);
unsigned char key_scan(void);
uchar code send_dat[]={0x01,0x03,0x05,0x07,0x09};
void main(void)
{   
	unsigned char speed = 0;
	unsigned char key_value=0x00;
	serial_one_init();
	Rom_CS=1;
	lcd_cs1=0;
	initial_lcd();	
	clear_screen();    //clear all dots
	display_GB2312_string(0,1,"����");
	
	display_GB2312_string(2,1,"�ٶȣ�");  
	display_GB2312_string(4,1,"�ƶ���");
	
	while(1)
	{

		speed=RcvStr(0x90,0x40);
		key_value=key_scan();
//		clear_screen();	
		if(key_value==0x7e)
		{
			serial_one_send_byte(send_dat[0]);
			display_GB2312_string(0,40,"ǰ��");
		}
		else if(key_value==0xee)
		{
			serial_one_send_byte(send_dat[1]);
			display_GB2312_string(0,40,"����");
		}
		else if(key_value==0x7b)
		{
			serial_one_send_byte(send_dat[2]);
			display_GB2312_string(0,40,"��ת");
		}
		else if(key_value==0x77)
		{
			serial_one_send_byte(send_dat[3]);
			display_GB2312_string(0,40,"��ת");
		}
		num_4(2,40,speed);

	}
}
void num_4(unsigned char y, unsigned char x,unsigned int num)
{
	switch(num/1000)
	{
		case 0 : display_GB2312_string(y,x,"0");	break;
		case 1 : display_GB2312_string(y,x,"1");	break;
		case 2 : display_GB2312_string(y,x,"2");	break;
		case 3 : display_GB2312_string(y,x,"3");	break;
		case 4 : display_GB2312_string(y,x,"4");	break;
		case 5 : display_GB2312_string(y,x,"5");	break;
		case 6 : display_GB2312_string(y,x,"6");	break;
		case 7 : display_GB2312_string(y,x,"7");	break;
		case 8 : display_GB2312_string(y,x,"8");	break;
		case 9 : display_GB2312_string(y,x,"9");	break;
		default :break;
	}
	switch(num/100%10)
	{
		case 0 : display_GB2312_string(y,x+7,"0");	break;
		case 1 : display_GB2312_string(y,x+7,"1");	break;
		case 2 : display_GB2312_string(y,x+7,"2");	break;
		case 3 : display_GB2312_string(y,x+7,"3");	break;
		case 4 : display_GB2312_string(y,x+7,"4");	break;
		case 5 : display_GB2312_string(y,x+7,"5");	break;
		case 6 : display_GB2312_string(y,x+7,"6");	break;
		case 7 : display_GB2312_string(y,x+7,"7");	break;
		case 8 : display_GB2312_string(y,x+7,"8");	break;
		case 9 : display_GB2312_string(y,x+7,"9");	break;
		default :break;
	}
	switch(num/10%10)
	{
		case 0 : display_GB2312_string(y,x+15,"0");	break;
		case 1 : display_GB2312_string(y,x+15,"1");	break;
		case 2 : display_GB2312_string(y,x+15,"2");	break;
		case 3 : display_GB2312_string(y,x+15,"3");	break;
		case 4 : display_GB2312_string(y,x+15,"4");	break;
		case 5 : display_GB2312_string(y,x+15,"5");	break;
		case 6 : display_GB2312_string(y,x+15,"6");	break;
		case 7 : display_GB2312_string(y,x+15,"7");	break;
		case 8 : display_GB2312_string(y,x+15,"8");	break;
		case 9 : display_GB2312_string(y,x+15,"9");	break;
		default :break;
	}
	switch(num%10)
	{
		case 0 : display_GB2312_string(y,x+22,"0");	break;
		case 1 : display_GB2312_string(y,x+22,"1");	break;
		case 2 : display_GB2312_string(y,x+22,"2");	break;
		case 3 : display_GB2312_string(y,x+22,"3");	break;
		case 4 : display_GB2312_string(y,x+22,"4");	break;
		case 5 : display_GB2312_string(y,x+22,"5");	break;
		case 6 : display_GB2312_string(y,x+22,"6");	break;
		case 7 : display_GB2312_string(y,x+22,"7");	break;
		case 8 : display_GB2312_string(y,x+22,"8");	break;
		case 9 : display_GB2312_string(y,x+22,"9");	break;
		default :break;
	}
}
/********************
 //����ɨ��
**********************/
unsigned char key_scan(void)		
{
	unsigned char key_value=0xff,left_num=0xfe,for_num=0;
	for(for_num=0;for_num<4;for_num++)
	{
		P3=left_num;
		if((P3!=left_num)||P42==0||P44==0)
		{
			key_value=(P3&0x3f)|((P4&0x04)<<4)|((P4&0x10)<<3);
			break;
		}
		left_num=_crol_(left_num,1);
	}
	return key_value;
}