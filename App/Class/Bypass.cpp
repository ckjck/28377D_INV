/*==========================================================*/ 	
/* Title		:	Bypass.cpp								*/ 	
/* Description	: 	Class_Bypass definition				*/ 	
/* Date			:	Apr.2008								*/ 	
/*==========================================================*/
#include "Bypass.h"


/************************************************************************************
��������	App_BpVarInit()	
����������	System�������ʼ��		
�������ã�	
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
		
		m_i16KVbpa_0 = 4119;					//����ϵ��
		m_i16KVbpb_0 = 4119;                    //����ϵ��
		m_i16KVbpc_0 = 4119;                    //����ϵ��
		
		m_i16PointcntBpM_0 = K50HzPointCntCnst;
		m_unDthetaBpMean_0.half.hword=K50HzDthetaCnst;		//60Hz@8KHz
		
	}
/************************************************************************************
��������App_BpParamConfig()
************************************************************************************/
	void	Class_Bypass::App_BpParamConfig(void)
	{
		//��·��ѹ��Ƶ�ʷ�Χ����
		switch( objMonInterface.m_i16VbpUplimitSet)
		{
			case 0:											//��·������Χ����+15%
				m_i16VbpUplimit = 1178;						//��·������Χ����
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

		switch (objMonInterface.m_i16VbpDnlimitSet)		//��·������Χ����-20%
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
		
		//��·������Χ���ñ��
		m_i16VbpUpDnlimitSet1 = 5*objMonInterface.m_i16VbpUplimitSet + objMonInterface.m_i16VbpDnlimitSet;	//խ��·��Χ
		m_i16VbpUpDnlimitSet2 = 5*objMonInterface.m_i16VbpUplimitSet + 4;				//����·��Χ
	

		//��·Ƶ�ʷ�Χ���ñ��  
		m_i16FbpRangIndex =	4*objMonInterface.m_i16FbpLimitSet + objMonInterface.m_i16FbpTraceSet;
		//m_i16FbpRangIndex =	objMonInterface.m_i16FbpLimitSet + 16;
		//m_i16FbpRangIndex = 17;

		//���Ƶ������ӳ��  
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
					
			//��Ƶ�ʱ�����Χ�ϸ��ٷ�Χխʱ����Ƶ�ʸ��ٷ�ΧΪ׼����					
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
			//��Ƶ�ʱ�����Χ�ϸ��ٷ�Χխʱ����Ƶ�ʸ��ٷ�ΧΪ׼����					
			if(m_i16FbpUpLimit < m_i16FbpUpTrace)
			{
				m_i16FbpUpLimit = m_i16FbpUpTrace;
			}
			if(m_i16FbpDnLimit > m_i16FbpDnTrace)
			{
				m_i16FbpDnLimit = m_i16FbpDnTrace;
			}
		}
		
		//����ϵ������
		switch (objMonInterface.m_st_wUPSModeRun.bVoltOut)
		{
			case 0:																								//400v
				m_i16KVbpNormal = 4119;																			// �����������
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
		

		//��·��ѹ����ϵ��У��
		m_i16KVbpa_0 = (((INT32)objMonInterface.m_i16KsVbpaSet)*m_i16KVbpNormal )>>12;
		m_i16KVbpb_0 = (((INT32)objMonInterface.m_i16KsVbpbSet)*m_i16KVbpNormal )>>12;
		m_i16KVbpc_0 = (((INT32)objMonInterface.m_i16KsVbpcSet)*m_i16KVbpNormal )>>12;
		
		//��·�����㣬1024-->100%
//		m_i16OverCurrPoint=(((INT32)1126)*objMonInterface.m_i16GrantKVA)/objMonInterface.m_i16ModelKVA;	
		m_i16OverCurrPoint = 1024;

		//�����·�쳣ʱ����·ģֵ�ķ�ֵ
		m_i16VbpMDnlimitFast =((((((INT32)(m_i16VbpDnlimit1))<<2) * 5792) >>12) * 973) >> 10;   //Vrms * 4 * 1.414 * 0.95
		m_i16VbpMUplimitFast =((((((INT32)(m_i16VbpUplimit))<<2) * 5792) >>12) * 1075) >> 10;   //Vrms * 4 * 1.414 * 1.05
		m_i16DthetaBpUplimitFast = (((INT32)m_i16FbpUpLimit) * 1887)>>12;	//f/100/8000*1024*360;
		m_i16DthetaBpDnlimitFast = (((INT32)m_i16FbpDnLimit) * 1887)>>12;	//f/100/8000*1024*360;
	}

/************************************************************************************
��������	vBypass_Fault()
����������	��·�����ж�ģ�� 
�������ã�	��			
************************************************************************************/	
	void	Class_Bypass::Alg_BpFault(void)
	{
		if ( objParallel.m_st_wParaSignal.bInvBpStatus == 1 )							//���๩��
		{
			if ( (m_st_FlagVbpTotalWide.bBpRange==3) && (objLoad.m_st_wFlagLoad.bInvOvloadTimeout == 1) )//��������ʱ��
			{
				objInvCtrl.m_st_wFlagInvOnoff.bBpFailOff = 1;							//����·�쳣�ػ�λ
				objSystem.m_st_wFlagFault.bBpFailOff = 1;								//��·�쳣�ػ��澯����
			}
		}
		else															//��·�๩��
		{
			//����������ػ���·5���ӹ������ڻ�iBptoinvTimes����
			if ( (m_st_FlagVbpTotal.bBpRange == 3) && (( objLoad.m_st_wFlagLoad.bOvload == 1 ) 
				|| ( objXfer.m_st_XferResult.bBpSideDelay != 0 )
				|| (objSystem.m_st_wFlagPrompt.bSwitchTimesClamp==1)))
			
			{
				objInvCtrl.m_st_wFlagInvOnoff.bBpFailOff = 1;							//����·�쳣�ػ�λ
				objSystem.m_st_wFlagFault.bBpFailOff = 1;								//����·�쳣����λ
			}
		}
		

		// ���ϻ�ģʽ����������������£�������տ��Ͽ�  ��  ���δ����ʱ��·��������
		//					��  ��������������·���ɸ��ٻ���λδ������������·�쳣�ػ���		

		if((objSystem.m_st_wFlagSystem.bSelfAgingAction  == 1) && (objInverter.m_st_wFlagInv.bInv != 0) )
		{
			if(objSwitches.m_st_FlagBreaker.bQ5 ==0 )	
			{
					objInvCtrl.m_st_wFlagInvOnoff.bBpFailOff = 1;							//����·�쳣�ػ�λ
					objSystem.m_st_wFlagFault.bBpFailOff = 1;								//����·�쳣����λ
			}
//			else if(objXfer.m_st_wXferCmd.bSwitchStatus ==0 && m_st_FlagVbpTotal.bBpRange!= 0 )
			else if((objXfer.m_st_wXferCmd.bSwitchStatus  == 0)  && ( m_st_FlagVbpTotal.bBpRange > 2 ))
			{
					objInvCtrl.m_st_wFlagInvOnoff.bBpFailOff = 1;							//����·�쳣�ػ�λ
					objSystem.m_st_wFlagFault.bBpFailOff = 1;								//����·�쳣����λ
			}
			
			else if((objXfer.m_st_wXferCmd.bSwitchStatus  == 1)
				&&( (m_st_FlagVbpTotal.bBpTrace !=  1) || (objInverter.m_st_wFlagInv.bPhaseLock == 0)) )
			{
					objInvCtrl.m_st_wFlagInvOnoff.bBpFailOff = 1;							//����·�쳣�ػ�λ
					objSystem.m_st_wFlagFault.bBpFailOff = 1;								//����·�쳣����λ
			}
			else
			{}
				
		}	

		if ( ( objInverter.m_st_wFlagInv.bInv == 0 ) && ( objInvCtrl.m_i16FlagOffReason >=3 ) )				//������ѹر��ҹػ�ԭ��>=3
		{
			objSystem.m_st_wFlagFault.bBpFailOff = 0;										//����·�쳣����λ
		}
	}


/************************************************************************************
��������	vVbpseq_Fault()
����������	��·�����ж�ģ��
�������ã�	��			
************************************************************************************/

	void	Class_Bypass::Alg_BpSeqFault(void)
	{
		//����·��ѹ��Чֵ����100V@277Vʱ����ԭ״̬����
		if ( objSystem.m_st_wFlagSystem.bTimer40ms == 1 )							//40ms��ʱ��
		{

			if ( m_unVbpMMean_0.half.hword < 2091 )
			{
				if ( m_i16BpInvtCntPer40ms > 0 )    
				{
					m_i16BpInvtCntPer40ms --;	
				}
			}

			else if ( m_unDthetaBpBase_0.half.hword < -K30HzDthetaCnst )	//��·��ѹ>100V��Ƶ��<-30Hz
			{
				m_i16BpInvtCntPer40ms ++;									//��·����ȷ�ϼ�������
				if ( m_i16BpInvtCntPer40ms >= 10 )							//0.4s
				{
					m_i16BpInvtCntPer40ms = 200;								//ȷ�����򣬻ָ�ʱ�䱣֤Ϊ8s
					m_st_FlagVbpTotal.bBpInvtSeq = 1;					//����·����
					objSystem.m_st_wFlagFault.bBpInvtSeq = 1;						//����·�������
				}
			}
			else
			{
				if ( m_i16BpInvtCntPer40ms > 0 )
				{
					m_i16BpInvtCntPer40ms --;								//��·����ȷ�ϼ����ݼ�
				}
				
				if ( m_i16BpInvtCntPer40ms < 10 )
				{
					m_st_FlagVbpTotal.bBpInvtSeq = 0;					//����·����
					objSystem.m_st_wFlagFault.bBpInvtSeq = 0;						//����·�������
				}
			}

		}		

	}
/************************************************************************************
��������	Alg_BackFeedFault()
����������	IGBT����·���������ж�ģ��
�������ã�	��			
************************************************************************************/
void	Class_Bypass::Alg_BpBackFeedFault(void)
{	
	//��·���༰��������
	if(objSystem.m_st_wFlagSystem.bTimer14ms == 1)	
	{		
		if((objDigitalIO.m_st_wLocalSignal.bBpSTS == 0)&&((objBypass.m_st_FlagVbpTotal.bBpRange==3))
		 	&& (objOutput.m_i16VoutaPrms>739) && (objOutput.m_i16VoutbPrms>739) && (objOutput.m_i16VoutcPrms>739)		//�����ѹ����200V
			&&(objParallel.m_st_wParaSignal.bInvBpStatus== 1))					//������乩��
		{
			if (objDigitalIO.m_st_wHardwareNewSignal.bBPBackFeedDetect ==1)							//����·����IO�ź�
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
				objSystem.m_st_wFlagFault.bBypFeedBackFault=1;							//�͸���ذ壬�Լ����Դ������ϼ�¼					
			}
		}
		else
		{	
			m_i16FeedbackVcomCnt = 0;
		}
						
		if((objSystem.m_st_wFlagFault.bBypFeedBackFault==1)||(objSystem.m_st_wFlagStatus.bMainFeedBack == 1))
		{
			
			objDigitalIO.m_st_wLocalSignal.bBPFeedAction = 1;						//���ౣ��CB�ѿ��źţ��͸�����				
		}
		else
		{
			objDigitalIO.m_st_wLocalSignal.bBPFeedAction = 0;
		}
	}

}

/************************************************************************************
��������	Dat_TempCurrentGrant()
����������	��·������Ȩģ�飬���ݵ�ǰ���������¶ȣ�����·�������н���
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
��������	Dat_IbpFlag()
����������	��·������־����ģ�飬���ݵ�ǰ������·���������Ȩֵ�Ĵ�С���ӹ��ɱ�����·������С�ı�־
�������ã�	igap()				�ز���ӳ���
************************************************************************************/
void	Class_Bypass::Dat_IbpFlag(void)
{
	INT16	i16MainAX;

	//���������־������

	
		i16MainAX = m_st_FlagIbpABC.bBpA;
		m_st_FlagIbpABC.bBpA = igap(m_i16IbpaGrant, &BpCurrentTable, i16MainAX);

		i16MainAX = m_st_FlagIbpABC.bBpB;
		m_st_FlagIbpABC.bBpB = igap(m_i16IbpbGrant, &BpCurrentTable, i16MainAX);

		i16MainAX = m_st_FlagIbpABC.bBpC;
		m_st_FlagIbpABC.bBpC = igap(m_i16IbpcGrant, &BpCurrentTable, i16MainAX);
	

	//�ۺϸ����־���γ��ܱ�־
	m_st_FlagIbpTotal.bTotal = m_st_FlagIbpABC.bBpA | m_st_FlagIbpABC.bBpB | m_st_FlagIbpABC.bBpC;
}

/************************************************************************************
��������	App_OverCurrentDelay()
����������	��·������ʱģ�飬���ݹ�����־��ȷ����ͬ���ص����µ���ʱ
************************************************************************************/
void	Class_Bypass::App_OverCurrentDelay(void)
{
	if((objXfer.m_st_wXferCmd.bSwitchStatus == 0) && (objXfer.m_st_XferResult.bBpSTS == 1))	//��·����ʱ							
	{
		if ( objSystem.m_st_wFlagSystem.bTimer40ms == 1 )								//40ms��ʱ��
		{
			switch(m_st_FlagIbpTotal.bTotal)
			{
				case 15:
						m_i16OvCurrentDlyCntPer40ms += 3000;	//Ibp>400%ʱ��200ms	
						break;
				case 7:
						m_i16OvCurrentDlyCntPer40ms += 600;		//400%>Ibp>150%ʱ��1s
						break;
				case 3:
						m_i16OvCurrentDlyCntPer40ms += 10;		//150%>Ibp>125%ʱ��1min
						break;
				case 1:
						m_i16OvCurrentDlyCntPer40ms += 2;			//125%>Ibp>100%ʱ��5min
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
				

				//��ֹ����
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
��������	Alg_OvcurrFault()
����������	IGBT����·���������ж�ģ��
�������ã�	��			
************************************************************************************/
void	Class_Bypass::Alg_OvcurrFault(void)
{
	//��·�����磬����������
	if ( objDigitalIO.m_st_wLocalSignal.bBpSTS == 1 )							//��·�๩��
	{
		//���ݻ��ͽ���ʹ�ã��������ñ���m_i16OverCurrPoint����
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
��������	vVbp_Flag()			
����������	��·��־����ģ��
�������ã�	igap()		�ز���ӳ���
************************************************************************************/
	void	Class_Bypass::Dat_VbpFlag(void)
	{
		UINT16	u16Back, u16Temp;		
		INT16		i16MainAX;
		
		//��·�����ѹ�ز��խ��Χ��		
		if((m_i16VbpUpDnlimitSet1 >= 0) && (m_i16VbpUpDnlimitSet1 <= 14) )
		{ 
			u16Back = m_st_FlagVbpAB1.bVbpA;
			m_st_FlagVbpAB1.bVbpA = igap (m_unVbpaRms.half.hword, &VbpRangeTable[m_i16VbpUpDnlimitSet1], u16Back);	//���ûز����
			u16Back = m_st_FlagVbpAB1.bVbpB;
			m_st_FlagVbpAB1.bVbpB = igap (m_unVbpbRms.half.hword, &VbpRangeTable[m_i16VbpUpDnlimitSet1], u16Back);	//���ûز����
			u16Back = m_st_FlagVbpFC1.bVbpC;
			m_st_FlagVbpFC1.bVbpC = igap (m_unVbpcRms.half.hword, &VbpRangeTable[m_i16VbpUpDnlimitSet1], u16Back);	//���ûز����			
		}
		//BY wxw201004 ��������ʱ��·�ĵ�ѹ��������ѹ��������Լ������Լ���
		//�������Ƶ����52Hz-48Hz��������·�������ͳ�����״̬�仯
		
		
		//��·�����ѹ�ز����Χ��
		if((m_i16VbpUpDnlimitSet2 >= 0) && (m_i16VbpUpDnlimitSet2 <= 14) )
		{ 
			u16Back = m_st_FlagVbpAB2.bVbpA;
			m_st_FlagVbpAB2.bVbpA = igap (m_unVbpaRms.half.hword, &VbpRangeTable[m_i16VbpUpDnlimitSet2], u16Back);	//���ûز����
			u16Back = m_st_FlagVbpAB2.bVbpB;
			m_st_FlagVbpAB2.bVbpB = igap (m_unVbpbRms.half.hword, &VbpRangeTable[m_i16VbpUpDnlimitSet2], u16Back);	//���ûز����
			u16Back = m_st_FlagVbpFC2.bVbpC;
			m_st_FlagVbpFC2.bVbpC = igap (m_unVbpcRms.half.hword, &VbpRangeTable[m_i16VbpUpDnlimitSet2], u16Back);	//���ûز����			
		}	
	
		
		//zqf 060606
		//��·խ<-->��Χ��ѡ����л�
		u16Temp=(objXfer.m_st_XferResult.bInvKM == 1)
				&&((objInvCtrl.m_st_wFlagInvOnoff.bNeverRecoverOff == 1)
				||(objInvCtrl.m_st_wFlagInvOnoff.bInvFaultOff == 1)||(objSystem.m_st_wFlagPrompt.bPow15vFault == 1)
				||(objSystem.m_st_wFlagPrompt.bDCAbnormalOff == 1)||(objSystem.m_st_wFlagFault.bVoutAbnormal == 1)
				||(objInvCtrl.m_st_wFlagInvOnoff.bAutoRecoverOff == 1)||(objLoad.m_st_wFlagLoad.bInvOvloadTimeout == 1));							
		
		if((objInverter.m_st_wFlagInv.bInv == 3) &&(objXfer.m_st_XferResult.bBpSTS == 1)
			&&(((objMonInterface.m_st_wUPSModeRun.bConfig == 0) && (objMonInterface.m_st_wUPSModeRun.bMode == 1))
			||((objMonInterface.m_st_wUPSModeRun.bConfig == 2) && (objMonInterface.m_st_wUPSModeRun.bMasterECO == 1))))
		{
			m_st_wBpAmpRangeFlag.bLocalWide=0;							//ECOģʽ����·ֻѡ��խ��Χ
		}
		else
		{
			if(u16Temp)
			{
				m_st_wBpAmpRangeFlag.bLocalWide = 1;						//������·ѡ���Χ
			}
			else if((objXfer.m_st_XferResult.bInvKMBak == 0) && (objXfer.m_st_XferResult.bInvKM == 1))
			{
				m_st_wBpAmpRangeFlag.bLocalWide = 0;						//������·ѡ��խ��Χ
			}
		}
			
		if((objMonInterface.m_st_wUPSModeRun.bConfig == 3)&&(m_st_wBpAmpRangeFlag.bCANWide == 1))
		{
			m_st_wBpAmpRangeFlag.bParaWide = 1;							//������·ѡ���Χ
		}
		else
		{
			m_st_wBpAmpRangeFlag.bParaWide = 0;							//������·ѡ��խ��Χ
		}

		//���ο�Χ
		m_st_wBpAmpRangeFlag.bLocalWide = 0;
		m_st_wBpAmpRangeFlag.bParaWide = 0;	
			
		if((m_st_wBpAmpRangeFlag.bLocalWide == 1)||(m_st_wBpAmpRangeFlag.bParaWide == 1))
		{	
			S2U(m_st_FlagVbpAB) = S2U(m_st_FlagVbpAB2);
			m_st_FlagVbpFC.bVbpC =  m_st_FlagVbpFC2.bVbpC;
			m_i16VbpDnlimit = 512;										//��·������Χ���ޣ�-50%
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
			m_i16VbpOKCntPer40ms = 50;											//��·��խ��Χ����������Χ����������2s��ʱ
			m_i16VbpNOKCntPer14ms = 0;											//��·��խ��Χ����������Χ�����ټ�ʱ��Чֵ�оݵļ���������
		}
		
		m_st_wBpAmpRangeFlag.bLocalWideBak = m_st_wBpAmpRangeFlag.bLocalWide;	//����
		m_st_wBpAmpRangeFlag.bParaWideBak = m_st_wBpAmpRangeFlag.bParaWide;
		m_st_FlagVbpTotal.bBpRangeBak=m_st_FlagVbpTotal.bBpRange;			//������·���ټ��
		
		//��·Ƶ�ʻز��
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
		
		
		//���ڼ�ʱ��Чֵ����·��ѹ��־�ۺ�
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
					
					//zqf 060616--����ģʽ�£���ʱ��Чֵ�����޺ͳ������о�ʱ��ֿ�����
					if(((m_i16VbpNOKCntPer14ms >= 4 )&& (i16MainAX == 16))			//������ȷ��4��
						||((m_i16VbpNOKCntPer14ms>=12) &&( i16MainAX==1))		//������ȷ��12��
						||((m_i16VbpNOKCntPer14ms>=2) && (objInverter.m_st_wFlagInv.bInv==3)	//ECO��������ȷ��2��
						&&(((objMonInterface.m_st_wUPSModeRun.bConfig == 0) && (objMonInterface.m_st_wUPSModeRun.bMode == 1))
						||((objMonInterface.m_st_wUPSModeRun.bConfig == 2) && (objMonInterface.m_st_wUPSModeRun.bMasterECO == 1)))))
					{
						m_st_FlagVbpAB.bVbpA = m_st_FlagVbpAB.bVbpB = m_st_FlagVbpFC.bVbpC = i16MainAX;
						m_st_FlagVbpTotal.bBpRange = 3; 
						m_i16VbpOKCntPer40ms = 0;
						//By wxw20100118����ڶ��·�£��Ƚ���·������������Ȼ����������٣���������52Hz����48Hz�����⡣
						//return;
					}
			}				



			//��·��ѹ��Ƶ�ʱ�־�ۺ�	
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
			//zqf 061120--ȥ��ECO���������µ�ģֵ�оݣ���Ϊ����Щ��·���繤�������оݻ�
			//�ȿ����챨�����Ӷ����¼���л���
/*			else if((objInverter.m_st_wFlagInv.bInv == 3)					//ECO��������������·ģֵ�ж�
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

		//zqf 060606--������·��Χ��ѹ��Ƶ�ʱ�־�ۺ�
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
		
		//20050105 ZXF ������·���ټ��
		if((m_st_FlagVbpTotal.bBpRange < 2) && (m_st_FlagVbpTotal.bBpRangeBak > 2 )
		&&(m_i16Bp10sChkCntPer40ms == 0))
		{
				m_st_FlagVbpTotal.bBpQk10St = 1;		//�������·SCR���
		}

		if((m_st_FlagVbpTotal.bBpQk10St == 1) && (m_st_FlagVbpTotal.bBpQKfinish == 0)				 //��û�н�����·SCR���
			&& (objXfer.m_st_XferResult.bBpSTS == 1))		//��·SCRʵ�ʵ�ͨ
		{
			if(objSystem.m_st_wFlagSystem.bTimer40ms == 1 )
			{
				m_i16Bp10sChkCntPer40ms++;
				m_st_FlagVbpTotal.bBps10Fast = 1;
				if(m_i16Bp10sChkCntPer40ms >= 200)
				{
					m_st_FlagVbpTotal.bBps4FaultDelay = 1;			//�ϵ���·��ͨǰ8s�����п��������·���������
                                   m_st_FlagVbpTotal.bBpSTSOpenChkEnable=1;          //������·scr��·��Чֵ�о�

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
		
		//zqf 060714--���������ѹ����ֵ�ο��ļ���:4.12��ʽ
		//zqf 061013--��ֵ�ο���m_i16VrefSetMd����ΪiVrefRunMd_0
		objOutput.m_i16wVoutFastRef1 = (((((INT32)objInvAlg.m_i16wVrefRunMd_0)*922)>>10)*m_i16VbpDnlimit)>>10;	//m_i16VbpDnlimit*m_i16wVrefRunMd_0*0.9*4096/(1024*4096)
		//By wxw20100416 ���������ͻ����������û��ת��·�����������ѹ�쳣����ת���
		
		objOutput.m_i16wVoutFastRef2 = (((INT32)objInvAlg.m_i16wVrefRunMd_0)*553)>>10;						//60%
		
		
		objOutput.m_i16wVoutFastRef3 = (((INT32)objInvAlg.m_i16wVrefRunMd_0)*717)>>10;						//70%
		objOutput.m_i16wVoutFastRef4 = (((INT32)objInvAlg.m_i16wVrefRunMd_0)*1229)>>10;					//120%
	}

/************************************************************************************
//�������ƣ�    Dat_Int_BpPreDeal()
************************************************************************************/

#pragma CODE_SECTION(".Epwm1Funcs") 			

void 	Class_Bypass::Dat_Int_BpPreDeal(void)
{
	INT16	i16IntAx_0,i16IntAlpha_0,i16IntBeta_0;
	//��ȡ����
	m_i16Vbpa_0 = objADDrv.m_i16ADC_Vbpa_0;
	m_i16Vbpb_0 = objADDrv.m_i16ADC_Vbpb_0; 
	m_i16Vbpc_0 = objADDrv.m_i16ADC_Vbpc_0;

	// ��·���ѹ���㣬����Ϊ6.10
	m_i16Vbpa_0 = (((INT32)m_i16Vbpa_0)*m_i16KVbpa_0)>>10;
	m_i16Vbpb_0 = (((INT32)m_i16Vbpb_0)*m_i16KVbpb_0)>>10;
	m_i16Vbpc_0 = (((INT32)m_i16Vbpc_0)*m_i16KVbpc_0)>>10;

	
	
		// ��·��ѹʸ��ģ����λ����
		i16IntAlpha_0 = (((INT32)m_i16Vbpa_0*2-m_i16Vbpb_0-m_i16Vbpc_0)*K1div3Cnst)>>12;
		i16IntBeta_0 = (((INT32)m_i16Vbpb_0-m_i16Vbpc_0)*KSqrt3invCnst)>>12;

		i16IntAx_0 = (((INT32)i16IntAlpha_0)*i16IntAlpha_0+((INT32)i16IntBeta_0)*i16IntBeta_0)>>16;
		UPDNLMT16(i16IntAx_0,1023,1);
		m_i16VbpM_0 = sqrtTab[i16IntAx_0];
		m_i16SinQBp_0 = (((INT32)i16IntBeta_0)<<12)/m_i16VbpM_0;
		m_i16CosQBp_0 = (((INT32)i16IntAlpha_0)<<12)/m_i16VbpM_0;
	

}

/************************************************************************************
//�������ƣ�    Dat_BpRMSPower()
************************************************************************************/
void 	Class_Bypass::Dat_BpRMSPower(void)
{	
	INT32	i32temp;
	//��·���ѹ��Чֵ����
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
	 
	if((m_i16FbpMean>=3000) &&( m_i16FbpMean<=8000))		//m_i16PointcntBpM_0ά�� 
	{
		i32temp = ((INT32)5000)*K50HzPointCntCnst;
		m_i16PointcntBpM_0 = i32temp/m_i16FbpMean;
	}
	else
	{
		m_i16PointcntBpM_0 =K30HzPointCntCnst;	
	}


//	//��·�ߵ�ѹ��Чֵ����
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
//�������ƣ�    Dat_Int_BpCal()
//��������:     ��·��ѹ����ƽ���ͼ���
//�㷨˵��:	    1���˲������·��ѹ��3/2�任�������������ֵ������һ�������ڵ���
//		        Ӧֵ�����λ��پ��˲���õ���·ʸ���ǻ�������
//				2���ֱ�A��B��C��ĵ�ѹƽ���ۼ�
//			    3����������ʱ��ƽ���ͱ��ݣ��ҽ�������
************************************************************************************/
#pragma CODE_SECTION(".Epwm1Funcs") 			

void 	Class_Bypass::Dat_Int_BpCal(void)
{
	INT16 i16IntAx_0,i16IntBx_0,i16IntCx_0;
	// ������·ʸ���ǻ�������
	m_unVbpaFilter_0.dword = FILTER(2,m_i16Vbpa_0,m_unVbpaFilter_0.dword);
	m_unVbpbFilter_0.dword = FILTER(2,m_i16Vbpb_0,m_unVbpbFilter_0.dword);
	m_unVbpcFilter_0.dword = FILTER(2,m_i16Vbpc_0,m_unVbpcFilter_0.dword);

    // �˲������·��ѹʸ��ģ����λ����
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


	// ��·���ѹƽ�����ۼ�12.20��ʽ
	m_unVbpaSum_0.dword += ((((INT32)m_i16Vbpa_0)*m_i16Vbpa_0)>>4);	 
	m_unVbpbSum_0.dword += ((((INT32)m_i16Vbpb_0)*m_i16Vbpb_0)>>4);
	m_unVbpcSum_0.dword += ((((INT32)m_i16Vbpc_0)*m_i16Vbpc_0)>>4);

	// ��·�ߵ�ѹƽ�����ۼ�12.20��ʽ
	m_i16Vbpab_0 = m_i16Vbpa_0-m_i16Vbpb_0; 
	m_i16Vbpbc_0 = m_i16Vbpb_0-m_i16Vbpc_0; 
	m_i16Vbpca_0 = m_i16Vbpc_0-m_i16Vbpa_0; 
	m_unVbpabSum_0.dword += ((((INT32)m_i16Vbpab_0)*m_i16Vbpab_0)>>4);	 
	m_unVbpbcSum_0.dword += ((((INT32)m_i16Vbpbc_0)*m_i16Vbpbc_0)>>4);
	m_unVbpcaSum_0.dword += ((((INT32)m_i16Vbpca_0)*m_i16Vbpc_0)>>4);

	m_i16PointcntBp_0++;
	if(m_i16PointcntBp_0 >= m_i16PointcntBpM_0)				     // ��·����
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
