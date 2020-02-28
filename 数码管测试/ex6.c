#include"stc15f2k60s2.h"
#include"intrins.h"

unsigned char code dis[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

void delay(void)//ÑÓÊ±º¯Êý
{
    unsigned char i,j,k;
    for(i=0; i<20; i++)
    {
        for(j=0; j<2; j++)
        {						
            for(k=0; k<20; k++);
        }
    } 
}

void main(void)
{  	  

	while(1)  
		{
		 P00=0;
		 P01=0;
		 P02=0;


		 P2=dis[9];
		 delay();

		 P00=1;
		 P01=0;
		 P02=0;
		 P2=dis[8];
		 delay();
		 P00=0;
		 P01=1;
		 P02=0;	 P2=dis[7];
		 delay();
		 P00=1;
		 P01=1;
		 P02=0;	  P2=dis[6];
		 delay();
		 P00=0;
		 P01=0;
		 P02=1;	   P2=dis[5];
		 delay();
		 P00=1;
		 P01=0;
		 P02=1;	  P2=dis[4];
		 delay();
		 P00=0;
		 P01=1;
		 P02=1;	   P2=dis[3];
		 delay();
		 P00=1;
		 P01=1;
		 P02=1;	  P2=dis[2];
		 delay();
		 delay();

	
	
	    }
}




