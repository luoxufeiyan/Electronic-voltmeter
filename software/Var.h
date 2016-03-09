#ifndef VAR_h
#define VAR_h
#include<reg52.h>


#define  uint unsigned int  
#define  uchar unsigned char 

sbit rs = P2^5;        
sbit rw = P2^6;          
sbit en = P2^7;        
sbit OE  = P2^2;  
sbit EOC = P2^1; 
sbit ST  = P2^0;
sbit CLK = P3^3;
sbit LED = P3^4;
uchar AD_DATA;   //保存IN0和IN1经AD转换后的数据
uchar str[16] = {0};
uint  i = 0;
float SUM_AD = 0;
float t;

uchar code table[]={'0','1','2','3','4','5','6','7','8','9'};
uchar code title[]="Voltage"; 
 #endif