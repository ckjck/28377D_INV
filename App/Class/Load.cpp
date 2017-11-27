/*==========================================================*/ 	
/* Title		:	Load.cpp														*/ 	
/* Description	: 	Class_Load definition											*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#include "Load.h"


/************************************************************************************
函数名：	Dat_LoadDeal()
功能描述：	负载判别模块，负载大小判断及过载延时处理
函数调用：	Dat_LoadFlag():		负载标志处理模块，根据当前本机逆变各相负载的大小，加工成本机逆变负载大小的标志
			Dat_OvloadFlag():		过载确认模块，根据负载瞬时过载的情况，加工出确认后的过载标志
			App_OvloadDelay():	过载延时处理模块，根据负载过载和负载大小的情况，加工处理出过载延时到标志
			App_OvloadSignal():	过载信号处理模块，用于控制并机总线信号INV_OVERLOAD的发送
************************************************************************************/

	void	Class_Load::Dat_LoadDeal(void)
	{
		Dat_LoadGrant();				//根据环境温度对有功功率过载判断进行降额

		Alg_ShortCircuitDetect();

		Dat_LoadFlag();
		
		Dat_OvloadFlag();
		
		App_OvloadDelay();
		
		App_OvloadSignal();

	}

/************************************************************************************
函数名：	Dat_LoadFlag()
功能描述：	负载标志处理模块，根据当前本机逆变各相负载的大小，加工成本机逆变负载大小的标志
函数调用：	igap()				回差处理子程序
************************************************************************************/
	void	Class_Load::Dat_LoadGrant()
{
		m_i16PinvaGrant = ((INT32)(objInverter.m_i16Pinva)*objInverter.m_i16TempKWGrant)>>10;
		m_i16PinvbGrant = ((INT32)(objInverter.m_i16Pinvb)*objInverter.m_i16TempKWGrant)>>10;
		m_i16PinvcGrant = ((INT32)(objInverter.m_i16Pinvc)*objInverter.m_i16TempKWGrant)>>10;
		
		m_i16SinvaGrant = ((INT32)(objInverter.m_i16Sinva)*objInverter.m_i16TempKVAGrant)>>10;
		m_i16SinvbGrant = ((INT32)(objInverter.m_i16Sinvb)*objInverter.m_i16TempKVAGrant)>>10;
		m_i16SinvcGrant = ((INT32)(objInverter.m_i16Sinvc)*objInverter.m_i16TempKVAGrant)>>10;
		
		m_i16PSinvaGrantMAX = MAX2(m_i16PinvaGrant,m_i16SinvaGrant);
		m_i16PSinvbGrantMAX = MAX2(m_i16PinvbGrant,m_i16SinvbGrant);	
		m_i16PSinvcGrantMAX = MAX2(m_i16PinvcGrant,m_i16SinvcGrant);	
		
		m_i16PoutaGrant = ((INT32)(objOutput.m_i16Pouta)*objInverter.m_i16TempKWGrant)>>10;
		m_i16PoutbGrant = ((INT32)(objOutput.m_i16Poutb)*objInverter.m_i16TempKWGrant)>>10;
		m_i16PoutcGrant = ((INT32)(objOutput.m_i16Poutc)*objInverter.m_i16TempKWGrant)>>10;
		
		m_i16SoutaGrant = ((INT32)(objOutput.m_i16Souta)*objInverter.m_i16TempKVAGrant)>>10;
		m_i16SoutbGrant = ((INT32)(objOutput.m_i16Soutb)*objInverter.m_i16TempKVAGrant)>>10;
		m_i16SoutcGrant = ((INT32)(objOutput.m_i16Soutc)*objInverter.m_i16TempKVAGrant)>>10;
		
		m_i16PSoutaGrantMAX = MAX2(m_i16PoutaGrant,m_i16SoutaGrant);
		m_i16PSoutbGrantMAX = MAX2(m_i16PoutbGrant,m_i16SoutbGrant);	
		m_i16PSoutcGrantMAX = MAX2(m_i16PoutcGrant,m_i16SoutcGrant);				
}
/************************************************************************************
函数名：	Alg_ShortCircuitDetect()
功能描述：输出短路故障判断模块
函数调用：	无			
************************************************************************************/
	void	Class_Load::Alg_ShortCircuitDetect(void)
	{

		//逆变器开启完毕，某线电压低于110V且电流大于0.7倍额定值
		//Nxb+突加整流满载，逆变电压在一个工频周期内会被严重拉跨。
		if ( ( objInverter.m_st_wFlagInv.bInv == 3 ) 
			&& ( ( (( objInverter.m_i16VinvaPrms < 370 )||(objInverter.m_i16VinvabPrms<370))&& ( objInverter.m_i16IinvaPrms > 717 ))
			|| ( (( objInverter.m_i16VinvbPrms < 370 )||(objInverter.m_i16VinvbcPrms<370))  && ( objInverter.m_i16IinvbPrms > 717  ))
			|| ( (( objInverter.m_i16VinvcPrms < 370 ) ||(objInverter.m_i16VinvcaPrms<370))&& ( objInverter.m_i16IinvcPrms > 717  ) )) )
											//在线间短路时增加电线压判断
		{
			objSystem.m_st_wFlagPrompt.bShortCircuit = 1;									//置逆变输出短路标志
		}
		else
		{
			objSystem.m_st_wFlagPrompt.bShortCircuit = 0;									//清逆变输出短路标志
		}
	}
/************************************************************************************
函数名：	Dat_LoadFlag()
功能描述：	负载标志处理模块，根据当前本机逆变各相负载的大小，加工成本机逆变负载大小的标志
函数调用：	igap()				回差处理子程序
************************************************************************************/
	void	Class_Load::Dat_LoadFlag()

	{
		INT16	i16MainAX,i16MainCX;
		//本机逆变侧供电
		if((objSystem.m_st_wFlagPrompt.bShortCircuit == 1) 
			||(m_st_wFlagLoad.bImpactSwitch == 1) 
			||((objXfer.m_st_wXferCmd.bSwitchStatus == 1) && (m_st_FlagInvLoadCT.bLoadTotalT<2)))
		{
			m_i16Powerbacka = 0;
			m_i16Powerbackb = 0;
			m_i16Powerbackc = 0;
		}
		else if( (objXfer.m_st_wXferCmd.bSwitchStatus == 1 )
			&& (m_st_FlagInvLoadCT.bLoadTotalT > 2)
			&& (m_i32OvloadDlyCntPer40ms > 10000)
			&& (m_i32OvloadDlyCntPer40ms < 80000))	
		{
			m_i16Powerbacka = objOutput.m_i16wPFoutLeada-1024;
			m_i16Powerbackb = objOutput.m_i16wPFoutLeadb-1024;
			m_i16Powerbackc = objOutput.m_i16wPFoutLeadc-1024;
		}
		
		if ( objSystem.m_i16wNumMainloop == 5 )									//本机逆变负载标志处理
		{
			i16MainCX = m_st_FlagInvLoadAB.bLoadA;								//本机逆变A相负载标志
			m_st_FlagInvLoadAB.bLoadA = igap ( m_i16PSinvaGrantMAX, &InvLoadTable, i16MainCX );//调用回差处理函数

			i16MainCX = m_st_FlagInvLoadAB.bLoadB;								//本机逆变B相负载标志
			m_st_FlagInvLoadAB.bLoadB = igap ( m_i16PSinvbGrantMAX, &InvLoadTable, i16MainCX );//调用回差处理函数

			i16MainCX = m_st_FlagInvLoadCT.bLoadC;						//本机逆变C相负载标志
			m_st_FlagInvLoadCT.bLoadC = igap ( m_i16PSinvcGrantMAX, &InvLoadTable, i16MainCX );//调用回差处理函数

			//逆变电感功率过载处理及标志合成
			i16MainCX = m_st_FlagILLoadAB.bLoadA;							//本机逆变电感A相负载标志
			m_st_FlagILLoadAB.bLoadA = igap ( objInverter.m_i16SLinva, &InvLoadTable, i16MainCX );	//调用回差处理函数

			i16MainCX = m_st_FlagILLoadAB.bLoadB;							//本机逆变电感B相负载标志
			m_st_FlagILLoadAB.bLoadB = igap ( objInverter.m_i16SLinvb, &InvLoadTable, i16MainCX );	//调用回差处理函数

			i16MainCX = m_st_FlagILLoadCT.bLoadC;							//本机逆变电感C相负载标志
			m_st_FlagILLoadCT.bLoadC = igap ( objInverter.m_i16SLinvc, &InvLoadTable, i16MainCX );	//调用回差处理函数


			m_st_FlagILLoadCT.bLoadT = m_st_FlagILLoadAB.bLoadA | m_st_FlagILLoadAB.bLoadB
									| m_st_FlagILLoadCT.bLoadC;				//本机逆变电感过载总标志综合

//			if(m_st_FlagILLoadCT.bLoadT == 7)
//			{
//				m_st_FlagILLoadCT.bLoadT = 15;
//			}
//			else if(m_st_FlagILLoadCT.bLoadT == 15)
//			{
//				m_st_FlagILLoadCT.bLoadT = 31;
//			}
			
			m_st_FlagInvLoadCT.bLoadT = m_st_FlagInvLoadAB.bLoadA | m_st_FlagInvLoadAB.bLoadB
									| m_st_FlagInvLoadCT.bLoadC ;							//本机逆变负载总标志综合

			m_st_FlagInvLoadCT.bLoadTotalT = m_st_FlagInvLoadCT.bLoadT  | m_st_FlagILLoadCT.bLoadT;  	//电感及逆变器过载总标志
//			m_st_FlagInvLoadCT.bLoadTotalT = m_st_FlagInvLoadCT.bLoadT ;

			if((m_st_FlagInvLoadAB.bLoadA>2)||(m_st_FlagILLoadAB.bLoadA>2))	//A相输出KVA或KW过载
			{
				//判断A相是否有输出电感过载,如有,记下功率因数值
				if((objInverter.m_i16SLinva > objInverter.m_i16IinvaPrms) && (objInverter.m_i16SLinva < objInverter.m_i16IinvaPrms*2))	//条件中已排除被除数为0的情况
				{					
					objOutput.m_i16wPFoutLeada = ((INT32)objInverter.m_i16SLinva*1024)/objInverter.m_i16IinvaPrms;
				}
				else
				{				
					objOutput.m_i16wPFoutLeada = 1024;
				}
			}
			else
			{
				objOutput.m_i16wPFoutLeada = 1024;
			}

			if((m_st_FlagInvLoadAB.bLoadB > 2)||(m_st_FlagILLoadAB.bLoadB > 2))	//B相输出KVA或KW过载
			{
				//判断B相是否有输出电感过载,如有,记下功率因数值
				if((objInverter.m_i16SLinvb > objInverter.m_i16IinvbPrms) && (objInverter.m_i16SLinvb < objInverter.m_i16IinvbPrms*2))	//条件中已排除被除数为0的情况

				{
					objOutput.m_i16wPFoutLeadb = ((INT32)objInverter.m_i16SLinvb*1024)/objInverter.m_i16IinvbPrms;
				}
				else
				{				
					objOutput.m_i16wPFoutLeadb = 1024;
				}
			}
			else
			{
				objOutput.m_i16wPFoutLeadb = 1024;
			}

			if((m_st_FlagInvLoadCT.bLoadC > 2)||(m_st_FlagILLoadCT.bLoadC > 2))	//C相输出KVA或KW过载
			{

				//判断C相是否有输出电感过载,如有,记下功率因数值
				if((objInverter.m_i16SLinvc > objInverter.m_i16IinvcPrms) && (objInverter.m_i16SLinvc < objInverter.m_i16IinvcPrms*2))	//条件中已排除被除数为0的情况

				{
					objOutput.m_i16wPFoutLeadc = ((INT32)objInverter.m_i16SLinvc*1024)/objInverter.m_i16IinvcPrms;
				}
				else
				{				
					objOutput.m_i16wPFoutLeadc = 1024;
				}
			}
			else
			{
				objOutput.m_i16wPFoutLeadc = 1024;
			}
		
		}
		
		
		if ( objSystem.m_i16wNumMainloop == 6 )											//本机输出负载标志处理
		{
		
			if(m_i16Powerbacka <= 50)
			{
				m_i16Powerbacka = 0;
			}
			else if(m_i16Powerbacka >= 200)
			{
				m_i16Powerbacka = 200;
			}
			
			if(m_i16Powerbackb <= 50)
			{
				m_i16Powerbackb = 0;
			}
			else if(m_i16Powerbackb >= 200)
			{
				m_i16Powerbackb = 200;
			}
			
			if(m_i16Powerbackc <= 50)
			{
				m_i16Powerbackc = 0;
			}
			else if(m_i16Powerbackc >= 200)
			{
				m_i16Powerbackc = 200;
			}

			i16MainAX = m_i16PSoutaGrantMAX+m_i16Powerbacka;

			i16MainCX = m_st_FlagOutLoadABCT.bLoadA;								//本机输出A相负载标志
			m_st_FlagOutLoadABCT.bLoadA = igap ( i16MainAX, &OutLoadTable, i16MainCX );	//调用回差处理函数

			i16MainAX=m_i16PSoutbGrantMAX+m_i16Powerbackb;
			i16MainCX = m_st_FlagOutLoadABCT.bLoadB;								//本机输出B相负载标志
			m_st_FlagOutLoadABCT.bLoadB = igap ( i16MainAX, &OutLoadTable, i16MainCX );	//调用回差处理函数

			i16MainAX=m_i16PSoutcGrantMAX+m_i16Powerbackc;
			i16MainCX = m_st_FlagOutLoadABCT.bLoadC;								//本机输出C相负载标志
			m_st_FlagOutLoadABCT.bLoadC = igap ( i16MainAX, &OutLoadTable, i16MainCX );	//调用回差处理函数

			m_st_FlagOutLoadABCT.bLoadT = m_st_FlagOutLoadABCT.bLoadA | m_st_FlagOutLoadABCT.bLoadB
										| m_st_FlagOutLoadABCT.bLoadC;									//本机输出负载总标志综合
		}
	}

/************************************************************************************
函数名：	Dat_OvloadFlag()
功能描述：	过载确认模块，根据负载瞬时过载的情况，加工出确认后的过载标志
函数调用：	无
************************************************************************************/

	void	Class_Load::Dat_OvloadFlag()

	{
		if ( objSystem.m_st_wFlagSystem.bTimer40ms == 1 )									//40ms定时到
		{
			if ( m_st_wFlagLoad.bPSoutHigh == 1 )								//输出检测过载
			{
				if ( ( m_st_FlagOutLoadABCT.bLoadT == 0 ) 
					&& ( objSystem.m_st_wFlagPrompt.bShortCircuit == 0 ) )					//输出负载<95%，且非瞬时短路
				{
					m_st_wFlagLoad.bPSoutHigh = 0;								//清输出检测过载标志
					m_i16PSoutHighConfCntPer40ms = 0;									//清输出过载确认计数
				}		
			}
			else															//输出检测不过载
			{
				if ( ( m_st_FlagOutLoadABCT.bLoadT == 7 ) 
					|| ( objSystem.m_st_wFlagPrompt.bShortCircuit == 1 ) )					//输出负载>105%或瞬时短路
				{
					m_i16PSoutHighConfCntPer40ms +=2;									//输出过载确认计数递增
					
//					if ( objSystem.m_st_wFlagPrompt.bShortCircuit == 1 )					//瞬时短路
//					{
//						m_i16PSoutHighConfCntPer40ms ++;	
//					}
									
					if ( m_i16PSoutHighConfCntPer40ms >= 6 )
					{
						m_st_wFlagLoad.bPSoutHigh = 1;							//置输出检测过载标志
						m_i16PSoutHighConfCntPer40ms = 6;
					}
				}
				else
				{
					m_i16PSoutHighConfCntPer40ms = 0;									//清输出过载确认计数
				}
			}
		}

		if ( objXfer.m_st_wXferCmd.bSwitchStatus == 1 )								//本机逆变侧供电
		{
			if ( objSystem.m_st_wFlagSystem.bTimer40ms == 1 )								//40ms定时到
			{
				if ( m_st_wFlagLoad.bOvload == 1 )								//输出总过载
				{
					if ( ( m_st_FlagInvLoadCT.bLoadTotalT == 0 ) 
						&& ( objSystem.m_st_wFlagPrompt.bShortCircuit == 0 ) )				//逆变负载<95%，且非瞬时短路
					{
						m_st_wFlagLoad.bOvload = 0;								//清输出总过载标志
						m_i16OvloadConfCntPer40ms = 0;									//逆变过载确认计数清零
					}		
				}
				else														//输出不过载
				{
					if ( ( m_st_FlagInvLoadCT.bLoadTotalT > 2 ) 
						|| ( objSystem.m_st_wFlagPrompt.bShortCircuit == 1 ) )				//逆变负载>105%或瞬时短路
					{
						m_i16OvloadConfCntPer40ms +=2;									//逆变过载确认计数递增
						
						if ( objSystem.m_st_wFlagPrompt.bShortCircuit == 1 )					//瞬时短路
						{
							m_i16OvloadConfCntPer40ms ++;		
						}
						
						if ( m_i16OvloadConfCntPer40ms >= 8 )
						{
							m_st_wFlagLoad.bOvload = 1;							//置输出总过载标志
							m_i16OvloadConfCntPer40ms = 8;
						}
					}
					else
					{
						m_i16OvloadConfCntPer40ms = 0;									//清逆变过载确认计数
					}
				}
			}
		}
		else																//本机旁路侧供电
		{
			m_i16OvloadConfCntPer40ms = 0;												//逆变过载确认系数清零
			if(objXfer.m_st_XferResult.bBpSTS == 1)
			{
				m_st_wFlagLoad.bOvload = m_st_wFlagLoad.bPSoutHigh;						//输出总过载标志
			}	
			else
			{
				m_st_wFlagLoad.bOvload = 0;
			}
		}
	}


/************************************************************************************
函数名：	App_OvloadDelay()
功能描述：	过载延时处理模块，根据负载过载和负载大小的情况，加工处理出过载延时到标志
函数调用：	无
************************************************************************************/

	void	Class_Load::App_OvloadDelay()

	{
		if ( objXfer.m_st_wXferCmd.bSwitchStatus == 1 )								//本机逆变侧供电
		{
			if ( objSystem.m_st_wFlagSystem.bTimer40ms == 1 )								//40ms定时到
			{
				if ( m_st_wFlagLoad.bOvload == 0 )								//输出不过载
				{
					m_i32OvloadDlyCntPer40ms -= 60;
					if ( m_i32OvloadDlyCntPer40ms < 0 )								//限幅于零
					{
						m_i32OvloadDlyCntPer40ms = 0;
					}
				}
				else														//输出总过载
				{
					if ( ( m_st_FlagInvLoadCT.bLoadTotalT == 31 ) 
						|| ( objSystem.m_st_wFlagPrompt.bShortCircuit == 1 ) )				//逆变负载>150%或瞬时短路 
					{
						m_i32OvloadDlyCntPer40ms += 30600;

					}
					else if (m_st_FlagInvLoadCT.bLoadTotalT == 15 )					//150%>逆变负载>125%
					{
						m_i32OvloadDlyCntPer40ms += 60;
					}
					else															//125%>逆变负载>105%
					{
						m_i32OvloadDlyCntPer40ms += 12;
					}
					
					//过载延时不足，1小时补1分钟
					if ( m_i32OvloadDlyCntPer40ms >= 91500 )
					{
						m_st_wFlagLoad.bInvOvloadTimeout = 1;					//逆变过载延时到
						m_i32OvloadDlyCntPer40ms = 91500;
					}
				}
			}
			//(本/并机)系统旁路正常，旁路晶闸管未断路且逆变电压被拉跨
			//旁路上下限内缩
			if ( ( objInvAlg.m_st_wIntMainFlag.bVinvFast == 1 ) && ( objBypass.m_st_FlagVbpTotal.bBpRange == 0 )
				&& ( objParallel.m_st_wParaSignalF.bSystemUnlockF == 0 ) && ( objSwitches.m_st_FlagBreaker.bBpSTSOpen == 0 ) 
				&& ( objBypass.m_i16VbpaPrms <= 1106 ) && ( objBypass.m_i16VbpaPrms >= 942 )
				&& ( objBypass.m_i16VbpbPrms <= 1106 ) && ( objBypass.m_i16VbpbPrms >= 942 )
				&& ( objBypass.m_i16VbpcPrms <= 1106 ) && ( objBypass.m_i16VbpcPrms >= 942 ) )
			{
				m_st_wFlagLoad.bImpactSwitch = 1;								//置本机逆变冲击跳旁路标志
			}
		}
		else																//本机旁路侧供电
		{
			m_st_wFlagLoad.bInvOvloadTimeout = 0;								//清逆变过载延时到
			m_i32OvloadDlyCntPer40ms = 0;												//过载延时计数器清零
			m_st_wFlagLoad.bImpactSwitch = 0;									//清本机冲击跳旁路标志
		}
	}


/************************************************************************************
函数名：	App_OvloadSignal()
功能描述：	过载信号处理模块，用于控制并机总线信号INV_OVERLOAD的发送
函数调用：	无
************************************************************************************/

	void	Class_Load::App_OvloadSignal()

	{
		//zqf 060413--考虑并机下容错，TEST模式下如果误合维修旁路则引导其它机子切旁路
		//当过载超时 或 外部维修开关闭合 或 （内部维修空开合上且外部输出空开也闭合）
		if ( (m_st_wFlagLoad.bInvOvloadTimeout == 1 ))
			//Nxm系统单机下各模块维修空开闭合均报"用户操作错误"不再报"邻机请求转旁路"
//		|| ((objParallel.m_st_wParaSignalF.bCabQ3BpStatusF == 1) 
//			   ||((objDigitalIO.m_st_HardwareSignal.bQ3BP == 1 ) && ( objParallel.m_st_wParaSignalF.bExtOutStatusF == 1)))) 

		{
			objParallel.m_st_wParaSignal.bInvOverload = 1;
			m_i16OvloadSignalCntPerMloop = 400;
		}
		else
		{
			if ( m_st_wFlagLoad.bImpactSwitch == 1 )								//本机冲击跳旁路
			{
				objParallel.m_st_wParaSignal.bInvOverload = 1;
				m_i16OvloadSignalCntPerMloop = 20;
			}
			//旁路在保护范围内（基于并机系统旁路相同的假设）且发生INV_OCP时，向其它机发信过载信号	
//			else if(( objBypass.m_st_FlagVbpTotal.bBpRange <=2 )&&(objDigitalIO.m_st_HardwareSignal.bIGBTOCP ==1)
//				&&(objXfer.m_st_wXferCmd.bIGBTOCPBak ==0) &&(objSystem.m_st_wFlagSystem.bAfterParamDly==1))			
//			{
//				objParallel.m_st_wParaSignal.bInvOverload = 1;
//				m_i16OvloadSignalCntPerMloop = 20;
//			}			

			else
			{
				if ( m_i16OvloadSignalCntPerMloop > 0 )
				{
					m_i16OvloadSignalCntPerMloop --;
					objParallel.m_st_wParaSignal.bInvOverload = 1;
				}
				else
				{
					m_i16OvloadSignalCntPerMloop = 0;
					objParallel.m_st_wParaSignal.bInvOverload = 0;
				}

			}
		}
	}



//===========================================================================
// End of file.
//===========================================================================
