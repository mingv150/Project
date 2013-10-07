/*******************************************************************************
*Copyright(C),2012-2013,mingv150,All rights reserved
*FileName:
*Description:
*Author:
*Version:
*Date:
*******************************************************************************/

#define _DRV_PHASECHK_C_
#include <htc.h>
#include "../Common.h"
#include "../Model.h"
#include "../oopc.h"

#include "Drv_PhaseChk.h"
#include "Drv_Hardware.h"
#include "Drv_UserInterface.h"
#include "Drv_NTCSensor.h"

t_PhaseChk st_PhaseChk_Dat = 
{
    1,1,1,1,1,1,1,1
};


/*******************************************************************************
*Function:
*Description:
*Input:
*Output:
*******************************************************************************/
void Drv_PhaseChk_1msScan(void)
{
    static bool Phc1New;
    static bool Phc2New;
    static u8 Phc1Count = 0;
    static u8 Phc2Count = 0;
    static u8 Debounce = 0;
    
    Phc1New = HW_PHC1PINRED();
    Phc2New = HW_PHC2PINRED();

    if(st_PhaseChk_Dat.Phc1Old != Phc1New)
    {
        st_PhaseChk_Dat.Phc1Old = Phc1New;

        if(Phc1New == 0)
        {
            Phc1Count = 0;
        }
        else
        {
            if(PHASECHK_JUDGE1(Phc2Count)) //small
            {
                st_PhaseChk_Dat.ResultNew = TRUE;
            }
            else
            {
                st_PhaseChk_Dat.ResultNew = FALSE;
            }
        }

    }
    Phc1Count++;

    Drv_NTC_MesureIRQ();
    
    if(st_PhaseChk_Dat.Phc2Old != Phc2New)
    {
        st_PhaseChk_Dat.Phc2Old = Phc2New;
        
        if(Phc2New == 0)
        {
            Phc2Count = 0;
        }
        else
        {
            if(PHASECHK_JUDGE2(Phc1Count)) //Big
            {
                st_PhaseChk_Dat.ResultNew= TRUE;
            }
            else
            {
                st_PhaseChk_Dat.ResultNew = FALSE;
            }
        }
    }
    Phc2Count++;

    if((Phc1Count>PHASECHK_NOCHG) || (Phc2Count>PHASECHK_NOCHG))
    {
        st_PhaseChk_Dat.ResultNew = FALSE;
    }

    if(st_PhaseChk_Dat.ResultOld != st_PhaseChk_Dat.ResultNew)
    {
        st_PhaseChk_Dat.ResultOld = st_PhaseChk_Dat.ResultNew;
        Debounce = 0;
    }
    else
    {
        Debounce++;
        if(Debounce > 20)
        {
            st_PhaseChk_Dat.Result = st_PhaseChk_Dat.ResultOld;
        }
    }

    Drv_NTC_MesureIRQ();
}


/*******************************************************************************
*Function:
*Description:
*Input:
*Output:
*******************************************************************************/
u8 Drv_PhaseChk_GetEvent()
{
    return st_PhaseChk_Dat.Result;
}