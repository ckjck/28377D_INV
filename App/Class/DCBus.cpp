/*==========================================================*/ 	
/* Title		:	DCBus.cpp													*/ 	
/* Description	: 	Class_DCBus definition											*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#include "DCBus.h"

/************************************************************************************
��������	App_DCBusVarInit()	
����������	DCBus�������ʼ��		
�������ã�	
***********************************************************************************/
inline	 void	Class_DCBus::App_DCBusVarInit(void)
	{
		m_i16KVdcbus_0 = 5064;            //����ϵ��
		m_i16KVbusPAdjust_0 = 4096; 
		m_i16KVbusNAdjust_0 = 4096;
		m_i16VdcOverCnt_0 = 0;
	}
/************************************************************************************
��������App_DCBusParamConfig()
************************************************************************************/
inline	void	Class_DCBus::App_DCBusParamConfig(void)
	{
		//����ϵ������
		m_i16KVdcbus_0 = 5064;              //����ϵ��
		m_i16KVbusPAdjust_0 = 4096; 
		m_i16KVbusNAdjust_0 = 4096;
	}

/************************************************************************************
��������	vDC_Fault()
����������	ĸ�߹����ж�ģ��
�������ã�	��			
************************************************************************************/
	void	Class_DCBus::Alg_DCBusFault(void)
	{
		//ĸ�ߵ�ѹ�͹ػ�������·����������£������������·�쳣��ֱ�ӹػ���
		objSystem.m_st_wFlagPrompt.bDCAbnormalOff = 0;										//��ֱ����ѹ�쳣����λ
		
		if (  m_i16Vdc_0 < VdcDnlimCnst  )											//������ѹ���ֵ����600V
		{
			objSystem.m_st_wFlagFault.bDCAbnormal = 1;										//��ֱ��ĸ�ߵ�ѹ�쳣����

			if ( objInverter.m_st_wFlagInv.bInv != 0 )										//���������
			{
				objSystem.m_st_wFlagPrompt.bDCAbnormalOff = 1;								//��ĸ�ߵ�ѹ�͹ػ�
				objDebug.m_st_wDebug.bDCAbnormalOff1 = 1;
			}
		}
		else
		{
			if ( objSystem.m_st_wFlagSystem.bTimer40ms == 1 )								//40ms��ʱ��
			{
				if (  m_i16Vdc_0 < VdcLowCnst )									//������ѹ���ֵ����660V
				{
					m_i16VdcLowCntPer40ms ++;											//ĸ�ߵ���330V��ʱ����������
					if ( m_i16VdcLowCntPer40ms >= 250 )								//10s
					{
						m_i16VdcLowCntPer40ms = 250;
						objSystem.m_st_wFlagFault.bDCAbnormal = 1;							//��ֱ��ĸ�ߵ�ѹ�쳣����
						if ( objInverter.m_st_wFlagInv.bInv != 0 )							//�޷���250
						{
							objSystem.m_st_wFlagPrompt.bDCAbnormalOff = 1;					//��ĸ�ߵ�ѹ�͹ػ�
							objDebug.m_st_wDebug.bDCAbnormalOff2 = 1;
						}
					}
					else if(m_i16VdcLowCntPer40ms < 100)
					{
						objSystem.m_st_wFlagFault.bDCAbnormal = 0;							//��ֱ��ĸ�ߵ�ѹ�쳣����
					}
				}
				else
				{
					m_i16VdcLowCntPer40ms --;											//ĸ�ߵ���330V��ʱ�������ݼ�
					if ( m_i16VdcLowCntPer40ms <= 0 )
					{
						m_i16VdcLowCntPer40ms = 0;									//�޷�����
						objSystem.m_st_wFlagFault.bDCAbnormal = 0;							//��ֱ��ĸ�ߵ�ѹ�쳣����
					}
				}
			}
		}		

		if ( objDigitalIO.m_st_HardwareSignal.bDcOvp == 1 )								//ֱ����ѹ 0��Ч
		{
			objSystem.m_st_wFlagFault.bDCOV = 1;											//��ֱ����ѹ����
			objInvCtrl.m_st_wFlagInvOnoff.bManualRecoverOff = 1;							//�ÿ���Ϊ�ָ����Ϲػ�λ ��DC-OV�ź���������

			if ( objInverter.m_st_wFlagInv.bInv != 0 )										//���������
			{
				objSystem.m_st_wFlagPrompt.bDCAbnormalOff = 1;								//��ĸ�ߵ�ѹ�쳣�ػ�
			}
		}
		else
		{
			objSystem.m_st_wFlagFault.bDCOV = 0;	
		}	 

		//�������ر� (������·�͵��) ʱ�����ر���������ӿ�ĸ���ж��ٶ�?
		if((objDigitalIO.m_st_HardwareSignal.bRecOff == 1) && (objInverter.m_st_wFlagInv.bInv != 0))
		{
			objSystem.m_st_wFlagPrompt.bDCAbnormalOff = 1;								//��ĸ�ߵ�ѹ�͹ػ�
			if(objDigitalIO.m_st_wLocalNewSignal.bRECOvTemp == 1)
			{
				objInvCtrl.m_st_wFlagInvOnoff.bAutoRecoverOff = 1;
			}
			else
			{
				objInvCtrl.m_st_wFlagInvOnoff.bManualRecoverOff = 1;						//�ÿ���Ϊ�ָ���Ϲػ��?
			}	
			objDebug.m_st_wDebug.bDCAbnormalOff3 = 1;
		}

		//��·��Χ�쳣��ֱ��ĸ���쳣ֱ�ӹػ�������ػ��������
		if ((objSystem.m_st_wFlagPrompt.bDCAbnormalOff == 1) && (objBypass.m_st_FlagVbpTotalWide.bBpRange == 3))
		{
			objInvCtrl.m_st_wFlagInvOnoff.bManualRecoverOff = 1;						//�ÿ���Ϊ�ָ����Ϲػ�λ
		}
			
	}

/************************************************************************************
//�������ƣ�    Alg_Int_DCOVProtect()
//��������:     ֻ�е���·�쳣�Ž��д˴��� 
//	            �ж�ĸ�ߵ�ѹ��˲ʱֵ������ѹ��ѹ>920Vʱ,�������������1ms���󿪷��������
//	            ����ѹ��ѹ<880V��־λ����
***********************************************************************************/
 
#pragma CODE_SECTION(".Epwm1Funcs") 			

void 	Class_DCBus::Alg_Int_DCOVProtect(void)
{
    if((objInverter.m_st_wFlagInv.bInv!=0) && (objSystem.m_st_wFlagPrompt.bPow15vFault == 0))     			 // ������ر�/������Դ�����£��������ģ��
   {
	objInvAlg.m_st_wIntMainFlag1_0.bDcOvTotalBak = objInvAlg.m_st_wIntMainFlag1_0.bDcOvTotal;  			// BIT12==1����ĸ���й�ѹ

        if((objBypass.m_st_FlagVbpTotal.bBpRange == 3) && (objMonInterface.m_st_wUPSModeRun.bConfig == 3))      // ��·�쳣��־/����ģʽ��,����ĸ�ߵ�ѹ�߷����嶯��
        {
            // �ж�ĸ���Ƿ��ѹ
            if(m_i16VdcSmp_0 < 4652)                                          // 920*4096/810=4652
            {
            		if(m_i16VdcSmp_0 <= 4449) 
            		{
				objInvAlg.m_st_wIntMainFlag1_0.bDcOvTotal = 0;     // 880V*4096/810=4449
            		}
            }
            else 
            {
			objInvAlg.m_st_wIntMainFlag1_0.bDcOvTotal = 1;
            }
        }
        else 
        {
		objInvAlg.m_st_wIntMainFlag1_0.bDcOvTotal = 0;
        }
        
        // DCOV_FASTCHK_END
        if((objInvAlg.m_st_wIntMainFlag1_0.bDcOvTotalBak == 0) && ( objInvAlg.m_st_wIntMainFlag1_0.bDcOvTotal == 1))
        {
           	 m_i16DCOVPWMShutCnt_0 = 5;
            	objInvAlg.m_st_wIntMainFlag1_0.bDcOvPluseOffDoing = 1;
        }


		//�������͵Ŀ�������,GpioDataRegs.GPADAT.bit.GPIO28ΪӲ���������ź�
		
		
		
        if(objInvAlg.m_st_wIntMainFlag1_0.bDcOvPluseOffDoing == 1)
        {
            if(m_i16DCOVPWMShutCnt_0 > 0)
            {
                m_i16DCOVPWMShutCnt_0--;
                objPWMDrv.Drv_PWMInactive();	   // PWM���ǿ��Ϊ����?
            }
            else
            {
                m_i16DCOVPWMShutCnt_0 = 0;
                objInvAlg.m_st_wIntMainFlag1_0.bDcOvPluseOffDoing = 0;
                objPWMDrv.Drv_PWMActive();	   // PWM���
            }
        }
	}
	if(objInverter.m_st_wFlagInv.bInv ==0)
	{
		m_i16VdcOverCnt_0 = 0;
		objSystem.m_st_wFlagPrompt.bDCAbnormalOff1 = 0;
		m_i16DcOverDelayCnt_0 --;
		if(m_i16DcOverDelayCnt_0 <= 0)
		{
			objLoad.m_st_wFlagLoad.bShortCircuit = 0;
			m_i16DcOverDelayCnt_0 = 0;
		}	
	}
}

/************************************************************************************
//�������ƣ�    Dat_DCBusRMS()
************************************************************************************/
void 	Class_DCBus::Dat_DCBusRMS(void)
{
	//ֱ��ĸ�ߵ�ѹ�˲�,�˲��������Կ���Ϊ30ms 
	m_unVdcSmp_0.dword += ( (((INT32)m_i16VdcSmp_0)<<16)-m_unVdcSmp_0.dword)>>K30msFilterCnst;
	m_i16Vdc_0 = m_unVdcSmp_0.half.hword;
				
}

/************************************************************************************
//�������ƣ�    Dat_Int_DCBusPreDeal()
************************************************************************************/
 
#pragma CODE_SECTION(".Epwm1Funcs") 			

void 	Class_DCBus::Dat_Int_DCBusPreDeal(void)
{
	
	 //��ȡ����ֵ
	 m_i16VbusP_0 = objADDrv.m_i16ADC_VbusP_0;
	 m_i16VbusN_0 = objADDrv.m_i16ADC_VbusN_0;
	 
	 // ĸ�ߵ�ѹ���㣬����ֱ�Ϊ4.12
	m_i16VbusP_0 = (((INT32)m_i16VbusP_0)*m_i16KVdcbus_0)>>12;	
	m_i16VbusP_0 = (((INT32)m_i16VbusP_0)*m_i16KVbusPAdjust_0)>>12;
	
	m_i16VbusN_0 = (((INT32)m_i16VbusN_0)*m_i16KVdcbus_0)>>12;       
	m_i16VbusN_0 = (((INT32)m_i16VbusN_0)*m_i16KVbusNAdjust_0)>>12;
	 
	 m_i16VdcSmp_0 = (m_i16VbusP_0+m_i16VbusN_0)>>1;	 	
	// ĸ�ߵ�ѹ����·�����ѹ���㣬����ֱ�Ϊ5.11��6.10
//	m_i16VdcSmp_0 = (((INT32)m_i16VdcSmp_0)*m_i16KVdcbus_0)>>11;
}


//===========================================================================
// End of file.
//===========================================================================
