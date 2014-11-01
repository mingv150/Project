/****************************************************************************
Copyright (C), 2014, Mingv150, All rights reserved
FileName: /Driver/BSP/BspGpio.h
Description:  
Author:  
Version:  
Changelog: 
*****************************************************************************/

#ifndef _BspGpio_H_
#define _BspGpio_H_

/*For common use*/
/*Macro Definition*/
#define BspGpio_PURPOSE 1
#define BspGpio_OUTPUT 0
#define BspGpio_INPUT 1
#define BspGpio_LOW 0
#define BspGpio_HIGH 1
#define BspGpio_PORTAC(x) (TRISA = (x))
#define BspGpio_PORTAD(x) (PORTA = (x))
#define BspGpio_PORTBC(x) (TRISB = (x))
#define BspGpio_PORTBD(x) (PORTB = (x))
#define BspGpio_PORTCC(x) (TRISC = (x))
#define BspGpio_PORTCD(x) (PORTC = (x))

#define BspGpio_BUZZERC(x) (TRISC0 = (x))
#define BspGpio_BUZZEROUT(x) (RC0=(x))
#define BspGpio_BUZZERIN()  (RC0)

#define BspGpio_SCLKC(x) (TRISE0=(x))
#define BspGpio_SDATC(x) (TRISE1=(x))
#define BspGpio_CSC(x) (TRISE2=(x))

#define BspGpio_SCLKOUT(x) (RE0=(x))
#define BspGpio_SDATOUT(x) (RE1=(x))
#define BspGpio_SDATIN() (RE1)
#define BspGpio_CSOUT(x) (RE2=(x))

#define BspGpio_KEY1C(x) (TRISD0=(x))
#define BspGpio_KEY1IN() (RD0)
#define BspGpio_KEY2C(x) (TRISD1=(x))
#define BspGpio_KEY2IN() (RD1)
#define BspGpio_KEY3C(x) (TRISD2=(x))
#define BspGpio_KEY3IN() (RD2)
#define BspGpio_KEY4C(x) (TRISD3=(x))
#define BspGpio_KEY4IN() (RD3)
#define BspGpio_KEY5C(x) (TRISD4=(x))
#define BspGpio_KEY5OUT(x) (RD4=(x))
#define BspGpio_KEY6C(x) (TRISD5=(x))
#define BspGpio_KEY6OUT(x) (RD5=(x))
#define BspGpio_KEY7C(x) (TRISD6=(x))
#define BspGpio_KEY7OUT(x) (RD6=(x))


/*Data Type Specification*/

#ifdef _BspGpio_C_
/*For local users*/
/*Macro Definition*/
/*Data Type Specification*/
#else
/*For external users*/
/*Macro Definition*/
/*Data Type Specification*/
/*Interface Declarations*/
void BspGpio_Buzzer(void);
#endif

#endif	