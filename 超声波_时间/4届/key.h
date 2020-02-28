#include <STC15F2K60S2.H>

#define	 S7 0x01
#define	 S6 0x02
#define	 S5 0x03
#define	 S4 0x04
#define Key_down_long 0x80
#define Key_up 0x00
unsigned char key( void );
unsigned char key_state( void );