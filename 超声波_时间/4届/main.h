#include <STC15F2K60S2.H>
#include <INTRINS.H>

#define u8 unsigned char
#define u16 unsigned int

void Timer1Init(void); 
void Timer2Init( void );
void init( void );
void Delay5ms();
void CSB_Tx_40khz(void);	