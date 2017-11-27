/*==========================================================*/
/* Title		:	Switches.cpp													*/
/* Description	: 	Class_Switches definition										*/
/* Date		:	Apr.2008														*/
/*==========================================================*/
#include "Switches.h"



/************************************************************************************
函数名：	App_SwitchesVarInit()
功能描述：	Breakers类变量初始化
函数调用：
***********************************************************************************/
inline	 void	Class_Switches::App_SwitchesVarInit(void)
	{
		m_st_FlagBreaker.bFuse=1;				//Breaker Intial status
//		m_i16KbackFeed_0 = 0x20A7;
	}

/************************************************************************************
函数名：App_SwitchesParamConfig()
************************************************************************************/
//inline	void	Class_Switches::App_SwitchesParamConfig(void)
//	{
//		m_i16KbackFeed_0 = 0x20A7;
//	}

/************************************************************************************
//函数名称：    Dat_Int_VBackFeedCal()
************************************************************************************/

//#pragma CODE_SECTION(".Epwm1Funcs")

//void 	Class_Switches::Dat_Int_VBackFeedCal(void)
//{
	  //读取采样值
/*	m_i16VbackFeed_0 = objADDrv.m_i16ADC_VbackFeed_0;

	m_i16VbackFeed_0 = ((INT32)m_i16VbackFeed_0*m_i16KbackFeed_0)>>10;

	//旁路反灌保护零地电压有效值累加
	m_unVbackFeedSum_0.dword += (((INT32)m_i16VbackFeed_0*m_i16VbackFeed_0)>>4);

	if(objBypass.m_i16PointcntBp_0 == 0)
	{
		m_unVbackFeedSumM_0 = m_unVbackFeedSum_0;
		m_unVbackFeedSum_0.dword = 0;
	}
*/
//}

/************************************************************************************
//函数名称：    Dat_VBackFeedRMS()
************************************************************************************/
//void 	Class_Switches::Dat_VBackFeedRMS(void)
//{
//	INT32	i32temp;
	//旁路反灌保护零地电压有效值计算
//	i32temp = m_unVbackFeedSumM_0.dword/objBypass.m_i16PointcntBpM_0;
//	m_i16VbackFeedPrms = isqrt(i32temp);
//	m_unVbackFeedRms.dword += (((INT32)m_i16VbackFeedPrms<<16)-m_unVbackFeedRms.dword)>>K20msFilterCnst;

//}

/************************************************************************************
函数名：	Dat_BreakerStatus()
功能描述：	开关状态处理模块，根据相应输入端口的信号，系统当前的状态来确定各个空开、熔断器
			及静态开关所处的状态
函数调用：	无
************************************************************************************/

	void	Class_Switches::Dat_BreakerStatus()
	{

		//并机信号备份，增强外部输出开关及维修旁路信号的抗扰性
	    m_st_FlagBreaker.bQ3bp = (objDigitalIO.m_st_HardwareSignal.bQ3BP |
	    						(objParallel.m_st_wParaSignalF.bCabQ3BpStatusF & objParallel.m_st_wParaSignalFBak.bCabQ3BpStatusF))
	    				    &(!objSystem.m_st_wFlagSystem.bUPSTestMode);            //调测模式，认为Q3BP断开

		if(objMonInterface.m_st_wUPSModeRun.bConfig == 3)
		{
		  	m_st_FlagBreaker.bQ5 = (objDigitalIO.m_st_HardwareSignal.bQ5
		  	                     & (objParallel.m_st_wParaSignalF.bExtOutStatusF | objParallel.m_st_wParaSignalFBak.bExtOutStatusF))
		  	                  |objSystem.m_st_wFlagSystem.bUPSTestMode;			 //调测模式，认为Q5闭合
		}
		else
		{
	    		m_st_FlagBreaker.bQ5 = objDigitalIO.m_st_HardwareSignal.bQ5 | objSystem.m_st_wFlagSystem.bUPSTestMode; //调测模式，认为Q5闭合
		}

        m_st_FlagBreaker.bQ5 = 1;


		//zqf051020-输出熔丝断用Nxe的判据
		if ( objDigitalIO.m_st_HardwareSignal.bFuse == 0 )									//Fuse断
		{
			m_st_FlagBreaker.bFuse = 0;
			if( objBypass.m_st_FlagVbpTotalWide.bBpRange <= 1 )
			{
				objInvCtrl.m_st_wFlagInvOnoff.bNeverRecoverOff = 1;							//旁路宽范围正常，置不可恢复关机命令
			}
			else if(objMonInterface.m_st_wParallelData.iNumOn >1)				//NXr系列输出熔丝在供电台数大于1台时退出供电
			{
				objInvCtrl.m_st_wFlagInvOnoff.bNeverRecoverOff = 1;
			}

		}

		m_st_FlagBreaker.bInvKM = objXfer.m_st_XferResult.bInvKM;						//逆变STS
		m_st_FlagBreaker.bBpSTS = objXfer.m_st_XferResult.bBpSTS;							//旁路STS

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


		//此处条件可能不足，待补充
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

		//修改旁路供电的定义，已与面板状态一致
		m_st_FlagBreaker.bBpOn = (objSystem.m_st_wFlagStatus.bSupply == 1);


		//ZDS20030827 并机信号备份，增强外部输出开关及维修旁路信号的抗扰性
//		S2U(objParallel.m_st_wParaSignalFBak) = S2U(objParallel.m_st_wParaSignalF);

	}

/************************************************************************************
函数名：	Alg_Int_BpSTSShort()
功能描述：
函数调用：	无
************************************************************************************/
/*
	#pragma CODE_SECTION(".Epwm1Funcs")

	void	Class_Switches::Alg_Int_BpSTSShort(void)
	{
		INT16	i16TempShortIa;
		INT16	i16TempShortIb;
		INT16	i16TempShortIc;
		//解决逆变电流与输出电流因不同轮采样和硬件通道延时差异，导致误报“旁路晶闸管故障”问题
		m_unIinvaFilter_0.dword = FILTER(0, objInverter.m_i16Iinva_0, m_unIinvaFilter_0.dword);
		m_unIinvbFilter_0.dword = FILTER(0, objInverter.m_i16Iinvb_0, m_unIinvbFilter_0.dword);
		m_unIinvcFilter_0.dword = FILTER(0, objInverter.m_i16Iinvc_0, m_unIinvcFilter_0.dword);

		i16TempShortIa = objOutput.m_i16Iouta_0- m_unIinvaFilter_0.half.hword;
		i16TempShortIb = objOutput.m_i16Ioutb_0- m_unIinvbFilter_0.half.hword;
		i16TempShortIc = objOutput.m_i16Ioutc_0- m_unIinvcFilter_0.half.hword;

		if(objXfer.m_st_XferResult.bBpSTS == 0)//Q2合，旁路不供电
		{
			//电流差30％，采用加2减1计数
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
			objInvAlg.m_st_wIntMainFlag.bBpSTSShort = 1;							//置旁路晶闸管短路标志
		}
	}
*/
/************************************************************************************
函数名：	Alg_Int_STSDiffFastChk()
功能描述：	输出异常快速检测，添加此模块用于晶闸管断路时快速检测并切换
函数调用：	无
************************************************************************************/

#pragma CODE_SECTION(".Epwm1Funcs")

	void	Class_Switches::Alg_Int_STSDiffFastChk(void)
	{
		INT16	i16IntAx_0,i16IntBx_0,i16IntCx_0,i16IntDx_0,i16IntEx_0,i16IntFx_0;

		if(((objInvAlg.m_st_wIntMainFlag.bVbpFastChk == 1) ||(objInvAlg.m_st_wIntMainFlag.bVinvFastChk == 1)) //逆变或旁路快速检测中？
			&& ((objBypass.m_st_FlagVbpTotal.bBps10Fast == 1) || ((objInvAlg.m_st_wIntMainFlag.bVinvFast == 0)&&(objInvAlg.m_st_wIntMainFlag.bVbpFast == 0)))
			&& (objXfer.m_st_wXferCmd.bSwitchMode!=2))	//旁路10s快检中或逆变、旁路快检正常，且非间断切换
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
函数名：	Alg_Int_INVKMFastChk()
功能描述：	输出异常快速检测，添加此模块用于晶闸管断路时快速检测并切换
函数调用：	无
************************************************************************************/

#pragma CODE_SECTION(".Epwm1Funcs")

	void	Class_Switches::Alg_Int_INVKMFastChk(void)
	{
		INT16	i16SoutaTotalMean;			//平均输出功率
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
			||((objInverter.m_i16Sinvc < 102)&&(i16SoutcTotalMean>307)))    //某相的功率小于10%且平均功率大于30%
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
函数名：	Alg_STSShortFault()
功能描述：	晶闸管短路故障
函数调用：	无
************************************************************************************/
	void	Class_Switches::Alg_STSShortFault(void)
	{
		if( objSystem.m_st_wFlagSystem.bTimer14ms == 1)
		{
			if(objInverter.m_st_wFlagInv.bInv == 0)
			{
				if( ( ( objOutput.m_i16VoutaPrms > 559 ) || ( objOutput.m_i16VoutbPrms > 559 ) || ( objOutput.m_i16VoutcPrms > 559 ) )
				&&(objXfer.m_st_XferResult.bInvKM == 0))						//输出电压大于120v且本机继电器没有吸合，防止旁路超保护时误报
				{
					if((abs(objInverter.m_i16Vinva_0 - objOutput.m_i16Vouta_0)< 372)				//逆变电压与旁路电压瞬时值相差20v
					||(abs(objInverter.m_i16Vinvb_0 - objOutput.m_i16Voutb_0)< 372)
					||(abs(objInverter.m_i16Vinvc_0 - objOutput.m_i16Voutc_0)< 372))
					{
						if ( ( objInverter.m_i16IinvaPrms > 45 ) || ( objInverter.m_i16IinvbPrms > 45 ) || ( objInverter.m_i16IinvcPrms > 45 ) )
						{												//逆变电流大于额定电流的 4.4%
							m_i16InvSTSFault1CntPer14ms ++;
						}

					}
					else
					{
						if (( m_i16InvSTSFault1CntPer14ms >0)  && (m_i16InvSTSFault1CntPer14ms <= 36 ))
						{
							m_i16InvSTSFault1CntPer14ms --;					//逆变晶闸管故障确认计数递减
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
		if ( objSystem.m_st_wFlagSystem.bTimer40ms == 1 )							//40ms定时到
		{
//旁路晶闸管短路禁止计数器---当三相电压全有时,不通过检输出电压报旁路晶闸管故障
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
					m_i16InvSTSFaultDisableCntPer40ms --;					//逆变晶闸管故障确认计数递减
				}
			}



			//条件1：（（单机且Q3bp断））或（并机且Q3bp断））且（旁路逆变STS都关断），输出有电压
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
																	//输出电压>120v
				{
					m_i16BpSTSForwardFaultCntPer40ms ++;						//旁路晶闸管故障确认计数FEED FORWARD
					if ( m_i16BpSTSForwardFaultCntPer40ms >= 25 )				// 1s
					{
						m_i16BpSTSForwardFaultCntPer40ms = 25;

						if(objSwitches.m_st_FlagBreaker.bBpSTSShort==0 && objSwitches.m_st_FlagBreaker.bInvKMShort == 0 )
						{
							if ( objInverter.m_st_wFlagInv.bInv == 0)	
							{
								objSwitches.m_st_FlagBreaker.bBpSTSShort = 1;		//置旁路晶闸管短路标志
								if((objMonInterface.m_st_wUPSModeRun.bConfig == 0)||(objDigitalIO.m_st_wHardwareNewSignal.bBPBackFeedDetect ==1))
								{
									objSwitches.m_st_FlagBreaker.bReqBpSTSShort = 0;				//本机
								}
								else
								{
									objSwitches.m_st_FlagBreaker.bReqBpSTSShort = 1;				//邻机
								}
								objDebug.m_st_wDebug.bBpSTSShort1 = 1;
	    					//置不可恢复关机标志

							}
 							else			//Q2断 逆变器开启
 							{
 								objSwitches.m_st_FlagBreaker.bInvKMShort = 1;		//置逆变晶闸管短路标志
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
				m_i16BpSTSForwardFaultCntPer40ms = 0;							//旁路晶闸管故障确认计数FEED FORWARD清零
			}
				

			//逆变晶闸管短路判据
			//条件2：逆变不开机，有逆变电压和逆变电流
			if ( objInverter.m_st_wFlagInv.bInv > 0 ) 								//逆变开机
			{
				m_i16InvSTSFaultCntPer40ms =0;
			}
			else
			{
				if( ( objInverter.m_i16VinvaPrms > 559 ) || ( objInverter.m_i16VinvbPrms > 559 ) || ( objInverter.m_i16VinvcPrms > 559 ) )
																	//逆变电压大于 120V
				{
					m_i16InvSTSFaultCntPer40ms ++;

					}
					else
					{
						if ( m_i16InvSTSFaultCntPer40ms > 0  && m_i16InvSTSFaultCntPer40ms <15000 )
						{
							m_i16InvSTSFaultCntPer40ms --;					//逆变晶闸管故障确认计数递减
						}
					}

				}

			}
			if (( m_i16InvSTSFaultCntPer40ms >= 15000 )||(m_i16InvSTSFault1CntPer14ms >=36))			//10min
			{
				m_i16InvSTSFaultCntPer40ms = 15000;
				m_i16InvSTSFault1CntPer14ms = 36;
				objSwitches.m_st_FlagBreaker.bInvKMShort = 1;			//置逆变晶闸管短路标志
				objDebug.m_st_wDebug.bInvKMShort2 = 1;
			}
	

			//旁路晶闸管短路判据
			//条件2：旁路不发驱动，反灌CT产生硬件信号，且输出有10%的电流
			if( objSystem.m_st_wFlagSystem.bTimer40ms == 1 )
			{
				if(objDigitalIO.m_st_wLocalSignal.bBpSTS == 0)
				{
					//单机反灌确认，用于锁存标志bBpSTSShortLatch
					if((objDigitalIO.m_st_wHardwareNewSignal.bBPBackFeedDetect ==1)							//有旁路反灌IO信号
						&&(( objOutput.m_i16IoutaPrms  > 102)			//输出电流大于10%的额定
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
				objSwitches.m_st_FlagBreaker.bBpSTSShort = 1;			//置旁路晶闸管短路标志
				objDebug.m_st_wDebug.bBpSTSShort2 = 1;
				objInvCtrl.m_st_wFlagInvOnoff.bNeverRecoverOff=1;	   //置不可恢复关机标志
			}


	

		if((objInvAlg.m_st_wIntMainFlag1_0.bBpInvOverCurrent == 1) && (objInvAlg.m_st_wIntMainFlag1_0.bBpInvOverCurrentReason == 0)) //快检主旁共通
		{
			objSwitches.m_st_FlagBreaker.bBpSTSShort = 1;			//置旁路晶闸管短路标志
			objSwitches.m_st_FlagBreaker.bReqBpSTSShort = 0;				//本机
			objDebug.m_st_wDebug.bBpSTSShort3 = 1;
			objInvCtrl.m_st_wFlagInvOnoff.bNeverRecoverOff=1;	    					//置不可恢复关机标志
		}

		if((objInvAlg.m_st_wIntMainFlag1_0.bBpInvOverCurrent == 1) && (objInvAlg.m_st_wIntMainFlag1_0.bBpInvOverCurrentReason == 1)) //快检主旁共通
		{
			objSwitches.m_st_FlagBreaker.bInvKMShort = 1;
		}
///////////////////////////////////////////////


/*
		//旁路反灌保护功能
		if(objSystem.m_st_wFlagSystem.bTimer14ms == 1)
		{
			if((objDigitalIO.m_st_HardwareSignal.bBpSTSAct ==0)&& (objXfer.m_st_XferResult.bBpSTS==0)
			 && (objBypass.m_st_FlagVbpTotal.bBpRange==3) && (objSwitches.m_st_FlagBreaker.bQ2==1)
			 &&	(objOutput.m_i16VoutaPrms>739) && (objOutput.m_i16VoutbPrms>739) && (objOutput.m_i16VoutcPrms>739)
			 && (objBypass.m_i16VbpaPrms<=444) && (objBypass.m_i16VbpbPrms<=444) && (objBypass.m_i16VbpcPrms<=444))  	//输出电压大于200V，旁路电压小于120V
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
					objDigitalIO.m_st_wLocalSignal.bBackFeed=1;							//反灌保护CB脱扣信号，送给检测板
					//zqf 060918--旁路晶闸管短路和旁路反灌保护区分开
					objSwitches.m_st_FlagBreaker.bBackFeed=1;							//送给监控板，以及用以触发故障记录
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
			objInvCtrl.m_st_wFlagInvOnoff.bNeverRecoverOff=1;	    					//置不可恢复关机标志
		}
	}

/************************************************************************************
函数名：	Alg_STSOpenFault()
功能描述：	晶闸管断路故障
函数调用：	无
************************************************************************************/
	void	Class_Switches::Alg_STSOpenFault(void)
	{

		if ( objSystem.m_st_wFlagSystem.bTimer40ms == 1 )				//40ms定时到
		{

			//旁路STS断路故障判断条件:旁路STS导通且某相旁路电压和输出电压差持续超过15%,5s确认时间
			if (( objXfer.m_st_XferResult.bBpSTS == 1 )
			    &&(objDigitalIO.m_st_wLocalSignal.bBPSF == 1)
			    &&(objBypass.m_st_FlagVbpTotal.bBpSTSOpenChkEnable==1)
			    &&((abs(objOutput.m_i16VoutaPrms-objBypass.m_i16VbpaPrms)>154)
				||(abs(objOutput.m_i16VoutbPrms-objBypass.m_i16VbpbPrms)>154)
				||(abs(objOutput.m_i16VoutcPrms-objBypass.m_i16VbpcPrms)>154)))			//旁路电压和输出电压差超过10%
			{
				m_i16BpSTSOpenCntPer40ms ++;										//旁路晶闸管断路计数器递增
				if ( m_i16BpSTSOpenCntPer40ms >=125 )								//5s
				{
					m_i16BpSTSOpenCntPer40ms = 125;
					objSwitches.m_st_FlagBreaker.bBpSTSOpen = 1;			//置旁路晶闸管断路标志
					objDebug.m_st_wDebug.bBpSTSOpen1 = 1;
				}
			}
			else
			{
				if ( m_i16BpSTSOpenCntPer40ms > 0 && m_i16BpSTSOpenCntPer40ms <125 )
				{
					m_i16BpSTSOpenCntPer40ms --;									//旁路晶闸管断路计数器递减
				}
			}

			//逆变STS断路故障判断条件:逆变STS导通且某相逆变电压和输出电压差持续超过15% 确认5秒
			if ((objXfer.m_st_XferResult.bInvKM == 1) &&((abs(objOutput.m_i16VoutaPrms-objInverter.m_i16VinvaPrms)>154)
				||(abs(objOutput.m_i16VoutbPrms-objInverter.m_i16VinvbPrms)>154)
				||(abs(objOutput.m_i16VoutcPrms-objInverter.m_i16VinvcPrms)>154)))


			{
				m_i16InvSTSOpenCntPer40ms ++;										//逆变晶闸管断路计数器递增
				if ( m_i16InvSTSOpenCntPer40ms >=25*5 )							//5s
				{
					m_i16InvSTSOpenCntPer40ms = 25*5;
					//objSwitches.m_st_FlagBreaker.bInvKMOpen = 1;						//媚姹渚д⒐芏下繁曛?
					objDebug.m_st_wDebug.bInvKMOpen1 = 1;
				}
			}
			else
			{
				if ( m_i16InvSTSOpenCntPer40ms > 0 && m_i16InvSTSOpenCntPer40ms <25*5 )
				{
					m_i16InvSTSOpenCntPer40ms --;									//逆变晶闸管断路计数器递减
				}
			}
		}

		//利用输出电压快速检测来快速判断晶闸管断路
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
			objSwitches.m_st_FlagBreaker.bInvKMOpen=0;			 //熔丝断后屏蔽逆变晶闸管断路故障
		}

		if ( ( objXfer.m_st_wXferCmd.bSwitchStatus == 1 ) && ( objSwitches.m_st_FlagBreaker.bInvKMOpen == 1 )
//			&& ( objBypass.m_st_FlagVbpTotalWide.bBpRange <=1 ) && ( objSwitches.m_st_FlagBreaker.bBpSTSOpen == 0 ) )
			&& ( objSwitches.m_st_FlagBreaker.bBpSTSOpen == 0 ) )
		{
			if ( objBypass.m_st_FlagVbpTotalWide.bBpRange <=1 )
			{
				objInvCtrl.m_st_wFlagInvOnoff.bNeverRecoverOff = 1;							//置不可恢复关机标志
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
