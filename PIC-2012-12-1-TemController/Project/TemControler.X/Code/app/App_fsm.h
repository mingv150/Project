/*******************************************************************************
*Copyright(C),2012-2013,mingv150,All rights reserved
*FileName:
*Description:
*Author:
*Version:
*Date:
*******************************************************************************/
#ifndef _APP_FSM_H_
#define _APP_FSM_H_


enum state
{
    STATE_POWEOFF = 0,
    STATE_NORMAL,
    STATE_DISWTEM, 
    STATE_SETUP,
    STATE_F01,
    STATE_F02,
    STATE_F03,
    STATE_F04,
    STATE_F05,
    STATE_F06,
    STATE_F07,
    STATE_F08,
    STATE_F09,
    STATE_F10,
    STATE_F11,
    STATE_F12,
    STATE_F13,
    STATE_F14,
    STATE_F15,
    STATE_F16,
    STATE_F17,
    STATE_F18,
    STATE_F19,
    STATE_F20,
    STATE_F21,
    STATE_F22,
    STATE_F23,
    STATE_F24,
    STATE_F25,
    STATE_F99 = 99+3,
    STATE_ERR,
    STATE_SETUPERR,
};



#define FSM_INITTIMER(cnt) (cnt) = 0
#define FSM_TIMEOUTCHK(cnt,maxcnt) (((cnt)>(maxcnt))?1:0)

#define FSM_DISWTMTIME 10000
#define FSM_SETUPNDTIME 10000

#define FSM_PARAMINVALID 0xffff


#define FSM_TEMINVAILD 2000



typedef struct CtrParam
{
    s16 WatTem;
    s16 RoomTem;
    s16 AllTemAim;
    s16 ConstTemAim;
    s16 DiffTemAim;
}t_CtrParam;

typedef struct SetupParam
{
    s16 HighTemTHR;
    s16 LowTemTHR;
    
    s16 ConstHighTHR;
    s16 ConstLowTHR;
    
    s16 DiffHighTHR;
    s16 DiffLowTHR;
    
    s16 CompRunTHR;
    s16 CompStopTHR;
    
    s16 CompDlyTHR;
    s16 EngyLowTHR;
    s16 RoomTemCal;
    s16 WatTemCal;
    u16 Password;
    struct 
    {
        u8 SwRoomTemDis:1;
        u8 SwPumpOnOff:1;
        u8 SwPumpwithCps:1;
        u8 SwCpsCTR:1;       
        u8 SwTemCTRMode:1;
        u8 SwPhaseChk:1;
        u8 SwRemote:1;
        u8 SwReserve:1;
        u8 SwReserveByte;
    }OptBit;
    
}t_SetupParam;

#define FSM_EEPMAGICNUM 0xA5
#define FSM_EEPMAGADDR 0x7f
#define FSM_EEPSETUPADDR (FSM_EEPMAGADDR + 2)
#define FSM_EEPOTHERADDR (FSM_EEPSETUPADDR + sizeof(t_SetupParam))
#define FSM_EEPDIFFADDR (FSM_EEPOTHERADDR + 2)
#define FSM_EEPCONSTADDR (FSM_EEPDIFFADDR + 2)


#ifndef _APP_FSM_C_

void App_Fsm_InitParam(void);
void App_Fsm_RecvKeyEvent(u8 KeyValue);
void App_Fsm_RecvSensorEvent(u8 InputValue);
void App_fsm_StateDispose();
void App_fsm_CtrLogic();
void App_fsm_ErrScan(void);
void App_Fsm_StartCheck(void);
void App_fsm_Spkoff(void);
void App_fsm_Spkon(void);
void App_Fsm_GetTemperature(void);


#endif

#endif

