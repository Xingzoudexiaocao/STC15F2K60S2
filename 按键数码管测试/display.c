#include"stc15f2k60s2.h"
#include "display.h"

unsigned char code dis[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0xff,0x00};		 //�������ʾ��
extern unsigned char dis_num[]={11,11,11,11,11,11,11,11};									 //ȫ����������ܻ�����

unsigned char a=0;	 //����ȫ�ֱ�������ʾ����ܶ�ѡ

void display()
{
  ch_=(ch_&0xf8)|a;				  //ֻ�ı����λ���ָ���λ����
  cmd=1;						  //��573ѡ�񿪹�
  dat_=dis[dis_num[a]];			  //����ʾ�뷢�͵�dat
  cmd=0;						  //�ر�573ѡ�񿪹�
  a++;							  //��ѡ���ּ�1ѡ����һ��
  if(a>=8)						  //�ж϶�ѡ�����Ƿ����
  a=0;							  //�����0��ʼ

  
}