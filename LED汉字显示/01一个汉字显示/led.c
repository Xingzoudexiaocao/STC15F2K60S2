#include <reg51.h>

extern unsigned char code  table0[32] ;
//������������������һ�����ֵ�32���ֽڵ���ʾ��

/*



*/

void sendonebyte(unsigned char  x)
{
  char i;	
	for(i=0;i<8;i++)          //ѭ��8�ΰѱ��봫�������� 	
	{
		x=x>>1;       //����һλ��ȡ�����ֽڵ����λ 
		DS=CY;            //�����ֽڵ����λ����R
		SHCP=0;          //����������595�������� 
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
    //ѡ��0��
    //�͵�0�еĵ�һ���ֽ�
     //�͵�0�еĵڶ����ֽ�
    //��ʱ

     //ѡ��1��
    //�͵�1�еĵ�һ���ֽ�
     //�͵�1�еĵڶ����ֽ�
    //��ʱ

}


