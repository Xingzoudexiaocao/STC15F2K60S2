#include "display.h"
#include "main.h"
u8 code display_dat [] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0x8e,0xc6,0xbf};
void display( unsigned char *cache )
{
	static u8 for_num = 0 , display_crol = 0x01;
	P0 = 0xff;
	P2 = P2 & 0x1f | 0xc0;
	P0 = display_crol;
	P2 = P2 & 0x1f;
	P0 = 0xff;

	P2 = P2 & 0x1f | 0xe0;
	P0 = display_dat[ cache[ for_num ] ];
	P2 = P2 & 0x1f;

	display_crol = _crol_(display_crol,1);
	if(++for_num == 8)
	{
		for_num = 0;
	}

}

