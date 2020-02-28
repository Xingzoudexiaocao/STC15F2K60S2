#include "key.h"

unsigned char Key_Scan(void)
{
	static unsigned char state = 0;
	static unsigned char LAST_Dat = 0xff,NOW_Dat = 0xff;
	static unsigned int Long_Num = 0;
	unsigned char Key_Value = 0xff;
	NOW_Dat = key();
	switch(state)
	{
	case 0 :  	if(NOW_Dat != LAST_Dat)			  //�ж��Ƿ��а�������
					state = 1;
				break;
	case 1 :  	if(NOW_Dat == LAST_Dat)			  //ȥ����ֹ���Ž���
					state = 2;
			  	else
			  		state = 0;
				break;
	case 2 :  	if(NOW_Dat == LAST_Dat)			  //����ǳ�������3��������Ƿ��ض̰���ֵ
					state = 3;
			  	else
			 	{
			  	state = 0;
				Key_Value = Shot_Dat;
				}
				break;
	case 3 : 	if(NOW_Dat == LAST_Dat)			 //������ʱ������趨ʱ�䣬
				{								 //���س�����ֵ�����򷵻ض̰���ֵ
					Long_Num++;
					if(Long_Num > 2000)
					{
						Long_Num = 0;
						state = 4;
						Key_Value = Long_Dat;
					}
				}
				else
				{
					state = 0 ;
					Key_Value = Shot_Dat;
				}	
				break;
	case 4 : 	if(NOW_Dat != LAST_Dat)			//��ⳤ�������Ƿ��ɿ�
				{
					state = 0;
				}
				break;
	}				  
	LAST_Dat = NOW_Dat;
	return Key_Value;
}
unsigned char key(void)
{
	unsigned char For_Num=0;
	unsigned char value = 0xff;
	unsigned char left_num = 0xfe;

	for(For_Num=0;For_Num<4;For_Num++)
	{
		P3 = left_num;
		if((P3 != left_num)||(P42==0)||(P44==0))
		{
	  		value = (P3&0x3f) | ((P42&0x04)<<4) | ((P44&0x10)<<3);
			break;
		}
		left_num=_crol_(left_num,1);
	}
	return value;
}