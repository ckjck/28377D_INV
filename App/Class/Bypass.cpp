/*==========================================================*/ 	
/* Title		:	Bypass.cpp								*/ 	
/* Description	: 	Class_Bypass definition				*/ 	
/* Date			:	Apr.2008								*/ 	
/*==========================================================*/
#include "Bypass.h"


/************************************************************************************
函数名：	App_BpVarInit()	
功能描述：	System类变量初始化		
函数调用：	
***********************************************************************************/

	 void	Class_Bypass::App_BpVarInit(void)
	{
		//Vbp/Fbp Range Setting
		//m_i16FbpRangIndex = 0;					//iwFbpLimitSet*4 + m_i16FbpTraceSet	
		m_i16FbpRangIndex = 16;					//iwFbpLimitSet + 16
		
		m_i16FbpUpTrace = 5200;
		m_i16FbpDnTrace = 4800;
		m_i16FbpUpLimit = 6000;					//+20%
		m_i16FbpDnLimit = 4000;					//-20%
		
		m_i16VbpUpDnlimitSet1=0;				// 4*VbpUplimitSet+VbpDnlimitSet
		m_i16VbpUpDnlimitSet2=4;				// 4*VbpUplimitSet+4
		
		m_i16VbpUplimit = 1178;					//+15%
		m_i16VbpDnlimit = 819;					//-20%
		
		m_i16KVbpa_0 = 4119;					//定标系数
		m_i16KVbpb_0 = 4119;                    //定标系数
		m_i16KVbpc_0 = 4119;                    //定标系数
		
		m_i16PointcntBpM_0 = K50HzPointCntCnst;
		m_unDthetaBpMean_0.half.hword=K50HzDthetaCnst;		//60Hz@8KHz
		
	}
/************************************************************************************
函数名：App_BpParamConfig()
************************************************************************************/
	void	Class_Bypass::App_BpParamConfig(void)
	{
		//旁路电压、频率范围设置
		switch( objMonInterface.m_i16VbpUplimitSet)
		{
			case 0:											//旁路保护范围上限+15%
				m_i16VbpUplimit = 1178;						//旁路保护范围上限
				break;
			case 1:										//+10%
				m_i16VbpUplimit = 1126;
				break;
			case 2:										//+20%	
				m_i16VbpUplimit = 1229 ;
				break;
			default:
				m_i16VbpUplimit = 1178;
				break;
				
			
		}

		switch (objMonInterface.m_i16VbpDnlimitSet)		//旁路保护范围下限-20%
		{
			case 0:				
				m_i16VbpDnlimit1 = 819;
				break;
			case 1:
				m_i16VbpDnlimit1 = 921;				//-10%
				break;
			case 2:
				m_i16VbpDnlimit1 = 716;				//-30%
				break;
			case 3:
				m_i16VbpDnlimit1 = 614;				//-40%
				break;
			default:
				m_i16VbpDnlimit1 = 819;				//-20%
				break;
		}
		
		//旁路保护范围设置编号
		m_i16VbpUpDnlimitSet1 = 5*objMonInterface.m_i16VbpUplimitSet + objMonInterface.m_i16VbpDnlimitSet;	//窄旁路范围
		m_i16VbpUpDnlimitSet2 = 5*objMonInterface.m_i16VbpUplimitSet + 4;				//宽旁路范围
	

		//旁路频率范围设置编号  
		m_i16FbpRangIndex =	4*objMonInterface.m_i16FbpLimitSet + objMonInterface.m_i16FbpTraceSet;
		//m_i16FbpRangIndex =	objMonInterface.m_i16FbpLimitSet + 16;
		//m_i16FbpRangIndex = 17;

		//输出频率设置映射  
		if(objMonInterface.m_st_wUPSModeRun.bFreqOut == 0)				//50Hz
		{

			switch(objMonInterface.m_i16FbpLimitSet)
			{
				case 0:									//20%
					m_i16FbpUpLimit = 6000;
					m_i16FbpDnLimit = 4000;
					break;
				case 1:									//10%
					m_i16FbpUpLimit = 5500;
			 		m_i16FbpDnLimit = 4500;
					break;
//				case 2:									// 5%
//					m_i16FbpUpLimit = 5250;
//					m_i16FbpDnLimit = 4750;
//					break;
//				case 3:									// 2.5%
//					m_i16FbpUpLimit = 5125;
//					m_i16FbpDnLimit = 4875;
//					break;
				default:
					m_i16FbpUpLimit = 6000;
					m_i16FbpDnLimit = 4000;
					break;				
			
			}
			
			switch(objMonInterface.m_i16FbpTraceSet)
			{
				case 0:							//+-2Hz
					m_i16FbpUpTrace = 5200;
					m_i16FbpDnTrace = 4800;
					break;
				case 1:							//+-1Hz
					m_i16FbpUpTrace = 5100;
					m_i16FbpDnTrace = 4900; 
					break;
				case 2:							//+-3Hz
					m_i16FbpUpTrace = 5300;
					m_i16FbpDnTrace = 4700; 
					break;
				case 3:							//+-0.5Hz
					m_i16FbpUpTrace = 5050;
					m_i16FbpDnTrace = 4950; 
					break;
				default:
					m_i16FbpUpTrace = 5200;
					m_i16FbpDnTrace = 4800;
					break;
			}
					
			//当频率保护范围较跟踪范围窄时，以频率跟踪范围为准！！					
			if(m_i16FbpUpLimit < m_i16FbpUpTrace) 
			{
				m_i16FbpUpLimit = m_i16FbpUpTrace;
			}
			if(m_i16FbpDnLimit > m_i16FbpDnTrace) 
			{
				m_i16FbpDnLimit = m_i16FbpDnTrace;
			}
			
		}
		else									//60Hz
		{
			switch(objMonInterface.m_i16FbpLimitSet)
			{
				case 0:										//20%
					m_i16FbpUpLimit = 7200;
					m_i16FbpDnLimit = 4800;
					break;
				case 1:										//10%
					m_i16FbpUpLimit = 6600;
					m_i16FbpDnLimit = 5400;
					break;
//				case 2:										// 5%
//					m_i16FbpUpLimit = 6300;
//					m_i16FbpDnLimit = 5700;
//					break;
//				case 3:										// 2.5%	
//					m_i16FbpUpLimit = 6150;
//					m_i16FbpDnLimit = 5850;
//					break;
				default:
					m_i16FbpUpLimit = 7200;
					m_i16FbpDnLimit = 4800;
					break;				
			}
			
			switch(objMonInterface.m_i16FbpTraceSet)
			{
				case 0:							//+-2Hz
					m_i16FbpUpTrace = 6200;
					m_i16FbpDnTrace = 5800;	 
					break;
				case 1:							//+-1Hz
					m_i16FbpUpTrace = 6100;
					m_i16FbpDnTrace = 5900; 
					break;
				case 2:							//+-3Hz
					m_i16FbpUpTrace = 6300;
					m_i16FbpDnTrace = 5700; 
					break;
				case 3:							//+-0.5Hz
					m_i16FbpUpTrace = 6050;
					m_i16FbpDnTrace = 5950; 
					break;
				default:
					m_i16FbpUpTrace = 6200;
					m_i16FbpDnTrace = 5800;	 
					break;
			}				
			//当频率保护范围较跟踪范围窄时，以频率跟踪范围为准！！					
			if(m_i16FbpUpLimit < m_i16FbpUpTrace)
			{
				m_i16FbpUpLimit = m_i16FbpUpTrace;
			}
			if(m_i16FbpDnLimit > m_i16FbpDnTrace)
			{
				m_i16FbpDnLimit = m_i16FbpDnTrace;
			}
		}
		
		//采样系数设置
		switch (objMonInterface.m_st_wUPSModeRun.bVoltOut)
		{
			case 0:																								//400v
				m_i16KVbpNormal = 4119;																			// 正样检测板更改
				break;				            													
			case 1:																									//380v
				m_i16KVbpNormal = 4119;   													
				break;                    													
			case 2:																									//415v
				m_i16KVbpNormal = 4119;
				break;
			default:
				m_i16KVbpNormal = 4119;  
				break;							
			}
		

		//旁路电压定标系数校正
		m_i16KVbpa_0 = (((INT32)objMonInterface.m_i16KsVbpaSet)*m_i16KVbpNormal )>>12;
		m_i16KVbpb_0 = (((INT32)objMonInterface.m_i16KsVbpbSet)*m_i16KVbpNormal )>>12;
		m_i16KVbpc_0 = (((INT32)objMonInterface.m_i16KsVbpcSet)*m_i16KVbpNormal )>>12;
		
		//旁路过流点，1024-->100%
//		m_i16OverCurrPoint=(((INT32)1126)*objMonInterface.m_i16GrantKVA)/objMonInterface.m_i16ModelKVA;	
		m_i16OverCurrPoint = 1024;

		//快检旁路异常时，旁路模值的阀值
		m_i16VbpMDnlimitFast =((((((INT32)(m_i16VbpDnlimit1))<<2) * 5792) >>12) * 973) >> 10;   //Vrms * 4 * 1.414 * 0.95
		m_i16VbpMUplimitFast =((((((INT32)(m_i16VbpUplimit))<<2) * 5792) >>12) * 1075) >> 10;   //Vrms * 4 * 1.414 * 1.05
		m_i16DthetaBpUplimitFast = (((INT32)m_i16FbpUpLimit) * 1887)>>12;	//f/100/8000*1024*360;
		m_i16DthetaBpDnlimitFast = (((INT32)m_i16FbpDnLimit) * 1887)>>12;	//f/100/8000*1024*360;
	}

/************************************************************************************
函数名：	vBypass_Fault()
功能描述：	旁路故障判断模块 
函数调用：	无			
************************************************************************************/	
	void	Class_Bypass::Alg_BpFault(void)
	{
		if ( objParallel.m_st_wParaSignal.bInvBpStatus == 1 )							//逆变侧供电
		{
			if ( (m_st_FlagVbpTotalWide.bBpRange==3) && (objLoad.m_st_wFlagLoad.bInvOvloadTimeout == 1) )//逆变过载延时到
			{
				objInvCtrl.m_st_wFlagInvOnoff.bBpFailOff = 1;							//置旁路异常关机位
				objSystem.m_st_wFlagFault.bBpFailOff = 1;								//旁路异常关机告警故障
			}
		}
		else															//旁路侧供电
		{
			//本机输出过载或旁路5分钟工作区内或iBptoinvTimes受限
			if ( (m_st_FlagVbpTotal.bBpRange == 3) && (( objLoad.m_st_wFlagLoad.bOvload == 1 ) 
				|| ( objXfer.m_st_XferResult.bBpSideDelay != 0 )
				|| (objSystem.m_st_wFlagPrompt.bSwitchTimesClamp==1)))
			
			{
				objInvCtrl.m_st_wFlagInvOnoff.bBpFailOff = 1;							//置旁路异常关机位
				objSystem.m_st_wFlagFault.bBpFailOff = 1;								//置旁路异常故障位
			}
		}
		

		// 自老化模式且逆变器开启条件下，若输出空开断开  或  逆变未供电时旁路超保护，
		//					或  （逆变器供电后旁路不可跟踪或相位未锁定），置旁路异常关机。		

		if((objSystem.m_st_wFlagSystem.bSelfAgingAction  == 1) && (objInverter.m_st_wFlagInv.bInv != 0) )
		{
			if(objSwitches.m_st_FlagBreaker.bQ5 ==0 )	
			{
					objInvCtrl.m_st_wFlagInvOnoff.bBpFailOff = 1;							//置旁路异常关机位
					objSystem.m_st_wFlagFault.bBpFailOff = 1;								//置旁路异常故障位
			}
//			else if(objXfer.m_st_wXferCmd.bSwitchStatus ==0 && m_st_FlagVbpTotal.bBpRange!= 0 )
			else if((objXfer.m_st_wXferCmd.bSwitchStatus  == 0)  && ( m_st_FlagVbpTotal.bBpRange > 2 ))
			{
					objInvCtrl.m_st_wFlagInvOnoff.bBpFailOff = 1;							//置旁路异常关机位
					objSystem.m_st_wFlagFault.bBpFailOff = 1;								//置旁路异常故障位
			}
			
			else if((objXfer.m_st_wXferCmd.bSwitchStatus  == 1)
				&&( (m_st_FlagVbpTotal.bBpTrace !=  1) || (objInverter.m_st_wFlagInv.bPhaseLock == 0)) )
			{
					objInvCtrl.m_st_wFlagInvOnoff.bBpFailOff = 1;							//置旁路异常关机位
					objSystem.m_st_wFlagFault.bBpFailOff = 1;								//置旁路异常故障位
			}
			else
			{}
				
		}	

		if ( ( objInverter.m_st_wFlagInv.bInv == 0 ) && ( objInvCtrl.m_i16FlagOffReason >=3 ) )				//逆变器已关闭且关机原因>=3
		{
			objSystem.m_st_wFlagFault.bBpFailOff = 0;										//清旁路异常故障位
		}
	}


/************************************************************************************
函数名：	vVbpseq_Fault()
功能描述：	旁路反序判断模块
函数调用：	无			
************************************************************************************/

	void	Class_Bypass::Alg_BpSeqFault(void)
	{
		//当旁路电压有效值低于100V@277V时保持原状态不变
		if ( objSystem.m_st_wFlagSystem.bTimer40ms == 1 )							//40ms定时到
		{

			if ( m_unVbpMMean_0.half.hword < 2091 )
			{
				if ( m_i16BpInvtCntPer40ms > 0 )    
				{
					m_i16BpInvtCntPer40ms --;	
				}
			}

			else if ( m_unDthetaBpBase_0.half.hword < -K30HzDthetaCnst )	//旁路电压>100V且频率<-30Hz
			{
				m_i16BpInvtCntPer40ms ++;									//旁路反序确认计数递增
				if ( m_i16BpInvtCntPer40ms >= 10 )							//0.4s
				{
					m_i16BpInvtCntPer40ms = 200;								//确定反序，恢复时间保证为8s
					m_st_FlagVbpTotal.bBpInvtSeq = 1;					//置旁路反序
					objSystem.m_st_wFlagFault.bBpInvtSeq = 1;						//置旁路反序故障
				}
			}
			else
			{
				if ( m_i16BpInvtCntPer40ms > 0 )
				{
					m_i16BpInvtCntPer40ms --;								//旁路反序确认计数递减
				}
				
				if ( m_i16BpInvtCntPer40ms < 10 )
				{
					m_st_FlagVbpTotal.bBpInvtSeq = 0;					//清旁路反序
					objSystem.m_st_wFlagFault.bBpInvtSeq = 0;						//清旁路反序故障
				}
			}

		}		

	}
/************************************************************************************
函数名：	Alg_BackFeedFault()
功能描述：	IGBT及旁路过流故障判断模块
函数调用：	无			
************************************************************************************/
void	Class_Bypass::Alg_BpBackFeedFault(void)
{	
	//旁路反灌及保护功能
	if(objSystem.m_st_wFlagSystem.bTimer14ms == 1)	
	{		
		if((objDigitalIO.m_st_wLocalSignal.bBpSTS == 0)&&((objBypass.m_st_FlagVbpTotal.bBpRange==3))
		 	&& (objOutput.m_i16VoutaPrms>739) && (objOutput.m_i16VoutbPrms>739) && (objOutput.m_i16VoutcPrms>739)		//输出电压大于200V
			&&(objParallel.m_st_wParaSignal.bInvBpStatus== 1))					//本机逆变供电
		{
			if (objDigitalIO.m_st_wHardwareNewSignal.bBPBackFeedDetect ==1)							//有旁路反灌IO信号
			{
				if(m_i16FeedbackVcomCnt<500)
				{
					m_i16FeedbackVcomCnt++;	
				}				
			}	
			else
			{
				if(m_i16FeedbackVcomCnt>0)	
				{	
					m_i16FeedbackVcomCnt--;
				}
			}	
			
			if(m_i16FeedbackVcomCnt>=350)	 
			{
				objSystem.m_st_wFlagFault.bBypFeedBackFault=1;							//送给监控板，以及用以触发故障记录					
			}
		}
		else
		{	
			m_i16FeedbackVcomCnt = 0;
		}
						
		if((objSystem.m_st_wFlagFault.bBypFeedBackFault==1)||(objSystem.m_st_wFlagStatus.bMainFeedBack == 1))
		{
			
			objDigitalIO.m_st_wLocalSignal.bBPFeedAction = 1;						//反灌保护CB脱扣信号，送给检测板				
		}
		else
		{
			objDigitalIO.m_st_wLocalSignal.bBPFeedAction = 0;
		}
	}

}

/************************************************************************************
函数名：	Dat_TempCurrentGrant()
功能描述：	旁路电流授权模块，根据当前本机环境温度，对旁路电流进行降额
************************************************************************************/
void	Class_Bypass::Dat_TempCurrentGrant(void)
{

	m_i16IbpaGrant = (((INT32)objOutput.m_i16IoutaPrms) * objSystem.m_i161DivGrantRate)>>10; 
	m_i16IbpbGrant = (((INT32)objOutput.m_i16IoutbPrms) * objSystem.m_i161DivGrantRate)>>10; 
	m_i16IbpcGrant = (((INT32)objOutput.m_i16IoutcPrms) * objSystem.m_i161DivGrantRate)>>10;
	
	if(objInverter.m_i16wEnvirTemp <1078)
	{
		m_i16IbpaGrant = (((INT32)m_i16IbpaGrant) * 1138)>>10; 
		m_i16IbpbGrant = (((INT32)m_i16IbpbGrant) * 1138)>>10; 
		m_i16IbpcGrant = (((INT32)m_i16IbpcGrant) * 1138)>>10; 
	}
	
}

/************************************************************************************
函数名：	Dat_IbpFlag()
功能描述：	旁路电流标志处理模块，根据当前本机旁路各相电流授权值的大小，加工成本机旁路电流大小的标志
函数调用：	igap()				回差处理子程序
************************************************************************************/
void	Class_Bypass::Dat_IbpFlag(void)
{
	INT16	i16MainAX;

	//各相电流标志的生成

	
		i16MainAX = m_st_FlagIbpABC.bBpA;
		m_st_FlagIbpABC.bBpA = igap(m_i16IbpaGrant, &BpCurrentTable, i16MainAX);

		i16MainAX = m_st_FlagIbpABC.bBpB;
		m_st_FlagIbpABC.bBpB = igap(m_i16IbpbGrant, &BpCurrentTable, i16MainAX);

		i16MainAX = m_st_FlagIbpABC.bBpC;
		m_st_FlagIbpABC.bBpC = igap(m_i16IbpcGrant, &BpCurrentTable, i16MainAX);
	

	//综合各相标志，形成总标志
	m_st_FlagIbpTotal.bTotal = m_st_FlagIbpABC.bBpA | m_st_FlagIbpABC.bBpB | m_st_FlagIbpABC.bBpC;
}

/************************************************************************************
函数名：	App_OverCurrentDelay()
功能描述：	旁路过流延时模块，根据过流标志，确定不同过载电流下的延时
************************************************************************************/
void	Class_Bypass::App_OverCurrentDelay(void)
{
	if((objXfer.m_st_wXferCmd.bSwitchStatus == 0) && (objXfer.m_st_XferResult.bBpSTS == 1))	//旁路供电时							
	{
		if ( objSystem.m_st_wFlagSystem.bTimer40ms == 1 )								//40ms定时到
		{
			switch(m_st_FlagIbpTotal.bTotal)
			{
				case 15:
						m_i16OvCurrentDlyCntPer40ms += 3000;	//Ibp>400%时，200ms	
						break;
				case 7:
						m_i16OvCurrentDlyCntPer40ms += 600;		//400%>Ibp>150%时，1s
						break;
				case 3:
						m_i16OvCurrentDlyCntPer40ms += 10;		//150%>Ibp>125%时，1min
						break;
				case 1:
						m_i16OvCurrentDlyCntPer40ms += 2;			//125%>Ibp>100%时，5min
						break;
				case 0:
						m_i16OvCurrentDlyCntPer40ms -= 60;
						if(m_i16OvCurrentDlyCntPer40ms<0)
						{
							m_i16OvCurrentDlyCntPer40ms = 0;
						}
						break;
				default:
						break;
			}
			if(m_i16OvCurrentDlyCntPer40ms>=15000)
			{
				m_st_FlagIbpTotal.bIbpOverTimeOut = 1;
				m_i16OvCurrentDlyCntPer40ms = 15000;
				

				//禁止开机
				m_st_FlagIbpTotal.bIbpOverFault = 1;
				objInvCtrl.m_st_wFlagInvOnoff.bManualRecoverOff = 1;
			}
		}
	}
	else
	{
		m_i16OvCurrentDlyCntPer40ms = 0;
	}
}
/************************************************************************************
函数名：	Alg_OvcurrFault()
功能描述：	IGBT及旁路过流故障判断模块
函数调用：	无			
************************************************************************************/
void	Class_Bypass::Alg_OvcurrFault(void)
{
	//旁路不供电，计数器清零
	if ( objDigitalIO.m_st_wLocalSignal.bBpSTS == 1 )							//旁路侧供电
	{
		//兼容机型降额使用，过流点用变量m_i16OverCurrPoint代替
		if( objSystem.m_st_wFlagSystem.bTimer40ms == 1 )
		{
			if(objSystem.m_st_wFlagFault.bBpOverCurrent == 1)
			{
				if(m_st_FlagIbpTotal.bTotal == 0)
				{
					m_i16BpOvCurrCntPer40ms--;
					if(m_i16BpOvCurrCntPer40ms<=0)
					{
						objSystem.m_st_wFlagFault.bBpOverCurrent = 0;
						m_i16BpOvCurrCntPer40ms = 0;
					}
				}
			}
			else
			{	
				if(m_st_FlagIbpTotal.bTotal >= 1)
				{
					m_i16BpOvCurrCntPer40ms++;
					if(m_i16BpOvCurrCntPer40ms >=25)
					{
						m_i16BpOvCurrCntPer40ms = 25;
						objSystem.m_st_wFlagFault.bBpOverCurrent = 1;
					}
				}
				else
				{
					m_i16BpOvCurrCntPer40ms = 0;
				}
			}
			
		}
	}
	else
	{
		m_i16BpOvCurrCntPer40ms = 0;
		objSystem.m_st_wFlagFault.bBpOverCurrent = 0;
	}
}
/************************************************************************************
函数名：	vVbp_Flag()			
功能描述：	旁路标志处理模块
函数调用：	igap()		回差处理子程序
************************************************************************************/
	void	Class_Bypass::Dat_VbpFlag(void)
	{
		UINT16	u16Back, u16Temp;		
		INT16		i16MainAX;
		
		//旁路各相电压回差处理（窄范围）		
		if((m_i16VbpUpDnlimitSet1 >= 0) && (m_i16VbpUpDnlimitSet1 <= 14) )
		{ 
			u16Back = m_st_FlagVbpAB1.bVbpA;
			m_st_FlagVbpAB1.bVbpA = igap (m_unVbpaRms.half.hword, &VbpRangeTable[m_i16VbpUpDnlimitSet1], u16Back);	//调用回差处理函数
			u16Back = m_st_FlagVbpAB1.bVbpB;
			m_st_FlagVbpAB1.bVbpB = igap (m_unVbpbRms.half.hword, &VbpRangeTable[m_i16VbpUpDnlimitSet1], u16Back);	//调用回差处理函数
			u16Back = m_st_FlagVbpFC1.bVbpC;
			m_st_FlagVbpFC1.bVbpC = igap (m_unVbpcRms.half.hword, &VbpRangeTable[m_i16VbpUpDnlimitSet1], u16Back);	//调用回差处理函数			
		}
		//BY wxw201004 三单反灌时旁路的电压即是逆变电压，逆变器自己跟踪自己，
		//导致输出频率在52Hz-48Hz波动，旁路在正常和超跟踪状态变化
		
		
		//旁路各相电压回差处理（宽范围）
		if((m_i16VbpUpDnlimitSet2 >= 0) && (m_i16VbpUpDnlimitSet2 <= 14) )
		{ 
			u16Back = m_st_FlagVbpAB2.bVbpA;
			m_st_FlagVbpAB2.bVbpA = igap (m_unVbpaRms.half.hword, &VbpRangeTable[m_i16VbpUpDnlimitSet2], u16Back);	//调用回差处理函数
			u16Back = m_st_FlagVbpAB2.bVbpB;
			m_st_FlagVbpAB2.bVbpB = igap (m_unVbpbRms.half.hword, &VbpRangeTable[m_i16VbpUpDnlimitSet2], u16Back);	//调用回差处理函数
			u16Back = m_st_FlagVbpFC2.bVbpC;
			m_st_FlagVbpFC2.bVbpC = igap (m_unVbpcRms.half.hword, &VbpRangeTable[m_i16VbpUpDnlimitSet2], u16Back);	//调用回差处理函数			
		}	
	
		
		//zqf 060606
		//旁路窄<-->宽范围的选择和切换
		u16Temp=(objXfer.m_st_XferResult.bInvKM == 1)
				&&((objInvCtrl.m_st_wFlagInvOnoff.bNeverRecoverOff == 1)
				||(objInvCtrl.m_st_wFlagInvOnoff.bInvFaultOff == 1)||(objSystem.m_st_wFlagPrompt.bPow15vFault == 1)
				||(objSystem.m_st_wFlagPrompt.bDCAbnormalOff == 1)||(objSystem.m_st_wFlagFault.bVoutAbnormal == 1)
				||(objInvCtrl.m_st_wFlagInvOnoff.bAutoRecoverOff == 1)||(objLoad.m_st_wFlagLoad.bInvOvloadTimeout == 1));							
		
		if((objInverter.m_st_wFlagInv.bInv == 3) &&(objXfer.m_st_XferResult.bBpSTS == 1)
			&&(((objMonInterface.m_st_wUPSModeRun.bConfig == 0) && (objMonInterface.m_st_wUPSModeRun.bMode == 1))
			||((objMonInterface.m_st_wUPSModeRun.bConfig == 2) && (objMonInterface.m_st_wUPSModeRun.bMasterECO == 1))))
		{
			m_st_wBpAmpRangeFlag.bLocalWide=0;							//ECO模式下旁路只选择窄范围
		}
		else
		{
			if(u16Temp)
			{
				m_st_wBpAmpRangeFlag.bLocalWide = 1;						//本机旁路选择宽范围
			}
			else if((objXfer.m_st_XferResult.bInvKMBak == 0) && (objXfer.m_st_XferResult.bInvKM == 1))
			{
				m_st_wBpAmpRangeFlag.bLocalWide = 0;						//本机旁路选择窄范围
			}
		}
			
		if((objMonInterface.m_st_wUPSModeRun.bConfig == 3)&&(m_st_wBpAmpRangeFlag.bCANWide == 1))
		{
			m_st_wBpAmpRangeFlag.bParaWide = 1;							//并机旁路选择宽范围
		}
		else
		{
			m_st_wBpAmpRangeFlag.bParaWide = 0;							//并机旁路选择窄范围
		}

		//屏蔽宽范围
		m_st_wBpAmpRangeFlag.bLocalWide = 0;
		m_st_wBpAmpRangeFlag.bParaWide = 0;	
			
		if((m_st_wBpAmpRangeFlag.bLocalWide == 1)||(m_st_wBpAmpRangeFlag.bParaWide == 1))
		{	
			S2U(m_st_FlagVbpAB) = S2U(m_st_FlagVbpAB2);
			m_st_FlagVbpFC.bVbpC =  m_st_FlagVbpFC2.bVbpC;
			m_i16VbpDnlimit = 512;										//旁路保护范围下限：-50%
		}
		else
		{	
			S2U(m_st_FlagVbpAB) = S2U(m_st_FlagVbpAB1);
			
			m_st_FlagVbpFC.bVbpC = m_st_FlagVbpFC1.bVbpC;
			
			m_i16VbpDnlimit = m_i16VbpDnlimit1;
		}		
		
		if((m_st_FlagVbpTotal.bBpRange == 3) && (m_st_FlagVbpTotalWide.bBpRange < 3)
			&&(((m_st_wBpAmpRangeFlag.bLocalWide == 1) && (m_st_wBpAmpRangeFlag.bLocalWideBak == 0))		
			||((m_st_wBpAmpRangeFlag.bParaWide == 1) && (m_st_wBpAmpRangeFlag.bParaWideBak == 0))))
		{
			m_i16VbpOKCntPer40ms = 50;											//旁路从窄范围超保护到宽范围超跟踪无需2s延时
			m_i16VbpNOKCntPer14ms = 0;											//旁路从窄范围超保护到宽范围超跟踪即时有效值判据的计数器清零
		}
		
		m_st_wBpAmpRangeFlag.bLocalWideBak = m_st_wBpAmpRangeFlag.bLocalWide;	//备份
		m_st_wBpAmpRangeFlag.bParaWideBak = m_st_wBpAmpRangeFlag.bParaWide;
		m_st_FlagVbpTotal.bBpRangeBak=m_st_FlagVbpTotal.bBpRange;			//用于旁路快速检测
		
		//旁路频率回差处理
		if(objMonInterface.m_st_wUPSModeRun.bFreqOut == 0)
		{		
			if((m_i16FbpRangIndex >= 0) && (m_i16FbpRangIndex <= 17) )
			{
				u16Back = m_st_FlagVbpFC.bVbpF;
				m_st_FlagVbpFC.bVbpF = igap (m_i16FbpMean, &Fbp50HzTable[m_i16FbpRangIndex], u16Back);
			}	
	
		}
		else
		{		
			if((m_i16FbpRangIndex >= 0) && (m_i16FbpRangIndex <= 17) )
			{
				u16Back = m_st_FlagVbpFC.bVbpF;
				m_st_FlagVbpFC.bVbpF = igap (m_i16FbpMean, &Fbp60HzTable[m_i16FbpRangIndex], u16Back);
			}
		}
		
		
		//基于即时有效值的旁路电压标志综合
		if(m_st_FlagVbpTotal.bBpInvtSeq == 1)	
		{
			m_st_FlagVbpTotal.bBpRange = 3;
			m_i16VbpOKCntPer40ms = 0;
		}
		else
		{

			if((m_i16VbpaPrms>m_i16VbpUplimit) ||(m_i16VbpbPrms>m_i16VbpUplimit)
				||(m_i16VbpcPrms>m_i16VbpUplimit))
			{
				i16MainAX = 16;
			}
			else if((m_i16VbpaPrms<m_i16VbpDnlimit) || (m_i16VbpbPrms<m_i16VbpDnlimit)
				 ||(m_i16VbpcPrms<m_i16VbpDnlimit))
			{
				i16MainAX = 1;
			}
			else
			{
				i16MainAX = 0;
			}


			if(objSystem.m_st_wFlagSystem.bTimer14ms == 1)
			{
				if(i16MainAX==0)		
				{
					if(m_i16VbpNOKCntPer14ms>0)
					{
						m_i16VbpNOKCntPer14ms--;	
					}
				}
				else
				{	
					if(m_i16VbpNOKCntPer14ms<20)
					{
						m_i16VbpNOKCntPer14ms++;
					}
				}
			}


			if (i16MainAX != 0)
			{	
					
					//zqf 060616--正常模式下，即时有效值超上限和超下限判据时间分开设置
					if(((m_i16VbpNOKCntPer14ms >= 4 )&& (i16MainAX == 16))			//超上限确认4次
						||((m_i16VbpNOKCntPer14ms>=12) &&( i16MainAX==1))		//超下限确认12次
						||((m_i16VbpNOKCntPer14ms>=2) && (objInverter.m_st_wFlagInv.bInv==3)	//ECO待机工况确认2次
						&&(((objMonInterface.m_st_wUPSModeRun.bConfig == 0) && (objMonInterface.m_st_wUPSModeRun.bMode == 1))
						||((objMonInterface.m_st_wUPSModeRun.bConfig == 2) && (objMonInterface.m_st_wUPSModeRun.bMasterECO == 1)))))
					{
						m_st_FlagVbpAB.bVbpA = m_st_FlagVbpAB.bVbpB = m_st_FlagVbpFC.bVbpC = i16MainAX;
						m_st_FlagVbpTotal.bBpRange = 3; 
						m_i16VbpOKCntPer40ms = 0;
						//By wxw20100118解决在额定旁路下，先将旁路调到超保护，然后调到超跟踪，逆变器输出52Hz或者48Hz的问题。
						//return;
					}
			}				



			//旁路电压、频率标志综合	
			u16Temp=m_st_FlagVbpAB.bVbpA|m_st_FlagVbpAB.bVbpB
					|m_st_FlagVbpFC.bVbpC|m_st_FlagVbpFC.bVbpF;

			if((u16Temp&0x0011) != 0)
			{
				m_st_FlagVbpTotal.bBpRange = 3;
				m_i16VbpOKCntPer40ms = 0;
			}
			else if(m_i16VbpOKCntPer40ms < 50)
			{
				if(objSystem.m_st_wFlagSystem.bTimer40ms == 1)
				{
					m_i16VbpOKCntPer40ms++;
				}
				m_st_FlagVbpTotal.bBpRange = 3;
			}
			else if(u16Temp!=0)
			{
				m_st_FlagVbpTotal.bBpRange = 1;
			}
			//zqf 061120--去掉ECO待机工况下的模值判据：因为在有些旁路掉电工况，该判据会
			//比快检更快报出，从而导致间断切换。
/*			else if((objInverter.m_st_wFlagInv.bInv == 3)					//ECO待机工况增加旁路模值判断
				&&(((m_st_wUPSModeRun.bConfig == 0) && (m_st_wUPSModeRun.bMode == 1))
				||((m_st_wUPSModeRun.bConfig == 2) && (m_st_wUPSModeRun.bMasterECO == 1))))
			{
				if(m_unVbpMMean_0.half.hword < 4923)		//85%
				{
					m_st_FlagVbpAB.bVbpA = 2;
					m_st_FlagVbpAB.bVbpB = 2;
					m_st_FlagVbpFC.bVbpC = 2;
					m_st_FlagVbpTotal.bBpRange = 1;
				}	
				else if(m_unVbpMMean_0.half.hword>6661)	//115%
				{
					m_st_FlagVbpAB.bVbpA = 8;
					m_st_FlagVbpAB.bVbpB = 8;
					m_st_FlagVbpFC.bVbpC = 8;
					m_st_FlagVbpTotal.bBpRange = 1;
				}
				else
				{
					m_st_FlagVbpTotal.bBpRange = 0;
				}
			}
*/		
			else
			{
				m_st_FlagVbpTotal.bBpRange = 0;
			}
		}

		if((m_st_FlagVbpTotal.bBpRange != 3)  && (m_st_FlagVbpFC.bVbpF == 0))
		{
			m_st_FlagVbpTotal.bBpTrace = 1;
		}
		else
		{
			m_st_FlagVbpTotal.bBpTrace = 0;
		}

		//zqf 060606--增加旁路宽范围电压、频率标志综合
		u16Temp=m_st_FlagVbpAB2.bVbpA|m_st_FlagVbpAB2.bVbpB
				|m_st_FlagVbpFC2.bVbpC|m_st_FlagVbpFC.bVbpF;
				
		if((u16Temp&0x0011) != 0)
		{
			m_st_FlagVbpTotalWide.bBpRange = 3;
			m_i16VbpOKWideCntPer40ms = 0;
		}
		else if(m_i16VbpOKWideCntPer40ms < 50)
		{
			if(objSystem.m_st_wFlagSystem.bTimer40ms == 1) 
			{
				m_i16VbpOKWideCntPer40ms++;
			}
			m_st_FlagVbpTotalWide.bBpRange = 3;
		}
		else 
		{
			if(u16Temp != 0)	
			{
				m_st_FlagVbpTotalWide.bBpRange = 1;
			}
			else			
			{
				m_st_FlagVbpTotalWide.bBpRange = 0;
			}
		}
		
		//20050105 ZXF 增加旁路快速检测
		if((m_st_FlagVbpTotal.bBpRange < 2) && (m_st_FlagVbpTotal.bBpRangeBak > 2 )
		&&(m_i16Bp10sChkCntPer40ms == 0))
		{
				m_st_FlagVbpTotal.bBpQk10St = 1;		//需进行旁路SCR快检
		}

		if((m_st_FlagVbpTotal.bBpQk10St == 1) && (m_st_FlagVbpTotal.bBpQKfinish == 0)				 //还没有进行旁路SCR快检
			&& (objXfer.m_st_XferResult.bBpSTS == 1))		//旁路SCR实际导通
		{
			if(objSystem.m_st_wFlagSystem.bTimer40ms == 1 )
			{
				m_i16Bp10sChkCntPer40ms++;
				m_st_FlagVbpTotal.bBps10Fast = 1;
				if(m_i16Bp10sChkCntPer40ms >= 200)
				{
					m_st_FlagVbpTotal.bBps4FaultDelay = 1;			//上电旁路导通前8s不进行快判配合旁路软件的运行
                                   m_st_FlagVbpTotal.bBpSTSOpenChkEnable=1;          //允许旁路scr开路有效值判据

				}
				if(m_i16Bp10sChkCntPer40ms >= 450)
				{
					m_i16Bp10sChkCntPer40ms = 450;
					m_st_FlagVbpTotal.bBps10Fast = 0;
					m_st_FlagVbpTotal.bBpQk10St = 0;
					m_st_FlagVbpTotal.bBps4FaultDelay = 0;
					m_st_FlagVbpTotal.bBpQKfinish = 1;
				}	
			}
		}
		
		//zqf 060714--增加输出电压快检幅值参考的计算:4.12格式
		//zqf 061013--幅值参考由m_i16VrefSetMd更改为iVrefRunMd_0
		objOutput.m_i16wVoutFastRef1 = (((((INT32)objInvAlg.m_i16wVrefRunMd_0)*922)>>10)*m_i16VbpDnlimit)>>10;	//m_i16VbpDnlimit*m_i16wVrefRunMd_0*0.9*4096/(1024*4096)
		//By wxw20100416 解决三单下突加整流性载没有转旁路，报出输出电压异常或者转电池
		
		objOutput.m_i16wVoutFastRef2 = (((INT32)objInvAlg.m_i16wVrefRunMd_0)*553)>>10;						//60%
		
		
		objOutput.m_i16wVoutFastRef3 = (((INT32)objInvAlg.m_i16wVrefRunMd_0)*717)>>10;						//70%
		objOutput.m_i16wVoutFastRef4 = (((INT32)objInvAlg.m_i16wVrefRunMd_0)*1229)>>10;					//120%
	}

/************************************************************************************
//函数名称：    Dat_Int_BpPreDeal()
************************************************************************************/

#pragma CODE_SECTION(".Epwm1Funcs") 			

void 	Class_Bypass::Dat_Int_BpPreDeal(void)
{
	INT16	i16IntAx_0,i16IntAlpha_0,i16IntBeta_0;
	//读取数据
	m_i16Vbpa_0 = objADDrv.m_i16ADC_Vbpa_0;
	m_i16Vbpb_0 = objADDrv.m_i16ADC_Vbpb_0; 
	m_i16Vbpc_0 = objADDrv.m_i16ADC_Vbpc_0;

	// 旁路相电压计算，定标为6.10
	m_i16Vbpa_0 = (((INT32)m_i16Vbpa_0)*m_i16KVbpa_0)>>10;
	m_i16Vbpb_0 = (((INT32)m_i16Vbpb_0)*m_i16KVbpb_0)>>10;
	m_i16Vbpc_0 = (((INT32)m_i16Vbpc_0)*m_i16KVbpc_0)>>10;

	
	
		// 旁路电压矢量模与相位处理
		i16IntAlpha_0 = (((INT32)m_i16Vbpa_0*2-m_i16Vbpb_0-m_i16Vbpc_0)*K1div3Cnst)>>12;
		i16IntBeta_0 = (((INT32)m_i16Vbpb_0-m_i16Vbpc_0)*KSqrt3invCnst)>>12;

		i16IntAx_0 = (((INT32)i16IntAlpha_0)*i16IntAlpha_0+((INT32)i16IntBeta_0)*i16IntBeta_0)>>16;
		UPDNLMT16(i16IntAx_0,1023,1);
		m_i16VbpM_0 = sqrtTab[i16IntAx_0];
		m_i16SinQBp_0 = (((INT32)i16IntBeta_0)<<12)/m_i16VbpM_0;
		m_i16CosQBp_0 = (((INT32)i16IntAlpha_0)<<12)/m_i16VbpM_0;
	

}

/************************************************************************************
//函数名称：    Dat_BpRMSPower()
************************************************************************************/
void 	Class_Bypass::Dat_BpRMSPower(void)
{	
	INT32	i32temp;
	//旁路相电压有效值计算
	i32temp = m_unVbpaSumM_0.dword/m_i16PointcntBpM_0;
	m_i16VbpaPrms = isqrt(i32temp);
	m_unVbpaRms.dword += ((((INT32)m_i16VbpaPrms)<<16)-m_unVbpaRms.dword)>>K30msFilterCnst;
	
	i32temp = m_unVbpbSumM_0.dword/m_i16PointcntBpM_0;
	m_i16VbpbPrms = isqrt(i32temp);
	m_unVbpbRms.dword += ((((INT32)m_i16VbpbPrms)<<16)-m_unVbpbRms.dword)>>K30msFilterCnst;
	
	i32temp = m_unVbpcSumM_0.dword/m_i16PointcntBpM_0;
	m_i16VbpcPrms = isqrt(i32temp);
	m_unVbpcRms.dword += ((((INT32)m_i16VbpcPrms)<<16)-m_unVbpcRms.dword)>>K30msFilterCnst;
	
	i32temp = (INT32)(abs(m_unDthetaBpBase_0.half.hword));
	m_unDthetaBpMean_0.dword+=( (i32temp<<16) -m_unDthetaBpMean_0.dword)>>K30msFilterCnst;

	i32temp = ((INT32)(m_unDthetaBpMean_0.half.hword))*KFMeanCalCnst;
	m_i16FbpMean = i32temp>>11;
	 
	if((m_i16FbpMean>=3000) &&( m_i16FbpMean<=8000))		//m_i16PointcntBpM_0维护 
	{
		i32temp = ((INT32)5000)*K50HzPointCntCnst;
		m_i16PointcntBpM_0 = i32temp/m_i16FbpMean;
	}
	else
	{
		m_i16PointcntBpM_0 =K30HzPointCntCnst;	
	}


//	//旁路线电压有效值计算
//	i32temp = m_unVbpabSumM_0.dword/m_i16PointcntBpM_0;
//	m_i16VbpabPrms = isqrt(i32temp);
//	m_unVbpabRms.dword += ((((INT32)m_i16VbpabPrms)<<16)-m_unVbpabRms.dword)>>K30msFilterCnst;
//	
//	i32temp = m_unVbpbcSumM_0.dword/m_i16PointcntBpM_0;
//	m_i16VbpbcPrms = isqrt(i32temp);
//	m_unVbpbcRms.dword += ((((INT32)m_i16VbpbcPrms)<<16)-m_unVbpbcRms.dword)>>K30msFilterCnst;
//	
//	i32temp = m_unVbpcaSumM_0.dword/m_i16PointcntBpM_0;
//	m_i16VbpcaPrms = isqrt(i32temp);
//	m_unVbpcaRms.dword += ((((INT32)m_i16VbpcaPrms)<<16)-m_unVbpcaRms.dword)>>K30msFilterCnst;

	
}

/************************************************************************************
//函数名称：    Dat_Int_BpCal()
//功能描述:     旁路电压周期平方和计算
//算法说明:	    1、滤波后的旁路电压经3/2变换后求得正、余弦值，与上一开关周期的相
//		        应值求的相位差，再经滤波后得到旁路矢量角基本步长
//				2、分别将A、B、C相的电压平方累加
//			    3、点数到达时将平方和备份，且将其清零
************************************************************************************/
#pragma CODE_SECTION(".Epwm1Funcs") 			

void 	Class_Bypass::Dat_Int_BpCal(void)
{
	INT16 i16IntAx_0,i16IntBx_0,i16IntCx_0;
	// 计算旁路矢量角基本步长
	m_unVbpaFilter_0.dword = FILTER(2,m_i16Vbpa_0,m_unVbpaFilter_0.dword);
	m_unVbpbFilter_0.dword = FILTER(2,m_i16Vbpb_0,m_unVbpbFilter_0.dword);
	m_unVbpcFilter_0.dword = FILTER(2,m_i16Vbpc_0,m_unVbpcFilter_0.dword);

    // 滤波后的旁路电压矢量模与相位处理
	i16IntAx_0 = (((INT32)m_unVbpaFilter_0.half.hword*2-m_unVbpbFilter_0.half.hword-m_unVbpcFilter_0.half.hword)*K1div3Cnst)>>12;
	i16IntBx_0 = (((INT32)m_unVbpbFilter_0.half.hword-m_unVbpcFilter_0.half.hword)*KSqrt3invCnst)>>12;

	i16IntCx_0 = (((INT32)i16IntAx_0)*i16IntAx_0+((INT32)i16IntBx_0)*i16IntBx_0)>>16;
	UPDNLMT16(i16IntCx_0,1023,1);
	i16IntCx_0 = sqrtTab[i16IntCx_0];
	m_i16SinQBpFilter_0 = (((INT32)i16IntBx_0)<<12)/i16IntCx_0;
	m_i16CosQBpFilter_0 = (((INT32)i16IntAx_0)<<12)/i16IntCx_0;

	i16IntAx_0 = (((INT32)m_i16SinQBpFilter_0)*m_i16CosQBpOld_0-((INT32)m_i16CosQBpFilter_0)*m_i16SinQBpOld_0+256)>>9;// 1.15FORMAT
	m_i16DthetaBpInst_0 = (((INT32)i16IntAx_0)*KRadToDegCnst+1024)>>11;                       // 6.10FORMAT

	//m_unDthetaBpBase_0.dword = FILTER(10,m_i16DthetaBpInst_0,m_unDthetaBpBase_0.dword);

	m_unDthetaTemp_0.dword = FILTER(6,m_i16DthetaBpInst_0,m_unDthetaTemp_0.dword);
	m_unDthetaBpBase_0.dword = FILTER(6,m_unDthetaTemp_0.half.hword,m_unDthetaBpBase_0.dword);
	m_i16SinQBpOld_0 = m_i16SinQBpFilter_0;
	m_i16CosQBpOld_0 = m_i16CosQBpFilter_0;

	m_unVbpMMean_0.dword = FILTER(7,m_i16VbpM_0,m_unVbpMMean_0.dword);


	// 旁路相电压平方和累加12.20格式
	m_unVbpaSum_0.dword += ((((INT32)m_i16Vbpa_0)*m_i16Vbpa_0)>>4);	 
	m_unVbpbSum_0.dword += ((((INT32)m_i16Vbpb_0)*m_i16Vbpb_0)>>4);
	m_unVbpcSum_0.dword += ((((INT32)m_i16Vbpc_0)*m_i16Vbpc_0)>>4);

	// 旁路线电压平方和累加12.20格式
	m_i16Vbpab_0 = m_i16Vbpa_0-m_i16Vbpb_0; 
	m_i16Vbpbc_0 = m_i16Vbpb_0-m_i16Vbpc_0; 
	m_i16Vbpca_0 = m_i16Vbpc_0-m_i16Vbpa_0; 
	m_unVbpabSum_0.dword += ((((INT32)m_i16Vbpab_0)*m_i16Vbpab_0)>>4);	 
	m_unVbpbcSum_0.dword += ((((INT32)m_i16Vbpbc_0)*m_i16Vbpbc_0)>>4);
	m_unVbpcaSum_0.dword += ((((INT32)m_i16Vbpca_0)*m_i16Vbpc_0)>>4);

	m_i16PointcntBp_0++;
	if(m_i16PointcntBp_0 >= m_i16PointcntBpM_0)				     // 旁路过零
	{
		m_unVbpaSumM_0 = m_unVbpaSum_0;     
		m_unVbpbSumM_0 = m_unVbpbSum_0;
		m_unVbpcSumM_0 = m_unVbpcSum_0;
		m_unVbpabSumM_0 = m_unVbpabSum_0;     
		m_unVbpbcSumM_0 = m_unVbpbcSum_0;
		m_unVbpcaSumM_0 = m_unVbpcaSum_0;

		m_i16PointcntBp_0 = 0;

		m_unVbpaSum_0.dword = 0;
		m_unVbpbSum_0.dword = 0;
		m_unVbpcSum_0.dword = 0;
		m_unVbpabSum_0.dword = 0;
		m_unVbpbcSum_0.dword = 0;
		m_unVbpcaSum_0.dword = 0;
		
	}
}

void	Class_Bypass::CheckBypassK(void)
{
if(objBypass.m_i16KVbpb_0 != 4119)
	{
	GpioDataRegs.GPBTOGGLE.bit.GPIO34= 1;
	objBypass.m_i16KVbpb_0 = 4119;
	}

}

void	Class_Bypass::CheckBypass(void)
{
if(objBypass.m_i16KVbpb_0 != 4119)
{
GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1;
}
}
//===========================================================================
// End of file.
//===========================================================================
