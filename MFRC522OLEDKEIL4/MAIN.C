#include <STC15F2K60S2.H>
#include "main.h"
#include "oled.h"
#include "mfrc522.h"	

unsigned char code DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 		 //��Ƭ��Կ  
unsigned char idata MLastSelectedSnr[4]; 										//��ƬUID���Cache
unsigned char idata RevBuffer[16];  											//��ȡ���ݴ��
unsigned char data SerBuffer[16];												//�������ݴ��

struct User{				  //�����û�����
	int ID;
	int Vaccine;
	int Day;
}User_Data;


void InitializeSystem()				 //��ʼ������
{
	P0 = 0xFF; P1 = 0xFF; P3 = 0xFF; 

			
	PcdReset();		  //RC522��ʼ��
	PcdAntennaOff(); //�ر�����
	PcdAntennaOn();  //������
	M500PcdConfigISOType( 'A' ); //���ÿ�����ΪA	
	
}

//
//void LCD12864_Display_num(unsigned char x,unsigned char y,unsigned int z){		 //LCD��ָ��λ����ʾ��λ����
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


void Rx_Data_Parsing(unsigned char *Rx_Data){		 //�������ݽ���
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

void Tx_Data_Parsing(){		   //�������ݽ���

	SerBuffer[0] =   (User_Data.ID >>8) & 0x00ff;
	SerBuffer[1] =   User_Data.ID & 0x00ff;

	SerBuffer[2] =   (User_Data.Vaccine >>8) & 0x00ff;
	SerBuffer[3] =   User_Data.Vaccine & 0x00ff;

	SerBuffer[4] =   (User_Data.Day >>8) & 0x00ff;
	SerBuffer[5] =   User_Data.Day & 0x00ff;
}

unsigned char key_scan(){			  //��ȡ������ֵ 
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
					if(User_Data.Vaccine){					//��ʾ���ñ�־
						OLED_ShowString(80,4,"Yes",16);
					}else{
						OLED_ShowString(80,4,"No ",16);
					}
					OLED_ShowNum(60,6,User_Data.Day,4,16);
					while(1){
						delay_ms(100);			   //����������Ż���ʾˢ��
						set_flag = ~set_flag;		   //���ñ�ʶ�ж�λ
						if(Rfid_Button2 == key_scan())	   //�������ý���д
						{
							break;
						}
						switch(key_state){
							case 1 :if(Rfid_Button3 == key_scan())			  //�жϼӼ�
										User_Data.ID++; 
									else if(Rfid_Button4 == key_scan())
										User_Data.ID--;
									if(User_Data.ID < 0)		   //����Խ���ж�
										User_Data.ID = 9999;
									else if(User_Data.ID >9999)
										User_Data.ID =0;
									if(set_flag){					//��ʾ���ñ�־
										OLED_ShowString(32,2,":",16);
										OLED_ShowString(64,4,":",16);
										OLED_ShowString(32,6,":",16);
									}else{							//������ñ�־
										OLED_ShowString(32,2," ",16);
									}
									OLED_ShowNum(60,2,User_Data.ID,4,16);
						  		break;
							case 2 :if(Rfid_Button3 == key_scan())				//�жϼӼ�
										User_Data.Vaccine = ++User_Data.Vaccine %2; 
									else if(Rfid_Button4 == key_scan())
										User_Data.Vaccine = ++User_Data.Vaccine %2;

									if(set_flag){					//��ʾ���ñ�־
										OLED_ShowString(64,4,":",16);
										OLED_ShowString(32,2,":",16);
										OLED_ShowString(32,6,":",16);
									}else{							//������ñ�־
										OLED_ShowString(64,4," ",16);
									}
									if(User_Data.Vaccine){					//��ʾ���ñ�־
										OLED_ShowString(80,4,"Yes",16);
									}else{
										OLED_ShowString(80,4,"No ",16);
									}							
						  		break;
							case 0 :if(Rfid_Button3 == key_scan())			 //�жϼӼ�
										User_Data.Day++; 
									else if(Rfid_Button4 == key_scan())
										User_Data.Day--;
									if(User_Data.Day < 0)				//����Խ���ж�
										User_Data.Day = 9999;
									else if(User_Data.Day >9999)
										User_Data.Day =0;
									if(set_flag){					//��ʾ���ñ�־
										OLED_ShowString(32,6,":",16);
										OLED_ShowString(32,2,":",16);
										OLED_ShowString(64,4,":",16);
									}else{							//������ñ�־
										OLED_ShowString(32,6," ",16);
									}
									OLED_ShowNum(60,6,User_Data.Day,4,16);
						  		break;
							default : break;
							}
							if(Rfid_Button1 == key_scan())	   //��������
							{
								key_state = ++key_state % 3;
							}		
						}
						OLED_Init();								   //wait card page
						OLED_Clear();
						OLED_ShowString(8,2,"Wait for CARDS ",16);


	
//						while(0 != PcdHalt())��
						while(0 != PcdRequest(RFCfgReg,&MLastSelectedSnr[0]));	    //��ȡ��Ƭ����	
				 		while(0 != PcdAnticoll(&MLastSelectedSnr[0]));			   // ��ȡ��ƬUID
						while(0 != PcdSelect(MLastSelectedSnr));				  //ѡ�п�Ƭ
						while(0 != PcdAuthState(PICC_AUTHENT1A,4,DefaultKey,MLastSelectedSnr));	   //������Կ
						Tx_Data_Parsing();	//����Ҫ���͵�����
						while(0 != PcdWrite(4,&SerBuffer));							 //ָ��λ��д������
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
					while(0 != PcdRequest(RFCfgReg,&MLastSelectedSnr[0]));		 //��ȡ��Ƭ����
			 		while(0 != PcdAnticoll(&MLastSelectedSnr[0]));				 //��ȡ��ƬUID
					while(0 != PcdSelect(MLastSelectedSnr));						//ѡ�п�Ƭ
					while(0 != PcdAuthState(PICC_AUTHENT1A,4,DefaultKey,MLastSelectedSnr));	//������Կ
					while(0 != PcdRead(4,&RevBuffer));							//��ȡָ��λ�õ�����
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
					if(User_Data.Vaccine){					//��ʾ���ñ�־
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