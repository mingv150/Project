/****************************************************************************
Copyright (C), 2014, Mingv150, All rights reserved
FileName: /Driver/BSP/BspSystem.c
Description:  
Author:  
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
#include "BspSystem.h"

/****************************************************************************
Global Data Structure:
*****************************************************************************/
/*Configuration Bit Setting*/
__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_OFF & MCLRE_OFF & CP_ON & CPD_OFF & BOREN_ON & IESO_OFF & FCMEN_ON & DEBUG_ON);
//__CONFIG(BOR4V_BOR40V & WRT_OFF);


static u8 BspSystem_Purpose;

/****************************************************************************
Function: FunctionName
Description: 
Input: 
Output:
Notes: 
*****************************************************************************/
static void BspSystem_Action(void)
{
	
}

extern u8 u8_Timer_Flash;
extern u8 u8_Timer_scan;
void interrupt IRQ(void)
{
    static u8 TimeBase1 = 0;
    static u8 TimeBase2 = 0;
    static u8 TimeBase3 = 0;
    static u8 TimeBase4 = 0;


    if(T0IF&&T0IE)
    {
        TMR0=179;
        
        /*4*/
        if(++TimeBase1 == 2)
        {
        	TimeBase1 = 0;
        	u8_Timer_scan = ~u8_Timer_scan;

        }

        /*32*/
        if(++TimeBase2 == 41)
        {
        	TimeBase2 = 0;

        }
        
        /*64*/
        if(++TimeBase3 == 63)
        {
        	TimeBase3 = 0;

        }
        
        /*128*/
        if(++TimeBase4 == 129)
        {
        	TimeBase4 = 0;
        	u8_Timer_Flash = ~u8_Timer_Flash;

        }
        
        T0IF=0;
    }
}




void BspSystem_TmrInit(void)
{
    /*Enable Prescale*/
    /*Clock = 4915200Hz*/
    PSA=0;
    T0CS=0;
    
    /*1:64*/
    PS0=1;PS1=0;PS2=1;  
    /*timeout time:997HZ*/
    TMR0=179;
    /*Tiemr0 int en*/
    T0IE=1;
    GIE=1;

    /*Timer 1 Setting*/
    T1CON = 0b00100100;
	TMR1IE = 0;
    TMR1IF = 0;
    //T1CKPS0=0;T1CKPS1=1;
    //T1OSCEN=0;
    //TMR1CS=0;

    TMR1H=0x00;TMR1L=0x00;
}