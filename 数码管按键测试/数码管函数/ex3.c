
#include <STC15F2K60S2.H>

#include<intrins.h>

unsigned char code tab[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
unsigned char dbuf[8] = {0,1,2,3,4,5,6,7};  //显示缓冲区
unsigned char dspcom=0;

unsigned char key_scan();

void display(void);
void delay(void)//延时函数
{
    unsigned char i,j;
    for(i=0; i<200; i++)
    {
        for(j=0; j<1; j++)
        {						
          ;
        }
    } 
}
void main()//主函数
{

	unsigned char aa;

	while(1)
		{ 
		  aa=key_scan(); 
		display();
		 delay();
		}
}

unsigned char key_scan() //按键扫描函数
{
   unsigned char key_value=0xff,n,i;
   n=0xfe;
   for(i=1;i<=4;i++)
	{
            P3=n;     //赋值当前扫描行的扫描值
		 if(P3!=n)//判断当前行是否有按键按下
		 {
          key_value=P3;//获取到键值
		  break;	
		 }
		 n=(n<<1)|(n>>7);//若当前行没有按键，循环左移得到下一行的扫描值
	}
	return key_value; //返回键值
}

//显示函数
void display(void)
{   
	P0=(P0&0x1f)|(dspcom<<5);
	P2=tab[dbuf[dspcom]];

  if(++dspcom == 8)
	  {
	   dspcom = 0;
    }
}