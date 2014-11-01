/****************************************************************************
Copyright (C), 2014, Mingv150, All rights reserved
FileName: /Layer/Module/TestMain.c
Description:  
Author:  
Version:  
Changelog: 
*****************************************************************************/

#define _TestMain_C_
/****************************************************************************
Global header file:
*****************************************************************************/
#include <htc.h>
#include "Common.h"
#include "Model.h"
#include "oopc.h"

/****************************************************************************
Local header file:
*****************************************************************************/
#include "./Driver/BSP/BspSystem.h"
#include "./Driver/BSP/BspGpio.h"
#include "./Driver/LCD/Lcd.h"
#include "./Driver/LCD/LcdMenu.h"
#include "./Driver/Key/Key.h"
#include "TestMain.h"

/****************************************************************************
Global Data Structure:
*****************************************************************************/
t_SetupParam SetupParam;
u8 u8_Timer_Flash;
u8 u8_Timer_scan;
/*******************************************************************************
*Function: void main()
*Description: main function
*Input:none
*Output:none
*******************************************************************************/
void main(void)
{
	u16 count = 0;
  u16 KeyTemp;
  u8 SystemMode = 0;
  u8 Key;
  
  Lcd_Init();

  BspSystem_TmrInit();
  LcdMenu_Welcome();
  delayms(1500);

	while(1)
 	{

      if(u8_Timer_scan)
      {
        KeyTemp = Key_Scan();
        Key_Debounce(KeyTemp);
      }

      Key = Key_GetEvent();
      LcdMenu_Setup(Key,&SetupParam);

  	}

}
