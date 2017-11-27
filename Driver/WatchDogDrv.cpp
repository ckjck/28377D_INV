/*==========================================================*/
/* Title		:	WatchDogDrv.cpp								*/
/* Description	: 	Class_ADCDrv realization 				*/
/* Date			:	Apr.2008								*/
/*==========================================================*/
#include "WatchDogDrv.h"


/************************************************************************************/
void Class_WatchDogDrv::Drv_KickDog(void)
{	
	m_i16KickDogCntPerMloop++;
	if(m_i16KickDogCntPerMloop >= K2msMainCnst)						// 2ms喂狗一次
	{
		m_i16KickDogCntPerMloop = 0;
    	EALLOW;
		if ( objTimerDrv.m_u16ClkUs_0 == objTimerDrv.m_u16ClkUsBack )					// 监视定时中断，故障下激活
		{
			EPwm1Regs.ETCLR.bit.INT = 1;				// Clear INT flag for EPWM1 Period Interrupt
			PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;		// Acknowledge this interrupt to receive more interrupts from group 3
		}
		objTimerDrv.m_u16ClkUsBack = objTimerDrv.m_u16ClkUs_0;							// 更新时钟单位备份
		
    	WdRegs.WDKEY.bit.WDKEY = 0x0055;
    	WdRegs.WDKEY.bit.WDKEY = 0x00AA; 				// This will clear the watchdog counter
		EDIS;	
	}
}

// This function disables the watchdog timer.
void Class_WatchDogDrv::Drv_DisableDog(void)
{
    volatile Uint16 temp;

    //
    // Grab the clock config first so we don't clobber it
    //
    EALLOW;
    temp = WdRegs.WDCR.all & 0x0007;
    WdRegs.WDCR.all = 0x0068 | temp;
    EDIS;

}

// This function enables the watchdog timer.
void Class_WatchDogDrv::Drv_EnableDog(void)
{
    EALLOW;
    WdRegs.WDCR.all = 0x002C;
    EDIS;
}

//===========================================================================
// End of file.
//===========================================================================
