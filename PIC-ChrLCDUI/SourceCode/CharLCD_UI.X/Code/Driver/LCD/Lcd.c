/****************************************************************************
Copyright (C), 2014, Mingv150, All rights reserved
FileName: /Driver/Lcd/Lcd.c
Description:  
Author:  mingv150@163.com
Version:  
Changelog: 
*****************************************************************************/

#define _Lcd_C_
/****************************************************************************
Global header file:
*****************************************************************************/
#include <htc.h>
#include "../../Common.h"
#include "../../Model.h"
#include "../../oopc.h"

/****************************************************************************
Local header file:
*****************************************************************************/
#include "../BSP/BspGpio.h"
#include "Lcd.h"

/****************************************************************************
Global Data Structure:
*****************************************************************************/


/****************************************************************************
Function: Name
Description:
Input:
Output:
Notes:
*****************************************************************************/
void Lcd_Delay(u16 t)
{
	u8 i;
 	while(t--)
 	{
  		for(i=0;i<125;i++);
 	}  
}


/****************************************************************************
Function: Name
Description:
Input:
Output:
Notes:
*****************************************************************************/
static void SendByte(u8 Dbyte)
{
     u8 i;
     u8 temp;
     for(i=0;i<8;i++)
     {
     	BspGpio_SCLKC(BspGpio_OUTPUT);
		BspGpio_SCLKOUT(BspGpio_LOW);

        BspGpio_SDATC(BspGpio_OUTPUT);
        temp = (Dbyte&0x80) ? 1 : 0 ;
	  BspGpio_SDATOUT(temp);
        BspGpio_SCLKOUT(BspGpio_HIGH);

        BspGpio_SCLKOUT(BspGpio_LOW);
        Dbyte=Dbyte<<1;       
     }
}


/****************************************************************************
Function: Name
Description:
Input:
Output:
Notes:
*****************************************************************************/
static u8 ReceiveByte(void)
{
     u8 i,temp1,temp2;
     temp1 = 0;
     temp2 = 0;
     for(i=0;i<8;i++)
     {
           temp1=temp1<<1;
           BspGpio_SCLKC(BspGpio_OUTPUT);
           BspGpio_SCLKOUT(BspGpio_LOW);

           BspGpio_SCLKOUT(BspGpio_HIGH);

           BspGpio_SCLKOUT(BspGpio_LOW);
           BspGpio_SDATC(BspGpio_INPUT);
           if(BspGpio_SDATIN()) temp1++;
     }

     for(i=0;i<8;i++)
     {
           temp2=temp2<<1;
           BspGpio_SCLKC(BspGpio_OUTPUT);
           BspGpio_SCLKOUT(BspGpio_LOW);

           BspGpio_SCLKOUT(BspGpio_HIGH);

           BspGpio_SCLKOUT(BspGpio_LOW);
           BspGpio_SDATC(BspGpio_INPUT);
           if(BspGpio_SDATIN()) temp2++;
     }

     return ((0xf0&temp1)+(0x0f&(temp2>>4));
}


/****************************************************************************
Function: Name
Description:
Input:
Output:
Notes:
*****************************************************************************/
static void Lcd_CheckBusy(void)
{
     do   SendByte(0xfc);         
     while(0x80&ReceiveByte());       //BF(.7)=1 Busy
}


/****************************************************************************
Function: Name
Description:
Input:
Output:
Notes:
*****************************************************************************/
static u8 Lcd_ReadData(void)
{
    u8 dat;
    SendByte(0xfe);   
    dat = ReceiveByte();

    return dat;
}


/****************************************************************************
Function: Name
Description:
Input:
Output:
Notes:
*****************************************************************************/
static void Lcd_WriteCmd(u8 Cbyte )
{
  BspGpio_CSC(BspGpio_OUTPUT);
  BspGpio_CSOUT(BspGpio_HIGH);
  Lcd_CheckBusy();
  SendByte(0xf8);            
  SendByte(0xf0&Cbyte);      
  SendByte(0xf0&Cbyte<<4);   
  BspGpio_CSOUT(BspGpio_LOW);
}


/****************************************************************************
Function: Name
Description:
Input:
Output:
Notes:
*****************************************************************************/
static void Lcd_WriteData(u8 Dbyte )
{
  BspGpio_CSC(BspGpio_OUTPUT);
  BspGpio_CSOUT(BspGpio_HIGH);
  Lcd_CheckBusy();
  SendByte(0xfa);              
  SendByte(0xf0&Dbyte);    
  SendByte(0xf0&Dbyte<<4); 
  BspGpio_CSOUT(BspGpio_LOW);
}


/****************************************************************************
Function: Name
Description:
Input:
Output:
Notes:
*****************************************************************************/
void Lcd_Init(void)
{
  ANSEL = 0x00;
  Lcd_Delay(10);
  Lcd_WriteCmd(0x30);       
  Lcd_Delay(1);
  Lcd_WriteCmd(0x30);          
  Lcd_Delay(1);
  Lcd_WriteCmd(0x0c);          
  Lcd_Delay(1);
  Lcd_WriteCmd(0x01);   
  Lcd_Delay(10);
}


/****************************************************************************
Function: Name
Description:
Input:
Output:
Notes:
*****************************************************************************/
void Lcd_StrDisp(u8 x,u8 y,const u8 *s)
{
  const u8 addr_tab[]=
  {
  0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,
  0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,
  0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,
  0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,
  };
  Lcd_WriteCmd(addr_tab[8*x+y]);
  while(*s>0)
  {
    Lcd_WriteData(*s);   
    s++;   
  }
}


/****************************************************************************
Function: Name
Description:
Input:
Output:
Notes:
*****************************************************************************/
void Lcd_ClrScreen(void)
{
  Lcd_WriteCmd(CLEAR_SCREEN);
}