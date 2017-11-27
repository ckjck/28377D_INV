/*==========================================================*/ 	
/* Title		:	InverterControl.cpp											*/ 	
/* Description	: 	Class_InverterControl definition									*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#include "InverterControl.h"


/************************************************************************************
��������	App_InvCtrlVarInit()	
����������	InverterControl�������ʼ��		
�������ã�	
***********************************************************************************/
inline	 void	Class_InverterControl::App_InvCtrlVarInit(void)
	{
		//INT16ernal Status
		m_i16FlagOffReason = 3;
	}
/************************************************************************************
��������	App_InvCtrl()
����������	�������Ϊ����ģ�飬����������Ŀ��ػ�������������ֵ����λ���٣����ش���������
					�ź��߿���
�������ã�
			App_InvOnoffAgent()�� 		����������ػ�����û�������ڲ��������ģ��
			App_InvOnoffAction()��		����������ػ�����ִ��ģ��
			App_InvMaster():				�����������ģ�飨���жϳ����е�MASTER_IDģ����ϣ�
			App_InvTrace():				�������λ����ֵ���ٿ���ģ�飬����������LBS�����ĸ�������
			App_InvSoftstart():			����������������ȵ���ģ��
			App_InvStandby():				������л�����״̬�б�ģ��
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
��������	App_InvOnoffAgent()
����������	����������ػ�����û�������ڲ��������ģ��
�������ã�	App_InvOffReason()				ά���ػ�ԭ���ӳ���
************************************************************************************/


	void	Class_InverterControl::App_InvOnoffAgent()
	{
		
		//��������������־ά��
		if(m_st_wFlagInvOnoff.bFinalOnOff == 0)
		{
			if((objSwitches.m_st_FlagBreaker.bBpSTSShort == 1)||(objSwitches.m_st_FlagBreaker.bInvKMShort == 1)||(objSwitches.m_st_FlagBreaker.bInvKMOpen == 1)
				||(objSwitches.m_st_FlagBreaker.bFuse == 0)
				||(objDigitalIO.m_st_wLocalSignal.bModuleReadyOk ==0)
				||(objDigitalIO.m_st_HardwareSignal.bRecOff == 1))
			{
				objSystem.m_st_wFlagPrompt.bInvOnEnable = 0;				//������ֹ
				S2U(m_st_wFlagInvOnoff) &= 0xFFF8;				//�����п������� BIT2 1 0
			}

			//�ڲ�������رպ� �� ���ڲ�ά����·�պ� �� ���ⲿά�޿��رպ� �� �ⲿ������رպϣ���
			else if((objDigitalIO.m_st_HardwareSignal.bQ5 == 1) && ((objDigitalIO.m_st_HardwareSignal.bQ3BP == 1) 
			   		||((objParallel.m_st_wParaSignalF.bCabQ3BpStatusF==1) && (objParallel.m_st_wParaSignalF.bExtOutStatusF == 1))))
			{
				objSystem.m_st_wFlagPrompt.bInvOnEnable = 0;				//������ֹ
				S2U(m_st_wFlagInvOnoff) &= 0xFFF8;				//�����п������� BIT2 1 0
			}
			
			else if((S2U(objSystem.m_st_wFlagFault)&0x2DB1) != 0)		//except Bit15 14 12 9 6 3 2 1

			{
				objSystem.m_st_wFlagPrompt.bInvOnEnable = 0;				//������ֹ
				S2U(m_st_wFlagInvOnoff) &= 0xFFF8;				//�����п������� BIT2 1 0
			}
			else if(objSystem.m_st_wFlagPrompt.bPow15vFault == 1)
			{
				objSystem.m_st_wFlagPrompt.bInvOnEnable = 0;				//������ֹ
				S2U(m_st_wFlagInvOnoff) &= 0xFFF8;				//�����п������� BIT2 1 0
			}
			else if(m_i16FlagOffReason >= 5)
			{
				objSystem.m_st_wFlagPrompt.bInvOnEnable = 0;				//������ֹ
				S2U(m_st_wFlagInvOnoff) &= 0xFFF8;				//�����п������� BIT2 1 0
			}
			else if((m_i16FlagOffReason == 4)
				&& (objSystem.m_st_wFlagStatus.bEPO == 1))
			{
				objSystem.m_st_wFlagPrompt.bInvOnEnable = 0;				//������ֹ
				S2U(m_st_wFlagInvOnoff) &= 0xFFF8;				//�����п������� BIT2 1 0
			}
			
			//���ϻ�ģʽ�£���·������������տ��Ͽ�����ֹ����			
			else if((objSystem.m_st_wFlagSystem.bSelfAgingAction == 1) 
//				&& ( m_st_FlagVbpTotal.bBpRange!=0||m_st_FlagBreaker.bQ5!=1 ) )
				&&( (objBypass.m_st_FlagVbpTotal.bBpRange > 2)||(objSwitches.m_st_FlagBreaker.bQ5 != 1) ) )			
			{
				objSystem.m_st_wFlagPrompt.bInvOnEnable = 0;				//������ֹ
				S2U(m_st_wFlagInvOnoff) &= 0xFFF8;				//�����п������� BIT2 1 0
			}
		
			else if((m_i16FlagOffReason == 4) &&(objSystem.m_st_wFlagStatus.bOutDisable == 1))
			{
				objSystem.m_st_wFlagPrompt.bInvOnEnable = 1;				//��������
				S2U(m_st_wFlagInvOnoff) &= 0xFFF8;				//�����п������� BIT2 1 0
			}
		
			else
			{
				objSystem.m_st_wFlagPrompt.bInvOnEnable = 1;				//��������
			}
        }

		// ��ǿ���
		if(m_st_wFlagInvOnoff.bFinalOnOff == 0)					  	 		//�ػ�״̬
		{
			
			if((S2U(m_st_wFlagInvOnoff) & 0x7FF0) != 0)					  	//�����κιػ����
			{
				S2U(m_st_FlagInvOnoffBak) |= (S2U(m_st_wFlagInvOnoff)&0x7FF0); 	//�����µĹػ�����
			}	
		    else
		    {
				S2U(m_st_FlagInvOnoffBak) &= 0xFFF0;					  	//�������п���������û��ػ���������л���
				S2U(m_st_FlagInvOnoffBak) |= (S2U(m_st_wFlagInvOnoff)&0x0007); 	//��¼�µĿ�������
		    }	
			
		}
		else														  	//����״̬
		{
			if((S2U(m_st_wFlagInvOnoff)&0x7FF0) != 0)						  	//�����κιػ����
			{
				S2U(m_st_FlagInvOnoffBak) &= 0x000F;					  	//�������йػ�����
				
				S2U(m_st_FlagInvOnoffBak) |= (S2U(m_st_wFlagInvOnoff)&0x7FF0); 	//��¼�µĹػ�����
			}
				
		}


		//��������ػ�����
		if((S2U(m_st_wFlagInvOnoff)&0x7FF0) != 0)				//�����κιػ�����
		{
			if(m_st_wFlagInvOnoff.bNeverRecoverOff == 1)		//13
			{
				m_st_wFlagInvOnoff.bFinalOnOff = 0;
				m_i16FlagOffReason = 5;
			}

			else if((m_st_wFlagInvOnoff.bOutDisableOff == 1)
					||(objDigitalIO.m_st_wHardwareNewSignal.bModuleReady  == 0) //ģ��û��׼���ò�������
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

			//�û��ػ����л�����ػ�  �û��ػ��ĵڶ���ѭ�������л��Թػ����ڵ�����ѭ��ִ���û��ػ�
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
		else											//���������										
		{

			if((m_st_wFlagInvOnoff.bFinalOnOff == 0) && ((S2U(m_st_wFlagInvOnoff)&0x0007) != 0))
			{
				m_st_wFlagInvOnoff.bFinalOnOff = 1;
			}	
		}	

		S2U(m_st_wFlagInvOnoff) &= 0x8008;					//�����ܿ��ػ�λ���û��ػ��л�λ
		objInvCtrl.App_InvOffReason();	
	}


/************************************************************************************
��������	App_InvOffReason()
����������	��ģ������ά���ػ�ԭ�򣬻�ά������л��ػ���ʱ�Կ�������
�������ã�	��
************************************************************************************/

	void	Class_InverterControl::App_InvOffReason()
	{
		if ( m_st_wFlagInvOnoff.bFinalOnOff == 1 )								//����
		{
			m_i16FlagOffReason = 0;
			m_i16AutoOnDlyCntPer40ms = 0;
		}
		else																//�ػ�
		{
			if ( m_i16FlagOffReason == 1 )
			{
				if ( objSystem.m_st_wFlagSystem.bTimer40ms == 1 )
				{
					//zqf 060616-�ӿ����ʱ�䣺10s-->1s
					if ( m_i16AutoOnDlyCntPer40ms >= 25 )								
					{
						//zqf 061106-��Сʱ�����л��������������������������Сʱ������					
						if(objSystem.m_st_wFlagPrompt.bSwitchTimesClamp == 0)
						{
							if((objXfer.m_st_XferResult.bBpSideDelay != 0)
							&&(objXfer.m_i16BPDelayCnt < 120))
							{
								m_st_wFlagInvOnoff.bSwitchAutoOn = 0;				//������л��Կ�������
							}
							else
							{
								m_st_wFlagInvOnoff.bSwitchAutoOn = 1;				//������л��Կ�������
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
��������	App_InvOnoffAction()
����������	��ģ�鴦������������ػ�������״̬ת������ά���������״̬��־
������ã?��
************************************************************************************/

	void	Class_InverterControl::App_InvOnoffAction()
	
	{
		if ( m_st_wFlagInvOnoff.bFinalOnOff == 1 )								//����
		{
			if ( objInverter.m_st_wFlagInv.bInv != 0 )										//����״̬
			{
				if ( ( objInverter.m_st_wFlagInv.bInv == 1 ) && ( objSystem.m_st_wFlagSystem.bTimer40ms == 1 ) )
				{
					m_i16InvSSCntPer40ms++;
					if ( m_i16InvSSCntPer40ms >= 500 )								//zqf 060616: 5s-->50s
					{
						objInverter.m_st_wFlagInv.bInv = 3;									//�������
					}
				}
			}
			else															//�ػ�״̬
			{				                                           
				objInverter.m_st_wFlagInv.bInv = 1;											//������
				m_i16InvSSCntPer40ms = 0;												//�������������ʱ
//				objParallel.m_st_wParaSignal.bInvState=1;									//INV_STAT=1
			}
		}
		else																//�ػ�
		{
			if ( objInverter.m_st_wFlagInv.bInv != 0 )										//����״̬
			{					                                           
				objInverter.m_st_wFlagInv.bInv = 0;											//�ر������
				m_i16InvSSCntPer40ms = 0;												//�������������ʱ
				objXfer.m_i16wBptoinvSwCntPer40ms = 0;											//��·������л��ȴ���ʱ
//				objParallel.m_st_wParaSignal.bInvState=0;									//INV_STAT=0
			}
		}		

	}
	
/************************************************************************************
��������	App_InvMaster()
����������	��ģ�����ھ����������������俪��������տ����Ϻ󣬱����ż��������������У�
			���жϳ����е�MASTER_IDģ����ϡ�
�������ã�	��
************************************************************************************/


	void	Class_InverterControl::App_InvMaster()
	
	{
		INT16		i16TempINVOnMod,i16Temp;
// ������俪��״̬��ɢ�źŵ��γ�
		i16TempINVOnMod = objDigitalIO.m_st_wHardwareNewSignal.bInvID;
		if ( ( objInverter.m_st_wFlagInv.bInv != 0 ) && ( objSwitches.m_st_FlagBreaker.bQ5 == 1 ) )		//�����������Q5��
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
		
            //��������

			if ( ( objInverter.m_st_wFlagInv.bInv != 0 ) && ( objSwitches.m_st_FlagBreaker.bQ5 == 1 ) )		//�����������Q5��
			{
				if(objParallel.m_st_wParaSignal.bLocalMaster == 0)						//������������
				{
					if(objParallel.m_st_wParaSignalF.bMasterOnlineF == 0)                 //ϵͳ������
					{
						i16Temp = (i16TempINVOnMod<<2)-3;
						
						if(++m_i16MasterOnlineConfCnt >= i16Temp)
						{
							m_i16MasterOnlineConfCnt = i16Temp;
							objParallel.m_st_wParaSignal.bLocalMaster = 1;				 //�ñ���Ϊ����
						}

					}
					else
					{
						m_i16MasterOnlineConfCnt=0;
				        objParallel.m_st_wParaSignal.bLocalMaster= 0;				 //ϵͳ���������ñ���Ϊ�ӻ�
					}
				}
			}
			else
			{
				m_i16MasterOnlineConfCnt = 0;
				objParallel.m_st_wParaSignal.bLocalMaster= 0;				 //���δ������Q5�ϣ��ñ���Ϊ�ӻ�
			}
		

//�������յĵ�λ,��ϵͳ��������ϵͳ�ӻ�
         if((objParallel.m_st_wParaSignal.bLocalMaster==1)
         &&(objParallel.m_st_wParaSignalF.bLeader_Rack==1))
         	{

			objParallel.m_st_wParaSignal.bMasterOnline = 1;	

		    }
        else
        	{
			objParallel.m_st_wParaSignal.bMasterOnline = 0;				
		    }


		
//��Ƶͬ���źŵķ���
		if(objParallel.m_st_wParaSignal.bMasterOnline == 1) 				//�������͹�Ƶͬ���ź�
		{
			EALLOW;
			EPwm6Regs.TZCLR.bit.OST = 1;
			EDIS;
		}
		else												                //�ӻ���ֹ���͹�Ƶͬ���ź�
		{
			EALLOW;
			EPwm6Regs.TZFRC.bit.OST = 1;
			EDIS;
		}

	}





/************************************************************************************
��������	App_InvTrace()
����������	��������ٿ��Ƽ������ж�
�������ã�	App_TraceSignal()�� 		����ϵͳ���������ź�SYSTEM_UNLOCK����
			App_TraceControl()��		�������λ�����ȸ���Ŀ�����
			App_TraceJudge()��		�������λ������������־����
************************************************************************************/

	void Class_InverterControl::App_InvTrace()
	{

		App_TraceSignal();
		
		App_TraceControl();

		App_TraceJudge();
				
	}

/************************************************************************************
��������	App_TraceSignal()
����������	����ϵͳ���������ź�SYSTEM_UNLOCK����
�������ã�	��		
************************************************************************************/
	
	void	Class_InverterControl::App_TraceSignal()
	
	{
		if ( objParallel.m_st_wParaSignal.bMasterOnline == 1 )				//����
		{
			
			if ( objInverter.m_st_wFlagInv.bPhaseTrace == 3 )				//��LBSԴ			
			{
				objParallel.m_st_wParaSignal.bSystemUnlock = 1;				//SYSTEM_UNLOCK=1
			}
			else															//����LBSԴ
			{

				if ( ( objBypass.m_st_FlagVbpTotal.bBpRange == 0 ) && ( objInvAlg.m_st_wIntMainFlag1_0.bBpPhaseLock == 1 ) )	//��·���ٷ�Χ��������λ����
				{
					objParallel.m_st_wParaSignal.bSystemUnlock = 0;			//SYSTEM_UNLOCK=0

				}
				else														//��·���ٷ�Χ�������λδ����
				{
					objParallel.m_st_wParaSignal.bSystemUnlock = 1;			//SYSTEM_UNLOCK=1
				}
			}
		}
		else																//�ӻ�									
		{
			if ( ( objInverter.m_st_wFlagInv.bInv != 0 ) && ( objSwitches.m_st_FlagBreaker.bQ5 == 1 ) )		//�����������Q5��
			{
				if ( objBypass.m_st_FlagVbpTotal.bBpRange == 0 )							//��·���ٷ�Χ����
				{
					objParallel.m_st_wParaSignal.bSystemUnlock = 0;							//SYSTEM_UNLOCK=0
				}
				else														//��·���ٷ�Χ����
				{
					objParallel.m_st_wParaSignal.bSystemUnlock = 1;							//SYSTEM_UNLOCK=1
				}
			}
			else															//�����δ������Q5��	
			{
				objParallel.m_st_wParaSignal.bSystemUnlock = 0;								//SYSTEM_UNLOCK=0
			}
		
		}
	




}

/************************************************************************************
��������	App_TraceControl()
����������	�������λ�����ȸ���Ŀ�����
�������ã�	l_div_i()					������
************************************************************************************/

	void Class_InverterControl::App_TraceControl()
	
	{		
		if ( ( objInverter.m_st_wFlagInv.bInv == 3 ) && ( objSwitches.m_st_FlagBreaker.bQ5 == 1 ) )
		{
			if(objParallel.m_st_wParaSignal.bMasterOnline == 1 )					//����������
			{
				if ( objParallel.m_st_wParaSignal.bInvBpStatus == 1 )						//��������������
				{
					if ((((objLBS.m_st_LBSStatus.bLBSDetect == 2) && (objMonInterface.m_st_wUPSModeRun.bLBS != 0))
						||((objLBS.m_st_LBSStatus.bLBSDetect == 1) && (objMonInterface.m_st_wUPSModeRun.bLBS == 1)))
						&&(objInvAlg.m_i16DthetaLBSBase_0 > objInvAlg.m_i16FreqSetMin_0)
						&&(objInvAlg.m_i16DthetaLBSBase_0 < objInvAlg.m_i16FreqSetMax_0))
					{
						objInverter.m_st_wFlagInv.bPhaseTrace = 3;                //һ���������LBS
						objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 1;						//����������ٹ�Ƶͬ���ź�
						
						objInverter.m_st_wFlagInv.bAmpTrace = 0;							//��ֵ�
						objInverter.m_i16wVrefTgtMd = objInverter.m_i16VrefSetMd;				
					}

					else if ( objParallel.m_st_wParaSignalF.bSystemUnlockF == 0)		//��·�ɸ��� ���������κ�һ������ģ����·�쳣����ֵ��������·
					{
						objInverter.m_st_wFlagInv.bPhaseTrace = 1;                       //һ�����������·
//						objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 0;					//��������Ҳ������·
						objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 1;					//����������ٹ�Ƶͬ���ź�
						if((objMonInterface.m_st_wUPSModeRun.bConfig == 0)&&(objMonInterface.m_st_wUPSModeRun.bMode == 1)&&(objXfer.m_st_XferResult.bEcoInvMdUpFlag==1))
						{
							objInverter.m_st_wFlagInv.bAmpTrace = 1;					//��ֵ����·
							objInverter.m_i16wVrefTgtMd =  objBypass.m_unVbpMMean_0.half.hword+52;	
						}
						else
						{
							objInverter.m_st_wFlagInv.bAmpTrace = 0;					//��ֵ�
							objInverter.m_i16wVrefTgtMd = objInverter.m_i16VrefSetMd;
						}
					}
					else if(objBypass.m_st_FlagVbpTotal.bBpTrace == 1)					//��·���ٷ�Χ����
					{
						objInverter.m_st_wFlagInv.bPhaseTrace = 1;						//һ�����������·
						objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 1;					//����������ٹ�Ƶͬ���ź�
						objInverter.m_st_wFlagInv.bAmpTrace = 0;							//��ֵ�
						objInverter.m_i16wVrefTgtMd = objInverter.m_i16VrefSetMd;
					}
					else
					{
						objInverter.m_st_wFlagInv.bPhaseTrace = 0;						//һ�����౾��
						objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 1;					//����������ٹ�Ƶͬ���ź�
						objInverter.m_st_wFlagInv.bAmpTrace = 0;							//��ֵ�
						objInverter.m_i16wVrefTgtMd = objInverter.m_i16VrefSetMd;
					}
				}
				else if ( objXfer.m_st_wXferCmd.bSwitchStatus == 0 )						//INV_STS=0,������·��
				{
					if ( objParallel.m_st_wParaSignalF.bSystemUnlockF == 0)		//��·�ɸ��� �����������κ�һ������ģ����·�쳣����ֵ��������·
					{
						objInverter.m_st_wFlagInv.bPhaseTrace = 1;                       //һ�����������·
//						objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 0;					//��������Ҳ������·
						objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 1;					//����������ٹ�Ƶͬ���ź�
						
						objInverter.m_st_wFlagInv.bAmpTrace = 1;							//��ֵ����·
						objInverter.m_i16wVrefTgtMd =( objBypass.m_unVbpMMean_0.half.hword+52);
					}
					else if(objBypass.m_st_FlagVbpTotal.bBpTrace == 1)					//��ģ����·�ɸ��٣���������ģ����·���ɸ���
					{
						objInverter.m_st_wFlagInv.bPhaseTrace = 1;						//һ�����������·
						objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 1;					//����������ٹ�Ƶͬ���ź�
						objInverter.m_st_wFlagInv.bAmpTrace = 0;							//��ֵ�
						objInverter.m_i16wVrefTgtMd = objInverter.m_i16VrefSetMd;
					}
					else
					{
						objInverter.m_st_wFlagInv.bPhaseTrace = 0;						//һ�����౾��
						objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 1;					//����������ٹ�Ƶͬ���ź�
						objInverter.m_st_wFlagInv.bAmpTrace = 0;							//��ֵ�
						objInverter.m_i16wVrefTgtMd = objInverter.m_i16VrefSetMd;
					}
				}

			}
			else															//�����Ǵӻ�
			{
				if ( objParallel.m_st_wParaSignalF.bInvBpStatusF == 1 )						//ϵͳ����������
				{
					if ( objParallel.m_st_wParaSignalF.bSystemUnlockF == 0)					//��·�ɸ��� ���������κ�һ������ģ����·�쳣����ֵ��������·
					{
						objInverter.m_st_wFlagInv.bPhaseTrace = 1;							//һ���������·
//						objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 0;						//�����������·
						objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 1;					//����������ٹ�Ƶͬ���ź�
						
						objInverter.m_st_wFlagInv.bAmpTrace = 0;								//��ֵ�
						objInverter.m_i16wVrefTgtMd = objInverter.m_i16VrefSetMd;
					}
					else													//SYSTEM_UNLOCK=1����·���ٷ�Χ����
					{
						objInverter.m_st_wFlagInv.bPhaseTrace = 2;							//һ��������ٹ�Ƶͬ���ź�
						objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 1;						//����������ٹ�Ƶͬ���ź�
						objInverter.m_st_wFlagInv.bAmpTrace = 0;								//��ֵ�
						objInverter.m_i16wVrefTgtMd = objInverter.m_i16VrefSetMd;
					}

				}
				else														//ϵͳ��������·��
				{
					if ( objXfer.m_st_wXferCmd.bSwitchStatus == 0 )					//������·��
					{
						if  ( objParallel.m_st_wParaSignalF.bSystemUnlockF == 0)            //��·�ɸ��� ���������κ�һ������ģ����·�쳣����ֵ��������·

						{
							objInverter.m_st_wFlagInv.bPhaseTrace = 1;						//һ���������·
//							objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 0;					//�����������·
							objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 1;					//����������ٹ�Ƶͬ���ź�
							
							objInverter.m_st_wFlagInv.bAmpTrace = 1;							//��ֵ��·
							objInverter.m_i16wVrefTgtMd = (objBypass.m_unVbpMMean_0.half.hword+52);
						}
						else												//SYSTEM_UNLOCK_F=1
						{
							objInverter.m_st_wFlagInv.bPhaseTrace = 2;						//һ��������ٹ�Ƶͬ���ź�
							objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 1;					//����������ٹ�Ƶͬ���ź�
							objInverter.m_st_wFlagInv.bAmpTrace = 0;							//��ֵ�
							objInverter.m_i16wVrefTgtMd = objInverter.m_i16VrefSetMd;
						}
					}
				}
			}
		}
		else if(objParallel.m_st_wParaSignal.bMasterOnline == 1 )							//����������
		{
			objInverter.m_st_wFlagInv.bPhaseTrace = 0;										//һ�����౾��
			objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 1;									//����������ٹ�Ƶͬ���ź�
			objInverter.m_st_wFlagInv.bAmpTrace = 0;											//����,��ֵ�
			objInverter.m_i16wVrefTgtMd = objInverter.m_i16VrefSetMd;
		}
		else
		{
			objInverter.m_st_wFlagInv.bPhaseTrace = 2;										//һ��������ٹ�Ƶͬ���ź�
			objInverter.m_st_wFlagInv.bPLL2PhaseTrace = 1;									//����������ٹ�Ƶͬ���ź�
			objInverter.m_st_wFlagInv.bAmpTrace = 0;											//��ֵ�
			objInverter.m_i16wVrefTgtMd = objInverter.m_i16VrefSetMd;
		}
	}	
								
/************************************************************************************
��������	App_TraceJudge()
����������	�������λ������������־����
�������ã�	��
************************************************************************************/
	
	void	 Class_InverterControl::App_TraceJudge()
	
	{
		if ( objInverter.m_st_wFlagInv.bInv == 0 )											//�����δ����
		{
			if ( objXfer.m_st_wXferCmd.bSwitchStatus == 1 )							//ϵͳ��������
			{
				objInverter.m_st_wFlagInv.bAmpLock = 0;										//�����������־
			}
			else															//ϵͳ������·��
			{
				objInverter.m_st_wFlagInv.bPhaseLock = 0;
				objInverter.m_st_wFlagInv.bAmpLock = 0;										//����λ������������־
			}
			
			m_i16AmpLockTgtCnt = 0;
			m_i16AmpLockBpCnt = 0;
		}
		else																//�����������
		{
 			//������������л���������ұ���,����乩������·������
			if ( ( objInverter.m_st_wFlagInv.bInv == 1 ))
//					||( (objXfer.m_st_wXferCmd.bSwitchStatus == 1) &&( objBypass.m_st_FlagVbpTotal.bBpRange == 3)  ) )	
			{
				objInverter.m_st_wFlagInv.bPhaseLock = 0;
				objInverter.m_st_wFlagInv.bAmpLock = 0;										//����λ������������־
			
				m_i16AmpLockTgtCnt = 0;
				m_i16AmpLockBpCnt = 0;
			}
			else															//�������������Ҳ�����
			{
				objInverter.m_st_wFlagInv.bPhaseLock = objInvAlg.m_st_wIntMainFlag.bPhaseLock;  				//��INT16MainFlag�п�����λ������־
				
				//�������ϻ�ģʽ���������������ϼ���Ϊ��������
				if(objSystem.m_st_wFlagSystem.bSelfAgingAction == 1)
				{
					objInverter.m_st_wFlagInv.bAmpLock = 1;
				}
				else														//����ֵ�Ƕֵ
				{
					if ( objInverter.m_st_wFlagInv.bAmpTrace == 1 )			//����·
					{
						m_i16AmpLockTgtCnt = 0;
						
						if ( ( ( objBypass.m_i16VbpaPrms - objInverter.m_i16VinvaPrms > 47 ) || ( objInverter.m_i16VinvaPrms - objBypass.m_i16VbpaPrms > 47) ) 
							|| ( (objBypass.m_i16VbpbPrms - objInverter.m_i16VinvbPrms > 47 ) || ( objInverter.m_i16VinvbPrms - objBypass.m_i16VbpbPrms > 47) ) 
							|| ( (objBypass.m_i16VbpcPrms - objInverter.m_i16VinvcPrms > 47 ) || ( objInverter.m_i16VinvcPrms - objBypass.m_i16VbpcPrms > 47) ) 
							|| ( ( objInverter.m_i16wVrefTgtMd - (objInverter.m_unVinvMMean_0.half.hword) ) > 132 )
							|| ( ( (objInverter.m_unVinvMMean_0.half.hword) - objInverter.m_i16wVrefTgtMd ) > 132 ))		//// 1.414*5V
						{
							objInverter.m_st_wFlagInv.bAmpLock = 0;			//�����������־
							m_i16AmpLockBpCnt = 0;
						}
						else
						{
							m_i16AmpLockBpCnt++;
							if(m_i16AmpLockBpCnt >= 1000)
							{
								m_i16AmpLockBpCnt = 1000;
								objInverter.m_st_wFlagInv.bAmpLock = 1;		//�÷���������־
							}
											
						}
					}
					else													//����											
					{
						m_i16AmpLockBpCnt = 0;
						
						if ( ( ( objInverter.m_i16wVrefTgtMd - (objInverter.m_unVinvMMean_0.half.hword) ) > 132 )
							|| ( ( (objInverter.m_unVinvMMean_0.half.hword) - objInverter.m_i16wVrefTgtMd ) > 132 ) )	// 1.414*5V
						{
							m_i16AmpLockTgtCnt++;
							if(m_i16AmpLockTgtCnt >= 1000)
							{
								m_i16AmpLockTgtCnt = 1000;
								objInverter.m_st_wFlagInv.bAmpLock = 0;				//�����������־
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
��������	App_InvSoftstart()
����������	��ģ�鴦������������������ѹ�ĸ���ֵ
�������ã�	��
************************************************************************************/

	void	Class_InverterControl::App_InvSoftstart()
	
	{
		if ( objInverter.m_st_wFlagInv.bInv == 0 )										//������ر�
		{
			objInvAlg.m_i16wVrefRunMd_0 = 0;
		}
		else															//���������
		{
			if ( objInvAlg.m_i16wVrefRunMd_0< objInverter.m_i16wVrefTgtMd )
			{		
				objInvAlg.m_i16wVrefRunMd_0 += 2;											//ԭ3
				if ( objInvAlg.m_i16wVrefRunMd_0 >= objInverter.m_i16wVrefTgtMd )
				{
					objInvAlg.m_i16wVrefRunMd_0 = objInverter.m_i16wVrefTgtMd;
				}
			}
			else if ( objInvAlg.m_i16wVrefRunMd_0 > objInverter.m_i16wVrefTgtMd )
			{
				objInvAlg.m_i16wVrefRunMd_0 -= 1;										//ԭ3
				if ( objInvAlg.m_i16wVrefRunMd_0 <= objInverter.m_i16wVrefTgtMd )
				{
					objInvAlg.m_i16wVrefRunMd_0 = objInverter.m_i16wVrefTgtMd;
				}
			}
		}
	}

/************************************************************************************
��������	App_InvStandby()
����������	��ģ�鴦�����ͬ�������ź�INV_ON_STANDBY
�������ã�	��
************************************************************************************/

	void	Class_InverterControl::App_InvStandby()
	
	{
		if ( ( objInverter.m_st_wFlagInv.bInv != 0 )  && ( objSwitches.m_st_FlagBreaker.bQ5 == 1 ) )			//�����������Q5��
		{
			
			if ( objXfer.m_st_wXferCmd.bSwitchStatus == 1 )							//���๩��
			{
				objParallel.m_st_wParaSignal.bInvOnStandby = 0;								//INV_ON_STANDBY=0
			}
			else															//bSwitchStatus=0,��·�๩��
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
