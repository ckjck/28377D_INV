/*==========================================================*/ 	
/* Title		:	InverterControl.cpp											*/ 	
/* Description	: 	Class_InverterControl definition									*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#include "InverterControl.h"


/************************************************************************************
函数名：	App_InvCtrlVarInit()	
功能描述：	InverterControl类变量初始化		
函数调用：	
***********************************************************************************/
inline	 void	Class_InverterControl::App_InvCtrlVarInit(void)
	{
		//INT16ernal Status
		m_i16FlagOffReason = 3;
	}
/************************************************************************************
函数名：	App_InvCtrl()
功能描述：	逆变器行为控制模块，控制逆变器的开关机，软启动及幅值、相位跟踪，过载处理及各并机
					信号线控制
函数调用：
			App_InvOnoffAgent()： 		逆变器开、关机命令（用户命令或内部命令）受理模块
			App_InvOnoffAction()：		逆变器开、关机动作执行模块
			App_InvMaster():				逆变主机竞争模块（需中断程序中的MASTER_ID模块配合）
			App_InvTrace():				逆变器相位、幅值跟踪控制模块，包括并机及LBS工况的跟踪需求
			App_InvSoftstart():			逆变器软启动及幅度调整模块
			App_InvStandby():				逆变器切换整备状态判别模块
************************************************************************************/

	
	void	Class_InverterControl::App_InvCtrl()

	{
		App_InvOnoffAgent();
		
		App_InvOnoffAction();

		App_InvMaster();

		App_InvTrace();

		App_InvSoftstart();

		App_InvStandby();
		
	}


/************************************************************************************
函数名：	App_InvOnoffAgent()
功能描述：	逆变器开、关机命令（用户命令或内部命令）受理模块
函数调用：	App_InvOffReason()				维护关机原因子程序
************************************************************************************/


	void	Class_InverterControl::App_InvOnoffAgent()
	{
		
		//逆变器开机允许标志维护
		if(m_st_wFlagInvOnoff.bFinalOnOff == 0)
		{
			if((objSwitches.m_st_FlagBreaker.bBpSTSShort == 1)||(objSwitches.m_st_FlagBreaker.bInvKMShort == 1)||(objSwitches.m_st_FlagBreaker.bInvKMOpen == 1)
				||(objSwitches.m_st_FlagBreaker.bFuse == 0)
				||(objDigitalIO.m_st_wLocalSignal.bModuleReadyOk ==0)
				||(objDigitalIO.m_st_HardwareSignal.bRecOff == 1))
			{
				objSystem.m_st_wFlagPrompt.bInvOnEnable = 0;				//开机禁止
				S2U(m_st_wFlagInvOnoff) &= 0xFFF8;				//清所有开机命令 BIT2 1 0
			}

			//内部输出开关闭合 且 （内部维修旁路闭合 或 （外部维修开关闭合 且 外部输出开关闭合））
			else if((objDigitalIO.m_st_HardwareSignal.bQ5 == 1) && ((objDigitalIO.m_st_HardwareSignal.bQ3BP == 1) 
			   		||((objParallel.m_st_wParaSignalF.bCabQ3BpStatusF==1) && (objParallel.m_st_wParaSignalF.bExtOutStatusF == 1))))
			{
				objSystem.m_st_wFlagPrompt.bInvOnEnable = 0;				//开机禁止
				S2U(m_st_wFlagInvOnoff) &= 0xFFF8;				//清所有开机命令 BIT2 1 0
			}
			
			else if((S2U(objSystem.m_st_wFlagFault)&0x2DB1) != 0)		//except Bit15 14 12 9 6 3 2 1

			{
				objSystem.m_st_wFlagPrompt.bInvOnEnable = 0;				//开机禁止
				S2U(m_st_wFlagInvOnoff) &= 0xFFF8;				//清所有开机命令 BIT2 1 0
			}
			else if(objSystem.m_st_wFlagPrompt.bPow15vFault == 1)
			{
				objSystem.m_st_wFlagPrompt.bInvOnEnable = 0;				//开机禁止
				S2U(m_st_wFlagInvOnoff) &= 0xFFF8;				//清所有开机命令 BIT2 1 0
			}
			else if(m_i16FlagOffReason >= 5)
			{
				objSystem.m_st_wFlagPrompt.bInvOnEnable = 0;				//开机禁止
				S2U(m_st_wFlagInvOnoff) &= 0xFFF8;				//清所有开机命令 BIT2 1 0
			}
			else if((m_i16FlagOffReason == 4)
				&& (objSystem.m_st_wFlagStatus.bEPO == 1))
			{
				objSystem.m_st_wFlagPrompt.bInvOnEnable = 0;				//开机禁止
				S2U(m_st_wFlagInvOnoff) &= 0xFFF8;				//清所有开机命令 BIT2 1 0
			}
			
			//自老化模式下，旁路超保护或输出空开断开，禁止开机			
			else if((objSystem.m_st_wFlagSystem.bSelfAgingAction == 1) 
//				&& ( m_st_FlagVbpTotal.bBpRange!=0||m_st_FlagBreaker.bQ5!=1 ) )
				&&( (objBypass.m_st_FlagVbpTotal.bBpRange > 2)||(objSwitches.m_st_FlagBreaker.bQ5 != 1) ) )			
			{
				objSystem.m_st_wFlagPrompt.bInvOnEnable = 0;				//开机禁止
				S2U(m_st_wFlagInvOnoff) &= 0xFFF8;				//清所有开机命令 BIT2 1 0
			}
		
			else if((m_i16FlagOffReason == 4) &&(objSystem.m_st_wFlagStatus.bOutDisable == 1))
			{
				objSystem.m_st_wFlagPrompt.bInvOnEnable = 1;				//开机允许
				S2U(m_st_wFlagInvOnoff) &= 0xFFF8;				//清所有开机命令 BIT2 1 0
			}
		
			else
			{
				objSystem.m_st_wFlagPrompt.bInvOnEnable = 1;				//开机允许
			}
        }

		// 加强诊断
		if(m_st_wFlagInvOnoff.bFinalOnOff == 0)					  	 		//关机状态
		{
			
			if((S2U(m_st_wFlagInvOnoff) & 0x7FF0) != 0)					  	//存在任何关机命令？
			{
				S2U(m_st_FlagInvOnoffBak) |= (S2U(m_st_wFlagInvOnoff)&0x7FF0); 	//或入新的关机命令
			}	
		    else
		    {
				S2U(m_st_FlagInvOnoffBak) &= 0xFFF0;					  	//先清所有开机命令和用户关机命令（用于切换）
				S2U(m_st_FlagInvOnoffBak) |= (S2U(m_st_wFlagInvOnoff)&0x0007); 	//记录新的开机命令
		    }	
			
		}
		else														  	//开机状态
		{
			if((S2U(m_st_wFlagInvOnoff)&0x7FF0) != 0)						  	//存在任何关机命令？
			{
				S2U(m_st_FlagInvOnoffBak) &= 0x000F;					  	//先清所有关机命令
				
				S2U(m_st_FlagInvOnoffBak) |= (S2U(m_st_wFlagInvOnoff)&0x7FF0); 	//记录新的关机命令
			}
				
		}


		//逆变器开关机控制
		if((S2U(m_st_wFlagInvOnoff)&0x7FF0) != 0)				//存在任何关机命令
		{
			if(m_st_wFlagInvOnoff.bNeverRecoverOff == 1)		//13
			{
				m_st_wFlagInvOnoff.bFinalOnOff = 0;
				m_i16FlagOffReason = 5;
			}

			else if((m_st_wFlagInvOnoff.bOutDisableOff == 1)
					||(objDigitalIO.m_st_wHardwareNewSignal.bModuleReady  == 0) //模块没有准备好不允许开机
					||(m_st_wFlagInvOnoff.bInvFaultOff == 1)
					||(m_st_wFlagInvOnoff.bEPO == 1))			//9 7 6 5	
			{
				m_st_wFlagInvOnoff.bFinalOnOff = 0;
				if(m_i16FlagOffReason < 4) 
				{ 
					m_i16FlagOffReason = 4;
					
				}	
			}
		
			else if(m_st_wFlagInvOnoff.bInstantOff == 1)		//8 
			{
				m_st_wFlagInvOnoff.bFinalOnOff = 0;
				if(m_i16FlagOffReason < 3) 
				{
					m_i16FlagOffReason = 3;
				}	
			}
			else if(m_st_wFlagInvOnoff.bUserOffAct == 1)		//4
			{
				m_st_wFlagInvOnoff.bFinalOnOff = 0;
				if(m_i16FlagOffReason < 3) 
				{	
					m_i16FlagOffReason = 3;
				}	
			}

			else if((m_st_wFlagInvOnoff.bManualRecoverOff == 1)
					||(m_st_wFlagInvOnoff.bBpFailOff == 1))		//12 10
			{
				m_st_wFlagInvOnoff.bFinalOnOff = 0;
				if(m_i16FlagOffReason < 3)
				{	
					 m_i16FlagOffReason = 3;
				}
			}
			else if(m_st_wFlagInvOnoff.bAutoRecoverOff == 1)	//11
			{
				m_st_wFlagInvOnoff.bFinalOnOff = 0;
				if(m_i16FlagOffReason < 2) 
				{
					m_i16FlagOffReason = 2;
				}	
			}

			//用户关机先切换，后关机  用户关机的第二主循环屏蔽切换自关机，在第三主循环执行用户关机
			else if((m_st_wFlagInvOnoff.bSwitchAutoOff == 1) && ( objSystem.m_st_wFlagStatus.bUserOffBak == 0))	//14
			{
				m_st_wFlagInvOnoff.bFinalOnOff = 0;
				if(m_i16FlagOffReason < 1)
				{
					 m_i16FlagOffReason = 1;
				}
			}
			else 
			{}

		}
		else											//开机命令处理										
		{

			if((m_st_wFlagInvOnoff.bFinalOnOff == 0) && ((S2U(m_st_wFlagInvOnoff)&0x0007) != 0))
			{
				m_st_wFlagInvOnoff.bFinalOnOff = 1;
			}	
		}	

		S2U(m_st_wFlagInvOnoff) &= 0x8008;					//保留总开关机位及用户关机切换位
		objInvCtrl.App_InvOffReason();	
	}


/************************************************************************************
函数名：	App_InvOffReason()
功能描述：	本模块用于维护关机原因，还维护输出切换关机延时自开机命令
函数调用：	无
************************************************************************************/

	void	Class_InverterControl::App_InvOffReason()
	{
		if ( m_st_wFlagInvOnoff.bFinalOnOff == 1 )								//开机
		{
			m_i16FlagOffReason = 0;
			m_i16AutoOnDlyCntPer40ms = 0;
		}
		else																//关机
		{
			if ( m_i16FlagOffReason == 1 )
			{
				if ( objSystem.m_st_wFlagSystem.bTimer40ms == 1 )
				{
					//zqf 060616-加快回切时间：10s-->1s
					if ( m_i16AutoOnDlyCntPer40ms >= 25 )								
					{
						//zqf 061106-本小时过载切换次数到，不自起逆变器，待下小时再自起					
						if(objSystem.m_st_wFlagPrompt.bSwitchTimesClamp == 0)
						{
							if((objXfer.m_st_XferResult.bBpSideDelay != 0)
							&&(objXfer.m_i16BPDelayCnt < 120))
							{
								m_st_wFlagInvOnoff.bSwitchAutoOn = 0;				//置输出切换自开机命令
							}
							else
							{
								m_st_wFlagInvOnoff.bSwitchAutoOn = 1;				//置输出切换自开机命令
							}
						}
						m_i16AutoOnDlyCntPer40ms = 25;
					}
					else
					{
						m_i16AutoOnDlyCntPer40ms++;
					}
				}
			}
			else if( m_i16FlagOffReason == 2 )
			{
				if(objDigitalIO.m_st_wLocalNewSignal.bRECOvTemp == 0)
				{
					m_st_wFlagInvOnoff.bFaultAutoOn = 1;
				}
			}
			else
			{
				if ( m_i16FlagOffReason==4 )
				{
					if ( ( objSystem.m_st_wFlagStatus.bEPO == 0 ) && ( objSystem.m_st_wFlagStatus.bInvFault == 0 )
							&& ( objSystem.m_st_wFlagStatus.bOutDisable == 0 ) )
					{
						m_i16FlagOffReason = 3;
					}
				}
				m_i16AutoOnDlyCntPer40ms = 0;
			}
		}
	}

/************************************************************************************
函数名：	App_InvOnoffAction()
功能描述：	本模块处理逆变器开、关机动作及状态转换，并维护逆变运行状态标志
饔茫?无
************************************************************************************/

	void	Class_InverterControl::App_InvOnoffAction()
	
	{
		if ( m_st_wFlagInvOnoff.bFinalOnOff == 1 )								//开机
		{
			if ( objInverter.m_st_wFlagInv.bInv != 0 )										//开机状态
			{
				if ( ( objInverter.m_st_wFlagInv.bInv == 1 ) && ( objSystem.m_st_wFlagSystem.bTimer40ms == 1 ) )
				{
					m_i16InvSSCntPer40ms++;
					if ( m_i16InvSSCntPer40ms >= 500 )								//zqf 060616: 5s-->50s
					{
						objInverter.m_st_wFlagInv.bInv = 3;									//启动完毕
					}
				}
			}
			else															//关机状态
			{				                                           
				objInverter.m_st_wFlagInv.bInv = 1;											//软启动
				m_i16InvSSCntPer40ms = 0;												//逆变器软启动计时
//				objParallel.m_st_wParaSignal.bInvState=1;									//INV_STAT=1
			}
		}
		else																//关机
		{
			if ( objInverter.m_st_wFlagInv.bInv != 0 )										//开机状态
			{					                                           
				objInverter.m_st_wFlagInv.bInv = 0;											//关闭逆变器
				m_i16InvSSCntPer40ms = 0;												//逆变器软启动计时
				objXfer.m_i16wBptoinvSwCntPer40ms = 0;											//旁路至逆变切换等待延时
//				objParallel.m_st_wParaSignal.bInvState=0;									//INV_STAT=0
			}
		}		

	}
	
/************************************************************************************
函数名：	App_InvMaster()
功能描述：	本模块用于竞争逆变主机，当逆变开机且输出空开合上后，本机才加入主机竞争行列，
			需中断程序中的MASTER_ID模块配合。
函数调用：	无
************************************************************************************/


	void	Class_InverterControl::App_InvMaster()
	
	{
		INT16		i16TempINVOnMod,i16Temp;
// 本机逆变开启状态离散信号的形成
		i16TempINVOnMod = objDigitalIO.m_st_wHardwareNewSignal.bInvID;
		if ( ( objInverter.m_st_wFlagInv.bInv != 0 ) && ( objSwitches.m_st_FlagBreaker.bQ5 == 1 ) )		//逆变器开启且Q5合
		{
			if((objParallel.m_st_wParaSignalF.bInvBpStatusF == 1)&&(objParallel.m_st_wParaSignal.bInvBpStatus == 0))
			{
				S2U(objDigitalIO.m_st_wLocalNewSignal )=S2U( objDigitalIO.m_st_wLocalNewSignal)&0X0F83F;
			}
			else
			{
				switch(objDigitalIO.m_st_wHardwareNewSignal.bInvID)
				{
				case 1:
						objDigitalIO.m_st_wLocalNewSignal.bINVOnMod1 = 1;
						objDigitalIO.m_st_wLocalNewSignal.bINVOnMod2 = 0;
						objDigitalIO.m_st_wLocalNewSignal.bINVOnMod3 = 0;
						objDigitalIO.m_st_wLocalNewSignal.bINVOnMod4 = 0;
						objDigitalIO.m_st_wLocalNewSignal.bINVOnMod5 = 0;
						break;
				case 2:
						objDigitalIO.m_st_wLocalNewSignal.bINVOnMod1 = 0;
						objDigitalIO.m_st_wLocalNewSignal.bINVOnMod2 = 1;
						objDigitalIO.m_st_wLocalNewSignal.bINVOnMod3 = 0;
						objDigitalIO.m_st_wLocalNewSignal.bINVOnMod4 = 0;
						objDigitalIO.m_st_wLocalNewSignal.bINVOnMod5 = 0;
						break;
				case 3:
						objDigitalIO.m_st_wLocalNewSignal.bINVOnMod1 = 0;
						objDigitalIO.m_st_wLocalNewSignal.bINVOnMod2 = 0;
						objDigitalIO.m_st_wLocalNewSignal.bINVOnMod3 = 1;
						objDigitalIO.m_st_wLocalNewSignal.bINVOnMod4 = 0;
						objDigitalIO.m_st_wLocalNewSignal.bINVOnMod5 = 0;
						break;
				case 4:
						objDigitalIO.m_st_wLocalNewSignal.bINVOnMod1 = 0;
						objDigitalIO.m_st_wLocalNewSignal.bINVOnMod2 = 0;
						objDigitalIO.m_st_wLocalNewSignal.bINVOnMod3 = 0;
						objDigitalIO.m_st_wLocalNewSignal.bINVOnMod4 = 1;
						objDigitalIO.m_st_wLocalNewSignal.bINVOnMod5 = 0;
						break;
				case 5:
						objDigitalIO.m_st_wLocalNewSignal.bINVOnMod1 = 0;
						objDigitalIO.m_st_wLocalNewSignal.bINVOnMod2 = 0;
						objDigitalIO.m_st_wLocalNewSignal.bINVOnMod3 = 0;
						objDigitalIO.m_st_wLocalNewSignal.bINVOnMod4 = 0;
						objDigitalIO.m_st_wLocalNewSignal.bINVOnMod5 = 1;
						break;
				default:
        				break;
				}
			}
			
		}
		else
		{				
				S2U(objDigitalIO.m_st_wLocalNewSignal )=S2U( objDigitalIO.m_st_wLocalNewSignal)&0X0F83F;
		}
		
            //主机竞争

			if ( ( objInverter.m_st_wFlagInv.bInv != 0 ) && ( objSwitches.m_st_FlagBreaker.bQ5 == 1 ) )		//逆变器开启且Q5合
			{
				if(objParallel.m_st_wParaSignal.bLocalMaster == 0)						//本机不是主机
				{
					if(objParallel.m_st_wParaSignalF.bMasterOnlineF == 0)                 //系统无主机
					{
						i16Temp = (i16TempINVOnMod<<2)-3;
						
						if(++m_i16MasterOnlineConfCnt >= i16Temp)
						{
							m_i16MasterOnlineConfCnt = i16Temp;
							objParallel.m_st_wParaSignal.bLocalMaster = 1;				 //置本机为主机
						}

					}
					else
					{
						m_i16MasterOnlineConfCnt=0;
				        objParallel.m_st_wParaSignal.bLocalMaster= 0;				 //系统有主机，置本机为从机
					}
				}
			}
			else
			{
				m_i16MasterOnlineConfCnt = 0;
				objParallel.m_st_wParaSignal.bLocalMaster= 0;				 //逆变未开启或Q5断，置本机为从机
			}
		

//决定最终的地位,是系统主机还是系统从机
         if((objParallel.m_st_wParaSignal.bLocalMaster==1)
         &&(objParallel.m_st_wParaSignalF.bLeader_Rack==1))
         	{

			objParallel.m_st_wParaSignal.bMasterOnline = 1;	

		    }
        else
        	{
			objParallel.m_st_wParaSignal.bMasterOnline = 0;				
		    }


		
//工频同步信号的发送
		if(objParallel.m_st_wParaSignal.bMasterOnline == 1) 				//主机发送工频同步信号
		{
			EALLOW;
			EPwm6Regs.TZCLR.bit.OST = 1;
			EDIS;
		}
		else												                //从机禁止发送工频同步信号
		{
			EALLOW;
			EPwm6Regs.TZFRC.bit.OST = 1;
			EDIS;
		}

	}





/************************************************************************************
函数名：	App_InvTrace()
功能描述：	逆变器跟踪控制及锁定判断
函数调用：	App_TraceSignal()： 		并机系统跟踪总线信号SYSTEM_UNLOCK处理
			App_TraceControl()：		逆变器相位、幅度跟踪目标控制
			App_TraceJudge()：		逆变器相位、幅度锁定标志处理
************************************************************************************/

	void Class_InverterControl::App_InvTrace()
	{

		App_TraceSignal();
		
		App_TraceControl();

		App_TraceJudge();
				
	}

/************************************************************************************
函数名：	App_TraceSignal()
功能描述：	并机系统跟踪总线信号SYSTEM_UNLOCK处理
函数调用：	无		
************************************************************************************/
	
	void	Class_InverterControl::App_TraceSignal()
	
	{
		if ( objParallel.m_st_wParaSignal.bMasterOnline == 1 )				//主机
		{
			
			if ( objInverter.m_st_wFlagInv.bPhaseTrace == 3 )				//跟LBS源			
			{
				objParallel.m_st_wParaSignal.bSystemUnlock = 1;				//SYSTEM_UNLOCK=1
			}
			else															//不跟LBS源
			{

				if ( ( objBypass.m_st_FlagVbpTotal.bBpRange == 0 ) && ( objInvAlg.m_st_wIntMainFlag1_0.bBpPhaseLock == 1 ) )	//旁路跟踪范围以内且相位锁定
				{
					objParallel.m_st_wParaSignal.bSystemUnlock = 0;			//SYSTEM_UNLOCK=0

				}
				else														//旁路跟踪范围以外或相位未锁定
				{
					objParallel.m_st_wParaSignal.bSystemUnlock = 1;			//SYSTEM_UNLOCK=1
				}
			}
		}
		else																//从机									
		{
			if ( ( objInverter.m_st_wFlagInv.bInv != 0 ) && ( objSwitches.m_st_FlagBreaker.bQ5 == 1 ) )		//逆变器开启且Q5合
			{
				if ( objBypass.m_st_FlagVbpTotal.bBpRange == 0 )							//旁路跟踪范围以内
				{
					objParallel.m_st_wParaSignal.bSystemUnlock = 0;							//SYSTEM_UNLOCK=0
				}
				else														//旁路跟踪范围以外
				{
					objParallel.m_st_wParaSignal.bSystemUnlock = 1;							//SYSTEM_UNLOCK=1
				}
			}
			else															//逆变器未开启或Q5断	
			{
				objParallel.m_st_wParaSignal.bSystemUnlock = 0;								//SYSTEM_UNLOCK=0
			}
		
		}
	




}

/************************************************************************************
函数名：	App_TraceControl()
功能描述：	逆变器相位、幅度跟踪目标控制
函数调用：	l_div_i()					长除法
************************************************************************************/

	void Class_InverterControl::App_TraceControl()
	
	{		
		if ( ( objInverter.m_st_wFlagInv.bInv == 3 ) && ( objSwitches.m_st_FlagBreaker.bQ5 == 1 ) )
		{
			if(objParallel.m_st_wParaSignal.bMasterOnline == 1 )					//本机是主机
			{
				if ( objParallel.m_st_wParaSignal.bInvBpStatus == 1 )						//本机工作在逆变侧
				{
					if ((((objLBS.m_st_LBSStatus.bLBSDetect == 2) && (objMonInterface.m_st_wUPSModeRun.bLBS != 0))
						||((objLBS.m_st_LBSStatus.bLBSDetect == 1) && (objMonInterface.m_st_wUPSModeRun.bLBS == 1)))
						&&(objInvAlg.m_i16DthetaLBSBase_0 > objInvAlg.m_i16FreqSetMin_0)
						&&(objInvAlg.m_i16DthetaLBSBase_0 < objInvAlg.m_i16FreqSetMax_0))
					{
						objInverter.m_st_wFlagInv.bPhaseTrace = 3;                //一级锁相跟踪LBS
						objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 1;						//二级锁相跟踪工频同步信号
						
						objInverter.m_st_wFlagInv.bAmpTrace = 0;							//幅值额定
						objInverter.m_i16wVrefTgtMd = objInverter.m_i16VrefSetMd;				
					}

					else if ( objParallel.m_st_wParaSignalF.bSystemUnlockF == 0)		//旁路可跟踪 本机柜内任何一个功率模块旁路异常，幅值都不跟旁路
					{
						objInverter.m_st_wFlagInv.bPhaseTrace = 1;                       //一级锁相跟踪旁路
//						objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 0;					//二级锁相也跟踪旁路
						objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 1;					//二级锁相跟踪工频同步信号
						if((objMonInterface.m_st_wUPSModeRun.bConfig == 0)&&(objMonInterface.m_st_wUPSModeRun.bMode == 1)&&(objXfer.m_st_XferResult.bEcoInvMdUpFlag==1))
						{
							objInverter.m_st_wFlagInv.bAmpTrace = 1;					//幅值跟旁路
							objInverter.m_i16wVrefTgtMd =  objBypass.m_unVbpMMean_0.half.hword+52;	
						}
						else
						{
							objInverter.m_st_wFlagInv.bAmpTrace = 0;					//幅值额定
							objInverter.m_i16wVrefTgtMd = objInverter.m_i16VrefSetMd;
						}
					}
					else if(objBypass.m_st_FlagVbpTotal.bBpTrace == 1)					//旁路跟踪范围以外
					{
						objInverter.m_st_wFlagInv.bPhaseTrace = 1;						//一级锁相跟踪旁路
						objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 1;					//二级锁相跟踪工频同步信号
						objInverter.m_st_wFlagInv.bAmpTrace = 0;							//幅值额定
						objInverter.m_i16wVrefTgtMd = objInverter.m_i16VrefSetMd;
					}
					else
					{
						objInverter.m_st_wFlagInv.bPhaseTrace = 0;						//一级锁相本振
						objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 1;					//二级锁相跟踪工频同步信号
						objInverter.m_st_wFlagInv.bAmpTrace = 0;							//幅值额定
						objInverter.m_i16wVrefTgtMd = objInverter.m_i16VrefSetMd;
					}
				}
				else if ( objXfer.m_st_wXferCmd.bSwitchStatus == 0 )						//INV_STS=0,本机旁路侧
				{
					if ( objParallel.m_st_wParaSignalF.bSystemUnlockF == 0)		//旁路可跟踪 若本机柜内任何一个功率模块旁路异常，幅值都不跟旁路
					{
						objInverter.m_st_wFlagInv.bPhaseTrace = 1;                       //一级锁相跟踪旁路
//						objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 0;					//二级锁相也跟踪旁路
						objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 1;					//二级锁相跟踪工频同步信号
						
						objInverter.m_st_wFlagInv.bAmpTrace = 1;							//幅值跟旁路
						objInverter.m_i16wVrefTgtMd =( objBypass.m_unVbpMMean_0.half.hword+52);
					}
					else if(objBypass.m_st_FlagVbpTotal.bBpTrace == 1)					//本模块旁路可跟踪，机柜内有模块旁路不可跟踪
					{
						objInverter.m_st_wFlagInv.bPhaseTrace = 1;						//一级锁相跟踪旁路
						objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 1;					//二级锁相跟踪工频同步信号
						objInverter.m_st_wFlagInv.bAmpTrace = 0;							//幅值额定
						objInverter.m_i16wVrefTgtMd = objInverter.m_i16VrefSetMd;
					}
					else
					{
						objInverter.m_st_wFlagInv.bPhaseTrace = 0;						//一级锁相本振
						objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 1;					//二级锁相跟踪工频同步信号
						objInverter.m_st_wFlagInv.bAmpTrace = 0;							//幅值额定
						objInverter.m_i16wVrefTgtMd = objInverter.m_i16VrefSetMd;
					}
				}

			}
			else															//本机是从机
			{
				if ( objParallel.m_st_wParaSignalF.bInvBpStatusF == 1 )						//系统工作在逆变侧
				{
					if ( objParallel.m_st_wParaSignalF.bSystemUnlockF == 0)					//旁路可跟踪 本机柜内任何一个功率模块旁路异常，幅值都不跟旁路
					{
						objInverter.m_st_wFlagInv.bPhaseTrace = 1;							//一级锁相跟旁路
//						objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 0;						//二级锁相跟旁路
						objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 1;					//二级锁相跟踪工频同步信号
						
						objInverter.m_st_wFlagInv.bAmpTrace = 0;								//幅值额定
						objInverter.m_i16wVrefTgtMd = objInverter.m_i16VrefSetMd;
					}
					else													//SYSTEM_UNLOCK=1或旁路跟踪范围以外
					{
						objInverter.m_st_wFlagInv.bPhaseTrace = 2;							//一级锁相跟踪工频同步信号
						objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 1;						//二级锁相跟踪工频同步信号
						objInverter.m_st_wFlagInv.bAmpTrace = 0;								//幅值额定
						objInverter.m_i16wVrefTgtMd = objInverter.m_i16VrefSetMd;
					}

				}
				else														//系统工作在旁路侧
				{
					if ( objXfer.m_st_wXferCmd.bSwitchStatus == 0 )					//本机旁路侧
					{
						if  ( objParallel.m_st_wParaSignalF.bSystemUnlockF == 0)            //旁路可跟踪 本机柜内任何一个功率模块旁路异常，幅值都不跟旁路

						{
							objInverter.m_st_wFlagInv.bPhaseTrace = 1;						//一级锁相跟旁路
//							objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 0;					//二级锁相跟旁路
							objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 1;					//二级锁相跟踪工频同步信号
							
							objInverter.m_st_wFlagInv.bAmpTrace = 1;							//幅值旁路
							objInverter.m_i16wVrefTgtMd = (objBypass.m_unVbpMMean_0.half.hword+52);
						}
						else												//SYSTEM_UNLOCK_F=1
						{
							objInverter.m_st_wFlagInv.bPhaseTrace = 2;						//一级锁相跟踪工频同步信号
							objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 1;					//二级锁相跟踪工频同步信号
							objInverter.m_st_wFlagInv.bAmpTrace = 0;							//幅值额定
							objInverter.m_i16wVrefTgtMd = objInverter.m_i16VrefSetMd;
						}
					}
				}
			}
		}
		else if(objParallel.m_st_wParaSignal.bMasterOnline == 1 )							//本机是主机
		{
			objInverter.m_st_wFlagInv.bPhaseTrace = 0;										//一级锁相本振
			objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 1;									//二级锁相跟踪工频同步信号
			objInverter.m_st_wFlagInv.bAmpTrace = 0;											//本振,幅值额定
			objInverter.m_i16wVrefTgtMd = objInverter.m_i16VrefSetMd;
		}
		else
		{
			objInverter.m_st_wFlagInv.bPhaseTrace = 2;										//一级锁相跟踪工频同步信号
			objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 1;									//二级锁相跟踪工频同步信号
			objInverter.m_st_wFlagInv.bAmpTrace = 0;											//幅值额定
			objInverter.m_i16wVrefTgtMd = objInverter.m_i16VrefSetMd;
		}
	}	
								
/************************************************************************************
函数名：	App_TraceJudge()
功能描述：	逆变器相位、幅度锁定标志处理
函数调用：	无
************************************************************************************/
	
	void	 Class_InverterControl::App_TraceJudge()
	
	{
		if ( objInverter.m_st_wFlagInv.bInv == 0 )											//逆变器未开启
		{
			if ( objXfer.m_st_wXferCmd.bSwitchStatus == 1 )							//系统处于逆变侧
			{
				objInverter.m_st_wFlagInv.bAmpLock = 0;										//清辐度锁定标志
			}
			else															//系统处于旁路侧
			{
				objInverter.m_st_wFlagInv.bPhaseLock = 0;
				objInverter.m_st_wFlagInv.bAmpLock = 0;										//清相位，辐度锁定标志
			}
			
			m_i16AmpLockTgtCnt = 0;
			m_i16AmpLockBpCnt = 0;
		}
		else																//逆变器己开启
		{
 			//逆变器软启动中或启动完毕且本振,或逆变供电且旁路超保护
			if ( ( objInverter.m_st_wFlagInv.bInv == 1 ))
//					||( (objXfer.m_st_wXferCmd.bSwitchStatus == 1) &&( objBypass.m_st_FlagVbpTotal.bBpRange == 3)  ) )	
			{
				objInverter.m_st_wFlagInv.bPhaseLock = 0;
				objInverter.m_st_wFlagInv.bAmpLock = 0;										//清相位，辐度锁定标志
			
				m_i16AmpLockTgtCnt = 0;
				m_i16AmpLockBpCnt = 0;
			}
			else															//逆变器启动完毕且不本振
			{
				objInverter.m_st_wFlagInv.bPhaseLock = objInvAlg.m_st_wIntMainFlag.bPhaseLock;  				//从INT16MainFlag中拷贝相位锁定标志
				
				//对于自老化模式，逆变器软启动完毕即认为幅度锁定
				if(objSystem.m_st_wFlagSystem.bSelfAgingAction == 1)
				{
					objInverter.m_st_wFlagInv.bAmpLock = 1;
				}
				else														//逆变幅值非额定值
				{
					if ( objInverter.m_st_wFlagInv.bAmpTrace == 1 )			//跟旁路
					{
						m_i16AmpLockTgtCnt = 0;
						
						if ( ( ( objBypass.m_i16VbpaPrms - objInverter.m_i16VinvaPrms > 47 ) || ( objInverter.m_i16VinvaPrms - objBypass.m_i16VbpaPrms > 47) ) 
							|| ( (objBypass.m_i16VbpbPrms - objInverter.m_i16VinvbPrms > 47 ) || ( objInverter.m_i16VinvbPrms - objBypass.m_i16VbpbPrms > 47) ) 
							|| ( (objBypass.m_i16VbpcPrms - objInverter.m_i16VinvcPrms > 47 ) || ( objInverter.m_i16VinvcPrms - objBypass.m_i16VbpcPrms > 47) ) 
							|| ( ( objInverter.m_i16wVrefTgtMd - (objInverter.m_unVinvMMean_0.half.hword) ) > 132 )
							|| ( ( (objInverter.m_unVinvMMean_0.half.hword) - objInverter.m_i16wVrefTgtMd ) > 132 ))		//// 1.414*5V
						{
							objInverter.m_st_wFlagInv.bAmpLock = 0;			//清辐度锁定标志
							m_i16AmpLockBpCnt = 0;
						}
						else
						{
							m_i16AmpLockBpCnt++;
							if(m_i16AmpLockBpCnt >= 1000)
							{
								m_i16AmpLockBpCnt = 1000;
								objInverter.m_st_wFlagInv.bAmpLock = 1;		//置辐度锁定标志
							}
											
						}
					}
					else													//本振											
					{
						m_i16AmpLockBpCnt = 0;
						
						if ( ( ( objInverter.m_i16wVrefTgtMd - (objInverter.m_unVinvMMean_0.half.hword) ) > 132 )
							|| ( ( (objInverter.m_unVinvMMean_0.half.hword) - objInverter.m_i16wVrefTgtMd ) > 132 ) )	// 1.414*5V
						{
							m_i16AmpLockTgtCnt++;
							if(m_i16AmpLockTgtCnt >= 1000)
							{
								m_i16AmpLockTgtCnt = 1000;
								objInverter.m_st_wFlagInv.bAmpLock = 0;				//清辐度锁定标志
							}				
						}
						else
						{
							objInverter.m_st_wFlagInv.bAmpLock = 1;
							m_i16AmpLockTgtCnt = 0;
						}
					}
				}
			}
		}
		
		S2U(objParallel.m_st_wParaSignalFBak) = S2U(objParallel.m_st_wParaSignalF);

		if(objParallel.m_st_wParaSignal.bInvBpStatus == 0)
		{
			objXfer.m_st_XferResult.bEcoInvMdUpFlag = 0;
			objXfer.m_st_XferResult.bEcoInvMdUpOk = 0;
			
		}
		else
		{
			if((objXfer.m_st_XferResult.bEcoInvMdUpFlag == 1)&&(objInverter.m_st_wFlagInv.bAmpLock == 1))
			{
				objXfer.m_st_XferResult.bEcoInvMdUpOk = 1;
			}
			else
			{
				objXfer.m_st_XferResult.bEcoInvMdUpOk = 0;
			}
		}

	}


/************************************************************************************
函数名：	App_InvSoftstart()
功能描述：	本模块处理软启动过程中逆变电压的给定值
函数调用：	无
************************************************************************************/

	void	Class_InverterControl::App_InvSoftstart()
	
	{
		if ( objInverter.m_st_wFlagInv.bInv == 0 )										//逆变器关毕
		{
			objInvAlg.m_i16wVrefRunMd_0 = 0;
		}
		else															//逆变器启动
		{
			if ( objInvAlg.m_i16wVrefRunMd_0< objInverter.m_i16wVrefTgtMd )
			{		
				objInvAlg.m_i16wVrefRunMd_0 += 2;											//原3
				if ( objInvAlg.m_i16wVrefRunMd_0 >= objInverter.m_i16wVrefTgtMd )
				{
					objInvAlg.m_i16wVrefRunMd_0 = objInverter.m_i16wVrefTgtMd;
				}
			}
			else if ( objInvAlg.m_i16wVrefRunMd_0 > objInverter.m_i16wVrefTgtMd )
			{
				objInvAlg.m_i16wVrefRunMd_0 -= 1;										//原3
				if ( objInvAlg.m_i16wVrefRunMd_0 <= objInverter.m_i16wVrefTgtMd )
				{
					objInvAlg.m_i16wVrefRunMd_0 = objInverter.m_i16wVrefTgtMd;
				}
			}
		}
	}

/************************************************************************************
函数名：	App_InvStandby()
功能描述：	本模块处理逆变同步跟踪信号INV_ON_STANDBY
函数调用：	无
************************************************************************************/

	void	Class_InverterControl::App_InvStandby()
	
	{
		if ( ( objInverter.m_st_wFlagInv.bInv != 0 )  && ( objSwitches.m_st_FlagBreaker.bQ5 == 1 ) )			//逆变器开启且Q5合
		{
			
			if ( objXfer.m_st_wXferCmd.bSwitchStatus == 1 )							//逆变侧供电
			{
				objParallel.m_st_wParaSignal.bInvOnStandby = 0;								//INV_ON_STANDBY=0
			}
			else															//bSwitchStatus=0,旁路侧供电
			{
				if ( ( objXfer.m_st_wXferCmd.bSwitchDesire == 1 ) &&( ( objInverter.m_st_wFlagInv.bPhaseLock == 1 ) 
					||((objInverter.m_st_wFlagInv.bPhaseTrace == 0) && (objParallel.m_st_wParaSignal.bMasterOnline==1)))
					&& ( objInverter.m_st_wFlagInv.bAmpLock == 1 ) )
				{
					objParallel.m_st_wParaSignal.bInvOnStandby = 0;							//INV_ON_STANDBY=0
				
				}
				else
				{
					objParallel.m_st_wParaSignal.bInvOnStandby = 1;							//INV_ON_STANDBY=1

				}
			}
		}
		else
		{
			objParallel.m_st_wParaSignal.bInvOnStandby = 0;									//INV_ON_STANDBY=0
		}
	}
	


//===========================================================================
// End of file.
//===========================================================================
