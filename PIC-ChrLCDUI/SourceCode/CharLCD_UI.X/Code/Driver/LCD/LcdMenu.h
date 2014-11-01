/****************************************************************************
Copyright (C), 2014, Mingv150, All rights reserved
FileName: /Driver/Lcd/LcdMenu.h
Description:  
Author:  
Version:  
Changelog: 
*****************************************************************************/

#ifndef _LcdMenu_H_
#define _LcdMenu_H_

/*For common use*/
/*Macro Definition*/
#define LcdMenu_PURPOSE 1
/*Data Type Specification*/
typedef struct
{
	u16 PaperNum;
	u8 ShuaJiao;
	u8 ShuaLiao;
	u8 Startup;
	u8 Back;
}t_SetupParam;

typedef struct
{
	u8 num[3][10];
}
t_FactoryParam;

#ifdef _LcdMenu_C_
/*For local users*/
#define LcdMenu_COMPANYNAME "厦门电子科技公司"
#define LcdMenu_TELPHONE "05925100520"
/*Macro Definition*/
/*Data Type Specification*/
extern u8 u8_Timer_Flash;
#else
/*For external users*/
/*Macro Definition*/
/*Data Type Specification*/
/*Interface Declarations*/
void LcdMenu_Factory(u8 KeyEvent,t_FactoryParam *pParam);
void LcdMenu_Setup(u8 KeyEvent,t_SetupParam *pParam);
void LcdMenu_Normal(u8 KeyEvent,u16 PaperNum);
void LcdMenu_Welcome(void);
#endif

#endif