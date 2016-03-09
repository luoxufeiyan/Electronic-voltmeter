#include<reg52.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<intrins.h>
#include<stdio.h>


#define  uint unsigned int  
#define  uchar unsigned char 

  
sbit rs = P2^5;     //数据&&指令控制总线        
sbit rw = P2^6;     //读写控制线     
sbit en = P2^7;     //使能控制线

sbit OE  = P2^2;    
sbit EOC = P2^1; 
sbit ST  = P2^0;
sbit CLK = P3^3;
sbit LED = P3^7;    //Define LED

uchar AD_DATA;   //保存IN0和IN1经AD转换后的数据
uchar str[16];
float t;


uchar code table[]={'0','1','2','3','4','5','6','7','8','9'};
uchar code table1[]="Current Voltage"; 

/**********延时函数************/ 

void delay(uint n)       //延时函数                        
{       uint x,y;       
    for(x=n;x>0;x--)          
        for(y=110;y>0;y--); 
 } 
 
 
/**********1602初始化模块************/   
void lcd_wcom(uchar com)  //1602写命令函数                  
{       
    rs=0;           //选择指令寄存器     
    rw=0;           //选择写       
    P0=com;         //把命令字送入P2       
    delay(5);       //延时一小会儿，让1602准备接收数据      
    en=1;           //使能线电平变化，命令送入1602的8位数据口      
    en=0;  
}   
void lcd_wdat(uchar dat)        //1602写数据函数        
{       
    rs=1;        //选择数据寄存器     
    rw=0;        //选择写       
    P0=dat;      //把要显示的数据送入P2       
    delay(5);    //延时一小会儿，让1602准备接收数据      
    en=1;        //使能线电平变化，数据送入1602的8位数据口  
    en=0;  
}   
void lcd_init()        //1602初始化函数        
{       
    lcd_wcom(0x38);    //8位数据，双列，5*7字形            
    lcd_wcom(0x0c);    //开启显示屏，关光标，光标不闪烁       
    lcd_wcom(0x06);    //显示地址递增，即写一个数据后，显示位置右移一位 
    lcd_wcom(0x01);    //清屏  
} 

/***********显示模块***************/
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
 
/*********系统初始化***********/ 
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
  LED=0;      //默认关闭LED
} 
 
  
/***********AD转换函数通道IN0 **********/ 
void AD() 
{ 
	ST=0; 
	delay(10); 
	ST=1;             //启动AD转换
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
