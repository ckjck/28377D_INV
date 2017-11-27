/*==========================================================*/
/* Title		:	Switches.cpp													*/
/* Description	: 	Class_Switches definition										*/
/* Date		:	Apr.2008														*/
/*==========================================================*/
#include "Switches.h"



/************************************************************************************
��������	App_SwitchesVarInit()
����������	Breakers�������ʼ��
�������ã�
***********************************************************************************/
inline	 void	Class_Switches::App_SwitchesVarInit(void)
	{
		m_st_FlagBreaker.bFuse=1;				//Breaker Intial status
//		m_i16KbackFeed_0 = 0x20A7;
	}

/************************************************************************************
��������App_SwitchesParamConfig()
************************************************************************************/
//inline	void	Class_Switches::App_SwitchesParamConfig(void)
//	{
//		m_i16KbackFeed_0 = 0x20A7;
//	}

/************************************************************************************
//�������ƣ�    Dat_Int_VBackFeedCal()
************************************************************************************/

//#pragma CODE_SECTION(".Epwm1Funcs")

//void 	Class_Switches::Dat_Int_VBackFeedCal(void)
//{
	  //��ȡ����ֵ
/*	m_i16VbackFeed_0 = objADDrv.m_i16ADC_VbackFeed_0;

	m_i16VbackFeed_0 = ((INT32)m_i16VbackFeed_0*m_i16KbackFeed_0)>>10;

	//��·���ౣ����ص�ѹ��Чֵ�ۼ�
	m_unVbackFeedSum_0.dword += (((INT32)m_i16VbackFeed_0*m_i16VbackFeed_0)>>4);

	if(objBypass.m_i16PointcntBp_0 == 0)
	{
		m_unVbackFeedSumM_0 = m_unVbackFeedSum_0;
		m_unVbackFeedSum_0.dword = 0;
	}
*/
//}

/************************************************************************************
//�������ƣ�    Dat_VBackFeedRMS()
************************************************************************************/
//void 	Class_Switches::Dat_VBackFeedRMS(void)
//{
//	INT32	i32temp;
	//��·���ౣ����ص�ѹ��Чֵ����
//	i32temp = m_unVbackFeedSumM_0.dword/objBypass.m_i16PointcntBpM_0;
//	m_i16VbackFeedPrms = isqrt(i32temp);
//	m_unVbackFeedRms.dword += (((INT32)m_i16VbackFeedPrms<<16)-m_unVbackFeedRms.dword)>>K20msFilterCnst;

//}

/************************************************************************************
��������	Dat_BreakerStatus()
����������	����״̬����ģ�飬������Ӧ����˿ڵ��źţ�ϵͳ��ǰ��״̬��ȷ�������տ����۶���
			����̬����������״̬
�������ã�	��
************************************************************************************/

	void	Class_Switches::Dat_BreakerStatus()
	{

		//�����źű��ݣ���ǿ�ⲿ������ؼ�ά����·�źŵĿ�����
	    m_st_FlagBreaker.bQ3bp = (objDigitalIO.m_st_HardwareSignal.bQ3BP |
	    						(objParallel.m_st_wParaSignalF.bCabQ3BpStatusF & objParallel.m_st_wParaSignalFBak.bCabQ3BpStatusF))
	    				    &(!objSystem.m_st_wFlagSystem.bUPSTestMode);            //����ģʽ����ΪQ3BP�Ͽ�

		if(objMonInterface.m_st_wUPSModeRun.bConfig == 3)
		{
		  	m_st_FlagBreaker.bQ5 = (objDigitalIO.m_st_HardwareSignal.bQ5
		  	                     & (objParallel.m_st_wParaSignalF.bExtOutStatusF | objParallel.m_st_wParaSignalFBak.bExtOutStatusF))
		  	                  |objSystem.m_st_wFlagSystem.bUPSTestMode;			 //����ģʽ����ΪQ5�պ�
		}
		else
		{
	    		m_st_FlagBreaker.bQ5 = objDigitalIO.m_st_HardwareSignal.bQ5 | objSystem.m_st_wFlagSystem.bUPSTestMode; //����ģʽ����ΪQ5�պ�
		}

        m_st_FlagBreaker.bQ5 = 1;


		//zqf051020-�����˿����Nxe���о�
		if ( objDigitalIO.m_st_HardwareSignal.bFuse == 0 )									//Fuse��
		{
			m_st_FlagBreaker.bFuse = 0;
			if( objBypass.m_st_FlagVbpTotalWide.bBpRange <= 1 )
			{
				objInvCtrl.m_st_wFlagInvOnoff.bNeverRecoverOff = 1;							//��·��Χ�������ò��ɻָ��ػ�����
			}
			else if(objMonInterface.m_st_wParallelData.iNumOn >1)				//NXrϵ�������˿�ڹ���̨������1̨ʱ�˳�����
			{
				objInvCtrl.m_st_wFlagInvOnoff.bNeverRecoverOff = 1;
			}

		}

		m_st_FlagBreaker.bInvKM = objXfer.m_st_XferResult.bInvKM;						//���STS
		m_st_FlagBreaker.bBpSTS = objXfer.m_st_XferResult.bBpSTS;							//��·STS

		objDigitalIO.m_st_wLocalSignal.bBpSTSBak = 0;

		if ( ( m_st_FlagBreaker.bQ5 == 1 ) && ( objBypass.m_st_FlagVbpTotal.bBpInvtSeq != 1 )
			&& ( objBypass.m_st_FlagVbpTotal.bBpRange != 3 ) && ( objSystem.m_st_wFlagStatus.bEPO != 1 )
			&& ( objSystem.m_st_wFlagStatus.bOutDisable != 1 ) )
		{
			m_st_FlagBreaker.bBp = 1;
		}
		else
		{
			m_st_FlagBreaker.bBp= 0;
		}


		//�˴��������ܲ��㣬������
		if ( ( m_st_FlagBreaker.bQ5 == 1 ) && ( objInverter.m_st_wFlagInv.bInv == 3 )
			&& ( objXfer.m_st_XferResult.bBpSideDelay == 0 ) && ( objSystem.m_st_wFlagPrompt.bSwitchTimesClamp==0 ) )
		{
			m_st_FlagBreaker.bInv = 1;
		}
		else
		{
			m_st_FlagBreaker.bInv = 0;
		}



		if ( ( m_st_FlagBreaker.bInv == 1 ) && ( objXfer.m_st_XferResult.bInvKM == 1 ) )
		{
			m_st_FlagBreaker.bOn = 1;
		}
		else
		{
			m_st_FlagBreaker.bOn = 0;
		}

		//�޸���·����Ķ��壬�������״̬һ��
		m_st_FlagBreaker.bBpOn = (objSystem.m_st_wFlagStatus.bSupply == 1);


		//ZDS20030827 �����źű��ݣ���ǿ�ⲿ������ؼ�ά����·�źŵĿ�����
//		S2U(objParallel.m_st_wParaSignalFBak) = S2U(objParallel.m_st_wParaSignalF);

	}

/************************************************************************************
��������	Alg_Int_BpSTSShort()
����������
�������ã�	��
************************************************************************************/
/*
	#pragma CODE_SECTION(".Epwm1Funcs")

	void	Class_Switches::Alg_Int_BpSTSShort(void)
	{
		INT16	i16TempShortIa;
		INT16	i16TempShortIb;
		INT16	i16TempShortIc;
		//��������������������ͬ�ֲ�����Ӳ��ͨ����ʱ���죬�����󱨡���·��բ�ܹ��ϡ�����
		m_unIinvaFilter_0.dword = FILTER(0, objInverter.m_i16Iinva_0, m_unIinvaFilter_0.dword);
		m_unIinvbFilter_0.dword = FILTER(0, objInverter.m_i16Iinvb_0, m_unIinvbFilter_0.dword);
		m_unIinvcFilter_0.dword = FILTER(0, objInverter.m_i16Iinvc_0, m_unIinvcFilter_0.dword);

		i16TempShortIa = objOutput.m_i16Iouta_0- m_unIinvaFilter_0.half.hword;
		i16TempShortIb = objOutput.m_i16Ioutb_0- m_unIinvbFilter_0.half.hword;
		i16TempShortIc = objOutput.m_i16Ioutc_0- m_unIinvcFilter_0.half.hword;

		if(objXfer.m_st_XferResult.bBpSTS == 0)//Q2�ϣ���·������
		{
			//������30�������ü�2��1����
			if( ( abs(i16TempShortIa)>1738)
			    ||( abs(i16TempShortIb)>1738)
			    ||( abs(i16TempShortIc)>1738) )					//4096*1.414*30%
			{
				if(m_i16BpSTSBackwardFaultCnt_0 < (K200msCnst<<1))
				{
					m_i16BpSTSBackwardFaultCnt_0 += 2;
				}
			}
			else
			{
				if(m_i16BpSTSBackwardFaultCnt_0 > 0)
				{
					m_i16BpSTSBackwardFaultCnt_0--;
				}
			}
		}
		else
		{
			m_i16BpSTSBackwardFaultCnt_0 = 0;
		}

		if(m_i16BpSTSBackwardFaultCnt_0 >= K200msCnst)					//K200msCnst/2=100ms
		{
			objInvAlg.m_st_wIntMainFlag.bBpSTSShort = 1;							//����·��բ�ܶ�·��־
		}
	}
*/
/************************************************************************************
��������	Alg_Int_STSDiffFastChk()
����������	����쳣���ټ�⣬��Ӵ�ģ�����ھ�բ�ܶ�·ʱ���ټ�Ⲣ�л�
�������ã�	��
************************************************************************************/

#pragma CODE_SECTION(".Epwm1Funcs")

	void	Class_Switches::Alg_Int_STSDiffFastChk(void)
	{
		INT16	i16IntAx_0,i16IntBx_0,i16IntCx_0,i16IntDx_0,i16IntEx_0,i16IntFx_0;

		if(((objInvAlg.m_st_wIntMainFlag.bVbpFastChk == 1) ||(objInvAlg.m_st_wIntMainFlag.bVinvFastChk == 1)) //������·���ټ���У�
			&& ((objBypass.m_st_FlagVbpTotal.bBps10Fast == 1) || ((objInvAlg.m_st_wIntMainFlag.bVinvFast == 0)&&(objInvAlg.m_st_wIntMainFlag.bVbpFast == 0)))
			&& (objXfer.m_st_wXferCmd.bSwitchMode!=2))	//��·10s����л���䡢��·����������ҷǼ���л�
		{
			if(objXfer.m_st_XferResult.bBpSTS == 1)
			{
				i16IntAx_0 = objBypass.m_i16Vbpa_0;
				i16IntBx_0 = objBypass.m_i16Vbpb_0;
				i16IntCx_0 = objBypass.m_i16Vbpc_0;
			}
			else if(objXfer.m_st_XferResult.bInvKM == 1)
			{
				i16IntAx_0 = objInverter.m_i16Vinva_0;
				i16IntBx_0 = objInverter.m_i16Vinvb_0;
				i16IntCx_0 = objInverter.m_i16Vinvc_0;
			}
			else
			{
				m_i16SCRDiffFastChkCnt_0 = 0;
				objInvAlg.m_st_wIntMainFlag.bVoutdiffFast = 0;
				return;
			}

			i16IntDx_0 = abs(objOutput.m_i16Vouta_0-i16IntAx_0)-801;					//54V:	54/277*4096
			i16IntEx_0 = abs(objOutput.m_i16Voutb_0-i16IntBx_0)-801;					//54V:	54/277*4096	
			i16IntFx_0 = abs(objOutput.m_i16Voutc_0-i16IntCx_0)-801;					//54V:	54/277*4096
			
			
			if((i16IntDx_0>0)||(i16IntEx_0>0)||(i16IntFx_0>0))
			{
				m_i16SCRDiffFastChkCnt_0++;
				i16IntDx_0 = i16IntDx_0-801;
				i16IntEx_0 = i16IntEx_0-801;
				i16IntFx_0 = i16IntFx_0-801;
				if((i16IntDx_0 > 0)||(i16IntEx_0 > 0)||(i16IntFx_0 > 0))
				{
					m_i16SCRDiffFastChkCnt_0++;							//108V
					i16IntDx_0 = i16IntDx_0-801;
					i16IntEx_0 = i16IntEx_0-801;
					i16IntFx_0 = i16IntFx_0-801;
					if((i16IntDx_0>0)||(i16IntEx_0>0)||(i16IntFx_0>0))
					{
						m_i16SCRDiffFastChkCnt_0++;						//162V
					}
				}
				if(m_i16SCRDiffFastChkCnt_0 > 21)
				{
					m_i16SCRDiffFastChkCnt_0 = 21;
					objInvAlg.m_st_wIntMainFlag.bVoutdiffFast = 1;
				}
			}
			else
			{
				m_i16SCRDiffFastChkCnt_0--;
				if(	m_i16SCRDiffFastChkCnt_0 < 0)
				{
					m_i16SCRDiffFastChkCnt_0 = 0;
					objInvAlg.m_st_wIntMainFlag.bVoutdiffFast = 0;
				}
			}

		}
		else
		{
			m_i16SCRDiffFastChkCnt_0 = 0;
			objInvAlg.m_st_wIntMainFlag.bVoutdiffFast = 0;
		}
	}
/************************************************************************************
��������	Alg_Int_INVKMFastChk()
����������	����쳣���ټ�⣬��Ӵ�ģ�����ھ�բ�ܶ�·ʱ���ټ�Ⲣ�л�
�������ã�	��
************************************************************************************/

#pragma CODE_SECTION(".Epwm1Funcs")

	void	Class_Switches::Alg_Int_INVKMFastChk(void)
	{
		INT16	i16SoutaTotalMean;			//ƽ���������
		INT16	i16SoutbTotalMean;
		INT16	i16SoutcTotalMean;
		INT16     i16temp;


		i16temp=objMonInterface.m_st_wParallelData.iTotalNumOn;
		UPDNLMT16(i16temp,KTotalOnINVMaxCnst,1);
		i16SoutaTotalMean = (objSystem.m_i16RackSoutaTotal / i16temp);
		i16SoutbTotalMean = (objSystem.m_i16RackSoutbTotal / i16temp);
		i16SoutcTotalMean = (objSystem.m_i16RackSoutcTotal / i16temp);

		 if((objXfer.m_st_XferResult.bInvKM == 1)
		 &&(objMonInterface.m_st_wUPSModeRun.bConfig ==3)
		 &&(objPowCANSched.m_i16PowerCanTotalNum > 1))
		 {
			if(((objInverter.m_i16Sinva < 102)&&(i16SoutaTotalMean>307))
			||((objInverter.m_i16Sinvb < 102)&&(i16SoutbTotalMean>307))
			||((objInverter.m_i16Sinvc < 102)&&(i16SoutcTotalMean>307)))    //ĳ��Ĺ���С��10%��ƽ�����ʴ���30%
			{
				m_i16InvKMFastChkCnt++;
				if(m_i16InvKMFastChkCnt >=1600)		//400ms
				{
					m_i16InvKMFastChkCnt = 1600;
					objInvAlg.m_st_wIntMainFlag.bVinvKMFast =1;

				}
			}
			 else
		 	{

				m_i16InvKMFastChkCnt--;
				if(m_i16InvKMFastChkCnt <= 0)
				{
					m_i16InvKMFastChkCnt =0;
					objInvAlg.m_st_wIntMainFlag.bVinvKMFast =0;
				}
			 }

		 }
		 else
		 {
			m_i16InvKMFastChkCnt =0;
			objInvAlg.m_st_wIntMainFlag.bVinvKMFast =0;

		 }


	}
/************************************************************************************
��������	Alg_STSShortFault()
����������	��բ�ܶ�·����
�������ã�	��
************************************************************************************/
	void	Class_Switches::Alg_STSShortFault(void)
	{
		if( objSystem.m_st_wFlagSystem.bTimer14ms == 1)
		{
			if(objInverter.m_st_wFlagInv.bInv == 0)
			{
				if( ( ( objOutput.m_i16VoutaPrms > 559 ) || ( objOutput.m_i16VoutbPrms > 559 ) || ( objOutput.m_i16VoutcPrms > 559 ) )
				&&(objXfer.m_st_XferResult.bInvKM == 0))						//�����ѹ����120v�ұ����̵���û�����ϣ���ֹ��·������ʱ��
				{
					if((abs(objInverter.m_i16Vinva_0 - objOutput.m_i16Vouta_0)< 372)				//����ѹ����·��ѹ˲ʱֵ���20v
					||(abs(objInverter.m_i16Vinvb_0 - objOutput.m_i16Voutb_0)< 372)
					||(abs(objInverter.m_i16Vinvc_0 - objOutput.m_i16Voutc_0)< 372))
					{
						if ( ( objInverter.m_i16IinvaPrms > 45 ) || ( objInverter.m_i16IinvbPrms > 45 ) || ( objInverter.m_i16IinvcPrms > 45 ) )
						{												//���������ڶ������ 4.4%
							m_i16InvSTSFault1CntPer14ms ++;
						}

					}
					else
					{
						if (( m_i16InvSTSFault1CntPer14ms >0)  && (m_i16InvSTSFault1CntPer14ms <= 36 ))
						{
							m_i16InvSTSFault1CntPer14ms --;					//��侧բ�ܹ���ȷ�ϼ����ݼ�
						}
					}

				}
				else
				{
					if( m_i16InvSTSFault1CntPer14ms >0)
					{
						m_i16InvSTSFault1CntPer14ms--;
					}
				}
			}
		}
		if ( objSystem.m_st_wFlagSystem.bTimer40ms == 1 )							//40ms��ʱ��
		{
//��·��բ�ܶ�·��ֹ������---�������ѹȫ��ʱ,��ͨ���������ѹ����·��բ�ܹ���
			if ( ( objOutput.m_i16VoutaPrms > 559 ) && ( objOutput.m_i16VoutbPrms > 559 ) &&  ( objOutput.m_i16VoutcPrms > 559 ) )
			{
				m_i16InvSTSFaultDisableCntPer40ms++;
				if ( m_i16InvSTSFaultDisableCntPer40ms >= 13 )				// 0.5s
				{
					m_i16InvSTSFaultDisableCntPer40ms = 13;
				}
			}
			else
			{
				if (( m_i16InvSTSFaultDisableCntPer40ms >0)  && (m_i16InvSTSFaultDisableCntPer40ms <= 13))
				{
					m_i16InvSTSFaultDisableCntPer40ms --;					//��侧բ�ܹ���ȷ�ϼ����ݼ�
				}
			}



			//����1������������Q3bp�ϣ����򣨲�����Q3bp�ϣ����ң���·���STS���ضϣ�������е�ѹ
			if (((objMonInterface.m_st_wUPSModeRun.bConfig != 3 && objSwitches.m_st_FlagBreaker.bQ3bp == 0)
//				&& (objSwitches.m_st_FlagBreaker.bQ5 == 0 ||objSwitches.m_st_FlagBreaker.bQ3bp == 0))
//				|| (objMonInterface.m_st_wUPSModeRun.bConfig == 3 && objSwitches.m_st_FlagBreaker.bQ5 == 0))
				|| (objMonInterface.m_st_wUPSModeRun.bConfig == 3 && objSwitches.m_st_FlagBreaker.bQ3bp == 0
					&& objParallel.m_st_wParaSignalF.bInvBpStatusF == 0 ))
				&& (objXfer.m_st_XferResult.bBpSTS == 0)
				&&(objDigitalIO.m_st_wLocalSignal.bBPSF == 0)
				&& (objXfer.m_st_XferResult.bInvKM == 0)
				&&(m_i16InvSTSFaultDisableCntPer40ms != 13))
			{
				if ( ( objOutput.m_i16VoutaPrms > 559 ) || ( objOutput.m_i16VoutbPrms > 559 ) || ( objOutput.m_i16VoutcPrms > 559 ) )
																	//�����ѹ>120v
				{
					m_i16BpSTSForwardFaultCntPer40ms ++;						//��·��բ�ܹ���ȷ�ϼ���FEED FORWARD
					if ( m_i16BpSTSForwardFaultCntPer40ms >= 25 )				// 1s
					{
						m_i16BpSTSForwardFaultCntPer40ms = 25;

						if(objSwitches.m_st_FlagBreaker.bBpSTSShort==0 && objSwitches.m_st_FlagBreaker.bInvKMShort == 0 )
						{
							if ( objInverter.m_st_wFlagInv.bInv == 0)	
							{
								objSwitches.m_st_FlagBreaker.bBpSTSShort = 1;		//����·��բ�ܶ�·��־
								if((objMonInterface.m_st_wUPSModeRun.bConfig == 0)||(objDigitalIO.m_st_wHardwareNewSignal.bBPBackFeedDetect ==1))
								{
									objSwitches.m_st_FlagBreaker.bReqBpSTSShort = 0;				//����
								}
								else
								{
									objSwitches.m_st_FlagBreaker.bReqBpSTSShort = 1;				//�ڻ�
								}
								objDebug.m_st_wDebug.bBpSTSShort1 = 1;
	    					//�ò��ɻָ��ػ���־

							}
 							else			//Q2�� ���������
 							{
 								objSwitches.m_st_FlagBreaker.bInvKMShort = 1;		//����侧բ�ܶ�·��־
 								objDebug.m_st_wDebug.bInvKMShort1 = 1;
 							}
						}

 					}
				}
				else
				{
					if ( (m_i16BpSTSForwardFaultCntPer40ms > 0 ) && (m_i16BpSTSForwardFaultCntPer40ms <25 ) )
					{
						m_i16BpSTSForwardFaultCntPer40ms --;
					}
				}
			}
			else
			{
				m_i16BpSTSForwardFaultCntPer40ms = 0;							//��·��բ�ܹ���ȷ�ϼ���FEED FORWARD����
			}
				

			//��侧բ�ܶ�·�о�
			//����2����䲻������������ѹ��������
			if ( objInverter.m_st_wFlagInv.bInv > 0 ) 								//��俪��
			{
				m_i16InvSTSFaultCntPer40ms =0;
			}
			else
			{
				if( ( objInverter.m_i16VinvaPrms > 559 ) || ( objInverter.m_i16VinvbPrms > 559 ) || ( objInverter.m_i16VinvcPrms > 559 ) )
																	//����ѹ���� 120V
				{
					m_i16InvSTSFaultCntPer40ms ++;

					}
					else
					{
						if ( m_i16InvSTSFaultCntPer40ms > 0  && m_i16InvSTSFaultCntPer40ms <15000 )
						{
							m_i16InvSTSFaultCntPer40ms --;					//��侧բ�ܹ���ȷ�ϼ����ݼ�
						}
					}

				}

			}
			if (( m_i16InvSTSFaultCntPer40ms >= 15000 )||(m_i16InvSTSFault1CntPer14ms >=36))			//10min
			{
				m_i16InvSTSFaultCntPer40ms = 15000;
				m_i16InvSTSFault1CntPer14ms = 36;
				objSwitches.m_st_FlagBreaker.bInvKMShort = 1;			//����侧բ�ܶ�·��־
				objDebug.m_st_wDebug.bInvKMShort2 = 1;
			}
	

			//��·��բ�ܶ�·�о�
			//����2����·��������������CT����Ӳ���źţ��������10%�ĵ���
			if( objSystem.m_st_wFlagSystem.bTimer40ms == 1 )
			{
				if(objDigitalIO.m_st_wLocalSignal.bBpSTS == 0)
				{
					//��������ȷ�ϣ����������־bBpSTSShortLatch
					if((objDigitalIO.m_st_wHardwareNewSignal.bBPBackFeedDetect ==1)							//����·����IO�ź�
						&&(( objOutput.m_i16IoutaPrms  > 102)			//�����������10%�Ķ
						||( objOutput.m_i16IoutbPrms  > 102)
						||( objOutput.m_i16IoutcPrms  > 102)))
					{
						if(m_i16STSShortCntPer40ms < 10)
						{
							m_i16STSShortCntPer40ms ++;				
						}
					}
					else
					{
						if(m_i16STSShortCntPer40ms>0)
						{
							m_i16STSShortCntPer40ms--;
						}
					}
				}
				else
				{
					m_i16STSShortCntPer40ms = 0;
				}
			}
			if(m_i16STSShortCntPer40ms == 10)
			{
				objSwitches.m_st_FlagBreaker.bBpSTSShort = 1;			//����·��բ�ܶ�·��־
				objDebug.m_st_wDebug.bBpSTSShort2 = 1;
				objInvCtrl.m_st_wFlagInvOnoff.bNeverRecoverOff=1;	   //�ò��ɻָ��ػ���־
			}


	

		if((objInvAlg.m_st_wIntMainFlag1_0.bBpInvOverCurrent == 1) && (objInvAlg.m_st_wIntMainFlag1_0.bBpInvOverCurrentReason == 0)) //������Թ�ͨ
		{
			objSwitches.m_st_FlagBreaker.bBpSTSShort = 1;			//����·��բ�ܶ�·��־
			objSwitches.m_st_FlagBreaker.bReqBpSTSShort = 0;				//����
			objDebug.m_st_wDebug.bBpSTSShort3 = 1;
			objInvCtrl.m_st_wFlagInvOnoff.bNeverRecoverOff=1;	    					//�ò��ɻָ��ػ���־
		}

		if((objInvAlg.m_st_wIntMainFlag1_0.bBpInvOverCurrent == 1) && (objInvAlg.m_st_wIntMainFlag1_0.bBpInvOverCurrentReason == 1)) //������Թ�ͨ
		{
			objSwitches.m_st_FlagBreaker.bInvKMShort = 1;
		}
///////////////////////////////////////////////


/*
		//��·���ౣ������
		if(objSystem.m_st_wFlagSystem.bTimer14ms == 1)
		{
			if((objDigitalIO.m_st_HardwareSignal.bBpSTSAct ==0)&& (objXfer.m_st_XferResult.bBpSTS==0)
			 && (objBypass.m_st_FlagVbpTotal.bBpRange==3) && (objSwitches.m_st_FlagBreaker.bQ2==1)
			 &&	(objOutput.m_i16VoutaPrms>739) && (objOutput.m_i16VoutbPrms>739) && (objOutput.m_i16VoutcPrms>739)
			 && (objBypass.m_i16VbpaPrms<=444) && (objBypass.m_i16VbpbPrms<=444) && (objBypass.m_i16VbpcPrms<=444))  	//�����ѹ����200V����·��ѹС��120V
			{

				if(m_unVbackFeedRms.half.hword>=739)						//200V
				{
					if(m_i16FeedbackVcomCntPer14ms<500)	m_i16FeedbackVcomCntPer14ms++;
				}
				else
				{
					if(m_i16FeedbackVcomCntPer14ms>0)		m_i16FeedbackVcomCntPer14ms--;
				}

				if(m_i16FeedbackVcomCntPer14ms>=350)
				{
					objDigitalIO.m_st_wLocalSignal.bBackFeed=1;							//���ౣ��CB�ѿ��źţ��͸�����
					//zqf 060918--��·��բ�ܶ�·����·���ౣ�����ֿ�
					objSwitches.m_st_FlagBreaker.bBackFeed=1;							//�͸���ذ壬�Լ����Դ������ϼ�¼
					objDebug.m_st_wDebug.bBpSTSShort3 = 1;
				}
			}
			else
			{
				m_i16FeedbackVcomCntPer14ms=0;
			}

		}
*/
		if(objSwitches.m_st_FlagBreaker.bInvKMShort == 1)
		{
			objInvCtrl.m_st_wFlagInvOnoff.bNeverRecoverOff=1;	    					//�ò��ɻָ��ػ���־
		}
	}

/************************************************************************************
��������	Alg_STSOpenFault()
����������	��բ�ܶ�·����
�������ã�	��
************************************************************************************/
	void	Class_Switches::Alg_STSOpenFault(void)
	{

		if ( objSystem.m_st_wFlagSystem.bTimer40ms == 1 )				//40ms��ʱ��
		{

			//��·STS��·�����ж�����:��·STS��ͨ��ĳ����·��ѹ�������ѹ���������15%,5sȷ��ʱ��
			if (( objXfer.m_st_XferResult.bBpSTS == 1 )
			    &&(objDigitalIO.m_st_wLocalSignal.bBPSF == 1)
			    &&(objBypass.m_st_FlagVbpTotal.bBpSTSOpenChkEnable==1)
			    &&((abs(objOutput.m_i16VoutaPrms-objBypass.m_i16VbpaPrms)>154)
				||(abs(objOutput.m_i16VoutbPrms-objBypass.m_i16VbpbPrms)>154)
				||(abs(objOutput.m_i16VoutcPrms-objBypass.m_i16VbpcPrms)>154)))			//��·��ѹ�������ѹ���10%
			{
				m_i16BpSTSOpenCntPer40ms ++;										//��·��բ�ܶ�·����������
				if ( m_i16BpSTSOpenCntPer40ms >=125 )								//5s
				{
					m_i16BpSTSOpenCntPer40ms = 125;
					objSwitches.m_st_FlagBreaker.bBpSTSOpen = 1;			//����·��բ�ܶ�·��־
					objDebug.m_st_wDebug.bBpSTSOpen1 = 1;
				}
			}
			else
			{
				if ( m_i16BpSTSOpenCntPer40ms > 0 && m_i16BpSTSOpenCntPer40ms <125 )
				{
					m_i16BpSTSOpenCntPer40ms --;									//��·��բ�ܶ�·�������ݼ�
				}
			}

			//���STS��·�����ж�����:���STS��ͨ��ĳ������ѹ�������ѹ���������15% ȷ��5��
			if ((objXfer.m_st_XferResult.bInvKM == 1) &&((abs(objOutput.m_i16VoutaPrms-objInverter.m_i16VinvaPrms)>154)
				||(abs(objOutput.m_i16VoutbPrms-objInverter.m_i16VinvbPrms)>154)
				||(abs(objOutput.m_i16VoutcPrms-objInverter.m_i16VinvcPrms)>154)))


			{
				m_i16InvSTSOpenCntPer40ms ++;										//��侧բ�ܶ�·����������
				if ( m_i16InvSTSOpenCntPer40ms >=25*5 )							//5s
				{
					m_i16InvSTSOpenCntPer40ms = 25*5;
					//objSwitches.m_st_FlagBreaker.bInvKMOpen = 1;						//���侧բ�ܶ�·���?
					objDebug.m_st_wDebug.bInvKMOpen1 = 1;
				}
			}
			else
			{
				if ( m_i16InvSTSOpenCntPer40ms > 0 && m_i16InvSTSOpenCntPer40ms <25*5 )
				{
					m_i16InvSTSOpenCntPer40ms --;									//��侧բ�ܶ�·�������ݼ�
				}
			}
		}

		//���������ѹ���ټ���������жϾ�բ�ܶ�·
		if (( objInvAlg.m_st_wIntMainFlag.bVoutdiffFast == 1)
			||(objInvAlg.m_st_wIntMainFlag.bVinvKMFast == 1))
		{
			objDebug.m_st_wDebug.bSTSOpenFast = 1;

			if ( objSwitches.m_st_FlagBreaker.bInvKM == 1 )
			{
				objSwitches.m_st_FlagBreaker.bInvKMOpen = 1;
			}
			else if ((objSwitches.m_st_FlagBreaker.bBpSTS == 1)&&(objDigitalIO.m_st_wLocalSignal.bBPSF == 1))
			{
				objSwitches.m_st_FlagBreaker.bBpSTSOpen = 1;
			}
		}

		if(objSwitches.m_st_FlagBreaker.bFuse == 0)
		{
			objSwitches.m_st_FlagBreaker.bInvKMOpen=0;			 //��˿�Ϻ�������侧բ�ܶ�·����
		}

		if ( ( objXfer.m_st_wXferCmd.bSwitchStatus == 1 ) && ( objSwitches.m_st_FlagBreaker.bInvKMOpen == 1 )
//			&& ( objBypass.m_st_FlagVbpTotalWide.bBpRange <=1 ) && ( objSwitches.m_st_FlagBreaker.bBpSTSOpen == 0 ) )
			&& ( objSwitches.m_st_FlagBreaker.bBpSTSOpen == 0 ) )
		{
			if ( objBypass.m_st_FlagVbpTotalWide.bBpRange <=1 )
			{
				objInvCtrl.m_st_wFlagInvOnoff.bNeverRecoverOff = 1;							//�ò��ɻָ��ػ���־
			}
			else if(objMonInterface.m_st_wParallelData.iNumOn > 1)
			{
				objInvCtrl.m_st_wFlagInvOnoff.bNeverRecoverOff = 1;
			}

		}
	}

//===========================================================================
// End of file.
//===========================================================================
