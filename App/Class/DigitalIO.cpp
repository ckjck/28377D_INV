/*==========================================================*/ 	
/* Title		:	DigitalIO.cpp													*/ 	
/* Description	: 	Class_DigitalIO definition										*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#include "DigitalIO.h"

/************************************************************************************
��������	App_DigitalIOVarInit()	
����������	DigitalIO�������ʼ��		
�������ã�	
***********************************************************************************/
	 void	Class_DigitalIO::App_DigitalIOVarInit(void)
	{
		m_st_HardwareSignal.bFuse = 1;						
		m_st_HardwareSignal.bFanFault = 0;
		m_un_ExInAReg.BIT.bEPO = 1;		//EPO		
		m_st_wLocalSignal.bLed = 0;				//LocalSignal DS lights
		

    	m_un_ExOutAReg.all = 0;
		m_un_ExOutBReg.all = 0;
		m_un_ExOutCReg.all = 0;
		
    	m_un_ExOutAReg.BIT.bFrameID = 1;
		m_un_ExOutBReg.BIT.bFrameID = 2;
		m_un_ExOutCReg.BIT.bFrameID = 3;

		m_i16BpBackFeedFIoConfCntPer1ms = 0;

		m_i16IMCCtrlConfCnt=0;
	}

/************************************************************************************
��������	App_IMCCtrl()	
����������IMC���ܼ���ɢ�źſ���		
�������ã�	
***********************************************************************************/
	 void	Class_DigitalIO::App_IMCCtrl(void)
	{
		//���ܼ�IMC �������
		if(objSystem.m_st_wFlagSystem.bTimer1ms == 1)
		{
			if ( m_un_ExInAReg.BIT.bPower15VFault == 0  )	//POW_OK = 0
			{
				m_i16IMCCtrlConfCnt++;
				if(m_i16IMCCtrlConfCnt>=5)
				{
					m_i16IMCCtrlConfCnt=5;
					m_st_wLocalNewSignal.bIMC_Enable =0;
				}
			}
			else
			{
				m_i16IMCCtrlConfCnt--;
				if(m_i16IMCCtrlConfCnt<=0)
				{
					m_i16IMCCtrlConfCnt=0;
					m_st_wLocalNewSignal.bIMC_Enable =1;
				}
			}
		}

	}

/************************************************************************************
//�������ƣ�    Dat_Int_MainInterfaceOut()   
//����������    ����ж϶���������Ʊ��������
************************************************************************************/

#pragma CODE_SECTION(".Epwm1Funcs") 			

void 	Class_DigitalIO::Dat_Int_MainInterfaceOut(void)
{	
	INT16	i16IntAx_0;
	objInvAlg.m_i16wVrefActMd_0 = objInvAlg.m_i16wVrefRunMd_0;
	//��������ź���SPI�������֮���ӳ���ϵ
	if(objSystem.m_st_wFlagSystem.bIoTestMode == 1)
	{
		S2U(objParallel.m_st_wParaSignalBak) = S2U(objParallel.m_st_wParaSignal);		//IoTestMode
	}
	else
	{
		//Except bit8: MASTER_ID & bit10: MASTER_COMM
		S2U(objParallel.m_st_wParaSignalBak) = (((S2U(objParallel.m_st_wParaSignal)^S2U(objParallel.m_st_wParaSignalBak))&0x01FF)^S2U(objParallel.m_st_wParaSignalBak));	
	}
		
	//zqf 060414--CR40
	//zxj 060429--ע��objParallel.m_st_wParaSignalBak�ж�����͸���������3��IO�źŲ�������
	i16IntAx_0=S2U(objParallel.m_st_wParaSignalBak);
	if((objParallel.m_st_wParaMaint.bParaSignalMask == 1 )|| (objMonInterface.m_st_wUPSModeRun.bConfig == 0))
	{
		//���ǵ����²�������LBS�źţ�Ϊ�˼򻯴��룬�źŸ���̬��Ҳ����ͬ����
		//(�źŸ���̬�£��Ѿ����������н�LBS�ź������δ������߼��ϲ�������)
		//bJcontrol�ź������������
		//By wxw20091026	������system_unlockλ�������㣬������·�������£������ȴ������·
		//���±���ʱ�����Ƶ��Ϊ52Hz��48Hz
		//By wxw20091209	������bInvOnStandbyλ�������㣬��������ػ���ʱ�����߼���л���
		//i16IntAx_0 = i16IntAx_0 & 0xF810;
		//By wxw20100302 ��������ſ�LBS�ź�
		//i16IntAx_0 = i16IntAx_0 & 0xF816;
		i16IntAx_0 = i16IntAx_0 & 0xF8D6;
	}
	if(objSystem.m_st_wFlagSystem.bUPSTestMode == 1)		
	{
		//����ģʽ��ֻ��bInvOverloadλ���͸���������3��IO�źţ�bJcontrol�ź������������
		i16IntAx_0 = i16IntAx_0 & 0xC811;
	}
		
	if(objSystem.m_st_wFlagSystem.bWDStart==1)
	{
		S2U(m_st_wLocalSignal) = S2U(m_st_wLocalSignal)^0x0080;
	}

	//��·�����ѿ�����
	/*
	m_un_ExOutAReg.BIT.bPWMEN = m_st_wLocalSignal.bInvOffToCPLD;
	m_un_ExOutAReg.BIT.bPWMENbak = m_st_wLocalSignal.bInvOffToCPLD;
    m_un_ExOutCReg.BIT.bINV_OVERLOAD = objParallel.m_st_wParaSignal.bInvOverload;
    m_un_ExOutCReg.BIT.bSYSTEM_UNLOCK = objParallel.m_st_wParaSignal.bSystemUnlock;
    m_un_ExOutCReg.BIT.bINV_ON_STANDBY = objParallel.m_st_wParaSignal.bInvOnStandby;
    m_un_ExOutCReg.BIT.bINV_BP_STATUS = objParallel.m_st_wParaSignal.bInvBpStatus;
    m_un_ExOutCReg.BIT.bMASTER_ONLINE = objParallel.m_st_wParaSignal.bMasterOnline;
    m_un_ExOutCReg.BIT.bModelOnline = m_st_wLocalSignal.bModelOnline;
    m_un_ExOutCReg.BIT.bBPSCR = m_st_wLocalSignal.bBpSTS;
	m_un_ExOutBReg.BIT.bIMC_Enable = m_st_wLocalNewSignal.bIMC_Enable;
    m_un_ExOutBReg.BIT.bMACID = 1 << (m_st_wHardwareNewSignal.bInvID-1);


	if(objInverter.m_st_wFlagInv.bInv == 0)
	{
	m_un_ExOutAReg.BIT.bInvRunStatus = 0;
	m_un_ExOutAReg.BIT.bInvOn = 0;
	}
	else if(objInverter.m_st_wFlagInv.bInv == 1)
	{
	m_un_ExOutAReg.BIT.bInvOn = 1;
	m_un_ExOutAReg.BIT.bInvRunStatus = 1;
	}
	else if(objInverter.m_st_wFlagInv.bInv == 3)
	{
	m_un_ExOutAReg.BIT.bInvRunStatus = 3;
	m_un_ExOutAReg.BIT.bInvOn = 1;
	}
	else
	{
	m_un_ExOutAReg.BIT.bInvRunStatus = 0;
	m_un_ExOutAReg.BIT.bInvOn = 0;
	}
	*/

   
}

/************************************************************************************
//�������ƣ�    Dat_Int_MainInterfaceIn()   
//����������    ����ж϶���������Ʊ���������
************************************************************************************/

#pragma CODE_SECTION(".Epwm1Funcs") 			

void 	Class_DigitalIO::Dat_Int_MainInterfaceIn(void)
{

	//zqf 060414--CR40
	//�źŸ���̬������������ģʽ���β����źţ�(�źŸ���̬������ģʽ���������߼������о���
	//���յ���LBS�źŽ������Σ��ڴ�Ϊ�򻯴����뵥������һ�£���������)
	if((objParallel.m_st_wParaMaint.bParaSignalMask == 1) || ( objSystem.m_st_wFlagSystem.bUPSTestMode == 1))
	{
		//BIT0~10��ֵ��objParallel.m_st_wParaSignal��ͬ,BIT11~15 bExtOutStatusF��bCabQ3BpStatusF��bLBSDetect��bJControl,bConnectOKF���ֲ���
		S2U(objParallel.m_st_wParaSignalF) = (((S2U(objParallel.m_st_wParaSignal)&0x07FF)^S2U(objParallel.m_st_wParaSignalF))&0x07FF)^S2U(objParallel.m_st_wParaSignalF);

		objParallel.m_st_wParaSignalF.bLeader_Rack = 1;
	}
	objParallel.m_st_wParaSignalF.bLeader_Rack = 1;
				
}

/************************************************************************************
��������	Dat_DigitalInput()
����������	EXTIO������ɼ�ģ�飬�ɼ�EXTIO�ڵ����뿪����������ȷ�ϴ���
��ڱ���:   m_st_wSPIDataIn2_0		Ӳ������ԭʼ�ź�
���ڱ���:	m_st_HardwareSignal			ȷ�Ϻ���ź�
�������ã�	��		
************************************************************************************/

	void	Class_DigitalIO::Dat_DigitalInput()
	{

		objParallel.m_st_wParaSignalF.bCabQ3BpStatusF = m_st_wLocalNewSignal.bCAB_Q3BP_STATUS;
		objParallel.m_st_wParaSignalF.bExtOutStatusF = m_st_wLocalNewSignal.bEXT_OUT_STATUS;
		
		objParallel.m_st_wParaSignalF.bSystemUnlockF = m_un_ExInDReg.BIT.bSystem_Unlock_F;  //���ͬ�������ź�(��ɢ�����ź�����)��0��ʾ��俪�����
		objParallel.m_st_wParaSignalF.bInvOnStandbyF = m_un_ExInDReg.BIT.bInv_ON_Standby_F;
        objParallel.m_st_wParaSignalF.bInvBpStatusF = m_un_ExInDReg.BIT.bInv_BP_Status_F;
        objParallel.m_st_wParaSignalF.bMasterOnlineF = m_un_ExInDReg.BIT.bMaster_Online_F;

	 	

		//IO�ź�ȷ�Ϸ�ʽ����Ϊ��ÿ��Ӳ���źŶ���ȷ��

		
		//m_i16IoConfCntPer1ms[1] Q2ȷ�ϼ�����
		//m_i16IoConfCntPer1ms[2] Q3BPȷ�ϼ�����
		//m_i16IoConfCntPer1ms[3] Q5ȷ�ϼ�����
		//m_i16IoConfCntPer1ms[4] Fuseȷ�ϼ�����
		//m_i16IoConfCntPer1ms[5] DCOVPȷ�ϼ�����
		//m_i16IoConfCntPer1ms[6] FanFaultȷ�ϼ�����
		//m_i16IoConfCntPer1ms[7] RecOffȷ�ϼ�����
		//m_i16IoConfCntPer1ms[8] EPOȷ�ϼ�����
		//m_i16IoConfCntPer1ms[9] Inv OVERLOAD signalȷ�ϼ�����
		//m_i16IoConfCntPer1ms[10] InvIDȷ�ϼ�����
		//m_i16IoConfCntPer1ms[11] ModuleReadyȷ�ϼ�����
		//m_i16IoConfCntPer1ms[12] BPCTȷ�ϼ�����
		
		//m_i16IoConfCntPer1ms[14] bBpOnlineȷ�ϼ�����


		//m_i16IoConfCntPer1ms[17] ����������������ȷ��
		//m_i16IoConfCntPer1ms[18] ����������������ȷ��



		//m_i16IoConfCntPer1ms[21] IGBT����ȷ��
		if(objSystem.m_st_wFlagSystem.bTimer1ms == 1)
		{

		    
			
			
			//---------Q3BPȷ�ϣ�����4ms���Ͽ�2ms��--------
/*			if(m_un_ExInAReg.BIT.bQ3Bp== 1)
			{
				m_i16IoConfCntPer1ms[2] += 3;
				if(m_i16IoConfCntPer1ms[2] >= K10msBased1msCnst)
				{
					m_i16IoConfCntPer1ms[2] = K10msBased1msCnst;
					m_st_HardwareSignal.bQ3BP = 1;						
				}
			}
			else
			{
				m_i16IoConfCntPer1ms[2] -= 5;
				if(m_i16IoConfCntPer1ms[2] <= 0)
				{
					m_i16IoConfCntPer1ms[2] = 0;
					m_st_HardwareSignal.bQ3BP = 0;	
				}
			}
*/			
			//-------Q5ȷ�ϣ�����4ms���Ͽ�10ms��--------
			if(m_un_ExInAReg.BIT.bQ5)
			{
				m_i16IoConfCntPer1ms[3] += 3;					
				
				if(m_i16IoConfCntPer1ms[3] >= K10msBased1msCnst)
				{
					m_i16IoConfCntPer1ms[3] = K10msBased1msCnst;
					m_st_HardwareSignal.bQ5 = 1;						
				}
			}
			else
			{
				m_i16IoConfCntPer1ms[3]--;
				if(m_i16IoConfCntPer1ms[3] <= 0)
				{
					m_i16IoConfCntPer1ms[3] = 0;
					m_st_HardwareSignal.bQ5 = 0;	
				}
			}
			m_st_HardwareSignal.bQ5 = 1;        // ģ�鵥��������
			//------------bFuseȷ��3ms---------
			if(m_un_ExInAReg.BIT.bInvFuseFault ==  1)
			{
				m_i16IoConfCntPer1ms[4]++;
				if(m_i16IoConfCntPer1ms[4] >= K3msBased1msCnst)
				{
					m_i16IoConfCntPer1ms[4] = K3msBased1msCnst;
					m_st_HardwareSignal.bFuse = 0;//debugonly						
				}
			}
			else
			{
				m_i16IoConfCntPer1ms[4]--;
				if(m_i16IoConfCntPer1ms[4] <= 0)
				{
					m_i16IoConfCntPer1ms[4] = 0;
					m_st_HardwareSignal.bFuse = 1;	
				}
			}													
			//bTestModeȷ�ϣ�������ֱ������m_st_SPIDataIn2_0��
			//bServiceModeȷ�ϣ�������ֱ������m_st_SPIDataIn2_0��
			//------------bDCOVPȷ��3ms---------
			if(m_un_ExInAReg.BIT.bVBusOVP == 0)
			{
				m_i16IoConfCntPer1ms[5]++;
				if(m_i16IoConfCntPer1ms[5] >= K3msBased1msCnst)
				{
					m_i16IoConfCntPer1ms[5] = K3msBased1msCnst;
					m_st_HardwareSignal.bDcOvp = 1;						
				}
			}
			else
			{
				m_i16IoConfCntPer1ms[5]--;
				if(m_i16IoConfCntPer1ms[5] <= 0)
				{
					m_i16IoConfCntPer1ms[5] = 0;
					m_st_HardwareSignal.bDcOvp = 0;	
				}
			}
			//------------bFanFaultȷ��2ms---------
			if(0)
			{
				m_i16IoConfCntPer1ms[6]++;
				if(m_i16IoConfCntPer1ms[6] >= K2msBased1msCnst)
				{
					m_i16IoConfCntPer1ms[6] = K2msBased1msCnst;
					m_st_HardwareSignal.bFanFault = 1;						
				}
			}
			else
			{
				m_i16IoConfCntPer1ms[6]--;
				if(m_i16IoConfCntPer1ms[6] <= 0)
				{
					m_i16IoConfCntPer1ms[6] = 0;
					m_st_HardwareSignal.bFanFault = 0;	
				}
			}
												
			//------------bRecOffȷ��3ms---------
			if(m_un_ExInAReg.BIT.bRecOff ==  1)
			{
				m_i16IoConfCntPer1ms[7]++;
				if(m_i16IoConfCntPer1ms[7] >= K3msBased1msCnst)
				{
					m_i16IoConfCntPer1ms[7] = K3msBased1msCnst;
					m_st_HardwareSignal.bRecOff = 1;					// 1 off
				}
			}
			else
			{
				m_i16IoConfCntPer1ms[7]--;
				if(m_i16IoConfCntPer1ms[7] <= 0)
				{
					m_i16IoConfCntPer1ms[7] = 0;
					m_st_HardwareSignal.bRecOff = 0;	
				}
			}																								
			//------------bEPOȷ��---------
			if(m_un_ExInAReg.BIT.bEPO == 0)
			{
				m_i16IoConfCntPer1ms[8]++;						//��Ч-->��Ч��10ms
				if(m_i16IoConfCntPer1ms[8] >= K10msBased1msCnst)
				{
					m_i16IoConfCntPer1ms[8] = K10msBased1msCnst;
					m_un_ExInARegConf.BIT.bEPO = 0;			//�͵�ƽ��Ч				
				}
			}
			else
			{
				m_i16IoConfCntPer1ms[8]--;						//��Ч-->��Ч��10ms
				if(m_i16IoConfCntPer1ms[8] <= 0)
				{
					m_i16IoConfCntPer1ms[8] = 0;
					m_un_ExInARegConf.BIT.bEPO = 1;	
				}
			}
			
			if ( m_un_ExInARegConf.BIT.bEPO == 0  )								
			{
				objInvCtrl.m_st_wFlagInvOnoff.bEPO = 1;						//�ý����ػ�״̬
				objSystem.m_st_wFlagStatus.bEPO = 1;						//�ý����ػ�״̬
			}
			
            //------------Inv OVERLOAD signalȷ��---------
			if(m_un_ExInDReg.BIT.bInv_OverLoad_F == 1)
			{
				m_i16IoConfCntPer1ms[9]++;						//��Ч-->��Ч��10ms
				if(m_i16IoConfCntPer1ms[9] >= K10msBased1msCnst)
				{
					m_i16IoConfCntPer1ms[9] = K10msBased1msCnst;
					objParallel.m_st_wParaSignalF.bInvOverloadF = 1;			// 1Ϊ������				
				}
			}
			else
			{
				m_i16IoConfCntPer1ms[9]--;						//��Ч-->��Ч��10ms
				if(m_i16IoConfCntPer1ms[9] <= 0)
				{
					m_i16IoConfCntPer1ms[9] = 0;
					objParallel.m_st_wParaSignalF.bInvOverloadF = 0;	
				}
			}

//---------------------------bInvIDȷ��---------------------------------����ͨ����λ�����㣬ֻ����λ�����ȼ�С�ڣ�
if(m_i16InvIDOkFlag == 0)				//ģ��ID֮�ϵ��һ��
{	
	m_u16InvID = ((1 - GpioDataRegs.GPADAT.bit.GPIO6) + ((1- GpioDataRegs.GPADAT.bit.GPIO7)<<1) + ((1- GpioDataRegs.GPADAT.bit.GPIO8 )<<2)
				  + ((1- GpioDataRegs.GPADAT.bit.GPIO9)<<3));// + ((1- GpioDataRegs.GPADAT.bit.GPIO10 )<<4));
	if(m_u16InvIDOld == m_u16InvID)
	{
		m_i16IoConfCntPer1ms[10]++;
		if(m_i16IoConfCntPer1ms[10] >= K10msBased1msCnst)
		{
				m_i16IoConfCntPer1ms[10] = K10msBased1msCnst;
				m_i16InvIDOkFlag = 1;
				m_st_wHardwareNewSignal.bInvID = m_u16InvID;
				objMonInterface.m_i16wSerialNumSet_0 = m_st_wHardwareNewSignal.bInvID-1+0x10;
				if((m_u16InvID <1 ) || (m_u16InvID > 10))
				{
					objMonInterface.m_st_IdErrorFlag.bUsIdNumExpire = 1;	//ģ��ID������Χ
				}
				else
				{
					objMonInterface.m_st_IdErrorFlag.bUsIdNumExpire = 0;	//ģ��IDδ������Χ
				}
       	}
	}
	else
	{
		m_u16InvIDOld =  m_u16InvID;
		m_i16IoConfCntPer1ms[10] = 0;
	}
}
//--------------------------bModuleReadyȷ��----------------------------
			if(m_un_ExInAReg.BIT.bModuleReady == 1)		//m_i16IoConfCntPer1ms[12] BPCTȷ�ϼ�����
			{
				m_i16IoConfCntPer1ms[11]++;
				if(m_i16IoConfCntPer1ms[11] >= K3msBased1msCnst)
				{
					m_i16IoConfCntPer1ms[11] = K3msBased1msCnst;
					m_st_wHardwareNewSignal.bModuleReady = 1;
				}
			}
			else
			{
				m_i16IoConfCntPer1ms[11]--;
				if(m_i16IoConfCntPer1ms[11] <= 0)
				{
					m_i16IoConfCntPer1ms[11] = 0;
					m_st_wHardwareNewSignal.bModuleReady = 0;		//0 not Ready
				}
			}
			
//-----------------------bBPCTȷ��------------------------------------------
/*			if(m_st_wSPIDataIn2_0.bRevBP_BACKFEED_DETECT == 0)		//m_i16IoConfCntPer1ms[12] BPCTȷ�ϼ�����
			{
				m_i16IoConfCntPer1ms[12]++;
				if(m_i16IoConfCntPer1ms[12] >= K3msBased1msCnst)
				{
					m_i16IoConfCntPer1ms[12] = K3msBased1msCnst;
					m_st_wHardwareNewSignal.bBPBackFeedDetect = 1;
				}
			}
			else
			{
				m_i16IoConfCntPer1ms[12]--;
				if(m_i16IoConfCntPer1ms[12] <= 0)
				{
					m_i16IoConfCntPer1ms[12] = 0;
					m_st_wHardwareNewSignal.bBPBackFeedDetect = 0;
				}
			}*/
//------------------------��·����ȷ��----------------------------------
			if(m_un_ExInAReg.BIT.bBpFBDetect == 0)		
			{
				m_i16BpBackFeedFIoConfCntPer1ms++;
				if(m_i16BpBackFeedFIoConfCntPer1ms >= K10msBased1msCnst)
				{
					m_i16BpBackFeedFIoConfCntPer1ms = K10msBased1msCnst;
					m_st_wHardwareNewSignal.bBPBackFeedDetect_F = 0;
				}
			}
			else
			{
				m_i16BpBackFeedFIoConfCntPer1ms--;
				if(m_i16BpBackFeedFIoConfCntPer1ms <= 0)
				{
					m_i16BpBackFeedFIoConfCntPer1ms = 0;
					m_st_wHardwareNewSignal.bBPBackFeedDetect_F = 1;
				}
			}
//-------------------------------bRECFaultȷ��----------------------------------
/*
			if(m_st_wSPIDataIn4_0.bRECFault == 1)		//m_i16IoConfCntPer1ms[12] BPCTȷ�ϼ�����
			{
				m_i16IoConfCntPer1ms[13]++;
				if(m_i16IoConfCntPer1ms[13] >= K10msBased1msCnst)
				{
					m_i16IoConfCntPer1ms[13] = K10msBased1msCnst;
					m_st_wLocalNewSignal.bRECFault = 1;
				}
			}
			 else if(m_st_wSPIDataIn4_0.bRECFault == 2)
			{
				m_i16IoConfCntPer1ms[15]++;
				if(m_i16IoConfCntPer1ms[15] >= K2msBased1msCnst)
				{
					m_i16IoConfCntPer1ms[15] = K2msBased1msCnst;
					m_st_wLocalNewSignal.bRECFault = 2;
				}
			}
			 else
			 {
				m_i16IoConfCntPer1ms[13] -=5;
				m_i16IoConfCntPer1ms[15]--;
				if((m_i16IoConfCntPer1ms[13] <= 0)&&(m_i16IoConfCntPer1ms[15] <= 0))
				{
					m_i16IoConfCntPer1ms[13] = 0;
					m_i16IoConfCntPer1ms[15] = 0;
					m_st_wLocalNewSignal.bRECFault = 0;
				}

			 }*/

//--------------------------binvAutoOnȷ��----------------------------
			if(m_un_ExInAReg.BIT.bInvAutoOnEn == 1)		//m_i16IoConfCntPer1ms[16] ����Կ���ȷ�ϼ�����
			{
				m_i16IoConfCntPer1ms[16]++;
				if(m_i16IoConfCntPer1ms[16] >= K10msBased1msCnst)
				{
					m_i16IoConfCntPer1ms[16] = K10msBased1msCnst;
					m_st_HardwareSignal.bInvAutoOnEn = 1;       //��������Կ���������ϵͳ��Ҫ����������
				}
			}
			else
			{
				m_i16IoConfCntPer1ms[16]--;
				if(m_i16IoConfCntPer1ms[16] <= 0)
				{
					m_i16IoConfCntPer1ms[16] = 0;
					m_st_HardwareSignal.bInvAutoOnEn = 0;		
				}
			}



#if(0)
//----------------------����������������ȷ��--------------------
if(m_un_ExInDReg.BIT.bMaster_Online_F == 1)
{
    m_i16IoConfCntPer1ms[17]++;
    if(m_i16IoConfCntPer1ms[17] >=K10msBased1msCnst )
    {
        m_i16IoConfCntPer1ms[17] = K10msBased1msCnst;
        objParallel.m_st_wParaSignalF.bMasterOnlineF = 1;       //������
    }
}
else
{
    m_i16IoConfCntPer1ms[17]--;
    if(m_i16IoConfCntPer1ms[17] <=0)
    {
        m_i16IoConfCntPer1ms[17] = 0;
        objParallel.m_st_wParaSignalF.bMasterOnlineF = 0;        //������

    }       

}
#endif
			 
//-----------------------bBpSTSShortFaultȷ��------------------------------------------
			//By wxw20090601 Adapter15~20k bBpSTSShortFault�������Ҽ��		
		//-----------------------bBpOvCurrFaultȷ��------------------------------------------
		//By wxw20090625 Adapter15~20k bBpOvCurrFault�������Ҽ��	
//-------------------------------bBPSFȷ��-----------------------------//
			if(m_un_ExInDReg.BIT.bBPSCR_F == 0)
			{
				m_i16IoConfCntPer1ms[18]++;
				if(m_i16IoConfCntPer1ms[18] >=K2msBased1msCnst )
				{
					m_i16IoConfCntPer1ms[18] = K2msBased1msCnst;
					m_st_wLocalSignal.bBPSF = 0;
				}
			}
			else
			{
				m_i16IoConfCntPer1ms[18]--;
				if(m_i16IoConfCntPer1ms[18] <=0)
				{
					m_i16IoConfCntPer1ms[18] = 0;
					m_st_wLocalSignal.bBPSF = 1;		//1��Ч

				}		
		
			}

//-------------------------------bQ3BPȷ��-----------------------------//
			if(m_un_ExInAReg.BIT.bQ3Bp == 1)
			{
				m_i16IoConfCntPer1ms[19]++;
				if(m_i16IoConfCntPer1ms[19] >=K4msBased1msCnst )
				{
					m_i16IoConfCntPer1ms[19] = K4msBased1msCnst;
					m_st_HardwareSignal.bQ3BP = 0;
				}
			}
			else
			{
				m_i16IoConfCntPer1ms[19]--;
				if(m_i16IoConfCntPer1ms[19] <= 0)
				{
					m_i16IoConfCntPer1ms[19] = 0;
					m_st_HardwareSignal.bQ3BP = 1;

				}		
		
			}
			
//-------------------------------bVoutAbnomalFromBPȷ��-----------------------------//			

//-------------------------------bOUT_PFȷ��-----------------------------//
			if(m_un_ExInAReg.BIT.bRev == 1)
			{
				m_i16IoConfCntPer1ms[20]++;
				if(m_i16IoConfCntPer1ms[20] >=K4msBased1msCnst )
				{
					m_i16IoConfCntPer1ms[20] = K4msBased1msCnst;
					m_st_wLocalNewSignal.bOUT_PF = 1;
				}
			}
			else
			{
				m_i16IoConfCntPer1ms[20]--;
				if(m_i16IoConfCntPer1ms[20] <=0)
				{
					m_i16IoConfCntPer1ms[20] = 0;
					m_st_wLocalNewSignal.bOUT_PF = 0;

				}		
		
			}
//-------------------------------invɢ��������ȷ��-----------------------------//  

			if(m_un_ExInAReg.BIT.bInvHeatSinkOVTemp == 0)
			{
				m_i16IoConfCntPer1ms[21]++;
				if(m_i16IoConfCntPer1ms[21] >= K200msBased1msCnst )
				{
					m_i16IoConfCntPer1ms[21] = 30000;
					m_st_wHardwareNewSignal.bInvHeatSinkOVTemp = 1;         // 1Ϊ����
				}
			}
			else
			{
				m_i16IoConfCntPer1ms[21]--;
				if(m_i16IoConfCntPer1ms[21] <=0)
				{
					m_i16IoConfCntPer1ms[21] = 0;
					m_st_wHardwareNewSignal.bInvHeatSinkOVTemp = 0;

				}		
		
			}
}
}		


/************************************************************************************
��������	Dat_DigitalOutput()			
����������	EXTIO�������ģ�飬�����߼��л��Ľ���Ͳ����ź����Ҫ��ȣ�������źż��д���һ��������
�������ã�	��
************************************************************************************/

	void	Class_DigitalIO::Dat_DigitalOutput()

	{
		m_st_wLocalSignal.bBpSTS = objXfer.m_st_XferResult.bBpSTS;						//������·STSλ
		m_st_wLocalSignal.bInvKM = objXfer.m_st_XferResult.bInvKM;						//�������STSλ
		//����SCR�Ŀ�ͨʱ���Ա�֤�̵���������
		if(m_un_ExOutAReg.BIT.bINVKM != m_st_wLocalSignal.bInvKM)
		    m_un_ExOutAReg.BIT.bINVSCR = 1;

		if((m_un_ExOutAReg.BIT.bINVSCR == 1)&&(objSystem.m_st_wFlagSystem.bTimer1ms == 1))
		{
		    m_u16InvSCROffCnt++;
		    if(m_u16InvSCROffCnt == 30)
		    {
		        m_u16InvSCROffCnt = 0;
		        m_un_ExOutAReg.BIT.bINVSCR = 0;
		    }
		    
		}

		//ע�����¸�ֵ���ʱ�򣬲��ܷ�����������SCR��ͨif���֮ǰ�������ܿ�ͨSCR
		m_un_ExOutAReg.BIT.bINVKM = m_st_wLocalSignal.bInvKM;


   	    if((objInverter.m_st_wFlagInv.bInv == 3)&&(m_un_ExOutAReg.BIT.bINVKM == 1))
	    {
	        m_un_ExOutAReg.BIT.bInvRunStatus = 2;
	        m_un_ExOutAReg.BIT.bInvOn = 1;
	    }
	    else if((objInverter.m_st_wFlagInv.bInv != 0)&&(m_un_ExOutAReg.BIT.bINVKM == 0))
	    {
            if(objSystem.m_st_wFlagSystem.bTimer40ms == 1)
                m_u16InvRunStatusCnt++;
            if(m_u16InvRunStatusCnt < 3)
                m_un_ExOutAReg.BIT.bInvRunStatus = 0;
            else if(m_u16InvRunStatusCnt < 9)
                m_un_ExOutAReg.BIT.bInvRunStatus = 2;
            else
                m_u16InvRunStatusCnt = 0;
        }
	    else
	    {
	        m_un_ExOutAReg.BIT.bInvRunStatus = 5;
	        m_un_ExOutAReg.BIT.bInvOn = 0;
	    }     



        m_un_ExOutAReg.BIT.bPWMEN = m_st_wLocalSignal.bInvOffToCPLD;
        m_un_ExOutAReg.BIT.bPWMENbak = m_st_wLocalSignal.bInvOffToCPLD;
        m_un_ExOutCReg.BIT.bINV_OVERLOAD = objParallel.m_st_wParaSignal.bInvOverload;
        m_un_ExOutCReg.BIT.bSYSTEM_UNLOCK = objParallel.m_st_wParaSignal.bSystemUnlock;
        m_un_ExOutCReg.BIT.bINV_ON_STANDBY = objParallel.m_st_wParaSignal.bInvOnStandby;
        m_un_ExOutCReg.BIT.bINV_BP_STATUS = objParallel.m_st_wParaSignal.bInvBpStatus;
        m_un_ExOutCReg.BIT.bMASTER_ONLINE = objParallel.m_st_wParaSignal.bMasterOnline;
        m_un_ExOutCReg.BIT.bModuleOnline = m_st_wLocalSignal.bModelOnline;
        m_un_ExOutCReg.BIT.bBPSCR = m_st_wLocalSignal.bBpSTS;
        m_un_ExOutBReg.BIT.bIMC_Enable = m_st_wLocalNewSignal.bIMC_Enable;
        m_un_ExOutBReg.BIT.bMACID = 1<<(m_st_wHardwareNewSignal.bInvID-1);

        
		/*
		//���״̬��
		if(objSystem.m_st_wFlagStatus.bInv == 0)
		{
		    m_un_ExOutAReg.BIT.bInvRunStatus = 0;
		}
		else if(objSystem.m_st_wFlagStatus.bInv == 3)
		{
		    m_un_ExOutAReg.BIT.bInvRunStatus = 3;
		}
		else if(objSystem.m_st_wFlagStatus.bInv == 1)
		{
            if(objSystem.m_st_wFlagSystem.bTimer40ms == 1)
                m_u16InvRunStatusCnt++;
            if(m_u16InvRunStatusCnt < 10)
                 m_un_ExOutAReg.BIT.bInvRunStatus = 0;
            else if(m_u16InvRunStatusCnt < 20)
                 m_un_ExOutAReg.BIT.bInvRunStatus = 3;
            else
                 m_u16InvRunStatusCnt = 0;
            
		}*/
	}


/************************************************************************************
��������	Dat_DigitalOutput()			
����������	EXTIO�������ģ�飬�����߼��л��Ľ���Ͳ����ź����Ҫ��ȣ�������źż��д���һ��������
�������ã�	��
************************************************************************************/
	INT16	Class_DigitalIO::Dat_InvIDOk(void)
	{
		return (m_i16InvIDOkFlag);	
	}

//#pragma CODE_SECTION(".Epwm1Funcs")

//SPI Receive Singal
void	Class_DigitalIO::Dat_Int_SetExInReg(UINT16	u16_InAReg)
{
	union 
	{
		UINT16 								all;
		struct
		{
			UINT16	bRve					:14;				// :
			UINT16	bFrameID				:2;					// :			
		}									BIT;
	}ExInReg;		
	
		

	
	ExInReg.all = u16_InAReg;	
	
	switch(ExInReg.BIT.bFrameID)
	{
		
		case (0):
		{
			m_un_ExInAReg.all = ExInReg.all;
			break;
		}
		case (1):
		{
			m_un_ExInBReg.all = ExInReg.all;
			break;	
		}
		case (2):
		{
			m_un_ExInCReg.all = ExInReg.all;
			break;
		}
		case (3):
		{
			m_un_ExInDReg.all = ExInReg.all;
		}
		default:
		{
			break;
		}
	}

}

//===========================================================================
// End of file.
//===========================================================================
