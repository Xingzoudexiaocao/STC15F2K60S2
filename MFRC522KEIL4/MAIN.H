#include <string.h>

//////////////////////////////////
//端口定义
/////////////////////////////////////////////////////////////////////
//MFRC522
sbit     MF522_RST  =    P1^4;//4                   //RC500片选
sbit     MF522_NSS  =    P1^0;//2
sbit     MF522_SCK  =    P1^1;//1
sbit     MF522_SI   =    P1^2;//0
sbit		 MF522_SO   =    P1^3;//3

//按键
sbit     KEY1  =    P3^2;
sbit     KEY2  =    P3^3;
sbit     KEY3  =    P3^4;
sbit	 KEY4  =    P3^5;

#define Menu 0x00
#define Rfid_Button1 0x0e
#define Rfid_Button2 0x0d
#define Rfid_Button3 0x0b
#define Rfid_Button4 0x07
  

//按
/////////////////////////////////////////////////////////////////////
//函数原型
/////////////////////////////////////////////////////////////////////
void InitializeSystem();  

/*************常量定义****************/

#define MAIN_Fosc                   11059200L       //主时钟,晶振频率不同的直接在这里修改频率
#define serial_one_read_max         32              //定义缓存区的长度
#define serial_one_baud_rate        9600L           //波特率,只需在这里修改波特率
#define Timer1_Reload               (65536UL -(MAIN_Fosc / 4 / serial_one_baud_rate))    //用定时器1做波特率发生器，计算定时器1的重装值    
#define u8 unsigned char
