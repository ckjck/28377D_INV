
/*==========================================================*/ 	
/* Title		:	Transfer.cpp													*/ 	
/* Description	: 	Class_Transfer definition										*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#include "Transfer.h"


/************************************************************************************
函数名：	App_XferOut()
功能描述：	输出切换模块，扩展UPS在各种组态和工况下的输出切换逻辑及动作
函数调用：	App_XferCommand()
			App_XferProcess()
			App_XferExecute()
************************************************************************************/

	void	Class_Transfer::App_XferOut()

	{
		
		App_XferCommand();
		
		App_XferProcess();
		
		App_XferExecute();

		
		INT16	i16MainAX;
		//诊断代码，记录最近10次跳旁路原因
		if ((m_st_wXferCmd.bSwitchStatus == 0) && (m_st_XferCmdBack.bSwitchStatus == 1))
		{
			for(i16MainAX = 8; i16MainAX>=0; i16MainAX--)
			{
				m_i16XferBpIDArray[i16MainAX+1] = m_i16XferBpIDArray[i16MainAX];
			}
		
			if (m_i16XferBpID == 3)
			{
				m_i16XferBpIDArray[0] = S2U(objInvCtrl.m_st_FlagInvOnoffBak);
			}
			else
			{
				m_i16XferBpIDArray[0] = m_i16XferBpID;
			}
		}
	
		S2U(m_st_XferCmdBack) = S2U(m_st_wXferCmd);	
		

	}

/************************************************************************************
函数名：	App_XferCommand()
功能描述：	控制各种组态及工况下的输出切换意愿，并选择需明确决定的切换模式
函数调用：	无	
************************************************************************************/
const  INT16	MAINTAIN_INV_SIDE =0;					
const  INT16	DESIRE_BP_OVERLOAD = 1;
const  INT16	DESIRE_BP_IMPACT = 2;
const  INT16	DESIRE_BP_NINTSW = 3;
const  INT16	DESIRE_BP_NORMAL = 4;
const  INT16	DESIRE_BP_ABNORMAL = 5;
                                     
const  INT16	MAINTAIN_BP_SIDE = 0;
const  INT16	MAINTAIN_BP_PROMPT = 1;					
const  INT16	DESIRE_INV_40S = 2;
const  INT16	DESIRE_INV_SLAVE = 3;
const  INT16	DESIRE_INV_INTSW = 4;

	void	Class_Transfer::App_XferCommand()
	{

		INT16		i16DesireInvSide;					//逆变侧预期切换分支
		INT16		i16DesireBpSide;					//旁路侧预期切换分支

		objSystem.m_st_wFlagPrompt.bTotalCapPrompt0 = 0;

		//逆变侧	
		if(m_st_wXferCmd.bSwitchStatus == 1)				
		{
			
			objLoad.m_st_wFlagLoad.bReqSwitch = 0;
			m_i16SystemOvloadCntPerMloop = 0;

			
			if(objSwitches.m_st_FlagBreaker.bQ5 != 1)           
			{	
				m_i16XferBpID = 1;
				i16DesireInvSide = DESIRE_BP_NORMAL;
			}	

			// BYPASS禁止开机
			else if(objSwitches.m_st_FlagBreaker.bQ3bp != 0)           
			{	
				m_i16XferBpID = 2;
				i16DesireInvSide = DESIRE_BP_NORMAL;
			}	
			
			else if (objInvCtrl.m_st_wFlagInvOnoff.bUserOff == 1)
			{	
				m_i16XferBpID=3;
				i16DesireInvSide = DESIRE_BP_NORMAL;
            		}
			else if (objSystem.m_st_wFlagPrompt.bDCAbnormalOff == 1)
			{	
				m_i16XferBpID = 3;
				i16DesireInvSide = DESIRE_BP_NORMAL;
           		 }
			
			else if(objInverter.m_st_wFlagInv.bInv != 3)           
			{	
				m_i16XferBpID = 3;
				i16DesireInvSide = DESIRE_BP_NORMAL;
			}	
			else if(objLoad.m_st_wFlagLoad.bInvOvloadTimeout == 1)           
			{	
				m_i16XferBpID = 4;
				i16DesireInvSide = DESIRE_BP_OVERLOAD;
			}				
			else if(objLoad.m_st_wFlagLoad.bImpactSwitch == 1)           
			{	
				m_i16XferBpID = 5;
				i16DesireInvSide = DESIRE_BP_IMPACT;
			}	
			else if((objMonInterface.m_st_wUPSModeRun.bConfig == 3) && (objParallel.m_st_wParaSignalF.bInvOverloadF == 1))           
			{	
				m_i16XferBpID = 6;
				objLoad.m_st_wFlagLoad.bReqSwitch = 1;
				i16DesireInvSide = DESIRE_BP_IMPACT;
			}	
			//By wxw20090813 增加单机逆变侧时，ECO的条件
			//加入bSystemUnlockF防止由于bSystemUnlockF复位的滞后性，导致切旁路时逆变没有跟踪旁路的幅值
			else if((((objMonInterface.m_st_wUPSModeRun.bConfig == 0) && (objMonInterface.m_st_wUPSModeRun.bMode== 1))
					||((objMonInterface.m_st_wUPSModeRun.bConfig == 2) && (objMonInterface.m_st_wUPSModeRun.bMasterECO == 1)))
					&& (m_st_XferResult.bInvSideDelay == 0)&& (objBypass.m_st_FlagVbpTotal.bBpRange == 0)
					&& (objSwitches.m_st_FlagBreaker.bBpSTSOpen != 1)&&(objParallel.m_st_wParaSignalF.bSystemUnlockF == 0))
			{
				m_i16XferBpID = 7;
				i16DesireInvSide = DESIRE_BP_NINTSW;
			}
			//zqf 060718--增加输出电压异常切换
			else if(objSystem.m_st_wFlagFault.bVoutAbnormal == 1)           
			{	
				m_i16XferBpID = 8;
				i16DesireInvSide = DESIRE_BP_ABNORMAL;
			}			
			else
			{
				m_i16XferBpID = 0;
				i16DesireInvSide = MAINTAIN_INV_SIDE;
			}

	
			switch(i16DesireInvSide)
			{
				case	MAINTAIN_INV_SIDE:
							m_st_wXferCmd.bSwitchDesire = 1;
							m_st_wXferCmd.bSwitchMode = 0;
							break;
							
				case	DESIRE_BP_OVERLOAD:
							m_st_wXferCmd.bSwitchDesire = 0;
							m_st_wXferCmd.bSwitchMode = 0;
							m_st_wXferCmd.bOvloadSwitch = 1;
							m_i16wBptoinvTimes++;
							m_st_XferResult.bBpSideDelay = 2;			
							break;
							
				case	DESIRE_BP_IMPACT:
							m_st_wXferCmd.bSwitchDesire = 0;
							m_st_wXferCmd.bSwitchMode = 0;
							m_st_wXferCmd.bOvloadSwitch = 1;
							break;
							
				case	DESIRE_BP_NINTSW:
							m_st_wXferCmd.bSwitchDesire = 0;
							m_st_wXferCmd.bSwitchMode = 1;
							break;
							
				case	DESIRE_BP_NORMAL:	
							m_st_wXferCmd.bSwitchDesire = 0;
							m_st_wXferCmd.bSwitchMode = 0;
							break;
				
				//zqf 060718--输出电压异常切换,马上自动回切，有次数限制
				case	DESIRE_BP_ABNORMAL:
							m_st_wXferCmd.bSwitchDesire = 0;
							m_st_wXferCmd.bSwitchMode = 0;
							m_st_wXferCmd.bOvloadSwitch = 1;
							m_i16wBptoinvTimes++;		
							break;
										
				default:	
							break;	
			}

		}		

		//旁路侧
		else
		{
			if((objLoad.m_st_wFlagLoad.bReqSwitch == 1) && (objParallel.m_st_wParaSignalF.bInvOverloadF == 1))
			{
				m_i16SystemOvloadCntPerMloop++;
				if(m_i16SystemOvloadCntPerMloop >= 500)
				{
					 m_i16SystemOvloadCntPerMloop = 500; 
				}
				if(m_i16SystemOvloadCntPerMloop == 100)
				{
					m_i16wBptoinvTimes++;
					m_st_XferResult.bBpSideDelay = 2;
				}
			}

			//BYP禁止开机 
			if((objSwitches.m_st_FlagBreaker.bQ5 == 0)||(objSwitches.m_st_FlagBreaker.bQ3bp == 1)||(objInverter.m_st_wFlagInv.bInv != 3))
			{
				i16DesireBpSide = MAINTAIN_BP_SIDE; 
			}
			
			//用户关机先切换，后关机   禁止逆变切旁路的第二周期回切
			else if(objSystem.m_st_wFlagStatus.bUserOffBak == 1 )
			{	
				i16DesireBpSide = MAINTAIN_BP_SIDE;
			}				

			//zqf060310--为防止并机旁路不均流导致某些机器旁路过载而整个系统无发回切逆变
			//ITA40K旁路过流运行切变，防止并机旁路不均流尤其是三单模式
			else if(((objLoad.m_st_wFlagLoad.bOvload == 1) && (objMonInterface.m_st_wUPSModeRun.bConfig != 3))
					||((objMonInterface.m_st_wUPSModeRun.bConfig == 3)&&(objParallel.m_st_wParaSignalF.bInvOverloadF == 1))
					||(objSystem.m_st_wFlagPrompt.bSwitchTimesClamp == 1)|| ( m_st_XferResult.bBpSideDelay != 0 )
					)
			{	
				i16DesireBpSide = MAINTAIN_BP_SIDE;	
			}
			 
			//zqf 060413--进入隔离态非TEST模式时的输出禁止，且停留在旁路侧
			else if(objParallel.m_st_wParaMaint.bOutIsoDisable == 1)
			{	
				i16DesireBpSide=MAINTAIN_BP_SIDE;	
			}

			else if(objMonInterface.m_st_wUPSModeRun.bConfig == 3)				//并机
			{
				if(objParallel.m_st_wParaSignalF.bInvBpStatusF == 1)
				{
					i16DesireBpSide = DESIRE_INV_SLAVE;	
				}	
				
				else if(m_st_wXferCmd.bOvloadSwitch == 1)
				{
					//zqf060310--不同容量并联
//					if(iPSoutTotalMax<=iNumInv*973)
					if(((((INT32)objSystem.m_i16RackSoutTotalMax*KBpKVA*20)>>10) <=( ((INT32)objMonInterface.m_i16ModelKVA*1024*objMonInterface.m_st_wParallelData.iTotalNumInv*19)/objInverter.m_i16TempKVAGrant))				//实际带载小于等于0.95倍供电容量
						&&((((INT32)objSystem.m_i16RackPoutTotalMax*KBpKVA*20)>>10) <=( ((INT32)objMonInterface.m_i16ModelKVA*1024*objMonInterface.m_st_wParallelData.iTotalNumInv*19)/objInverter.m_i16TempKWGrant)))			//实际带载小于等于0.95倍供电容量
                 	{
						i16DesireBpSide = DESIRE_INV_40S;  
					}
					else   
					{
						i16DesireBpSide = MAINTAIN_BP_PROMPT;  
					}

				}
				else
				{
					//zqf060310--不同容量并联

//					if((objCAN.iPSoutTotalMax*3)<=(objCAN.iCapacitySum_InvOn*10))			//实际带载小于等于供电容量
					if(((((INT32)objSystem.m_i16RackSoutTotalMax*KBpKVA)>>10)<=(((INT32)objMonInterface.m_i16ModelKVA*1024*objMonInterface.m_st_wParallelData.iTotalNumInv)/objInverter.m_i16TempKVAGrant))
						&&((((INT32)objSystem.m_i16RackPoutTotalMax*KBpKVA)>>10)<=(((INT32)objMonInterface.m_i16ModelKVA*1024*objMonInterface.m_st_wParallelData.iTotalNumInv)/objInverter.m_i16TempKWGrant)) )
					{	
						i16DesireBpSide = DESIRE_INV_40S;  
					}
					else  
					{
						i16DesireBpSide = MAINTAIN_BP_PROMPT; 
					}
				}
						
			}
			else										//单机
			{
			
				if ( ((objMonInterface.m_st_wUPSModeRun.bConfig == 0) && (objMonInterface.m_st_wUPSModeRun.bMode == 1))
					||((objMonInterface.m_st_wUPSModeRun.bConfig == 2) && (objMonInterface.m_st_wUPSModeRun.bMasterECO == 1)))
				{
					if((objBypass.m_st_FlagVbpTotal.bBpRange != 0) || ((objInvAlg.m_st_wIntMainFlag.bVbpFast == 1) )
						||(objSwitches.m_st_FlagBreaker.bBpSTSOpen == 1))	
					{	
						i16DesireBpSide = DESIRE_INV_INTSW;  
					}
					else 
					{
						i16DesireBpSide = MAINTAIN_BP_SIDE;	
					}
				}
				else  
				{
					i16DesireBpSide = DESIRE_INV_40S;	 
				}
							
			}

			switch(i16DesireBpSide)
			{
			
				case	MAINTAIN_BP_SIDE:
							m_st_wXferCmd.bSwitchDesire =0;
							m_st_wXferCmd.bSwitchMode = 0;
							break;
				case	MAINTAIN_BP_PROMPT:
							m_st_wXferCmd.bSwitchDesire = 0;
							m_st_wXferCmd.bSwitchMode = 0;
							objSystem.m_st_wFlagPrompt.bTotalCapPrompt0 = 1;
							break;
				case	DESIRE_INV_40S:
							m_st_wXferCmd.bSwitchDesire = 1;
							m_st_wXferCmd.bSwitchMode = 0;
							if((objParallel.m_st_wParaSignal.bMasterOnline == 1) && (objSystem.m_st_wFlagSystem.bTimer40ms == 1))
							{
								if(m_i16wBptoinvSwCntPer40ms < 1000)	
								{
									m_i16wBptoinvSwCntPer40ms++; 
								}
							}							
							break;
				case	DESIRE_INV_SLAVE:
							m_st_wXferCmd.bSwitchDesire = 1;
							m_st_wXferCmd.bSwitchMode = 0;
							break;
				case	DESIRE_INV_INTSW:	
							m_st_wXferCmd.bSwitchDesire = 1;
							m_st_wXferCmd.bSwitchMode = 2;
							m_st_XferResult.bInvSideDelay = 2;							
							break;
				default:
							break;	
			}

		}

	}
	
/************************************************************************************
函数名：	App_XferProcess()
功能描述：	负责控制切换模式，选择切换时刻，维护切换过程
函数调用：	无	
************************************************************************************/

const	INT16		SW_BREAK_BPSIDE	= 0;
const	INT16		SW_BREAK_INVSIDE = 1;
const	INT16		SW_MAINTAIN_BP = 2;
const	INT16		SW_TOINV_INTSW	= 3;
const	INT16		SW_TOINV_NINTSW	 = 4;
const	INT16		SW_MAINTAIN_INV	 = 5;
const	INT16		SW_TOBP_INTSW = 6;
const	INT16		SW_TOBP_NINTSW = 7;

		
	void	Class_Transfer::App_XferProcess()
	{

		INT16		i16SwitchProc;
		INT16		i16IntSwitchAct;
		INT16		i16MainAX,i16MainBX;
		i16SwitchProc =	SW_BREAK_BPSIDE;	
		i16IntSwitchAct = 0;
	
		objSystem.m_st_wFlagPrompt.bUnitCapPrompt = 0;
		objSystem.m_st_wFlagStatus.bIntSwitch = 0;

		if(objXfer.m_st_wXferCmd.bSwitchInterval == 1)
		{
			i16MainAX=(INT16)(objTimerDrv.m_u16ClkUs_0 - objXfer.m_u16ClkUsIntSw);
			
			if(i16MainAX<0)	
			{
				i16MainAX = i16MainAX+CLKUSPRDCNST;
			}

			//间断时间须根据机型（50/60）、用户设置的间断时间及切换方向确定				
			if(objXfer.m_st_wXferCmd.bSwitchStatus == 1)						//Bp to inv
			{
				switch(objMonInterface.m_i16IntSwTimeSet)
				{
					case 0:
						
							if(objMonInterface.m_st_wUPSModeRun.bFreqOut == 0)  
							{	
		//						i16MainBX = K10msCnst;
								i16MainBX = K17msCnst;						//16ms@50Hz 
							}
							else  
							{ 
								i16MainBX = K14msCnst;						//14ms@60Hz	
							}
						
						break;
					case 1:
						i16MainBX = K40msCnst;						//40ms
						break;      
					case 2:         
						i16MainBX = K60msCnst;						//60ms
						break;      
					case 3:         
						i16MainBX = K80msCnst;						//80ms
						break;      
					default:        
						i16MainBX = K100msCnst;						//100ms
						break;
						
				}																		
			}	
			else												//Inv to bp
			{
				switch(objMonInterface.m_i16IntSwTimeSet)
				{
					case 0:
					//用户关机先切换，后关机	逆变切旁路最小间断时间3ms			
	//					i16MainBX = K3msCnst;						//3ms
						i16MainBX = K8msCnst;
						break;
					case 1:
						i16MainBX = K40msCnst;						//40ms
						break;      
					case 2:         
						i16MainBX = K60msCnst;						//60ms
						break;      
					case 3:         
						i16MainBX = K80msCnst;						//80ms
						break;      
					default:        
						i16MainBX = K100msCnst;						//100ms
						break;
						
				}
			}	

			if(i16MainAX >= i16MainBX)	
			{
				m_st_wXferCmd.bSwitchInterval = 0;
				
				if(m_st_wXferCmd.bSwitchStatus == 1)
				{
					objParallel.m_st_wParaSignal.bInvBpStatus = 1;
				}	
				else 
				{
					objParallel.m_st_wParaSignal.bInvBpStatus = 0;
				}	
			}
			
		}
		else
		{
			if(objXfer.m_st_wXferCmd.bSwitchStatus == 0)				//旁路侧
			{
				if(objXfer.m_st_wXferCmd.bSwitchDesire == 0)
				{
					i16SwitchProc = SW_MAINTAIN_BP;
				}	
				else if((objXfer.m_st_wXferCmd.bSwitchMode == 2)
						&& (((objInvAlg.m_st_wIntMainFlag.bVbpFast == 1) &&(objOutput.m_st_OutFlag.bVoutFast == 1))||(objSwitches.m_st_FlagBreaker.bBpSTSOpen == 1))
						&& (objInverter.m_st_wFlagInv.bPhaseLock == 1))
				{
					i16SwitchProc = SW_TOINV_NINTSW;
				}	
				else if(objXfer.m_st_wXferCmd.bSwitchMode == 2)
				{
					i16SwitchProc = SW_TOINV_INTSW;
				}
//B				
				else if((objBypass.m_st_FlagVbpTotal.bBpRange == 0)
						||((objMonInterface.m_st_wUPSModeRun.bConfig == 3) && (objParallel.m_st_wParaSignal.bMasterOnline == 0))
						||((objMonInterface.m_st_wUPSModeRun.bConfig == 3) && (objParallel.m_st_wParaSignalF.bInvBpStatusF == 1)))								
				{
					if((objBypass.m_st_FlagVbpTotal.bBpRange !=0) && (objParallel.m_st_wParaSignalF.bInvBpStatusF == 1)
						&&(objInverter.m_st_wFlagInv.bPhaseLock == 1)&& (objInverter.m_st_wFlagInv.bInv == 3))
						
					{																				
						i16SwitchProc = SW_TOINV_INTSW;						
																		
					}
					else if((objInverter.m_st_wFlagInv.bPhaseLock == 1) && (objInverter.m_st_wFlagInv.bAmpLock == 1) 
						&& ((objMonInterface.m_st_wUPSModeRun.bConfig != 3)||(objParallel.m_st_wParaSignal.bMasterOnline == 1)
						||(objParallel.m_st_wParaSignalF.bInvBpStatusF == 1)))
					{
						if(objParallel.m_st_wParaSignal.bMasterOnline == 1)
							
						{
							if((objParallel.m_st_wParaSignalF.bInvOnStandbyF == 0)||(m_i16wBptoinvSwCntPer40ms >= 1000))
							{
								
								if((objParallel.m_st_wParaSignalF.bInvOnStandbyF == 0)&&(objParallel.m_st_wParaSignalF.bSystemUnlockF == 0))
								{								
									i16SwitchProc = SW_TOINV_NINTSW;									
								}
								else if((objParallel.m_st_wParaSignalF.bSystemUnlockF == 1)||(m_i16wBptoinvSwCntPer40ms >= 1000))	//旁路正常但主机跟踪不上时从机间断切换
								{
								
									i16IntSwitchAct = 1;					//间断切换时主机给出间断切换提示
								}
							}
						}
						else
						{
							if((objParallel.m_st_wParaSignalF.bSystemUnlockF == 1)||(objParallel.m_st_wParaSignalF.bInvOnStandbyF != 0))	//旁路正常但主机跟踪不上时从机间断切换
							{
							
								i16SwitchProc = SW_TOINV_INTSW;
							}
							else
							{
								
								i16SwitchProc = SW_TOINV_NINTSW;
							}
						}
					}
						
					else if((objParallel.m_st_wParaSignal.bMasterOnline == 1) && (m_i16wBptoinvSwCntPer40ms >= 1000))
					{	
						i16IntSwitchAct = 1;
					}	
					else
					{
						i16SwitchProc = SW_BREAK_BPSIDE;
					}
				}					
	
				else
				{
					i16IntSwitchAct = 1;
				}

//C	
				if(i16IntSwitchAct == 1)
				{
									  
					if((objMonInterface.m_st_wUPSModeRun.bConfig != 3)
					   ||((objXfer.m_st_wXferCmd.bOvloadSwitch == 0) 
						&&(((((INT32)objSystem.m_i16RackSoutTotalMax*KBpKVA)>>10)<=(((INT32)objMonInterface.m_i16ModelKVA*1024*objMonInterface.m_st_wParallelData.iTotalNumInv)/objInverter.m_i16TempKVAGrant))
						&&((((INT32)objSystem.m_i16RackPoutTotalMax*KBpKVA)>>10)<=(((INT32)objMonInterface.m_i16ModelKVA*1024*objMonInterface.m_st_wParallelData.iTotalNumInv)/objInverter.m_i16TempKWGrant))))				//没有过载切换且实际带载小于等于本机容量
							||((objXfer.m_st_wXferCmd.bOvloadSwitch == 1) 
								&&(((((INT32)objSystem.m_i16RackSoutTotalMax*KBpKVA*20)>>10) <=( ((INT32)objMonInterface.m_i16ModelKVA*1024*objMonInterface.m_st_wParallelData.iTotalNumInv*19)/objInverter.m_i16TempKVAGrant))
									&&((((INT32)objSystem.m_i16RackPoutTotalMax*KBpKVA*20)>>10) <=( ((INT32)objMonInterface.m_i16ModelKVA*1024*objMonInterface.m_st_wParallelData.iTotalNumInv*19)/objInverter.m_i16TempKWGrant)))))		//过载切换且实际带载小于等于0.95倍本机容量
//					if((objMonInterface.m_st_wUPSModeRun.bConfig != 3)||(objXfer.m_st_wXferCmd.bOvloadSwitch == 0))
					{
						if((objXfer.m_st_XferResult.bBpSTS == 1)
							||((objMonInterface.m_st_wUPSModeRun.bConfig == 3) && (objOutput.m_unVoutMMean_0.half.hword >= 2800)))
						{
							//间断时间超过40ms时需用户确认		
							if((objXfer.m_st_wXferCmd.bOvloadSwitch == 0)||(objMonInterface.m_i16IntSwTimeSet >=1 ))
							{
								if((objParallel.m_st_wParaSignal.bMasterOnline == 1)
									&&((objParallel.m_st_wParaSignalF.bInvOnStandbyF == 0)||(m_i16wBptoinvSwCntPer40ms >= 1000)))
								{

									objSystem.m_st_wFlagStatus.bIntSwitch = 1;

								}
								
								if((objMonInterface.m_st_wMonitorOrder.bIntSwitchConf == 1)&&( (objParallel.m_st_wParaSignalF.bInvOnStandbyF == 0)
									||(m_i16wBptoinvSwCntPer40ms >= 1000)))
								{
									i16SwitchProc = SW_TOINV_INTSW;
								}	
								else
								{
									i16SwitchProc = SW_BREAK_BPSIDE;
								}
							}
							else
							{
								if((objParallel.m_st_wParaSignal.bMasterOnline == 1)&&
									((objParallel.m_st_wParaSignalF.bInvOnStandbyF == 0)||(m_i16wBptoinvSwCntPer40ms >= 1000)))
								{
									i16SwitchProc=SW_TOINV_INTSW;
								}
							}
						}
						else if((objMonInterface.m_st_wUPSModeRun.bConfig != 3) 
							|| ((objParallel.m_st_wParaSignal.bMasterOnline == 1)
							&&((objParallel.m_st_wParaSignalF.bInvOnStandbyF == 0)||(m_i16wBptoinvSwCntPer40ms >= 1000))))
						{	
							i16SwitchProc = SW_TOINV_INTSW;
						}
			
					}
					else
					{
						objSystem.m_st_wFlagPrompt.bUnitCapPrompt = 1;
						i16SwitchProc = SW_BREAK_BPSIDE;
					}
				}		
	
			}
			else 										//逆变侧
			{
				if(m_st_wXferCmd.bSwitchDesire == 1)
				{
					i16SwitchProc = SW_MAINTAIN_INV;
					m_st_XferResult.bEcoInvMdUpFlag = 0;
				}
				else if(m_st_wXferCmd.bSwitchMode == 1)
				{
					if(objInverter.m_st_wFlagInv.bPhaseLock == 1)
					{
						m_st_XferResult.bEcoInvMdUpFlag = 1;
						if(m_st_XferResult.bEcoInvMdUpOk == 1)
						{
							i16SwitchProc = SW_TOBP_NINTSW;
						}
						else
						{
							i16SwitchProc = SW_BREAK_INVSIDE;
						}
					}	
					else
					{
						m_st_XferResult.bEcoInvMdUpFlag = 0;
						i16SwitchProc = SW_BREAK_INVSIDE;	
					}	
				}			
				else if((objBypass.m_st_FlagVbpTotal.bBpRange == 0)
					&& (((objInverter.m_st_wFlagInv.bPhaseLock == 1)&&(objParallel.m_st_wParaSignalF.bSystemUnlockF==0))
					||(objLoad.m_st_wFlagLoad.bImpactSwitch == 1)))
				{	
					i16SwitchProc = SW_TOBP_NINTSW;
				}
				else
				{
					i16SwitchProc = SW_TOBP_INTSW;
				}
			}

			switch(i16SwitchProc)
			{
		
				case	SW_BREAK_BPSIDE:
				case	SW_BREAK_INVSIDE:
							break;
				case	SW_MAINTAIN_BP:
							objParallel.m_st_wParaSignal.bInvBpStatus = 0;
							break;                                      
				case	SW_TOINV_INTSW: 
							if(m_st_wXferCmd.bWaitingZeroCross == 0)
							{
								m_st_wXferCmd.bWaitingZeroCross = 1;
							}
							if(((objInvAlg.m_st_wIntMainFlag2_0.bBptoInvZero  == 1)||(objInvAlg.m_st_wIntMainFlag2_0.bWaitingZeroCrossDelayok== 1))
								||((objParallel.m_st_wParaSignal.bMasterOnline == 0) && (objParallel.m_st_wParaSignalF.bInvBpStatusF == 1)))
							{
								objParallel.m_st_wParaSignal.bInvBpStatus = 1;
								m_st_wXferCmd.bSwitchInterval = 1;	
								m_st_wXferCmd.bSwitchStatus = 1;
								m_u16ClkUsIntSw = objTimerDrv.m_u16ClkUs_0;
								m_st_wXferCmd.bWaitingZeroCross = 0;
							}
							break;
				case	SW_TOINV_NINTSW:
							objParallel.m_st_wParaSignal.bInvBpStatus=1;
							m_st_wXferCmd.bSwitchInterval=0;	
							m_st_wXferCmd.bSwitchStatus=1;	
							break;

				case	SW_MAINTAIN_INV:
							objParallel.m_st_wParaSignal.bInvBpStatus = 1;
							break;

				case	SW_TOBP_INTSW:
							objParallel.m_st_wParaSignal.bInvBpStatus = 1;
							m_st_wXferCmd.bSwitchInterval = 1;	
							m_st_wXferCmd.bSwitchStatus = 0;
							m_u16ClkUsIntSw = objTimerDrv.m_u16ClkUs_0;
							objInvCtrl.m_st_wFlagInvOnoff.bSwitchAutoOff = 1;							
							break;
				case	SW_TOBP_NINTSW:
							objParallel.m_st_wParaSignal.bInvBpStatus = 0;
							m_st_wXferCmd.bSwitchInterval = 0;	
							m_st_wXferCmd.bSwitchStatus = 0;
							if((objMonInterface.m_st_wUPSModeRun.bMode == 1)&&(m_st_XferResult.bEcoInvMdUpFlag == 1))
							{
								objInvCtrl.m_st_wFlagInvOnoff.bSwitchAutoOff =0;	
							}
							else
							{
								objInvCtrl.m_st_wFlagInvOnoff.bSwitchAutoOff =1;
							}
							break;

				default:
							break;
			}

			if(i16SwitchProc != SW_TOINV_INTSW)
			{
				m_st_wXferCmd.bWaitingZeroCross = 0;
			}
		}
		if(objXfer.m_st_wXferCmd.bSwitchStatus == 1)
		{
			objMonInterface.m_st_wMonitorOrder.bIntSwitchConf = 0;
		}
	}
	
/************************************************************************************
函数名：	App_XferExecute()
功能描述：	执行切换动作，直接控制旁路/逆变STS驱动信号
函数调用：	无	
************************************************************************************/

	void	Class_Transfer::App_XferExecute()

	{
		INT16	i16MainAX;
		//zqf 061127--注意代码位置，该备份标志在vVbp_Flag()和vSwitch_Execute()模块中用到
		m_st_XferResult.bInvKMBak = m_st_XferResult.bInvKM;		//备份
		
		if((objInverter.m_st_wFlagInv.bInv == 0) && (objInvCtrl.m_i16FlagOffReason >= 3))
		{	
			m_i16wBptoinvTimes = 0;
			m_st_wXferCmd.bOvloadSwitch = 0;
			m_st_XferResult.bBpSideDelay = 0;
			m_st_XferResult.bInvSideDelay = 0;
		}
	
		if(objParallel.m_st_wParaSignal.bInvBpStatus == 0)							//旁路侧
		{


			//在系统在旁路侧,置清零
			//定义回定变量的地址为：0x9D00-0x9D03
			DSPCrazyA = 0x00; 
			DSPCrazyB = 0x00;
			DSPCrazyC = 0x00;
			DSPCrazyD = 0x00;

			if(m_i16KMOffDelay >= K10msCnst)
			{
				objParallel.m_st_wParaSignal.bInvState=0;									//INV_STAT=1
				m_st_XferResult.bInvKM = 0;
			}	
			m_st_XferResult.bInvSideDelay = 0;
			m_i16BpSTSOffDelay = 0;
			m_i16BpSTSOffCnt=0;
			m_i16KMOnCntPer14ms = 0;

			if(m_st_XferResult.bBpSideDelay != 0)		
			{
				if(m_st_XferResult.bBpSideDelay == 3)
				{
					m_i16BPDelayCnt = (INT16)(objTimerDrv.m_u16ClkS_0-m_u16ClkSBpside);
					if(m_i16BPDelayCnt < 0) 
					{
						m_i16BPDelayCnt += 3600;
					}
					if(m_i16BPDelayCnt >= 300)
					{		
						m_st_XferResult.bBpSideDelay = 0;	
					}
				}
				else
				{	
					m_u16ClkSBpside = objTimerDrv.m_u16ClkS_0;
					m_st_XferResult.bBpSideDelay = 3;	
				}
			
			}
			
			//解除POW_OK封旁路条件	
			if (objBypass.m_st_FlagVbpTotal.bBpRange > 1)
			{	
				m_st_XferResult.bBpSTS = 0;
			}
			else if((objBypass.m_st_FlagVbpTotal.bBpInvtSeq == 1)||(objSystem.m_st_wFlagStatus.bEPO == 1)
				||(objSystem.m_st_wFlagStatus.bOutDisable == 1)||(objParallel.m_st_wParaMaint.bOutIsoDisable == 1))	
			{	
				m_st_XferResult.bBpSTS = 0;
			}
			else if((objMonInterface.m_st_wUPSModeRun.bConfig == 3) && (objParallel.m_st_wParaSignalF.bInvBpStatusF == 1))
			{	
				m_st_XferResult.bBpSTS = 0;
			}
			//By wxw20090531 Adapter20K增加了旁路过流延时到屏蔽输出的功能
			else if(objBypass.m_st_FlagIbpTotal.bIbpOverTimeOut == 1)
			{
				m_st_XferResult.bBpSTS = 0;
			}
			/*else if(objOutput.m_st_OutFlag.bShortCircuit == 1)
			{
				m_st_XferResult.bBpSTS = 0;
			}*/
			else
			{
				m_st_XferResult.bBpSTS = 1;
			}
		}
		else													//逆变侧
		{

		//在系统在逆变侧时,置标志
		//定义回定变量的地址为：0x9EFC-0x9EFF
			DSPCrazyA = 0xEB95; 
			DSPCrazyB = 0xEB95;
			DSPCrazyC = 0xEB95;
			DSPCrazyD = 0xEB95;
	    	

			//自老化模式下，直接进入旁路是否可放开的判断
			if(objSystem.m_st_wFlagSystem.bSelfAgingAction == 1)  
			{
				if((objSystem.m_st_wFlagStatus.bEPO == 1)
					||(objSystem.m_st_wFlagStatus.bOutDisable == 1))
				{	
					m_st_XferResult.bBpSTS = 0;
				}
				else if((objBypass.m_st_FlagVbpTotal.bBpInvtSeq == 1)||(objBypass.m_st_FlagVbpTotal.bBpRange > 2))
				{	
					m_st_XferResult.bBpSTS = 0;
				}
				//排除并机及ECO模式
//				else if ((objMonInterface.m_st_wUPSModeRun.bConfig == 3)||(objMonInterface.m_st_wUPSModeRun.bMode == 1))
				else if (objMonInterface.m_st_wUPSModeRun.bMode == 1)
				{
					m_st_XferResult.bBpSTS = 0;	
				}				
				else
				{
					m_st_XferResult.bBpSTS = 1;
				}
			}						
			else
			{
				m_st_XferResult.bBpSTS = 0;							//封锁BP_STS
			}		

			if(m_st_wXferCmd.bSwitchStatus == 1)
			{
				m_i16wBptoinvSwCntPer40ms = 0;
				m_st_XferResult.bBpSideDelay = 0;
				m_st_wXferCmd.bOvloadSwitch = 0;
			}

			if(m_st_XferResult.bInvSideDelay != 0)		
			{
				if(m_st_XferResult.bInvSideDelay == 3)
				{
					i16MainAX = (INT16)(objTimerDrv.m_u16ClkS_0-m_u16ClkSInvside);
					if(i16MainAX < 0) 
					{
						i16MainAX += 3600;
					}
					if(i16MainAX >= 300)
					{	
						m_st_XferResult.bInvSideDelay = 0;
					}	
				}
				else
				{	
					m_u16ClkSInvside = objTimerDrv.m_u16ClkS_0;
					m_st_XferResult.bInvSideDelay = 3;	
				}
			
			}

			/*if((objSwitches.m_st_FlagBreaker.bQ5 == 0) || (objSwitches.m_st_FlagBreaker.bQ3bp == 1)
				|| (objInverter.m_st_wFlagInv.bInv != 3) || (objParallel.m_st_wParaMaint.bOutIsoDisable == 1))*/
			if((objSwitches.m_st_FlagBreaker.bQ5 == 0) || (objSwitches.m_st_FlagBreaker.bQ3bp == 1)
				  || (objParallel.m_st_wParaMaint.bOutIsoDisable == 1))
			{	
				m_st_XferResult.bInvKM = 0;
			}
			else if((m_st_wXferCmd.bSwitchInterval == 1)  && (m_st_wXferCmd.bSwitchStatus == 1))
			{
				m_st_XferResult.bInvKM = 0;
			}
			else
			{ 
				objParallel.m_st_wParaSignal.bInvState=1;									//INV_STAT=1
				m_st_XferResult.bInvKM = 1;
			}

			if((objSystem.m_st_wFlagSystem.bTimer14ms == 1) && (m_st_XferResult.bInvKM == 1))
			{
				m_i16KMOnCntPer14ms ++;
				if(m_i16KMOnCntPer14ms >= 10)
				{
					m_i16KMOnCntPer14ms  = 10;
				}	
			}
	
		}				
	}





//===========================================================================
// End of file.
//===========================================================================
