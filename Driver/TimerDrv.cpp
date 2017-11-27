/*==========================================================*/
/* Title		:	TimerDrv.cpp								*/
/* Description	: 	Class_ADCDrv realization 				*/
/* Date			:	Apr.2008								*/
/*==========================================================*/
#include "TimerDrv.h"

/**********************************************************************************
�������ƣ�	Drv_CPUTimerInit()
����������	CPUTimerģ���ʼ��
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
��������	Drv_SysTimer()
����������	40ms��ʱģ��
��������:	��
************************************************************************************/
	void	Class_TimerDrv::Drv_SysTimer()

	{
		INT16	i16temp;
		
		objSystem.m_st_wFlagSystem.bTimer1ms = 0;							//��1ms��ʱ��־						
		i16temp = ((INT32)m_u16ClkUs_0)/K1msCnst;
		
		if ( i16temp != m_i16NumFor1ms )	
		{
			m_i16NumFor1ms = i16temp;
			objSystem.m_st_wFlagSystem.bTimer1ms = 1;						//��1ms��ʱ��־
		}
		
		objSystem.m_st_wFlagSystem.bTimer14ms = 0;							//��14ms��ʱ��־
	
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
			objSystem.m_st_wFlagSystem.bTimer14ms = 1;						//��14ms��ʱ��־
			objFaultRecord.m_u16wClkCntPer14ms++;

			//CAN��ʱ������
			ProtocolObj.Timer_Expire_Handle();
		}

		objSystem.m_st_wFlagSystem.bTimer40ms = 0;							//��40ms��ʱ��־
		i16temp = ((INT32)m_u16ClkUs_0)/K40msCnst;					//40ms��
		
		if ( i16temp != m_i16NumFor40ms )	
		{
			m_i16NumFor40ms = i16temp;
			objSystem.m_st_wFlagSystem.bTimer40ms = 1;						//��40ms��ʱ��־
		
		}										
		if ( m_u16ClkH_0 != m_u16ClkHOld )
		{
			objXfer.m_i16wBptoinvTimes = 0;									//�������·��������
			m_u16ClkHOld = m_u16ClkH_0;									//����Сʱ����
		}
	}

/************************************************************************************
//�������ƣ�    Drv_Int_ClockMaintain()
//��������:	    ά��ʵʱʱ��,������ʱ֮��
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
	//�������ݺ���ʷ��¼ʱ��		
		objSystem.m_u16TimerBig1ms = 0;										//��1.125ms��ʱ��־
		i16temp = ((INT32)m_u16ClkUs_0)/KBig1msCnst;						
		if ( i16temp != m_i16NumForBig1ms )	
		{
			m_i16NumForBig1ms = i16temp;
			objSystem.m_u16TimerBig1ms = 1;						//��1ms��ʱ��־
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
