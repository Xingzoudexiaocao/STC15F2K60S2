
#include <STC15F2K60S2.H>

#include<intrins.h>

unsigned char code tab[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
unsigned char dbuf[8] = {0,1,2,3,4,5,6,7};  //��ʾ������
unsigned char dspcom=0;

unsigned char key_scan();

void display(void);
void delay(void)//��ʱ����
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
void main()//������
{

	unsigned char aa;

	while(1)
		{ 
		  aa=key_scan(); 
		display();
		 delay();
		}
}

unsigned char key_scan() //����ɨ�躯��
{
   unsigned char key_value=0xff,n,i;
   n=0xfe;
   for(i=1;i<=4;i++)
	{
            P3=n;     //��ֵ��ǰɨ���е�ɨ��ֵ
		 if(P3!=n)//�жϵ�ǰ���Ƿ��а�������
		 {
          key_value=P3;//��ȡ����ֵ
		  break;	
		 }
		 n=(n<<1)|(n>>7);//����ǰ��û�а�����ѭ�����Ƶõ���һ�е�ɨ��ֵ
	}
	return key_value; //���ؼ�ֵ
}

//��ʾ����
void display(void)
{   
	P0=(P0&0x1f)|(dspcom<<5);
	P2=tab[dbuf[dspcom]];

  if(++dspcom == 8)
	  {
	   dspcom = 0;
    }
}