/****************************************************************************
Copyright (C), Year, Mingv150, All rights reserved
FileName: Key.h
Description:  
Author:  mingv150@163.com
Version:  
Changelog: 
*****************************************************************************/

#ifndef _Key_H_
#define _Key_H_

/*Macro Definition*/
/*Data Type Specification*/
enum event
{
    EVENT_NONE = 0,
    EVENT_KEYUPS,
    EVENT_KEYUPL,
    EVENT_KEYDOWNS,
    EVENT_KEYDOWNL,
    EVENT_KEYLEFTS,
    EVENT_KEYLEFTL,
    EVENT_KEYRIGHTS,
    EVENT_KEYRIGHTL,
    EVENT_KEYPLUSS,
    EVENT_KEYPLUSL,
    EVENT_KEYMINUSS,
    EVENT_KEYMINUSL,
    EVENT_KEYSETUPS,
    EVENT_KEYSETUPL,
    EVENT_KEYENTERS,
    EVENT_KEYENTERL,
    /*添加按键事件请在这边添加*/
    EVENT_MAX
};

#ifdef _Key_C_
/*Macro Definition*/

/*修改抗抖动时间请在这边修改*/
/*短按键抗抖动时间*/
#define SHORTDEBOUNCE 5
/*长按键抗抖动时间*/
#define LONGDEBOUNCE  10000

/*添加或修改按键定义请在下面进行*/
#define KEY3SHORT 0x0001
#define KEY3LONG (KEY3SHORT+0x8000)

#define KEY8SHORT 0x0002
#define KEY8LONG (KEY2SHORT+0x8000)

#define KEY12SHORT 0x0004
#define KEY12LONG (KEY7SHORT+0x8000)

#define KEY2SHORT 0x0008
#define KEY2LONG (KEY1SHORT+0x8000)

#define KEY6SHORT 0x0010
#define KEY6LONG (KEY8SHORT+0x8000)

#define KEY11SHORT 0x0020
#define KEY11LONG (KEY6SHORT+0x8000)

#define KEY7SHORT 0x0040
#define KEY7LONG (KEY5SHORT+0x8000)

#define KEY5SHORT 0x0080
#define KEY5LONG (KEY4SHORT+0x8000)

#define KEY10SHORT 0x0100
#define KEY10LONG (KEY12SHORT+0x8000)

#define KEY1SHORT 0x0200
#define KEY1LONG (KEY11SHORT+0x8000)

#define KEY4SHORT 0x0400
#define KEY4LONG (KEY10SHORT+0x8000)

#define KEY9SHORT 0x0800
#define KEY9LONG (KEY9SHORT+0x8000)

#define EVENT_FIFOLEN 8

/*Data Type Specification*/
/*按键缓冲区*/
typedef struct EventMage
{
    u8 EventFiFo[EVENT_FIFOLEN];    
    u8 FiFoHead;
    u8 FiFoTail;
}t_EventMage;

#else
/*Macro Definition*/
/*Data Type Specification*/
/*Interface Declarations*/
u16 Key_Scan(void);
u16 Key_Debounce(u16 KeyState);
void Key_PutEvent(u16 KeyValue);
u8 Key_GetEvent(void);
#endif

#endif