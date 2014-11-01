/****************************************************************************
Copyright (C), 2014, Mingv150, All rights reserved
FileName: /Driver/Lcd/Lcd.h
Description:  
Author:  
Version:  
Changelog: 
*****************************************************************************/

#ifndef _Lcd_H_
#define _Lcd_H_

/*For common use*/
/*Macro Definition*/
#define Lcd_PURPOSE 1
/*Data Type Specification*/

#ifdef _Lcd_C_
/*For local users*/
/*Macro Definition*/
/*Data Type Specification*/
#else
/*For external users*/
/*Macro Definition*/
/*Data Type Specification*/
/*Interface Declarations*/
void Lcd_Init(void);
void Lcd_StrDisp(u8 x,u8 y,const u8 *s);
void delayms(u16 t);
void hanzi_Cur(u8 x,u8 y);
void Lcd_WriteCmd(u8 Cbyte );
#endif

#endif