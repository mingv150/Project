/*******************************************************************************
*Copyright(C),2012-2013,mingv150,All rights reserved
*FileName:
*Description:
*Author:
*Version:
*Date:
*******************************************************************************/
#define _APP_FSM_C_
/*common*/
#include <htc.h>
#include "../Common.h"
#include "../Model.h"
#include "../oopc.h"

/*myself*/
#include "App_fsm.h"

/*mycall*/
#include "../drv/Drv_Hardware.h"
#include "../drv/Drv_Eep.h"
#include "../drv/Drv_UserInterface.h"
#include "../drv/Drv_NTCSensor.h"
#include "../drv/Drv_Timer.h"
#include "../drv/Drv_PhaseChk.h"
#include "../drv/Drv_Event.h"
#include "App_pid.h"



t_SetupParam st_Fsm_SetupParam = 
{
    500,    //HighTemTHR Default Value
    100,     //LowTemTHR Default Value							 
    400,    //ConstHighTHR Default Value
    -150,    //ConstLowTHR Default Value
    99,     //DiffHighTHR Default Value
    -99,    //DiffLowTHR Default Value  
    15,     //CompRunTHR Default Value
    -15,    //CompStopTHR Default Value   
    60,     //CompDlyTHR Default Value
    30,     //EngyLowTHR Default Value
    0,      //RoomTemCal Default Value
    0,      //WatTemCal Default Value
    0,      //Password Default Value
    {1,1,1,0,0,1,0,0,0}
};

t_CtrParam st_Fsm_CtrParam = 
{
    FSM_TEMINVAILD,FSM_TEMINVAILD,
    500,
    200,25
};

u8 f_SoftwarePwrOn = FALSE;



/*******************************************************************************
*Function:
*Description:
*Input:
*Output:
*******************************************************************************/
void App_fsm_Spkcb(void)
{
    static bool level = 0;
    level = ~level;
    SPKPINOUT(level);
}

void App_fsm_AllLedOff(void)
{
    Drv_UI_LedDis(0,UI_LEDALL);
}

/*******************************************************************************
*Function:
*Description:
*Input:
*Output:
*******************************************************************************/
void App_fsm_Spkoff(void)
{
    SPKPINOUT(0);
}


void App_fsm_Spkon(void)
{
    SPKPINOUT(1);
}

/*******************************************************************************
*Function:
*Description:
*Input:
*Output:
*******************************************************************************/
void App_fsm_CompOnOff(u8 on)
{
    HW_COMPPINOUT(on);
    Drv_UI_LedDis(on,UI_LEDCOMP);
    Drv_UI_LedDis(0,UI_LEDCOMPF);
    if(st_Fsm_SetupParam.OptBit.SwPumpwithCps)
    {
        HW_PUMPPINOUT(on);
        Drv_UI_LedDis(on,UI_LEDPUMP);
    }
}


/*******************************************************************************
*Function:
*Description:1ms call
*Input:
*Output:
*******************************************************************************/
void App_fsm_PumpOnOff(u8 On)
{
    HW_PUMPPINOUT(On);
    Drv_UI_LedDis(On,UI_LEDPUMP);
}


/*******************************************************************************
*Function:
*Description:
*Input:
*Output:
*******************************************************************************/
void App_Fsm_InitParam(void)
{
    u8 MagicNum; 
    
    Drv_Eep_GetParam(FSM_EEPMAGADDR,&MagicNum,1);
    
    if(MagicNum == FSM_EEPMAGICNUM)
    {
        Drv_Eep_GetParam(FSM_EEPSETUPADDR,(u8 *)&st_Fsm_SetupParam,sizeof(st_Fsm_SetupParam));
        Drv_Eep_GetParam(FSM_EEPDIFFADDR,(u8 *)&(st_Fsm_CtrParam.DiffTemAim),2);
        Drv_Eep_GetParam(FSM_EEPCONSTADDR,(u8 *)&(st_Fsm_CtrParam.ConstTemAim),2);
    }
    else
    {
        MagicNum = FSM_EEPMAGICNUM;
        
        st_Fsm_SetupParam.HighTemTHR = 500;
        st_Fsm_SetupParam.LowTemTHR = 100;
        st_Fsm_SetupParam.ConstHighTHR = 400;
        st_Fsm_SetupParam.ConstLowTHR = -150;
        st_Fsm_SetupParam.DiffHighTHR = 99;
        st_Fsm_SetupParam.DiffLowTHR = -99;
        st_Fsm_SetupParam.CompRunTHR = 15;
        st_Fsm_SetupParam.CompStopTHR = -15;
        st_Fsm_SetupParam.CompDlyTHR = 60;
        st_Fsm_SetupParam.EngyLowTHR = 30;
        st_Fsm_SetupParam.RoomTemCal = 0;
        st_Fsm_SetupParam.WatTemCal = 0;
        st_Fsm_SetupParam.Password = 0;

        st_Fsm_SetupParam.OptBit.SwRoomTemDis = 1;
        st_Fsm_SetupParam.OptBit.SwPumpOnOff = 1;
        st_Fsm_SetupParam.OptBit.SwPumpwithCps = 1;
        st_Fsm_SetupParam.OptBit.SwCpsCTR = 0;
        st_Fsm_SetupParam.OptBit.SwTemCTRMode = 0;
        st_Fsm_SetupParam.OptBit.SwPhaseChk = 1;
        st_Fsm_SetupParam.OptBit.SwRemote = 0;
        st_Fsm_SetupParam.OptBit.SwReserve = 0;
        st_Fsm_SetupParam.OptBit.SwReserveByte = 0;

        st_Fsm_CtrParam.DiffTemAim = 25;
        st_Fsm_CtrParam.ConstTemAim = 200;
        
        Drv_Eep_CompareSave(FSM_EEPSETUPADDR,(const u8 *)&st_Fsm_SetupParam,sizeof(st_Fsm_SetupParam));
        Drv_Eep_CompareSave(FSM_EEPDIFFADDR,(const u8 *)&(st_Fsm_CtrParam.DiffTemAim),2);
        Drv_Eep_CompareSave(FSM_EEPCONSTADDR,(const u8 *)&(st_Fsm_CtrParam.ConstTemAim),2);
        Drv_Eep_CompareSave(FSM_EEPMAGADDR,(const u8 *)&MagicNum,1);
    }
}


/*******************************************************************************
*Function:
*Description:
*Input:
*Output:
*******************************************************************************/
void App_Fsm_RecvKeyEvent(u8 KeyValue)
{
    u8 temp;
    
    switch(KeyValue)
    {
        case UI_KEY1SHORT:
            temp = EVENT_KEY1S;            
            break;
        
        case UI_KEY1LONG:
            temp = EVENT_KEY1L;
            break;

        case UI_KEY2SHORT:
            temp = EVENT_KEY2S;
            break;

        case UI_KEY2LONG:
            temp = EVENT_KEY2L;
            break;

        case UI_KEY3SHORT:
            temp = EVENT_KEY3S;
            break;

        case UI_KEY3LONG:
            temp = EVENT_KEY3L;
            break;

        case UI_KEY4SHORT:
            temp = EVENT_KEY4S;
            break;

        case UI_KEY4LONG:
            temp = EVENT_KEY4L;
            break; 

        case UI_KEY12LONG:
            temp = EVENT_KEY12L;
            break;
            
        case UI_KEY14LONG:
            temp = EVENT_KEY14L;    
            break;
            
        default:
            return;
    }

    Drv_Event_PutEventToTail(temp);
}

void App_Fsm_GetTemperature(void)
{
    if(Drv_NTC_GetTempResult(&st_Fsm_CtrParam.WatTem,&st_Fsm_CtrParam.RoomTem))
    {
        st_Fsm_CtrParam.WatTem += st_Fsm_SetupParam.WatTemCal;
        st_Fsm_CtrParam.RoomTem += st_Fsm_SetupParam.RoomTemCal;
    }
}


static u8 EngyLow = 0;


void App_fsm_ErrScan(void)
{
    static u8 Delay1 = 0;
    static u8 Delay2 = 0;
    static u8 Delay3 = 0;
    static u8 Delay4 = 0;
    static u8 Delay5 = 0;
    
    if(!Drv_PhaseChk_GetEvent() && st_Fsm_SetupParam.OptBit.SwPhaseChk)
    {
        Delay1 = 0;
        Delay2 = 0;
        Delay3 = 0;
        Delay4 = 0;
        Delay5 = 0;
        Drv_Event_PutEventToTail(EVENT_ERR1_PHASEERR);
        return;
    }

    if(u8_UI_SensorErr == UI_OVERLOADERR)
    {
        if(Delay1++ > 10)
        {
            Delay1 = 10;
            Drv_Event_PutEventToTail(EVENT_ERR0_OVERLOAD);
        }
        return;
    }
    Delay1 = 0;
    
    if(u8_UI_SensorErr == UI_OPSERR)
    {
        if(Delay2++ > 100)
        {
            Delay2 = 100;
            Drv_Event_PutEventToTail(EVENT_ERR2_OPS);
        }
        return;
    }
    Delay2 = 0;
    
    if(u8_UI_SensorErr == UI_HVOVERERR)
    {
        if(Delay3++ > 10)
        {
            Delay3 = 10;
            Drv_Event_PutEventToTail(EVENT_ERR3_HVERR);
        }
        return;
    }
    Delay3 = 0;
    
    if(u8_UI_SensorErr == UI_LVOVERERR)
    {
        if(Delay4++ > 10)
        {
            Delay4 = 10;
            Drv_Event_PutEventToTail(EVENT_ERR4_LVERR);
        }
        return;
    }
    Delay4 = 0;

    #if(NTC_NTCTYPE == NTC_TYPE1)
    if(st_Fsm_CtrParam.WatTem <= -400)
    #elif(NTC_NTCTYPE == NTC_TYPE2)
    if(st_Fsm_CtrParam.WatTem <= -450)
    #endif    
    {
        Drv_Event_PutEventToTail(EVENT_ERR7_WNTCERR);
        return;
    }

    #if(NTC_NTCTYPE == NTC_TYPE1)
    if(st_Fsm_CtrParam.RoomTem <= -400)
    #elif(NTC_NTCTYPE == NTC_TYPE2)
    if(st_Fsm_CtrParam.RoomTem <= -450)
    #endif
    {
        Drv_Event_PutEventToTail(EVENT_ERR6_RNTCERR);
        return;
    }
   
    if(st_Fsm_CtrParam.WatTem > st_Fsm_SetupParam.HighTemTHR)
    {
        /*OverHeat*/
        /*Turn off when HighTemTHR = 100*/
        if(st_Fsm_SetupParam.HighTemTHR != 1000)
        Drv_Event_PutEventToTail(EVENT_ERR8_OVERHEAR);
        return;
    }
    
    if(st_Fsm_CtrParam.WatTem < st_Fsm_SetupParam.LowTemTHR)
    {
        //LowTem
        if(st_Fsm_SetupParam.LowTemTHR != -400)
        Drv_Event_PutEventToTail(EVENT_ERR9_LOWTEMP);
        return;
    }

    if(EngyLow)
    {
        Drv_Event_PutEventToTail(EVENT_ERR5_EGYLOW);
        return;
    }
    
    if(Delay5++ > 10)
    {
        Delay5 = 0;
        Drv_Event_PutEventToTail(EVENT_NOERR);
    }    
    
}

/*******************************************************************************
*Function:
*Description:1ms call
*Input:
*Output:
*******************************************************************************/
void App_fsm_CtrLogic(void)
{
    static u8 ComStartPrepare = 0;
    static u8 ComDebounce = 50;
    
    static u8 CompDlyTmrID = TIMER_NOTIMER;
    static u8 CompStartDelayTmrID = TIMER_NOTIMER;
    static u8 EngyLowWarnTmrID = TIMER_NOTIMER;
    
    /*Warn Logic*/
    if(f_SoftwarePwrOn)
    {
        f_SoftwarePwrOn = FALSE;
        ComStartPrepare = 0;
        Drv_Timer_Cancel(&CompDlyTmrID);
        Drv_Timer_Cancel(&EngyLowWarnTmrID);
        EngyLow = 0;
        Drv_Timer_Cancel(&CompStartDelayTmrID);
    }


    /*Pump Control Logic*/
    if(!st_Fsm_SetupParam.OptBit.SwPumpwithCps)
    {
        App_fsm_PumpOnOff(1);
    }
    else
    {
        if(HW_COMPPINRED())
        App_fsm_PumpOnOff(1);
    }

    if(st_Fsm_SetupParam.OptBit.SwCpsCTR)
    {
        if(CompStartDelayTmrID == TIMER_NOTIMER)
        {
            Drv_Timer_Create(&CompStartDelayTmrID,((u32)st_Fsm_SetupParam.CompDlyTHR)<<10,1,NULL);         
        }
        else
        {
            if(Drv_Timer_TimeOutChk(&CompStartDelayTmrID))
            {
                App_fsm_CompOnOff(1);      
            }
			else
			{
				if(HW_COMPPINRED() != 1)
				{
					Drv_UI_LedDis(1,UI_LEDCOMP);
					Drv_UI_LedDis(1,UI_LEDCOMPF);
				}
			}
        }
        
        return;
    }

    Drv_Timer_Cancel(&CompStartDelayTmrID);

    /*Compressor Control Logic*/
    if(st_Fsm_CtrParam.WatTem >= (st_Fsm_CtrParam.AllTemAim + st_Fsm_SetupParam.CompRunTHR))
    {
        if(ComDebounce-- < 40)
        {
            ComDebounce = 50;

            if(!ComStartPrepare)
            {
                /*Prepare to Start Compressor*/
                
                Drv_Timer_Create(&CompDlyTmrID,((u32)st_Fsm_SetupParam.CompDlyTHR)<<10,1,NULL);

                Drv_Timer_Create(&EngyLowWarnTmrID,((u32)st_Fsm_SetupParam.EngyLowTHR*60)<<10,1,NULL);

                if((CompDlyTmrID != TIMER_NOTIMER) && (EngyLowWarnTmrID != TIMER_NOTIMER))
                {
                    ComStartPrepare = 1;

                    Drv_UI_LedDis(1,UI_LEDCOMP);
            
                    Drv_UI_LedDis(1,UI_LEDCOMPF);
                }
                else
                {
                    ComStartPrepare = 0;
                    
                    Drv_Timer_Cancel(&CompDlyTmrID);

                    Drv_Timer_Cancel(&EngyLowWarnTmrID);     
                }
            }
            else
            {
                if(Drv_Timer_TimeOutChk(&CompDlyTmrID))
                {
                    /*Start Compressor*/
                    App_fsm_CompOnOff(1);
                }
                
                if(Drv_Timer_TimeOutChk(&EngyLowWarnTmrID))
                {
                    EngyLow = 1;
                }
            }
        }  
    }
    else if(st_Fsm_CtrParam.WatTem <= (st_Fsm_CtrParam.AllTemAim + st_Fsm_SetupParam.CompStopTHR))
    {
        
        if(ComDebounce++ > 60)
        {
            /*Stop Compressor*/
            
            ComDebounce = 50;

            ComStartPrepare = 0;

            Drv_Timer_Cancel(&CompDlyTmrID);

            Drv_Timer_Cancel(&EngyLowWarnTmrID);

            EngyLow = 0;

            if(!st_Fsm_SetupParam.OptBit.SwCpsCTR)
            App_fsm_CompOnOff(0);
        }
    }
    else
    {
        if((st_Fsm_CtrParam.WatTem > st_Fsm_CtrParam.AllTemAim))
        {
            if(Drv_Timer_TimeOutChk(&EngyLowWarnTmrID))
            {
                EngyLow = 1;
            }
        }
        else
        {
            Drv_Timer_Cancel(&EngyLowWarnTmrID);
            EngyLow = 0;
        }
    
        if(CompDlyTmrID != TIMER_NOTIMER)
        {
            ComStartPrepare = 0;
            
            Drv_UI_LedDis(0,UI_LEDCOMP);
            
            Drv_UI_LedDis(0,UI_LEDCOMPF);

            Drv_Timer_Cancel(&CompDlyTmrID);

            Drv_Timer_Cancel(&EngyLowWarnTmrID);
            EngyLow = 0;
        }
    }
}


/*******************************************************************************
*Function:
*Description:1ms call
*Input:
*Output:
*******************************************************************************/
void App_fsm_StateDispose(void)
{
    u8 Event;
    static u16 Password = 000;
    static u8 PasswordOK = 0;
    static u8 StateSys = STATE_NORMAL;
    static u8 StateMenu = STATE_F01;
    static u8 StateTimerID = TIMER_NOTIMER;
    static bool floatflag;
    static u8 SpkAlarmTmrID = TIMER_NOTIMER;
    static u8 SpkKeyTmrID = TIMER_NOTIMER;
    static s16 *Param = NULL;
    u8 bitindex = 0xff;
    static u8 ParamAlter = FALSE;
    s16 ValueMax;
    s16 ValueMin;
    u8 ErrCode = 0;
    u8 temp = 0;
    
    Event = Drv_Event_GetEvent();

    if(Event == EVENT_KEY1S || Event == EVENT_KEY2S || Event == EVENT_KEY3S || Event == EVENT_KEY4S)
    {
        if(SpkKeyTmrID == TIMER_NOTIMER)
        {
            Drv_Timer_Create(&SpkKeyTmrID,250,1,NULL);
            App_fsm_Spkon();
        }
    }

    if(Drv_Timer_TimeOutChk(&SpkKeyTmrID))
    {
        App_fsm_Spkoff();
    }
    
    switch(StateSys)
    {
        case STATE_POWEOFF:   
            /*Power off Function*/
            Drv_UI_StrDis(0,UI_NUMDUMMY,UI_NUMDUMMY,UI_NUMDUMMY);
            Drv_UI_StrDis(1,UI_NUMDUMMY,UI_NUMDUMMY,UI_NUMDUMMY);
            Drv_UI_LedDis(1,UI_LEDPWR);
            App_fsm_CompOnOff(0);
            App_fsm_PumpOnOff(0);
            HW_ALARMPINOUT(0);
            u8_UI_SensorErr = 0x00;
            f_SoftwarePwrOn= TRUE;
            
            if(Event == EVENT_KEY1S)
            {
                App_Fsm_InitParam();
                StateSys = STATE_NORMAL;
            }
            
            #if SIMULATION
            if(Event == EVENT_KEY2L)
            #else
            if(Event == EVENT_KEY12L)
            #endif
            {
                StateMenu = STATE_F01;
                StateSys = STATE_SETUP;
            }
            break;
            
        case STATE_NORMAL:
        
            /*Normal Function*/ 
            Drv_UI_NumDis(0,1,st_Fsm_CtrParam.WatTem);
            Drv_UI_LedDis(1,UI_LEDPWR);

            HW_ALARMPINOUT(1);
            
            if(st_Fsm_SetupParam.OptBit.SwTemCTRMode)
            {
                st_Fsm_CtrParam.AllTemAim = st_Fsm_CtrParam.DiffTemAim + st_Fsm_CtrParam.RoomTem;
                Drv_UI_NumDis(1,1,st_Fsm_CtrParam.DiffTemAim);
            }
            else
            {
                st_Fsm_CtrParam.AllTemAim = st_Fsm_CtrParam.ConstTemAim; 
                Drv_UI_NumDis(1,1,st_Fsm_CtrParam.ConstTemAim);
            }            
            
			/*Event process in Normal Mode*/
            switch(Event)
            {
                case EVENT_KEY1S:
                    if(st_Fsm_SetupParam.OptBit.SwRemote)
                        break;
                    Drv_UI_LedDis(0,UI_LEDALL);
                    StateSys = STATE_POWEOFF;
                    break;
                case EVENT_KEY2S:
                    Drv_Timer_Create(&StateTimerID,5000,1,NULL);
                    StateSys = STATE_DISWTEM;
                    break;
                case EVENT_KEY3S:
                case EVENT_KEY3L:
                    if(st_Fsm_SetupParam.OptBit.SwTemCTRMode)
                    {
                        if(++st_Fsm_CtrParam.DiffTemAim > st_Fsm_SetupParam.DiffHighTHR)
                        {
                             st_Fsm_CtrParam.DiffTemAim = st_Fsm_SetupParam.DiffHighTHR;
                        }
                        Drv_Eep_CompareSave(FSM_EEPDIFFADDR,(const u8 *)&(st_Fsm_CtrParam.DiffTemAim),2);
                    }
                    else
                    {
                        if(++st_Fsm_CtrParam.ConstTemAim > st_Fsm_SetupParam.ConstHighTHR)
                        {
                            st_Fsm_CtrParam.ConstTemAim = st_Fsm_SetupParam.ConstHighTHR;
                        }
                        Drv_Eep_CompareSave(FSM_EEPCONSTADDR,(const u8 *)&(st_Fsm_CtrParam.ConstTemAim),2);
                    }
                    break;
                    
                case EVENT_KEY4S:
                case EVENT_KEY4L:
                    if(st_Fsm_SetupParam.OptBit.SwTemCTRMode)
                    {
                        if(--st_Fsm_CtrParam.DiffTemAim < st_Fsm_SetupParam.DiffLowTHR)
                        {
                            st_Fsm_CtrParam.DiffTemAim = st_Fsm_SetupParam.DiffLowTHR;
                        }
                        Drv_Eep_CompareSave(FSM_EEPDIFFADDR,(const u8 *)&(st_Fsm_CtrParam.DiffTemAim),2);
                    }
                    else
                    {
                        if(--st_Fsm_CtrParam.ConstTemAim < st_Fsm_SetupParam.ConstLowTHR)
                        {
                            st_Fsm_CtrParam.ConstTemAim = st_Fsm_SetupParam.ConstLowTHR;
                        }
                        Drv_Eep_CompareSave(FSM_EEPCONSTADDR,(const u8 *)&(st_Fsm_CtrParam.ConstTemAim),2);
                    }
                    break;
                    
                #if SIMULATION
                case EVENT_KEY2L:
                #else
                case EVENT_KEY12L:
                #endif
                    StateMenu = STATE_F01;
                    StateSys = STATE_SETUP;
                    break;
                default:
                    break;
            }

            if(Event >= EVENT_ERR0_OVERLOAD && Event != EVENT_NOERR)
            {
                StateSys = STATE_ERR;
            }
            
            break;
            
        case STATE_DISWTEM:
            if(st_Fsm_SetupParam.OptBit.SwRoomTemDis)
                Drv_UI_NumDis(0,1,st_Fsm_CtrParam.RoomTem);
            else
                Drv_UI_NumDis(0,1,250);
                
            Drv_UI_NumFlash(0x07);

            if(Drv_Timer_TimeOutChk(&StateTimerID) || StateTimerID == TIMER_NOTIMER)
            {
                StateSys = STATE_NORMAL;
                Drv_UI_NumFlash(0x00);
            }
            
            #if SIMULATION
            if(Event == EVENT_KEY2L)
            #else
            if(Event == EVENT_KEY12L)
            #endif
            {
                StateMenu = STATE_F01;
                StateSys = STATE_SETUP;
                Drv_UI_NumFlash(0x00);
            }
            break;
            
        case STATE_SETUP:
            if(Event >= EVENT_ERR0_OVERLOAD && Event != EVENT_NOERR)
            {
                StateSys = STATE_ERR;
            }
        case STATE_SETUPERR:
            if(st_Fsm_SetupParam.Password != 0 && PasswordOK == 0 && StateMenu != STATE_F21)
            {
                StateMenu = STATE_F99;
            }
            switch(StateMenu)
            {
                case STATE_F01:
                    Param = (s16 *)(&st_Fsm_SetupParam.HighTemTHR);
                    ValueMax = 1000;
                    ValueMin = 102;
                    floatflag = 1;
                    break;
                    
                case STATE_F02:
                    Param = (s16 *)(&st_Fsm_SetupParam.LowTemTHR);
                    ValueMax = 500;
                    ValueMin = -400;
                    floatflag = 1;
                    break;
                    
                case STATE_F03:
                    Param = (s16 *)(&st_Fsm_SetupParam.ConstHighTHR);
                    ValueMax = 1000;
                    ValueMin = st_Fsm_SetupParam.ConstLowTHR;
                    floatflag = 1;
                    break;
                    
                case STATE_F04:
                    Param = (s16 *)(&st_Fsm_SetupParam.ConstLowTHR);
                    ValueMax = st_Fsm_SetupParam.ConstHighTHR;
                    ValueMin = -400;
                    floatflag = 1;
                    break;

                case STATE_F05:
                    Param = (s16 *)(&st_Fsm_SetupParam.DiffHighTHR);
                    ValueMax = 99;
                    ValueMin = st_Fsm_SetupParam.DiffLowTHR;
                    floatflag = 1;
                    break;
                    
                case STATE_F06:
                    Param = (s16 *)(&st_Fsm_SetupParam.DiffLowTHR);
                    ValueMax = st_Fsm_SetupParam.DiffHighTHR;
                    ValueMin = -99;
                    floatflag = 1;
                    break;
             
                case STATE_F07:                  
                    Param = (s16 *)(&st_Fsm_SetupParam.CompRunTHR);
                    ValueMax = 99;
                    ValueMin = st_Fsm_SetupParam.CompStopTHR;
                    floatflag = 1;
                    break;

                case STATE_F08:
                    Param = (s16 *)(&st_Fsm_SetupParam.CompStopTHR);
                    ValueMax = st_Fsm_SetupParam.CompRunTHR;
                    ValueMin = -99;
                    floatflag = 1;
                    break;
                    
                case STATE_F09:
                    Param = (s16 *)(&st_Fsm_SetupParam.CompDlyTHR);
                    ValueMax = 250;
                    ValueMin = 0;
                    floatflag = 0;
                    break;
                             
                case STATE_F10:
                    Param = (s16 *)(&st_Fsm_SetupParam.EngyLowTHR);
                    ValueMax = 60;
                    ValueMin = 0;
                    floatflag = 0;
                    break;
                    
                case STATE_F11:
                    Param = (s16 *)(&st_Fsm_SetupParam.RoomTemCal);
                    ValueMax = 400;
                    ValueMin = -400;
                    floatflag = 1;
                    break;
                    
                case STATE_F12:
                    Param = (s16 *)(&st_Fsm_SetupParam.WatTemCal);
                    ValueMax = 400;
                    ValueMin = -400;
                    floatflag = 1;
                    break;       
                    
                case STATE_F13:
                    Param = (u16 *)(&st_Fsm_SetupParam.OptBit);
                    bitindex = 0;
                    break;
                    
                case STATE_F14:
                    bitindex = 1;
                    break;
                    
                case STATE_F15:
                    bitindex = 2;        
                    break;
                    
                case STATE_F16:
                    bitindex = 3;
                    break;
                    
                case STATE_F17:
                    bitindex = 4;
                    break;
                    
                case STATE_F18:
                    bitindex = 5;
                    break;
                    
                case STATE_F19:
                    bitindex = 6;
                    break;
                    
                case STATE_F20:
                    bitindex = 7;
                    break; 
                    
                case STATE_F21:
                    Param = (u16 *)(&st_Fsm_SetupParam.Password);
                    ValueMax = 999;
                    ValueMin = 0;
                    floatflag = 0;
                    break;
                    
                case STATE_F99:
                    Param = (u16 *)(&Password);
                    ValueMax = 999;
                    ValueMin = 0;
                    floatflag = 0;
                    break;
            }

            Drv_UI_StrDis(0,0xF,(StateMenu-3)/10,(StateMenu-3)%10);
            
            if(Param != NULL)
            {
                if(bitindex != 0xff)
                {
                    if(REDBIT((*Param),bitindex))
                    {
                        Drv_UI_StrDis(1,UI_NUMDUMMY,UI_STRO,UI_STRN);
                    }
                    else
                    {
                        Drv_UI_StrDis(1,UI_STRO,UI_STRF,UI_STRF);
                    }  
                }
                else
                {
                    Drv_UI_NumDis(1,floatflag,(s16)*Param);
                }
            }

            if((Event == EVENT_KEY3S || Event == EVENT_KEY3L)&& Param != NULL)
            {
                ParamAlter = TRUE;
                if(bitindex != 0xff)
                {
                    REDBIT(*Param,bitindex) ? CLRBIT(*Param,bitindex):SETBIT(*Param,bitindex);
                }
                else
                {
                    ((*Param)>=1000 || (*Param)<= -100)?((*Param)+=10):((*Param)++);

                    if((*Param) > ValueMax)
                    {
                        *Param = ValueMax;
                    }
                }
            }
            
            if((Event == EVENT_KEY4S || Event == EVENT_KEY4L)&& Param != NULL)
            {
                ParamAlter= TRUE;
                if(bitindex != 0xff)
                {
                    REDBIT(*Param,bitindex) ? CLRBIT(*Param,bitindex):SETBIT(*Param,bitindex);
                }
                else
                {
                    ((*Param)>=1000 || (*Param)<= -100)?((*Param)-= 10):((*Param)--);
                    
                    if((*Param) < ValueMin)
                    {
                        *Param = ValueMin;
                    }
                }
            }

            if((bitindex == 7 && REDBIT(*Param,bitindex)) || Event == EVENT_KEY14L)
            {
                temp = 0;
                Drv_Eep_CompareSave(FSM_EEPMAGADDR,(const u8 *)&temp,1);
                CLRBIT(*Param,bitindex);
                Drv_UI_LedDis(0,UI_LEDALL);
                StateSys = STATE_POWEOFF;
            }
            
            if(Event == EVENT_KEY2S)
            {
                if(StateMenu == STATE_F99)
                {
                    if(Password == st_Fsm_SetupParam.Password)
                    {
                        PasswordOK = 1;
                        Password = 0;
                        StateMenu = STATE_F01;
                    }
                    else
                    {
                        PasswordOK = 0;
                    }
                }
                else
                {
                    if((++StateMenu)>STATE_F21)
                    {
                        StateMenu = STATE_F01;
                    }
                    
                    if(ParamAlter)
                    {
                        ParamAlter = FALSE;
                        Drv_Eep_CompareSave(FSM_EEPSETUPADDR,(const u8 *)(&st_Fsm_SetupParam),sizeof(st_Fsm_SetupParam));
                    }
                }
            }

            if(Event == EVENT_KEY1S)
            {
                PasswordOK = 0;
                Password = 0;
                
                if(ParamAlter)
                {
                    ParamAlter = FALSE;
                    Drv_Eep_CompareSave(FSM_EEPSETUPADDR,(const u8 *)(&st_Fsm_SetupParam),sizeof(st_Fsm_SetupParam));
                }
                
                if(StateSys == STATE_SETUPERR)
                {
                    StateSys = STATE_POWEOFF;
                    Drv_Timer_Cancel(&SpkAlarmTmrID);
                    App_fsm_Spkoff();
                    Drv_UI_LedDis(0,UI_LEDALL);
                    Drv_UI_LedDis(0,UI_LEDALARMF);
                }
                else
                {
                    StateSys = STATE_NORMAL;
                }
            }
           
            break;
            
        case STATE_ERR:
        
            if(Event >= EVENT_ERR0_OVERLOAD && Event != EVENT_NOERR)
            {
                if(SpkAlarmTmrID == TIMER_NOTIMER)
                {
                    Drv_Timer_Create(&SpkAlarmTmrID,250,TIMER_INFINITE,App_fsm_Spkcb);
                }
                
                Drv_UI_LedDis(1,UI_LEDALARM);
                Drv_UI_LedDis(1,UI_LEDALARMF);

                HW_ALARMPINOUT(0);
                
                switch(Event)
                {
                    case EVENT_ERR0_OVERLOAD:
                        ErrCode = 0;
                        App_fsm_CompOnOff(0);
                        if(!st_Fsm_SetupParam.OptBit.SwPumpOnOff)
                        App_fsm_PumpOnOff(0);
                        break;
                    case EVENT_ERR1_PHASEERR:
                        ErrCode = 1;
                        App_fsm_CompOnOff(0);
                        App_fsm_PumpOnOff(0);
                        break;
                    case EVENT_ERR2_OPS:
                        ErrCode = 2;
                        App_fsm_CompOnOff(0);
                        if(!st_Fsm_SetupParam.OptBit.SwPumpOnOff)
                        App_fsm_PumpOnOff(0);
                        break;
                    case EVENT_ERR3_HVERR:
                        ErrCode = 3;
                        App_fsm_CompOnOff(0);
                        if(!st_Fsm_SetupParam.OptBit.SwPumpOnOff)
                        App_fsm_PumpOnOff(0);
                        break;
                    case EVENT_ERR4_LVERR:
                        ErrCode = 4;
                        App_fsm_CompOnOff(0);
                        if(!st_Fsm_SetupParam.OptBit.SwPumpOnOff)
                        App_fsm_PumpOnOff(0);
                        break;
                    case EVENT_ERR5_EGYLOW:
                        ErrCode = 5;
                        break;
                    case EVENT_ERR6_RNTCERR:
                        ErrCode = 6;
                        App_fsm_CompOnOff(0);
                        if(!st_Fsm_SetupParam.OptBit.SwPumpOnOff)
                        App_fsm_PumpOnOff(0);
                        break;
                    case EVENT_ERR7_WNTCERR:
                        ErrCode = 7;
                        App_fsm_CompOnOff(0);
                        if(!st_Fsm_SetupParam.OptBit.SwPumpOnOff)
                        App_fsm_PumpOnOff(0);
                        break;
                    case EVENT_ERR8_OVERHEAR:
                        ErrCode = 8;
                        break;
                    case EVENT_ERR9_LOWTEMP:
                        ErrCode = 9;
                        App_fsm_CompOnOff(0);
                        if(!st_Fsm_SetupParam.OptBit.SwPumpOnOff)
                        App_fsm_PumpOnOff(0);
                        break;
                    case EVENT_ERR10_NOTRUN:
                        ErrCode = 10;
                        break;
                     
                    default:
                        break;
                }
                
                Drv_UI_StrDis(0,UI_STRA,UI_STRL,ErrCode);
            }
            
            #if SIMULATION
            if(Event == EVENT_KEY2L)
            #else
            //if(Event == EVENT_KEY12L || Event == EVENT_NOERR)
            if(Event == EVENT_KEY12L)
            #endif
            {
                //HW_ALARMPINOUT(0);
                //Drv_UI_LedDis(0,UI_LEDALARM);
                //Drv_UI_LedDis(0,UI_LEDALARMF);
                //Drv_Timer_Cancel(&SpkAlarmTmrID);
                //App_fsm_Spkoff();
                //FSM_INITTIMER(StateTime);
                //if(Event == EVENT_KEY12L)
                {
                    StateMenu = STATE_F01;
                    StateSys = STATE_SETUPERR;
                }
                //else
                //{
                //    StateSys = STATE_NORMAL;
                //}
            }
            
            if(Event == EVENT_KEY1S)
            {
                HW_ALARMPINOUT(0);
                Drv_UI_LedDis(0,UI_LEDALL);
                Drv_Timer_Cancel(&SpkAlarmTmrID);
                App_fsm_Spkoff();
                StateSys = STATE_POWEOFF;
            }
            
            break;
    }
    
    if((StateSys != STATE_ERR) && (StateSys != STATE_POWEOFF) && (StateSys != STATE_SETUPERR))
    {
        App_fsm_CtrLogic();
    }
}
