/*==========================================================*/ 	
/* Title		:	System.cpp													*/ 	
/* Description	: 	Class_System definition											*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#include "System.h"

/************************************************************************************
��������App_SysParamConfig()
************************************************************************************/
	void	Class_System::App_SysParamConfig(void)
	{
		m_i161DivGrantRate=((INT32)37*1024)/objMonInterface.m_i16GrantKVA;	//��Ȩ�ʵĵ��� 6.10 format  
		UPDNLMT16(m_i161DivGrantRate,4096,512);														//m_i16ModelKVA=(1~4)m_i16wGrantKVA
		
		//zqf 061225--���Ӷ�m_i161DivGrantKVA�ĸ��£�CAN��ʹ�ã�
		m_i161DivGrantKVA = 3072/objMonInterface.m_i16GrantKVA;         								//ע��m_i161DivGrantKVA�ķ�Χ��Ŀǰû�з�Χά��					
	} 
	
/************************************************************************************
��������	App_SysVarInit()	
����������	System�������ʼ��		
�������ã�	
***********************************************************************************/
	 void	Class_System::App_SysVarInit(void)
	{

	#ifndef	_GZTEST
		m_st_wFlagSystem.bParamSetReq=1;			//Parameter Setting Request upon reset
	#endif
		m_i161DivGrantKVA = 3072/objMonInterface.m_i16GrantKVA;																			//=3072/objMonInterface.m_i16GrantKVA=3072/80;
		
		EALLOW;                                             											
    	m_u16DSPRevision = DevCfgRegs.REVID;												//Get the DSP Revision
    	EDIS; 
		objDigitalIO.m_st_wLocalSignal.bFaultInd =1;
		objDigitalIO.m_st_wLocalSignal.bNormalInd = 1;
    	                                              											   													   			
	}
/************************************************************************************
��������	App_ModelOnline()	
����������	System�������ʼ��		
�������ã�	
***********************************************************************************/
void	Class_System::App_ModelOnline(void)
{
	if(m_st_wFlagFault.bAuxPS2Fault ==0)     					
	{
		//����ģ�����߱�־����������·���ӹ���·����Ȩ                                                    											
    		objDigitalIO.m_st_wLocalSignal.bModelOnline = 1;   
		
	}
	else					//������Դ������˳�����·�Ŀ���
	{			
    		objDigitalIO.m_st_wLocalSignal.bModelOnline = 0;   
	}
}


/************************************************************************************
��������	vOper_Fault()
����������	���������ж�ģ��
�������ã�	��			
************************************************************************************/
	void	Class_System::Alg_OperFault(void)
	{
		//zqf 060912--�����ϵ�bInterlock�źź���仯��Q5��Q3bpͬʱ�պϣ�������ϲ����������������
		if ((( objInverter.m_st_wFlagInv.bInv != 0 ) && ( objSwitches.m_st_FlagBreaker.bQ3bp == 1 ) 
			&& ( objSwitches.m_st_FlagBreaker.bQ5 == 1 ))
			|| ( ( objMonInterface.m_st_wUPSModeRun.bConfig == 3 ) && ( objSwitches.m_st_FlagBreaker.bQ3bp == 1 ) 
			&& ( objParallel.m_st_wParaSignalF.bInvBpStatusF == 1 ) ) 
			//zqf 060413--��̨TESTģʽ�����£����ڲ�����տ�����ʱ���߼����˳�TESTģʽ�������û���������
			|| objParallel.m_st_wParaMaint.bTestAbnormal == 1 )
		{
			m_st_wFlagFault.bOperFault = 1;																//�ò�������λ
			objInvCtrl.m_st_wFlagInvOnoff.bManualRecoverOff = 1;										//�ÿ���Ϊ�ָ����Ϲػ�
		}                                                                                   			
		else                                                                                			
		{                                                                                   			
			m_st_wFlagFault.bOperFault = 0;																//���������λ
		}
	}
	
/************************************************************************************
��������	vSystem_Fault()
����������	ϵͳ����(����/��Դ)�ж�ģ��
�������ã�	��			
************************************************************************************/
	void	Class_System::Alg_SysFault(void)
	{
		
		//POW_OK��ʱȷ��
		if ((objDigitalIO.m_un_ExInAReg.BIT.bPower15VFault == 0)&& (m_i16PowerOKWrongCntPerMloop < 10) )			//POW_OK = 0
		{
			m_i16PowerOKWrongCntPerMloop++;
		}
		else if (( objDigitalIO.m_un_ExInAReg.BIT.bPower15VFault == 1)&&(m_i16PowerOKWrongCntPerMloop > 0 ))
		{
			m_i16PowerOKWrongCntPerMloop--;
		}	

		if (m_i16PowerOKWrongCntPerMloop >= 3 )
		{
			m_st_wFlagPrompt.bPow15vFault = 1;	//��POW_OK����λ
			m_st_wFlagFault.bAuxPS2Fault =1;			
			objInvCtrl.m_st_wFlagInvOnoff.bManualRecoverOff = 1;										//�ÿ���Ϊ�ָ��ػ�λ
			objPWMDrv.Drv_PWMInactive();	                                   							// PWM���ǿ��Ϊ����̬
		}
		else
		{
			m_st_wFlagPrompt.bPow15vFault = 0;
			m_st_wFlagFault.bAuxPS2Fault =0;			//2//�� POW_Fault����λ

			//zqf051020:POWOK ���쳣������ʱ��������������������������ź�����������			
			if((m_st_wFlagStatus.bPow15vFaultBak == 1) && (objInvCtrl.m_i16FlagOffReason < 5) )
			{
				objMonInterface.m_st_wMonitorOrder.bReset = 1;
			}
		}

		m_st_wFlagStatus.bPow15vFaultBak = m_st_wFlagPrompt.bPow15vFault;

		//Ϊֱ�����ȣ����Ƿ�������޹أ��ҹ����ź�Ϊ�͵�ƽ
		if(m_st_wFlagSystem.bTimer1ms == 1)
		{
			if ( (objDigitalIO.m_st_HardwareSignal.bFanFault == 1) && (objDCBus.m_i16Vdc_0>=2528) )	//���ȹ���
			{
				m_i16FanFaultCntPer1ms++;																//���ȹ���ȷ�ϼ���������
				if ( m_i16FanFaultCntPer1ms >= 3000 )													//3s
				{
					m_i16FanFaultCntPer1ms = 3000;													//�޷���3000
					m_st_wFlagFault.bFanFault = 1;												//�÷��ȹ���λ
				}
			}
			else
			{				
				m_i16FanFaultCntPer1ms = 0;									//�޷���0
				m_st_wFlagFault.bFanFault = 0;							//����ȹ���λ
			}
		}
	}
	
/************************************************************************************
��������	vDSPID_Check()			
����������	����־����ģ��
�������ã�	��	
***********************************************************************************/
	 void	Class_System::App_DSPIDCheck(void)
	{
		UINT16	j, u16DSPErr;
		
		u16DSPErr=0;

		//GPIO33
		for(j=0; j<10; j++)
		{	
//			DSP28x_usDelay(30);
			if( objDigitalIO.m_un_ExInBReg.BIT.bMPUProductVer !=1 )
			{
				u16DSPErr = 0;//debugonly
			}
		}
				
		if(u16DSPErr == 1)								//�����Ӳ����ƥ��
		{
			while(1)
			{
				objTimerDrv.Drv_usDelay(30000);  			
				objDigitalIO.m_st_wLocalSignal.bDSPFirmWareOK = 0;
			}
		}
		else
		{
			objTimerDrv.Drv_usDelay(30000); 
			objDigitalIO.m_st_wLocalSignal.bDSPFirmWareOK = 1;
			objDigitalIO.m_st_wHardwareNewSignal.bDSPFirmWareOK = 1;
			
		}
	}
/***********************************************************************************
��������   vCPLD_Ver()
���������� ���CPLD�汾ȷ��
�������ã� ��
************************************************************************************/
	void	Class_System::App_CPLD_Ver()
	{
	  switch ( objDigitalIO.m_un_ExInBReg.BIT.bMPUProductVer )
	  {
	  	case 0:
	  		m_i16CPLDVerD1 = 1;
	  		m_i16CPLDVerD2 = 0;
	  		m_i16CPLDVerD3 = 0;
	  		break;

	  	case 1:
	  		m_i16CPLDVerD1 = 1;
	  		m_i16CPLDVerD2 = 1;
	  		m_i16CPLDVerD3 = 0;
	  		break;

	  	case 2:
	  		m_i16CPLDVerD1 = 2;
	  		m_i16CPLDVerD2 = 2;
	  		m_i16CPLDVerD3 = 0;
	  		break;               
                             
	  	case 3:              
	  		m_i16CPLDVerD1 = 3;
	  		m_i16CPLDVerD2 = 3;
	  		m_i16CPLDVerD3 = 0;
	  		break;

	  	case 4:
	  		m_i16CPLDVerD1 = 4;
	  		m_i16CPLDVerD2 = 4;
	  		m_i16CPLDVerD3 = 0;
	  		break;               
                             
	  	case 5:              
	  		m_i16CPLDVerD1 = 5;
	  		m_i16CPLDVerD2 = 5;
	  		m_i16CPLDVerD3 = 0;
	  		break;               
                             
	  	case 6:              
	  		m_i16CPLDVerD1 = 6;
	  		m_i16CPLDVerD2 = 6;
	  		m_i16CPLDVerD3 = 0;
	  		break;               
                             
	  	case 7:              
	  		m_i16CPLDVerD1 = 7;
	  		m_i16CPLDVerD2 = 7;
	  		m_i16CPLDVerD3 = 0;
	  		break;

	  	case 8:
	  		m_i16CPLDVerD1 = 8;
	  		m_i16CPLDVerD2 = 8;
	  		m_i16CPLDVerD3 = 0;
	  		break;               
                             
	  	case 9:              
	  		m_i16CPLDVerD1 = 9;
	  		m_i16CPLDVerD2 = 9;
	  		m_i16CPLDVerD3 = 0;
	  		break;               
                             
	  	case 10:             
	  		m_i16CPLDVerD1 = 10;
	  		m_i16CPLDVerD2 = 0;
	  		m_i16CPLDVerD3 = 0;
	  		break;               
                             
	  	case 11:             
	  		m_i16CPLDVerD1 = 11;
	  		m_i16CPLDVerD2 = 1;
	  		m_i16CPLDVerD3 = 0;
	  		break;

	  	case 12:
	  		m_i16CPLDVerD1 = 12;
	  		m_i16CPLDVerD2 = 2;
	  		m_i16CPLDVerD3 = 0;
	  		break;               
                             
	  	case 13:             
	  		m_i16CPLDVerD1 = 13;
	  		m_i16CPLDVerD2 = 3;
	  		m_i16CPLDVerD3 = 0;
	  		break;               
                             
	  	case 14:             
	  		m_i16CPLDVerD1 = 14;
	  		m_i16CPLDVerD2 = 4;
	  		m_i16CPLDVerD3 = 0;
	  		break;               
                             
	  	case 15:             
	  		m_i16CPLDVerD1 = 15;
	  		m_i16CPLDVerD2 = 5;
	  		m_i16CPLDVerD3 = 0;
	  		break;

	  	default:
			break;

	  }
	}
/***********************************************************************************
��������   App_Module_Ok()
���������� ģ��׼��okȷ��
�������ã� ��
************************************************************************************/
	void	Class_System::App_Module_Ok()
{
	if(m_st_wFlagSystem.bTimer1ms == 1)
	{
		if(objDigitalIO.m_st_wHardwareNewSignal.bModuleReady == 1)
		{
			m_i16ModuleOkCnt++;
			if(m_i16ModuleOkCnt>=2000)				//ModuleReady�ź�����ȷ��2s
			{
				m_i16ModuleOkCnt = 2000;
				objDigitalIO.m_st_wLocalSignal.bModuleReadyOk = 1;
			}
			m_i16ModuleNotOkCnt = 0;
		}
		else
		{
			m_i16ModuleNotOkCnt++;
			if(m_i16ModuleNotOkCnt >= 5)			//ModuleReady�źŲ�����ȷ��5ms
			{
				m_i16ModuleNotOkCnt = 5;
				objDigitalIO.m_st_wLocalSignal.bModuleReadyOk = 0;
			}
			m_i16ModuleOkCnt = 0;
		}
		
	}
	 //ģ��δ����������ѿ���������ֶ��ɻָ��ػ�
	
	if((objDigitalIO.m_st_wLocalSignal.bModuleReadyOk  == 0)&&(objInverter.m_st_wFlagInv.bInv != 0))

	{
		objInvCtrl.m_st_wFlagInvOnoff.bManualRecoverOff=1;

	}

}
/***********************************************************************************
��������   App_Module_LED()
���������� ģ��led����ʾ����
�������ã� ��
************************************************************************************/
void	Class_System::App_Module_LED()
{
	if((objSystem.m_st_wFlagFault.bInvFault  == 1)||(objSystem.m_st_wFlagPrompt.bShortCircuit == 1)				//���ع���
		||(objSwitches.m_st_FlagBreaker.bBpSTSShort  == 1)||(objSwitches.m_st_FlagBreaker.bInvKMShort == 1)
		||(objSystem.m_st_wFlagFault.bDCAbnormal == 1)||(objDigitalIO.m_st_wLocalNewSignal.bRECFault ==2)
		||(objDigitalIO.m_st_wLocalSignal.bModuleReadyOk ==0) || (objMonInterface.m_st_IdErrorFlag.bUsIdNumExpire == 1)
		||(objMonInterface.m_st_IdErrorFlag.bUsIdNumOverlap == 1) ||(objSystem.m_st_wFlagFault.bBypFeedBackFault == 1))
	{
		objDigitalIO.m_st_wLocalSignal.bFaultInd = 0;						//�����
	}
	else if((objSystem.m_st_wFlagPrompt.bParaShareFault == 1)||(objLoad.m_st_wFlagLoad.bOvload==1)
		||(objSwitches.m_st_FlagBreaker.bInvKMOpen == 1)||(objSwitches.m_st_FlagBreaker.bBpSTSOpen == 1)
		||(objSwitches.m_st_FlagBreaker.bFuse == 0)||(objDigitalIO.m_st_wLocalNewSignal.bRECFault  == 1)
		||(objInverter.m_st_FlagVinvABC.bInvCurrUnbalancedWarn == 1)||(m_st_wFlagFault.bFanFault == 1))			//�����������ϣ�����������أ����̵�����·��·SCR��·������һ�����
	{
		
			if(objDigitalIO.m_st_wLocalSignal.bFaultInd ==1)			//�����˸
			{
				m_i16FaultLedCnt++;
				if(m_i16FaultLedCnt >=1000)
				{
					m_i16FaultLedCnt =0;
					objDigitalIO.m_st_wLocalSignal.bFaultInd = 0;
				}
			}
			else
			{
				m_i16FaultLedCnt++;
				if(m_i16FaultLedCnt >=1000)
				{
					m_i16FaultLedCnt =0;
					objDigitalIO.m_st_wLocalSignal.bFaultInd = 1;
				}

			}													//��Ƽ��̵ƾ���������ܹ���										
	}
	else
	{

		objDigitalIO.m_st_wLocalSignal.bFaultInd = 1;						//��Ʋ������̵������������ģ���޹���				
	}
//�̵���ʾ	
	if(objXfer.m_st_XferResult.bInvKM == 1)							//����е����̵Ƴ���
	{
		objDigitalIO.m_st_wLocalSignal.bNormalInd = 0;				
	}
	 else if(objInverter.m_st_wFlagInv.bInv!=0)   //�������������̵�һֱ��˸
	{
		if(objDigitalIO.m_st_wLocalSignal.bNormalInd ==1)			//�̵���˸
		{
			m_i16NormalLedCnt++;
			if(m_i16NormalLedCnt >=1000)
			{
				m_i16NormalLedCnt =0;
				objDigitalIO.m_st_wLocalSignal.bNormalInd = 0;
			}
		}
		else
		{
			m_i16NormalLedCnt++;
			if(m_i16NormalLedCnt >=1000)
			{
				m_i16NormalLedCnt =0;
				objDigitalIO.m_st_wLocalSignal.bNormalInd = 1;
			}

		}		

	}
	else if(objInverter.m_st_wFlagInv.bInv ==0)     			//��䲻�����̵Ƴ���
	{
		objDigitalIO.m_st_wLocalSignal.bNormalInd = 1;
	 }
	

}
//===========================================================================
// End of file.
//===========================================================================
