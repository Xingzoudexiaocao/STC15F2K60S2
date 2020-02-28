#include <STC15F2K60S2.H>
#include "main.h"
#include "oled.h"
#include "mfrc522.h"	

unsigned char code DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 		 //卡片密钥  
unsigned char idata MLastSelectedSnr[4]; 										//卡片UID存放Cache
unsigned char idata RevBuffer[16];  											//读取数据存放
unsigned char data SerBuffer[16];												//发送数据存放

struct User{				  //定义用户数据
	int ID;
	int Vaccine;
	int Day;
}User_Data;


void InitializeSystem()				 //初始化天线
{
	P0 = 0xFF; P1 = 0xFF; P3 = 0xFF; 

			
	PcdReset();		  //RC522初始化
	PcdAntennaOff(); //关闭天线
	PcdAntennaOn();  //打开天线
	M500PcdConfigISOType( 'A' ); //设置卡类型为A	
	
}

//
//void LCD12864_Display_num(unsigned char x,unsigned char y,unsigned int z){		 //LCD在指定位置显示四位数字
//
//	unsigned char display_code[5];
//
//	display_code[0] = z/1000+48;
//	display_code[1] = z/100%10+48;
//	display_code[2] = z/10%10+48;
//	display_code[3] = z%10+48;
//	display_code[4] = '\0';
//	lcd_pos(x,y);		
//	write_str(display_code);
//}


void Rx_Data_Parsing(unsigned char *Rx_Data){		 //接收数据解析
	User_Data.ID = Rx_Data[0];
	User_Data.ID = User_Data.ID <<8;
	User_Data.ID |= Rx_Data[1];

	User_Data.Vaccine = Rx_Data[2];
	User_Data.Vaccine = User_Data.Vaccine <<8;
	User_Data.Vaccine |= Rx_Data[3];

	User_Data.Day = Rx_Data[4];
	User_Data.Day = User_Data.Day <<8;
	User_Data.Day |= Rx_Data[5];
}

void Tx_Data_Parsing(){		   //发送数据解析

	SerBuffer[0] =   (User_Data.ID >>8) & 0x00ff;
	SerBuffer[1] =   User_Data.ID & 0x00ff;

	SerBuffer[2] =   (User_Data.Vaccine >>8) & 0x00ff;
	SerBuffer[3] =   User_Data.Vaccine & 0x00ff;

	SerBuffer[4] =   (User_Data.Day >>8) & 0x00ff;
	SerBuffer[5] =   User_Data.Day & 0x00ff;
}

unsigned char key_scan(){			  //获取独立键值 
	unsigned char key_value = 0x00;
	if(KEY1 == 0){
		key_value = Rfid_Button1;
	}
	else if(KEY2 == 0){
		key_value = Rfid_Button2;
	}
	else if(KEY3 == 0){
		key_value = Rfid_Button3;
	}
	else if(KEY4 == 0){
		key_value = Rfid_Button4;
	}
	return key_value;
}
void main()
{   
	
	unsigned char count_for = 0;
	unsigned char key_state = 1;
	bit set_flag = 0;
    InitializeSystem( );
	OLED_Init();									 //home page
	OLED_Clear();
	OLED_ShowString(8,0,"Welcome To Use",16);		   
	OLED_ShowString(16,2,"Pig Info Sys",16);
	OLED_ShowString(0,6,"Write       Read",16);

	User_Data.ID = 0;
	User_Data.Vaccine = 0;
	User_Data.Day = 0; 

    while(1)
    {	
		switch(key_scan()){

		 	case Rfid_Button1 : 
				while(1){
					OLED_Init();								//write page
					OLED_Clear();
					OLED_ShowString(8,0,"Write Pig Info",16);
					OLED_ShowString(0,2,"ID",16);
					OLED_ShowString(0,4,"Vaccine",16);
					OLED_ShowString(0,6,"Day",16);
					OLED_ShowNum(60,2,User_Data.ID,4,16);
					if(User_Data.Vaccine){					//显示设置标志
						OLED_ShowString(80,4,"Yes",16);
					}else{
						OLED_ShowString(80,4,"No ",16);
					}
					OLED_ShowNum(60,6,User_Data.Day,4,16);
					while(1){
						delay_ms(100);			   //按键防抖与放缓显示刷新
						set_flag = ~set_flag;		   //设置标识判断位
						if(Rfid_Button2 == key_scan())	   //跳出设置进入写
						{
							break;
						}
						switch(key_state){
							case 1 :if(Rfid_Button3 == key_scan())			  //判断加减
										User_Data.ID++; 
									else if(Rfid_Button4 == key_scan())
										User_Data.ID--;
									if(User_Data.ID < 0)		   //数据越界判断
										User_Data.ID = 9999;
									else if(User_Data.ID >9999)
										User_Data.ID =0;
									if(set_flag){					//显示设置标志
										OLED_ShowString(32,2,":",16);
										OLED_ShowString(64,4,":",16);
										OLED_ShowString(32,6,":",16);
									}else{							//清除设置标志
										OLED_ShowString(32,2," ",16);
									}
									OLED_ShowNum(60,2,User_Data.ID,4,16);
						  		break;
							case 2 :if(Rfid_Button3 == key_scan())				//判断加减
										User_Data.Vaccine = ++User_Data.Vaccine %2; 
									else if(Rfid_Button4 == key_scan())
										User_Data.Vaccine = ++User_Data.Vaccine %2;

									if(set_flag){					//显示设置标志
										OLED_ShowString(64,4,":",16);
										OLED_ShowString(32,2,":",16);
										OLED_ShowString(32,6,":",16);
									}else{							//清除设置标志
										OLED_ShowString(64,4," ",16);
									}
									if(User_Data.Vaccine){					//显示设置标志
										OLED_ShowString(80,4,"Yes",16);
									}else{
										OLED_ShowString(80,4,"No ",16);
									}							
						  		break;
							case 0 :if(Rfid_Button3 == key_scan())			 //判断加减
										User_Data.Day++; 
									else if(Rfid_Button4 == key_scan())
										User_Data.Day--;
									if(User_Data.Day < 0)				//数据越界判断
										User_Data.Day = 9999;
									else if(User_Data.Day >9999)
										User_Data.Day =0;
									if(set_flag){					//显示设置标志
										OLED_ShowString(32,6,":",16);
										OLED_ShowString(32,2,":",16);
										OLED_ShowString(64,4,":",16);
									}else{							//清除设置标志
										OLED_ShowString(32,6," ",16);
									}
									OLED_ShowNum(60,6,User_Data.Day,4,16);
						  		break;
							default : break;
							}
							if(Rfid_Button1 == key_scan())	   //进入设置
							{
								key_state = ++key_state % 3;
							}		
						}
						OLED_Init();								   //wait card page
						OLED_Clear();
						OLED_ShowString(8,2,"Wait for CARDS ",16);


	
//						while(0 != PcdHalt())；
						while(0 != PcdRequest(RFCfgReg,&MLastSelectedSnr[0]));	    //读取卡片类型	
				 		while(0 != PcdAnticoll(&MLastSelectedSnr[0]));			   // 读取卡片UID
						while(0 != PcdSelect(MLastSelectedSnr));				  //选中卡片
						while(0 != PcdAuthState(PICC_AUTHENT1A,4,DefaultKey,MLastSelectedSnr));	   //发送密钥
						Tx_Data_Parsing();	//解析要发送的数据
						while(0 != PcdWrite(4,&SerBuffer));							 //指定位置写入数据
						OLED_ShowString(8,2,"Write Data .  ",16);
						delay_ms(100);
						OLED_ShowString(8,2,"Write Data .. ",16);
						delay_ms(100);
						OLED_ShowString(8,2,"Write Data ...",16);
						delay_ms(300);
						OLED_Init();
						OLED_Clear();
						OLED_ShowString(8,2,"Sucess ",16);
						delay_ms(300);
						OLED_Init();							   //home page
						OLED_Clear();
						OLED_ShowString(8,0,"Welcome To Use",16);
						OLED_ShowString(16,2,"Pig Info Sys",16);
						OLED_ShowString(0,6,"Write       Read",16);
						break;
					}
				break;

			case Rfid_Button2 : 

				  	OLED_Init();								   //wait read page
					OLED_Clear();
					OLED_ShowString(8,0,"Read Pig Info",16);
					OLED_ShowString(0,4,"Waiting for Card",16);

//					while(0 != PcdHalt());
					while(0 != PcdRequest(RFCfgReg,&MLastSelectedSnr[0]));		 //读取卡片类型
			 		while(0 != PcdAnticoll(&MLastSelectedSnr[0]));				 //读取卡片UID
					while(0 != PcdSelect(MLastSelectedSnr));						//选中卡片
					while(0 != PcdAuthState(PICC_AUTHENT1A,4,DefaultKey,MLastSelectedSnr));	//发送密钥
					while(0 != PcdRead(4,&RevBuffer));							//读取指定位置的数据
					OLED_Init();
					OLED_Clear();
					OLED_ShowString(16,0,"Read Sucess",16);
					delay_ms(500);
					Rx_Data_Parsing(&RevBuffer);

					OLED_Init();										  //Read page
					OLED_Clear();
					OLED_ShowString(8,0,"Read Pig Info",16);
					OLED_ShowString(0,2,"ID",16);
					OLED_ShowString(0,4,"Vaccine",16);
					OLED_ShowString(0,6,"Day",16);
					OLED_ShowNum(60,2,User_Data.ID,4,16);
					if(User_Data.Vaccine){					//显示设置标志
						OLED_ShowString(80,4,"Yes",16);
					}else{
						OLED_ShowString(80,4,"No ",16);
					}
					OLED_ShowNum(60,6,User_Data.Day,4,16);
					delay_ms(200);
					while(key_scan() == 0x00);
					delay_ms(100);
					OLED_Init();								   //home page
					OLED_Clear();
					OLED_ShowString(8,0,"Welcome To Use",16);
					OLED_ShowString(16,2,"Pig Info Sys",16);
					OLED_ShowString(0,6,"Write       Read",16);
					break;

			default : break;
		}
	}

}