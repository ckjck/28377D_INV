/*==========================================================*/ 	
/* Title		:	FaultRecord.cpp												*/ 	
/* Description	: 	Class_FaultRecord definition										*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#include "FaultRecord.h"



/************************************************************************************
��������	App_FaultRecordVarInit()	
����������	FaultRecord�������ʼ��		
�������ã�	
***********************************************************************************/
inline	 void	Class_FaultRecord::App_FaultRecordVarInit(void)
	{
		//zqf060327 �˷��ϵ缴���ֹ���ʱ���ϴ����ϼ�¼ǰ����Ϊ��Ч���ݵ������Ҫ����ϼ�¼�ϴ�ǰ�������4�㡣
		m_i16TotalRecordGrp = 0;  
	}
/************************************************************************************
��������	App_FaultRecord()
����������	������ϼ�¼ģ��
�������ã�	App_FaultRecordTaxsis()���ϼ�¼������			
************************************************************************************/
//ע�⣺��ģ��ĵ���λ�úܹؼ�������ϸ���ǣ�����
//		����һ�����ϼ�¼���������ڹ����ϴ��ڼ䣬
//		��һֱˢ�»��������������һ���¹��ϣ���ˢ��
//		һ�κ����棬����ˢ�£�ֱ����һ�ι����ϴ��Ž���

 	void	Class_FaultRecord::App_FaultRecord(void)
	{
		UINT16 	*puint,u16Temp;
	
	
		if(m_i16FaultAriseinTx == 0)						//1:�����ϴ��ڼ��Ѿ�����һ�ι��ϣ����ټ�¼���� 
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
	
				//���ϱ�־������ÿ��ѭ��ˢ��		
				S2U(objSystem.m_st_wFlagFaultBak) = S2U(objSystem.m_st_wFlagFault);								
				S2U(objSystem.m_st_wFlagPromptBak) = S2U(objSystem.m_st_wFlagPrompt);	
				S2U(objSwitches.m_st_wFlagBreakerBak) =S2U(objSwitches.m_st_FlagBreaker);
				S2U(objSystem.m_st_wFlagStatusBak) = S2U(objSystem.m_st_wFlagStatus);
	
	
	 		if(objSystem.m_st_wFlagSystem.bRecordSaveRequest == 1)			// 1:�����ϴ��ڼ�	
			{ 
				if(objSystem.m_st_wFlagSystem.bNewFaultArise == 1)
				{
					m_i16FaultAriseinTx = 1;					// 1:�����ϴ��ڼ����µĹ��ϲ���
				}
	 		}
		
			if((objSystem.m_st_wFlagSystem.bNewFaultArise == 1) ||( objSystem.m_st_wFlagSystem.bTimer14ms == 1) )
			{
				//��¼��ʱ�̹�������
				puint = uFaultRecordBuff+ (m_i16CurrRecordGrpBuff<<5) - m_i16CurrRecordGrpBuff;	//*31
	
				u16Temp = (objInverter.m_st_wFlagInv.bInv)
					|(objInverter.m_st_wFlagInv.bPhaseTrace<<4)
					|(objInverter.m_st_wFlagInv.bAmpTrace<<6)
					|(objInverter.m_st_wFlagInv.bPhaseLock<<8)
					|(objInverter.m_st_wFlagInv.bAmpLock<<9)
					|((objInvCtrl.m_i16FlagOffReason& 0x000F)<<10)
					|(objDigitalIO.m_st_HardwareSignal.bRecOff)<<14	// ��ģ��б���Ӳ���źţ�0��Ч��
					|(objDigitalIO.m_st_wLocalSignal.bModuleReadyOk<<15);	
				//just 32 words!!!		
				*puint++ = S2U(objMonInterface.m_st_wUPSModeRun);					// 7
				*puint++ = S2U(objSystem.m_st_wFlagStatus);
				*puint++ = S2U(objSystem.m_st_wFlagFault);
				*puint++ = S2U(objSystem.m_st_wFlagPrompt);
				*puint++ = S2U(objSwitches.m_st_FlagBreaker);	
				*puint++ = m_u16wClkCntPer14ms;						// ����14ms���ۼ�������
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
				*puint++ = objDCBus.m_i16Vdc_0;							// ĸ�ߵ�ѹ��
//				*puint++ = 0;			// ��·���е㵽ĸ���е��ѹ							
			
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
		
				//�˷��ϵ缴���ֹ���ʱ���ϴ����ϼ�¼ǰ����Ϊ��Ч���ݵ������Ҫ����ϼ�¼�ϴ�ǰ�������4�㡣
	             if(m_i16TotalRecordGrp < 10) 
	            	{
				m_i16TotalRecordGrp++;	//��μ�1���޷�Ϊ10��
	            	}
				
				//�Ƶ�3��ʱ���屸�ݣ��Է��ִ�ǰ���ֵĹ��ϣ���һ��ѭ���ᴥ�����ϼ�¼�ϴ�          
	             if(m_i16TotalRecordGrp == 3)			
			{	
				S2U(objSystem.m_st_wFlagFaultBak) = 0;								
				S2U(objSystem.m_st_wFlagPromptBak) = 0;	
				S2U(objSwitches.m_st_wFlagBreakerBak) = 0;
				S2U(objSystem.m_st_wFlagStatusBak) = 0;
			}         
			}	
		}
	
		//�˷��ϵ缴���ֹ���ʱ���ϴ����ϼ�¼ǰ����Ϊ��Ч���ݵ������Ҫ����ϼ�¼�ϴ�ǰ�������4��	
		//���Ϲ����ϴ�����ʱ�ѻ����������ݰᵽ���ϼ�¼����
			if((objSystem.m_st_wFlagSystem.bRecordSaveRequest == 0)&&(objSystem.m_st_wFlagSystem.bNewFaultArise == 1)
				&& (m_i16TotalRecordGrp >= 4))
			{
				App_FaultRecordTaxsis();
											
				objSystem.m_st_wFlagSystem.bRecordSaveRequest = 1;			//���������ϴ�
				objEEPROM.m_i16FaultWriteCnt = 128;
				m_i16CurrRecordGrp = m_i16CurrRecordGrpBuff;
				m_i16FaultAriseinTx = 0;
			}
	}
	
/************************************************************************************
��������	App_FaultRecordTaxsis()
����������	���ϼ�¼���ݽ���Ԥ����:����
�������ã�	��			
************************************************************************************/
void	Class_FaultRecord::App_FaultRecordTaxsis()
{
	UINT16 u16Temp0;
	UINT16 *puint;
	INT16 i16Temp;
	
	//ָ���������1��ʼ��ַ
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
		puint = uFaultRecordBuff + (u16Temp0<<5) - u16Temp0;			//��0λ��8λ
	}	
	for(i16Temp=0; i16Temp<31; i16Temp++)
	{
		uFaultRecord[i16Temp] = *puint ;							//�ϴ�����ǰ�Ȱѹ��ϼ�¼�����������ݰᵽ���ϼ�¼��
		puint++;
	}
	
	//ָ���������2��ʼ��ַ
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
		uFaultRecord[i16Temp] = *puint ;							//��31��61λ
		puint++;
	}
	//ָ���������3��ʼ��ַ
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
		uFaultRecord[i16Temp] = *puint ;							//��62��92λ
		puint++;
	}
	//ָ���������4��ʼ��ַ
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
		uFaultRecord[i16Temp] = *puint ;							//��93��123λ
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
