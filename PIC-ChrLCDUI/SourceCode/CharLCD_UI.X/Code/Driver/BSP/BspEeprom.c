/****************************************************************************
Copyright (C), 2014, Mingv150, All rights reserved
FileName: /Layer/Module/BspEeprom.c
Description:  
Author:  mingv150@163.com
Version:  
Changelog: 
*****************************************************************************/

#define _BspEeprom_C_
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
#include "BspEeprom.h"

/****************************************************************************
Global Data Structure:
*****************************************************************************/
static u8 BspEeprom_Purpose;

/****************************************************************************
Function: FunctionName
Description: 
Input: 
Output:
Notes: 
*****************************************************************************/
void BspEeprom_SaveParam(u8 StartAddr,const u8 *pSrc,u8 Size)
{
    u8 i;
    u8 GIETemp = 0;
    u8 ReadTemp = 0;
    
    for(i=0;i<Size;i++)
    {
	do
	{
		do{;}
		while(WR == 1);                      
		EEADRL = (StartAddr+i);                
		EEDATL = *(pSrc+i);                   
		GIETemp = 0;
		if(GIE)
			GIETemp = 1;
		GIE = 0;
		EEPGD = 0;                           
		WREN = 1;                         
		EECON2 = 0x55;                   
		EECON2 = 0xAA;             
		WR = 1;                        
		do{;}
		while(WR == 1);                     
		WREN = 0;                        
		EEIF = 0;
			
		do{;}
		while(RD == 1);                      
		EEADRL = (StartAddr+i);               
		EEPGD = 0;                            
		RD = 1;                              
		do{;}
		while(RD == 1);                   
		ReadTemp = EEDATL;               
		if(GIETemp)
			GIE = 1;
	}while(ReadTemp != *(pSrc+i));
    }	
}


/****************************************************************************
Function: Name
Description:
Input:
Output:
Notes:
*****************************************************************************/
void BspEeprom_GetParam(u8 StartAddr,u8 *pDes,u8 Size)
{
    u8 i;
	u8 temp1;
	u8 temp2;

    for(i=0;i<Size;i++)
    {
		do
		{
			do{;}
			while(RD==1);                       
			EEADRL=(StartAddr+i);
			EEPGD=0;                         
			RD=1;                            
			do{;}
			while(RD==1);                    
			temp1 = EEDATL;
		
			do{;}
			while(RD==1);                    
			EEADRL=(StartAddr+i);
			EEPGD=0;                         
			RD=1;                            
			do{;}
			while(RD==1);                      
			temp2 = EEDATL;
		}while(temp1^temp2);
		
		*(pDes+i) = temp2;
    }
}