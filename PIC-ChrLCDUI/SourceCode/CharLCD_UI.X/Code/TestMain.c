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
  
  /*初始化液晶屏*/
  Lcd_Init();
  /*定时器初始化*/
  BspSystem_TmrInit();
  /*显示开机界面*/
  LcdMenu_Welcome();
  /*开机界面延时*/
  Lcd_Delay(10000);

  /*开机从eeprom中获取SetupParam参数*/
  BspEeprom_GetParam(SETUPPARAM_OFFSET,&SetupParam,sizeof(t_SetupParam));
  /*开机从eeprom中获取FactoryParam参数*/
  BspEeprom_GetParam(FACTORYPARAM_OFFSET,&FactoryParam,sizeof(t_FactoryParam));
  /*lcd清屏*/
  Lcd_ClrScreen();

	while(1)
 	{
      
      /*从按键缓冲区获取按键*/
      Key = Key_GetEvent();

      if(Key == EVENT_KEYSETUPS)
        {
          /*如果是设置键则进行菜单界面模式切换*/
          SystemMode++;
          SystemMode = (SystemMode >= TestMain_MAXMOD) ? TestMain_NORMALMOD : SystemMode; 
          Lcd_ClrScreen();   
        }

        switch(SystemMode)
        {
          /*正常模式下菜单*/
          case TestMain_NORMALMOD:
            LcdMenu_Normal(Key,PaperNum);
            break;
          /*设置模式下菜单*/
          case TestMain_SETUPMOD:
            LcdMenu_Setup(Key,&SetupParam);  
            break;
          /*工厂模式下菜单*/  
          case TestMain_FACTORYMOD:
            LcdMenu_Factory(Key,&FactoryParam);
            break;
        }
      //测试主函数执行一轮的时间
      //TRISD7=0;
      //RD7=~RD7;

  }

}
