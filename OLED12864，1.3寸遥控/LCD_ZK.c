#include <intrins.h>
#include <STC15F2K60S2.H>
#include "IIC.h"

sbit lcd_sclk=P1^0;  /*接口定义:lcd_sclk就是D0*/
sbit lcd_sid=P1^1;   /*接口定义:lcd_sid就是D1*/
sbit lcd_rs=P1^2;    /*接口定义:lcd_rs就是DC*/
sbit lcd_cs1=P1^3;   /*接口定义:lcd_cs1就是OLED的片选CS1*/
sbit Rom_OUT=P1^4;   /*字库IC接口定义:Rom_OUT就是OUT*/
sbit Rom_CS=P1^5;    /*字库IC接口定义Rom_CS就是字库IC的CS2*/

#define uchar unsigned char 
#define uint unsigned int
#define ulong unsigned long
#define MAIN_Fosc                   11059200L       //主时钟,晶振频率不同的直接在这里修改频率
#define serial_one_read_max         32              //定义缓存区的长度
#define serial_one_baud_rate        9600L           //波特率,只需在这里修改波特率
#define Timer1_Reload               (65536UL -(MAIN_Fosc / 4 / serial_one_baud_rate))    //用定时器1做波特率发生器，计算定时器1的重装值    
uchar UART_Dat;
void serial_one_init()
{
    SCON |= 0x40;          //8位数据
    P_SW1 &=  ~0xc0;       //UART1 使用P30 P31口  默认
    TR1 = 0;               //关闭定时器
    AUXR &= ~0x01;      //S1 BRT Use Timer1;
    TMOD &= ~(1<<6);    //Timer1 set As Timer
    TMOD &= ~0x30;      //Timer1_16bitAutoReload;2
    AUXR |=  (1<<6);    //Timer1 set as 1T mode
    TH1 = (uchar)(Timer1_Reload >> 8);		//定时器高八位赋初值
    TL1 = (uchar)Timer1_Reload;			//定时器低八位赋初值
    TR1 = 1;    //打开定时器
    PS = 1;     //高优先级中断
    REN = 1;    //允许接收
    ES  = 1;    //允许中断
    EA = 1;     //允许全局中断
}
//串口1发送一个字节
void serial_one_send_byte(uchar dat)
{
    SBUF = dat;   //发送数据
    while(!TI);   //等待发送完毕
    TI=0;         //清除发送标志
}
/********************* 串口1中断函数************************/
void UART1_int (void) interrupt 4
{
    if(RI)
    {
        RI = 0;//清除接受标志
        UART_Dat = SBUF;//将数据存入数组
    }
}


/*写指令到LCD模块*/
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

/*写数据到LCD模块*/
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

/*延时*/
void delay(int n_ms)               
{
 int j,k;
 for(j=0;j<n_ms;j++)
 for(k=0;k<110;k++);
}


/*LCD模块初始化*/
void initial_lcd()
{
	lcd_cs1=0;
	Rom_CS = 1;
//	lcd_reset=0;        /*低电平复位*/
	delay(20);
	//lcd_reset=1;		    /*复位完毕*/
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
	transfer_command_lcd(0xb0 + column);   /*设置页地址*/
	transfer_command_lcd((((page+2) & 0xf0) >> 4) | 0x10);	/*设置列地址的高4位*/
	transfer_command_lcd(((page+2)& 0x0f) | 0x00);	/*设置列地址的低4位*/	
}

/*全屏清屏*/
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

/*显示16x16点阵图像、汉字、生僻字或16x16点阵的其他图标*/
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
			transfer_data_lcd(*dp);					/*写数据到LCD,每写完一个8位的数据后列地址自动加1*/
			dp++;
		}
		page++;
	}
	lcd_cs1=1;
}

/*显示8x16点阵图像、ASCII, 或8x16点阵的自造字符、其他图标*/
void display_graphic_8x16(uint page,uchar column,uchar *dp)
{
	uint i,j;
	lcd_cs1=0;	
	for(j=2;j>0;j--)
	{
		lcd_address(column,page);
		for (i=0;i<8;i++)
		{	
			transfer_data_lcd(*dp);					/*写数据到LCD,每写完一个8位的数据后列地址自动加1*/
			dp++;
		}
		page++;
	}
	lcd_cs1=1;
}

/****送指令到晶联讯字库IC***/
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

/****从晶联讯字库IC中取汉字或字符数据（1个字节）***/
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

/*从相关地址（addrHigh：地址高字节,addrMid：地址中字节,addrLow：地址低字节）中连续读出DataLen个字节的数据到 pBuff的地址*/
/*连续读取*/
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
			/*国标简体（GB2312）汉字在晶联讯字库IC中的地址由以下公式来计算：*/
			/*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0*/
			/*由于担心8位单片机有乘法溢出问题，所以分三部取地址*/
			fontaddr = (text[i]- 0xb0)*94; 
			fontaddr += (text[i+1]-0xa1)+846;
			fontaddr = (ulong)(fontaddr*32);
			
			addrHigh = (fontaddr&0xff0000)>>16;  /*地址的高8位,共24位*/
			addrMid = (fontaddr&0xff00)>>8;      /*地址的中8位,共24位*/
			addrLow = fontaddr&0xff;	     /*地址的低8位,共24位*/
			get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,32 );/*取32个字节的数据，存到"fontbuf[32]"*/
			display_graphic_16x16(y,x,fontbuf);/*显示汉字到LCD上，y为页地址，x为列地址，fontbuf[]为数据*/
			i+=2;
			x+=16;
		}
		else if(((text[i]>=0xa1) &&(text[i]<=0xa3))&&(text[i+1]>=0xa1))
		{						
			/*国标简体（GB2312）15x16点的字符在晶联讯字库IC中的地址由以下公式来计算：*/
			/*Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0*/
			/*由于担心8位单片机有乘法溢出问题，所以分三部取地址*/
			fontaddr = (text[i]- 0xa1)*94; 
			fontaddr += (text[i+1]-0xa1);
			fontaddr = (ulong)(fontaddr*32);
			
			addrHigh = (fontaddr&0xff0000)>>16;  /*地址的高8位,共24位*/
			addrMid = (fontaddr&0xff00)>>8;      /*地址的中8位,共24位*/
			addrLow = fontaddr&0xff;	     /*地址的低8位,共24位*/
			get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,32 );/*取32个字节的数据，存到"fontbuf[32]"*/
			display_graphic_16x16(y,x,fontbuf);/*显示汉字到LCD上，y为页地址，x为列地址，fontbuf[]为数据*/
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

			get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,16 );/*取16个字节的数据，存到"fontbuf[32]"*/
			
			display_graphic_8x16(y,x,fontbuf);/*显示8x16的ASCII字到LCD上，y为页地址，x为列地址，fontbuf[]为数据*/
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
	display_GB2312_string(0,1,"方向：");
	
	display_GB2312_string(2,1,"速度：");  
	display_GB2312_string(4,1,"制动：");
	
	while(1)
	{

		speed=RcvStr(0x90,0x40);
		key_value=key_scan();
//		clear_screen();	
		if(key_value==0x7e)
		{
			serial_one_send_byte(send_dat[0]);
			display_GB2312_string(0,40,"前进");
		}
		else if(key_value==0xee)
		{
			serial_one_send_byte(send_dat[1]);
			display_GB2312_string(0,40,"后退");
		}
		else if(key_value==0x7b)
		{
			serial_one_send_byte(send_dat[2]);
			display_GB2312_string(0,40,"左转");
		}
		else if(key_value==0x77)
		{
			serial_one_send_byte(send_dat[3]);
			display_GB2312_string(0,40,"右转");
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
 //按键扫描
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