/*==========================================================*/ 	
/* Title		:	FaultRecord.cpp												*/ 	
/* Description	: 	Class_FaultRecord definition										*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#include "FaultRecord.h"



/************************************************************************************
函数名：	App_FaultRecordVarInit()	
功能描述：	FaultRecord类变量初始化		
函数调用：	
***********************************************************************************/
inline	 void	Class_FaultRecord::App_FaultRecordVarInit(void)
	{
		//zqf060327 克服上电即发现故障时，上传故障记录前三点为无效数据的情况。要求故障记录上传前必须记满4点。
		m_i16TotalRecordGrp = 0;  
	}
/************************************************************************************
函数名：	App_FaultRecord()
功能描述：	逆变侧故障记录模块
函数调用：	App_FaultRecordTaxsis()故障记录排序函数			
************************************************************************************/
//注意：本模块的调用位置很关键，需仔细考虑！！！
//		增加一个故障记录缓冲器，在故障上传期间，
//		将一直刷新缓冲器，如果发生一次新故障，则刷新
//		一次后锁存，不再刷新，直到下一次故障上传才解锁

 	void	Class_FaultRecord::App_FaultRecord(void)
	{
		UINT16 	*puint,u16Temp;
	
	
		if(m_i16FaultAriseinTx == 0)						//1:故障上传期间已经发生一次故障，不再记录数据 
		{	
		
			if(  ( (~S2U(objSystem.m_st_wFlagFaultBak)) & (S2U(objSystem.m_st_wFlagFault)) & FLAG_FAULT_MASK ) 
		     	||( (~S2U(objSystem.m_st_wFlagPromptBak)) & (S2U(objSystem.m_st_wFlagPrompt)) & FLAG_PROMPT_MASK) 
		     	||( (~S2U(objSwitches.m_st_wFlagBreakerBak)) & (S2U(objSwitches.m_st_FlagBreaker)) & FLAG_BREAKER_MASK)
		     	||( (~S2U(objSystem.m_st_wFlagStatusBak)) & (S2U(objSystem.m_st_wFlagStatus)) & FLAG_STATUS_MASK) )
			{
				objSystem.m_st_wFlagSystem.bNewFaultArise	= 1;
			}
			else
			{
				objSystem.m_st_wFlagSystem.bNewFaultArise	= 0;
			}
	
				//故障标志备份量每主循环刷新		
				S2U(objSystem.m_st_wFlagFaultBak) = S2U(objSystem.m_st_wFlagFault);								
				S2U(objSystem.m_st_wFlagPromptBak) = S2U(objSystem.m_st_wFlagPrompt);	
				S2U(objSwitches.m_st_wFlagBreakerBak) =S2U(objSwitches.m_st_FlagBreaker);
				S2U(objSystem.m_st_wFlagStatusBak) = S2U(objSystem.m_st_wFlagStatus);
	
	
	 		if(objSystem.m_st_wFlagSystem.bRecordSaveRequest == 1)			// 1:故障上传期间	
			{ 
				if(objSystem.m_st_wFlagSystem.bNewFaultArise == 1)
				{
					m_i16FaultAriseinTx = 1;					// 1:故障上传期间有新的故障产生
				}
	 		}
		
			if((objSystem.m_st_wFlagSystem.bNewFaultArise == 1) ||( objSystem.m_st_wFlagSystem.bTimer14ms == 1) )
			{
				//记录此时刻故障数据
				puint = uFaultRecordBuff+ (m_i16CurrRecordGrpBuff<<5) - m_i16CurrRecordGrpBuff;	//*31
	
				u16Temp = (objInverter.m_st_wFlagInv.bInv)
					|(objInverter.m_st_wFlagInv.bPhaseTrace<<4)
					|(objInverter.m_st_wFlagInv.bAmpTrace<<6)
					|(objInverter.m_st_wFlagInv.bPhaseLock<<8)
					|(objInverter.m_st_wFlagInv.bAmpLock<<9)
					|((objInvCtrl.m_i16FlagOffReason& 0x000F)<<10)
					|(objDigitalIO.m_st_HardwareSignal.bRecOff)<<14	// 共模电感饱和硬件信号（0有效）
					|(objDigitalIO.m_st_wLocalSignal.bModuleReadyOk<<15);	
				//just 32 words!!!		
				*puint++ = S2U(objMonInterface.m_st_wUPSModeRun);					// 7
				*puint++ = S2U(objSystem.m_st_wFlagStatus);
				*puint++ = S2U(objSystem.m_st_wFlagFault);
				*puint++ = S2U(objSystem.m_st_wFlagPrompt);
				*puint++ = S2U(objSwitches.m_st_FlagBreaker);	
				*puint++ = m_u16wClkCntPer14ms;						// 增加14ms到累加器记数
				*puint++ = u16Temp;
			
				*puint++ = objBypass.m_i16VbpaPrms;							// 4
				*puint++ = objBypass.m_i16VbpbPrms;
				*puint++ = objBypass.m_i16VbpcPrms;
				*puint++ = objBypass.m_i16FbpMean;
			
				*puint++ = objInverter.m_i16VinvaPrms;						// 7
				*puint++ = objInverter.m_i16VinvbPrms;
				*puint++ = objInverter.m_i16VinvcPrms;
				*puint++ = objInverter.m_i16IinvaPrms;
				*puint++ = objInverter.m_i16IinvbPrms;
				*puint++ = objInverter.m_i16IinvcPrms;
				*puint++ = objInverter.m_i16FinvMean;
			
				*puint++ = objOutput.m_i16VoutaPrms;						// 8
				*puint++ = objOutput.m_i16VoutbPrms;
				*puint++ = objOutput.m_i16VoutcPrms;
				*puint++ = objInvAlg.m_i16IinvMdCir_0;
				*puint++ = objInvAlg.m_i16IinvMqCir_0;
				*puint++ = objInvAlg.m_i16IinvMzCir_0;
				*puint++ = objDCBus.m_i16Vdc_0;							// 母线电压和
//				*puint++ = 0;			// 旁路假中点到母线中点电压							
			
				*puint++ = objOutput.m_i16Pouta;							// 6
				*puint++ = objOutput.m_i16Poutb;
				*puint++ = objOutput.m_i16Poutc;
				*puint++ = objInverter.m_i16IindaPrms;						
				*puint++ = objInverter.m_i16IindbPrms;
				*puint++ = objInverter.m_i16IindcPrms;	
		
				m_i16CurrRecordGrpBuff++;
			if(m_i16CurrRecordGrpBuff > 3) 
			{
				m_i16CurrRecordGrpBuff = 0;
			}
		
				//克服上电即发现故障时，上传故障记录前三点为无效数据的情况。要求故障记录上传前必须记满4点。
	             if(m_i16TotalRecordGrp < 10) 
	            	{
				m_i16TotalRecordGrp++;	//逐次加1，限幅为10。
	            	}
				
				//计到3点时，清备份，以发现此前出现的故障，下一主循环会触发故障记录上传          
	             if(m_i16TotalRecordGrp == 3)			
			{	
				S2U(objSystem.m_st_wFlagFaultBak) = 0;								
				S2U(objSystem.m_st_wFlagPromptBak) = 0;	
				S2U(objSwitches.m_st_wFlagBreakerBak) = 0;
				S2U(objSystem.m_st_wFlagStatusBak) = 0;
			}         
			}	
		}
	
		//克服上电即发现故障时，上传故障记录前三点为无效数据的情况。要求故障记录上传前必须记满4点	
		//符合故障上传条件时把缓冲器的内容搬到故障记录器中
			if((objSystem.m_st_wFlagSystem.bRecordSaveRequest == 0)&&(objSystem.m_st_wFlagSystem.bNewFaultArise == 1)
				&& (m_i16TotalRecordGrp >= 4))
			{
				App_FaultRecordTaxsis();
											
				objSystem.m_st_wFlagSystem.bRecordSaveRequest = 1;			//请求数据上传
				objEEPROM.m_i16FaultWriteCnt = 128;
				m_i16CurrRecordGrp = m_i16CurrRecordGrpBuff;
				m_i16FaultAriseinTx = 0;
			}
	}
	
/************************************************************************************
函数名：	App_FaultRecordTaxsis()
功能描述：	故障记录数据进行预处理:排序
函数调用：	无			
************************************************************************************/
void	Class_FaultRecord::App_FaultRecordTaxsis()
{
	UINT16 u16Temp0;
	UINT16 *puint;
	INT16 i16Temp;
	
	//指向故障数据1起始地址
	if ( m_i16CurrRecordGrpBuff == 0 )
	{
		u16Temp0= 3;
	}
	else
	{
		u16Temp0 = m_i16CurrRecordGrpBuff - 1;
	}

	if(u16Temp0 == 0)							
	{
		puint = uFaultRecordBuff;
	}
	else
	{
		puint = uFaultRecordBuff + (u16Temp0<<5) - u16Temp0;			//从0位到8位
	}	
	for(i16Temp=0; i16Temp<31; i16Temp++)
	{
		uFaultRecord[i16Temp] = *puint ;							//上传故障前先把故障记录缓冲器的内容搬到故障记录器
		puint++;
	}
	
	//指向故障数据2起始地址
	if ( m_i16CurrRecordGrpBuff <= 1 )
	{
		u16Temp0 = m_i16CurrRecordGrpBuff + 2;
	}
	else
	{
		u16Temp0 = m_i16CurrRecordGrpBuff - 2;
	}

	if(u16Temp0 == 0)							
	{
		puint = uFaultRecordBuff;
	}
	else
	{
		puint = uFaultRecordBuff + (u16Temp0<<5) - u16Temp0;			
	}	

	for(i16Temp=31; i16Temp<62; i16Temp++)
	{
		uFaultRecord[i16Temp] = *puint ;							//从31到61位
		puint++;
	}
	//指向故障数据3起始地址
	if ( m_i16CurrRecordGrpBuff <= 2 )
	{
		u16Temp0 = m_i16CurrRecordGrpBuff + 1;
	}
	else
	{
		u16Temp0 = m_i16CurrRecordGrpBuff - 3;
	}

	if(u16Temp0 == 0)							
	{
		puint = uFaultRecordBuff;
	}
	else
	{
		puint = uFaultRecordBuff + (u16Temp0<<5) - u16Temp0;			
	}	

	for(i16Temp = 62; i16Temp<93; i16Temp++)
	{
		uFaultRecord[i16Temp] = *puint ;							//从62到92位
		puint++;
	}
	//指向故障数据4起始地址
	if(m_i16CurrRecordGrpBuff == 0)							
	{
		puint = uFaultRecordBuff;
	}
	else
	{
		puint = uFaultRecordBuff + (m_i16CurrRecordGrpBuff<<5) - m_i16CurrRecordGrpBuff;			
	}	

	for(i16Temp = 93; i16Temp<124; i16Temp++)
	{
		uFaultRecord[i16Temp] = *puint ;							//从93到123位
		puint++;
	}
	uFaultRecord[124] = objMonInterface.m_u16TimeLabel3_0;
	uFaultRecord[125] = objMonInterface.m_u16TimeLabel2_0;
	uFaultRecord[126] = objMonInterface.m_u16TimeLabel1_0;
	uFaultRecord[127] = 0;
}

//===========================================================================
// End of file.
//===========================================================================
