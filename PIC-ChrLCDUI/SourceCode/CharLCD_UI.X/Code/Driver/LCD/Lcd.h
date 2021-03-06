/****************************************************************************
Copyright (C), 2014, Mingv150, All rights reserved
FileName: /Driver/Lcd/Lcd.h
Description:  
Author:  mingv150@163.com
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
#define CLEAR_SCREEN 0x01   
#define AC_INIT   0x02  
#define CURSE_ADD  0x06 
#define FUN_MODE  0x30 
#define DISPLAY_ON  0x0c 
#define DISPLAY_OFF  0x08 
#define CURSE_DIR  0x14  
#define SET_CG_AC  0x40 
#define SET_DD_AC  0x80
/*Data Type Specification*/
#else
/*For external users*/
/*Macro Definition*/
/*Data Type Specification*/
/*Interface Declarations*/
void Lcd_Delay(u16 t);
void Lcd_Init(void);
void Lcd_StrDisp(u8 x,u8 y,const u8 *s);
void Lcd_ClrScreen(void);
#endif

#endif