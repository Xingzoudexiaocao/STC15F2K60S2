# include"stc15f2k60s2.h"
#include"intrins.h"
unsigned char key_scan();
void delay(void)//��ʱ����
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

void main(void)
{  	  
	unsigned char aa;
	while(1)  
	{
	 aa=key_scan(); 
		 if(aa!=0xff) 
		 {
		 P00=0;
		 delay();
		 P00=~P00;
		 aa=0;
		 }
    }
}

unsigned char key_scan() //����ɨ�躯��
{
   unsigned char key_value=0xff,n,i;
   n=0xfe;
   for(i=1;i<=4;i++)
	{
            P3=n;     //��ֵ��ǰɨ���е�ɨ��ֵ1
		 if(P3!=n||P44==0||P42==0)//�жϵ�ǰ���Ƿ��а�������2
		 {
          key_value=((P3&0x3f)|((P4&0x10)<<3)|(P4&0x04)<<4);//��ȡ����ֵ3
				  break;	
		 }
		 n=(n<<1)|(n>>7);//����ǰ��û�а�����ѭ�����Ƶõ���һ�е�ɨ��ֵ
	}
	return key_value; //���ؼ�ֵ
}


