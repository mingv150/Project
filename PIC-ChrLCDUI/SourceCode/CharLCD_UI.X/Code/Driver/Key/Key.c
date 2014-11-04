/****************************************************************************
Copyright (C), 2014, Mingv150, All rights reserved
FileName: Key.c
Description:  
Author:  mingv150@163.com
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
    {0,0,0,0,0,0},
    0,
    0
};


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
            temp = EVENT_KEYUPS;
            break;

        case KEY1LONG:
            temp = EVENT_KEYUPL;
            break;

        case KEY2SHORT:
            temp = EVENT_KEYDOWNS;
            break;

        case KEY2LONG:
            temp = EVENT_KEYDOWNL;
            break;

        case KEY3SHORT:
            temp = EVENT_KEYLEFTS;
            break;

        case KEY3LONG:
            temp = EVENT_KEYLEFTL;
            break;

        case KEY4SHORT:
            temp = EVENT_KEYSETUPS;
            break;

        case KEY4LONG:
            temp = EVENT_KEYSETUPL;
            break;

        case KEY5SHORT:
            temp = EVENT_KEYMINUSS;
            break;

        case KEY5LONG:
            temp = EVENT_KEYMINUSL;
            break; 

        case KEY6SHORT:
            temp = EVENT_KEYRIGHTS;
            break;

        case KEY6LONG:
            temp = EVENT_KEYRIGHTL;
            break;

        case KEY7SHORT:
            temp = EVENT_NONE;
            break;

        case KEY7LONG:
            temp = EVENT_NONE;
            break;

        case KEY8SHORT:
            temp = EVENT_KEYENTERS;
            break;

        case KEY8LONG:
            temp = EVENT_KEYENTERL;
            break;

        case KEY9SHORT:
            temp = EVENT_KEYPLUSS;
            break;

        case KEY9LONG:
            temp = EVENT_KEYPLUSL;
            break;

        case KEY10SHORT:
            temp = EVENT_NONE;
            break;

        case KEY10LONG:
            temp = EVENT_NONE;
            break;

        case KEY11SHORT:
            temp = EVENT_NONE;
            break;

        case KEY11LONG:
            temp = EVENT_NONE;
            break;

        case KEY12SHORT:
            temp = EVENT_NONE;
            break;

        case KEY12LONG:
            temp = EVENT_NONE;
            break;

        default:
            return;
    }

    /*Key_PutEventToTail*/
    if(st_Event_EventMage.FiFoHead == (st_Event_EventMage.FiFoTail+1)%EVENT_FIFOLEN)
    {
        return;
    }
    st_Event_EventMage.EventFiFo[st_Event_EventMage.FiFoTail] = temp;
    st_Event_EventMage.FiFoTail = (++st_Event_EventMage.FiFoTail)%EVENT_FIFOLEN;

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
    u8 i;
    
    BspGpio_KEY1C(BspGpio_OUTPUT);
    BspGpio_KEY2C(BspGpio_OUTPUT);
    BspGpio_KEY3C(BspGpio_OUTPUT);
    BspGpio_KEY4C(BspGpio_OUTPUT);
    BspGpio_KEY5C(BspGpio_INPUT);
    BspGpio_KEY6C(BspGpio_INPUT);
    BspGpio_KEY7C(BspGpio_INPUT);

    BspGpio_KEY1OUT(BspGpio_HIGH);
    BspGpio_KEY2OUT(BspGpio_LOW);
    BspGpio_KEY3OUT(BspGpio_LOW);
    BspGpio_KEY4OUT(BspGpio_LOW);
    for(i=0;i<20;i++)
    {}
    if(BspGpio_KEY5IN())
        SETBIT(State,0);
    if(BspGpio_KEY6IN())
        SETBIT(State,1);
    if(BspGpio_KEY7IN())
        SETBIT(State,2);

    BspGpio_KEY2OUT(BspGpio_HIGH);
    BspGpio_KEY1OUT(BspGpio_LOW);
    BspGpio_KEY3OUT(BspGpio_LOW);
    BspGpio_KEY4OUT(BspGpio_LOW);
    for(i=0;i<20;i++)
    {}
    if(BspGpio_KEY5IN())
        SETBIT(State,3);
    if(BspGpio_KEY6IN())
        SETBIT(State,4);
    if(BspGpio_KEY7IN())
        SETBIT(State,5);

    BspGpio_KEY3OUT(BspGpio_HIGH);
    BspGpio_KEY1OUT(BspGpio_LOW);
    BspGpio_KEY2OUT(BspGpio_LOW);
    BspGpio_KEY4OUT(BspGpio_LOW);
    for(i=0;i<20;i++)
    {}
    if(BspGpio_KEY5IN())
        SETBIT(State,6);
    if(BspGpio_KEY6IN())
        SETBIT(State,7);
    if(BspGpio_KEY7IN())
        SETBIT(State,8);

    BspGpio_KEY4OUT(BspGpio_HIGH);
    BspGpio_KEY1OUT(BspGpio_LOW);
    BspGpio_KEY2OUT(BspGpio_LOW);
    BspGpio_KEY3OUT(BspGpio_LOW);
    for(i=0;i<20;i++)
    {}
    if(BspGpio_KEY5IN())
        SETBIT(State,9);
    if(BspGpio_KEY6IN())
        SETBIT(State,10);
    if(BspGpio_KEY7IN())
        SETBIT(State,11);

    BspGpio_KEY1OUT(BspGpio_LOW);
    BspGpio_KEY2OUT(BspGpio_LOW);
    BspGpio_KEY3OUT(BspGpio_LOW);
    BspGpio_KEY4OUT(BspGpio_LOW);

    return State;
}


/****************************************************************************
Function: Name
Description:
Input:
Output:
*****************************************************************************/
u16 Key_Debounce(u16 KeyState)
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
        return 0;
    }
    else if(ShortDebounce < SHORTDEBOUNCE)
    {
        ShortDebounce++;
        return 0;
    }
    else
    {
        //ShortDebounce = 0;
        KeyState = KeyStateBuf^0xffff;
        TrigKey = KeyState&(KeyState^ContKey);
        ContKey = KeyState;

        /*Just execute One time*/
        return TrigKey;

        #if 0 
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
        #endif
    }
}