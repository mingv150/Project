/****************************************************************************
Copyright (C), 2014, Mingv150, All rights reserved
FileName: /Layer/Module/TestMain.h
Description:  
Author:  mingv150@163.com
Version:  
Changelog: 
*****************************************************************************/

#ifndef _TestMain_H_
#define _TestMain_H_

/*For common use*/
/*Macro Definition*/
#define TestMain_PURPOSE 1
/*Data Type Specification*/
typedef enum
{
	TestMain_NORMALMOD = 0,
	TestMain_SETUPMOD,
	TestMain_FACTORYMOD,
	//添加模式请在这边添加
	TestMain_MAXMOD
}t_Sysmod;


#ifdef _TestMain_C_
/*For local users*/
/*Macro Definition*/
/*Data Type Specification*/
#else
/*For external users*/
/*Macro Definition*/
/*Data Type Specification*/
/*Interface Declarations*/
#endif

#endif	