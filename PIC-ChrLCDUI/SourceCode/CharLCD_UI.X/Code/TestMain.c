/****************************************************************************
Copyright (C), 2014, Mingv150, All rights reserved
FileName: /Layer/Module/TestMain.c
Description:  
Author:  mingv150@163.com
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
#include "./Driver/BSP/BspEeprom.h"
#include "./Driver/LCD/Lcd.h"
#include "./Driver/LCD/LcdMenu.h"
#include "./Driver/Key/Key.h"
#include "TestMain.h"

/****************************************************************************
Global Data Structure:
*****************************************************************************/
u8 PaperNum;
t_SetupParam SetupParam;
t_FactoryParam FactoryParam;
u8 u8_Timer_Flash = 0;


/*******************************************************************************
*Function: void main()
*Description: main function
*Input:none
*Output:none
*******************************************************************************/
void main(void)
{

  t_Sysmod SystemMode = TestMain_NORMALMOD;
  u8 Key;
  
  Lcd_Init();

  BspSystem_TmrInit();
  LcdMenu_Welcome();
  Lcd_Delay(10000);

  BspEeprom_GetParam(SETUPPARAM_OFFSET,&SetupParam,sizeof(t_SetupParam));
  BspEeprom_GetParam(FACTORYPARAM_OFFSET,&FactoryParam,sizeof(t_FactoryParam));

  Lcd_ClrScreen();

	while(1)
 	{
      //TRISD7=0;

      Key = Key_GetEvent();

      if(Key == EVENT_KEYSETUPS)
        {
          SystemMode++;
          SystemMode = (SystemMode >= TestMain_MAXMOD) ? TestMain_NORMALMOD : SystemMode; 
          Lcd_ClrScreen();   
        }

        switch(SystemMode)
        {
          case TestMain_NORMALMOD:
            LcdMenu_Normal(Key,PaperNum);
            break;

          case TestMain_SETUPMOD:
            LcdMenu_Setup(Key,&SetupParam);  
            break;

          case TestMain_FACTORYMOD:
            LcdMenu_Factory(Key,&FactoryParam);
            break;
        }

      //RD7=~RD7;

  }

}
