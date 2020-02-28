#include <STC15F2K60S2.H>
#include<intrins.h>
void delay(void)//延时函数
{
    unsigned char i,j,k;
    for(i=0; i<200; i++)
    {
        for(j=0; j<1; j++)
        {						
            for(k=0; k<200; k++);
        }
    } 
}
void main()//主函数
{
	unsigned char aa;
			 aa=0xfe;

	while(1)
		{ 
		 aa=_crol_(aa,1);
		 P1=aa;
		 delay();
		}
}

