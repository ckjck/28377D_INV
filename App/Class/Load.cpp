/*==========================================================*/ 	
/* Title		:	Load.cpp														*/ 	
/* Description	: 	Class_Load definition											*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#include "Load.h"


/************************************************************************************
��������	Dat_LoadDeal()
����������	�����б�ģ�飬���ش�С�жϼ�������ʱ����
�������ã�	Dat_LoadFlag():		���ر�־����ģ�飬���ݵ�ǰ���������ฺ�صĴ�С���ӹ��ɱ�����为�ش�С�ı�־
			Dat_OvloadFlag():		����ȷ��ģ�飬���ݸ���˲ʱ���ص�������ӹ���ȷ�Ϻ�Ĺ��ر�־
			App_OvloadDelay():	������ʱ����ģ�飬���ݸ��ع��غ͸��ش�С��������ӹ������������ʱ����־
			App_OvloadSignal():	�����źŴ���ģ�飬���ڿ��Ʋ��������ź�INV_OVERLOAD�ķ���
************************************************************************************/

	void	Class_Load::Dat_LoadDeal(void)
	{
		Dat_LoadGrant();				//���ݻ����¶ȶ��й����ʹ����жϽ��н���

		Alg_ShortCircuitDetect();

		Dat_LoadFlag();
		
		Dat_OvloadFlag();
		
		App_OvloadDelay();
		
		App_OvloadSignal();

	}

/************************************************************************************
��������	Dat_LoadFlag()
����������	���ر�־����ģ�飬���ݵ�ǰ���������ฺ�صĴ�С���ӹ��ɱ�����为�ش�С�ı�־
�������ã�	igap()				�ز���ӳ���
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
��������	Alg_ShortCircuitDetect()
���������������·�����ж�ģ��
�������ã�	��			
************************************************************************************/
	void	Class_Load::Alg_ShortCircuitDetect(void)
	{

		//�����������ϣ�ĳ�ߵ�ѹ����110V�ҵ�������0.7���ֵ
		//Nxb+ͻ���������أ�����ѹ��һ����Ƶ�����ڻᱻ�������硣
		if ( ( objInverter.m_st_wFlagInv.bInv == 3 ) 
			&& ( ( (( objInverter.m_i16VinvaPrms < 370 )||(objInverter.m_i16VinvabPrms<370))&& ( objInverter.m_i16IinvaPrms > 717 ))
			|| ( (( objInverter.m_i16VinvbPrms < 370 )||(objInverter.m_i16VinvbcPrms<370))  && ( objInverter.m_i16IinvbPrms > 717  ))
			|| ( (( objInverter.m_i16VinvcPrms < 370 ) ||(objInverter.m_i16VinvcaPrms<370))&& ( objInverter.m_i16IinvcPrms > 717  ) )) )
											//���߼��·ʱ���ӵ���ѹ�ж�
		{
			objSystem.m_st_wFlagPrompt.bShortCircuit = 1;									//����������·��־
		}
		else
		{
			objSystem.m_st_wFlagPrompt.bShortCircuit = 0;									//����������·��־
		}
	}
/************************************************************************************
��������	Dat_LoadFlag()
����������	���ر�־����ģ�飬���ݵ�ǰ���������ฺ�صĴ�С���ӹ��ɱ�����为�ش�С�ı�־
�������ã�	igap()				�ز���ӳ���
************************************************************************************/
	void	Class_Load::Dat_LoadFlag()

	{
		INT16	i16MainAX,i16MainCX;
		//�������๩��
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
		
		if ( objSystem.m_i16wNumMainloop == 5 )									//������为�ر�־����
		{
			i16MainCX = m_st_FlagInvLoadAB.bLoadA;								//�������A�ฺ�ر�־
			m_st_FlagInvLoadAB.bLoadA = igap ( m_i16PSinvaGrantMAX, &InvLoadTable, i16MainCX );//���ûز����

			i16MainCX = m_st_FlagInvLoadAB.bLoadB;								//�������B�ฺ�ر�־
			m_st_FlagInvLoadAB.bLoadB = igap ( m_i16PSinvbGrantMAX, &InvLoadTable, i16MainCX );//���ûز����

			i16MainCX = m_st_FlagInvLoadCT.bLoadC;						//�������C�ฺ�ر�־
			m_st_FlagInvLoadCT.bLoadC = igap ( m_i16PSinvcGrantMAX, &InvLoadTable, i16MainCX );//���ûز����

			//����й��ʹ��ش�����־�ϳ�
			i16MainCX = m_st_FlagILLoadAB.bLoadA;							//���������A�ฺ�ر�־
			m_st_FlagILLoadAB.bLoadA = igap ( objInverter.m_i16SLinva, &InvLoadTable, i16MainCX );	//���ûز����

			i16MainCX = m_st_FlagILLoadAB.bLoadB;							//���������B�ฺ�ر�־
			m_st_FlagILLoadAB.bLoadB = igap ( objInverter.m_i16SLinvb, &InvLoadTable, i16MainCX );	//���ûز����

			i16MainCX = m_st_FlagILLoadCT.bLoadC;							//���������C�ฺ�ر�־
			m_st_FlagILLoadCT.bLoadC = igap ( objInverter.m_i16SLinvc, &InvLoadTable, i16MainCX );	//���ûز����


			m_st_FlagILLoadCT.bLoadT = m_st_FlagILLoadAB.bLoadA | m_st_FlagILLoadAB.bLoadB
									| m_st_FlagILLoadCT.bLoadC;				//��������й����ܱ�־�ۺ�

//			if(m_st_FlagILLoadCT.bLoadT == 7)
//			{
//				m_st_FlagILLoadCT.bLoadT = 15;
//			}
//			else if(m_st_FlagILLoadCT.bLoadT == 15)
//			{
//				m_st_FlagILLoadCT.bLoadT = 31;
//			}
			
			m_st_FlagInvLoadCT.bLoadT = m_st_FlagInvLoadAB.bLoadA | m_st_FlagInvLoadAB.bLoadB
									| m_st_FlagInvLoadCT.bLoadC ;							//������为���ܱ�־�ۺ�

			m_st_FlagInvLoadCT.bLoadTotalT = m_st_FlagInvLoadCT.bLoadT  | m_st_FlagILLoadCT.bLoadT;  	//��м�����������ܱ�־
//			m_st_FlagInvLoadCT.bLoadTotalT = m_st_FlagInvLoadCT.bLoadT ;

			if((m_st_FlagInvLoadAB.bLoadA>2)||(m_st_FlagILLoadAB.bLoadA>2))	//A�����KVA��KW����
			{
				//�ж�A���Ƿ��������й���,����,���¹�������ֵ
				if((objInverter.m_i16SLinva > objInverter.m_i16IinvaPrms) && (objInverter.m_i16SLinva < objInverter.m_i16IinvaPrms*2))	//���������ų�������Ϊ0�����
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

			if((m_st_FlagInvLoadAB.bLoadB > 2)||(m_st_FlagILLoadAB.bLoadB > 2))	//B�����KVA��KW����
			{
				//�ж�B���Ƿ��������й���,����,���¹�������ֵ
				if((objInverter.m_i16SLinvb > objInverter.m_i16IinvbPrms) && (objInverter.m_i16SLinvb < objInverter.m_i16IinvbPrms*2))	//���������ų�������Ϊ0�����

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

			if((m_st_FlagInvLoadCT.bLoadC > 2)||(m_st_FlagILLoadCT.bLoadC > 2))	//C�����KVA��KW����
			{

				//�ж�C���Ƿ��������й���,����,���¹�������ֵ
				if((objInverter.m_i16SLinvc > objInverter.m_i16IinvcPrms) && (objInverter.m_i16SLinvc < objInverter.m_i16IinvcPrms*2))	//���������ų�������Ϊ0�����

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
		
		
		if ( objSystem.m_i16wNumMainloop == 6 )											//����������ر�־����
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

			i16MainCX = m_st_FlagOutLoadABCT.bLoadA;								//�������A�ฺ�ر�־
			m_st_FlagOutLoadABCT.bLoadA = igap ( i16MainAX, &OutLoadTable, i16MainCX );	//���ûز����

			i16MainAX=m_i16PSoutbGrantMAX+m_i16Powerbackb;
			i16MainCX = m_st_FlagOutLoadABCT.bLoadB;								//�������B�ฺ�ر�־
			m_st_FlagOutLoadABCT.bLoadB = igap ( i16MainAX, &OutLoadTable, i16MainCX );	//���ûز����

			i16MainAX=m_i16PSoutcGrantMAX+m_i16Powerbackc;
			i16MainCX = m_st_FlagOutLoadABCT.bLoadC;								//�������C�ฺ�ر�־
			m_st_FlagOutLoadABCT.bLoadC = igap ( i16MainAX, &OutLoadTable, i16MainCX );	//���ûز����

			m_st_FlagOutLoadABCT.bLoadT = m_st_FlagOutLoadABCT.bLoadA | m_st_FlagOutLoadABCT.bLoadB
										| m_st_FlagOutLoadABCT.bLoadC;									//������������ܱ�־�ۺ�
		}
	}

/************************************************************************************
��������	Dat_OvloadFlag()
����������	����ȷ��ģ�飬���ݸ���˲ʱ���ص�������ӹ���ȷ�Ϻ�Ĺ��ر�־
�������ã�	��
************************************************************************************/

	void	Class_Load::Dat_OvloadFlag()

	{
		if ( objSystem.m_st_wFlagSystem.bTimer40ms == 1 )									//40ms��ʱ��
		{
			if ( m_st_wFlagLoad.bPSoutHigh == 1 )								//���������
			{
				if ( ( m_st_FlagOutLoadABCT.bLoadT == 0 ) 
					&& ( objSystem.m_st_wFlagPrompt.bShortCircuit == 0 ) )					//�������<95%���ҷ�˲ʱ��·
				{
					m_st_wFlagLoad.bPSoutHigh = 0;								//����������ر�־
					m_i16PSoutHighConfCntPer40ms = 0;									//���������ȷ�ϼ���
				}		
			}
			else															//�����ⲻ����
			{
				if ( ( m_st_FlagOutLoadABCT.bLoadT == 7 ) 
					|| ( objSystem.m_st_wFlagPrompt.bShortCircuit == 1 ) )					//�������>105%��˲ʱ��·
				{
					m_i16PSoutHighConfCntPer40ms +=2;									//�������ȷ�ϼ�������
					
//					if ( objSystem.m_st_wFlagPrompt.bShortCircuit == 1 )					//˲ʱ��·
//					{
//						m_i16PSoutHighConfCntPer40ms ++;	
//					}
									
					if ( m_i16PSoutHighConfCntPer40ms >= 6 )
					{
						m_st_wFlagLoad.bPSoutHigh = 1;							//����������ر�־
						m_i16PSoutHighConfCntPer40ms = 6;
					}
				}
				else
				{
					m_i16PSoutHighConfCntPer40ms = 0;									//���������ȷ�ϼ���
				}
			}
		}

		if ( objXfer.m_st_wXferCmd.bSwitchStatus == 1 )								//�������๩��
		{
			if ( objSystem.m_st_wFlagSystem.bTimer40ms == 1 )								//40ms��ʱ��
			{
				if ( m_st_wFlagLoad.bOvload == 1 )								//����ܹ���
				{
					if ( ( m_st_FlagInvLoadCT.bLoadTotalT == 0 ) 
						&& ( objSystem.m_st_wFlagPrompt.bShortCircuit == 0 ) )				//��为��<95%���ҷ�˲ʱ��·
					{
						m_st_wFlagLoad.bOvload = 0;								//������ܹ��ر�־
						m_i16OvloadConfCntPer40ms = 0;									//������ȷ�ϼ�������
					}		
				}
				else														//���������
				{
					if ( ( m_st_FlagInvLoadCT.bLoadTotalT > 2 ) 
						|| ( objSystem.m_st_wFlagPrompt.bShortCircuit == 1 ) )				//��为��>105%��˲ʱ��·
					{
						m_i16OvloadConfCntPer40ms +=2;									//������ȷ�ϼ�������
						
						if ( objSystem.m_st_wFlagPrompt.bShortCircuit == 1 )					//˲ʱ��·
						{
							m_i16OvloadConfCntPer40ms ++;		
						}
						
						if ( m_i16OvloadConfCntPer40ms >= 8 )
						{
							m_st_wFlagLoad.bOvload = 1;							//������ܹ��ر�־
							m_i16OvloadConfCntPer40ms = 8;
						}
					}
					else
					{
						m_i16OvloadConfCntPer40ms = 0;									//��������ȷ�ϼ���
					}
				}
			}
		}
		else																//������·�๩��
		{
			m_i16OvloadConfCntPer40ms = 0;												//������ȷ��ϵ������
			if(objXfer.m_st_XferResult.bBpSTS == 1)
			{
				m_st_wFlagLoad.bOvload = m_st_wFlagLoad.bPSoutHigh;						//����ܹ��ر�־
			}	
			else
			{
				m_st_wFlagLoad.bOvload = 0;
			}
		}
	}


/************************************************************************************
��������	App_OvloadDelay()
����������	������ʱ����ģ�飬���ݸ��ع��غ͸��ش�С��������ӹ������������ʱ����־
�������ã�	��
************************************************************************************/

	void	Class_Load::App_OvloadDelay()

	{
		if ( objXfer.m_st_wXferCmd.bSwitchStatus == 1 )								//�������๩��
		{
			if ( objSystem.m_st_wFlagSystem.bTimer40ms == 1 )								//40ms��ʱ��
			{
				if ( m_st_wFlagLoad.bOvload == 0 )								//���������
				{
					m_i32OvloadDlyCntPer40ms -= 60;
					if ( m_i32OvloadDlyCntPer40ms < 0 )								//�޷�����
					{
						m_i32OvloadDlyCntPer40ms = 0;
					}
				}
				else														//����ܹ���
				{
					if ( ( m_st_FlagInvLoadCT.bLoadTotalT == 31 ) 
						|| ( objSystem.m_st_wFlagPrompt.bShortCircuit == 1 ) )				//��为��>150%��˲ʱ��· 
					{
						m_i32OvloadDlyCntPer40ms += 30600;

					}
					else if (m_st_FlagInvLoadCT.bLoadTotalT == 15 )					//150%>��为��>125%
					{
						m_i32OvloadDlyCntPer40ms += 60;
					}
					else															//125%>��为��>105%
					{
						m_i32OvloadDlyCntPer40ms += 12;
					}
					
					//������ʱ���㣬1Сʱ��1����
					if ( m_i32OvloadDlyCntPer40ms >= 91500 )
					{
						m_st_wFlagLoad.bInvOvloadTimeout = 1;					//��������ʱ��
						m_i32OvloadDlyCntPer40ms = 91500;
					}
				}
			}
			//(��/����)ϵͳ��·��������·��բ��δ��·������ѹ������
			//��·����������
			if ( ( objInvAlg.m_st_wIntMainFlag.bVinvFast == 1 ) && ( objBypass.m_st_FlagVbpTotal.bBpRange == 0 )
				&& ( objParallel.m_st_wParaSignalF.bSystemUnlockF == 0 ) && ( objSwitches.m_st_FlagBreaker.bBpSTSOpen == 0 ) 
				&& ( objBypass.m_i16VbpaPrms <= 1106 ) && ( objBypass.m_i16VbpaPrms >= 942 )
				&& ( objBypass.m_i16VbpbPrms <= 1106 ) && ( objBypass.m_i16VbpbPrms >= 942 )
				&& ( objBypass.m_i16VbpcPrms <= 1106 ) && ( objBypass.m_i16VbpcPrms >= 942 ) )
			{
				m_st_wFlagLoad.bImpactSwitch = 1;								//�ñ������������·��־
			}
		}
		else																//������·�๩��
		{
			m_st_wFlagLoad.bInvOvloadTimeout = 0;								//����������ʱ��
			m_i32OvloadDlyCntPer40ms = 0;												//������ʱ����������
			m_st_wFlagLoad.bImpactSwitch = 0;									//�屾���������·��־
		}
	}


/************************************************************************************
��������	App_OvloadSignal()
����������	�����źŴ���ģ�飬���ڿ��Ʋ��������ź�INV_OVERLOAD�ķ���
�������ã�	��
************************************************************************************/

	void	Class_Load::App_OvloadSignal()

	{
		//zqf 060413--���ǲ������ݴ�TESTģʽ��������ά����·������������������·
		//�����س�ʱ �� �ⲿά�޿��رպ� �� ���ڲ�ά�޿տ��������ⲿ����տ�Ҳ�պϣ�
		if ( (m_st_wFlagLoad.bInvOvloadTimeout == 1 ))
			//Nxmϵͳ�����¸�ģ��ά�޿տ��պϾ���"�û���������"���ٱ�"�ڻ�����ת��·"
//		|| ((objParallel.m_st_wParaSignalF.bCabQ3BpStatusF == 1) 
//			   ||((objDigitalIO.m_st_HardwareSignal.bQ3BP == 1 ) && ( objParallel.m_st_wParaSignalF.bExtOutStatusF == 1)))) 

		{
			objParallel.m_st_wParaSignal.bInvOverload = 1;
			m_i16OvloadSignalCntPerMloop = 400;
		}
		else
		{
			if ( m_st_wFlagLoad.bImpactSwitch == 1 )								//�����������·
			{
				objParallel.m_st_wParaSignal.bInvOverload = 1;
				m_i16OvloadSignalCntPerMloop = 20;
			}
			//��·�ڱ�����Χ�ڣ����ڲ���ϵͳ��·��ͬ�ļ��裩�ҷ���INV_OCPʱ�������������Ź����ź�	
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
