#ifndef UART_h
#define UART_h
#include<reg52.h>
#include<Var.h>

#define TURE  1
#define FALSE 0


void Uart_Send_Byte(unsigned char one_byte)      
{
    TI = 0;                    
    SBUF = one_byte;
    while (TI == 0);
    TI = 0;                      
}

unsigned char Uart_Putstring( unsigned char *puts ,unsigned char len)
{
	unsigned char i;
	for(i=0;i<len;i++)
		Uart_Send_Byte( *(puts+i) );
		return TURE;
}


unsigned char getbyte (void)
{
	unsigned int i;
	i=0;
	while(RI==0)
	{
		i++;
		if(i>2000)return 0;		
	}
	RI=0;
	return 	SBUF;
}

void Uart_Getstring(unsigned char *strings,unsigned char len)
{
	unsigned int i;
	for(i=0;i<len;i++)
	  *(strings+i)=getbyte();
}
#endif