#include <STC15F2K60S2.H>
#include "main.h"
#include "LCD12864.h"
#include "mfrc522.h"	

unsigned char code DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 		 //卡片密钥  
unsigned char idata MLastSelectedSnr[4]; 										//卡片UID存放Cache
unsigned char idata RevBuffer[16];  											//读取数据存放
unsigned char data SerBuffer[16];												//发送数据存放

struct User{				  //定义用户数据
	int Apparel_Orders;
	int Price;
	int Inventory;
	int Shipments;
}User_Data;


void InitializeSystem()				 //初始化天线
{
	P0 = 0xFF; P1 = 0xFF; P3 = 0xFF; 

			
	PcdReset();		  //RC522初始化
	PcdAntennaOff(); //关闭天线
	PcdAntennaOn();  //打开天线
	M500PcdConfigISOType( 'A' ); //设置卡类型为A
	delay_1ms(10);	
	
}


void LCD12864_Display_num(unsigned char x,unsigned char y,unsigned int z){		 //LCD在指定位置显示四位数字

	unsigned char display_code[5];

	display_code[0] = z/1000+48;
	display_code[1] = z/100%10+48;
	display_code[2] = z/10%10+48;
	display_code[3] = z%10+48;
	display_code[4] = '\0';
	lcd_pos(x,y);		
	write_str(display_code);
}


void Rx_Data_Parsing(unsigned char *Rx_Data){		 //接收数据解析
	User_Data.Apparel_Orders = Rx_Data[0];
	User_Data.Apparel_Orders = User_Data.Apparel_Orders <<8;
	User_Data.Apparel_Orders |= Rx_Data[1];

	User_Data.Price = Rx_Data[2];
	User_Data.Price = User_Data.Price <<8;
	User_Data.Price |= Rx_Data[3];

	User_Data.Inventory = Rx_Data[4];
	User_Data.Inventory = User_Data.Inventory <<8;
	User_Data.Inventory |= Rx_Data[5];

	User_Data.Shipments = Rx_Data[6];
	User_Data.Shipments = User_Data.Shipments <<8;
	User_Data.Shipments |= Rx_Data[7];
}

void Tx_Data_Parsing(){		   //发送数据解析

	SerBuffer[0] =   (User_Data.Apparel_Orders >>8) & 0x00ff;
	SerBuffer[1] =   User_Data.Apparel_Orders & 0x00ff;

	SerBuffer[2] =   (User_Data.Price >>8) & 0x00ff;
	SerBuffer[3] =   User_Data.Price & 0x00ff;

	SerBuffer[4] =   (User_Data.Inventory >>8) & 0x00ff;
	SerBuffer[5] =   User_Data.Inventory & 0x00ff;

	SerBuffer[6] =   (User_Data.Shipments >>8) & 0x00ff;
	SerBuffer[7] =   User_Data.Shipments & 0x00ff;

}

unsigned char key_scan(){			  //获取独立键值 
	unsigned char key_value = 0x00;
	key_value = key_value | KEY4;
	key_value = key_value << 1;

	key_value = key_value | KEY3;
	key_value = key_value << 1;

	key_value = key_value | KEY2;
	key_value = key_value << 1;

	key_value = key_value | KEY1;

	return key_value;
}
void main()
{   
	
	unsigned char count_for = 0;
	unsigned char key_state = 1;
	bit set_flag = 0;
    InitializeSystem( );
	lcd_init(); 
	lcd_pos(0,2);				//指定显示位置
	write_str("欢迎使用");	   	//显示数据
	lcd_pos(1,0);			
	write_str("服装仓储管理系统");
	lcd_pos(3,0);			
	write_str("录入");
	lcd_pos(3,6);				
	write_str("读取");
	User_Data.Apparel_Orders = 0;
	User_Data.Price	= 0;
	User_Data.Inventory = 0;
	User_Data.Shipments = 0; 

    while(1)
    {	
		switch(key_scan()){

		 	case Rfid_Button1 : 
				while(1){
					lcd_init(); 
					lcd_pos(0,0);			
					write_str("服装类目");
					lcd_pos(1,0);			
					write_str("单价:   ");
					lcd_pos(2,0);			
					write_str("库存:   ");
					lcd_pos(3,0);		
					write_str("出货量: ");
					
					while(1){
						delay_1ms(1000);			   //按键防抖与放缓显示刷新
						set_flag = ~set_flag;		   //设置标识判断位
						if(Rfid_Button2 == key_scan())	   //跳出设置进入写
						{
							break;
						}
						LCD12864_Display_num(0,5,User_Data.Apparel_Orders);
						LCD12864_Display_num(1,5,User_Data.Price);
						LCD12864_Display_num(2,5,User_Data.Inventory);
						LCD12864_Display_num(3,5,User_Data.Shipments);

						switch(key_state){
							case 1 :if(Rfid_Button3 == key_scan())			  //判断加减
										User_Data.Apparel_Orders++; 
									else if(Rfid_Button4 == key_scan())
										User_Data.Apparel_Orders--;
									if(User_Data.Apparel_Orders < 0)		   //数据越界判断
										User_Data.Apparel_Orders = 0;
									else if(User_Data.Apparel_Orders >9999)
										User_Data.Apparel_Orders =9999;
									if(set_flag){					//显示设置标志
										lcd_pos(0,4);				//指定显示位置
										write_str(":_");
									}else{							//清除设置标志
										lcd_pos(0,4);				//指定显示位置
										write_str(": ");
									}
									lcd_pos(1,4);				//清除其他位设置标志
									write_str("  ");
									lcd_pos(2,4);				
									write_str("  ");
									lcd_pos(3,4);			
									write_str("  ");
									LCD12864_Display_num(0,5,User_Data.Apparel_Orders);
						  		break;
							case 2 :if(Rfid_Button3 == key_scan())				//判断加减
										User_Data.Price++; 
									else if(Rfid_Button4 == key_scan())
										User_Data.Price--;
									if(User_Data.Price < 0)						//数据越界判断
										User_Data.Price = 0;
									else if(User_Data.Price >9999)
										User_Data.Price =9999;
									if(set_flag){
										lcd_pos(1,4);			
										write_str(" _");
									}else{
										lcd_pos(1,4);		
										write_str("  ");
									}
									lcd_pos(0,4);				//清除其他位设置标志
									write_str(": ");
									lcd_pos(2,4);			
									write_str("  ");
									lcd_pos(3,4);			
									write_str("  ");
									LCD12864_Display_num(1,5,User_Data.Price);
						  		break;
							case 3 :if(Rfid_Button3 == key_scan())			 //判断加减
										User_Data.Inventory++; 
									else if(Rfid_Button4 == key_scan())
										User_Data.Inventory--;
									if(User_Data.Inventory < 0)				//数据越界判断
										User_Data.Inventory = 0;
									else if(User_Data.Inventory >9999)
										User_Data.Inventory =9999;
									if(set_flag){
										lcd_pos(2,4);			
										write_str(" _");
									}else{
										lcd_pos(2,4);	
										write_str("  ");
									}
									lcd_pos(0,4);				//清除其他位设置标志
									write_str(": ");
									lcd_pos(1,4);			
									write_str("  ");
									lcd_pos(3,4);			
									write_str("  ");
									LCD12864_Display_num(2,5,User_Data.Inventory);
						  		break;
							case 4 :if(Rfid_Button3 == key_scan())	  //判断加减
										User_Data.Shipments++; 
									else if(Rfid_Button4 == key_scan())
										User_Data.Shipments--;
									if(User_Data.Shipments < 0)
										User_Data.Shipments = 0;
									else if(User_Data.Shipments >9999)
										User_Data.Shipments =9999;
									if(set_flag){
										lcd_pos(3,4);		
										write_str(" _");
									}else{
										lcd_pos(3,4);		
										write_str("  ");
									}
									lcd_pos(0,4);			//清除其他位设置标志
									write_str(": ");
									lcd_pos(1,4);		
									write_str("  ");
									lcd_pos(2,4);			
									write_str("  ");
									LCD12864_Display_num(3,5,User_Data.Shipments);
							  	break;
							default : break;
							}
							if(Rfid_Button1 == key_scan())	   //进入设置
							{
								key_state = key_state % 4;
								key_state++;
							}		
						}
						
						lcd_init(); 	
						lcd_pos(0,0);			
						write_str("请将卡片放置在读");
						lcd_pos(1,0);			
						write_str("卡器上");
						lcd_pos(2,0);			
						write_str("准备写入....");
	
//						while(0 != PcdHalt())；
						while(0 != PcdRequest(RFCfgReg,&MLastSelectedSnr[0]));	    //读取卡片类型	
				 		while(0 != PcdAnticoll(&MLastSelectedSnr[0]));			   // 读取卡片UID
						while(0 != PcdSelect(MLastSelectedSnr));				  //选中卡片
						while(0 != PcdAuthState(PICC_AUTHENT1A,4,DefaultKey,MLastSelectedSnr));	   //发送密钥
						Tx_Data_Parsing();	//解析要发送的数据
						while(0 != PcdWrite(4,&SerBuffer));							 //指定位置写入数据

		   				lcd_init(); 			//初始菜单
						lcd_pos(0,0);	
						write_str("写入成功");
						delay_1ms(15000);
						lcd_init(); 
						lcd_pos(0,2);		
						write_str("欢迎使用");
						lcd_pos(1,0);		
						write_str("服装仓储管理系统");
						lcd_pos(3,0);			
						write_str("录入");
						lcd_pos(3,6);		
						write_str("读取");
						break;
					}
				break;

			case Rfid_Button2 : 
		   			lcd_init(); 	
					lcd_pos(0,0);			
					write_str("请将卡片放置在读");
					lcd_pos(1,0);			
					write_str("卡器上");
					lcd_pos(2,0);			
					write_str("准备读取....");

//					while(0 != PcdHalt());
					while(0 != PcdRequest(RFCfgReg,&MLastSelectedSnr[0]));		 //读取卡片类型
			 		while(0 != PcdAnticoll(&MLastSelectedSnr[0]));				 //读取卡片UID
					while(0 != PcdSelect(MLastSelectedSnr));						//选中卡片
					while(0 != PcdAuthState(PICC_AUTHENT1A,4,DefaultKey,MLastSelectedSnr));	//发送密钥
					while(0 != PcdRead(4,&RevBuffer));							//读取指定位置的数据
					lcd_init(); 	
					lcd_pos(0,0);				//指定显示位置
					write_str("读卡成功！");
					delay_1ms(5000);
					Rx_Data_Parsing(&RevBuffer);
					lcd_init(); 
					lcd_pos(0,0);			
					write_str("服装类目 :");
					lcd_pos(1,0);			
					write_str("单价     :");
					lcd_pos(2,0);			
					write_str("库存     :");
					lcd_pos(3,0);			
					write_str("出货量   :");
					LCD12864_Display_num(0,5,User_Data.Apparel_Orders);		//显示读取数据
					LCD12864_Display_num(1,5,User_Data.Price);
					LCD12864_Display_num(2,5,User_Data.Inventory);
					LCD12864_Display_num(3,5,User_Data.Shipments);
					delay_1ms(15000);
					lcd_init(); 					//初始菜单
					lcd_pos(0,2);				
					write_str("欢迎使用");
					lcd_pos(1,0);			
					write_str("服装仓储管理系统");
					lcd_pos(3,0);			
					write_str("录入");
					lcd_pos(3,6);			
					write_str("读取");
					break;

			default : break;
		}
	}

}