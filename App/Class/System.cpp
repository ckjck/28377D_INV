/*==========================================================*/ 	
/* Title		:	System.cpp													*/ 	
/* Description	: 	Class_System definition											*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#include "System.h"

/************************************************************************************
函数名：App_SysParamConfig()
************************************************************************************/
	void	Class_System::App_SysParamConfig(void)
	{
		m_i161DivGrantRate=((INT32)37*1024)/objMonInterface.m_i16GrantKVA;	//授权率的倒数 6.10 format  
		UPDNLMT16(m_i161DivGrantRate,4096,512);														//m_i16ModelKVA=(1~4)m_i16wGrantKVA
		
		//zqf 061225--增加对m_i161DivGrantKVA的更新（CAN中使用）
		m_i161DivGrantKVA = 3072/objMonInterface.m_i16GrantKVA;         								//注意m_i161DivGrantKVA的范围，目前没有范围维护					
	} 
	
/************************************************************************************
函数名：	App_SysVarInit()	
功能描述：	System类变量初始化		
函数调用：	
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
函数名：	App_ModelOnline()	
功能描述：	System类变量初始化		
函数调用：	
***********************************************************************************/
void	Class_System::App_ModelOnline(void)
{
	if(m_st_wFlagFault.bAuxPS2Fault ==0)     					
	{
		//功率模块在线标志，传送至旁路，接管旁路控制权                                                    											
    		objDigitalIO.m_st_wLocalSignal.bModelOnline = 1;   
		
	}
	else					//辅助电源掉电后退出对旁路的控制
	{			
    		objDigitalIO.m_st_wLocalSignal.bModelOnline = 0;   
	}
}


/************************************************************************************
函数名：	vOper_Fault()
功能描述：	操作故障判断模块
函数调用：	无			
************************************************************************************/
	void	Class_System::Alg_OperFault(void)
	{
		//zqf 060912--检测板上的bInterlock信号含义变化（Q5和Q3bp同时闭合），软件上补逆变器开启的条件
		if ((( objInverter.m_st_wFlagInv.bInv != 0 ) && ( objSwitches.m_st_FlagBreaker.bQ3bp == 1 ) 
			&& ( objSwitches.m_st_FlagBreaker.bQ5 == 1 ))
			|| ( ( objMonInterface.m_st_wUPSModeRun.bConfig == 3 ) && ( objSwitches.m_st_FlagBreaker.bQ3bp == 1 ) 
			&& ( objParallel.m_st_wParaSignalF.bInvBpStatusF == 1 ) ) 
			//zqf 060413--后台TEST模式设置下，当内部输出空开合上时，逻辑上退出TEST模式，并报用户操作错误
			|| objParallel.m_st_wParaMaint.bTestAbnormal == 1 )
		{
			m_st_wFlagFault.bOperFault = 1;																//置操作故障位
			objInvCtrl.m_st_wFlagInvOnoff.bManualRecoverOff = 1;										//置可人为恢复故障关机
		}                                                                                   			
		else                                                                                			
		{                                                                                   			
			m_st_wFlagFault.bOperFault = 0;																//清操作故障位
		}
	}
	
/************************************************************************************
函数名：	vSystem_Fault()
功能描述：	系统故障(风扇/电源)判断模块
函数调用：	无			
************************************************************************************/
	void	Class_System::Alg_SysFault(void)
	{
		
		//POW_OK延时确认
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
			m_st_wFlagPrompt.bPow15vFault = 1;	//置POW_OK故障位
			m_st_wFlagFault.bAuxPS2Fault =1;			
			objInvCtrl.m_st_wFlagInvOnoff.bManualRecoverOff = 1;										//置可人为恢复关机位
			objPWMDrv.Drv_PWMInactive();	                                   							// PWM输出强制为高阻态
		}
		else
		{
			m_st_wFlagPrompt.bPow15vFault = 0;
			m_st_wFlagFault.bAuxPS2Fault =0;			//2//清 POW_Fault故障位

			//zqf051020:POWOK 从异常到正常时，补发故障清除命令－具体的作用信号由整流发出			
			if((m_st_wFlagStatus.bPow15vFaultBak == 1) && (objInvCtrl.m_i16FlagOffReason < 5) )
			{
				objMonInterface.m_st_wMonitorOrder.bReset = 1;
			}
		}

		m_st_wFlagStatus.bPow15vFaultBak = m_st_wFlagPrompt.bPow15vFault;

		//为直流风扇，和是否有输出无关，且故障信号为低电平
		if(m_st_wFlagSystem.bTimer1ms == 1)
		{
			if ( (objDigitalIO.m_st_HardwareSignal.bFanFault == 1) && (objDCBus.m_i16Vdc_0>=2528) )	//风扇故障
			{
				m_i16FanFaultCntPer1ms++;																//风扇故障确认计数器递增
				if ( m_i16FanFaultCntPer1ms >= 3000 )													//3s
				{
					m_i16FanFaultCntPer1ms = 3000;													//限幅于3000
					m_st_wFlagFault.bFanFault = 1;												//置风扇故障位
				}
			}
			else
			{				
				m_i16FanFaultCntPer1ms = 0;									//限幅于0
				m_st_wFlagFault.bFanFault = 0;							//清风扇故障位
			}
		}
	}
	
/************************************************************************************
函数名：	vDSPID_Check()			
功能描述：	逆变标志处理模块
函数调用：	无	
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
				
		if(u16DSPErr == 1)								//逆变软硬件不匹配
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
函数名：   vCPLD_Ver()
功能描述： 逆变CPLD版本确定
函数调用： 无
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
函数名：   App_Module_Ok()
功能描述： 模块准备ok确定
函数调用： 无
************************************************************************************/
	void	Class_System::App_Module_Ok()
{
	if(m_st_wFlagSystem.bTimer1ms == 1)
	{
		if(objDigitalIO.m_st_wHardwareNewSignal.bModuleReady == 1)
		{
			m_i16ModuleOkCnt++;
			if(m_i16ModuleOkCnt>=2000)				//ModuleReady信号正常确认2s
			{
				m_i16ModuleOkCnt = 2000;
				objDigitalIO.m_st_wLocalSignal.bModuleReadyOk = 1;
			}
			m_i16ModuleNotOkCnt = 0;
		}
		else
		{
			m_i16ModuleNotOkCnt++;
			if(m_i16ModuleNotOkCnt >= 5)			//ModuleReady信号不正常确认5ms
			{
				m_i16ModuleNotOkCnt = 5;
				objDigitalIO.m_st_wLocalSignal.bModuleReadyOk = 0;
			}
			m_i16ModuleOkCnt = 0;
		}
		
	}
	 //模块未就绪且逆变已开启则逆变手动可恢复关机
	
	if((objDigitalIO.m_st_wLocalSignal.bModuleReadyOk  == 0)&&(objInverter.m_st_wFlagInv.bInv != 0))

	{
		objInvCtrl.m_st_wFlagInvOnoff.bManualRecoverOff=1;

	}

}
/***********************************************************************************
函数名：   App_Module_LED()
功能描述： 模块led灯显示处理
函数调用： 无
************************************************************************************/
void	Class_System::App_Module_LED()
{
	if((objSystem.m_st_wFlagFault.bInvFault  == 1)||(objSystem.m_st_wFlagPrompt.bShortCircuit == 1)				//严重故障
		||(objSwitches.m_st_FlagBreaker.bBpSTSShort  == 1)||(objSwitches.m_st_FlagBreaker.bInvKMShort == 1)
		||(objSystem.m_st_wFlagFault.bDCAbnormal == 1)||(objDigitalIO.m_st_wLocalNewSignal.bRECFault ==2)
		||(objDigitalIO.m_st_wLocalSignal.bModuleReadyOk ==0) || (objMonInterface.m_st_IdErrorFlag.bUsIdNumExpire == 1)
		||(objMonInterface.m_st_IdErrorFlag.bUsIdNumOverlap == 1) ||(objSystem.m_st_wFlagFault.bBypFeedBackFault == 1))
	{
		objDigitalIO.m_st_wLocalSignal.bFaultInd = 0;						//红灯亮
	}
	else if((objSystem.m_st_wFlagPrompt.bParaShareFault == 1)||(objLoad.m_st_wFlagLoad.bOvload==1)
		||(objSwitches.m_st_FlagBreaker.bInvKMOpen == 1)||(objSwitches.m_st_FlagBreaker.bBpSTSOpen == 1)
		||(objSwitches.m_st_FlagBreaker.bFuse == 0)||(objDigitalIO.m_st_wLocalNewSignal.bRECFault  == 1)
		||(objInverter.m_st_FlagVinvABC.bInvCurrUnbalancedWarn == 1)||(m_st_wFlagFault.bFanFault == 1))			//并机均流故障，单机输出过载，逆变继电器开路旁路SCR开路及整流一般故障
	{
		
			if(objDigitalIO.m_st_wLocalSignal.bFaultInd ==1)			//红灯闪烁
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

			}													//红灯及绿灯均亮，输出能供电										
	}
	else
	{

		objDigitalIO.m_st_wLocalSignal.bFaultInd = 1;						//红灯不亮，绿灯亮，有输出，模块无故障				
	}
//绿灯显示	
	if(objXfer.m_st_XferResult.bInvKM == 1)							//输出有电则绿灯长亮
	{
		objDigitalIO.m_st_wLocalSignal.bNormalInd = 0;				
	}
	 else if(objInverter.m_st_wFlagInv.bInv!=0)   //软启动过程中绿灯一直闪烁
	{
		if(objDigitalIO.m_st_wLocalSignal.bNormalInd ==1)			//绿灯闪烁
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
	else if(objInverter.m_st_wFlagInv.bInv ==0)     			//逆变不开机绿灯长灭
	{
		objDigitalIO.m_st_wLocalSignal.bNormalInd = 1;
	 }
	

}
//===========================================================================
// End of file.
//===========================================================================
