/****************************************************************************
Copyright (C), 2014, Mingv150, All rights reserved
FileName: Key.c
Description:  
Author:  
Version:  
Changelog: 
*****************************************************************************/

#define _Key_C_
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
#include "Key.h"


/****************************************************************************
Global Data Structure:
*****************************************************************************/
static t_EventMage st_Event_EventMage = 
{
    {0,0,0,0},
    0,
    0
};


/****************************************************************************
Function: Name
Description:
Input:
Output:
*****************************************************************************/
void Key_PutEventToTail(u8 key)
{
    if(st_Event_EventMage.FiFoHead == (st_Event_EventMage.FiFoTail+1)%EVENT_FIFOLEN)
    {
        return;
    }
    st_Event_EventMage.EventFiFo[st_Event_EventMage.FiFoTail] = key;
    st_Event_EventMage.FiFoTail = (++st_Event_EventMage.FiFoTail)%EVENT_FIFOLEN;
}


/*******************************************************************************
*Function:
*Description:
*Input:
*Output:
*******************************************************************************/
u8 Key_GetEvent(void)
{
    u8 key;

    if(st_Event_EventMage.FiFoHead == st_Event_EventMage.FiFoTail)
    {
        return EVENT_NONE;
    }
    key = st_Event_EventMage.EventFiFo[st_Event_EventMage.FiFoHead];       
    st_Event_EventMage.FiFoHead = (++st_Event_EventMage.FiFoHead)%EVENT_FIFOLEN;
    return key;
}


/****************************************************************************
Function: Name
Description:
Input:
Output:
*****************************************************************************/
void Key_PutEvent(u16 KeyValue)
{
    u8 temp;
    
    switch(KeyValue)
    {
        case KEY1SHORT:
            temp = EVENT_KEY7S;
            break;

        case KEY1LONG:
            temp = EVENT_KEY7S;
            break;

        case KEY2SHORT:
            temp = EVENT_KEY7S;
            break;

        case KEY2LONG:
            temp = EVENT_KEY7S;
            break;

        case KEY3SHORT:
            temp = EVENT_KEYDOWNS;
            break;

        case KEY3LONG:
            temp = EVENT_KEYDOWNL;
            break;

        case KEY4SHORT:
            temp = EVENT_KEYPLUSS;
            break;

        case KEY4LONG:
            temp = EVENT_KEYPLUSL;
            break;

        case KEY5SHORT:
            temp = EVENT_KEYMINUSS;
            break;

        case KEY5LONG:
            temp = EVENT_KEYMINUSL;
            break; 

        case KEY6SHORT:
            temp = EVENT_KEY7S;
            break;

        case KEY6LONG:
            temp = EVENT_KEY7S;
            break;

        case KEY7SHORT:
            temp = EVENT_KEYUPS;
            break;

        case KEY7LONG:
            temp = EVENT_KEYUPL;
            break;

        case KEY8SHORT:
            temp = EVENT_KEY8S;
            break;

        case KEY8LONG:
            temp = EVENT_KEY8L;
            break;

        case KEY9SHORT:
            temp = EVENT_KEY9S;
            break;

        case KEY9LONG:
            temp = EVENT_KEY9L;
            break;

        case KEY10SHORT:
            temp = EVENT_KEY7S;
            break;

        case KEY10LONG:
            temp = EVENT_KEY7S;
            break;

        case KEY11SHORT:
            temp = EVENT_KEYRIGHTS;
            break;

        case KEY11LONG:
            temp = EVENT_KEYRIGHTL;
            break;

        case KEY12SHORT:
            temp = EVENT_KEYLEFTS;
            break;

        case KEY12LONG:
            temp = EVENT_KEYLEFTL;
            break;

            
        default:
            return;
    }

    Key_PutEventToTail(temp);
}


/*******************************************************************************
*Function:
*Description:
*Input: KeyState = 8 Key 0->down 1->up
*Output:
*******************************************************************************/
u16 Key_Scan(void)
{
    u16 State = 0;

    BspGpio_KEY1C(BspGpio_INPUT);
    BspGpio_KEY2C(BspGpio_INPUT);
    BspGpio_KEY3C(BspGpio_INPUT);
    BspGpio_KEY4C(BspGpio_INPUT);
    BspGpio_KEY5C(BspGpio_OUTPUT);
    BspGpio_KEY6C(BspGpio_OUTPUT);
    BspGpio_KEY7C(BspGpio_OUTPUT);

    BspGpio_KEY5OUT(BspGpio_LOW);
    BspGpio_KEY6OUT(BspGpio_HIGH);
    BspGpio_KEY7OUT(BspGpio_HIGH);

    if(BspGpio_KEY1IN())
        SETBIT(State,0);
    if(BspGpio_KEY2IN())
        SETBIT(State,1);
    if(BspGpio_KEY3IN())
        SETBIT(State,2);
    if(BspGpio_KEY4IN())
        SETBIT(State,3);
 

    BspGpio_KEY5OUT(BspGpio_HIGH);
    BspGpio_KEY6OUT(BspGpio_LOW);
    BspGpio_KEY7OUT(BspGpio_HIGH);

    if(BspGpio_KEY1IN())
        SETBIT(State,4);
    if(BspGpio_KEY2IN())
        SETBIT(State,5);
    if(BspGpio_KEY3IN())
        SETBIT(State,6);
    if(BspGpio_KEY4IN())
        SETBIT(State,7);

    BspGpio_KEY5OUT(BspGpio_HIGH);
    BspGpio_KEY6OUT(BspGpio_HIGH);
    BspGpio_KEY7OUT(BspGpio_LOW);

    if(BspGpio_KEY1IN())
        SETBIT(State,8);
    if(BspGpio_KEY2IN())
        SETBIT(State,9);
    if(BspGpio_KEY3IN())
        SETBIT(State,10);
    if(BspGpio_KEY4IN())
        SETBIT(State,11);

    return State;
}


/****************************************************************************
Function: Name
Description:
Input:
Output:
*****************************************************************************/
void Key_Debounce(u16 KeyState)
{
    u16 TrigKey = 0x0000;
    static u16 KeyStateBuf = 0x0000;
    static u16 ContKey = 0x0000;
    static u16 ContKeyBuf = 0x0000;
    static u8 ShortDebounce;
    static u16 LongDebounce;

    if(KeyState != KeyStateBuf)
    {
        KeyStateBuf = KeyState;
        ShortDebounce = 0;
    }
    else if(ShortDebounce < SHORTDEBOUNCE)
    {
        ShortDebounce++;
    }
    else
    {
        //ShortDebounce = 0;
        KeyState = KeyStateBuf^0xffff;
        TrigKey = KeyState&(KeyState^ContKey);
        ContKey = KeyState;

        /*Just execute One time*/
        Key_PutEvent(TrigKey);
        
        if(ContKey != ContKeyBuf)
        {
            ContKeyBuf = ContKey;
            LongDebounce = 0;
        }
        else if(LongDebounce < LONGDEBOUNCE)
        {
            LongDebounce++;
        }
        else
        {
            /**/
            //LongDebounce = LongDebounce>>1;
            //LongDebounce = LONGDEBOUNCE - 50;
            LongDebounce = 0;  
            SETBIT(ContKey,7);
            Key_PutEvent(ContKey);
        }
    }
}