#include"stc15f2k60s2.h"
#include "display.h"

unsigned char code dis[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0xff,0x00};		 //数码管显示码
extern unsigned char dis_num[]={11,11,11,11,11,11,11,11};									 //全局声明数码管缓冲区

unsigned char a=0;	 //定义全局变量，表示数码管段选

void display()
{
  ch_=(ch_&0xf8)|a;				  //只改变低三位保持高五位不变
  cmd=1;						  //打开573选择开关
  dat_=dis[dis_num[a]];			  //将显示码发送到dat
  cmd=0;						  //关闭573选择开关
  a++;							  //段选数字加1选择下一个
  if(a>=8)						  //判断段选数字是否溢出
  a=0;							  //溢出从0开始

  
}