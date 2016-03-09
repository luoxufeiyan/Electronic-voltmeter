#include<reg52.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<intrins.h>
#include<stdio.h>


#define  uint unsigned int  
#define  uchar unsigned char 

  
sbit rs = P2^5;     //����&&ָ���������        
sbit rw = P2^6;     //��д������     
sbit en = P2^7;     //ʹ�ܿ�����

sbit OE  = P2^2;    
sbit EOC = P2^1; 
sbit ST  = P2^0;
sbit CLK = P3^3;
sbit LED = P3^7;    //Define LED

uchar AD_DATA;   //����IN0��IN1��ADת���������
uchar str[16];
float t;


uchar code table[]={'0','1','2','3','4','5','6','7','8','9'};
uchar code table1[]="Current Voltage"; 

/**********��ʱ����************/ 

void delay(uint n)       //��ʱ����                        
{       uint x,y;       
    for(x=n;x>0;x--)          
        for(y=110;y>0;y--); 
 } 
 
 
/**********1602��ʼ��ģ��************/   
void lcd_wcom(uchar com)  //1602д�����                  
{       
    rs=0;           //ѡ��ָ��Ĵ���     
    rw=0;           //ѡ��д       
    P0=com;         //������������P2       
    delay(5);       //��ʱһС�������1602׼����������      
    en=1;           //ʹ���ߵ�ƽ�仯����������1602��8λ���ݿ�      
    en=0;  
}   
void lcd_wdat(uchar dat)        //1602д���ݺ���        
{       
    rs=1;        //ѡ�����ݼĴ���     
    rw=0;        //ѡ��д       
    P0=dat;      //��Ҫ��ʾ����������P2       
    delay(5);    //��ʱһС�������1602׼����������      
    en=1;        //ʹ���ߵ�ƽ�仯����������1602��8λ���ݿ�  
    en=0;  
}   
void lcd_init()        //1602��ʼ������        
{       
    lcd_wcom(0x38);    //8λ���ݣ�˫�У�5*7����            
    lcd_wcom(0x0c);    //������ʾ�����ع�꣬��겻��˸       
    lcd_wcom(0x06);    //��ʾ��ַ��������дһ�����ݺ���ʾλ������һλ 
    lcd_wcom(0x01);    //����  
} 

/***********��ʾģ��***************/
void LCD_Display(uchar *str)
{   
	uchar i;    
	for(i=0;i<strlen(str);i++)     
	{    
		lcd_wdat(str[i]);
		delay(10);
	}
}

void display(uchar x)
{
     uchar i,j,k;
	 i=x/100;
	 j=x/10%10;
	 k=x%10;
	 lcd_wcom(0x80+0x40);
	 lcd_wdat(table[i]);

	 lcd_wcom(0x80+0x41);
	 lcd_wdat(table[j]);

	 lcd_wcom(0x80+0x42);
	 lcd_wdat(table[k]);
}
/****Bluetooth***/
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
		return 1;
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
/***End of Bluetooth***/
 
/*********ϵͳ��ʼ��***********/ 
void init() 
{ 
	EA = 1;      //????
	SCON=0x50;
	PCON=0;
	TMOD = 0x22; //?????T0????
	TH0=254;     //??T0????CLK 500KHZ?? 
	TL0=254; 
	TR0=1;       //?????T0 
	TH1=0xfd;		 //??T1??2  9600BPS
	TL1=0xfd;
	TR1=1;
	ET0=1;
	ES=1;	
	ST=0; 
	OE=0; 
  LED=0;      //Ĭ�Ϲر�LED
} 
 
  
/***********ADת������ͨ��IN0 **********/ 
void AD() 
{ 
	ST=0; 
	delay(10); 
	ST=1;             //����ADת��
	delay(10); 
	ST=0; 
	while(0==EOC); 
	OE=1; 
	AD_DATA=P1; 
	OE=0; 
} 
/****************************/ 

void main()
{
   init();
   lcd_init();
   lcd_wcom(0x80);
   LCD_Display(table1);
   while(1)
   {
	   AD();
	   t=AD_DATA*5.0/255;       
	   sprintf((char*)str,"%.2f  ",t);     
	   lcd_wcom(0x80+0x40);
	   LCD_Display(str);
		 Uart_Putstring(str,4);// bluebooth
		 if(getbyte() == 0x33)
			 {
		 		 LED = 0;
				 delay(2);
			 }
		 else if(getbyte() == 0xaa)
			 {
		 		 LED = 1;
				 delay(2);
			 }
   }
}
void t0(void) interrupt 1 using 0 
{ 
    CLK=~CLK; 
}
