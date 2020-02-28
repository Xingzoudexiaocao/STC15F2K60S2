
void display();	   //函数声明
sfr dat_ = 0x80;   //定义dat_为P0   八位数据线
sfr ch_ = 0xa0;	   //定义ch_为P2	选择线
sbit cmd = P2^3;   //定义cmd为P23	573控制线
	