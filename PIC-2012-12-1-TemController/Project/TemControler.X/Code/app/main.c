/*******************************************************************************
*Copyright(C),2012-2013,mingv150,All rights reserved
*FileName:
*Description:
*Author:
*Version:
*Date:
*******************************************************************************/

#define _MAIN_C_
#include <htc.h>
#include "../Common.h"
#include "../Model.h"
#include "../oopc.h"

#include "../drv/Drv_Hardware.h"
#include "../drv/Drv_UserInterface.h"
#include "../drv/Drv_PhaseChk.h"
#include "../drv/Drv_NTCSensor.h"
#include "../drv/Drv_Timer.h"
#include "App_fsm.h"

bool InitComplete = 0;
/*******************************************************************************
*Function: void main()
*Description: main function
*Input:none
*Output:none
*******************************************************************************/
void main()
{
    u8 StartDelayTmrID;
    
    InitComplete = 0;
    Drv_Hw_IOInit();
    Drv_Hw_TmrInit();
    Drv_NTC_MesureInit();
    
    Drv_Timer_Create(&StartDelayTmrID,2000,1,NULL);
    App_fsm_Spkon();    
    while(!Drv_Timer_TimeOutChk(&StartDelayTmrID));   
    App_fsm_Spkoff();
	
    App_Fsm_InitParam();

    Drv_UI_LedDis(0,UI_LEDALL);

    Drv_Hw_WdtInit();
    
    while(1)
    {
        if(f_Hw_2ms)
        {
            f_Hw_2ms = 0;
            Drv_UI_KeyScan();
        }
        
        if(f_Hw_32ms)
        {
            f_Hw_32ms = 0;
        }
        
        if(f_Hw_64ms)
        {
            f_Hw_64ms = 0;
        }
        
        if(f_Hw_128ms)
        {
            f_Hw_128ms = 0;
            if(InitComplete)
             App_fsm_ErrScan();
             CLRWDT();
        }

        if(Drv_NTC_CalTemperature())
        {
            App_Fsm_GetTemperature();
            InitComplete = TRUE;
        }      
        
        if(InitComplete)
        {
            App_fsm_StateDispose();
        }
    }
}
