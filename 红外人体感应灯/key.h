#include <STC15F2K60S2.H>
#include <intrins.h>

#define Shot_Dat 0x11
#define Long_Dat 0x22

static unsigned char Key_Scan(void);
unsigned char key(void);