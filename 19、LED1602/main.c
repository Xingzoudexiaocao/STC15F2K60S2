/**************************************************
���ܣ���Һ�����ĵ�һ����ʾ"www.ccit.js.cn"
      �ڶ�����ʾ"2012-4-16"
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
    while(*P)				//�ж��ַ����Ƿ����
		lcd_dat(*P++);		//��ʾ"www.jsnm.edu.cn"
	lcd_cmd(0xc0);			//��ʾ�ڵڶ���
    P=str2;
	while(*P)
     	lcd_dat(*P++);
	while(1);
}
