/**************************************************
功能：在液晶屏的第一行显示"www.ccit.js.cn"
      第二行显示"2012-4-16"
*****************************************************/
#include"stc15fxxxx.h"
extern void init_lcd(void);
extern lcd_cmd(u8 cmd);
extern lcd_dat(u8 dat);
void main(void)
{
    u8 *P;
    u8 str1[]="www.jsnm.edu.cn";
    u8 str2[]="2018-9-22";
	init_lcd();
    P=str1;
    while(*P)				//判断字符串是否结束
		lcd_dat(*P++);		//显示"www.jsnm.edu.cn"
	lcd_cmd(0xc0);			//显示在第二行
    P=str2;
	while(*P)
     	lcd_dat(*P++);
	while(1);
}
