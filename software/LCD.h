#ifndef LCD_h
#define LCD_h
#include<reg52.h>
#include<Var.h>
/**********��ʱ����************/ 

void delay(uint n)                 
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
#endif
