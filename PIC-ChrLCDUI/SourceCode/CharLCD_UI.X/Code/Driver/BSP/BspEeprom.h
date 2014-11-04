/****************************************************************************
Copyright (C), 2014, Mingv150, All rights reserved
FileName: /Layer/Module/BspEeprom.h
Description:  
Author:  mingv150@163.com
Version:  
Changelog: 
*****************************************************************************/

#ifndef _BspEeprom_H_
#define _BspEeprom_H_

/*For common use*/
/*Macro Definition*/
#define BspEeprom_PURPOSE 1
/*Data Type Specification*/

#ifdef _BspEeprom_C_
/*For local users*/
/*Macro Definition*/
/*Data Type Specification*/
#else
/*For external users*/
/*Macro Definition*/
/*Data Type Specification*/
/*Interface Declarations*/
void BspEeprom_SaveParam(u8 StartAddr,const u8 *pSrc,u8 Size);
void BspEeprom_GetParam(u8 StartAddr,u8 *pDes,u8 Size);
#endif

#endif