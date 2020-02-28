#include <STC15F2K60S2.H>

#define MAIN_Fosc                   11059200L       //主时钟,晶振频率不同的直接在这里修改频率
#define serial_one_read_max         32              //定义缓存区的长度
#define serial_one_baud_rate        9600L           //波特率,只需在这里修改波特率
#define Timer1_Reload               (65536UL -(MAIN_Fosc / 4 / serial_one_baud_rate))    //用定时器1做波特率发生器，计算定时器1的重装值    


unsigned char  serial_one_read_data[2];
unsigned char  serial_one_read_count=0;

void serial_one_init();
void serial_one_send_string(unsigned char *dat);
void serial_one_send_length_string(unsigned char *dat,unsigned char length);