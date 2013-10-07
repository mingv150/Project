/*******************************************************************************
*Copyright(C),2012-2013,mingv150,All rights reserved
*FileName:
*Description:
*Author:
*Version:
*Date:
*******************************************************************************/

#define _DRV_TIMER_C_
/*Common*/
#include <htc.h>
#include "../Common.h"
#include "../Model.h"
#include "../oopc.h"

/*myself*/
#include "Drv_Timer.h"

#include "Drv_NTCSensor.h"


static t_TimerAttr st_Timer_Attr[TIMER_TIMERNUM]=
{
    {NULL,0,0,0,0,0,0,0,NULL},
    {NULL,0,0,0,0,0,0,0,NULL},
    {NULL,0,0,0,0,0,0,0,NULL},
    {NULL,0,0,0,0,0,0,0,NULL},
    {NULL,0,0,0,0,0,0,0,NULL}
};


/*******************************************************************************
*Function:
*Description:
*Input:
*Output:
*******************************************************************************/
#if 0
void Drv_Timer_Init(void)
{
    u8 i;

    for(i=0;i<TIMER_TIMERNUM;i++)
    {
		st_Timer_Attr[i].pTimerID = NULL;
		st_Timer_Attr[i].Reserve = 0;
		st_Timer_Attr[i].IsActive = 0;
		st_Timer_Attr[i].RepeatTotal = 0;
		st_Timer_Attr[i].RepeatCount = 0;
		st_Timer_Attr[i].Tick = 0;
		st_Timer_Attr[i].Time = 0;
		st_Timer_Attr[i].TimeOut = 0;
		st_Timer_Attr[i].CallBack = NULL;
    }
}
#endif

/*******************************************************************************
*Function:
*Description:
*Input:
*Output:
*******************************************************************************/
u8 Drv_Timer_Create(u8 *pTimerID,u32 MSecs,u8 Repeat,t_TimerCB FunCb)
{
    u8 i = 0;

    for(i=0;i<TIMER_TIMERNUM;i++)
    {
        if(!st_Timer_Attr[i].IsActive)
        {
            if(Repeat == 0)
            {
                Repeat = 1;
            }
            
            *pTimerID = (i+1);
            st_Timer_Attr[i].pTimerID = pTimerID;       
            st_Timer_Attr[i].Tick = 0;
            st_Timer_Attr[i].Time = MSecs;
			st_Timer_Attr[i].RepeatCount = 0;
            st_Timer_Attr[i].RepeatTotal = Repeat;
            st_Timer_Attr[i].TimeOut = 0;
            st_Timer_Attr[i].CallBack = FunCb;
            st_Timer_Attr[i].IsActive = 1;
            return TRUE;
        }
    }
    
    *pTimerID = TIMER_NOTIMER;
    return FALSE;
}


/*******************************************************************************
*Function:
*Description:
*Input:
*Output:
*******************************************************************************/
void Drv_Timer_Cancel(u8 *pTimerID)
{
    u8 i;

    for(i=0;i<TIMER_TIMERNUM;i++)
    {
        if((st_Timer_Attr[i].pTimerID == pTimerID) && (*pTimerID == (i+1)))
        {
            st_Timer_Attr[i].IsActive = 0;
            st_Timer_Attr[i].RepeatTotal = 0;
            st_Timer_Attr[i].RepeatCount = 0;
            st_Timer_Attr[i].Tick = 0;
            st_Timer_Attr[i].Time = 0;
            st_Timer_Attr[i].TimeOut = 0;
            st_Timer_Attr[i].CallBack = NULL;
            st_Timer_Attr[i].pTimerID = NULL;
            break;
        }
    }

    *pTimerID = TIMER_NOTIMER;
}


/*******************************************************************************
*Function:
*Description:
*Input:
*Output:
*******************************************************************************/
u8 Drv_Timer_TimeOutChk(u8 *pTimerID)
{
	u8 i;
	
	for(i=0;i<TIMER_TIMERNUM;i++)
	{
		if(st_Timer_Attr[i].pTimerID == pTimerID && (*pTimerID == (i+1)))
		{
			if(st_Timer_Attr[i].TimeOut)
			{
				st_Timer_Attr[i].IsActive = 0;
                st_Timer_Attr[i].RepeatTotal = 0;
                st_Timer_Attr[i].RepeatCount = 0;
                st_Timer_Attr[i].Tick = 0;
                st_Timer_Attr[i].Time = 0;
                st_Timer_Attr[i].TimeOut = 0;
                st_Timer_Attr[i].CallBack = NULL;
                st_Timer_Attr[i].pTimerID = NULL;
                *pTimerID = TIMER_NOTIMER;
				return TRUE;
			}
			break;
		}
	}
	return FALSE;
}


/*******************************************************************************
*Function:
*Description:
*Input:
*Output:
*******************************************************************************/
void Drv_Timer_Scan(void)
{
    u8 i;
    //u8 IDtemp = 0;

    for(i=0;i<TIMER_TIMERNUM;i++)
    {
        Drv_NTC_MesureIRQ();
        
        if(st_Timer_Attr[i].IsActive)
        {
            if(st_Timer_Attr[i].Tick == st_Timer_Attr[i].Time)
            {
                st_Timer_Attr[i].Tick = 0;

                if(st_Timer_Attr[i].RepeatTotal == TIMER_INFINITE)
                {
                    st_Timer_Attr[i].TimeOut = 1;
                    if(st_Timer_Attr[i].CallBack != NULL)
                    {
                        st_Timer_Attr[i].CallBack();

                        Drv_NTC_MesureIRQ();
                    }
                }
                else
                {
                    st_Timer_Attr[i].RepeatCount++;

                    if(st_Timer_Attr[i].RepeatCount <= st_Timer_Attr[i].RepeatTotal)
                    {
                        st_Timer_Attr[i].TimeOut = 1;
                        if(st_Timer_Attr[i].CallBack != NULL)
                        {
                            st_Timer_Attr[i].CallBack();

                            Drv_NTC_MesureIRQ();
                        }
                    }
                    else
                    {
                        if(st_Timer_Attr[i].CallBack != NULL)
                        {
                            /*Destory automatically */
                            st_Timer_Attr[i].IsActive = 0;
                            st_Timer_Attr[i].RepeatTotal = 0;
                            st_Timer_Attr[i].RepeatCount = 0;
                            st_Timer_Attr[i].Tick = 0;
                            st_Timer_Attr[i].Time = 0;
                            st_Timer_Attr[i].TimeOut = 0;
                            st_Timer_Attr[i].CallBack = NULL;
                            st_Timer_Attr[i].pTimerID = NULL;
                            *st_Timer_Attr[i].pTimerID = TIMER_NOTIMER;
                        }
                    }
                }
            }
            else
            {
                st_Timer_Attr[i].Tick++;
            }

            Drv_NTC_MesureIRQ();
        }
    }
}

