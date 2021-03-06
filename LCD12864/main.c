/************************************************************
功能描述：单片机12864液晶显示 
************************************************************/
#include"stc15fxxxx.h"
#include<stdlib.h>
bit  delay_1ms(u16 x);
bit lcd_busy(void);
bit  lcd_pos(u8 X,u8 Y);
bit lcd_init();
bit write_str(u8 *c);
/************************************************************
功能描述：在LCD12864上指定位置显示指定内容
入口参数：无
出口参数：无
************************************************************/
void main()
{
    lcd_init();       			//LCD初始化
	delay_1ms(3);	  			//延时

	lcd_pos(3,1);				//指定显示位置
	write_str("——牧童工作室");

	P0&=0xaf;
	P2=P2&0x1f|0xa0;
    serial_one_init();//串口初始化
	Timer0Init();
    while(1)
    {
		;
    }
}