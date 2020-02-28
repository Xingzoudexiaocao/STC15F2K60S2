#include <reg51.h>

extern unsigned char code  table0[32] ;
//？？？？？？？定义一个汉字的32个字节的显示码

/*



*/

void sendonebyte(unsigned char  x)
{
  char i;	
	for(i=0;i<8;i++)          //循环8次把编码传给锁存器 	
	{
		x=x>>1;       //右移一位，取出该字节的最低位 
		DS=CY;            //将该字节的最低位传给R
		SHCP=0;          //将数据移入595，上升沿 
		SHCP=1;                    
	}     
    
}



/*



*/
void diaplay(unsigned char *p)
{
    int i;
    for(i=0;i<16;i++)
   {
      sendonebyte(*(p+2*i+1));
      sendonebyte(p[2*i]);
      STCP=1;
      SHCP=I;
      STCP=0;

      //YANSHI
       for(x=0;x<40;x++);
   }
    //选第0行
    //送第0行的第一个字节
     //送第0行的第二个字节
    //延时

     //选第1行
    //送第1行的第一个字节
     //送第1行的第二个字节
    //延时

}


