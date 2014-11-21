/****************************************************************************
Copyright (C), 2014, Sharman Chen, All rights reserved
FileName: /Driver/Lcd/LcdMenu.c
Description:
Version:  
Changelog:   
Author: Sharman Chen
Email: mingv150@163.com
Mobile: 13720889921
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
#include "../BSP/BspEeprom.h"
#include "../Key/Key.h"
#include "LcdMenu.h"


/****************************************************************************
Global Data Structure:
*****************************************************************************/
static unsigned char LcdMenu_StrTemp[20] = {0};


/****************************************************************************
Function: Name
Description: 将数字转换成字符串
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
  { // ???radix蹇?』????6涔??
    if( value < 0 && radix == 10 )
    { // 濡??????板氨?ㄩ?浣?坊????凤?骞跺?瀛??涓插?绉?
      *pStr++ = '-';
      value = -value; //杞?负姝ｆ?
    }

    nValue = *(unsigned *)&value;

    do
    { // 寰??杞??姣??涓??瀛???村?缁??
      pStr[ ++nCount ] = szMap[ nValue % radix ];
      nValue /= radix;
    } while( nValue > 0 ); // 杞??缁?????绗?覆??炕??

    nIndex = ( nCount + 1 ) / 2; // 璁＄??轰?????垮害

    while( nIndex-- > 0 )
    { // 灏??绗?覆???绗??缈昏浆
      nTemp = pStr[ nIndex ];
      pStr[ nIndex ] = pStr[ nCount - nIndex ];
      pStr[ nCount - nIndex ] = nTemp;
    }
  }

  pStr[ nCount + 1 ] = '\0'; // 缃?????
  return str;
}


/****************************************************************************
Function: FunctionName
Description: 打印函数，打印字符串到LcdMenu_StrTemp
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

  for(i=0; i<sizeof(LcdMenu_StrTemp); i++)
  {
  	LcdMenu_StrTemp[i] = 0;
  }

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
Description: 屏蔽某个数字字符的显示
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
            break;
          }
        }
        i++;
      }
    }
    i++;
  } 
}


/****************************************************************************
Function: Name
Description: 屏蔽某个字符的显示
Input:
Output:
*****************************************************************************/
static void LcdMenu_CharMasK(u8 *StrMask, u8 *Str)
{
  u8 i = 0;
  u8 j = 0;

  while(Str[i] != '\0' &&  StrMask[j] != '\0')
  {
  	if(Str[i] == StrMask[j] )
  	{
  		Str[i] =  ' ';
                j++;
  	}
    	i++;
  } 
}


/****************************************************************************
Function: FunctionName
Description: 欢迎界面
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
Description: 普通模式下的界面
Input:
Output:
Notes:
*****************************************************************************/
void LcdMenu_Normal(u8 KeyEvent,u16 PaperNum)
{
	u8 num1,num2,num3,num4;
        static u8 Index = 0;
        static u8 FlashStop = 1;
        
	num4 = (PaperNum)%10;
	num3 = (PaperNum/10)%10;
	num2 = (PaperNum/100)%10;
	num1 = (PaperNum/1000)%10;

	if(KeyEvent == EVENT_KEYSETUPS)
	{
		//设置键处理：按下默认（0，0）位置光标开始闪烁
		FlashStop = 1;
		Index = 0;
	}

    if(KeyEvent == EVENT_KEYUPS)
	{
		//UP键处理：向上移动光标
		if(Index >= 1)
			Index = 0;
		else
			Index++;
	}

	if(KeyEvent == EVENT_KEYDOWNS)
	{
		//DOWN键处理：向下移动光标
		if(Index == 0)
			Index = 1;
		else
			Index--;
	}

	if(KeyEvent == EVENT_KEYENTERS)
	{
		//确认键，停止闪烁，并保存参数到EEPROM
		if (Index == 0)
			Index++;
		if(Index == 1)
			FlashStop = 0;

	}

	LcdMenu_VPrintf("张数清零:%dS",5);
	Lcd_StrDisp(0,0,LcdMenu_StrTemp);
	LcdMenu_VPrintf("张数:%d%d%d%d",num1,num2,num3,num4);
	Lcd_StrDisp(1,0,LcdMenu_StrTemp);
        
	LcdMenu_VPrintf("确认键: 确认");
        if(u8_Timer_Flash && Index==0 && FlashStop)
	 LcdMenu_CharMasK(" 确认",LcdMenu_StrTemp);
	Lcd_StrDisp(2,0,LcdMenu_StrTemp);
        
	LcdMenu_VPrintf("停止键: 停止");
        if(u8_Timer_Flash && Index==1 && FlashStop)
	 LcdMenu_CharMasK(" 停止",LcdMenu_StrTemp);
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
	static u8 FlashStop = 1;
	u8 num[3][4] ;

	//张数变量转换为个/十/百/千位 方便显示
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

	if(KeyEvent == EVENT_KEYSETUPS)
	{
		//设置键处理：按下默认（0，0）位置光标开始闪烁
		FlashStop = 1;
		Index = 0;
		Section = 0;
	}

	if(KeyEvent == EVENT_KEYDOWNS)
	{
		//DOWN键处理：向下移动光标
		if(Index >= 2)
			Index = 0;
		else
			Index++;
	}

	if(KeyEvent == EVENT_KEYUPS)
	{
		//UP键处理：向上移动光标
		if(Index == 0)
			Index = 2;
		else
			Index--;
	}

	if(KeyEvent == EVENT_KEYRIGHTS)
	{
		//Right键处理：向右移动光标
		if(Section >= 3)
			Section = 0;
		else
			Section++;
	}

	if(KeyEvent == EVENT_KEYLEFTS)
	{
		//Left键处理：向左移动光标
		if(Section == 0)
			Section = 3;
		else
			Section--;
	}

	if(KeyEvent == EVENT_KEYPLUSS)
	{
		//选中位加1
		if(num[Index][Section] >= 9)
			num[Index][Section] = 0;
		else
			num[Index][Section]++;
		Change = 1;
	}	

	if(KeyEvent == EVENT_KEYMINUSS)
	{
		//选中位减1
		if(num[Index][Section] > 0)
			num[Index][Section]--;
		else
			num[Index][Section] = 9;
		Change = 1;
	}

	if(KeyEvent == EVENT_KEYENTERS)
	{
		//确认键，停止闪烁，并保存参数到EEPROM
		if(Index >= 2)
			FlashStop = 0;
		else
			Index++;

		BspEeprom_SaveParam(SETUPPARAM_OFFSET,(const u8 *)pParam,sizeof(t_SetupParam));
	}

	pParam->PaperNum = num[0][0]*1000+num[0][1]*100+num[0][2]*10+num[0][3];
	pParam->ShuaJiao = num[1][0]*10+num[1][1];
	pParam->ShuaLiao = num[1][2]*10+num[1][3];
	pParam->Startup  = num[2][0]*10+num[2][1];
	pParam->Back     = num[2][2]*10+num[2][3];

	//打印“设备参数”到字符数组LcdMenu_StrTemp
	LcdMenu_VPrintf("设置参数");
	Lcd_StrDisp(0,0,LcdMenu_StrTemp);

	LcdMenu_VPrintf("张数设定:%d%d%d%d",num[0][0],num[0][1],num[0][2],num[0][3]);
	if(Index == 0 && u8_Timer_Flash && FlashStop)
		LcdMenu_NumMasK(LcdMenu_StrTemp, Section+1);
	Lcd_StrDisp(1,0,LcdMenu_StrTemp);

	LcdMenu_VPrintf("刷胶:%d%dD刷料:%d%dA",num[1][0],num[1][1],num[1][2],num[1][3]);
	if(Index == 1 && u8_Timer_Flash && FlashStop)
		LcdMenu_NumMasK(LcdMenu_StrTemp, Section+1);
	Lcd_StrDisp(2,0,LcdMenu_StrTemp);

	LcdMenu_VPrintf("启动:%d%dC返回:%d%dC",num[2][0],num[2][1],num[2][2],num[2][3]);
	if(Index == 2 && u8_Timer_Flash && FlashStop)
		LcdMenu_NumMasK(LcdMenu_StrTemp, Section+1);
	Lcd_StrDisp(3,0,LcdMenu_StrTemp);

}


/****************************************************************************
Function: void LcdMenu_Factory(u8 KeyEvent,t_FactoryParam *pParam)
Description: 工厂模式菜单处理
Input:
Output:
Notes:
*****************************************************************************/
void LcdMenu_Factory(u8 KeyEvent,t_FactoryParam *pParam)
{
	u8 i,j;
	//数据保存标志位
	u8 Change = 0;
	//光标左右位置索引
	static u8 Section = 0;
	//光标上下位置索引
	static u8 Index = 0;
	//闪烁停止标志位
	static u8 FlashStop = 1;


	if(KeyEvent == EVENT_KEYSETUPS)
	{
		//设置键处理：按下默认（0，0）位置光标开始闪烁
		FlashStop = 1;
		Index = 0;
		Section = 0;
	}

	if(KeyEvent == EVENT_KEYDOWNS)
	{
		//DOWN键处理：向下移动光标
		if(Index >= 2)
			Index = 0;
		else
			Index++;
	}

	if(KeyEvent == EVENT_KEYUPS)
	{
		//UP键处理：向上移动光标
		if(Index == 0)
			Index = 2;
		else
			Index--;
	}

	if(KeyEvent == EVENT_KEYRIGHTS)
	{
		//Right键处理：向右移动光标
		if(Index == 0)
		{
			if(Section >= 7)
				Section = 0;
			else
				Section++;
		}
		else
		{
			if(Section >= 9)
				Section = 0;
			else
				Section++;
		}

	}

	if(KeyEvent == EVENT_KEYLEFTS)
	{
		//Left键处理：向左移动光标
		if(Index == 0)
		{
			if(Section == 0)
				Section = 7;
			else
				Section--;
		}
		else
		{
			if(Section == 0)
				Section = 9;
			else
				Section--;
		}

	}

	for(i=0;i<3;i++)
	{
		for(j=0;j<10;j++)
		{
			if(pParam->num[i][j] > 9)
			{
				pParam->num[i][j] = 0;
			}	
		}

	}

	if(KeyEvent == EVENT_KEYPLUSS)
	{
		//选中位加1
		if(pParam->num[Index][Section] >= 9)
			pParam->num[Index][Section] = 0;
		else
			pParam->num[Index][Section]++;
		Change = 1;
	}	

	if(KeyEvent == EVENT_KEYMINUSS)
	{
		//选中位减一
		if(pParam->num[Index][Section] > 0)
			pParam->num[Index][Section]--;
		else
			pParam->num[Index][Section] = 9;
		Change = 1;
	}

	if(KeyEvent == EVENT_KEYENTERS)
	{
		//按确定键保存数据到EEPROM并停止闪烁
		if(Index >= 2)
			FlashStop = 0;
		else
			Index++;

		BspEeprom_SaveParam(FACTORYPARAM_OFFSET,(const u8 *)pParam,sizeof(t_FactoryParam));
	}

	//打印字符串"工厂模式"到LcdMenu_StrTemp数组中
	LcdMenu_VPrintf("工厂模式");
	//LcdMenu_StrTemp字符串数组送LCD显示
	Lcd_StrDisp(0,0,LcdMenu_StrTemp);
	
	//打印字符串""设备号:%d%d%d%d%d%d%d%d""到LcdMenu_StrTemp数组中
	LcdMenu_VPrintf("设备号:%d%d%d%d%d%d%d%d",pParam->num[0][0],pParam->num[0][1],pParam->num[0][2],pParam->num[0][3],pParam->num[0][4],pParam->num[0][5],pParam->num[0][6],pParam->num[0][7]);
	//进行数字位闪烁处理
	if(Index == 0 && u8_Timer_Flash && FlashStop)
		//处理闪烁
		LcdMenu_NumMasK(LcdMenu_StrTemp, Section+1);
	//LcdMenu_StrTemp字符串数组送LCD显示
	Lcd_StrDisp(1,0,LcdMenu_StrTemp);
	
	//打印字符串"编号:%d%d%d%d%d%d%d%d%d%d"到LcdMenu_StrTemp数组中
	LcdMenu_VPrintf("编号:%d%d%d%d%d%d%d%d%d%d",pParam->num[1][0],pParam->num[1][1],pParam->num[1][2],pParam->num[1][3],pParam->num[1][4],pParam->num[1][5],pParam->num[1][6],pParam->num[1][7],pParam->num[1][8],pParam->num[1][9]);
	//进行数字位闪烁处理
	if(Index == 1 && u8_Timer_Flash && FlashStop)
		LcdMenu_NumMasK(LcdMenu_StrTemp, Section+1);
	//LcdMenu_StrTemp字符串数组送LCD显示
	Lcd_StrDisp(2,0,LcdMenu_StrTemp);
	
	LcdMenu_VPrintf("状态:%d%d%d%d%d%d%d%d%d%d",pParam->num[2][0],pParam->num[2][1],pParam->num[2][2],pParam->num[2][3],pParam->num[2][4],pParam->num[2][5],pParam->num[2][6],pParam->num[2][7],pParam->num[2][8],pParam->num[2][9]);
	if(Index == 2 && u8_Timer_Flash && FlashStop)
		LcdMenu_NumMasK(LcdMenu_StrTemp, Section+1);
	Lcd_StrDisp(3,0,LcdMenu_StrTemp);
}
