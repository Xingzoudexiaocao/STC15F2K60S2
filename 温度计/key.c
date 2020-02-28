#include "key.h"

unsigned char key_state( void )
{
	static unsigned char new_dat = 0xff,old_dat = 0xff;
	static unsigned char state = 0;
	static unsigned int key_down_count = 0;
	unsigned char key_value = 0xff;
	new_dat = key();
	switch( state ) 
	{
	case 0 :  if( new_dat != old_dat )
			  		state = 1;
				break;
	case 1 :  if( new_dat == old_dat )
					state = 2;
			  else
			  		state = 0;
				break;
	case 2 :  if( new_dat == old_dat )
					state = 3;
			  else
			  {
			  		key_value = old_dat;
					state = 0;
			  }
				break;
	case 3 :  if( new_dat == old_dat )
			  {
			  		key_down_count++;
					if(key_down_count == 699)
					{
						key_down_count = 0;
						state = 4;
						key_value = old_dat | Key_down_long;
					}
			  }			
			  else
			  {
			  		key_down_count = 0;
					key_value = old_dat;
					state = 0;
			  }
				break;
	case 4 :  if( new_dat != old_dat )
			  	{
					state = 0;
					return Key_up;
				}
				break;
	}
	old_dat = new_dat;
	return key_value;
}

unsigned char key( void )
{
	unsigned char dat = 0xff;
	if( P30 == 0 )
		dat = S7;
	if( P31 == 0 )
		dat = S6;
	if( P32 == 0 )
		dat = S5;
	if( P33 == 0 )
		dat = S4;
	return dat;
}