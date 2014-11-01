/****************************************************************************
Copyright (C), 2014, Mingv150, All rights reserved
FileName: /Driver/Lcd/LcdMenu.c
Description:  
Author:  
Version:  
Changelog: 
*****************************************************************************/

#define _LcdMenu_C_
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
#include "stdarg.h"
#include "Lcd.h"
#include "LcdMenu.h"
#include "../Key/Key.h"

/****************************************************************************
Global Data Structure:
*****************************************************************************/
static unsigned char LcdMenu_StrTemp[20] = {0};


/****************************************************************************
Function: Name
Description:
Input:
Output:
*****************************************************************************/
static char *myitoa(int value, char *str, int radix)
{
  int nCount = -1, nIndex;
  char *pStr = str, nTemp;
  unsigned int nValue;

  //Character list
  static char szMap[] =
  {
    '0', '1', '2', '3', '4', '5',
    '6', '7', '8', '9', 'a', 'b',
    'c', 'd', 'e', 'f', 'g', 'h',
    'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't',
    'u', 'v', 'w', 'x', 'y', 'z'
  };

  if(radix >= 2 && radix <= 36)
  { // ???radixå¿?¡»????6ä¹??
    if( value < 0 && radix == 10 )
    { // å¦??????°å°±?¨é?ä½?·»????·ï?å¹¶å?å­??ä¸²å?ç§?
      *pStr++ = '-';
      value = -value; //è½?¸ºæ­£æ?
    }

    nValue = *(unsigned *)&value;

    do
    { // å¾??è½??æ¯??ä¸??å­???´å?ç»??
      pStr[ ++nCount ] = szMap[ nValue % radix ];
      nValue /= radix;
    } while( nValue > 0 ); // è½??ç»?????ç¬?¸²??¿»??

    nIndex = ( nCount + 1 ) / 2; // è®¡ç??ºä?????¿åº¦

    while( nIndex-- > 0 )
    { // å°??ç¬?¸²???ç¬??ç¿»è½¬
      nTemp = pStr[ nIndex ];
      pStr[ nIndex ] = pStr[ nCount - nIndex ];
      pStr[ nCount - nIndex ] = nTemp;
    }
  }

  pStr[ nCount + 1 ] = '\0'; // ç½?????
  return str;
}


/****************************************************************************
Function: FunctionName
Description: 
Input: 
Output: 
*****************************************************************************/
static void LcdMenu_VPrintf(const char *String,...)
{
  va_list pv_Start;
  char ch;
  const char *p_Str;
  char StrBuf[20];
  unsigned int d1;
  int d2;
  unsigned index = 0;
  char i = 0;

  va_start(pv_Start,String);

  while(*String)
  {
    if((*String) != '%')
    {
      LcdMenu_StrTemp[index] = *String++;
      index++;
    }
    else
    {
      switch (*(++String))
      {
      case 's':

        p_Str = va_arg(pv_Start,const char*);

        while(*p_Str)
        {
          LcdMenu_StrTemp[index] = *p_Str++;
          index++;
        }

        break;

      case 'x':

        d1 = va_arg(pv_Start,unsigned int);

        myitoa(d1,StrBuf,16);

        i = 0;
        while(StrBuf[i] != '\0')
        {
          LcdMenu_StrTemp[index] = StrBuf[i];
          i++;
          index++;
        }

        break;

      case 'd':

        //d2 = va_arg(pv_Start,int);
        d2 = va_arg(pv_Start,char);

        myitoa(d2,StrBuf,10);

        i = 0;
        while(StrBuf[i] != '\0')
        {
          LcdMenu_StrTemp[index] = StrBuf[i];
          i++;
          index++;
        }

        break;

      case 'c':

        ch = va_arg(pv_Start,char);

        LcdMenu_StrTemp[index] = ch;

        index++;

        break;

      default:

        ch = *String;

        LcdMenu_StrTemp[index] = ch;
        
        index++;

        break;
      }

      String++;
    }
  }

  LcdMenu_StrTemp[index] = '\0';

  va_end(pv_Start);
}


/****************************************************************************
Function: Name
Description:
Input:
Output:
*****************************************************************************/
static void LcdMenu_NumMasK(u8 *Str, u8 MaskBit)
{
  u8 i = 0;
  u8 count = 0;
  while(Str[i] != '\0')
  {
    if(Str[i] == ':')
    {
      i++;
      while(Str[i] != 'm')
      {
        if(Str[i]>= '0' && Str[i] <= '9')
        {
          count++;
          if(count == MaskBit)
          {
            Str[i] = ' ';
          }
        }
        i++;
      }
    }
    i++;
  } 
}


/****************************************************************************
Function: FunctionName
Description: 
Input: 
Output:
Notes: 
*****************************************************************************/
void LcdMenu_Welcome(void)
{
	LcdMenu_VPrintf(LcdMenu_COMPANYNAME);
	Lcd_StrDisp(0,0,LcdMenu_StrTemp);
	LcdMenu_VPrintf(LcdMenu_TELPHONE);
	Lcd_StrDisp(1,0,LcdMenu_StrTemp);
}


/****************************************************************************
Function: Name
Description:
Input:
Output:
Notes:
*****************************************************************************/
void LcdMenu_Normal(u8 KeyEvent,u16 PaperNum)
{
	u8 num1,num2,num3,num4;

	num4 = (PaperNum/10)%10;
	num3 = (PaperNum/100)%10;
	num2 = (PaperNum/1000)%10;
	num1 = (PaperNum/10000)%10;

	LcdMenu_VPrintf("ÕÅÊýÇåÁã:%dS",5);
	Lcd_StrDisp(0,0,LcdMenu_StrTemp);
	LcdMenu_VPrintf("ÕÅÊý:%d%d%d%d",num1,num2,num3,num4);
	Lcd_StrDisp(1,0,LcdMenu_StrTemp);
	LcdMenu_VPrintf("È·ÈÏ¼ü;È·ÈÏ");
	Lcd_StrDisp(2,0,LcdMenu_StrTemp);
	LcdMenu_VPrintf("Í£Ö¹¼ü;Í£Ö¹");
	Lcd_StrDisp(3,0,LcdMenu_StrTemp);
}


/****************************************************************************
Function: Name
Description:
Input:
Output:
Notes:
*****************************************************************************/
void LcdMenu_Setup(u8 KeyEvent,t_SetupParam *pParam)
{
	u8 Change = 0;
	static u8 Section = 0;
	static u8 Index = 0;
	u8 num[3][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0}};

	num[0][3]  = (pParam->PaperNum)%10;
	num[0][2]  = (pParam->PaperNum/10)%10;
	num[0][1]  = (pParam->PaperNum/100)%10;
	num[0][0]  = (pParam->PaperNum/1000)%10;

	num[1][3]  = (pParam->ShuaLiao)%10;
	num[1][2]  = (pParam->ShuaLiao/10)%10;
	num[1][1]  = (pParam->ShuaJiao)%10;
	num[1][0]  = (pParam->ShuaJiao/10)%10;

	num[2][3]  = (pParam->Back)%10;
	num[2][2]  = (pParam->Back/10)%10;
	num[2][1]  = (pParam->Startup)%10;
	num[2][0]  = (pParam->Startup/10)%10;

	if(KeyEvent == EVENT_KEYDOWNS)
	{
		if(Index >= 2)
			Index = 0;
		else
			Index++;
	}

	if(KeyEvent == EVENT_KEYUPS)
	{
		if(Index == 0)
			Index = 2;
		else
			Index--;
	}

	if(KeyEvent == EVENT_KEYRIGHTS)
	{
		if(Section >= 3)
			Section = 0;
		else
			Section++;
	}

	if(KeyEvent == EVENT_KEYLEFTS)
	{
		if(Section == 0)
			Section = 3;
		else
			Section--;
	}

	if(KeyEvent == EVENT_KEYPLUSS)
	{
		if(num[Index][Section] >= 9)
			num[Index][Section] = 0;
		else
			num[Index][Section]++;
		Change = 1;
	}	

	if(KeyEvent == EVENT_KEYMINUSS)
	{
		if(num[Index][Section] > 0)
			num[Index][Section]--;
		else
			num[Index][Section] = 9;
		Change = 1;
	}

	pParam->PaperNum = num[0][0]*1000+num[0][1]*100+num[0][2]*10+num[0][3];
	pParam->ShuaJiao = num[1][0]*10+num[1][1];
	pParam->ShuaLiao = num[1][2]*10+num[1][3];
	pParam->Startup  = num[2][0]*10+num[2][1];
	pParam->Back     = num[2][2]*10+num[2][3];

	LcdMenu_VPrintf("ÉèÖÃ²ÎÊý");
	Lcd_StrDisp(0,0,LcdMenu_StrTemp);
	LcdMenu_VPrintf("ÕÅÊýÉè¶¨:%d%d%d%d",num[0][0],num[0][1],num[0][2],num[0][3]);
	if(Index == 0 && u8_Timer_Flash)
		LcdMenu_NumMasK(LcdMenu_StrTemp, Section+1);
	Lcd_StrDisp(1,0,LcdMenu_StrTemp);
	LcdMenu_VPrintf("Ë¢½º:%d%dDË¢ÁÏ:%d%dA",num[1][0],num[1][1],num[1][2],num[1][3]);
	if(Index == 1 && u8_Timer_Flash)
		LcdMenu_NumMasK(LcdMenu_StrTemp, Section+1);
	Lcd_StrDisp(2,0,LcdMenu_StrTemp);
	LcdMenu_VPrintf("Æô¶¯:%d%dC·µ»Ø:%d%dC",num[2][0],num[2][1],num[2][2],num[2][3]);
	if(Index == 2 && u8_Timer_Flash)
		LcdMenu_NumMasK(LcdMenu_StrTemp, Section+1);
	Lcd_StrDisp(3,0,LcdMenu_StrTemp);

}


/****************************************************************************
Function: Name
Description:
Input:
Output:
Notes:
*****************************************************************************/
void LcdMenu_Factory(u8 KeyEvent,t_FactoryParam *pParam)
{
	u8 Change = 0;
	static u8 Section = 0;
	static u8 Index = 0;

	if(KeyEvent == EVENT_KEYDOWNS)
	{
		if(Index >= 2)
			Index = 0;
		else
			Index++;
	}

	if(KeyEvent == EVENT_KEYUPS)
	{
		if(Index == 0)
			Index = 2;
		else
			Index--;
	}

	if(KeyEvent == EVENT_KEYRIGHTS)
	{
		if(Section >= 9)
			Section = 0;
		else
			Section++;
	}

	if(KeyEvent == EVENT_KEYLEFTS)
	{
		if(Section == 0)
			Section = 9;
		else
			Section--;
	}

	if(KeyEvent == EVENT_KEYPLUSS)
	{
		if(pParam->num[Index][Section] >= 9)
			pParam->num[Index][Section] = 0;
		else
			pParam->num[Index][Section]++;
		Change = 1;
	}	

	if(KeyEvent == EVENT_KEYMINUSS)
	{
		if(pParam->num[Index][Section] > 0)
			pParam->num[Index][Section]--;
		else
			pParam->num[Index][Section] = 9;
		Change = 1;
	}

	LcdMenu_VPrintf("¹¤³§Ä£Ê½");
	Lcd_StrDisp(0,0,LcdMenu_StrTemp);
	LcdMenu_VPrintf("Éè±¸ºÅ;%d%d%d%d%d%d%d%d",pParam->num[0][0],pParam->num[0][1],pParam->num[0][2],pParam->num[0][3],pParam->num[0][4],pParam->num[0][5],pParam->num[0][6],pParam->num[0][7]);
	if(Index == 0 && u8_Timer_Flash)
		LcdMenu_NumMasK(LcdMenu_StrTemp, Section+1);
	Lcd_StrDisp(1,0,LcdMenu_StrTemp);
	LcdMenu_VPrintf("±àºÅ:%d%d%d%d%d%d%d%d%d%d",pParam->num[1][0],pParam->num[1][1],pParam->num[1][2],pParam->num[1][3],pParam->num[1][4],pParam->num[1][5],pParam->num[1][6],pParam->num[1][7],pParam->num[1][8],pParam->num[1][9]);
	if(Index == 1 && u8_Timer_Flash)
		LcdMenu_NumMasK(LcdMenu_StrTemp, Section+1);
	Lcd_StrDisp(2,0,LcdMenu_StrTemp);
	LcdMenu_VPrintf("×´Ì¬:%d%d%d%d%d%d%d%d%d%d",pParam->num[2][0],pParam->num[2][1],pParam->num[2][2],pParam->num[2][3],pParam->num[2][4],pParam->num[2][5],pParam->num[2][6],pParam->num[2][7],pParam->num[2][8],pParam->num[2][9]);
	if(Index == 2 && u8_Timer_Flash)
		LcdMenu_NumMasK(LcdMenu_StrTemp, Section+1);
	Lcd_StrDisp(3,0,LcdMenu_StrTemp);
}
