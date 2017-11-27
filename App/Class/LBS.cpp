/*==========================================================*/ 	
/* Title		:	LBS.cpp														*/ 	
/* Description	: 	Class_LBS definition											*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#include "LBS.h"




/************************************************************************************
//函数名称：    Dat_Int_LBSCapDeal()
//功能描述：	LBS和逆变过零信号检测(Cap2捕获LBS过零，Cap3捕获输出过零)
************************************************************************************/

//#pragma CODE_SECTION(".Epwm1Funcs") 			

void Class_LBS::Dat_Int_LBSCapDeal(void)
{
    // LBS过零捕获处理
	m_i16TLBSCnt_0++;
	m_st_LBSStatus.bCAPLBSFlag = 0;
	if(ECap2Regs.ECFLG.bit.CEVT1==1)
	{
		m_u32TLBSClkOld_0 = m_u32TLBSClk_0;
		m_u32TLBSClk_0 = ECap2Regs.CAP1;
		ECap2Regs.ECCLR.all = 0xFFFF;			 	// 清所有标志
			
		m_i32TLBS_0 = m_u32TLBSClk_0-m_u32TLBSClkOld_0;
		UPDNLMT32(m_i32TLBS_0,InvSyncCap45HzCnt,InvSyncCap66HzCnt);
			
		m_i16TLBSCnt_0 = 0;
		m_st_LBSStatus.bCAPLBSFlag = 1;
	}
	else if(m_i16TLBSCnt_0>=K30HzPointCntCnst)			// Flbs<30Hz
	{
		m_i16TLBSCnt_0 = 100;
		m_i32TLBS_0 = 0;
		m_u32TLBSClk_0 = ECap2Regs.TSCTR;
	}
}

/************************************************************************************
函数名：	Alg_LBSSignal()
功能描述：	本模块处理LBS信号，设置LBS总线状态
函数调用：	无
************************************************************************************/
	void	Class_LBS::Alg_LBSSignal()

	{
		//zqf 060413--隔离态或TEST模式下不发LBS信号
		if(objMonInterface.m_st_wUPSModeRun.bLBS==0 || objParallel.m_st_wParaMaint.bParaSignalMask == 1||objSystem.m_st_wFlagSystem.bUPSTestMode==1) 
		{
			objParallel.m_st_wParaSignal.bLBSAct=0;						//不发脉冲
			objParallel.m_st_wParaSignal.bLBSWide=0;
		}

		else if(objXfer.m_st_wXferCmd.bSwitchStatus==1)			//逆变侧供电
		
		{
			if(objParallel.m_st_wParaSignal.bMasterOnline==1&&objMonInterface.m_st_wUPSModeRun.bLBS==2)
			{
				if(m_st_LBSStatus.bLBSDetect!=2) 
				{
					objParallel.m_st_wParaSignal.bLBSAct=1;				//发窄脉冲
					objParallel.m_st_wParaSignal.bLBSWide=0;
				}
				else
				{
					objParallel.m_st_wParaSignal.bLBSAct=0;				//不发脉冲
					objParallel.m_st_wParaSignal.bLBSWide=0;
				}				
			}
			else	
			{
				objParallel.m_st_wParaSignal.bLBSAct=0;					//不发脉冲
				objParallel.m_st_wParaSignal.bLBSWide=0;
			}

		}
		else if(objXfer.m_st_XferResult.bBpSTS==1 && objBypass.m_st_FlagVbpTotal.bBpTrace==1)
		{
			objParallel.m_st_wParaSignal.bLBSAct=1;						//发宽脉冲
			objParallel.m_st_wParaSignal.bLBSWide=1;
		}
		else
		{
			objParallel.m_st_wParaSignal.bLBSAct=0;						//不发脉冲
			objParallel.m_st_wParaSignal.bLBSWide=0;
		}

		//Debug
		//if(objMonInterface.m_i16SelfAgingCurrRate == 80)
		//{
			//objParallel.m_st_wParaSignal.bLBSAct=0;						//发宽脉冲
			//objParallel.m_st_wParaSignal.bLBSWide=0;
		//}
		//LBS_SYN脉冲的接受判别
		if(objParallel.m_st_wParaMaint.bParaSignalMask == 1||objSystem.m_st_wFlagSystem.bUPSTestMode==1 )	
		{
			m_st_LBSStatus.bLBSDetect=0;					//LBS_NONE
		}
		else 
		{
			if(m_i16T1pCntLBSDct_0>=1350)					//1500*90%
			{
				m_st_LBSStatus.bLBSDetect=0;				//LBS_NONE
			}
			else if(m_i16T1pCntLBSDct_0>=525)   			//1500*35%
			{
				m_st_LBSStatus.bLBSDetect=2;				//LBS_BYP
			}

			else if(m_i16T1pCntLBSDct_0>=75)				//1500*5%
			{
				m_st_LBSStatus.bLBSDetect=1;				//LBS_INV
			}

			else
			{
				m_st_LBSStatus.bLBSDetect=0;				//LBS_NONE
			}
		}
		
		
		if(objSystem.m_st_wFlagSystem.bTimer40ms == 1)
		{
			m_i16LBSTimeDelayCnt_0++;
			if (m_i16LBSTimeDelayCnt_0 >= 1500)					//1分钟计数器
			{
				m_i16LBSTimeDelayCnt_0 = 1500;					//开机1分钟内不报LBS异常
			
				//LBS信号异常报故障处理
				if (objMonInterface.m_st_wUPSModeRun.bLBS!= 0)
				{
					m_st_LBSStatus.bLbsActive = 1;
					if (m_st_LBSStatus.bLBSDetect== 0)
					{
						m_i16LBSAbnormalCnt_0++;
						if (m_i16LBSAbnormalCnt_0 >= 25)		//1s
						{
							m_i16LBSAbnormalCnt_0 = 25;
							m_st_LBSStatus.bLbsAbnormal = 1;
						}
					}
					else
					{
						m_i16LBSAbnormalCnt_0--;
						if (m_i16LBSAbnormalCnt_0 <= 0)
						{
							m_i16LBSAbnormalCnt_0 =0;
							m_st_LBSStatus.bLbsAbnormal = 0;
						}
					}
				}
				else
				{
					m_st_LBSStatus.bLbsActive = 0;
					m_st_LBSStatus.bLbsAbnormal = 0;
				}
			
			}
		}	
		
	}
	
/************************************************************************************
函数名：	Alg_Int_LBSDetect()
功能描述：	LBS检测模块，检测LBS总线上的脉冲宽度或占空比
函数调用：	无
************************************************************************************/
	
	//#pragma CODE_SECTION(".Epwm1Funcs") 			
	
	void	Class_LBS::Alg_Int_LBSDetect(void)
	{
		if(m_i16T1pCntLBS_0<1500)	
		{
			m_i16T1pCntLBS_0++;
		}
		else
		{
			m_i16T1pCntLBSDct_0=m_i16T1pCntLBSDct0_0;
			m_i16T1pCntLBS_0=0;
			m_i16T1pCntLBSDct0_0=0;		
		}
		if(objParallel.m_st_wParaSignalF.bLBSDetect == 1)
		{
			m_i16T1pCntLBSDct0_0++;
		}
	}
	
//===========================================================================
// End of file.
//===========================================================================
