/****************************************************************************
Copyright (C), 2014, Mingv150, All rights reserved
FileName: /Driver/BSP/BspSystem.c
Description:  
Author:  mingv150@163.com
Version:  
Changelog: 
*****************************************************************************/

#define _BspSystem_C_
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
#include "../../oopc.h"
#include "../Key/Key.h"
#include "BspSystem.h"


/****************************************************************************
Global Data Structure:
*****************************************************************************/
/*Configuration Bit Setting*/
//__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_OFF & MCLRE_OFF & CP_ON & CPD_OFF & BOREN_ON & IESO_OFF & FCMEN_ON & DEBUG_ON);
//__CONFIG(BOR4V_BOR40V & WRT_OFF)
//__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_OFF & MCLRE_OFF & CP_OFF & CPD_OFF & BOREN_ON & IESO_OFF & FCMEN_ON);;
//__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_OFF & MCLRE_OFF & CP_OFF & CPD_OFF & BOREN_ON & IESO_OFF & FCMEN_OFF);
__CONFIG(FOSC_INTOSCIO & WDTE_OFF & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF & BOREN_ON & IESO_ON & FCMEN_ON);

extern u8 u8_Timer_Flash;


/****************************************************************************
Function: Name
Description:
Input:
Output:
Notes:
*****************************************************************************/
void interrupt IRQ(void)
{
    u8 TimeBase0;
    u16 KeyTemp;

    if(T0IF&&T0IE)
    {
        TMR0=179;

        KeyTemp = Key_Scan();

        KeyTemp = Key_Debounce(KeyTemp);

        Key_PutEvent(KeyTemp);
        
        /*200*/
        if(++TimeBase0 == 100)
        {
        	TimeBase0 = 0;
        	u8_Timer_Flash = ~u8_Timer_Flash;
        }
        
        T0IF=0;
    }
}


/****************************************************************************
Function: Name
Description:
Input:
Output:
Notes:
*****************************************************************************/
void BspSystem_TmrInit(void)
{
    /*Enable Prescale*/
    /*Clock = 4915200Hz*/

    //OSTS = 1;
    SCS = 1;

    IRCF0 = 1;
    IRCF1 = 1;
    IRCF2 = 1;
    //OSCCON = 0xf1;
    //LCDCON = 0x00;

    PSA=0;
    T0CS=0;
    /*1:64*/
    PS0=1;PS1=0;PS2=1;  
    /*timeout time:997HZ*/
    TMR0=179;
    /*Tiemr0 int en*/
    T0IE=1;
    GIE=1;
}