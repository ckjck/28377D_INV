/************************************************************************************
��������	App_SelfAgingAlgVarInit()	
����������	SelfAgingAlgorithm�������ʼ��		
�������ã�	
***********************************************************************************/
	 void	Class_SelfAgingAlgorithm::App_SelfAgingAlgVarInit(void)
	{
		m_i16AgingIindMdTgt = 1160;								//���ϻ�D�����Ϊ20%.
		m_i16AgingRefIQ_0 = 356;								//40uF���ݣ�2.8A����
		m_i16KSPWM = KSPWMCnst1;
	}
/************************************************************************************
��������App_SelfAgingAlgParamConfig()
************************************************************************************/
	void	Class_SelfAgingAlgorithm::App_SelfAgingAlgParamConfig(void)
	{
		//�ϵ�������ý���ǰ����ȷ�����ϻ�ģʽ�����ã����������������е���ģֵ��ӳ�䡣
		INT16 i16Temp;
		
		//��̨����Ϊ�������������������������������������·��ѹΪ��������(1024)����������Ϊ(59909*100)/1024=5792;
		/*i32Temp1 = 59310;
		i16Temp = objMonInterface.m_i16SelfAgingCurrRate - 20;
		UPDNLMT16(i16Temp,100,0);
		i16Temp1 = objBypass.m_unVbpaRms.half.hword;
		UPDNLMT16(i16Temp1,1200,900);
		m_i16AgingIindMdTgt = (i32Temp1*i16Temp/i16Temp1);
		UPDNLMT16(m_i16AgingIindMdTgt,6000,0);*/

		i16Temp = objMonInterface.m_i16SelfAgingCurrRate - 20;
		UPDNLMT16(i16Temp,110,10);
		m_i16AgingIindMdTgt = i16Temp*58;	  			// ������������ӳ��:5792/100
			
		if((objSystem.m_st_wFlagSystem.bAfterParamDly == 0) && ( objMonInterface.m_st_wUPSModeRun.bMode == 0))
		{
			if (objMonInterface.m_st_wUPSModeRun2.bInvRunMode == 1)
			{
				objSystem.m_st_wFlagSystem.bSelfAgingAction = 1;
			}
			else
			{
				objSystem.m_st_wFlagSystem.bSelfAgingAction = 0;
			}
		}

		//SPWM��������ϵ������
		switch(objMonInterface.m_st_wUPSModeRun.bVoltOut)
			{
				case 0:											//400V	
					m_i16KSPWM = KSPWMCnst2;
					break;
				case 1:											//380v
					m_i16KSPWM = KSPWMCnst1;
					break;
				case 2:											//415v
					m_i16KSPWM = KSPWMCnst3;   
					break;
					
				default:
					break;										
	}
	}
/************************************************************************************
//�������ƣ�    Alg_Int_AgingPhaseLock()
//����������	���ϻ����࣬������·��ѹ��ǡ����ж��Ƿ����ϡ�
//�㷨˵����	��ͨ��PI�����㷨
***********************************************************************************/
void 	Class_SelfAgingAlgorithm::Alg_Int_AgingPhaseLock(void)
{
	INT16	i16IntAx_0,i16IntBx_0,i16IntCx_0,i16IntDx_0,i16IntEx_0;

	if(objBypass.m_st_FlagVbpTotal.bBpTrace == 0)				// ��·���ɸ���
	{
		m_unAgingIoutTheta_0.dword = 0;
		m_unAgingSumPLL_0.dword  = 0;
		m_i16AgingPLLCnt_0 = 0;
		objInvAlg.m_st_wIntMainFlag.bPhaseLock = 0;				// ����λ������־
	}
	else
	{															// ������λ���
		i16IntBx_0 = objBypass.m_i16SinQBp_0;
		i16IntCx_0 = objBypass.m_i16CosQBp_0;
		i16IntAx_0 = ((INT32)i16IntBx_0*m_i16AgingCos0_0-(INT32)i16IntCx_0*m_i16AgingSin0_0)>>14;	// ������λ������ֵ,6.10
		m_i16AgingSinDphase_0 = i16IntAx_0;
		UPDNLMT16(i16IntAx_0,500,-500);							// ����������30��,1024Ϊ1,512Ϊ0.5,��30��
		m_i16AgingCosDphase_0 = ((INT32)i16IntCx_0*m_i16AgingCos0_0+(INT32)i16IntBx_0*m_i16AgingSin0_0)>>14;// ������λ������ֵ,6.10
		
		// PI������
		i16IntDx_0 = 11000;
		i16IntEx_0 = m_unAgingSumPLL_0.half.hword+(((INT32)i16IntAx_0*i16IntDx_0)>>12);
		UPDNLMT16(i16IntEx_0,1267,-1267);							// 1024-->1�� 1267/1024/360*8000=27.5Hz 55+-27.5Hz
	
		// ����������
		m_unAgingSumPLL_0.dword += (INT32)i16IntAx_0*1408;
		UPDNLMT32(m_unAgingSumPLL_0.dword,0x4F30000,-0x4F30000);
		
		// �жϵ�ǰ����Ƿ����
		m_unAgingIoutTheta_0.dword += (((INT32)i16IntEx_0+K55HzDthetaCnst)<<6);
		if(m_unAgingIoutTheta_0.dword>=23592960)
		{
			m_unAgingIoutTheta_0.dword -= 23592960;
		}
		
		// ��ȡsinQ��cosQ
		m_i16AgingSin0_0 = sintab[m_unAgingIoutTheta_0.half.hword];
		i16IntAx_0 = m_unAgingIoutTheta_0.half.hword+90;
    	if(i16IntAx_0>=360)
    	{
			i16IntAx_0 -= 360;
    	}
		m_i16AgingCos0_0 = sintab[i16IntAx_0];
		
		// �ж���λ�Ƿ�����,9.8��
		if((m_i16AgingCosDphase_0 < 512) || (abs(m_i16AgingSinDphase_0) > 174))
		{
			m_i16AgingPLLCnt_0--;
			if(m_i16AgingPLLCnt_0 <= 0)
			{
				m_i16AgingPLLCnt_0 = 0;
				objInvAlg.m_st_wIntMainFlag.bPhaseLock = 0;
				objInvAlg.m_st_wIntMainFlag1_0.bBpPhaseLock = 0;
			}
		}
		else
		{
			m_i16AgingPLLCnt_0++;
			if(m_i16AgingPLLCnt_0 >= 1500)
			{
				m_i16AgingPLLCnt_0 = 1500;
				objInvAlg.m_st_wIntMainFlag.bPhaseLock = 1;
				objInvAlg.m_st_wIntMainFlag1_0.bBpPhaseLock = 1;
			}
		}
	}
}

/************************************************************************************
//�������ƣ�    Alg_Int_AgingRefGen()
//����������	�ο���ѹ����������
//�㷨˵����    ���ݵ�ѹģʽ�����ģʽ�����������������ӻ���ٲο���ѹ(��ο�����)�õ�Ŀ��ֵ
***********************************************************************************/
void 	Class_SelfAgingAlgorithm::Alg_Int_AgingRefGen(void)
{
	INT16	i16IntAx_0;
	
	if((objXfer.m_st_wXferCmd.bSwitchStatus == 1) && (objSwitches.m_st_FlagBreaker.bInvKM == 1))	// ��乩����������INVKM�ɿ���ͨ
	{
		i16IntAx_0 = m_i16AgingIindMdTgt-m_i16AgingRefID_0;
		if(i16IntAx_0 != 0)
		{
			m_i16AgingCurRefCnt_0++;
			if(m_i16AgingCurRefCnt_0 >= 6)								// ÿ6���������ڵ���һ��
			{ 
				m_i16AgingCurRefCnt_0 = 0;
				if(i16IntAx_0 < 0)
				{
					m_i16AgingRefID_0--;
				}
				else
				{
					m_i16AgingRefID_0++;
				}
			}
		}
		m_i16AgingKpCur_0 = 250;
		m_i16AgingKiCur_0 = 40;
		m_i16AgingBpModeRef_0 = objBypass.m_unVbpMMean_0.half.hword;
	}
	else														// ��·�������乩����������INVKM����ͨ
	{
		i16IntAx_0 = objBypass.m_unVbpMMean_0.half.hword-m_i16AgingBpModeRef_0;
		if(i16IntAx_0 != 0)
		{
			m_i16AgingVolRefCnt_0++;
			if(m_i16AgingVolRefCnt_0 >= 3)								// ÿ3���������ڵ���һ��
			{
				m_i16AgingVolRefCnt_0 = 0;
				if(i16IntAx_0 < 0)
				{
					m_i16AgingBpModeRef_0--;
				}
				else
				{
					m_i16AgingBpModeRef_0++;
				}
			}
		}
		// ������λ��Ϊ����ģʽ��׼��
		m_i16AgingKpCur_0 = 0;
		m_i16AgingKiCur_0 = 0;
		m_unAgingSumID_0.dword = 0;
		m_unAgingSumIQ_0.dword = 0;
		m_unAgingSumIZ_0.dword = 0;
	}
}

/************************************************************************************
//�������ƣ�    Alg_Int_AgingPreDeal()
//����������	����������任
//�㷨˵����    �����任��ת�任
***********************************************************************************/
void 	Class_SelfAgingAlgorithm::Alg_Int_AgingPreDeal(void)
{
	INT16	i16IntAlpha_0,i16IntBeta_0;
	i16IntAlpha_0 = (((INT32)objInverter.m_i16Ila_0*2-objInverter.m_i16Ilb_0-objInverter.m_i16Ilc_0)*K1div3Cnst)>>12;
	i16IntBeta_0 = (((INT32)objInverter.m_i16Ilb_0-objInverter.m_i16Ilc_0)*KSqrt3invCnst)>>12;
	m_i16AgingIZ_0 = (((INT32)objInverter.m_i16Ila_0+objInverter.m_i16Ilb_0+objInverter.m_i16Ilc_0)*KSqrt2Div3Cnst)>>12;

	m_i16AgingID_0 = ((INT32)i16IntBeta_0*m_i16AgingSin0_0+(INT32)i16IntAlpha_0*m_i16AgingCos0_0)>>12;
	m_i16AgingIQ_0 = ((INT32)i16IntBeta_0*m_i16AgingCos0_0-(INT32)i16IntAlpha_0*m_i16AgingSin0_0)>>12;	
}

/************************************************************************************
//�������ƣ�    Alg_Int_AgingCurrReg()
//����������	������������
//�㷨˵����    ��ͨ������PI������
***********************************************************************************/
void 	Class_SelfAgingAlgorithm::Alg_Int_AgingCurrReg(void)
{
	INT16	i16IntAx_0;
  i16IntAx_0 = m_i16AgingRefID_0-m_i16AgingID_0;							// ���
	UPDNLMT16(i16IntAx_0,6000,-6000);	
	m_i16AgingPwmD_0 = m_unAgingSumID_0.half.hword+(((INT32)i16IntAx_0*m_i16AgingKpCur_0)>>12)+m_i16AgingBpModeRef_0;
	UPDNLMT16(m_i16AgingPwmD_0,8000,-8000);
	m_unAgingSumID_0.dword += (INT32)i16IntAx_0*m_i16AgingKiCur_0;
	UPDNLMT32(m_unAgingSumID_0.dword,0x17700000,-0x17700000);
	
    i16IntAx_0 = m_i16AgingRefIQ_0-m_i16AgingIQ_0;							// ���
	UPDNLMT16(i16IntAx_0,3000,-3000);
	m_i16AgingPwmQ_0 = m_unAgingSumIQ_0.half.hword+(((INT32)i16IntAx_0*m_i16AgingKpCur_0)>>12);
	UPDNLMT16(m_i16AgingPwmQ_0,4096,-4096);
	m_unAgingSumIQ_0.dword += (INT32)i16IntAx_0*m_i16AgingKiCur_0;
	UPDNLMT32(m_unAgingSumIQ_0.dword,0x8000000,-0x8000000);
	
	i16IntAx_0 = -m_i16AgingIZ_0;
	UPDNLMT16(i16IntAx_0,1000,-1000);
	m_i16AgingPwmZ_0 = m_unAgingSumIZ_0.half.hword+(((INT32)i16IntAx_0*m_i16AgingKpCur_0)>>12);
	UPDNLMT16(m_i16AgingPwmZ_0,1500,-1500);
	UPDNLMT16(i16IntAx_0,300,-300);
	m_unAgingSumIZ_0.dword += (INT32)i16IntAx_0*2;
	UPDNLMT32(m_unAgingSumIZ_0.dword,0x5DC000,-0x5DC000);
}

/************************************************************************************
//�������ƣ�    Alg_Int_AgingSPWM()
//����������	PWM����
***********************************************************************************/
void 	Class_SelfAgingAlgorithm::Alg_Int_AgingSPWM(void)
{
	INT16	i16IntAx_0,i16IntBx_0,i16IntCx_0,i16IntDx_0;
	INT16	i16IntAlpha_0,i16IntBeta_0;

	m_i16wUirD_0 = m_i16AgingPwmD_0;
	m_i16wUirQ_0 = m_i16AgingPwmQ_0;
	m_i16wUirZ_0 = m_i16AgingPwmZ_0;

	//ĸ�ߵ�ѹǰ��
	i16IntDx_0 = objDCBus.m_i16VbusP_0;
	UPDNLMT16(i16IntDx_0,4999,3000);
	i16IntDx_0 -= 3000;
	m_i161DivVbusP_0 = iVbusInvTab[i16IntDx_0];				//4096*4096/(VbusP������ֵ 405V��4096)

	i16IntDx_0 = objDCBus.m_i16VbusN_0;
	UPDNLMT16(i16IntDx_0,4999,3000);
	i16IntDx_0 -= 3000;
	m_i161DivVbusN_0 = iVbusInvTab[i16IntDx_0];				//4096*4096/(VbusN������ֵ 405V��4096)

	// ��ת/��ֹ�任
	i16IntAlpha_0 = (((INT32)m_i16wUirD_0) * m_i16AgingCos0_0 - (((INT32)m_i16wUirQ_0)*m_i16AgingSin0_0)) >> 12;
	i16IntBeta_0 = (((INT32)m_i16wUirD_0) * m_i16AgingSin0_0 + (((INT32)m_i16wUirQ_0)*m_i16AgingCos0_0)) >> 12;

	// 2/3�任
	m_i16VaAct_0 = i16IntAlpha_0+((((INT32)m_i16wUirZ_0) * K1DivSqrt2Cnst) >> 12);
	m_i16VbAct_0 = (-((INT32)i16IntAlpha_0) * K1Div2Cnst + (((INT32)i16IntBeta_0) * KSqrt3Div2Cnst)+(((INT32)m_i16wUirZ_0)*K1DivSqrt2Cnst)) >> 12;
	m_i16VcAct_0 = (-((INT32)i16IntAlpha_0) * K1Div2Cnst -(((INT32)i16IntBeta_0)  *KSqrt3Div2Cnst)+(((INT32)m_i16wUirZ_0)*K1DivSqrt2Cnst)) >> 12;


	// ��ת/��ֹ�任
	i16IntAlpha_0 = (((INT32)m_i16AgingRefID_0) * m_i16AgingCos0_0 - (((INT32)m_i16AgingRefIQ_0)*m_i16AgingSin0_0)) >> 12;
	i16IntBeta_0 = (((INT32)m_i16AgingRefID_0) * m_i16AgingSin0_0 + (((INT32)m_i16AgingRefIQ_0)*m_i16AgingCos0_0)) >> 12;

	// 2/3�任
	m_i16Iaref_0 = i16IntAlpha_0;
	m_i16Ibref_0 = (-((INT32)i16IntAlpha_0) * K1Div2Cnst + (((INT32)i16IntBeta_0) * KSqrt3Div2Cnst)) >> 12;
	m_i16Icref_0 = (-((INT32)i16IntAlpha_0) * K1Div2Cnst -(((INT32)i16IntBeta_0)  *KSqrt3Div2Cnst)) >> 12;

	UPDNLMT16(m_i16Iaref_0,1600,-1600);
	UPDNLMT16(m_i16Iaref_0,1600,-1600);
	UPDNLMT16(m_i16Iaref_0,1600,-1600);

	unIarefFilter_0.dword += (((long)m_i16Iaref_0<<11) - unIarefFilter_0.dword)>>2;
	unIbrefFilter_0.dword += (((long)m_i16Ibref_0<<11) - unIbrefFilter_0.dword)>>2;
	unIcrefFilter_0.dword += (((long)m_i16Icref_0<<11) - unIcrefFilter_0.dword)>>2;

	m_i16PWMaDeadCmp_0 = unIarefFilter_0.half.hword;
	m_i16PWMbDeadCmp_0 = unIbrefFilter_0.half.hword;
	m_i16PWMcDeadCmp_0 = unIcrefFilter_0.half.hword;
	
	UPDNLMT16(m_i16PWMaDeadCmp_0,50,-50);
	UPDNLMT16(m_i16PWMbDeadCmp_0,50,-50);
	UPDNLMT16(m_i16PWMcDeadCmp_0,50,-50);

	// ����Ƚ�ֵ
	i16IntAx_0 = ((INT32)(abs(m_i16VaAct_0))* m_i16KSPWM) >> 12;
	i16IntBx_0 = ((INT32)(abs(m_i16VbAct_0))* m_i16KSPWM) >> 12;
	i16IntCx_0 = ((INT32)(abs(m_i16VcAct_0))* m_i16KSPWM) >> 12;
	
	i16IntAx_0 += 8;										//��������360ns(Ӳ�������½���ʱ��ʧ)
	i16IntBx_0 += 8;										//��������360ns(Ӳ�������½���ʱ��ʧ)
	i16IntCx_0 += 8;										//��������360ns(Ӳ�������½���ʱ��ʧ)

	if (m_i16VaAct_0 > 0)
	{
		GpioDataRegs.GPACLEAR.bit.GPIO17 = 1;				//A��������
		i16IntAx_0 = (((INT32)i16IntAx_0 * m_i161DivVbusP_0) >> 12) + m_i16PWMaDeadCmp_0;
	}
	else
	{
		GpioDataRegs.GPASET.bit.GPIO17 = 1;				//A�ฺ����
		i16IntAx_0 = (((INT32)i16IntAx_0 * m_i161DivVbusN_0) >> 12) - m_i16PWMaDeadCmp_0;
	}

	if (m_i16VbAct_0 > 0)
	{
		GpioDataRegs.GPACLEAR.bit.GPIO18 = 1;				//B��������
		i16IntBx_0 = (((INT32)i16IntBx_0 * m_i161DivVbusP_0) >> 12) + m_i16PWMbDeadCmp_0;
	}
	else
	{
		GpioDataRegs.GPASET.bit.GPIO18 = 1;				//B�ฺ����
		i16IntBx_0 = (((INT32)i16IntBx_0 * m_i161DivVbusN_0) >> 12) - m_i16PWMbDeadCmp_0;
	}

	if (m_i16VcAct_0 > 0)
	{
		GpioDataRegs.GPACLEAR.bit.GPIO19 = 1;				//C��������
		i16IntCx_0 = (((INT32)i16IntCx_0 * m_i161DivVbusP_0) >> 12) + m_i16PWMcDeadCmp_0;
	}
	else
	{
		GpioDataRegs.GPASET.bit.GPIO19 = 1;				//C�ฺ����
		i16IntCx_0 = (((INT32)i16IntCx_0 * m_i161DivVbusN_0) >> 12) - m_i16PWMcDeadCmp_0;
	}

	UPDNLMT16(i16IntAx_0,KSwPrdCnst-20,20);
	UPDNLMT16(i16IntBx_0,KSwPrdCnst-20,20);
	UPDNLMT16(i16IntCx_0,KSwPrdCnst-20,20);

	// ���෢��
	m_i16PulseOffCount = 0;
	EPwm2Regs.CMPA.bit.CMPA = i16IntAx_0;				//A��S1������
	EPwm3Regs.CMPA.bit.CMPA = i16IntBx_0;				//B��S1������
	EPwm4Regs.CMPA.bit.CMPA = i16IntCx_0;				//C��S1������
}

/************************************************************************************
//�������ƣ�    Alg_Int_AgingOffReset()
//����������	�ػ�״̬��λ����
***********************************************************************************/
void 	Class_SelfAgingAlgorithm::Alg_Int_AgingOffReset(void)
{
    m_i16AgingBpModeRef_0 = 100;
	m_i16AgingRefID_0 = 500;
	
	m_i16AgingKpCur_0 = 0;
	m_i16AgingKiCur_0 = 0;
	m_unAgingSumID_0.dword = 0;
	m_unAgingSumIQ_0.dword = 0;
	m_unAgingSumIZ_0.dword = 0;
	
	unIarefFilter_0.dword = 0;
	unIbrefFilter_0.dword = 0;
	unIcrefFilter_0.dword = 0;
	
	m_i16PulseOffCount++;
	if(m_i16PulseOffCount > 3)                                                
	{                                                                   
		m_i16PulseOffCount = 3;                                             
		EPwm2Regs.CMPA.bit.CMPA = 0;						//A�������
		EPwm3Regs.CMPA.bit.CMPA = 0;						//B�������
		EPwm4Regs.CMPA.bit.CMPA = 0;						//C�������
	                                                                    
		GpioDataRegs.GPACLEAR.bit.GPIO17=1;					//A��������     
		GpioDataRegs.GPACLEAR.bit.GPIO18=1;					//B��������     
		GpioDataRegs.GPACLEAR.bit.GPIO19=1;					//C��������     
	}      
}

/*==========================================================*/ 	
/* Title		:	SelfAgingAlgorithm.cpp											*/ 	
/* Description	: 	Class_SelfAgingAlgorithm definition								*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/	
#include "SelfAgingAlgorithm.h"

/************************************************************************************
//�������ƣ�    Alg_Int_SelfAging()
//����������	ʵ�����ϻ�����
//�㷨˵����    ϵͳ��Ϊ���ϻ�ģʽʱ��ʵ�����ϻ�����
//��ڲ�����    objInvCtrl.m_st_wFlagInv ���ϻ�ģʽ��־λ
//���ڲ�����    ��
***********************************************************************************/
//#pragma CODE_SECTION(".Epwm1Funcs")

void 	Class_SelfAgingAlgorithm::Alg_Int_SelfAging(void)
{
	Alg_Int_AgingPhaseLock();
		
	if(objInverter.m_st_wFlagInv.bInv != 0)		         		// ���������
	{
		Alg_Int_AgingRefGen();            						// �ο���ѹ����������
    
		Alg_Int_AgingPreDeal();                					// ����������任
    
		Alg_Int_AgingCurrReg();                  				// ������������
    
		Alg_Int_AgingSPWM();                      				// SPWM����
		
		objPWMDrv.Drv_PWMActive();								//PWM���
	}
    else 
    {
   		Alg_Int_AgingOffReset();								// �ػ�״̬��λ����
   		
   		objPWMDrv.Drv_PWMInactive();							//��ֹPWM���
    }
}

//===========================================================================
// End of file.
//===========================================================================
