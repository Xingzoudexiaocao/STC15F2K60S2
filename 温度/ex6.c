# include"stc15f2k60s2.h"
#include"intrins.h"
#include "18b20.h"
unsigned char key_scan();
unsigned char code dis[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,
						  0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xef,0x39};
unsigned char  display_num[]={0xff,0xff,0xff,0xff};

void delay(void)//��ʱ����
{
    unsigned char i,j,k;
    for(i=0; i<20; i++)
    {
        for(j=0; j<15; j++)
        {						
            for(k=0; k<15; k++);
        }
    } 
}






void main(void)
{  	  	 
    

	while(1)  
		{
			temperature = rd_temperature();  //���¶�      
			temperature = temperature *6.25;
			display_num[0]=temperature/1000;	
			display_num[1]=temperature/100%10;
			display_num[2]=temperature/10%10;
			display_num[3]=temperature%10;


//			P03 = 0;
		 P00=0;
		 P01=0;
		 P02=0;
		 P2=dis[20];
		 delay();
//		 P03 = 1;
		 P00=0;
		 P01=0;
		 P02=1;
		 P2=dis[display_num[0]];
//		 P03 = 0;
		 delay();
//		 P03 = 1;
		 P00=1;
		 P01=0;
		 P02=1;
		 P2=dis[display_num[1]+ 9];
//		 P03 = 0;
		 delay();
//		 P03 = 1;
		 P00=0;
		 P01=1;
		 P02=1;
		 P2=dis[display_num[2]];
		 delay();
		 P00=1;
		 P01=1;
		 P02=1;
		 P2=dis[display_num[3]];
//		 P03 = 0;
		 delay();
	    }
}

void Timer0Init(void)		//50����@11.0592MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0x4C;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1;
	EA = 1; 
}
void time0() interrupt 1
{
	if(time0_num>=4000)
		time0_num=0;
	else
		time0_num++;
}


