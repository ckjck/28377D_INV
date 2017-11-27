/*==========================================================*/
/* Title		:	TimerDrv.cpp								*/
/* Description	: 	Class_ADCDrv realization 				*/
/* Date			:	Apr.2008								*/
/*==========================================================*/
#include "TimerDrv.h"

/**********************************************************************************
函数名称：	Drv_CPUTimerInit()
功能描述：	CPUTimer模块初始化
***********************************************************************************/
void	Class_TimerDrv::Drv_CPUTimerInit()
{
	// Only CPU Timer0 is used
	CpuTimer0Regs.PRD.all  = 0xFFFFFFFF;					// Initialize timer period to maximum, down counter
	CpuTimer0Regs.TPR.all  = 0;								// Initialize pre-scale counter to divide by 1 (SYSCLKOUT)
	CpuTimer0Regs.TPRH.all = 0;
	CpuTimer0Regs.TCR.bit.TSS = 1;							// Make sure timer is stopped
	CpuTimer0Regs.TCR.bit.TRB = 1;							// Reload all counter register with period value
	CpuTimer0Regs.TCR.bit.TIE = 0;							// The CPU-Timer0 interrupt is disabled
	CpuTimer0Regs.TCR.bit.TSS = 0;							// Restart the timer0
}
/************************************************************************************
函数名：	Drv_SysTimer()
功能描述：	40ms定时模块
函数调用:	无
************************************************************************************/
	void	Class_TimerDrv::Drv_SysTimer()

	{
		INT16	i16temp;
		
		objSystem.m_st_wFlagSystem.bTimer1ms = 0;							//清1ms定时标志						
		i16temp = ((INT32)m_u16ClkUs_0)/K1msCnst;
		
		if ( i16temp != m_i16NumFor1ms )	
		{
			m_i16NumFor1ms = i16temp;
			objSystem.m_st_wFlagSystem.bTimer1ms = 1;						//置1ms定时标志
		}
		
		objSystem.m_st_wFlagSystem.bTimer14ms = 0;							//清14ms定时标志
	
		if(objMonInterface.m_st_wUPSModeRun.bFreqOut == 1)
		{							
			i16temp = ((INT32)m_u16ClkUs_0)/K14msCnst;				//14ms@60Hz
		}
		else
		{        
			i16temp = ((INT32)m_u16ClkUs_0)/K16msCnst;				//16ms@50Hz
		}

		if ( i16temp!= m_i16NumFor14ms )	
		{
			m_i16NumFor14ms = i16temp;
			objSystem.m_st_wFlagSystem.bTimer14ms = 1;						//置14ms定时标志
			objFaultRecord.m_u16wClkCntPer14ms++;

			//CAN定时器处理
			ProtocolObj.Timer_Expire_Handle();
		}

		objSystem.m_st_wFlagSystem.bTimer40ms = 0;							//清40ms定时标志
		i16temp = ((INT32)m_u16ClkUs_0)/K40msCnst;					//40ms到
		
		if ( i16temp != m_i16NumFor40ms )	
		{
			m_i16NumFor40ms = i16temp;
			objSystem.m_st_wFlagSystem.bTimer40ms = 1;						//置40ms定时标志
		
		}										
		if ( m_u16ClkH_0 != m_u16ClkHOld )
		{
			objXfer.m_i16wBptoinvTimes = 0;									//清过载旁路切逆变次数
			m_u16ClkHOld = m_u16ClkH_0;									//更新小时备份
		}
	}

/************************************************************************************
//函数名称：    Drv_Int_ClockMaintain()
//功能描述:	    维护实时时钟,供程序定时之用
************************************************************************************/

#pragma CODE_SECTION(".Epwm1Funcs") 			

void 	Class_TimerDrv::Drv_Int_ClockMaintain(void)
{
    INT16	i16temp;
    m_u16ClkUs_0++;
	if(m_u16ClkUs_0>=CLKUSPRDCNST)	
	{
		m_u16ClkUs_0 = 0;			                 // 1/(3125*2*2*10ns)=8000
		m_u16ClkS_0++;
		if(m_u16ClkS_0>=3600)
		{
			m_u16ClkS_0 = 0;
			m_u16ClkH_0++;
			if(m_u16ClkH_0 >= 32000)
			{
				 m_u16ClkH_0 = 0;
			}
		}
	}
	//故障数据和历史纪录时基		
		objSystem.m_u16TimerBig1ms = 0;										//清1.125ms定时标志
		i16temp = ((INT32)m_u16ClkUs_0)/KBig1msCnst;						
		if ( i16temp != m_i16NumForBig1ms )	
		{
			m_i16NumForBig1ms = i16temp;
			objSystem.m_u16TimerBig1ms = 1;						//置1ms定时标志
		}
		
}

//----------------------------------
void 	Class_TimerDrv::Drv_usDelay(INT32 Count)
{
	while(Count>0)
    {
    	Count--;
    	asm(" RPT #99 || NOP");
    }
}


//===========================================================================
// End of file.
//===========================================================================
