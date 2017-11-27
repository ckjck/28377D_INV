/*==========================================================*/ 	
/* Title		:	MonitorInterface.cpp											*/ 	
/* Description	: 	Class_MonitorInterface definition									*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#include "MonitorInterface.h"

/************************************************************************************
函数名：	App_MonitorVarInit()	
功能描述：	MonitorINT16erface类变量初始化		
函数调用：	
***********************************************************************************/
	 void	Class_MonitorInterface::App_MonitorVarInit(void)
	{
		UINT16	u16Temp;
		m_i16FaultResetCntPerMloop = 400;				//INV_RESET端口的功能改变
		
		m_i16ModelKVA = 50;						//50kVA		
		m_i16GrantKVA = 50;			
		//m_st_wUPSModeRun.bConfig = 3;
		m_st_wUPSModeRun.bConfig = 0;
		m_st_wUPSModeRun.bMode = 0;
		m_st_wUPSModeRun.bVoltOut = 1;				//380V
		m_st_wUPSModeRun.bID208 = 0;				//zqf051020:480V仍然认为是400V 体系，有利于mntr.c中对接触器和晶闸管故障的分类
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
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_a = 4096;					//无效数据,赋默认值					
	}	
	
	u16Temp = objEEPROM.App_ParamRead(VoutBRevAddr);
	if ( ( u16Temp >= 3686 ) && ( u16Temp <= 4506 ) )
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_b =  u16Temp;
	}
	else
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_b = 4096;					//无效数据,赋默认值					
	}	
	
	u16Temp = objEEPROM.App_ParamRead(VoutCRevAddr);
	if ( ( u16Temp >= 3686 ) && ( u16Temp <= 4506 ) )
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_c =  u16Temp;
	}
	else
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_c= 4096;					//无效数据,赋默认值					
	}

	u16Temp = objEEPROM.App_ParamRead(IoutARevAddr);
	if ( ( u16Temp >= 3686 ) && ( u16Temp <= 4506 ) )
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_a =  u16Temp;
	}
	else
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_a = 4096;					//无效数据,赋默认值					
	}

	u16Temp = objEEPROM.App_ParamRead(IoutBRevAddr);
	if ( ( u16Temp >= 3686 ) && ( u16Temp <= 4506 ) )
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_b =  u16Temp;
	}
	else
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_b= 4096;					//无效数据,赋默认值					
	}

	u16Temp = objEEPROM.App_ParamRead(IoutCRevAddr);
	if ( ( u16Temp >= 3686 ) && ( u16Temp <= 4506 ) )
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_c=  u16Temp;
	}
	else
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_c = 4096;					//无效数据,赋默认值					
	}

	u16Temp = objEEPROM.App_ParamRead(VinvARevAddr);
	if ( ( u16Temp >= 3686 ) && ( u16Temp <= 4506 ) )
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_a =  u16Temp;
	}
	else
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_a= 4096;					//无效数据,赋默认值					
	}

	u16Temp = objEEPROM.App_ParamRead(VinvBRevAddr);
	if ( ( u16Temp >= 3686 ) && ( u16Temp <= 4506 ) )
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_b =  u16Temp;
	}
	else
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_b = 4096;					//无效数据,赋默认值					
	}

	u16Temp = objEEPROM.App_ParamRead(VinvCRevAddr);
	if ( ( u16Temp >= 3686 ) && ( u16Temp <= 4506 ) )
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_c =  u16Temp;
	}
	else
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_c = 4096;					//无效数据,赋默认值					
	}

	u16Temp = objEEPROM.App_ParamRead(IinvARevAddr);
	if ( ( u16Temp >= 3686 ) && ( u16Temp <= 4506 ) )
	{
		 m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_a=  u16Temp;
	}
	else
	{
		 m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_a = 4096;					//无效数据,赋默认值					
	}

	u16Temp = objEEPROM.App_ParamRead(IinvBRevAddr);
	if ( ( u16Temp >= 3686 ) && ( u16Temp <= 4506 ) )
	{
		 m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_b =  u16Temp;
	}
	else
	{
		 m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_b = 4096;					//无效数据,赋默认值					
	}

	u16Temp = objEEPROM.App_ParamRead(IinvCRevAddr);
	if ( ( u16Temp >= 3686 ) && ( u16Temp <= 4506 ) )
	{
		 m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_c=  u16Temp;
	}
	else
	{
		 m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_c= 4096;					//无效数据,赋默认值					
	}
	
	u16Temp = objEEPROM.App_ParamRead(VbpARevAddr);
	if ( ( u16Temp >= 3482 ) && ( u16Temp <= 4710 ) )
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_a=  u16Temp;
	}
	else
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_a= 4096;					//无效数据,赋默认值					
	}

	u16Temp = objEEPROM.App_ParamRead(VbpBRevAddr);
	if ( ( u16Temp >= 3482 ) && ( u16Temp <= 4710 ) )
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_b =  u16Temp;
	}
	else
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_b= 4096;					//无效数据,赋默认值					
	}
	
       u16Temp = objEEPROM.App_ParamRead(VbpCRevAddr);
	if ( ( u16Temp >= 3482 ) && ( u16Temp <= 4710 ) )
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_c=  u16Temp;
	}
	else
	{
		m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_c = 4096;					//无效数据,赋默认值					
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
		
	
		m_i16AlterPeriodSet = 0;					//主从热备份，不交换，最安全。
       
		m_i16FreqSlewRate = 1;					//0.1Hz/s
		m_i16VoltDispScale =2200;
		
		m_i16SelfAgingCurrRate = 20;				//20％ 
//版本初始化
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

		//告警和状态初始化
		//状态量
		//空开都输出化为断开
		m_st_InvSendMonitor.m_st_InvBreakerStatus.Word1_U.u16Word1All = 0;
		m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.u16Word2All = 0;

		//告警
		m_st_InvSendMonitor.m_st_InvWarn.Word1_U.u16Word1All = 0;
		m_st_InvSendMonitor.m_st_InvWarn.Word2_U.u16Word2All = 0;
		m_st_InvSendMonitor.m_st_InvWarn.Word3_U.u16Word3All = 0;

		//机架内并机状态初始化
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
函数名：	App_MonitorDeal()
功能描述：	监控处理模块，处理监控命令，设置参数和组织状态上传
函数调用：	App_MonitorOrder():		监控命令处理模块，处理监控发来的各类开机、关机、确认及其取消命令
			App_SendtoMonitor():	监控信息综合模块，处理从DSP到Monitor的上报信息
			App_MonitorParam():		监控参数映射模块，设置Monitor传递下来的参数
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
		//不能直接对宏进行赋值，不符合编码规范
		objMonInterface.ModuleNum = 2;

		//加工本机信息
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
文件名称：	App_ParallelLocalData( )
功能描述：	将本机的数据放到相应UPS数据存贮单元中
算法说明：	none

入口参数：	
			objSwitches.m_st_FlagBreaker
			m_i16Pouta
			m_i16Poutb
			m_i16Poutc
			m_i16Souta
			m_i16Soutb
			m_i16Soutc
			m_i16ModelKVA
			sParaData
			
出口参数：	
			sParaData
			uInvRealInfo
			iLocalStatusChanged
			
局部变量：	
			uLocalStatus
			uCanDataBuff1
函数调用：	none
*************************************************************/		
VOID Class_MonitorInterface::App_ParallelLocalData(VOID )
{
	_PARALLEL_SW1_T		uLocalStatus ;

	//本机ID,模块ID从0到14
	UINT16 	u16Idx;
	u16Idx = objDigitalIO.m_st_wHardwareNewSignal.bInvID-1;
	
	uLocalStatus.Word1_U.u16Word1All = 0;
		
	if ( objSwitches.m_st_FlagBreaker.bInv == 1 )			//逆变开启
	{
		uLocalStatus.Word1_U.tWord1Bits.bInvOn = 1;
	}
				
	if ( objSwitches.m_st_FlagBreaker.bOn == 1 )			//逆变供电
	{
		uLocalStatus.Word1_U.tWord1Bits.bInvPowerOn = 1;
	}
					
	//旁路供电判断
	if ( objSwitches.m_st_FlagBreaker.bBpOn == 1 )		//旁路供电
	{
		uLocalStatus.Word1_U.tWord1Bits.bBypPowerOn = 1;
	}
	

	// 增加旁路幅值范围标志
	if (objBypass.m_st_wBpAmpRangeFlag.bLocalWide == 1)
	{
		uLocalStatus.Word1_U.tWord1Bits.bBypRange = 1;  //本机在线标志	
	}

	/* 合成本机告警信息 */
	//告警信息需要监控下传TBD
#if (0)
	uCanDataBuff1 = ( S2U(objMonitor.uInvRealInfo) << 2 ) & 0xFC;
	sParaData[objMonInterface.m_i16wSerialNumSet_0-0x10].uSystemFlag |= uCanDataBuff1;
#endif
	uLocalStatus.Word1_U.tWord1Bits.bSingleOnLine = 1;	//本机在线标志
	uLocalStatus.Word1_U.tWord1Bits.bBypState = objSystem.m_st_wFlagStatus.bVbp;

	m_st_wParallelData.ParallelState[u16Idx] = uLocalStatus;
	
	
		

}


/************************************************************************************
函数名：	vMonitor_Order()
功能描述：	监控命令处理模块，处理监控发来的各类开机、关机、确认及其取消命令
函数调用：	无
************************************************************************************/

	void	Class_MonitorInterface::App_MonitorOrder()

	{
		INT16 	i16temp;
		//增加DSP死机后自开机命令及模块自开机命令
		if( (objSystem.m_st_wFlagPrompt.bInvOnEnable == 1)					 //开机允许 
			&& ((objInverter.m_st_wFlagInv.bDspCrazyOff ==  1)				//有DSP死机切旁路开机标志
			||((objParallel.m_st_wParaSignalF.bMasterOnlineF== 1)&&( objInverter.m_st_wFlagInv.bInv == 0))))		//有系统开机命令逆变将会自开机	，即机架内有主机开机
		{
			if(objSystem.m_st_wFlagSystem.bTimer1ms == 1)
			{
				if(m_i16SystemInvOnCnt<2000)
				{
						if(objSystem.m_st_wFlagPrompt.bSwitchTimesClamp ==0)
						{
							m_i16SystemInvOnCnt++;
							if(m_i16SystemInvOnCnt >=2000)				//确认2s
							{
								m_i16SystemInvOnCnt =2000;
								m_st_wMonitorOrder.bOn = 1;
							}

						}
				}
			}
		}

		if ( m_st_wMonitorOrder.bOn == 1 )					//逆变器开机命令
		{
			objInvCtrl.m_st_wFlagInvOnoff.bMonitorOn = 1;				//置监控开机状态
			m_st_wMonitorOrder.bOn = 0;						//清逆变器开机命令
			objInverter.m_st_wFlagInv.bDspCrazyOff=0; 
			
		}

		if ( m_st_wMonitorOrder.bInstantOff == 1 )			//立即关机命令
		{
			objInvCtrl.m_st_wFlagInvOnoff.bInstantOff = 1;				//置立即关机状态
			m_st_wMonitorOrder.bInstantOff = 0;				//清立即关机命令
		}
		//用户关机先切换，后关机  关机切换命令备份 持续3个主循环，要求硬件延时小于1个主循环
		objInvCtrl.m_st_wFlagInvOnoff.bUserOffAct = objSystem.m_st_wFlagStatus.bUserOffBak;
		objSystem.m_st_wFlagStatus.bUserOffBak = objInvCtrl.m_st_wFlagInvOnoff.bUserOff;
		objInvCtrl.m_st_wFlagInvOnoff.bUserOff = 0;											//清用户关机切换命令

		if ( m_st_wMonitorOrder.bOff == 1 )										//逆变器关机命令
		{
			if ( ( objInverter.m_st_wFlagInv.bInv != 0 ) && ( objParallel.m_st_wParaSignal.bInvBpStatus == 1 ) )    //逆变器已开启且本机逆变侧
			{
				//剩余供电 能力(以 台数计)=总台数-1
				i16temp = objPowCANSched.m_i16OnTotalNum- 1;
				UPDNLMT16(i16temp,KKeepOnINVMaxCnst,0); //目前仅考虑1+1

				if ( ( m_st_wUPSModeRun.bConfig == 3 ) && ( i16temp > 0 ) )		//并机，且正供电机架大于1

				{
                                   //(Nxm意义变为多个机架供电)
					//并机一台以上供电时清单机风险关机提示位
					objSystem.m_st_wFlagStatus.bSingleRiskOff = 0;

					//实际带载大于剩余供电能力时置位
					if(((((INT32)objSystem.m_i16RackSoutTotalMax*KBpKVA)>>10)>= (((INT32)objMonInterface.m_i16ModelKVA*1024)/objInverter.m_i16TempKVAGrant)*i16temp)
					   			||((((INT32)objSystem.m_i16RackPoutTotalMax*KBpKVA)>>10)>=(((INT32)objMonInterface.m_i16ModelKVA*1024)/objInverter.m_i16TempKWGrant)*i16temp))
					{
						objSystem.m_st_wFlagStatus.bParaRiskOff = 1;						//置并机系统风险关机提示位
					}
					else
					{
						objSystem.m_st_wFlagStatus.bParaRiskOff = 0;						//清并机系统风险关机提示位
					}

				}
				else 
				{
					//单台供电时清并机系统关机提示位(Nxm意义变为单机架供电)
					objSystem.m_st_wFlagStatus.bParaRiskOff = 0;
					//对于手动关机，若旁路超跟踪或逆变不同步，且间断时间设置不为最小，产生单机风险关机提示			
					//单机旁路超保护或旁路晶闸管断路		
					if ( (objBypass.m_st_FlagVbpTotal.bBpTptalProtect == 1)||(objSwitches.m_st_FlagBreaker.bBpSTSOpen == 1)
					||( ((objBypass.m_st_FlagVbpTotal.bBpRange == 1)||(objInverter.m_st_wFlagInv.bPhaseLock == 0)) && (m_i16IntSwTimeSet != 0))  )
					{
						objSystem.m_st_wFlagStatus.bSingleRiskOff = 1;						//置单机系统风险关机提示位
					}
					else
					{
						objSystem.m_st_wFlagStatus.bSingleRiskOff = 0;						//清单机系统风险关机提示位
					}

				}
				
				if ( ( objSystem.m_st_wFlagStatus.bSingleRiskOff == 1 ) || ( objSystem.m_st_wFlagStatus.bParaRiskOff == 1 ) )
				{
					if ( m_st_wMonitorOrder.bRiskOffConf == 2 )					//风险关机确认位为取消
					{
						m_st_wMonitorOrder.bOff = 0;								//清逆变器关机命令
						objSystem.m_st_wFlagStatus.bParaRiskOff = 0;						//清并机系统风险关机提示位
						objSystem.m_st_wFlagStatus.bSingleRiskOff = 0;						//清单机系统风险关机提示位
						m_st_wMonitorOrder.bRiskOffConf = 0;						//清风险关机确认位
					}
					else
					{
						if ( m_st_wMonitorOrder.bRiskOffConf == 1 )				//风险关机确认位为确认
						{
							objSystem.m_st_wFlagStatus.bParaRiskOff = 0;					//清并机系统风险关机提示位
							objSystem.m_st_wFlagStatus.bSingleRiskOff = 0;					//清单机系统风险关机提示位
							m_st_wMonitorOrder.bRiskOffConf = 0;					//清风险关机确认位
							objInvCtrl.m_st_wFlagInvOnoff.bUserOff = 1;						//置用户关机位
							m_st_wMonitorOrder.bOff = 0;							//清逆变器关机命令
						}
					}
				}	
				else
				{
					objSystem.m_st_wFlagStatus.bParaRiskOff = 0;							//清并机系统风险关机提示位
					objSystem.m_st_wFlagStatus.bSingleRiskOff = 0;							//清单机系统风险关机提示位
					m_st_wMonitorOrder.bRiskOffConf = 0;							//清风险关机确认位
					objInvCtrl.m_st_wFlagInvOnoff.bUserOff = 1;								//置用户关机位
					m_st_wMonitorOrder.bOff = 0;									//清逆变器关机命令
				}	
			}
			else
			{
				objSystem.m_st_wFlagStatus.bParaRiskOff = 0;								//清并机系统风险关机提示位
				objSystem.m_st_wFlagStatus.bSingleRiskOff = 0;								//清单机系统风险关机提示位
				m_st_wMonitorOrder.bRiskOffConf = 0;								//清风险关机确认位
				objInvCtrl.m_st_wFlagInvOnoff.bUserOff = 1;									//置用户关机位
				m_st_wMonitorOrder.bOff = 0;										//清逆变器关机命令
			}
		}

		if ( m_st_wMonitorOrder.bOutDisable == 1 )								//输出禁止命令
		{
			objInvCtrl.m_st_wFlagInvOnoff.bOutDisableOff = 1;								//置输出禁止态关机
			objSystem.m_st_wFlagStatus.bOutDisable = 1;									//置输出禁止状态
			m_st_wMonitorOrder.bOutDisable = 0;									//清输出禁止命令
		}

		if ( m_st_wMonitorOrder.bOutEnable == 1 )								//输出允许命令
		{
			objSystem.m_st_wFlagStatus.bOutDisable = 0;									//清输出禁止状态
			m_st_wMonitorOrder.bOutEnable = 0;									//清输出允许命令
		}

//zqf 060616--EPO功能优化：EPO硬件信号有效时，禁止故障清除命令
		if(objDigitalIO.m_un_ExInARegConf.BIT.bEPO == 0)
		{
			m_st_wMonitorOrder.bReset = 0;
		  //m_st_wLocalSignal.bReset = 0;									
			m_i16FaultResetCntPerMloop = 0;
		}
		else if ( m_st_wMonitorOrder.bReset == 1 )								//故障清除命令
		{

			if ( objInvCtrl.m_i16FlagOffReason <= 4 )										//非不可恢复故障关机
			{
//				m_st_wLocalSignal.bReset = 1;									//发RESET硬件信号
				m_i16FaultResetCntPerMloop = 400;
			}		
			m_st_wMonitorOrder.bReset = 0;	

		}
		else																//无故障清除命令
		{
			if ( m_i16FaultResetCntPerMloop <= 0 )										//故障清除计数器
			{
//				m_st_wLocalSignal.bReset = 0;									//停止发送RESET信号
			}
			else
			{
				//上电时发硬件RESET信号的操作  iSpanFaultReset被初始化为400。				
//				if(m_st_wFlagSystem.bAfterParamDly==0)  m_st_wLocalSignal.bReset = 1;	
				m_i16FaultResetCntPerMloop --;											//故障清除计数器递减
				if ( m_i16FaultResetCntPerMloop == 8 )
				{
					objSystem.m_st_wFlagStatus.bEPO = 0;									//清紧急关机状态
					objSystem.m_st_wFlagStatus.bInvFault = 0;								//清逆变器故障状态
//zqf 060613--更改逆变器过流为不可清除故障
//					objSystem.m_st_wFlagStatus.bIGBTOCP = 0;								//清IGBT过流保护状态
					objSystem.m_st_wFlagFault.bInvFault = 0;								//清逆变器故障状态
//					objSystem.m_st_wFlagFault.bIGBTOCP = 0;								//清IGBT过流保护状态					
					//20K增加故障清除主路反灌标志
					objSystem.m_st_wFlagStatus.bMainFeedBack = 0;		//故障清除主路反灌标志
					//增加三单下，逆变不均流标志
					objInverter.m_st_FlagVinvABC.bInvCurrUnbalancedWarn= 0;

					objBypass.m_st_FlagIbpTotal.bIbpOverFault = 0;
					objBypass.m_st_FlagIbpTotal.bIbpOverTimeOut = 0;
				}	
			}
		}

		//本段容错代码在监控无法命令时有效
		m_st_wMonitorOrder.bRiskOffConf=0;
		if(objInverter.m_st_wFlagInv.bInv == 0)
		{
			objSystem.m_st_wFlagStatus.bSingleRiskOff = objSystem.m_st_wFlagStatus.bParaRiskOff =0;	
		}
		
		
		//如果外部输出空开断或内部输出空开与维修空开均断则逻辑上认为单机（隔离状态）
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
			objParallel.m_st_wParaMaint.bParaSignalMask = 1;	//信号隔离态
		}
		else										
		{
			objParallel.m_st_wParaMaint.bParaSignalMask = 0;
		}

		//输出禁止态－－ （隔离状态 且 非TEST模式） 
		//或（当前设置为TEST模式 且 内部输出空开闭合 ） 
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
		//关机后清除自开机计数器
		if(objInvCtrl.m_st_wFlagInvOnoff.bFinalOnOff  ==0)
		{
			if(m_i16SystemInvOnCnt >= 2000)
			{
				m_i16SystemInvOnCnt = 0;
			}
		}
	}


/************************************************************************************
函数名：	vSendto_Monitor()
功能描述：	监控信息综合模块，处理从DSP到Monitor的上报信息
函数调用：	无
************************************************************************************/
	void	Class_MonitorInterface::App_SendtoMonitor()

	{
		if ( objSystem.m_st_wFlagSystem.bKeyModuleDly == 1 )							//核心模块运行前延时完毕
		{
			//By wxw20100416 优化:逆变态下，如果由旁路电压触发的超跟踪，不上传告警，但底层切换仍然走间断
			if((objBypass.m_st_FlagVbpTotal.bBpRange == 1) && (objBypass.m_st_FlagVbpFC.bVbpF == 0)
				&& (objXfer.m_st_wXferCmd.bSwitchStatus == 1))
			{
				objSystem.m_st_wFlagStatus.bVbp = 0;
			}
			else
			{
				objSystem.m_st_wFlagStatus.bVbp = objBypass.m_st_FlagVbpTotal.bBpRange;					//拷贝旁路状态
			}	
		}

		objSystem.m_st_wFlagStatus.bInv = objInverter.m_st_wFlagInv.bInv;								//拷贝逆变器状态

//		m_st_wFlagStatus.bLock = objInverter.m_st_wFlagInv.bPhaseLock;						//拷贝相位锁定标志
		//zqf 040613--共性问题更改，逆变开机时屏蔽逆变不同步信息上报
		//zqf 061117--解决旁路晶闸管关不断问题，逆变开机时逆变不同步信息上报屏蔽时间加长：8s-->20s
		if(objSystem.m_st_wFlagStatus.bInv == 3)
		{
			if(objSystem.m_st_wFlagSystem.bTimer40ms == 1)
			{
				objInverter.m_i16wInvWarn1LockConfCntPer40ms++; 					//逆变器开机锁定20s确认
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
			objSystem.m_st_wFlagStatus.bLock = objInverter.m_st_wFlagInv.bPhaseLock; 		//拷贝相位锁定标志
		}
		else								
		{
			objSystem.m_st_wFlagStatus.bLock =1;  
		}


		if ( ( objSwitches.m_st_FlagBreaker.bQ5 == 1 ) && ( objParallel.m_st_wParaSignal.bInvBpStatus == 1 ) )    //Q5合，逆变侧
		{
			m_i16SupplyStatusCnt = 0;
			objSystem.m_st_wFlagStatus.bSupply = 2;											//逆变供电
		}
		else if (( objSwitches.m_st_FlagBreaker.bQ5 == 1 ) && ( objXfer.m_st_XferResult.bBpSTS == 1 )) 															//
		{
			m_i16SupplyStatusCnt = 0;
		}
		else
		{
			m_i16SupplyStatusCnt ++;												//解决逆变到旁路切换时由于状态改变时序问题造成"均不供电"误报
			if(m_i16SupplyStatusCnt >= 3)
			{
				objSystem.m_st_wFlagStatus.bSupply = 0;									//均不供电

			}
											
		}




		objSystem.m_st_wFlagFault.bOvload = objLoad.m_st_wFlagLoad.bOvload;							//拷贝输出过载位
		objSystem.m_st_wFlagFault.bInvOvloadTimeout = objLoad.m_st_wFlagLoad.bInvOvloadTimeout;		//拷贝逆变过载延时到位

		objSystem.m_st_wFlagPrompt.bSwitchTimesClamp = 0;								//清本小时过载切换次数到
		if ( objXfer.m_i16wBptoinvTimes > m_i16BptoinvTimesSet )
		{
			objSystem.m_st_wFlagPrompt.bSwitchTimesClamp = 1;							//置本小时过载切换次数到
		}

		objSystem.m_st_wFlagPrompt.bImpactSwitch = objLoad.m_st_wFlagLoad.bImpactSwitch;			//拷贝本机逆变冲击跳旁路标志
		objSystem.m_st_wFlagPrompt.bReqSwitch = objLoad.m_st_wFlagLoad.bReqSwitch;					//拷贝邻机过载冲击跳旁路标志

		if ( objSystem.m_st_wFlagPrompt.bTotalCapPrompt0 == 1 )						//系统负载大于当前有效UPS总容量提示
		{
			if ( objSystem.m_st_wFlagSystem.bTimer40ms == 1 )							//40ms到
			{
				m_i16PromptCntPer40ms ++;
				
				//启动容量不足延时为6s
				if ( m_i16PromptCntPer40ms > 150 )								//提示确认延时 6秒钟
				{
					m_i16PromptCntPer40ms = 150;

					objSystem.m_st_wFlagPrompt.bTotalCapPrompt = 1;					//置系统负载大于当前有效UPS总容量(经延时)
				}
			}
		}
		else
		{
			objSystem.m_st_wFlagPrompt.bTotalCapPrompt = 0;							//清系统负载大于当前有效UPS总容量(经延时)
			m_i16PromptCntPer40ms = 0;											//清延时五分钟提示
		}
	}


/************************************************************************************
函数名：	vMonitor_Param()
功能描述：	监控参数映射模块，设置Monitor传递下来的参数
函数调用	l_div_i()		长除法
************************************************************************************/
	void	Class_MonitorInterface::App_MonitorParam()

	{
		INT16 i16MainAX;
		
		if ( (m_st_wInvReceiveMonitor.m_st_SysDSPSettings.u16SetParamFresh == 1)||(m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh != 0) )				//参数更新标志
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
			
			//参数更新完毕	

			m_st_wInvReceiveMonitor.m_st_SysDSPSettings.u16SetParamFresh = 0;					//清参数更新标志
		}

//本机架内最大输出总功率计算
		objSystem.m_i16PoutTotalMax = (INT32)objPowCANSched.m_i16PoutTotalMax;
		objSystem.m_i16SoutTotalMax = (INT32)objPowCANSched.m_i16SoutTotalMax;

//机架间最大输出功率计算
			//By wxw20090725 Adator15k~20kPowerCan代替监控传递负载大小，已在PowerCan
			//模块计算
			
		objSystem.m_i16RackPoutTotalMax = (INT32)objPowCANSched.m_i16PoutTotalMax;
		objSystem.m_i16RackSoutTotalMax = (INT32)objPowCANSched.m_i16SoutTotalMax;
		
		//主从热备份
		if ( m_st_wUPSModeRun.bConfig == 2 )					//热备份主机
		{
			if ( m_i16AlterPeriodSet > 0 )					//主从切换
			{
				i16MainAX = ((INT32)objTimerDrv.m_u16ClkH_0)/m_i16AlterPeriodSet;
				m_st_wUPSModeRun.bMasterECO = i16MainAX % 2;
			}
			else													//0：主从不切换
			{
				m_st_wUPSModeRun.bMasterECO = 0;				//主机一直在线
			}
		}
		else
		{
			m_st_wUPSModeRun.bMasterECO = 0;				//从机一痹谙?
		}

	}
/************************************************************************************
函数名：	App_Int_TimeLabel()
功能描述：	时间戳产生
函数调用	无
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
函数名：	App_MonInterfaceSend()
功能描述：	发送状态，告警，模拟量给监控
函数调用	无
************************************************************************************/
		void	Class_MonitorInterface::App_MonInterfaceWrite()
		{

/**************************************逆变上传状态变量**************************************************/
			//维修旁路空开状态移至旁路侧
//			m_st_InvSendMonitor.m_st_InvBreakerStatus.Word1_U.tWord1Bits.bMaintainBypBrk = (objDigitalIO.m_st_HardwareSignal.bQ3BP |
//																				objParallel.m_st_wParaSignalF.bCabQ3BpStatusF);
		
			//参数请求设置	
			m_st_InvSendMonitor.m_st_InvBreakerStatus.Word1_U.tWord1Bits.bParamSetReq = objSystem.m_st_wFlagSystem.bParamSetReq;

			
	
			//20K使用，UPS的充电器开启数量，用于共用电池组充电器管理
			m_st_InvSendMonitor.m_st_InvBreakerStatus.Word1_U.tWord1Bits.bChargerOnNum = objPowCANSched.m_i16ChgOnTotalNum;
			
			/*逆变发送的强制转浮充命令*/
			m_st_InvSendMonitor.m_st_InvBreakerStatus.Word1_U.tWord1Bits.bCmdCharge = objPowCANSched.m_i16ChgPBoostTotal;
			
			/*使能电池预告警*/
			m_st_InvSendMonitor.m_st_InvBreakerStatus.Word1_U.tWord1Bits.bEnablePreWarn = objPowCANSched.m_i16BattPreWarnTotal ;
			/*共同自检*/
			m_st_InvSendMonitor.m_st_InvBreakerStatus.Word1_U.tWord1Bits.bEnableSeftTest = objPowCANSched.m_i16BattSelfTestTotal ;
		
	
			

			//逆变开机状态
			m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bStartupEna = objSystem.m_st_wFlagPrompt.bInvOnEnable;
			//逆变状态
			m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bInvState = objSystem.m_st_wFlagStatus.bInv;
			//供电状态
			if ( objSystem.m_st_wFlagStatus.bOutDisable == 1 )
			{
				m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bInvSupplyState= 3;			//本机输出禁止
			}
			else
			{
				m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bInvSupplyState = objSystem.m_st_wFlagStatus.bSupply;
			}
			//EPO
			m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bEpoPrompt = objSystem.m_st_wFlagStatus.bEPO;
			//ModelID
			m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bModelID = objDigitalIO.m_st_wHardwareNewSignal.bInvID;
			
			/*Power CAN通讯状态*/
//			m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bPwrCANState = 
			
			/* <=====过程切换提示======> */
			if ( objSystem.m_st_wFlagStatus.bIntSwitch == 1 )
			{
				m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bProcSwPrompt = 1;			//旁路间断切换提示
			}
			else if ( objSystem.m_st_wFlagStatus.bSingleRiskOff == 1 )
			{
				m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bProcSwPrompt = 2;			//单机旁路异常关机提示
			}
			else if ( objSystem.m_st_wFlagStatus.bParaRiskOff == 1 )
			{
				m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bProcSwPrompt = 3;			//并机旁路异常关机提示
			}
			else if ( objSystem.m_st_wFlagPrompt.bTotalCapPrompt == 1 )
			{
				m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bProcSwPrompt = 4;			//启动容量不足提示
			}
			else if ( objSystem.m_st_wFlagPrompt.bUnitCapPrompt == 1 )
			{
				m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bProcSwPrompt = 5;			//无法间断切换提示
			}
			else
			{
				m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bProcSwPrompt = 0;			//无任何提示
			}

			if(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16RunMod == 2)
			{
				m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bECOMode = 1;			    //ECO模式
			}
			else
			{
				m_st_InvSendMonitor.m_st_InvBreakerStatus.Word2_U.tWord2Bits.bECOMode = 0;
			}
/****************************逆变上传告警量(第一个字)************************************************/
				/*逆变不同步*/
			
			if (( objSystem.m_st_wFlagStatus.bInv == 3 ) && ( objSystem.m_st_wFlagStatus.bLock == 0 ))
			{
				m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bInvSync = 1;									
			}
			else
			{
				m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bInvSync = 0;
			}
			/*逆变器故障*/ 
			m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bInvFault = objSystem.m_st_wFlagFault.bInvFault;				                     
			/*逆变器过温*/ 
			m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bInvOverTemp = objDigitalIO.m_st_wHardwareNewSignal.bInvHeatSinkOVTemp;
			/*单机输出过载*/
			if(objXfer.m_st_wXferCmd.bSwitchStatus ==1)
			{
				m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bSigOverLoad = objSystem.m_st_wFlagFault.bOvload;
			}
			else
			{
				m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bSigOverLoad = 0;
			}
			//对于20/15K项目只是报系统过载 应wxw要求去掉 2009-9-15
			/*if(MODULE_NUM==2)
			{
				m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bSigOverLoad = 0;
			}*/
			
			/*单机过载超时*/ 											            
			m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bSigOverLoadTimeOut = objSystem.m_st_wFlagFault.bInvOvloadTimeout | objLoad.m_st_wFlagLoad.bShortCircuit;
			/*交流输出过压故障*/									            
//			m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bOutOverVlt 
			/*输出电压异常*/								         
			m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bOutVltFualt = objSystem.m_st_wFlagPrompt.bVoutAbnormal;	
			/*输出直流分量过大*/ 										 
//			m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bDcCurrOffsetOver 	
			/*逆变器过流*/  						        
//			m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bInvOverCurr = objSystem.m_st_wFlagFault.bIGBTOCP;
			/*母线异常关机*/ 												             
			m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bBusAbnormalOff = objSystem.m_st_wFlagPrompt.bDCAbnormalOff;	
			/*直流母线过压故障*/										            
			m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bBusOverVlt =  objSystem.m_st_wFlagFault.bDCOV;	
			/*逆变电感过温*/											         
//			m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bInvInductOverTemp	
				
			/*风扇故障*/							             
			m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bFanFault = objSystem.m_st_wFlagFault.bFanFault;												/*风扇故障*/                 
			/*逆变继电器故障*/
			if ( ( objSwitches.m_st_FlagBreaker.bInvKMShort == 1 ) || ( objSwitches.m_st_FlagBreaker.bInvKMOpen == 1 ) 
					||(objInverter.m_st_FlagVinvABC.bInvCurrUnbalancedWarn == 1))
			{
				m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bInvRelay = 1;								
			}
			else
			{			
				m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bInvRelay = 0;
			}
			/*输出熔丝断*/													          
			m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1Bits.bOutFuseFault = !(objSwitches.m_st_FlagBreaker.bFuse);	
			/*辅助电源2掉电*/ 										               
//			m_st_InvSendMonitor.m_st_InvWarn.Word1_U.tWord1BitsbAuxPwr2Down											

/****************************逆变上传告警量(第二个字)************************************************/	           
			/*用户操作错误*/
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
			//输出短路
			m_st_InvSendMonitor.m_st_InvWarn.Word2_U.tWord2Bits.bOutShortFault = objSystem.m_st_wFlagPrompt.bShortCircuit;
/****************************逆变上传告警量(第三个字)************************************************/
			/*旁路情况(电压或频率)0:normal 1:超保护2:超跟踪*/					
			m_st_InvSendMonitor.m_st_InvWarn.Word3_U.tWord3Bits.bBypState =objSystem.m_st_wFlagStatus.bVbp;	
			/*旁路相序反*/ 	
			m_st_InvSendMonitor.m_st_InvWarn.Word3_U.tWord3Bits.bBypPhaseReverse = objSystem.m_st_wFlagFault.bBpInvtSeq; 
			/*负载冲击转旁路*/
			m_st_InvSendMonitor.m_st_InvWarn.Word3_U.tWord3Bits.bToBypByLoadPuls = objSystem.m_st_wFlagPrompt.bImpactSwitch; 
			/*旁路切换次数限制*/
			m_st_InvSendMonitor.m_st_InvWarn.Word3_U.tWord3Bits.bBypSwTimesLtd = objSystem.m_st_wFlagPrompt.bSwitchTimesClamp;
			/*旁路过流故障*/	
//			m_st_InvSendMonitor.m_st_InvWarn.Word3_U.tWord2Bits.bBypOverCurr	= objSystem.m_st_wFlagFault.bBpOverCurrent;  
			/*旁路晶闸管故障0:normal 1:反灌2:开路3:短路*/ 
			
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

			
			
			/*旁路异常关机告警*/
			m_st_InvSendMonitor.m_st_InvWarn.Word3_U.tWord3Bits.bBypAbnorOff	= objSystem.m_st_wFlagFault.bBpFailOff;
			/*并机板故障*/ 	                
//			m_st_InvSendMonitor.m_st_InvWarn.Word3_U.tWord3Bits. bParaBoardFault	
			/*并机均流故障*/
			m_st_InvSendMonitor.m_st_InvWarn.Word3_U.tWord3Bits. bSysShareCurr = objSystem.m_st_wFlagPrompt.bParaShareFault;
			/*并机线连接故障*/	
//			m_st_InvSendMonitor.m_st_InvWarn.Word3_U.tWord3Bits. bParaLineFault = objSystem.m_st_wFlagPrompt.bParaCableFault;

			/*20K一个机架内只有一个模块,不存在内部通信故障*/ 	
			m_st_InvSendMonitor.m_st_InvWarn.Word3_U.tWord3Bits. bParaCom = 0;
            
            /*并机系统过载*/		
            m_st_InvSendMonitor.m_st_InvWarn.Word3_U.tWord3Bits. bSysOverLoad = objSystem.m_st_wFlagPrompt.bParaOvload;
            /*邻module请求转旁路*/	
            m_st_InvSendMonitor.m_st_InvWarn.Word3_U.tWord3Bits.bToBypReq = objSystem.m_st_wFlagPrompt.bReqSwitch;
/*****************************逆变上传模拟量**********************************************/  
			/*相输出有功功率*/			
	        m_st_InvSendMonitor.m_st_InvAnalog.OutActPwr.u16Phase_a = (UINT16)(objInverter.m_i16PinvaforShow);                                                  	
            m_st_InvSendMonitor.m_st_InvAnalog.OutActPwr.u16Phase_b = (UINT16)(objInverter.m_i16PinvbforShow);
            m_st_InvSendMonitor.m_st_InvAnalog.OutActPwr.u16Phase_c = (UINT16)(objInverter.m_i16PinvcforShow);

			/*相输出视在功率*/
           	 m_st_InvSendMonitor.m_st_InvAnalog.OutApparentPwr.u16Phase_a = (UINT16)(objInverter.m_i16SinvaforShow);
        	m_st_InvSendMonitor.m_st_InvAnalog.OutApparentPwr.u16Phase_b = (UINT16)(objInverter.m_i16SinvbforShow); 
        	m_st_InvSendMonitor.m_st_InvAnalog.OutApparentPwr.u16Phase_c = (UINT16)(objInverter.m_i16SinvcforShow);  

			/*相输出功率因数*/
        	if ( objInverter.m_i16Sinva < Sout5CNST )					//视在功率<5%,功率因数=0
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
            
           	if ( objInverter.m_i16Sinvb < Sout5CNST )					//视在功率<5%,功率因数=0
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
            
            if ( objInverter.m_i16Sinvc < Sout5CNST )					//视在功率<5%,功率因数=0
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

		    /*输出相电压*/
		  	m_st_InvSendMonitor.m_st_InvAnalog.OutPhaseVlt.u16Phase_a = (UINT16)((((INT32)( objOutput.m_unVoutaRms.half.hword )) * m_i16VoltDispScale)>>10);
           	m_st_InvSendMonitor.m_st_InvAnalog.OutPhaseVlt.u16Phase_b = (UINT16)((((INT32)( objOutput.m_unVoutbRms.half.hword )) * m_i16VoltDispScale)>>10);
           	m_st_InvSendMonitor.m_st_InvAnalog.OutPhaseVlt.u16Phase_c = (UINT16)((((INT32)( objOutput.m_unVoutcRms.half.hword )) * m_i16VoltDispScale)>>10);
		 
			/*输出相电流*/
         	m_st_InvSendMonitor.m_st_InvAnalog.OutPHaseCurr.u16Phase_a = (UINT16)( objInverter.m_i16IinvaforShow  );
         	m_st_InvSendMonitor.m_st_InvAnalog.OutPHaseCurr.u16Phase_b = (UINT16)( objInverter.m_i16IinvbforShow  );
        	m_st_InvSendMonitor.m_st_InvAnalog.OutPHaseCurr.u16Phase_c = (UINT16)( objInverter.m_i16IinvcforShow  );

			/*逆变相电压*/ 
           	m_st_InvSendMonitor.m_st_InvAnalog.InvPhaseVlt.u16Phase_a = (UINT16)((((INT32)( objInverter.m_unVinvaRms.half.hword )) * m_i16VoltDispScale)>>10);
			m_st_InvSendMonitor.m_st_InvAnalog.InvPhaseVlt.u16Phase_b = (UINT16)((((INT32)( objInverter.m_unVinvbRms.half.hword )) * m_i16VoltDispScale)>>10);
			m_st_InvSendMonitor.m_st_InvAnalog.InvPhaseVlt.u16Phase_c = (UINT16)((((INT32)( objInverter.m_unVinvcRms.half.hword )) * m_i16VoltDispScale)>>10);

			/*逆变相电流*/
        	m_st_InvSendMonitor.m_st_InvAnalog.InvPhaseCurr.u16Phase_a = (UINT16)( objInverter.m_i16IinvaforShow );
           	m_st_InvSendMonitor.m_st_InvAnalog.InvPhaseCurr.u16Phase_b = (UINT16)( objInverter.m_i16IinvbforShow );
           	m_st_InvSendMonitor.m_st_InvAnalog.InvPhaseCurr.u16Phase_c = (UINT16)( objInverter.m_i16IinvcforShow ); 

			/*电感相电流*/
           	m_st_InvSendMonitor.m_st_InvAnalog.InductCurr.u16Phase_a = (UINT16)( objInverter.m_i16IlaforShow );
           	m_st_InvSendMonitor.m_st_InvAnalog.InductCurr.u16Phase_b = (UINT16)( objInverter.m_i16IlbforShow );
           	m_st_InvSendMonitor.m_st_InvAnalog.InductCurr.u16Phase_c = (UINT16)( objInverter.m_i16IlcforShow ); 

			/*旁路相电压*/
           	m_st_InvSendMonitor.m_st_InvAnalog.BypPhaseVlt.u16Phase_a = (UINT16)((((INT32)( objBypass.m_unVbpaRms.half.hword )) * m_i16VoltDispScale)>>10);
        	m_st_InvSendMonitor.m_st_InvAnalog.BypPhaseVlt.u16Phase_b = (UINT16)((((INT32)( objBypass.m_unVbpbRms.half.hword )) * m_i16VoltDispScale)>>10);
        	m_st_InvSendMonitor.m_st_InvAnalog.BypPhaseVlt.u16Phase_c = (UINT16)((((INT32)( objBypass.m_unVbpcRms.half.hword )) * m_i16VoltDispScale)>>10);

			/*相输出负载百分比*/
        	m_st_InvSendMonitor.m_st_InvAnalog.OutLoadRate.u16Phase_a = (UINT16)( objInverter.m_i16LoadRatea );
        	m_st_InvSendMonitor.m_st_InvAnalog.OutLoadRate.u16Phase_b = (UINT16)( objInverter.m_i16LoadRateb );
        	m_st_InvSendMonitor.m_st_InvAnalog.OutLoadRate.u16Phase_c = (UINT16)( objInverter.m_i16LoadRatec );
        	/*相逆变峰值比*/
        	if ( objInverter.m_i16IinvaPrms < Current5CNST )				//逆变电流<5%,峰值比=1.4
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
		
		if ( objInverter.m_i16IinvbPrms < Current5CNST )				//逆变电流<5%,峰值比=1.4
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
		
		if ( objInverter.m_i16IinvcPrms < Current5CNST )				//逆变电流<5%,峰值比=1.4
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
		
			/*逆变频率*/
		if ( ( objInverter.m_i16VinvaPrms < Voltage80CNST ) || ( objInverter.m_i16VinvbPrms < Voltage80CNST )
				|| ( objInverter.m_i16VinvcPrms < Voltage80CNST ) )
		{
			m_st_InvSendMonitor.m_st_InvAnalog.OutFreq.u16Self = 0;
		}
		else
		{			
			m_st_InvSendMonitor.m_st_InvAnalog.OutFreq.u16Self = (UINT16)( objInverter.m_i16FinvMean );			
		}
			/*旁路频率*/
			//与整流侧逻辑一致，当旁路电压模值低于有效值30V对应的模时三相频率认为是零
		if ( objBypass.m_unVbpMMean_0.half.hword < 790 )
		{	
			m_st_InvSendMonitor.m_st_InvAnalog.BypFreq.u16Self = 0;
		}
		else
		{
			m_st_InvSendMonitor.m_st_InvAnalog.BypFreq.u16Self = (UINT16)( objBypass.m_i16FbpMean );			
		}
			/*逆变测试变量*/
		m_st_InvSendMonitor.m_st_InvAnalog.DbgVar.u16Dbg1 = *pDSPVar1Adr;
		m_st_InvSendMonitor.m_st_InvAnalog.DbgVar.u16Dbg2 = *pDSPVar2Adr;
		m_st_InvSendMonitor.m_st_InvAnalog.DbgVar.u16Dbg3 = *pDSPVar3Adr;   
/*************************************版本信息上传******************************************************/  
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
函数名：	App_MonInterfaceReceive()                                                         
功能描述：	接受来自监控的参数设置及命令                                                   
函数调用	无                                                                             
************************************************************************************/      
	void	Class_MonitorInterface::App_MonInterfaceRead()
	{				
 /********************************系统设置量*************************************************/

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
 			 m_i16ModelKVA = m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16CapType;       	 	/*机型容量类型*/                 
         		 m_i16GrantKVA = m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16CapType;
         	 //系统电压等级设置           	
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
 	 
        	  //工作模式设置 
        	 if(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16RunMod == 1)
        	  {
        	  	m_st_wUPSModeRun.bMode = 0;        	  	
        	  } 
        	  else if(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16RunMod == 2)
        	  {
        	  	m_st_wUPSModeRun.bMode = 1;
        	  }

/* 	 
        	  //单并机工作模式设置
        	  //单机
              if(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16UpsConfigType  == 1)
        	  {
        	  	m_st_wUPSModeRun.bConfig = 0;        	  	
        	  } 

			  // 并机
        	  else if(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16UpsConfigType == 2)
        	  {
        	  	m_st_wUPSModeRun.bConfig = 3;
        	  }
			  //热备份主机
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
              //输出频率等级设置   
             if(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutFreq == 50)
              {
              	m_st_wUPSModeRun.bFreqOut = 0;
              } 
              else if(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutFreq == 60) 
              {
              	m_st_wUPSModeRun.bFreqOut = 1;             
              }
  

			   //自老化输出电流百分比设置 
            if(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16AgingOutCurrRate <= 130)
              {
              	m_i16SelfAgingCurrRate = (INT16)(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16AgingOutCurrRate);
              }  

			   //逆变器工作模式设置
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
			  
			  
			   //输出间断切换时间设置ms 
               if(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutSwitchTime <= 4)
              {
              	m_i16IntSwTimeSet = (INT16)(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutSwitchTime);
              }  
              //输出频率跟踪速率设置  
              if((m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutFreqTrackRate >= 1)
			&&(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutFreqTrackRate <= 30))
              {
              	m_i16FreqSlewRate = (INT16)(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutFreqTrackRate);
              }                    	

			 //逆变器输出功率因数
         	 m_st_wUPSModeRun.bPowerFactor = m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16InvOutFactor;

			  //输出电压微调系数设置                  
         	if((m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutVltMicroAdj >= 263)
			&&(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutVltMicroAdj <= 291))
              {
              	m_i16VrefSetRms = (INT16)(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutVltMicroAdj);
              }
 
         	  //旁路切换次数设置 
         	 if((m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypSwitchTime >= 1)
			  && (m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypSwitchTime <= 10))
              {
              	m_i16BptoinvTimesSet = (INT16)(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypSwitchTime);
              }       

			   //旁路频率跟踪范围设置 
         	 if((m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypFreqTrackLtd >= 1)
			&& (m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypFreqTrackLtd <= 4))
              {
              	m_i16FbpTraceSet = (INT16)(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypFreqTrackLtd-1);
              } 
                  

		 		 //旁路电压保护范围设置(上限)
		 if((m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypVltUpLtd >= 1)
		 	&& (m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypVltUpLtd <= 3))  //旁路电压范围上限可设置为+20%
         {
              m_i16VbpUplimitSet = (INT16)(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypVltUpLtd - 1);              	
         }
 

			  //旁路电压保护范围设置(下限) 
		if((m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypVltDownLtd >= 1)
			&& (m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypVltDownLtd <= 4))
              {
              	m_i16VbpDnlimitSet = (INT16)(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypVltDownLtd-1);
              }
                       
			   //旁路频率保护范围设置 
		if((m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypFreqLtd == 1)
			||(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypFreqLtd == 2))
//			||(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypFreqLtd == 3)
//			||(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypFreqLtd == 4))
              {
              	m_i16FbpLimitSet = (INT16)(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypFreqLtd-1);
              }
 
			 //旁路反灌保护用短路器 			       			  
//			  m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypProtectBrk	 
      
			//机架号
//			m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16RMIdx

              //并机机架数量  
//             m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16RMNum

               //本机基本模块数
               //20k底层逆变采用基本模块数和冗余模块数，但接收的参数来自于并机基本台数和冗余台数
              if(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16RMNum <= 4)
              {
              	m_i16BasicNum = (INT16)(m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16RMNum);
              }
 

			 //本机冗余模块数
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

 /************************************模拟量的接受 ***********************************************/
//环境温度
 		objInverter.m_i16wEnvirTemp = (INT16)m_st_wInvReceiveMonitor.m_st_InvRecvAnalog.u16EnvirTemp;
//机架内并机输出总有功功率
 		//objSystem.m_i16PoutaTotal = (INT16)m_st_wInvReceiveMonitor.m_st_InvRecvAnalog.OutActPwr.u16Phase_a;
		//objSystem.m_i16PoutbTotal = (INT16)m_st_wInvReceiveMonitor.m_st_InvRecvAnalog.OutActPwr.u16Phase_b;
		//objSystem.m_i16PoutcTotal = (INT16)m_st_wInvReceiveMonitor.m_st_InvRecvAnalog.OutActPwr.u16Phase_c;
		objSystem.m_i16PoutaTotal = objPowCANSched.m_i16PoutaTotal;
		objSystem.m_i16PoutbTotal = objPowCANSched.m_i16PoutbTotal;
		objSystem.m_i16PoutcTotal = objPowCANSched.m_i16PoutcTotal;
//机架内并机输出总视在功率
		//objSystem.m_i16SoutaTotal = (INT16)m_st_wInvReceiveMonitor.m_st_InvRecvAnalog.OutApparentPwr.u16Phase_a;
		//objSystem.m_i16SoutbTotal = (INT16)m_st_wInvReceiveMonitor.m_st_InvRecvAnalog.OutApparentPwr.u16Phase_b;
		//objSystem.m_i16SoutcTotal = (INT16)m_st_wInvReceiveMonitor.m_st_InvRecvAnalog.OutApparentPwr.u16Phase_c;
		objSystem.m_i16SoutaTotal = objPowCANSched.m_i16SoutaTotal;
		objSystem.m_i16SoutbTotal = objPowCANSched.m_i16SoutbTotal;
		objSystem.m_i16SoutcTotal = objPowCANSched.m_i16SoutcTotal;
		
/////////////////////////////////////////////////////////////////////////////
//机架间并联并机系统输出总有功功率
		//By wxw20090725 Adator15k~20kPowerCan代替监控传递负载大小
        objSystem.m_i16RackPoutaTotal =  objPowCANSched.m_i16PoutaTotal;     
		objSystem.m_i16RackPoutbTotal =  objPowCANSched.m_i16PoutbTotal;
		objSystem.m_i16RackPoutcTotal =  objPowCANSched.m_i16PoutcTotal;
//机架间并联并机系统输出总视在功率
		objSystem.m_i16RackSoutaTotal =  objPowCANSched.m_i16SoutaTotal;     
		objSystem.m_i16RackSoutbTotal =  objPowCANSched.m_i16SoutbTotal;
		objSystem.m_i16RackSoutcTotal =  objPowCANSched.m_i16SoutcTotal;
/************************************旁路柜空开及维修空开 ***********************************************/		
//内部维修空开
	//	objDigitalIO.m_st_HardwareSignal.bQ3BP = m_st_wBypBreakData.Word1_U.tWord1Bits.bQ3Bp;
//旁路柜维修空开
	//	objDigitalIO.m_st_wLocalNewSignal.bCAB_Q3BP_STATUS = m_st_wBypBreakData.Word1_U.tWord1Bits.bExyQ3Bp;
//旁路柜输出空开
	//	objDigitalIO.m_st_wLocalNewSignal.bEXT_OUT_STATUS = m_st_wBypBreakData.Word1_U.tWord1Bits.bQ5;

 /************************************监控侧加工的系统状态 ***********************************************/
//系统旁路超保护状态
 		objBypass.m_st_FlagVbpTotal.bBpTptalProtect =  m_st_MonRealMsg.Word1_U.tWord1Bits.bBypOverProtectState;
 
 //电池自检状态
		//objPowCANSched.m_st_BatteryStatus.bBattSelfTest = m_st_MonRealMsg.Word1_U.tWord1Bits.bBatSeftTest;
 		if (m_u16BatDeepSelfRecved)
		{
			objPowCANSched.m_st_BatteryStatus.bBattSelfTest	= m_st_MonRealMsg.Word1_U.tWord1Bits.bBatSeftTest;
			
		}

		else
		{
			objPowCANSched.m_st_BatteryStatus.bBattSelfTest	= 0;
		}
/**********************************监控校正设置****************************************/
		if(m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh != 0)
		{
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_a >= 3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_a <= 4506))
			{	
				 m_i16KsVoutaSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_a;	        //A相输出电压校正系数 (4096)
			}
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_b >= 3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_b <= 4506))
			{	
				 m_i16KsVoutbSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_b;	        //B相输出电压校正系数 (4096)
			}
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_c >= 3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_c <= 4506))
			{	
				 m_i16KsVoutcSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_c;	        //C相输出电压校正系数 (4096)
			}	
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_a >= 3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_a <= 4506))
			{	
				 m_i16KsIoutaSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_a;	        //A相输出电流校正系数 (4096)
			}
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_b >= 3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_b <= 4506))
			{	
				 m_i16KsIoutbSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_b;	        //B相输出电流校正系数 (4096)
			}
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_c>=3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_c<=4506))
			{	
				 m_i16KsIoutcSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_c;	        //C相输出电流校正系数 (4096)
			}  
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_a >= 3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_a <= 4506))
			{	
				 m_i16KsVinvaSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_a;	        //A相逆变电压校正系数 (4096)
			}
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_b>=3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_b<=4506))
			{	
				 m_i16KsVinvbSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_b;	        //B相逆变电压校正系数 (4096)
			}
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_c >= 3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_c <= 4506))
			{	
				 m_i16KsVinvcSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_c;	        //C相逆变电压校正系数 (4096)
			}  
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_a >= 3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_a <= 4506))
			{	
				 m_i16KsIinvaSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_a;	        //A相逆变电流校正系数 (4096)
			}
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_b >= 3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_b <= 4506))
			{	
				 m_i16KsIinvbSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_b;	        //B相逆变电流校正系数 (4096)
			}
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_c >= 3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_c <= 4506))
			{	
				 m_i16KsIinvcSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_c;	        //C相逆变电流校正系数 (4096)
			} 
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_a >= 3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_a <= 4506))
			{	
				 m_i16KsVbpaSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_a;	        //A相旁路电压校正系数 (4096)
			}
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_b >= 3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_b <= 4506))
			{	
				 m_i16KsVbpbSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_b;	        //B相v电压校正系数 (4096)
			}
			if((m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_c >= 3686)
				&& (m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_c <= 4506))
			{	
				 m_i16KsVbpcSet = m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_c;	        //C相旁路电压校正系数 (4096)
			} 

/***********************************逆变调试变量地址*************************************/		
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

		//2017.06.10 逆变调试参数
		//		if((m_st_RecSet_UD.SetParam.u16DegVar1	 >= 3686)//2017.06.10不需要设置范围
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
/**********************************接受系统命令***************************************/
         if(m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16FaultClrCmd == 1)
         {
         	m_st_wMonitorOrder.bReset =1;						//故障消除
         	m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16FaultClrCmd = 0;
         }
         if(m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16ManualInvOnCmd == 1)
         {
         	m_st_wMonitorOrder.bOn =1;							//开机
         	m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16ManualInvOnCmd = 0;
         }
         if(m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16ManulaInvOffCmd == 1)
         {
         	m_st_wMonitorOrder.bOff =1;							//关机(手动关机)
         	m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16ManulaInvOffCmd = 0;
         }
         if(m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16EpoCmd == 1)
         {
         	m_st_wMonitorOrder.bEPO =1;							//紧急关机(关旁路)
         	m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16EpoCmd = 0;
         }
         if(m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16EodCmd == 1)
         {
         	m_st_wMonitorOrder.bInstantOff =1;					//关机(电池模块请求关逆变)
         	m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16EodCmd = 0;
         }
         if(m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16OutputDisableCmd ==1)
         {
         	m_st_wMonitorOrder.bOutDisable =1;					//输出禁止(直接管输出)
         	m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16OutputDisableCmd = 0;				
         }
         else if(m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16OutputEnableCmd == 1 )
         {
         	m_st_wMonitorOrder.bOutEnable =1;					//输出允许(直接放开输出)
         	m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16OutputEnableCmd = 0;
         }         
         if(m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16AbnrmlShutConfirmCmd == 1)
         {
         	m_st_wMonitorOrder.bRiskOffConf =1;					//确认异常关机
         	m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16AbnrmlShutConfirmCmd = 0;
         }
         else if(m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16AbnrmllShutCancelCmd == 1)
         {
         	m_st_wMonitorOrder.bRiskOffConf =2;					//取消异常关机
         	m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16AbnrmllShutCancelCmd = 0;
         }
         if(m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16IndirectSwitchCmd == 1)
         {
         	m_st_wMonitorOrder.bIntSwitchConf =1;				//间断切换确认信息回应
         	m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16IndirectSwitchCmd =0;
         }
         if(m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16InvFwUpdateCmd == 1)
         {
         	m_st_wMonitorOrder.bFlashUpdate =1;					//START FLASH UPDATE
         	if(objDigitalIO.m_un_ExInARegConf.BIT.bEPO == 0)  //   EPO 逆变固件升级
		{				
			// 关中断，禁止狗
			DINT;
			objWatchDogDrv.Drv_DisableDog();

			//拷贝API函数至RAM
			objRAMDrv.Drv_MemCopy(&Flash28_API_LoadStart, &Flash28_API_LoadEnd, &Flash28_API_RunStart);

			objFlashUpdate.FlagProgram();
						
			m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16InvFwUpdateCmd =0;	// 清升级命令
			
			if(objSystem.m_u16DSPRevision >= 3)			//版本号为C及以上的DSP才喂狗
			{
				objWatchDogDrv.Drv_EnableDog();
			}
				
			while(1)
			{
				asm(" NOP ");						//死循环,不喂狗复位DSP
			}
			
		}         	
		else
		{
			m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16InvFwUpdateCmd =0;	// 清升级命令
		}
         }     

	//---增加电池相关命令
	if (1 == m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16SelfChkDeepCmd)
	{
		m_u16BatDeepSelfRecved = 1;

		//清命令
		m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16SelfChkDeepCmd = 0;
	}

	//维护自检命令
	 if (1 == m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16SelfChk20PercentCmd)
	{
		m_u16BatDeepSelfRecved = 0;

		//清命令
		m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16SelfChk20PercentCmd = 0;
	}

	 //自检结束
	  if (1 == m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16SelfChkEndCmd)
	{
		m_u16BatDeepSelfRecved = 0;

		//清命令
		m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16SelfChkEndCmd = 0;
	}
	 
	
	//----------------------------
	

		//20K项目中增加 wenbo 090610
		//主路反灌信号

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
函数名：	App_MonInterfaceWrite()
功能描述：	发送状态，告警，模拟量给监控
函数调用	无
************************************************************************************/
	void	Class_MonitorInterface::Byp_App_MonInterfaceWrite()
	{

/**************************************旁路上传状态变量**************************************************/
			//BCB连接状态
			m_st_BypSendMonitor.m_st_BypBreakerStatus.Word1_U.tWord1Bits.bBCBConnect = 0;
			//无旁路模块,屏蔽旁路供电状态的显示
			m_st_BypSendMonitor.m_st_BypBreakerStatus.Word1_U.tWord1Bits.bBpSupplyState =2;
			
			//EPO
			m_st_BypSendMonitor.m_st_BypBreakerStatus.Word1_U.tWord1Bits.bEpoPrompt = objSystem.m_st_wFlagStatus.bEPO;

			//维修空开状态
			m_st_BypSendMonitor.m_st_BypBreakerStatus.Word1_U.tWord1Bits.bBypMaintBreak = objSwitches.m_st_FlagBreaker.bQ3bp;

            //
            m_st_BypSendMonitor.m_st_BypBreakerStatus.Word1_U.tWord1Bits.bBypOnlinestate = objMonInterface.m_i16_DegVar1;
			//故障记录上传请求
//			m_st_BypSendMonitor.m_st_BypBreakerStatus.Word1_U.tWord1Bits.bRecordTxRequest = objSystem.m_st_wFlagSystem.bRecordTxRequest;
/****************************旁路上传告警量************************************************/
			//旁路过流
			m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bBypOverCurr = objSystem.m_st_wFlagFault.bBpOverCurrent | objBypass.m_st_FlagIbpTotal.bIbpOverTimeOut; 
			//旁路反灌
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

			//邻机旁路晶闸管故障
			if((objSwitches.m_st_FlagBreaker.bBpSTSShort == 1) && (objSwitches.m_st_FlagBreaker.bReqBpSTSShort == 1))
			{
				m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bReqBypScrFault = 1;
			}
			
			//风扇故障
			m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bFanFault =0;
			//电池接地故障
			m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bBatEarthFault =0;
			//电池房环境异常
			m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bBatRoomEnvirAbnormal =0;
			//旁路模块过温
			m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bModOvTemp = 0;
			//辅助电源1掉电
			m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bAuxPwr1Down = 0 ;
			//辅助电源2掉电
			m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bAuxPwr2Down = 0 ;
			//输出过载
			//m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bOvload =objSystem.m_st_wFlagFault.bOvload|objSystem.m_st_wFlagPrompt.bParaOvload;
			
			//m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bOvload =objSystem.m_st_wFlagPrompt.bParaOvload;
			
			//Power ok故障
//			m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bPowOK = objSystem.m_st_wFlagPrompt.bPow15vFault;
			/*旁路情况(电压或频率)0:normal 1:超保护2:超跟踪*/
			m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bBypState = objSystem.m_st_wFlagStatus.bVbp;
			/*旁路相序反*/ 
			m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bBypPhaseReverse = objSystem.m_st_wFlagFault.bBpInvtSeq;

			/*并机通信故障*/
			m_st_BypSendMonitor.m_st_BypWarn.Word1_U.tWord1Bits.bParallelCommErr = objPowCANSched.PowCanParaInfoEntry.UpsOnlineInfo.uOnlineFlag.tBits.bSysCommerr;

/*****************************旁路上传模拟量**********************************************/  
			/*旁路相电压*/	
			m_st_BypSendMonitor.m_st_BypAnalog.BypPhaseVlt.u16Phase_a =	(UINT16)((((INT32)( objBypass.m_unVbpaRms.half.hword )) * m_i16VoltDispScale)>>10);			
			m_st_BypSendMonitor.m_st_BypAnalog.BypPhaseVlt.u16Phase_b =	(UINT16)((((INT32)( objBypass.m_unVbpbRms.half.hword )) * m_i16VoltDispScale)>>10);			
			m_st_BypSendMonitor.m_st_BypAnalog.BypPhaseVlt.u16Phase_c =	(UINT16)((((INT32)( objBypass.m_unVbpcRms.half.hword )) * m_i16VoltDispScale)>>10);			
			/*旁路相电流*/
			m_st_BypSendMonitor.m_st_BypAnalog.BypPhaseCurr.u16Phase_a = (UINT16)( objOutput.m_i16IoutaforShow );
			m_st_BypSendMonitor.m_st_BypAnalog.BypPhaseCurr.u16Phase_b = (UINT16)( objOutput.m_i16IoutbforShow );
			m_st_BypSendMonitor.m_st_BypAnalog.BypPhaseCurr.u16Phase_c = (UINT16)( objOutput.m_i16IoutcforShow );
			//输出相电压
			m_st_BypSendMonitor.m_st_BypAnalog.BypOutPhaseVlt.u16Phase_a = (UINT16)((((INT32)( objOutput.m_unVoutaRms.half.hword )) * m_i16VoltDispScale)>>10);
			m_st_BypSendMonitor.m_st_BypAnalog.BypOutPhaseVlt.u16Phase_b = (UINT16)((((INT32)( objOutput.m_unVoutbRms.half.hword )) * m_i16VoltDispScale)>>10);
			m_st_BypSendMonitor.m_st_BypAnalog.BypOutPhaseVlt.u16Phase_c = (UINT16)((((INT32)( objOutput.m_unVoutcRms.half.hword )) * m_i16VoltDispScale)>>10);
			//输出功率因数
			if ( objOutput.m_i16Souta < Sout5CNST )					//视在功率<5%,功率因数=0
			{			
				m_st_BypSendMonitor.m_st_BypAnalog.OutFactor.u16Phase_a = 0;
			}
			else if(objSystem.m_st_wFlagSystem.bSelfAgingAction == 1)//自老化时功率因数为1
			{
				m_st_BypSendMonitor.m_st_BypAnalog.OutFactor.u16Phase_a =100;
			}
			else
			{
				m_st_BypSendMonitor.m_st_BypAnalog.OutFactor.u16Phase_a = (UINT16)( objOutput.m_i16PFouta );

			}
			if ( objOutput.m_i16Soutb < Sout5CNST )					//视在功率<5%,功率因数=0
			{			
				m_st_BypSendMonitor.m_st_BypAnalog.OutFactor.u16Phase_b = 0;
			}
			else if(objSystem.m_st_wFlagSystem.bSelfAgingAction == 1)//自老化时功率因数为1
			{
				m_st_BypSendMonitor.m_st_BypAnalog.OutFactor.u16Phase_b =100;
			}
			else
			{
				m_st_BypSendMonitor.m_st_BypAnalog.OutFactor.u16Phase_b = (UINT16)( objOutput.m_i16PFoutb );

			}
			if ( objOutput.m_i16Soutc < Sout5CNST )					//视在功率<5%,功率因数=0
			{			
				m_st_BypSendMonitor.m_st_BypAnalog.OutFactor.u16Phase_c = 0;
			}
			else if(objSystem.m_st_wFlagSystem.bSelfAgingAction == 1)//自老化时功率因数为1
			{
				m_st_BypSendMonitor.m_st_BypAnalog.OutFactor.u16Phase_c =100;
			}
			else
			{
				m_st_BypSendMonitor.m_st_BypAnalog.OutFactor.u16Phase_c = (UINT16)( objOutput.m_i16PFoutc );

			}
			//输出频率
			if ( ( objOutput.m_i16VoutaPrms < Voltage80CNST ) || ( objOutput.m_i16VoutbPrms < Voltage80CNST )
				|| ( objOutput.m_i16VoutcPrms < Voltage80CNST ) )
			{
				m_st_BypSendMonitor.m_st_BypAnalog.BypOutFreq.u16Self = 0;
			}
			else
			{
				m_st_BypSendMonitor.m_st_BypAnalog.BypOutFreq.u16Self = (UINT16)(objOutput.m_i16FoutMean);
			}
			/*旁路频率*/ 
			if ( objBypass.m_unVbpMMean_0.half.hword < 790 )
			{                                     
				  m_st_BypSendMonitor.m_st_BypAnalog.BypFreq.u16Self = 0;
			}
			else
			{
				 m_st_BypSendMonitor.m_st_BypAnalog.BypFreq.u16Self = (UINT16)( objBypass.m_i16FbpMean );
			}
			//输出负载百分比
			m_st_BypSendMonitor.m_st_BypAnalog.OutLoadRate.u16Phase_a = (UINT16)( objOutput.m_i16LoadRatea );
			m_st_BypSendMonitor.m_st_BypAnalog.OutLoadRate.u16Phase_b = (UINT16)( objOutput.m_i16LoadRateb );
			m_st_BypSendMonitor.m_st_BypAnalog.OutLoadRate.u16Phase_c = (UINT16)( objOutput.m_i16LoadRatec );
			//输出峰值比
			if ( objOutput.m_i16IoutaPrms < Current5CNST )				//输出电流<5%,峰值比=1.4
			{
				m_st_BypSendMonitor.m_st_BypAnalog.OutPeakRate.u16Phase_a = 0;
			}
			//未考虑自老化
			else
			{
				m_st_BypSendMonitor.m_st_BypAnalog.OutPeakRate.u16Phase_a =(UINT16)((((INT32)( objOutput.m_u16IoutaCrest_0 )) * 10)>>8);
			}
			if ( objOutput.m_i16IoutbPrms < Current5CNST )				//输出电流<5%,峰值比=1.4
			{
				m_st_BypSendMonitor.m_st_BypAnalog.OutPeakRate.u16Phase_b = 0;
			}
			//未考虑自老化
			else
			{
				m_st_BypSendMonitor.m_st_BypAnalog.OutPeakRate.u16Phase_b =(UINT16)((((INT32)( objOutput.m_u16IoutbCrest_0 )) * 10)>>8);
			}
			if ( objOutput.m_i16IoutcPrms < Current5CNST )				//输出电流<5%,峰值比=1.4
			{
				m_st_BypSendMonitor.m_st_BypAnalog.OutPeakRate.u16Phase_c = 0;
			}
			//未考虑自老化
			else
			{
				m_st_BypSendMonitor.m_st_BypAnalog.OutPeakRate.u16Phase_c =(UINT16)((((INT32)( objOutput.m_u16IoutcCrest_0 )) * 10)>>8);
			}

			/*环境温度*/
			//m_st_BypSendMonitor.m_st_BypAnalog.EnvirTemp.u16Self = 0;//改从整流取
			//电池温度  
			//m_st_BypSendMonitor.m_st_BypAnalog.BatteryInTemp.u16Self =(UINT16) (objSystem.m_unTmpBatIn.half.hword);  
			//m_st_BypSendMonitor.m_st_BypAnalog.BatteryInTemp.u16BattInTemp =0;
			//m_st_BypSendMonitor.m_st_BypAnalog.BatteryInTemp.u16BattOutTemp =0;
			//输出有功功率
			m_st_BypSendMonitor.m_st_BypAnalog.OutActPwr.u16Phase_a = (UINT16)(objOutput.m_i16Pouta);
                     m_st_BypSendMonitor.m_st_BypAnalog.OutActPwr.u16Phase_b = (UINT16)(objOutput.m_i16Poutb);
         		m_st_BypSendMonitor.m_st_BypAnalog.OutActPwr.u16Phase_c =  (UINT16)(objOutput.m_i16Poutc); 

			//ljie check
			/*m_st_ParaRackData[m_i16RMIdx].m_i16Pouta= (UINT16)(objOutput.m_i16Pouta);
                     m_st_ParaRackData[m_i16RMIdx].m_i16Poutb = (UINT16)(objOutput.m_i16Poutb);
         		m_st_ParaRackData[m_i16RMIdx].m_i16Poutc =  (UINT16)(objOutput.m_i16Poutc); 
			//输出视在功率
			m_st_ParaRackData[m_i16RMIdx].m_i16Souta= (UINT16)(objOutput.m_i16Souta);
			m_st_ParaRackData[m_i16RMIdx].m_i16Soutb =(UINT16)(objOutput.m_i16Soutb);
			m_st_ParaRackData[m_i16RMIdx].m_i16Soutc=(UINT16)(objOutput.m_i16Soutc);*/

			m_st_BypSendMonitor.m_st_BypAnalog.OutApparentPwr.u16Phase_a = (UINT16)(objOutput.m_i16Souta);
			m_st_BypSendMonitor.m_st_BypAnalog.OutApparentPwr.u16Phase_b =(UINT16)(objOutput.m_i16Soutb);
			m_st_BypSendMonitor.m_st_BypAnalog.OutApparentPwr.u16Phase_c =(UINT16)(objOutput.m_i16Soutc);

			//并机有功功率
			m_st_BypSendMonitor.m_st_BypAnalog.RackOutActPwr.u16Phase_a = (UINT16)(objSystem.m_i16PoutaTotal);
			m_st_BypSendMonitor.m_st_BypAnalog.RackOutActPwr.u16Phase_b = (UINT16)(objSystem.m_i16PoutbTotal);
			m_st_BypSendMonitor.m_st_BypAnalog.RackOutActPwr.u16Phase_c = (UINT16)(objSystem.m_i16PoutcTotal);

			//并机视在功率
			m_st_BypSendMonitor.m_st_BypAnalog.RackOutApparentPwr.u16Phase_a = (UINT16)(objSystem.m_i16SoutaTotal);
			m_st_BypSendMonitor.m_st_BypAnalog.RackOutApparentPwr.u16Phase_b = (UINT16)(objSystem.m_i16SoutbTotal);
			m_st_BypSendMonitor.m_st_BypAnalog.RackOutApparentPwr.u16Phase_c = (UINT16)(objSystem.m_i16SoutcTotal);


			/*旁路测试变量*/
			//m_st_BypSendMonitor.m_st_BypAnalog.BypDebugVari.u16Dbg1 = 0;
			//m_st_BypSendMonitor.m_st_BypAnalog.BypDebugVari.u16Dbg2 = 0;
			//m_st_BypSendMonitor.m_st_BypAnalog.BypDebugVari.u16Dbg3 = 0;   
     }		
//===========================================================================
// End of file.                                    		
//===========================================================================
