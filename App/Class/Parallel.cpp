/*==========================================================*/ 	
/* Title		:	Parallel.cpp													*/ 	
/* Description	: 	Class_Parallel definition											*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#include "Parallel.h"


/************************************************************************************
��������	App_ParaVarInit()	
����������	Parallel�������ʼ��		
�������ã�	
***********************************************************************************/
//inline	 void	Class_Parallel::App_ParaVarInit(void)
//	{
//		m_i16KIavg_0 = 3882;
//	}
/************************************************************************************
��������App_ParaParamConfig()
************************************************************************************/
//	void	Class_Parallel::App_ParaParamConfig(void)
//	{
		//����ϵ������
//		switch (objMonInterface.m_st_wUPSModeRun.bVoltOut)
//		{
//			case 0:																								//400v
//				m_i16KIavg_0 = 3882;																			// �����������
//				break;				            													
//			case 1:																									//380v
//				m_i16KIavg_0 = 3713;   													
//				break;                    													
//			case 2:																									//415v
//				m_i16KIavg_0 = 4050;
//				break;
//			default:
//				break;							
//		}		
//	}

/************************************************************************************
��������	vPara_Fault()
����������	���������ж�ģ��
�������ã�	��			
************************************************************************************/
	void	Class_Parallel::Alg_ParaFault(void)
	{
		if ( objMonInterface.m_st_wUPSModeRun.bConfig == 3 )										//����ϵͳ
		{
//YWF_H20051222�������ظ澯����ȷ��
			if ( objSystem.m_st_wFlagSystem.bTimer40ms == 1 )
			{
				if(((((INT32)objSystem.m_i16SoutTotalMax*KBpKVA*20)>>10 )<=(((INT32)objMonInterface.m_i16ModelKVA *1024 *objMonInterface.m_i16BasicNum*19)/objInverter.m_i16TempKVAGrant))
				&&((((INT32)objSystem.m_i16PoutTotalMax*KBpKVA*20)>>10 )<=(((INT32)objMonInterface.m_i16ModelKVA * 1024 * objMonInterface.m_i16BasicNum*19) /objInverter.m_i16TempKWGrant)))		//С��95%
				{
					if(m_i16ParaFaultCntPer40ms>0)
					{
						m_i16ParaFaultCntPer40ms--;
					}
					
				}
				else if(((((INT32)objSystem.m_i16SoutTotalMax*KBpKVA*20)>>10 )> (((INT32)objMonInterface.m_i16ModelKVA *1024*objMonInterface.m_i16BasicNum*21)/objInverter.m_i16TempKVAGrant))	
						||((((INT32)objSystem.m_i16PoutTotalMax*KBpKVA*20)>>10 )> (((INT32)objMonInterface.m_i16ModelKVA * 1024 *objMonInterface.m_i16BasicNum*21) /objInverter.m_i16TempKWGrant)))//����105%
				{
					if(m_i16ParaFaultCntPer40ms<25)
					{
						m_i16ParaFaultCntPer40ms++;
					}
				}
					
			}

			if(m_i16ParaFaultCntPer40ms > 20)
			{
				objSystem.m_st_wFlagPrompt.bParaOvload = 1;							//�ò���ϵͳ����λ
			}
			else
			{
				objSystem.m_st_wFlagPrompt.bParaOvload = 0;							//�岢��ϵͳ����λ
			}
			
		}
		else
		{																						//�岢����������
			m_i16ParaFaultCntPer40ms = 0;
			//�ǲ���ϵͳ�岢��ϵͳ����λ
			objSystem.m_st_wFlagPrompt.bParaOvload = 0;									//�岢��ϵͳ����λ
		}
	}
	
/************************************************************************************
//�������ƣ�    Dat_Int_ParaPreDeal()
************************************************************************************/
 
//#pragma CODE_SECTION(".Epwm1Funcs") 			

//void 	Class_Parallel::Dat_Int_ParaPreDeal(void)
//{
//	INT16	i16IntAlpha_0,i16IntBeta_0;
	// ƽ���������㣬����Ϊ7.9
	//zqf 060615--������͵Ĳ�ͬ����������ƽ������ϵ��У����
//	m_i16KIavgAdj_0=((INT32)m_i16KIavg_0*objCAN.iKCapAdj)>>12;    
	
	// ����������ƽ��ֵ���㼰3/2�任	
	// ����ģʽ����乩�硢�ǣ������źŸ���̬�͵���ģʽ��
/*	if((objMonInterface.m_st_wUPSModeRun.bConfig == 3) && (objXfer.m_st_XferResult.bInvKM == 1)
		&& ( m_st_wParaMaint.bParaSignalMask != 1) && ( objSystem.m_st_wFlagSystem.bUPSTestMode != 1))
	{
		m_i16Icira_0 = objInverter.m_i16Iinva_0	- m_i16IoutaMean_0;
		m_i16Icirb_0 = objInverter.m_i16Iinvb_0	- m_i16IoutbMean_0;
		m_i16Icirc_0 = objInverter.m_i16Iinvc_0	- m_i16IoutcMean_0;
		
	    UPDNLMT16(m_i16Icira_0,0x0400,-0x0400);
	    UPDNLMT16(m_i16Icirb_0,0x0400,-0x0400);
	    UPDNLMT16(m_i16Icirc_0,0x0400,-0x0400);

		m_i16Iciraavg_0 = (m_i16Icira_0 + m_i16Iciraavg_0)>>1;
		m_i16Icirbavg_0 = (m_i16Icirb_0 + m_i16Icirbavg_0)>>1;
		m_i16Icircavg_0 = (m_i16Icirc_0 + m_i16Icircavg_0)>>1;
		
		i16IntAlpha_0 = (((INT32)m_i16Iciraavg_0*2-m_i16Icirbavg_0-m_i16Icircavg_0)*K1div3Cnst)>>12;
	    i16IntBeta_0 = (((INT32)m_i16Icirbavg_0-m_i16Icircavg_0)*KSqrt3invCnst)>>12;

	    m_i16IcirMdAvg_0 = ((INT32)i16IntBeta_0*objInvAlg.m_i16SinQ0Ref_0+(INT32)i16IntAlpha_0*objInvAlg.m_i16CosQ0Ref_0)>>12;
	    m_i16IcirMqAvg_0 = ((INT32)i16IntBeta_0*objInvAlg.m_i16CosQ0Ref_0-(INT32)i16IntAlpha_0*objInvAlg.m_i16SinQ0Ref_0)>>12;
	    
	    i16IntAlpha_0 = (((INT32)m_i16Icira_0*2-m_i16Icirb_0-m_i16Icirc_0)*K1div3Cnst)>>12;
	    i16IntBeta_0 = (((INT32)m_i16Icirb_0-m_i16Icirc_0)*KSqrt3invCnst)>>12;

	    m_i16IcirMd_0 = ((INT32)i16IntBeta_0*objInvAlg.m_i16SinQ0Ref_0+(INT32)i16IntAlpha_0*objInvAlg.m_i16CosQ0Ref_0)>>12;
	    m_i16IcirMq_0 = ((INT32)i16IntBeta_0*objInvAlg.m_i16CosQ0Ref_0-(INT32)i16IntAlpha_0*objInvAlg.m_i16SinQ0Ref_0)>>12;
	}
	else
	{
		m_i16IcirMd_0 = 0;
		m_i16IcirMq_0 = 0;
		
		m_i16IcirMdAvg_0 = 0;
		m_i16IcirMqAvg_0 = 0;
		
		m_i16Iciraavg_0 = 0;
		m_i16Icirbavg_0 = 0;
		m_i16Icircavg_0 = 0;

	}
*/
//}

/************************************************************************************
��������	Alg_Int_ParaStatus()
����������	��������״̬��⣬ʵʱ��Ⲣ��ϵͳ������֮��ľ���״̬	
�������ã�	��
************************************************************************************/
	void	Class_Parallel::Alg_ParaStatus(void)
	{
		
		if((objMonInterface.m_st_wUPSModeRun.bConfig == 3) && (objXfer.m_st_wXferCmd.bSwitchStatus == 1)&&(objMonInterface.m_st_wParallelData.iTotalNumOn > 1)&&(objSystem.m_st_wFlagSystem.bSelfAgingAction==0))	//��������乩��̨��2̨��2̨����,�����ϻ�ģʽ
		{
			if( objSystem.m_st_wFlagSystem.bTimer14ms == 1 )
			{
				if((abs(objSystem.m_i16SoutaTotal - objInverter.m_i16Sinva*objMonInterface.m_st_wParallelData.iNumOn)>=(307*objMonInterface.m_st_wParallelData.iNumOn))
					||(abs(objSystem.m_i16SoutbTotal - objInverter.m_i16Sinvb*objMonInterface.m_st_wParallelData.iNumOn)>=(307*objMonInterface.m_st_wParallelData.iNumOn))
					||(abs(objSystem.m_i16SoutcTotal - objInverter.m_i16Sinvc*objMonInterface.m_st_wParallelData.iNumOn)>=(307*objMonInterface.m_st_wParallelData.iNumOn)))

				{
						if(m_i16ShareFaultCnt_0 >= 268) 
						{
							objSystem.m_st_wFlagPrompt.bParaShareFault = 1;	
							return;
						}
						m_i16ShareFaultCnt_0++;
					
				}
				else
				{
					if(m_i16ShareFaultCnt_0 <=0 ) 
					{
						objSystem.m_st_wFlagPrompt.bParaShareFault = 0;	
						return;
					}
					m_i16ShareFaultCnt_0--;
				}
			}	
			if(m_i16ShareFaultCnt_0>179) 	
			{
				objSystem.m_st_wFlagPrompt.bParaShareFault = 1;	
			}
			else if(m_i16ShareFaultCnt_0 < 89)						
			{
				objSystem.m_st_wFlagPrompt.bParaShareFault = 0;	 
			}
			
		}
		else
		{
			m_i16ShareFaultCnt_0 = 0;
			objSystem.m_st_wFlagPrompt.bParaShareFault = 0;	
		}
		
	}


//===========================================================================
// End of file.
//===========================================================================
