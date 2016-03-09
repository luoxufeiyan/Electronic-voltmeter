#ifndef LCD_h
#define LCD_h
#include<reg52.h>
#include<Var.h>
/**********延时函数************/ 

void delay(uint n)                 
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
#endif
