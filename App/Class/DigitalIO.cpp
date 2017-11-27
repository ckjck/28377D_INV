/*==========================================================*/ 	
/* Title		:	DigitalIO.cpp													*/ 	
/* Description	: 	Class_DigitalIO definition										*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#include "DigitalIO.h"

/************************************************************************************
函数名：	App_DigitalIOVarInit()	
功能描述：	DigitalIO类变量初始化		
函数调用：	
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
函数名：	App_IMCCtrl()	
功能描述：IMC机架间离散信号控制		
函数调用：	
***********************************************************************************/
	 void	Class_DigitalIO::App_IMCCtrl(void)
	{
		//机架间IMC 允许控制
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
//函数名称：    Dat_Int_MainInterfaceOut()   
//功能描述：    完成中断对主程序控制变量的输出
************************************************************************************/

#pragma CODE_SECTION(".Epwm1Funcs") 			

void 	Class_DigitalIO::Dat_Int_MainInterfaceOut(void)
{	
	INT16	i16IntAx_0;
	objInvAlg.m_i16wVrefActMd_0 = objInvAlg.m_i16wVrefRunMd_0;
	//建立输出信号与SPI输出变量之间的映射关系
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
	//zxj 060429--注意objParallel.m_st_wParaSignalBak中定义的送给整流器的3个IO信号不能屏蔽
	i16IntAx_0=S2U(objParallel.m_st_wParaSignalBak);
	if((objParallel.m_st_wParaMaint.bParaSignalMask == 1 )|| (objMonInterface.m_st_wUPSModeRun.bConfig == 0))
	{
		//考虑单机下不能屏蔽LBS信号，为了简化代码，信号隔离态下也作相同处理
		//(信号隔离态下，已经在主程序中将LBS信号作屏蔽处理，即逻辑上不发脉冲)
		//bJcontrol信号由主程序控制
		//By wxw20091026	单机下system_unlock位被误清零，导致旁路超跟踪下，逆变器却跟踪旁路
		//导致本振时输出的频率为52Hz或48Hz
		//By wxw20091209	单机下bInvOnStandby位被误清零，逆变冲击负载回切时，误走间断切换，
		//i16IntAx_0 = i16IntAx_0 & 0xF810;
		//By wxw20100302 单机下需放开LBS信号
		//i16IntAx_0 = i16IntAx_0 & 0xF816;
		i16IntAx_0 = i16IntAx_0 & 0xF8D6;
	}
	if(objSystem.m_st_wFlagSystem.bUPSTestMode == 1)		
	{
		//调测模式，只发bInvOverload位、送给整流器的3个IO信号，bJcontrol信号由主程序控制
		i16IntAx_0 = i16IntAx_0 & 0xC811;
	}
		
	if(objSystem.m_st_wFlagSystem.bWDStart==1)
	{
		S2U(m_st_wLocalSignal) = S2U(m_st_wLocalSignal)^0x0080;
	}

	//旁路反灌脱扣命令
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
//函数名称：    Dat_Int_MainInterfaceIn()   
//功能描述：    完成中断对主程序控制变量的输入
************************************************************************************/

#pragma CODE_SECTION(".Epwm1Funcs") 			

void 	Class_DigitalIO::Dat_Int_MainInterfaceIn(void)
{

	//zqf 060414--CR40
	//信号隔离态、单机及调测模式屏蔽并机信号，(信号隔离态及调测模式在主程序逻辑处理中均会
	//对收到的LBS信号进行屏蔽，在此为简化代码与单机处理一致，不做屏蔽)
	if((objParallel.m_st_wParaMaint.bParaSignalMask == 1) || ( objSystem.m_st_wFlagSystem.bUPSTestMode == 1))
	{
		//BIT0~10赋值与objParallel.m_st_wParaSignal相同,BIT11~15 bExtOutStatusF，bCabQ3BpStatusF，bLBSDetect，bJControl,bConnectOKF保持不变
		S2U(objParallel.m_st_wParaSignalF) = (((S2U(objParallel.m_st_wParaSignal)&0x07FF)^S2U(objParallel.m_st_wParaSignalF))&0x07FF)^S2U(objParallel.m_st_wParaSignalF);

		objParallel.m_st_wParaSignalF.bLeader_Rack = 1;
	}
	objParallel.m_st_wParaSignalF.bLeader_Rack = 1;
				
}

/************************************************************************************
函数名：	Dat_DigitalInput()
功能描述：	EXTIO口输入采集模块，采集EXTIO口的输入开关量，并做确认处理
入口变量:   m_st_wSPIDataIn2_0		硬件输入原始信号
出口变量:	m_st_HardwareSignal			确认后的信号
函数调用：	无		
************************************************************************************/

	void	Class_DigitalIO::Dat_DigitalInput()
	{

		objParallel.m_st_wParaSignalF.bCabQ3BpStatusF = m_st_wLocalNewSignal.bCAB_Q3BP_STATUS;
		objParallel.m_st_wParaSignalF.bExtOutStatusF = m_st_wLocalNewSignal.bEXT_OUT_STATUS;
		
		objParallel.m_st_wParaSignalF.bSystemUnlockF = m_un_ExInDReg.BIT.bSystem_Unlock_F;  //逆变同步跟踪信号(离散总线信号输入)，0表示逆变开启完成
		objParallel.m_st_wParaSignalF.bInvOnStandbyF = m_un_ExInDReg.BIT.bInv_ON_Standby_F;
        objParallel.m_st_wParaSignalF.bInvBpStatusF = m_un_ExInDReg.BIT.bInv_BP_Status_F;
        objParallel.m_st_wParaSignalF.bMasterOnlineF = m_un_ExInDReg.BIT.bMaster_Online_F;

	 	

		//IO信号确认方式更改为：每个硬件信号独立确认

		
		//m_i16IoConfCntPer1ms[1] Q2确认计数器
		//m_i16IoConfCntPer1ms[2] Q3BP确认计数器
		//m_i16IoConfCntPer1ms[3] Q5确认计数器
		//m_i16IoConfCntPer1ms[4] Fuse确认计数器
		//m_i16IoConfCntPer1ms[5] DCOVP确认计数器
		//m_i16IoConfCntPer1ms[6] FanFault确认计数器
		//m_i16IoConfCntPer1ms[7] RecOff确认计数器
		//m_i16IoConfCntPer1ms[8] EPO确认计数器
		//m_i16IoConfCntPer1ms[9] Inv OVERLOAD signal确认计数器
		//m_i16IoConfCntPer1ms[10] InvID确认计数器
		//m_i16IoConfCntPer1ms[11] ModuleReady确认计数器
		//m_i16IoConfCntPer1ms[12] BPCT确认计数器
		
		//m_i16IoConfCntPer1ms[14] bBpOnline确认计数器


		//m_i16IoConfCntPer1ms[17] 机架内有主机在线确认
		//m_i16IoConfCntPer1ms[18] 机架内有主机在线确认



		//m_i16IoConfCntPer1ms[21] IGBT过温确认
		if(objSystem.m_st_wFlagSystem.bTimer1ms == 1)
		{

		    
			
			
			//---------Q3BP确认（合上4ms，断开2ms）--------
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
			//-------Q5确认（合上4ms，断开10ms）--------
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
			m_st_HardwareSignal.bQ5 = 1;        // 模块单机调试用
			//------------bFuse确认3ms---------
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
			//bTestMode确认（保留，直接利用m_st_SPIDataIn2_0）
			//bServiceMode确认（保留，直接利用m_st_SPIDataIn2_0）
			//------------bDCOVP确认3ms---------
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
			//------------bFanFault确认2ms---------
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
												
			//------------bRecOff确认3ms---------
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
			//------------bEPO确认---------
			if(m_un_ExInAReg.BIT.bEPO == 0)
			{
				m_i16IoConfCntPer1ms[8]++;						//无效-->有效：10ms
				if(m_i16IoConfCntPer1ms[8] >= K10msBased1msCnst)
				{
					m_i16IoConfCntPer1ms[8] = K10msBased1msCnst;
					m_un_ExInARegConf.BIT.bEPO = 0;			//低电平有效				
				}
			}
			else
			{
				m_i16IoConfCntPer1ms[8]--;						//有效-->无效：10ms
				if(m_i16IoConfCntPer1ms[8] <= 0)
				{
					m_i16IoConfCntPer1ms[8] = 0;
					m_un_ExInARegConf.BIT.bEPO = 1;	
				}
			}
			
			if ( m_un_ExInARegConf.BIT.bEPO == 0  )								
			{
				objInvCtrl.m_st_wFlagInvOnoff.bEPO = 1;						//置紧急关机状态
				objSystem.m_st_wFlagStatus.bEPO = 1;						//置紧急关机状态
			}
			
            //------------Inv OVERLOAD signal确认---------
			if(m_un_ExInDReg.BIT.bInv_OverLoad_F == 1)
			{
				m_i16IoConfCntPer1ms[9]++;						//无效-->有效：10ms
				if(m_i16IoConfCntPer1ms[9] >= K10msBased1msCnst)
				{
					m_i16IoConfCntPer1ms[9] = K10msBased1msCnst;
					objParallel.m_st_wParaSignalF.bInvOverloadF = 1;			// 1为逆变过载				
				}
			}
			else
			{
				m_i16IoConfCntPer1ms[9]--;						//有效-->无效：10ms
				if(m_i16IoConfCntPer1ms[9] <= 0)
				{
					m_i16IoConfCntPer1ms[9] = 0;
					objParallel.m_st_wParaSignalF.bInvOverloadF = 0;	
				}
			}

//---------------------------bInvID确认---------------------------------不能通过移位来计算，只是移位的优先级小于＋
if(m_i16InvIDOkFlag == 0)				//模块ID之上电读一次
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
					objMonInterface.m_st_IdErrorFlag.bUsIdNumExpire = 1;	//模块ID超出范围
				}
				else
				{
					objMonInterface.m_st_IdErrorFlag.bUsIdNumExpire = 0;	//模块ID未超出范围
				}
       	}
	}
	else
	{
		m_u16InvIDOld =  m_u16InvID;
		m_i16IoConfCntPer1ms[10] = 0;
	}
}
//--------------------------bModuleReady确认----------------------------
			if(m_un_ExInAReg.BIT.bModuleReady == 1)		//m_i16IoConfCntPer1ms[12] BPCT确认计数器
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
			
//-----------------------bBPCT确认------------------------------------------
/*			if(m_st_wSPIDataIn2_0.bRevBP_BACKFEED_DETECT == 0)		//m_i16IoConfCntPer1ms[12] BPCT确认计数器
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
//------------------------旁路反灌确认----------------------------------
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
//-------------------------------bRECFault确认----------------------------------
/*
			if(m_st_wSPIDataIn4_0.bRECFault == 1)		//m_i16IoConfCntPer1ms[12] BPCT确认计数器
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

//--------------------------binvAutoOn确认----------------------------
			if(m_un_ExInAReg.BIT.bInvAutoOnEn == 1)		//m_i16IoConfCntPer1ms[16] 逆变自开机确认计数器
			{
				m_i16IoConfCntPer1ms[16]++;
				if(m_i16IoConfCntPer1ms[16] >= K10msBased1msCnst)
				{
					m_i16IoConfCntPer1ms[16] = K10msBased1msCnst;
					m_st_HardwareSignal.bInvAutoOnEn = 1;       //允许逆变自开机，不过系统中要有主机在线
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
//----------------------机架内有主机在线确认--------------------
if(m_un_ExInDReg.BIT.bMaster_Online_F == 1)
{
    m_i16IoConfCntPer1ms[17]++;
    if(m_i16IoConfCntPer1ms[17] >=K10msBased1msCnst )
    {
        m_i16IoConfCntPer1ms[17] = K10msBased1msCnst;
        objParallel.m_st_wParaSignalF.bMasterOnlineF = 1;       //有主机
    }
}
else
{
    m_i16IoConfCntPer1ms[17]--;
    if(m_i16IoConfCntPer1ms[17] <=0)
    {
        m_i16IoConfCntPer1ms[17] = 0;
        objParallel.m_st_wParaSignalF.bMasterOnlineF = 0;        //无主机

    }       

}
#endif
			 
//-----------------------bBpSTSShortFault确认------------------------------------------
			//By wxw20090601 Adapter15~20k bBpSTSShortFault可以自我检测		
		//-----------------------bBpOvCurrFault确认------------------------------------------
		//By wxw20090625 Adapter15~20k bBpOvCurrFault可以自我检测	
//-------------------------------bBPSF确认-----------------------------//
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
					m_st_wLocalSignal.bBPSF = 1;		//1有效

				}		
		
			}

//-------------------------------bQ3BP确认-----------------------------//
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
			
//-------------------------------bVoutAbnomalFromBP确认-----------------------------//			

//-------------------------------bOUT_PF确认-----------------------------//
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
//-------------------------------inv散热器过温确认-----------------------------//  

			if(m_un_ExInAReg.BIT.bInvHeatSinkOVTemp == 0)
			{
				m_i16IoConfCntPer1ms[21]++;
				if(m_i16IoConfCntPer1ms[21] >= K200msBased1msCnst )
				{
					m_i16IoConfCntPer1ms[21] = 30000;
					m_st_wHardwareNewSignal.bInvHeatSinkOVTemp = 1;         // 1为过温
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
函数名：	Dat_DigitalOutput()			
功能描述：	EXTIO输出处理模块，根据逻辑切换的结果和并机信号输出要求等，把输出信号集中处理到一个变量中
函数调用：	无
************************************************************************************/

	void	Class_DigitalIO::Dat_DigitalOutput()

	{
		m_st_wLocalSignal.bBpSTS = objXfer.m_st_XferResult.bBpSTS;						//拷贝旁路STS位
		m_st_wLocalSignal.bInvKM = objXfer.m_st_XferResult.bInvKM;						//拷贝逆变STS位
		//设置SCR的开通时间以保证继电器不拉弧
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

		//注意如下赋值语句时序，不能放于如上设置SCR开通if语句之前，否则不能开通SCR
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
		//逆变状态灯
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
函数名：	Dat_DigitalOutput()			
功能描述：	EXTIO输出处理模块，根据逻辑切换的结果和并机信号输出要求等，把输出信号集中处理到一个变量中
函数调用：	无
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
