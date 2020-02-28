#include <STC15F2K60S2.H>

#define MAIN_Fosc                   11059200L       //��ʱ��,����Ƶ�ʲ�ͬ��ֱ���������޸�Ƶ��
#define serial_one_read_max         32              //���建�����ĳ���
#define serial_one_baud_rate        9600L           //������,ֻ���������޸Ĳ�����
#define Timer1_Reload               (65536UL -(MAIN_Fosc / 4 / serial_one_baud_rate))    //�ö�ʱ��1�������ʷ����������㶨ʱ��1����װֵ    


unsigned char  serial_one_read_data[2];
unsigned char  serial_one_read_count=0;

void serial_one_init();
void serial_one_send_string(unsigned char *dat);
void serial_one_send_length_string(unsigned char *dat,unsigned char length);