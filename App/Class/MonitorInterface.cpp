/*==========================================================*/ 	
/* Title		:	MonitorInterface.cpp											*/ 	
/* Description	: 	Class_MonitorInterface definition									*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#include "MonitorInterface.h"

/************************************************************************************
��������	App_MonitorVarInit()	
����������	MonitorINT16erface�������ʼ��		
�������ã�	
***********************************************************************************/
	 void	Class_MonitorInterface::App_MonitorVarInit(void)
	{
		UINT16	u16Temp;
		m_i16FaultResetCntPerMloop = 400;				//INV_RESET�˿ڵĹ��ܸı�
		
		m_i16ModelKVA = 50;						//50kVA		
		m_i16GrantKVA = 50;			
		//m_st_wUPSModeRun.bConfig = 3;
		m_st_wUPSModeRun.bConfig = 0;
		m_st_wUPSModeRun.bMode = 0;
		m_st_wUPSModeRun.bVoltOut = 1;				//380V
		m_st_wUPSModeRun.bID208 = 0;				//zqf051020:480V��Ȼ��Ϊ��400V ��ϵ��������mntr.c�жԽӴ����;�բ�ܹ��ϵķ���
		m_st_wUPSModeRun.bFreqOut = 0;				//50Hz
		
		m_st_wUPSModeRun2.bInvRunMode = 0;
		
		m_i16wSerialNumSet_0 = 0x10+1;
		
		m_i16BptoinvTimesSet = 5;
		
		m_i16VrefSetRms = 220;
		
		m_i16FbpTraceSet = 0;						//+-2Hz
		m_i16FbpLimitSet = 0;						//+-10%
		m_i16VbpUplimitSet = 1;					//+15%
		m_i16VbpDnlimitSet = 0;					//-20%

		u16Temp = objEEPROM.App_ParamRead(VoutARevAddr);
	if ( ( u16Temp >= 3686 ) && ( u16Temp <= 4506 ) )
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_a =  u16Temp;
	}
	else
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_a = 4096;					//��Ч����,��Ĭ��ֵ					
	}	
	
	u16Temp = objEEPROM.App_ParamRead(VoutBRevAddr);
	if ( ( u16Temp >= 3686 ) && ( u16Temp <= 4506 ) )
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_b =  u16Temp;
	}
	else
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_b = 4096;					//��Ч����,��Ĭ��ֵ					
	}	
	
	u16Temp = objEEPROM.App_ParamRead(VoutCRevAddr);
	if ( ( u16Temp >= 3686 ) && ( u16Temp <= 4506 ) )
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_c =  u16Temp;
	}
	else
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_c= 4096;					//��Ч����,��Ĭ��ֵ					
	}

	u16Temp = objEEPROM.App_ParamRead(IoutARevAddr);
	if ( ( u16Temp >= 3686 ) && ( u16Temp <= 4506 ) )
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_a =  u16Temp;
	}
	else
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_a = 4096;					//��Ч����,��Ĭ��ֵ					
	}

	u16Temp = objEEPROM.App_ParamRead(IoutBRevAddr);
	if ( ( u16Temp >= 3686 ) && ( u16Temp <= 4506 ) )
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_b =  u16Temp;
	}
	else
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_b= 4096;					//��Ч����,��Ĭ��ֵ					
	}

	u16Temp = objEEPROM.App_ParamRead(IoutCRevAddr);
	if ( ( u16Temp >= 3686 ) && ( u16Temp <= 4506 ) )
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_c=  u16Temp;
	}
	else
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_c = 4096;					//��Ч����,��Ĭ��ֵ					
	}

	u16Temp = objEEPROM.App_ParamRead(VinvARevAddr);
	if ( ( u16Temp >= 3686 ) && ( u16Temp <= 4506 ) )
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_a =  u16Temp;
	}
	else
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_a= 4096;					//��Ч����,��Ĭ��ֵ					
	}

	u16Temp = objEEPROM.App_ParamRead(VinvBRevAddr);
	if ( ( u16Temp >= 3686 ) && ( u16Temp <= 4506 ) )
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_b =  u16Temp;
	}
	else
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_b = 4096;					//��Ч����,��Ĭ��ֵ					
	}

	u16Temp = objEEPROM.App_ParamRead(VinvCRevAddr);
	if ( ( u16Temp >= 3686 ) && ( u16Temp <= 4506 ) )
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_c =  u16Temp;
	}
	else
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_c = 4096;					//��Ч����,��Ĭ��ֵ					
	}

	u16Temp = objEEPROM.App_ParamRead(IinvARevAddr);
	if ( ( u16Temp >= 3686 ) && ( u16Temp <= 4506 ) )
	{
		 m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_a=  u16Temp;
	}
	else
	{
		 m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_a = 4096;					//��Ч����,��Ĭ��ֵ					
	}

	u16Temp = objEEPROM.App_ParamRead(IinvBRevAddr);
	if ( ( u16Temp >= 3686 ) && ( u16Temp <= 4506 ) )
	{
		 m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_b =  u16Temp;
	}
	else
	{
		 m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_b = 4096;					//��Ч����,��Ĭ��ֵ					
	}

	u16Temp = objEEPROM.App_ParamRead(IinvCRevAddr);
	if ( ( u16Temp >= 3686 ) && ( u16Temp <= 4506 ) )
	{
		 m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_c=  u16Temp;
	}
	else
	{
		 m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_c= 4096;					//��Ч����,��Ĭ��ֵ					
	}
	
	u16Temp = objEEPROM.App_ParamRead(VbpARevAddr);
	if ( ( u16Temp >= 3482 ) && ( u16Temp <= 4710 ) )
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_a=  u16Temp;
	}
	else
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_a= 4096;					//��Ч����,��Ĭ��ֵ					
	}

	u16Temp = objEEPROM.App_ParamRead(VbpBRevAddr);
	if ( ( u16Temp >= 3482 ) && ( u16Temp <= 4710 ) )
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_b =  u16Temp;
	}
	else
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_b= 4096;					//��Ч����,��Ĭ��ֵ					
	}
	
       u16Temp = objEEPROM.App_ParamRead(VbpCRevAddr);
	if ( ( u16Temp >= 3482 ) && ( u16Temp <= 4710 ) )
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_c=  u16Temp;
	}
	else
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_c = 4096;					//��Ч����,��Ĭ��ֵ					
	}

		m_i16KsVbpaSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_a; 
		m_i16KsVbpbSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_b;
		m_i16KsVbpcSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_c;
		
		m_i16KsVinvaSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_a;
		m_i16KsVinvbSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_b;
		m_i16KsVinvcSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_c;
		
		m_i16KsVoutaSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_a;
		m_i16KsVoutbSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_b;
		m_i16KsVoutcSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_c;
		
		m_i16KsIinvaSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_a;
		m_i16KsIinvbSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_b;
		m_i16KsIinvcSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_c;
		
		m_i16KsIoutaSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_a;
		m_i16KsIoutbSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_b;
		m_i16KsIoutcSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_c;
		
	
		m_i16AlterPeriodSet = 0;					//�����ȱ��ݣ����������ȫ��
       
		m_i16FreqSlewRate = 1;					//0.1Hz/s
		m_i16VoltDispScale =2200;
		
		m_i16SelfAgingCurrRate = 20;				//20�� 
//�汾��ʼ��
		m_st_InvSendMonitor.m_st_ModuleVerInfo.ProtocolVer.u16ProductVer  = ProtocolVerVCnst;
		m_st_InvSendMonitor.m_st_ModuleVerInfo.ProtocolVer.u16BaseLineVer = ProtocolVerBCnst;
		m_st_InvSendMonitor.m_st_ModuleVerInfo.ProtocolVer.u16DevelopVer = ProtocolVerDCnst;

		m_st_InvSendMonitor.m_st_ModuleVerInfo.DspFwVer.u16ProductVer =  DspVerVCnst;                           
		m_st_InvSendMonitor.m_st_ModuleVerInfo.DspFwVer.u16BaseLineVer = DspVerBCnst ;                          
		m_st_InvSendMonitor.m_st_ModuleVerInfo.DspFwVer.u16DevelopVer  = DspVerDCnst ;                          
			                                                                                                    
		m_st_InvSendMonitor.m_st_ModuleVerInfo.CpldFwVer.u16ProductVer =  CpldVerVCnst;                         
		m_st_InvSendMonitor.m_st_ModuleVerInfo.CpldFwVer.u16BaseLineVer = CpldVerBCnst;                         
		m_st_InvSendMonitor.m_st_ModuleVerInfo.CpldFwVer.u16DevelopVer =  CpldVerDCnst; 

//		m_st_InvSendMonitor.m_st_ModuleVerInfo.CpldFwVer.u16ProductVer =  objSystem.m_i16CPLDVerD1;                         
//		m_st_InvSendMonitor.m_st_ModuleVerInfo.CpldFwVer.u16BaseLineVer = objSystem.m_i16CPLDVerD2;                         
//		m_st_InvSendMonitor.m_st_ModuleVerInfo.CpldFwVer.u16DevelopVer =  objSystem.m_i16CPLDVerD3;

		//�澯��״̬��ʼ��
		//״̬��
		//�տ��������Ϊ�Ͽ�
		m_st_InvSendMonitor.m_st_InvBreakerStatus.Word1_U.u16Word1All = 0;
		m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.u16Word2All = 0;

		//�澯
		m_st_InvSendMonitor.m_st_InvWarn.Word1_U.u16Word1All = 0;
		m_st_InvSendMonitor.m_st_InvWarn.Word2_U.u16Word2All = 0;
		m_st_InvSendMonitor.m_st_InvWarn.Word3_U.u16Word3All = 0;

		//�����ڲ���״̬��ʼ��
		objMonInterface.m_st_wParallelData.ParallelState[objMonInterface.m_i16wSerialNumSet_0-0x10].Word1_U.u16Word1All=0;
		objMonInterface.m_st_wParallelData.iNumBattInv = 0;
		objMonInterface.m_st_wParallelData.iNumInv = 0;
		objMonInterface.m_st_wParallelData.iNumOn = 0;
		objMonInterface.m_st_wParallelData.iNumBp = 0;
		objMonInterface.m_st_wParallelData.iNumUPS = 0;
		objMonInterface.m_st_wParallelData.iNumBattWarn = 0;
		objMonInterface.m_st_wParallelData.iNumBattInv = 0;
		objMonInterface.m_st_wParallelData.iParaWarnFlag = 0;
		objMonInterface.m_st_wParallelData.iParaRunFlag = 0;
		
		objMonInterface.m_st_wParallelData.iTotalNumInv=0;
		objMonInterface.m_st_wParallelData.iTotalNumOn=0;
		
		
	}
/************************************************************************************
��������	App_MonitorDeal()
����������	��ش���ģ�飬������������ò�������֯״̬�ϴ�
�������ã�	App_MonitorOrder():		��������ģ�飬�����ط����ĸ��࿪�����ػ���ȷ�ϼ���ȡ������
			App_SendtoMonitor():	�����Ϣ�ۺ�ģ�飬�����DSP��Monitor���ϱ���Ϣ
			App_MonitorParam():		��ز���ӳ��ģ�飬����Monitor���������Ĳ���
************************************************************************************/
	
	void	App_MonitorOrder( void );
	void	App_SendtoMonitor( void );
	void	App_MonitorParam( void );
	void    App_MonInterfaceWrite(void);
//	void	App_TimeLabel(void);
	void    App_MonInterfaceRead(void);

	void	Class_MonitorInterface::App_MonitorDeal()
	{
		//MODULE_NUM = 2;
		//����ֱ�ӶԺ���и�ֵ�������ϱ���淶
		objMonInterface.ModuleNum = 2;

		//�ӹ�������Ϣ
		App_ParallelLocalData( );
		
		App_MonInterfaceWrite();

		if(MODULE_NUM == 2)
		{
			Byp_App_MonInterfaceWrite();
		}	
		
		App_MonitorOrder();
		
		App_SendtoMonitor();
		
//		App_MonitorParam();
		
//		App_TimeLabel();
		                             		
		App_MonInterfaceRead();
		App_MonitorParam();
	}

/*************************************************************
�ļ����ƣ�	App_ParallelLocalData( )
����������	�����������ݷŵ���ӦUPS���ݴ�����Ԫ��
�㷨˵����	none

��ڲ�����	
			objSwitches.m_st_FlagBreaker
			m_i16Pouta
			m_i16Poutb
			m_i16Poutc
			m_i16Souta
			m_i16Soutb
			m_i16Soutc
			m_i16ModelKVA
			sParaData
			
���ڲ�����	
			sParaData
			uInvRealInfo
			iLocalStatusChanged
			
�ֲ�������	
			uLocalStatus
			uCanDataBuff1
�������ã�	none
*************************************************************/		
VOID Class_MonitorInterface::App_ParallelLocalData(VOID )
{
	_PARALLEL_SW1_T		uLocalStatus ;

	//����ID,ģ��ID��0��14
	UINT16 	u16Idx;
	u16Idx = objDigitalIO.m_st_wHardwareNewSignal.bInvID-1;
	
	uLocalStatus.Word1_U.u16Word1All = 0;
		
	if ( objSwitches.m_st_FlagBreaker.bInv == 1 )			//��俪��
	{
		uLocalStatus.Word1_U.tWord1Bits.bInvOn = 1;
	}
				
	if ( objSwitches.m_st_FlagBreaker.bOn == 1 )			//��乩��
	{
		uLocalStatus.Word1_U.tWord1Bits.bInvPowerOn = 1;
	}
					
	//��·�����ж�
	if ( objSwitches.m_st_FlagBreaker.bBpOn == 1 )		//��·����
	{
		uLocalStatus.Word1_U.tWord1Bits.bBypPowerOn = 1;
	}
	

	// ������·��ֵ��Χ��־
	if (objBypass.m_st_wBpAmpRangeFlag.bLocalWide == 1)
	{
		uLocalStatus.Word1_U.tWord1Bits.bBypRange = 1;  //�������߱�־	
	}

	/* �ϳɱ����澯��Ϣ */
	//�澯��Ϣ��Ҫ����´�TBD
#if (0)
	uCanDataBuff1 = ( S2U(objMonitor.uInvRealInfo) << 2 ) & 0xFC;
	sParaData[objMonInterface.m_i16wSerialNumSet_0-0x10].uSystemFlag |= uCanDataBuff1;
#endif
	uLocalStatus.Word1_U.tWord1Bits.bSingleOnLine = 1;	//�������߱�־
	uLocalStatus.Word1_U.tWord1Bits.bBypState = objSystem.m_st_wFlagStatus.bVbp;

	m_st_wParallelData.ParallelState[u16Idx] = uLocalStatus;
	
	
		

}


/************************************************************************************
��������	vMonitor_Order()
����������	��������ģ�飬�����ط����ĸ��࿪�����ػ���ȷ�ϼ���ȡ������
�������ã�	��
************************************************************************************/

	void	Class_MonitorInterface::App_MonitorOrder()

	{
		INT16 	i16temp;
		//����DSP�������Կ������ģ���Կ�������
		if( (objSystem.m_st_wFlagPrompt.bInvOnEnable == 1)					 //�������� 
			&& ((objInverter.m_st_wFlagInv.bDspCrazyOff ==  1)				//��DSP��������·������־
			||((objParallel.m_st_wParaSignalF.bMasterOnlineF== 1)&&( objInverter.m_st_wFlagInv.bInv == 0))))		//��ϵͳ����������佫���Կ���	��������������������
		{
			if(objSystem.m_st_wFlagSystem.bTimer1ms == 1)
			{
				if(m_i16SystemInvOnCnt<2000)
				{
						if(objSystem.m_st_wFlagPrompt.bSwitchTimesClamp ==0)
						{
							m_i16SystemInvOnCnt++;
							if(m_i16SystemInvOnCnt >=2000)				//ȷ��2s
							{
								m_i16SystemInvOnCnt =2000;
								m_st_wMonitorOrder.bOn = 1;
							}

						}
				}
			}
		}

		if ( m_st_wMonitorOrder.bOn == 1 )					//�������������
		{
			objInvCtrl.m_st_wFlagInvOnoff.bMonitorOn = 1;				//�ü�ؿ���״̬
			m_st_wMonitorOrder.bOn = 0;						//���������������
			objInverter.m_st_wFlagInv.bDspCrazyOff=0; 
			
		}

		if ( m_st_wMonitorOrder.bInstantOff == 1 )			//�����ػ�����
		{
			objInvCtrl.m_st_wFlagInvOnoff.bInstantOff = 1;				//�������ػ�״̬
			m_st_wMonitorOrder.bInstantOff = 0;				//�������ػ�����
		}
		//�û��ػ����л�����ػ�  �ػ��л������ ����3����ѭ����Ҫ��Ӳ����ʱС��1����ѭ��
		objInvCtrl.m_st_wFlagInvOnoff.bUserOffAct = objSystem.m_st_wFlagStatus.bUserOffBak;
		objSystem.m_st_wFlagStatus.bUserOffBak = objInvCtrl.m_st_wFlagInvOnoff.bUserOff;
		objInvCtrl.m_st_wFlagInvOnoff.bUserOff = 0;											//���û��ػ��л�����

		if ( m_st_wMonitorOrder.bOff == 1 )										//������ػ�����
		{
			if ( ( objInverter.m_st_wFlagInv.bInv != 0 ) && ( objParallel.m_st_wParaSignal.bInvBpStatus == 1 ) )    //������ѿ����ұ�������
			{
				//ʣ�๩�� ����(�� ̨����)=��̨��-1
				i16temp = objPowCANSched.m_i16OnTotalNum- 1;
				UPDNLMT16(i16temp,KKeepOnINVMaxCnst,0); //Ŀǰ������1+1

				if ( ( m_st_wUPSModeRun.bConfig == 3 ) && ( i16temp > 0 ) )		//����������������ܴ���1

				{
                                   //(Nxm�����Ϊ������ܹ���)
					//����һ̨���Ϲ���ʱ�嵥�����չػ���ʾλ
					objSystem.m_st_wFlagStatus.bSingleRiskOff = 0;

					//ʵ�ʴ��ش���ʣ�๩������ʱ��λ
					if(((((INT32)objSystem.m_i16RackSoutTotalMax*KBpKVA)>>10)>= (((INT32)objMonInterface.m_i16ModelKVA*1024)/objInverter.m_i16TempKVAGrant)*i16temp)
					   			||((((INT32)objSystem.m_i16RackPoutTotalMax*KBpKVA)>>10)>=(((INT32)objMonInterface.m_i16ModelKVA*1024)/objInverter.m_i16TempKWGrant)*i16temp))
					{
						objSystem.m_st_wFlagStatus.bParaRiskOff = 1;						//�ò���ϵͳ���չػ���ʾλ
					}
					else
					{
						objSystem.m_st_wFlagStatus.bParaRiskOff = 0;						//�岢��ϵͳ���չػ���ʾλ
					}

				}
				else 
				{
					//��̨����ʱ�岢��ϵͳ�ػ���ʾλ(Nxm�����Ϊ�����ܹ���)
					objSystem.m_st_wFlagStatus.bParaRiskOff = 0;
					//�����ֶ��ػ�������·�����ٻ���䲻ͬ�����Ҽ��ʱ�����ò�Ϊ��С�������������չػ���ʾ			
					//������·����������·��բ�ܶ�·		
					if ( (objBypass.m_st_FlagVbpTotal.bBpTptalProtect == 1)||(objSwitches.m_st_FlagBreaker.bBpSTSOpen == 1)
					||( ((objBypass.m_st_FlagVbpTotal.bBpRange == 1)||(objInverter.m_st_wFlagInv.bPhaseLock == 0)) && (m_i16IntSwTimeSet != 0))  )
					{
						objSystem.m_st_wFlagStatus.bSingleRiskOff = 1;						//�õ���ϵͳ���չػ���ʾλ
					}
					else
					{
						objSystem.m_st_wFlagStatus.bSingleRiskOff = 0;						//�嵥��ϵͳ���չػ���ʾλ
					}

				}
				
				if ( ( objSystem.m_st_wFlagStatus.bSingleRiskOff == 1 ) || ( objSystem.m_st_wFlagStatus.bParaRiskOff == 1 ) )
				{
					if ( m_st_wMonitorOrder.bRiskOffConf == 2 )					//���չػ�ȷ��λΪȡ��
					{
						m_st_wMonitorOrder.bOff = 0;								//��������ػ�����
						objSystem.m_st_wFlagStatus.bParaRiskOff = 0;						//�岢��ϵͳ���չػ���ʾλ
						objSystem.m_st_wFlagStatus.bSingleRiskOff = 0;						//�嵥��ϵͳ���չػ���ʾλ
						m_st_wMonitorOrder.bRiskOffConf = 0;						//����չػ�ȷ��λ
					}
					else
					{
						if ( m_st_wMonitorOrder.bRiskOffConf == 1 )				//���չػ�ȷ��λΪȷ��
						{
							objSystem.m_st_wFlagStatus.bParaRiskOff = 0;					//�岢��ϵͳ���չػ���ʾλ
							objSystem.m_st_wFlagStatus.bSingleRiskOff = 0;					//�嵥��ϵͳ���չػ���ʾλ
							m_st_wMonitorOrder.bRiskOffConf = 0;					//����չػ�ȷ��λ
							objInvCtrl.m_st_wFlagInvOnoff.bUserOff = 1;						//���û��ػ�λ
							m_st_wMonitorOrder.bOff = 0;							//��������ػ�����
						}
					}
				}	
				else
				{
					objSystem.m_st_wFlagStatus.bParaRiskOff = 0;							//�岢��ϵͳ���չػ���ʾλ
					objSystem.m_st_wFlagStatus.bSingleRiskOff = 0;							//�嵥��ϵͳ���չػ���ʾλ
					m_st_wMonitorOrder.bRiskOffConf = 0;							//����չػ�ȷ��λ
					objInvCtrl.m_st_wFlagInvOnoff.bUserOff = 1;								//���û��ػ�λ
					m_st_wMonitorOrder.bOff = 0;									//��������ػ�����
				}	
			}
			else
			{
				objSystem.m_st_wFlagStatus.bParaRiskOff = 0;								//�岢��ϵͳ���չػ���ʾλ
				objSystem.m_st_wFlagStatus.bSingleRiskOff = 0;								//�嵥��ϵͳ���չػ���ʾλ
				m_st_wMonitorOrder.bRiskOffConf = 0;								//����չػ�ȷ��λ
				objInvCtrl.m_st_wFlagInvOnoff.bUserOff = 1;									//���û��ػ�λ
				m_st_wMonitorOrder.bOff = 0;										//��������ػ�����
			}
		}

		if ( m_st_wMonitorOrder.bOutDisable == 1 )								//�����ֹ����
		{
			objInvCtrl.m_st_wFlagInvOnoff.bOutDisableOff = 1;								//�������ֹ̬�ػ�
			objSystem.m_st_wFlagStatus.bOutDisable = 1;									//�������ֹ״̬
			m_st_wMonitorOrder.bOutDisable = 0;									//�������ֹ����
		}

		if ( m_st_wMonitorOrder.bOutEnable == 1 )								//�����������
		{
			objSystem.m_st_wFlagStatus.bOutDisable = 0;									//�������ֹ״̬
			m_st_wMonitorOrder.bOutEnable = 0;									//�������������
		}

//zqf 060616--EPO�����Ż���EPOӲ���ź���Чʱ����ֹ�����������
		if(objDigitalIO.m_un_ExInARegConf.BIT.bEPO == 0)
		{
			m_st_wMonitorOrder.bReset = 0;
		  //m_st_wLocalSignal.bReset = 0;									
			m_i16FaultResetCntPerMloop = 0;
		}
		else if ( m_st_wMonitorOrder.bReset == 1 )								//�����������
		{

			if ( objInvCtrl.m_i16FlagOffReason <= 4 )										//�ǲ��ɻָ����Ϲػ�
			{
//				m_st_wLocalSignal.bReset = 1;									//��RESETӲ���ź�
				m_i16FaultResetCntPerMloop = 400;
			}		
			m_st_wMonitorOrder.bReset = 0;	

		}
		else																//�޹����������
		{
			if ( m_i16FaultResetCntPerMloop <= 0 )										//�������������
			{
//				m_st_wLocalSignal.bReset = 0;									//ֹͣ����RESET�ź�
			}
			else
			{
				//�ϵ�ʱ��Ӳ��RESET�źŵĲ���  iSpanFaultReset����ʼ��Ϊ400��				
//				if(m_st_wFlagSystem.bAfterParamDly==0)  m_st_wLocalSignal.bReset = 1;	
				m_i16FaultResetCntPerMloop --;											//��������������ݼ�
				if ( m_i16FaultResetCntPerMloop == 8 )
				{
					objSystem.m_st_wFlagStatus.bEPO = 0;									//������ػ�״̬
					objSystem.m_st_wFlagStatus.bInvFault = 0;								//�����������״̬
//zqf 060613--�������������Ϊ�����������
//					objSystem.m_st_wFlagStatus.bIGBTOCP = 0;								//��IGBT��������״̬
					objSystem.m_st_wFlagFault.bInvFault = 0;								//�����������״̬
//					objSystem.m_st_wFlagFault.bIGBTOCP = 0;								//��IGBT��������״̬					
					//20K���ӹ��������·�����־
					objSystem.m_st_wFlagStatus.bMainFeedBack = 0;		//���������·�����־
					//���������£���䲻������־
					objInverter.m_st_FlagVinvABC.bInvCurrUnbalancedWarn= 0;

					objBypass.m_st_FlagIbpTotal.bIbpOverFault = 0;
					objBypass.m_st_FlagIbpTotal.bIbpOverTimeOut = 0;
				}	
			}
		}

		//�����ݴ�����ڼ���޷�����ʱ��Ч
		m_st_wMonitorOrder.bRiskOffConf=0;
		if(objInverter.m_st_wFlagInv.bInv == 0)
		{
			objSystem.m_st_wFlagStatus.bSingleRiskOff = objSystem.m_st_wFlagStatus.bParaRiskOff =0;	
		}
		
		
		//����ⲿ����տ��ϻ��ڲ�����տ���ά�޿տ��������߼�����Ϊ����������״̬��
		if(m_st_wUPSModeRun.bConfig != 3)
		{
			m_i16OutputDelayCntPerMloop = 0;
			objParallel.m_st_wParaMaint.bIsolation = 0;			
		}
		else if (((objParallel.m_st_wParaSignalF.bExtOutStatusF == 0) && (objParallel.m_st_wParaSignalFBak.bExtOutStatusF == 0))
				|| ((objDigitalIO.m_st_HardwareSignal.bQ5 == 0) && (objDigitalIO.m_st_HardwareSignal.bQ3BP == 0)))
		{
//test		
//			objParallel.m_st_wParaMaint.bIsolation = 1;
//			m_i16ParaSignalExitMaskDelayCntPerMloop = 3;			
		}
		else
		{
			objParallel.m_st_wParaMaint.bIsolation = 0;
			if ( m_i16ParaSignalExitMaskDelayCntPerMloop > 0 )
			{
				m_i16ParaSignalExitMaskDelayCntPerMloop--;
			}
		}

		if ( m_i16ParaSignalExitMaskDelayCntPerMloop > 0 )		
		{
			objParallel.m_st_wParaMaint.bParaSignalMask = 1;	//�źŸ���̬
		}
		else										
		{
			objParallel.m_st_wParaMaint.bParaSignalMask = 0;
		}

		//�����ֹ̬���� ������״̬ �� ��TESTģʽ�� 
		//�򣨵�ǰ����ΪTESTģʽ �� �ڲ�����տ��պ� �� 
		if(((objParallel.m_st_wParaMaint.bIsolation ==1 )&&( objSystem.m_st_wFlagSystem.bUPSTestMode != 1)) 
			|| ((m_st_wUPSModeRun.bConfig == 3) &&( objParallel.m_st_wParaMaint.bTestAbnormal == 1) ))
		{
			objParallel.m_st_wParaMaint.bOutIsoDisable = 1;
			m_i16OutputDelayCntPerMloop = 25;
		}
		else if (m_i16OutputDelayCntPerMloop > 0)
		{
			m_i16OutputDelayCntPerMloop--;
		}
		else
		{
			objParallel.m_st_wParaMaint.bOutIsoDisable = 0;
			m_i16OutputDelayCntPerMloop = 0;
		}
		//�ػ�������Կ���������
		if(objInvCtrl.m_st_wFlagInvOnoff.bFinalOnOff  ==0)
		{
			if(m_i16SystemInvOnCnt >= 2000)
			{
				m_i16SystemInvOnCnt = 0;
			}
		}
	}


/************************************************************************************
��������	vSendto_Monitor()
����������	�����Ϣ�ۺ�ģ�飬�����DSP��Monitor���ϱ���Ϣ
�������ã�	��
************************************************************************************/
	void	Class_MonitorInterface::App_SendtoMonitor()

	{
		if ( objSystem.m_st_wFlagSystem.bKeyModuleDly == 1 )							//����ģ������ǰ��ʱ���
		{
			//By wxw20100416 �Ż�:���̬�£��������·��ѹ�����ĳ����٣����ϴ��澯�����ײ��л���Ȼ�߼��
			if((objBypass.m_st_FlagVbpTotal.bBpRange == 1) && (objBypass.m_st_FlagVbpFC.bVbpF == 0)
				&& (objXfer.m_st_wXferCmd.bSwitchStatus == 1))
			{
				objSystem.m_st_wFlagStatus.bVbp = 0;
			}
			else
			{
				objSystem.m_st_wFlagStatus.bVbp = objBypass.m_st_FlagVbpTotal.bBpRange;					//������·״̬
			}	
		}

		objSystem.m_st_wFlagStatus.bInv = objInverter.m_st_wFlagInv.bInv;								//���������״̬

//		m_st_wFlagStatus.bLock = objInverter.m_st_wFlagInv.bPhaseLock;						//������λ������־
		//zqf 040613--����������ģ���俪��ʱ������䲻ͬ����Ϣ�ϱ�
		//zqf 061117--�����·��բ�ܹز������⣬��俪��ʱ��䲻ͬ����Ϣ�ϱ�����ʱ��ӳ���8s-->20s
		if(objSystem.m_st_wFlagStatus.bInv == 3)
		{
			if(objSystem.m_st_wFlagSystem.bTimer40ms == 1)
			{
				objInverter.m_i16wInvWarn1LockConfCntPer40ms++; 					//�������������20sȷ��
			}

			if(objInverter.m_i16wInvWarn1LockConfCntPer40ms >= 500)
			{
				objInverter.m_i16wInvWarn1LockConfCntPer40ms = 500;
				objInverter.m_st_wFlagInv.bInvTraceConf = 1;
			}
				
		}
		else
		{
			objInverter.m_st_wFlagInv.bInvTraceConf = 0;
			objInverter.m_i16wInvWarn1LockConfCntPer40ms = 0;
		}

		if (objInverter.m_st_wFlagInv.bInvTraceConf == 1)	
		{
			objSystem.m_st_wFlagStatus.bLock = objInverter.m_st_wFlagInv.bPhaseLock; 		//������λ������־
		}
		else								
		{
			objSystem.m_st_wFlagStatus.bLock =1;  
		}


		if ( ( objSwitches.m_st_FlagBreaker.bQ5 == 1 ) && ( objParallel.m_st_wParaSignal.bInvBpStatus == 1 ) )    //Q5�ϣ�����
		{
			m_i16SupplyStatusCnt = 0;
			objSystem.m_st_wFlagStatus.bSupply = 2;											//��乩��
		}
		else if (( objSwitches.m_st_FlagBreaker.bQ5 == 1 ) && ( objXfer.m_st_XferResult.bBpSTS == 1 )) 															//
		{
			m_i16SupplyStatusCnt = 0;
		}
		else
		{
			m_i16SupplyStatusCnt ++;												//�����䵽��·�л�ʱ����״̬�ı�ʱ���������"��������"��
			if(m_i16SupplyStatusCnt >= 3)
			{
				objSystem.m_st_wFlagStatus.bSupply = 0;									//��������

			}
											
		}




		objSystem.m_st_wFlagFault.bOvload = objLoad.m_st_wFlagLoad.bOvload;							//�����������λ
		objSystem.m_st_wFlagFault.bInvOvloadTimeout = objLoad.m_st_wFlagLoad.bInvOvloadTimeout;		//������������ʱ��λ

		objSystem.m_st_wFlagPrompt.bSwitchTimesClamp = 0;								//�屾Сʱ�����л�������
		if ( objXfer.m_i16wBptoinvTimes > m_i16BptoinvTimesSet )
		{
			objSystem.m_st_wFlagPrompt.bSwitchTimesClamp = 1;							//�ñ�Сʱ�����л�������
		}

		objSystem.m_st_wFlagPrompt.bImpactSwitch = objLoad.m_st_wFlagLoad.bImpactSwitch;			//�����������������·��־
		objSystem.m_st_wFlagPrompt.bReqSwitch = objLoad.m_st_wFlagLoad.bReqSwitch;					//�����ڻ����س������·��־

		if ( objSystem.m_st_wFlagPrompt.bTotalCapPrompt0 == 1 )						//ϵͳ���ش��ڵ�ǰ��ЧUPS��������ʾ
		{
			if ( objSystem.m_st_wFlagSystem.bTimer40ms == 1 )							//40ms��
			{
				m_i16PromptCntPer40ms ++;
				
				//��������������ʱΪ6s
				if ( m_i16PromptCntPer40ms > 150 )								//��ʾȷ����ʱ 6����
				{
					m_i16PromptCntPer40ms = 150;

					objSystem.m_st_wFlagPrompt.bTotalCapPrompt = 1;					//��ϵͳ���ش��ڵ�ǰ��ЧUPS������(����ʱ)
				}
			}
		}
		else
		{
			objSystem.m_st_wFlagPrompt.bTotalCapPrompt = 0;							//��ϵͳ���ش��ڵ�ǰ��ЧUPS������(����ʱ)
			m_i16PromptCntPer40ms = 0;											//����ʱ�������ʾ
		}
	}


/************************************************************************************
��������	vMonitor_Param()
����������	��ز���ӳ��ģ�飬����Monitor���������Ĳ���
��������	l_div_i()		������
************************************************************************************/
	void	Class_MonitorInterface::App_MonitorParam()

	{
		INT16 i16MainAX;
		
		if ( (m_st_wInvReceiveMonitor.m_st_SysDSPSettings.u16SetParamFresh == 1)||(m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh != 0) )				//�������±�־
		{
			
			objSystem.App_SysParamConfig();
			objBypass.App_BpParamConfig();
			objInvAlg.App_InvAlgParamConfig();
			objInverter.App_InvParamConfig();
			objOutput.App_OutParamConfig();
			objDCBus.App_DCBusParamConfig();
//			objParallel.App_ParaParamConfig();
//			objSwitches.App_SwitchesParamConfig();	
			objSelfAgingAlg.App_SelfAgingAlgParamConfig();
			
			//�����������	

			m_st_wInvReceiveMonitor.m_st_SysDSPSettings.u16SetParamFresh = 0;					//��������±�־
		}

//���������������ܹ��ʼ���
		objSystem.m_i16PoutTotalMax = (INT32)objPowCANSched.m_i16PoutTotalMax;
		objSystem.m_i16SoutTotalMax = (INT32)objPowCANSched.m_i16SoutTotalMax;

//���ܼ����������ʼ���
			//By wxw20090725 Adator15k~20kPowerCan�����ش��ݸ��ش�С������PowerCan
			//ģ�����
			
		objSystem.m_i16RackPoutTotalMax = (INT32)objPowCANSched.m_i16PoutTotalMax;
		objSystem.m_i16RackSoutTotalMax = (INT32)objPowCANSched.m_i16SoutTotalMax;
		
		//�����ȱ���
		if ( m_st_wUPSModeRun.bConfig == 2 )					//�ȱ�������
		{
			if ( m_i16AlterPeriodSet > 0 )					//�����л�
			{
				i16MainAX = ((INT32)objTimerDrv.m_u16ClkH_0)/m_i16AlterPeriodSet;
				m_st_wUPSModeRun.bMasterECO = i16MainAX % 2;
			}
			else													//0�����Ӳ��л�
			{
				m_st_wUPSModeRun.bMasterECO = 0;				//����һֱ����
			}
		}
		else
		{
			m_st_wUPSModeRun.bMasterECO = 0;				//�ӻ�һ����?
		}

	}
/************************************************************************************
��������	App_Int_TimeLabel()
����������	ʱ�������
��������	��
************************************************************************************/
#pragma CODE_SECTION(".Epwm1Funcs")

void	Class_MonitorInterface::App_Int_TimeLabel(void)
{
	if(m_i16TimeSynCommand_0 == 1)
	{
		m_u16TimeLabel1_0 = m_SyncTime.u16TimeMS;
		m_u16TimeLabel2_0 = m_SyncTime.u16TimeS_L;
		m_u16TimeLabel3_0 = m_SyncTime.u16TimeS_H;
		
		m_i16TimeSynCommand_0 = 0;
	}
	if(objSystem.m_u16TimerBig1ms == 1)
	{
		m_u16TimeLabel1_0 ++;

		if(m_u16TimeLabel1_0>= 1000)
		{
			m_u16TimeLabel1_0 = 0;

			if(m_u16TimeLabel2_0 >= 65535)
			{
				m_u16TimeLabel3_0 ++;
			}
			
			m_u16TimeLabel2_0 ++;			
		}
	}
}

/************************************************************************************
��������	App_MonInterfaceSend()
����������	����״̬���澯��ģ���������
��������	��
************************************************************************************/
		void	Class_MonitorInterface::App_MonInterfaceWrite()
		{

/**************************************����ϴ�״̬����**************************************************/
			//ά����·�տ�״̬������·��
//			m_st_InvSendMonitor.m_st_InvBreakerStatus.Word1_U.tWord1Bits.bMaintainBypBrk = (objDigitalIO.m_st_HardwareSignal.bQ3BP |
//																				objParallel.m_st_wParaSignalF.bCabQ3BpStatusF);
		
			//������������	
			m_st_InvSendMonitor.m_st_InvBreakerStatus.Word1_U.tWord1Bits.bParamSetReq = objSystem.m_st_wFlagSystem.bParamSetReq;

			
	
			//20Kʹ�ã�UPS�ĳ�����������������ڹ��õ������������
			m_st_InvSendMonitor.m_st_InvBreakerStatus.Word1_U.tWord1Bits.bChargerOnNum = objPowCANSched.m_i16ChgOnTotalNum;
			
			/*��䷢�͵�ǿ��ת��������*/
			m_st_InvSendMonitor.m_st_InvBreakerStatus.Word1_U.tWord1Bits.bCmdCharge = objPowCANSched.m_i16ChgPBoostTotal;
			
			/*ʹ�ܵ��Ԥ�澯*/
			m_st_InvSendMonitor.m_st_InvBreakerStatus.Word1_U.tWord1Bits.bEnablePreWarn = objPowCANSched.m_i16BattPreWarnTotal ;
			/*��ͬ�Լ�*/
			m_st_InvSendMonitor.m_st_InvBreakerStatus.Word1_U.tWord1Bits.bEnableSeftTest = objPowCANSched.m_i16BattSelfTestTotal ;
		
	
			

			//��俪��״̬
			m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bStartupEna = objSystem.m_st_wFlagPrompt.bInvOnEnable;
			//���״̬
			m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bInvState = objSystem.m_st_wFlagStatus.bInv;
			//����״̬
			if ( objSystem.m_st_wFlagStatus.bOutDisable == 1 )
			{
				m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bInvSupplyState= 3;			//���������ֹ
			}
			else
			{
				m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bInvSupplyState = objSystem.m_st_wFlagStatus.bSupply;
			}
			//EPO
			m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bEpoPrompt = objSystem.m_st_wFlagStatus.bEPO;
			//ModelID
			m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bModelID = objDigitalIO.m_st_wHardwareNewSignal.bInvID;
			
			/*Power CANͨѶ״̬*/
//			m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bPwrCANState = 
			
			/* <=====�����л���ʾ======> */
			if ( objSystem.m_st_wFlagStatus.bIntSwitch == 1 )
			{
				m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bProcSwPrompt = 1;			//��·����л���ʾ
			}
			else if ( objSystem.m_st_wFlagStatus.bSingleRiskOff == 1 )
			{
				m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bProcSwPrompt = 2;			//������·�쳣�ػ���ʾ
			}
			else if ( objSystem.m_st_wFlagStatus.bParaRiskOff == 1 )
			{
				m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bProcSwPrompt = 3;			//������·�쳣�ػ���ʾ
			}
			else if ( objSystem.m_st_wFlagPrompt.bTotalCapPrompt == 1 )
			{
				m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bProcSwPrompt = 4;			//��������������ʾ
			}
			else if ( objSystem.m_st_wFlagPrompt.bUnitCapPrompt == 1 )
			{
				m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bProcSwPrompt = 5;			//�޷�����л���ʾ
			}
			else
			{
				m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bProcSwPrompt = 0;			//���κ���ʾ
			}

			if(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16RunMod == 2)
			{
				m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bECOMode = 1;			    //ECOģʽ
			}
			else
			{
				m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bECOMode = 0;
			}
/****************************����ϴ��澯��(��һ����)************************************************/
				/*��䲻ͬ��*/
			
			if (( objSystem.m_st_wFlagStatus.bInv == 3 ) && ( objSystem.m_st_wFlagStatus.bLock == 0 ))
			{
				m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bInvSync = 1;									
			}
			else
			{
				m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bInvSync = 0;
			}
			/*���������*/ 
			m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bInvFault = objSystem.m_st_wFlagFault.bInvFault;				                     
			/*���������*/ 
			m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bInvOverTemp = objDigitalIO.m_st_wHardwareNewSignal.bInvHeatSinkOVTemp;
			/*�����������*/
			if(objXfer.m_st_wXferCmd.bSwitchStatus ==1)
			{
				m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bSigOverLoad = objSystem.m_st_wFlagFault.bOvload;
			}
			else
			{
				m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bSigOverLoad = 0;
			}
			//����20/15K��Ŀֻ�Ǳ�ϵͳ���� ӦwxwҪ��ȥ�� 2009-9-15
			/*if(MODULE_NUM==2)
			{
				m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bSigOverLoad = 0;
			}*/
			
			/*�������س�ʱ*/ 											            
			m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bSigOverLoadTimeOut = objSystem.m_st_wFlagFault.bInvOvloadTimeout | objLoad.m_st_wFlagLoad.bShortCircuit;
			/*���������ѹ����*/									            
//			m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bOutOverVlt 
			/*�����ѹ�쳣*/								         
			m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bOutVltFualt = objSystem.m_st_wFlagPrompt.bVoutAbnormal;	
			/*���ֱ����������*/ 										 
//			m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bDcCurrOffsetOver 	
			/*���������*/  						        
//			m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bInvOverCurr = objSystem.m_st_wFlagFault.bIGBTOCP;
			/*ĸ���쳣�ػ�*/ 												             
			m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bBusAbnormalOff = objSystem.m_st_wFlagPrompt.bDCAbnormalOff;	
			/*ֱ��ĸ�߹�ѹ����*/										            
			m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bBusOverVlt =  objSystem.m_st_wFlagFault.bDCOV;	
			/*����й���*/											         
//			m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bInvInductOverTemp	
				
			/*���ȹ���*/							             
			m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bFanFault = objSystem.m_st_wFlagFault.bFanFault;												/*���ȹ���*/                 
			/*���̵�������*/
			if ( ( objSwitches.m_st_FlagBreaker.bInvKMShort == 1 ) || ( objSwitches.m_st_FlagBreaker.bInvKMOpen == 1 ) 
					||(objInverter.m_st_FlagVinvABC.bInvCurrUnbalancedWarn == 1))
			{
				m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bInvRelay = 1;								
			}
			else
			{			
				m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bInvRelay = 0;
			}
			/*�����˿��*/													          
			m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bOutFuseFault = !(objSwitches.m_st_FlagBreaker.bFuse);	
			/*������Դ2����*/ 										               
//			m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1BitsbAuxPwr2Down											

/****************************����ϴ��澯��(�ڶ�����)************************************************/	           
			/*�û���������*/
			m_st_InvSendMonitor.m_st_InvWarn.Word2_U.tWord2Bits.bUsrOpFault = objSystem.m_st_wFlagFault.bOperFault;	
			
			if(objLBS.m_st_LBSStatus.bLbsAbnormal == 1)
			{
				m_st_InvSendMonitor.m_st_InvWarn.Word2_U.tWord2Bits.bLBSstatus = 2;
			}
			else if(objLBS.m_st_LBSStatus.bLbsActive == 1)
			{
				m_st_InvSendMonitor.m_st_InvWarn.Word2_U.tWord2Bits.bLBSstatus = 1;
			}
			else
			{
				m_st_InvSendMonitor.m_st_InvWarn.Word2_U.tWord2Bits.bLBSstatus = 0;
			}
			//�����·
			m_st_InvSendMonitor.m_st_InvWarn.Word2_U.tWord2Bits.bOutShortFault = objSystem.m_st_wFlagPrompt.bShortCircuit;
/****************************����ϴ��澯��(��������)************************************************/
			/*��·���(��ѹ��Ƶ��)0:normal 1:������2:������*/					
			m_st_InvSendMonitor.m_st_InvWarn.Word3_U.tWord3Bits.bBypState =objSystem.m_st_wFlagStatus.bVbp;	
			/*��·����*/ 	
			m_st_InvSendMonitor.m_st_InvWarn.Word3_U.tWord3Bits.bBypPhaseReverse = objSystem.m_st_wFlagFault.bBpInvtSeq; 
			/*���س��ת��·*/
			m_st_InvSendMonitor.m_st_InvWarn.Word3_U.tWord3Bits.bToBypByLoadPuls = objSystem.m_st_wFlagPrompt.bImpactSwitch; 
			/*��·�л���������*/
			m_st_InvSendMonitor.m_st_InvWarn.Word3_U.tWord3Bits.bBypSwTimesLtd = objSystem.m_st_wFlagPrompt.bSwitchTimesClamp;
			/*��·��������*/	
//			m_st_InvSendMonitor.m_st_InvWarn.Word3_U.tWord2Bits.bBypOverCurr	= objSystem.m_st_wFlagFault.bBpOverCurrent;  
			/*��·��բ�ܹ���0:normal 1:����2:��·3:��·*/ 
			
			if((objSwitches.m_st_FlagBreaker.bBpSTSShort == 1) && (objSwitches.m_st_FlagBreaker.bReqBpSTSShort == 0))
			{
				m_st_InvSendMonitor.m_st_InvWarn.Word3_U.tWord3Bits.bBypScrFault	= 3; 
				
			}
			else if((objSwitches.m_st_FlagBreaker.bBpSTSOpen == 1)&&(objDigitalIO.m_un_ExInAReg.BIT.bBPSCRFromBp == 1))
			{
				m_st_InvSendMonitor.m_st_InvWarn.Word3_U.tWord3Bits.bBypScrFault = 2;
			} 
			else 
			{  
				m_st_InvSendMonitor.m_st_InvWarn.Word3_U.tWord3Bits.bBypScrFault = 0;
				
			}

			
			
			/*��·�쳣�ػ��澯*/
			m_st_InvSendMonitor.m_st_InvWarn.Word3_U.tWord3Bits.bBypAbnorOff	= objSystem.m_st_wFlagFault.bBpFailOff;
			/*���������*/ 	                
//			m_st_InvSendMonitor.m_st_InvWarn.Word3_U.tWord3Bits. bParaBoardFault	
			/*������������*/
			m_st_InvSendMonitor.m_st_InvWarn.Word3_U.tWord3Bits. bSysShareCurr = objSystem.m_st_wFlagPrompt.bParaShareFault;
			/*���������ӹ���*/	
//			m_st_InvSendMonitor.m_st_InvWarn.Word3_U.tWord3Bits. bParaLineFault = objSystem.m_st_wFlagPrompt.bParaCableFault;

			/*20Kһ��������ֻ��һ��ģ��,�������ڲ�ͨ�Ź���*/ 	
			m_st_InvSendMonitor.m_st_InvWarn.Word3_U.tWord3Bits. bParaCom = 0;
            
            /*����ϵͳ����*/		
            m_st_InvSendMonitor.m_st_InvWarn.Word3_U.tWord3Bits. bSysOverLoad = objSystem.m_st_wFlagPrompt.bParaOvload;
            /*��module����ת��·*/	
            m_st_InvSendMonitor.m_st_InvWarn.Word3_U.tWord3Bits.bToBypReq = objSystem.m_st_wFlagPrompt.bReqSwitch;
/*****************************����ϴ�ģ����**********************************************/  
			/*������й�����*/			
	        m_st_InvSendMonitor.m_st_InvAnalog.OutActPwr.u16Phase_a = (UINT16)(objInverter.m_i16PinvaforShow);                                                  	
            m_st_InvSendMonitor.m_st_InvAnalog.OutActPwr.u16Phase_b = (UINT16)(objInverter.m_i16PinvbforShow);
            m_st_InvSendMonitor.m_st_InvAnalog.OutActPwr.u16Phase_c = (UINT16)(objInverter.m_i16PinvcforShow);

			/*��������ڹ���*/
           	 m_st_InvSendMonitor.m_st_InvAnalog.OutApparentPwr.u16Phase_a = (UINT16)(objInverter.m_i16SinvaforShow);
        	m_st_InvSendMonitor.m_st_InvAnalog.OutApparentPwr.u16Phase_b = (UINT16)(objInverter.m_i16SinvbforShow); 
        	m_st_InvSendMonitor.m_st_InvAnalog.OutApparentPwr.u16Phase_c = (UINT16)(objInverter.m_i16SinvcforShow);  

			/*�������������*/
        	if ( objInverter.m_i16Sinva < Sout5CNST )					//���ڹ���<5%,��������=0
			{
				m_st_InvSendMonitor.m_st_InvAnalog.OutFactor.u16Phase_a = 0;
			}
		    else if(objSystem.m_st_wFlagSystem.bSelfAgingAction	==	0)
		    {
            	m_st_InvSendMonitor.m_st_InvAnalog.OutFactor.u16Phase_a = (UINT16)( objInverter.m_i16PFinva );
            }
            else
            {
            	m_st_InvSendMonitor.m_st_InvAnalog.OutFactor.u16Phase_a = 100;
            }
            
           	if ( objInverter.m_i16Sinvb < Sout5CNST )					//���ڹ���<5%,��������=0
		    {
			    m_st_InvSendMonitor.m_st_InvAnalog.OutFactor.u16Phase_b = 0;
		    }
		    else if(objSystem.m_st_wFlagSystem.bSelfAgingAction==0)
		    {
            	m_st_InvSendMonitor.m_st_InvAnalog.OutFactor.u16Phase_b = (UINT16)( objInverter.m_i16PFinvb );
            }
           	else
           	{
            	m_st_InvSendMonitor.m_st_InvAnalog.OutFactor.u16Phase_b = 100;
            }
            
            if ( objInverter.m_i16Sinvc < Sout5CNST )					//���ڹ���<5%,��������=0
		    {
			    m_st_InvSendMonitor.m_st_InvAnalog.OutFactor.u16Phase_c = 0;
		    }
		    else if(objSystem.m_st_wFlagSystem.bSelfAgingAction==0)
		    {
           	    m_st_InvSendMonitor.m_st_InvAnalog.OutFactor.u16Phase_c = (UINT16)( objInverter.m_i16PFinvc );
            }
      	    else
            {
                m_st_InvSendMonitor.m_st_InvAnalog.OutFactor.u16Phase_c = 100;
            }

		    /*������ѹ*/
		  	m_st_InvSendMonitor.m_st_InvAnalog.OutPhaseVlt.u16Phase_a = (UINT16)((((INT32)( objOutput.m_unVoutaRms.half.hword )) * m_i16VoltDispScale)>>10);
           	m_st_InvSendMonitor.m_st_InvAnalog.OutPhaseVlt.u16Phase_b = (UINT16)((((INT32)( objOutput.m_unVoutbRms.half.hword )) * m_i16VoltDispScale)>>10);
           	m_st_InvSendMonitor.m_st_InvAnalog.OutPhaseVlt.u16Phase_c = (UINT16)((((INT32)( objOutput.m_unVoutcRms.half.hword )) * m_i16VoltDispScale)>>10);
		 
			/*��������*/
         	m_st_InvSendMonitor.m_st_InvAnalog.OutPHaseCurr.u16Phase_a = (UINT16)( objInverter.m_i16IinvaforShow  );
         	m_st_InvSendMonitor.m_st_InvAnalog.OutPHaseCurr.u16Phase_b = (UINT16)( objInverter.m_i16IinvbforShow  );
        	m_st_InvSendMonitor.m_st_InvAnalog.OutPHaseCurr.u16Phase_c = (UINT16)( objInverter.m_i16IinvcforShow  );

			/*������ѹ*/ 
           	m_st_InvSendMonitor.m_st_InvAnalog.InvPhaseVlt.u16Phase_a = (UINT16)((((INT32)( objInverter.m_unVinvaRms.half.hword )) * m_i16VoltDispScale)>>10);
			m_st_InvSendMonitor.m_st_InvAnalog.InvPhaseVlt.u16Phase_b = (UINT16)((((INT32)( objInverter.m_unVinvbRms.half.hword )) * m_i16VoltDispScale)>>10);
			m_st_InvSendMonitor.m_st_InvAnalog.InvPhaseVlt.u16Phase_c = (UINT16)((((INT32)( objInverter.m_unVinvcRms.half.hword )) * m_i16VoltDispScale)>>10);

			/*��������*/
        	m_st_InvSendMonitor.m_st_InvAnalog.InvPhaseCurr.u16Phase_a = (UINT16)( objInverter.m_i16IinvaforShow );
           	m_st_InvSendMonitor.m_st_InvAnalog.InvPhaseCurr.u16Phase_b = (UINT16)( objInverter.m_i16IinvbforShow );
           	m_st_InvSendMonitor.m_st_InvAnalog.InvPhaseCurr.u16Phase_c = (UINT16)( objInverter.m_i16IinvcforShow ); 

			/*��������*/
           	m_st_InvSendMonitor.m_st_InvAnalog.InductCurr.u16Phase_a = (UINT16)( objInverter.m_i16IlaforShow );
           	m_st_InvSendMonitor.m_st_InvAnalog.InductCurr.u16Phase_b = (UINT16)( objInverter.m_i16IlbforShow );
           	m_st_InvSendMonitor.m_st_InvAnalog.InductCurr.u16Phase_c = (UINT16)( objInverter.m_i16IlcforShow ); 

			/*��·���ѹ*/
           	m_st_InvSendMonitor.m_st_InvAnalog.BypPhaseVlt.u16Phase_a = (UINT16)((((INT32)( objBypass.m_unVbpaRms.half.hword )) * m_i16VoltDispScale)>>10);
        	m_st_InvSendMonitor.m_st_InvAnalog.BypPhaseVlt.u16Phase_b = (UINT16)((((INT32)( objBypass.m_unVbpbRms.half.hword )) * m_i16VoltDispScale)>>10);
        	m_st_InvSendMonitor.m_st_InvAnalog.BypPhaseVlt.u16Phase_c = (UINT16)((((INT32)( objBypass.m_unVbpcRms.half.hword )) * m_i16VoltDispScale)>>10);

			/*��������ذٷֱ�*/
        	m_st_InvSendMonitor.m_st_InvAnalog.OutLoadRate.u16Phase_a = (UINT16)( objInverter.m_i16LoadRatea );
        	m_st_InvSendMonitor.m_st_InvAnalog.OutLoadRate.u16Phase_b = (UINT16)( objInverter.m_i16LoadRateb );
        	m_st_InvSendMonitor.m_st_InvAnalog.OutLoadRate.u16Phase_c = (UINT16)( objInverter.m_i16LoadRatec );
        	/*������ֵ��*/
        	if ( objInverter.m_i16IinvaPrms < Current5CNST )				//������<5%,��ֵ��=1.4
		{
         	   	m_st_InvSendMonitor.m_st_InvAnalog.OutPeakRate.u16Phase_a =0; 
       	 }
           	 else if(objSystem.m_st_wFlagSystem.bSelfAgingAction == 0)
		{ 
			m_st_InvSendMonitor.m_st_InvAnalog.OutPeakRate.u16Phase_a = (UINT16)((((INT32)( objInverter.m_u16IinvaCrest_0 )) * 10)>>8);
		}
		else
		{
			m_st_InvSendMonitor.m_st_InvAnalog.OutPeakRate.u16Phase_a = (UINT16)(((INT32)3584)>>8);
		}
		
		if ( objInverter.m_i16IinvbPrms < Current5CNST )				//������<5%,��ֵ��=1.4
		{
          	  	m_st_InvSendMonitor.m_st_InvAnalog.OutPeakRate.u16Phase_b =0; 
       	 }
            	else if(objSystem.m_st_wFlagSystem.bSelfAgingAction==0)
		{ 
			m_st_InvSendMonitor.m_st_InvAnalog.OutPeakRate.u16Phase_b = (UINT16)((((INT32)( objInverter.m_u16IinvbCrest_0 )) * 10)>>8);
		}
		else
		{
			m_st_InvSendMonitor.m_st_InvAnalog.OutPeakRate.u16Phase_b = (UINT16)(((INT32)3584)>>8);
		}
		
		if ( objInverter.m_i16IinvcPrms < Current5CNST )				//������<5%,��ֵ��=1.4
		{
            m_st_InvSendMonitor.m_st_InvAnalog.OutPeakRate.u16Phase_c =0; 
       	}
           	 else if(objSystem.m_st_wFlagSystem.bSelfAgingAction==0)
		{ 
			m_st_InvSendMonitor.m_st_InvAnalog.OutPeakRate.u16Phase_c = (UINT16)((((INT32)( objInverter.m_u16IinvcCrest_0 )) * 10)>>8);
		}
		else
		{
			m_st_InvSendMonitor.m_st_InvAnalog.OutPeakRate.u16Phase_c = (UINT16)(((INT32)3584)>>8);			
		}
		
			/*���Ƶ��*/
		if ( ( objInverter.m_i16VinvaPrms < Voltage80CNST ) || ( objInverter.m_i16VinvbPrms < Voltage80CNST )
				|| ( objInverter.m_i16VinvcPrms < Voltage80CNST ) )
		{
			m_st_InvSendMonitor.m_st_InvAnalog.OutFreq.u16Self = 0;
		}
		else
		{			
			m_st_InvSendMonitor.m_st_InvAnalog.OutFreq.u16Self = (UINT16)( objInverter.m_i16FinvMean );			
		}
			/*��·Ƶ��*/
			//���������߼�һ�£�����·��ѹģֵ������Чֵ30V��Ӧ��ģʱ����Ƶ����Ϊ����
		if ( objBypass.m_unVbpMMean_0.half.hword < 790 )
		{	
			m_st_InvSendMonitor.m_st_InvAnalog.BypFreq.u16Self = 0;
		}
		else
		{
			m_st_InvSendMonitor.m_st_InvAnalog.BypFreq.u16Self = (UINT16)( objBypass.m_i16FbpMean );			
		}
			/*�����Ա���*/
		m_st_InvSendMonitor.m_st_InvAnalog.DbgVar.u16Dbg1 = *pDSPVar1Adr;
		m_st_InvSendMonitor.m_st_InvAnalog.DbgVar.u16Dbg2 = *pDSPVar2Adr;
		m_st_InvSendMonitor.m_st_InvAnalog.DbgVar.u16Dbg3 = *pDSPVar3Adr;   
/*************************************�汾��Ϣ�ϴ�******************************************************/  
		m_st_InvSendMonitor.m_st_ModuleVerInfo.ProtocolVer.u16ProductVer =  ProtocolVerVCnst;                   
		m_st_InvSendMonitor.m_st_ModuleVerInfo.ProtocolVer.u16BaseLineVer = ProtocolVerBCnst;                   
		m_st_InvSendMonitor.m_st_ModuleVerInfo.ProtocolVer.u16DevelopVer =  ProtocolVerDCnst;                   
			                                                                                                    
		m_st_InvSendMonitor.m_st_ModuleVerInfo.DspFwVer.u16ProductVer =  DspVerVCnst;                           
		m_st_InvSendMonitor.m_st_ModuleVerInfo.DspFwVer.u16BaseLineVer = DspVerBCnst ;                          
		m_st_InvSendMonitor.m_st_ModuleVerInfo.DspFwVer.u16DevelopVer  = DspVerDCnst ;                          
			                                                                                                    
		m_st_InvSendMonitor.m_st_ModuleVerInfo.CpldFwVer.u16ProductVer =  objSystem.m_i16CPLDVerD1;                         
		m_st_InvSendMonitor.m_st_ModuleVerInfo.CpldFwVer.u16BaseLineVer = objSystem.m_i16CPLDVerD2;                         
		m_st_InvSendMonitor.m_st_ModuleVerInfo.CpldFwVer.u16DevelopVer =  objSystem.m_i16CPLDVerD3;  
		
			
	}
/*******************************************************************************************************
��������	App_MonInterfaceReceive()                                                         
����������	�������Լ�صĲ������ü�����                                                   
��������	��                                                                             
************************************************************************************/      
	void	Class_MonitorInterface::App_MonInterfaceRead()
	{				
 /********************************ϵͳ������*************************************************/

        if(objInverter.m_st_wFlagInv.bInv != 0)
        {
            m_st_wUPSModeRun.bConfig = 3;
        }
        else
        {
            m_st_wUPSModeRun.bConfig = 0;
        }
 		if(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.u16SetParamFresh == 1)
		{ 			
 			 m_i16ModelKVA = m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16CapType;       	 	/*������������*/                 
         		 m_i16GrantKVA = m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16CapType;
         	 //ϵͳ��ѹ�ȼ�����           	
         	 switch  (m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16SysVltGrade)
         	 { 
         		case 380:
         	 			m_st_wUPSModeRun.bVoltOut = 1;                   //380v
         	 			break;
         	 	case 400:                                                      
					m_st_wUPSModeRun.bVoltOut = 0;                   //400v 
					break;                                                  
			case 415:                                                      
					m_st_wUPSModeRun.bVoltOut = 2;                   //415v 
					break;                                                  
			case 480:                                                      
					m_st_wUPSModeRun.bVoltOut = 5;                   //480v 
					break;                                                  
			default:   				 
					break;                                                      
         	}
 	 
        	  //����ģʽ���� 
        	 if(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16RunMod == 1)
        	  {
        	  	m_st_wUPSModeRun.bMode = 0;        	  	
        	  } 
        	  else if(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16RunMod == 2)
        	  {
        	  	m_st_wUPSModeRun.bMode = 1;
        	  }

/* 	 
        	  //����������ģʽ����
        	  //����
              if(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16UpsConfigType  == 1)
        	  {
        	  	m_st_wUPSModeRun.bConfig = 0;        	  	
        	  } 

			  // ����
        	  else if(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16UpsConfigType == 2)
        	  {
        	  	m_st_wUPSModeRun.bConfig = 3;
        	  }
			  //�ȱ�������
        	  else if(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16UpsConfigType == 3)
        	  {
        	  	m_st_wUPSModeRun.bConfig = 1;
        	  }
			  
        	  else if(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16UpsConfigType == 4)
        	  {
        	  	m_st_wUPSModeRun.bConfig = 2;
        	  }
        	  else
        	  {
        	  m_st_wUPSModeRun.bConfig = 0;
        	  }
*/
              //���Ƶ�ʵȼ�����   
             if(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutFreq == 50)
              {
              	m_st_wUPSModeRun.bFreqOut = 0;
              } 
              else if(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutFreq == 60) 
              {
              	m_st_wUPSModeRun.bFreqOut = 1;             
              }
  

			   //���ϻ���������ٷֱ����� 
            if(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16AgingOutCurrRate <= 130)
              {
              	m_i16SelfAgingCurrRate = (INT16)(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16AgingOutCurrRate);
              }  

			   //���������ģʽ����
            if(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16InvRunMode ==2)
            {
              	 m_st_wUPSModeRun2.bInvRunMode = 2;
            } 
            else if (m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16InvRunMode ==1)
            {
              	 m_st_wUPSModeRun2.bInvRunMode = 1;
            }
            else
            {
            	m_st_wUPSModeRun2.bInvRunMode = 0;
            }
			  
			  
			   //�������л�ʱ������ms 
               if(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutSwitchTime <= 4)
              {
              	m_i16IntSwTimeSet = (INT16)(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutSwitchTime);
              }  
              //���Ƶ�ʸ�����������  
              if((m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutFreqTrackRate >= 1)
			&&(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutFreqTrackRate <= 30))
              {
              	m_i16FreqSlewRate = (INT16)(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutFreqTrackRate);
              }                    	

			 //����������������
         	 m_st_wUPSModeRun.bPowerFactor = m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16InvOutFactor;

			  //�����ѹ΢��ϵ������                  
         	if((m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutVltMicroAdj >= 263)
			&&(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutVltMicroAdj <= 291))
              {
              	m_i16VrefSetRms = (INT16)(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutVltMicroAdj);
              }
 
         	  //��·�л��������� 
         	 if((m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypSwitchTime >= 1)
			  && (m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypSwitchTime <= 10))
              {
              	m_i16BptoinvTimesSet = (INT16)(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypSwitchTime);
              }       

			   //��·Ƶ�ʸ��ٷ�Χ���� 
         	 if((m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypFreqTrackLtd >= 1)
			&& (m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypFreqTrackLtd <= 4))
              {
              	m_i16FbpTraceSet = (INT16)(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypFreqTrackLtd-1);
              } 
                  

		 		 //��·��ѹ������Χ����(����)
		 if((m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypVltUpLtd >= 1)
		 	&& (m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypVltUpLtd <= 3))  //��·��ѹ��Χ���޿�����Ϊ+20%
         {
              m_i16VbpUplimitSet = (INT16)(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypVltUpLtd - 1);              	
         }
 

			  //��·��ѹ������Χ����(����) 
		if((m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypVltDownLtd >= 1)
			&& (m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypVltDownLtd <= 4))
              {
              	m_i16VbpDnlimitSet = (INT16)(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypVltDownLtd-1);
              }
                       
			   //��·Ƶ�ʱ�����Χ���� 
		if((m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypFreqLtd == 1)
			||(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypFreqLtd == 2))
//			||(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypFreqLtd == 3)
//			||(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypFreqLtd == 4))
              {
              	m_i16FbpLimitSet = (INT16)(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypFreqLtd-1);
              }
 
			 //��·���ౣ���ö�·�� 			       			  
//			  m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypProtectBrk	 
      
			//���ܺ�
//			m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16RMIdx

              //������������  
//             m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16RMNum

               //��������ģ����
               //20k�ײ������û���ģ����������ģ�����������յĲ��������ڲ�������̨��������̨��
              if(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16RMNum <= 4)
              {
              	m_i16BasicNum = (INT16)(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16RMNum);
              }
 

			 //��������ģ����
              if(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16ReRMNum <= 3)
              {
              	m_i16RedunNum = (INT16)(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16ReRMNum);
              } 
              
         	   //LBS mode 0:none 1: master 2: slaver
        	 switch (m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16LBSMode)
         	 {
         	   		case 1:
         	   			m_st_wUPSModeRun.bLBS = 0;
         	   			break;
         	   		case 2:
         	   			m_st_wUPSModeRun.bLBS = 1;
         	   			break;
         	   		case 3:
         	   			m_st_wUPSModeRun.bLBS = 2;
         	   			break;
         	   		default:
         	   			break;        	   	
         	  }    

		//bat share
		if(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BatShare == 1)
		{
			m_st_wUPSModeRun2.bBattShare = 1;               //share module
		}
		else
		{
			m_st_wUPSModeRun2.bBattShare = 0;			//no share	
		}
		
		
     	 }

 /************************************ģ�����Ľ��� ***********************************************/
//�����¶�
 		objInverter.m_i16wEnvirTemp = (INT16)m_st_wInvReceiveMonitor.m_st_InvRecvAnalog.u16EnvirTemp;
//�����ڲ���������й�����
 		//objSystem.m_i16PoutaTotal = (INT16)m_st_wInvReceiveMonitor.m_st_InvRecvAnalog.OutActPwr.u16Phase_a;
		//objSystem.m_i16PoutbTotal = (INT16)m_st_wInvReceiveMonitor.m_st_InvRecvAnalog.OutActPwr.u16Phase_b;
		//objSystem.m_i16PoutcTotal = (INT16)m_st_wInvReceiveMonitor.m_st_InvRecvAnalog.OutActPwr.u16Phase_c;
		objSystem.m_i16PoutaTotal = objPowCANSched.m_i16PoutaTotal;
		objSystem.m_i16PoutbTotal = objPowCANSched.m_i16PoutbTotal;
		objSystem.m_i16PoutcTotal = objPowCANSched.m_i16PoutcTotal;
//�����ڲ�����������ڹ���
		//objSystem.m_i16SoutaTotal = (INT16)m_st_wInvReceiveMonitor.m_st_InvRecvAnalog.OutApparentPwr.u16Phase_a;
		//objSystem.m_i16SoutbTotal = (INT16)m_st_wInvReceiveMonitor.m_st_InvRecvAnalog.OutApparentPwr.u16Phase_b;
		//objSystem.m_i16SoutcTotal = (INT16)m_st_wInvReceiveMonitor.m_st_InvRecvAnalog.OutApparentPwr.u16Phase_c;
		objSystem.m_i16SoutaTotal = objPowCANSched.m_i16SoutaTotal;
		objSystem.m_i16SoutbTotal = objPowCANSched.m_i16SoutbTotal;
		objSystem.m_i16SoutcTotal = objPowCANSched.m_i16SoutcTotal;
		
/////////////////////////////////////////////////////////////////////////////
//���ܼ䲢������ϵͳ������й�����
		//By wxw20090725 Adator15k~20kPowerCan�����ش��ݸ��ش�С
        objSystem.m_i16RackPoutaTotal =  objPowCANSched.m_i16PoutaTotal;     
		objSystem.m_i16RackPoutbTotal =  objPowCANSched.m_i16PoutbTotal;
		objSystem.m_i16RackPoutcTotal =  objPowCANSched.m_i16PoutcTotal;
//���ܼ䲢������ϵͳ��������ڹ���
		objSystem.m_i16RackSoutaTotal =  objPowCANSched.m_i16SoutaTotal;     
		objSystem.m_i16RackSoutbTotal =  objPowCANSched.m_i16SoutbTotal;
		objSystem.m_i16RackSoutcTotal =  objPowCANSched.m_i16SoutcTotal;
/************************************��·��տ���ά�޿տ� ***********************************************/		
//�ڲ�ά�޿տ�
	//	objDigitalIO.m_st_HardwareSignal.bQ3BP = m_st_wBypBreakData.Word1_U.tWord1Bits.bQ3Bp;
//��·��ά�޿տ�
	//	objDigitalIO.m_st_wLocalNewSignal.bCAB_Q3BP_STATUS = m_st_wBypBreakData.Word1_U.tWord1Bits.bExyQ3Bp;
//��·������տ�
	//	objDigitalIO.m_st_wLocalNewSignal.bEXT_OUT_STATUS = m_st_wBypBreakData.Word1_U.tWord1Bits.bQ5;

 /************************************��ز�ӹ���ϵͳ״̬ ***********************************************/
//ϵͳ��·������״̬
 		objBypass.m_st_FlagVbpTotal.bBpTptalProtect =  m_st_MonRealMsg.Word1_U.tWord1Bits.bBypOverProtectState;
 
 //����Լ�״̬
		//objPowCANSched.m_st_BatteryStatus.bBattSelfTest = m_st_MonRealMsg.Word1_U.tWord1Bits.bBatSeftTest;
 		if (m_u16BatDeepSelfRecved)
		{
			objPowCANSched.m_st_BatteryStatus.bBattSelfTest	= m_st_MonRealMsg.Word1_U.tWord1Bits.bBatSeftTest;
			
		}

		else
		{
			objPowCANSched.m_st_BatteryStatus.bBattSelfTest	= 0;
		}
/**********************************���У������****************************************/
		if(m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh != 0)
		{
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_a >= 3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_a <= 4506))
			{	
				 m_i16KsVoutaSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_a;	        //A�������ѹУ��ϵ�� (4096)
			}
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_b >= 3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_b <= 4506))
			{	
				 m_i16KsVoutbSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_b;	        //B�������ѹУ��ϵ�� (4096)
			}
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_c >= 3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_c <= 4506))
			{	
				 m_i16KsVoutcSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_c;	        //C�������ѹУ��ϵ�� (4096)
			}	
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_a >= 3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_a <= 4506))
			{	
				 m_i16KsIoutaSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_a;	        //A���������У��ϵ�� (4096)
			}
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_b >= 3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_b <= 4506))
			{	
				 m_i16KsIoutbSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_b;	        //B���������У��ϵ�� (4096)
			}
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_c>=3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_c<=4506))
			{	
				 m_i16KsIoutcSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_c;	        //C���������У��ϵ�� (4096)
			}  
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_a >= 3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_a <= 4506))
			{	
				 m_i16KsVinvaSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_a;	        //A������ѹУ��ϵ�� (4096)
			}
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_b>=3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_b<=4506))
			{	
				 m_i16KsVinvbSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_b;	        //B������ѹУ��ϵ�� (4096)
			}
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_c >= 3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_c <= 4506))
			{	
				 m_i16KsVinvcSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_c;	        //C������ѹУ��ϵ�� (4096)
			}  
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_a >= 3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_a <= 4506))
			{	
				 m_i16KsIinvaSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_a;	        //A��������У��ϵ�� (4096)
			}
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_b >= 3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_b <= 4506))
			{	
				 m_i16KsIinvbSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_b;	        //B��������У��ϵ�� (4096)
			}
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_c >= 3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_c <= 4506))
			{	
				 m_i16KsIinvcSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_c;	        //C��������У��ϵ�� (4096)
			} 
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_a >= 3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_a <= 4506))
			{	
				 m_i16KsVbpaSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_a;	        //A����·��ѹУ��ϵ�� (4096)
			}
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_b >= 3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_b <= 4506))
			{	
				 m_i16KsVbpbSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_b;	        //B��v��ѹУ��ϵ�� (4096)
			}
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_c >= 3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_c <= 4506))
			{	
				 m_i16KsVbpcSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_c;	        //C����·��ѹУ��ϵ�� (4096)
			} 

/***********************************�����Ա�����ַ*************************************/		
		if(m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh == 0x8000)
		{
			if ( ( m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16DebugVar1Addr <= 4095  ) 
				|| ( ( m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16DebugVar1Addr >= 36864 ) 
				&&( m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16DebugVar1Addr <= 49151 ) ) )
			{
				pDSPVar1Adr = (UINT16 *)(  m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16DebugVar1Addr  );
			}
			if ( ( m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16DebugVar2Addr <= 4095  ) 
				|| ( ( m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16DebugVar2Addr >= 36864 ) 
				&& ( m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16DebugVar2Addr <= 49151 ) ) )
			{
				pDSPVar2Adr = (UINT16 *)(  m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16DebugVar2Addr  );
			}
		
		
			if ( ( m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16DebugVar3Addr <= 4095  ) 
				|| ( ( m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16DebugVar3Addr >= 36864 ) 
				&& ( m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16DebugVar3Addr <= 49151 ) ) )
			{
				pDSPVar3Adr =(UINT16 *)(  m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16DebugVar3Addr  );
			}
			m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh &= 0x7FFF;
		}
	}


			if(m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh2!=0)
	{

		//2017.06.10 �����Բ���
		//		if((m_st_RecSet_UD.SetParam.u16DegVar1	 >= 3686)//2017.06.10����Ҫ���÷�Χ
		//				&& (m_st_RecSet_UD.SetParam.u16DegVar1	<= 4506))
				{	
					 m_i16_DegVar1 = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16DegVar1;			
				}		
		//		if((m_st_RecSet_UD.SetParam.u16DegVar2	 >= 3686)
		//				&& (m_st_RecSet_UD.SetParam.u16DegVar2	<= 4506))
				{	
					 m_i16_DegVar2 = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16DegVar2;			
				}		
		//		if((m_st_RecSet_UD.SetParam.u16DegVar3	 >= 3686)
		//				&& (m_st_RecSet_UD.SetParam.u16DegVar3	<= 4506))
				{	
					 m_i16_DegVar3 = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16DegVar3;			
				}
		
				m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh2 = 0;//2017.06.10
	}
/**********************************����ϵͳ����***************************************/
         if(m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16FaultClrCmd == 1)
         {
         	m_st_wMonitorOrder.bReset =1;						//��������
         	m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16FaultClrCmd = 0;
         }
         if(m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16ManualInvOnCmd == 1)
         {
         	m_st_wMonitorOrder.bOn =1;							//����
         	m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16ManualInvOnCmd = 0;
         }
         if(m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16ManulaInvOffCmd == 1)
         {
         	m_st_wMonitorOrder.bOff =1;							//�ػ�(�ֶ��ػ�)
         	m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16ManulaInvOffCmd = 0;
         }
         if(m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16EpoCmd == 1)
         {
         	m_st_wMonitorOrder.bEPO =1;							//�����ػ�(����·)
         	m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16EpoCmd = 0;
         }
         if(m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16EodCmd == 1)
         {
         	m_st_wMonitorOrder.bInstantOff =1;					//�ػ�(���ģ����������)
         	m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16EodCmd = 0;
         }
         if(m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16OutputDisableCmd ==1)
         {
         	m_st_wMonitorOrder.bOutDisable =1;					//�����ֹ(ֱ�ӹ����)
         	m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16OutputDisableCmd = 0;				
         }
         else if(m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16OutputEnableCmd == 1 )
         {
         	m_st_wMonitorOrder.bOutEnable =1;					//�������(ֱ�ӷſ����)
         	m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16OutputEnableCmd = 0;
         }         
         if(m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16AbnrmlShutConfirmCmd == 1)
         {
         	m_st_wMonitorOrder.bRiskOffConf =1;					//ȷ���쳣�ػ�
         	m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16AbnrmlShutConfirmCmd = 0;
         }
         else if(m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16AbnrmllShutCancelCmd == 1)
         {
         	m_st_wMonitorOrder.bRiskOffConf =2;					//ȡ���쳣�ػ�
         	m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16AbnrmllShutCancelCmd = 0;
         }
         if(m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16IndirectSwitchCmd == 1)
         {
         	m_st_wMonitorOrder.bIntSwitchConf =1;				//����л�ȷ����Ϣ��Ӧ
         	m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16IndirectSwitchCmd =0;
         }
         if(m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16InvFwUpdateCmd == 1)
         {
         	m_st_wMonitorOrder.bFlashUpdate =1;					//START FLASH UPDATE
         	if(objDigitalIO.m_un_ExInARegConf.BIT.bEPO == 0)  //   EPO ���̼�����
		{				
			// ���жϣ���ֹ��
			DINT;
			objWatchDogDrv.Drv_DisableDog();

			//����API������RAM
			objRAMDrv.Drv_MemCopy(&Flash28_API_LoadStart, &Flash28_API_LoadEnd, &Flash28_API_RunStart);

			objFlashUpdate.FlagProgram();
						
			m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16InvFwUpdateCmd =0;	// ����������
			
			if(objSystem.m_u16DSPRevision >= 3)			//�汾��ΪC�����ϵ�DSP��ι��
			{
				objWatchDogDrv.Drv_EnableDog();
			}
				
			while(1)
			{
				asm(" NOP ");						//��ѭ��,��ι����λDSP
			}
			
		}         	
		else
		{
			m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16InvFwUpdateCmd =0;	// ����������
		}
         }     

	//---���ӵ���������
	if (1 == m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16SelfChkDeepCmd)
	{
		m_u16BatDeepSelfRecved = 1;

		//������
		m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16SelfChkDeepCmd = 0;
	}

	//ά���Լ�����
	 if (1 == m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16SelfChk20PercentCmd)
	{
		m_u16BatDeepSelfRecved = 0;

		//������
		m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16SelfChk20PercentCmd = 0;
	}

	 //�Լ����
	  if (1 == m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16SelfChkEndCmd)
	{
		m_u16BatDeepSelfRecved = 0;

		//������
		m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16SelfChkEndCmd = 0;
	}
	 
	
	//----------------------------
	

		//20K��Ŀ������ wenbo 090610
		//��·�����ź�

//		UCHAR i;
/*
			for(i=0;i<ParaNumCNST;i++)
			{
				if(objMonInterface.m_st_DiscreteState[i].Word1_U.tWord1Bits.bInBrk ==1)
				{
					objSystem.m_st_wFlagStatus.bMainFeedBack = 1;				
				}

				if(objMonInterface.m_st_DiscreteState[i].Word1_U.tWord1Bits.bChargerWorkP ==1)
				{
					objPowCANSched.m_st_BatteryStatus.bChargerWorkP = 1;
				}

				if(objMonInterface.m_st_DiscreteState[i].Word1_U.tWord1Bits.bChargerWorkN ==1)
				{
					objPowCANSched.m_st_BatteryStatus.bChargerWorkN = 1;				
				}
				else
				{
					objPowCANSched.m_st_BatteryStatus.bChargerWorkN = 0;	
				}

				if(objMonInterface.m_st_DiscreteState[i].Word1_U.tWord1Bits.bBoostState == 1)
				{
					objPowCANSched.m_st_BatteryStatus.bBattDischg = 1;
				}
				else
				{
					objPowCANSched.m_st_BatteryStatus.bBattDischg = 0;
				}
			}
*/
			UINT16 i16TempID;
			i16TempID = objDigitalIO.m_st_wHardwareNewSignal.bInvID - 1;

			objSystem.m_st_wFlagStatus.bMainFeedBack 
				= objMonInterface.m_st_DiscreteState[i16TempID].Word1_U.tWord1Bits.bInBrk;

			objPowCANSched.m_st_BatteryStatus.bChargerWorkP 
				= objMonInterface.m_st_DiscreteState[i16TempID].Word1_U.tWord1Bits.bChargerWorkP;

			objPowCANSched.m_st_BatteryStatus.bChargerWorkN 
				= objMonInterface.m_st_DiscreteState[i16TempID].Word1_U.tWord1Bits.bChargerWorkN;

			if(objMonInterface.m_st_DiscreteState[i16TempID].Word1_U.tWord1Bits.bBoostState == 1)
			{
				objPowCANSched.m_st_BatteryStatus.bBattDischg = 1;
				objPowCANSched.m_st_BatteryStatus.bChgBostP = 0;
			}
			else
			{
				objPowCANSched.m_st_BatteryStatus.bBattDischg = 0;

				if(objMonInterface.m_st_DiscreteState[i16TempID].Word1_U.tWord1Bits.bBoostState == 2)
				{
					objPowCANSched.m_st_BatteryStatus.bChgBostP = m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16PostBattEqualChgCmd;
				}
				else
				{
					objPowCANSched.m_st_BatteryStatus.bChgBostP = 0;
				}
				
			}
	}
		
		
/************************************************************************************
��������	App_MonInterfaceWrite()
����������	����״̬���澯��ģ���������
��������	��
************************************************************************************/
	void	Class_MonitorInterface::Byp_App_MonInterfaceWrite()
	{

/**************************************��·�ϴ�״̬����**************************************************/
			//BCB����״̬
			m_st_BypSendMonitor.m_st_BypBreakerStatus.Word1_U.tWord1Bits.bBCBConnect = 0;
			//����·ģ��,������·����״̬����ʾ
			m_st_BypSendMonitor.m_st_BypBreakerStatus.Word1_U.tWord1Bits.bBpSupplyState =2;
			
			//EPO
			m_st_BypSendMonitor.m_st_BypBreakerStatus.Word1_U.tWord1Bits.bEpoPrompt = objSystem.m_st_wFlagStatus.bEPO;

			//ά�޿տ�״̬
			m_st_BypSendMonitor.m_st_BypBreakerStatus.Word1_U.tWord1Bits.bBypMaintBreak = objSwitches.m_st_FlagBreaker.bQ3bp;

            //
            m_st_BypSendMonitor.m_st_BypBreakerStatus.Word1_U.tWord1Bits.bBypOnlinestate = objMonInterface.m_i16_DegVar1;
			//���ϼ�¼�ϴ�����
//			m_st_BypSendMonitor.m_st_BypBreakerStatus.Word1_U.tWord1Bits.bRecordTxRequest = objSystem.m_st_wFlagSystem.bRecordTxRequest;
/****************************��·�ϴ��澯��************************************************/
			//��·����
			m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bBypOverCurr = objSystem.m_st_wFlagFault.bBpOverCurrent | objBypass.m_st_FlagIbpTotal.bIbpOverTimeOut; 
			//��·����
			if(objSystem.m_st_wFlagFault.bBypFeedBackFault == 1)
			{
				m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bBypFeedBackFault  =1;
			}
			else if((objSwitches.m_st_FlagBreaker.bBpSTSOpen ==1)&&(objDigitalIO.m_un_ExInAReg.BIT.bBPSCRFromBp == 1))
			{
				m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bBypFeedBackFault  =2;
			}
			else
			{
				m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bBypFeedBackFault = 0;
			}						

			//�ڻ���·��բ�ܹ���
			if((objSwitches.m_st_FlagBreaker.bBpSTSShort == 1) && (objSwitches.m_st_FlagBreaker.bReqBpSTSShort == 1))
			{
				m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bReqBypScrFault = 1;
			}
			
			//���ȹ���
			m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bFanFault =0;
			//��ؽӵع���
			m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bBatEarthFault =0;
			//��ط������쳣
			m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bBatRoomEnvirAbnormal =0;
			//��·ģ�����
			m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bModOvTemp = 0;
			//������Դ1����
			m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bAuxPwr1Down = 0 ;
			//������Դ2����
			m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bAuxPwr2Down = 0 ;
			//�������
			//m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bOvload =objSystem.m_st_wFlagFault.bOvload|objSystem.m_st_wFlagPrompt.bParaOvload;
			
			//m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bOvload =objSystem.m_st_wFlagPrompt.bParaOvload;
			
			//Power ok����
//			m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bPowOK = objSystem.m_st_wFlagPrompt.bPow15vFault;
			/*��·���(��ѹ��Ƶ��)0:normal 1:������2:������*/
			m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bBypState = objSystem.m_st_wFlagStatus.bVbp;
			/*��·����*/ 
			m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bBypPhaseReverse = objSystem.m_st_wFlagFault.bBpInvtSeq;

			/*����ͨ�Ź���*/
			m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bParallelCommErr = objPowCANSched.PowCanParaInfoEntry.UpsOnlineInfo.uOnlineFlag.tBits.bSysCommerr;

/*****************************��·�ϴ�ģ����**********************************************/  
			/*��·���ѹ*/	
			m_st_BypSendMonitor.m_st_BypAnalog.BypPhaseVlt.u16Phase_a =	(UINT16)((((INT32)( objBypass.m_unVbpaRms.half.hword )) * m_i16VoltDispScale)>>10);			
			m_st_BypSendMonitor.m_st_BypAnalog.BypPhaseVlt.u16Phase_b =	(UINT16)((((INT32)( objBypass.m_unVbpbRms.half.hword )) * m_i16VoltDispScale)>>10);			
			m_st_BypSendMonitor.m_st_BypAnalog.BypPhaseVlt.u16Phase_c =	(UINT16)((((INT32)( objBypass.m_unVbpcRms.half.hword )) * m_i16VoltDispScale)>>10);			
			/*��·�����*/
			m_st_BypSendMonitor.m_st_BypAnalog.BypPhaseCurr.u16Phase_a = (UINT16)( objOutput.m_i16IoutaforShow );
			m_st_BypSendMonitor.m_st_BypAnalog.BypPhaseCurr.u16Phase_b = (UINT16)( objOutput.m_i16IoutbforShow );
			m_st_BypSendMonitor.m_st_BypAnalog.BypPhaseCurr.u16Phase_c = (UINT16)( objOutput.m_i16IoutcforShow );
			//������ѹ
			m_st_BypSendMonitor.m_st_BypAnalog.BypOutPhaseVlt.u16Phase_a = (UINT16)((((INT32)( objOutput.m_unVoutaRms.half.hword )) * m_i16VoltDispScale)>>10);
			m_st_BypSendMonitor.m_st_BypAnalog.BypOutPhaseVlt.u16Phase_b = (UINT16)((((INT32)( objOutput.m_unVoutbRms.half.hword )) * m_i16VoltDispScale)>>10);
			m_st_BypSendMonitor.m_st_BypAnalog.BypOutPhaseVlt.u16Phase_c = (UINT16)((((INT32)( objOutput.m_unVoutcRms.half.hword )) * m_i16VoltDispScale)>>10);
			//�����������
			if ( objOutput.m_i16Souta < Sout5CNST )					//���ڹ���<5%,��������=0
			{			
				m_st_BypSendMonitor.m_st_BypAnalog.OutFactor.u16Phase_a = 0;
			}
			else if(objSystem.m_st_wFlagSystem.bSelfAgingAction == 1)//���ϻ�ʱ��������Ϊ1
			{
				m_st_BypSendMonitor.m_st_BypAnalog.OutFactor.u16Phase_a =100;
			}
			else
			{
				m_st_BypSendMonitor.m_st_BypAnalog.OutFactor.u16Phase_a = (UINT16)( objOutput.m_i16PFouta );

			}
			if ( objOutput.m_i16Soutb < Sout5CNST )					//���ڹ���<5%,��������=0
			{			
				m_st_BypSendMonitor.m_st_BypAnalog.OutFactor.u16Phase_b = 0;
			}
			else if(objSystem.m_st_wFlagSystem.bSelfAgingAction == 1)//���ϻ�ʱ��������Ϊ1
			{
				m_st_BypSendMonitor.m_st_BypAnalog.OutFactor.u16Phase_b =100;
			}
			else
			{
				m_st_BypSendMonitor.m_st_BypAnalog.OutFactor.u16Phase_b = (UINT16)( objOutput.m_i16PFoutb );

			}
			if ( objOutput.m_i16Soutc < Sout5CNST )					//���ڹ���<5%,��������=0
			{			
				m_st_BypSendMonitor.m_st_BypAnalog.OutFactor.u16Phase_c = 0;
			}
			else if(objSystem.m_st_wFlagSystem.bSelfAgingAction == 1)//���ϻ�ʱ��������Ϊ1
			{
				m_st_BypSendMonitor.m_st_BypAnalog.OutFactor.u16Phase_c =100;
			}
			else
			{
				m_st_BypSendMonitor.m_st_BypAnalog.OutFactor.u16Phase_c = (UINT16)( objOutput.m_i16PFoutc );

			}
			//���Ƶ��
			if ( ( objOutput.m_i16VoutaPrms < Voltage80CNST ) || ( objOutput.m_i16VoutbPrms < Voltage80CNST )
				|| ( objOutput.m_i16VoutcPrms < Voltage80CNST ) )
			{
				m_st_BypSendMonitor.m_st_BypAnalog.BypOutFreq.u16Self = 0;
			}
			else
			{
				m_st_BypSendMonitor.m_st_BypAnalog.BypOutFreq.u16Self = (UINT16)(objOutput.m_i16FoutMean);
			}
			/*��·Ƶ��*/ 
			if ( objBypass.m_unVbpMMean_0.half.hword < 790 )
			{                                     
				  m_st_BypSendMonitor.m_st_BypAnalog.BypFreq.u16Self = 0;
			}
			else
			{
				 m_st_BypSendMonitor.m_st_BypAnalog.BypFreq.u16Self = (UINT16)( objBypass.m_i16FbpMean );
			}
			//������ذٷֱ�
			m_st_BypSendMonitor.m_st_BypAnalog.OutLoadRate.u16Phase_a = (UINT16)( objOutput.m_i16LoadRatea );
			m_st_BypSendMonitor.m_st_BypAnalog.OutLoadRate.u16Phase_b = (UINT16)( objOutput.m_i16LoadRateb );
			m_st_BypSendMonitor.m_st_BypAnalog.OutLoadRate.u16Phase_c = (UINT16)( objOutput.m_i16LoadRatec );
			//�����ֵ��
			if ( objOutput.m_i16IoutaPrms < Current5CNST )				//�������<5%,��ֵ��=1.4
			{
				m_st_BypSendMonitor.m_st_BypAnalog.OutPeakRate.u16Phase_a = 0;
			}
			//δ�������ϻ�
			else
			{
				m_st_BypSendMonitor.m_st_BypAnalog.OutPeakRate.u16Phase_a =(UINT16)((((INT32)( objOutput.m_u16IoutaCrest_0 )) * 10)>>8);
			}
			if ( objOutput.m_i16IoutbPrms < Current5CNST )				//�������<5%,��ֵ��=1.4
			{
				m_st_BypSendMonitor.m_st_BypAnalog.OutPeakRate.u16Phase_b = 0;
			}
			//δ�������ϻ�
			else
			{
				m_st_BypSendMonitor.m_st_BypAnalog.OutPeakRate.u16Phase_b =(UINT16)((((INT32)( objOutput.m_u16IoutbCrest_0 )) * 10)>>8);
			}
			if ( objOutput.m_i16IoutcPrms < Current5CNST )				//�������<5%,��ֵ��=1.4
			{
				m_st_BypSendMonitor.m_st_BypAnalog.OutPeakRate.u16Phase_c = 0;
			}
			//δ�������ϻ�
			else
			{
				m_st_BypSendMonitor.m_st_BypAnalog.OutPeakRate.u16Phase_c =(UINT16)((((INT32)( objOutput.m_u16IoutcCrest_0 )) * 10)>>8);
			}

			/*�����¶�*/
			//m_st_BypSendMonitor.m_st_BypAnalog.EnvirTemp.u16Self = 0;//�Ĵ�����ȡ
			//����¶�  
			//m_st_BypSendMonitor.m_st_BypAnalog.BatteryInTemp.u16Self =(UINT16) (objSystem.m_unTmpBatIn.half.hword);  
			//m_st_BypSendMonitor.m_st_BypAnalog.BatteryInTemp.u16BattInTemp =0;
			//m_st_BypSendMonitor.m_st_BypAnalog.BatteryInTemp.u16BattOutTemp =0;
			//����й�����
			m_st_BypSendMonitor.m_st_BypAnalog.OutActPwr.u16Phase_a = (UINT16)(objOutput.m_i16Pouta);
                     m_st_BypSendMonitor.m_st_BypAnalog.OutActPwr.u16Phase_b = (UINT16)(objOutput.m_i16Poutb);
         		m_st_BypSendMonitor.m_st_BypAnalog.OutActPwr.u16Phase_c =  (UINT16)(objOutput.m_i16Poutc); 

			//ljie check
			/*m_st_ParaRackData[m_i16RMIdx].m_i16Pouta= (UINT16)(objOutput.m_i16Pouta);
                     m_st_ParaRackData[m_i16RMIdx].m_i16Poutb = (UINT16)(objOutput.m_i16Poutb);
         		m_st_ParaRackData[m_i16RMIdx].m_i16Poutc =  (UINT16)(objOutput.m_i16Poutc); 
			//������ڹ���
			m_st_ParaRackData[m_i16RMIdx].m_i16Souta= (UINT16)(objOutput.m_i16Souta);
			m_st_ParaRackData[m_i16RMIdx].m_i16Soutb =(UINT16)(objOutput.m_i16Soutb);
			m_st_ParaRackData[m_i16RMIdx].m_i16Soutc=(UINT16)(objOutput.m_i16Soutc);*/

			m_st_BypSendMonitor.m_st_BypAnalog.OutApparentPwr.u16Phase_a = (UINT16)(objOutput.m_i16Souta);
			m_st_BypSendMonitor.m_st_BypAnalog.OutApparentPwr.u16Phase_b =(UINT16)(objOutput.m_i16Soutb);
			m_st_BypSendMonitor.m_st_BypAnalog.OutApparentPwr.u16Phase_c =(UINT16)(objOutput.m_i16Soutc);

			//�����й�����
			m_st_BypSendMonitor.m_st_BypAnalog.RackOutActPwr.u16Phase_a = (UINT16)(objSystem.m_i16PoutaTotal);
			m_st_BypSendMonitor.m_st_BypAnalog.RackOutActPwr.u16Phase_b = (UINT16)(objSystem.m_i16PoutbTotal);
			m_st_BypSendMonitor.m_st_BypAnalog.RackOutActPwr.u16Phase_c = (UINT16)(objSystem.m_i16PoutcTotal);

			//�������ڹ���
			m_st_BypSendMonitor.m_st_BypAnalog.RackOutApparentPwr.u16Phase_a = (UINT16)(objSystem.m_i16SoutaTotal);
			m_st_BypSendMonitor.m_st_BypAnalog.RackOutApparentPwr.u16Phase_b = (UINT16)(objSystem.m_i16SoutbTotal);
			m_st_BypSendMonitor.m_st_BypAnalog.RackOutApparentPwr.u16Phase_c = (UINT16)(objSystem.m_i16SoutcTotal);


			/*��·���Ա���*/
			//m_st_BypSendMonitor.m_st_BypAnalog.BypDebugVari.u16Dbg1 = 0;
			//m_st_BypSendMonitor.m_st_BypAnalog.BypDebugVari.u16Dbg2 = 0;
			//m_st_BypSendMonitor.m_st_BypAnalog.BypDebugVari.u16Dbg3 = 0;   
     }		
//===========================================================================
// End of file.                                    		
//===========================================================================
