/*==========================================================*/ 	
/* Title		:	LBS.cpp														*/ 	
/* Description	: 	Class_LBS definition											*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#include "LBS.h"




/************************************************************************************
//�������ƣ�    Dat_Int_LBSCapDeal()
//����������	LBS���������źż��(Cap2����LBS���㣬Cap3�����������)
************************************************************************************/

//#pragma CODE_SECTION(".Epwm1Funcs") 			

void Class_LBS::Dat_Int_LBSCapDeal(void)
{
    // LBS���㲶����
	m_i16TLBSCnt_0++;
	m_st_LBSStatus.bCAPLBSFlag = 0;
	if(ECap2Regs.ECFLG.bit.CEVT1==1)
	{
		m_u32TLBSClkOld_0 = m_u32TLBSClk_0;
		m_u32TLBSClk_0 = ECap2Regs.CAP1;
		ECap2Regs.ECCLR.all = 0xFFFF;			 	// �����б�־
			
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
��������	Alg_LBSSignal()
����������	��ģ�鴦��LBS�źţ�����LBS����״̬
�������ã�	��
************************************************************************************/
	void	Class_LBS::Alg_LBSSignal()

	{
		//zqf 060413--����̬��TESTģʽ�²���LBS�ź�
		if(objMonInterface.m_st_wUPSModeRun.bLBS==0 || objParallel.m_st_wParaMaint.bParaSignalMask == 1||objSystem.m_st_wFlagSystem.bUPSTestMode==1) 
		{
			objParallel.m_st_wParaSignal.bLBSAct=0;						//��������
			objParallel.m_st_wParaSignal.bLBSWide=0;
		}

		else if(objXfer.m_st_wXferCmd.bSwitchStatus==1)			//���๩��
		
		{
			if(objParallel.m_st_wParaSignal.bMasterOnline==1&&objMonInterface.m_st_wUPSModeRun.bLBS==2)
			{
				if(m_st_LBSStatus.bLBSDetect!=2) 
				{
					objParallel.m_st_wParaSignal.bLBSAct=1;				//��խ����
					objParallel.m_st_wParaSignal.bLBSWide=0;
				}
				else
				{
					objParallel.m_st_wParaSignal.bLBSAct=0;				//��������
					objParallel.m_st_wParaSignal.bLBSWide=0;
				}				
			}
			else	
			{
				objParallel.m_st_wParaSignal.bLBSAct=0;					//��������
				objParallel.m_st_wParaSignal.bLBSWide=0;
			}

		}
		else if(objXfer.m_st_XferResult.bBpSTS==1 && objBypass.m_st_FlagVbpTotal.bBpTrace==1)
		{
			objParallel.m_st_wParaSignal.bLBSAct=1;						//��������
			objParallel.m_st_wParaSignal.bLBSWide=1;
		}
		else
		{
			objParallel.m_st_wParaSignal.bLBSAct=0;						//��������
			objParallel.m_st_wParaSignal.bLBSWide=0;
		}

		//Debug
		//if(objMonInterface.m_i16SelfAgingCurrRate == 80)
		//{
			//objParallel.m_st_wParaSignal.bLBSAct=0;						//��������
			//objParallel.m_st_wParaSignal.bLBSWide=0;
		//}
		//LBS_SYN����Ľ����б�
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
			if (m_i16LBSTimeDelayCnt_0 >= 1500)					//1���Ӽ�����
			{
				m_i16LBSTimeDelayCnt_0 = 1500;					//����1�����ڲ���LBS�쳣
			
				//LBS�ź��쳣�����ϴ���
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
��������	Alg_Int_LBSDetect()
����������	LBS���ģ�飬���LBS�����ϵ������Ȼ�ռ�ձ�
�������ã�	��
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
