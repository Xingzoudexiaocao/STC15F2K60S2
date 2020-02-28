#include <STC15F2K60S2.H>
#include<intrins.h>

#define uchar unsigned char
#define uint unsigned int
 
uchar code CW[8]={0x09,0x01,0x03,0x02,0x06,0x04,0x0c,0x08};    
 
void motor_cw(void);
void delay500us(void);

void delaynms(uint aa)
{
  uchar bb;
  while(aa--)
  {
   for(bb=0;bb<115;bb++)     
   {
    ;
   }
  }
}

void delay500us(void)
{
  int j;
  for(j=0;j<57;j++)
  {
   ;
  }
}


void motor_cw(void)
{
  uchar i,j;
  for(j=0;j<8;j++)                 
  {
    for(i=0;i<8;i++)  
   {
      P1=CW[i];
      delaynms(12);   
    }
  }
}


void main(void)
{
 uchar r;
 uchar N=64;
          
 while(1)
 {
  
    for(r=0;r<N;r++)
       motor_cw();  
 }
  
}      
 