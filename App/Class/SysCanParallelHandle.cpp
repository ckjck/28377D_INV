/***************************************************************************************
* 
* Filename: 		SysCanParallelHandle.cpp
* Contributor:
* Author: 		liujie	
* Date: 			7/28/2008
* Copyright 		2008 ENPC Corporation, All Rights Reserved
*
* Description: 	
* 
* 
* Version
* Last rev by:		liujie
* Last rev date:	7/28/2008
* Last rev desc:	
****************************************************************************************/

#include "SysCanParallelHandle.h"



/* ***********************************************************
�ļ����ƣ�	void Class_CAN::vCAN_Var_Init( )
����������	CAN���������ʼ������Ĭ������¶���Ϊ�Ǵӻ�
�㷨˵����	none

��ڲ�����	none
���ڲ�����	sCanFlag
			ECanaRegs
			
�ֲ�������	i
				tempECanReg
�������ã�	none
*********************************************************** */

void SysCanParallelHandle::vCAN_Var_Init( )
{
	UINT16 i;
	struct	ECAN_REGS	tempECanReg;
	
	S2U ( sCanFlag ) = 0;
	iCanAloneConfirmCnt = 0;    
	//iCANTxPrdStatus = 1;
	sCanFlag.bCANTxPrdStatus = 1;
	iFrameIDLow = 1;  
	iFrameIDHigh = 1;
	sCanFlag.bAlone = 0; 
	sCanFlag.bUPSSingleOline = 0;		
	sCanFlag.bErr = 0;
	sCanFlag.bVersion = 1;
	sCanFlag.bReceived = 0;
	uMACIDFlag.iMACIDFlagWord = 0;
	sCanFlag.bCommErrJudgePeriod = 0; 
	sCanFlag.bVersionErr = 0;
	iVersionTxFlagCounter = 0;
		
	//�������������
	for (i=0; i<=ParaNumCNST-1; i++)
	{
		sParaData[i].m_i16Pouta = 0;
		sParaData[i].m_i16Poutb = 0;
		sParaData[i].m_i16Poutc = 0;
		sParaData[i].m_i16Souta = 0;
		sParaData[i].m_i16Soutb = 0;
		sParaData[i].m_i16Soutc = 0;
		sParaData[i].m_i16ModelKVA = 0;
		sParaData[i].m_i16wGrantKVA = 0;
		sParaData[i].uSystemFlag = 0;
		sMACIDErrCounter[i] = 0;//20061008
	}	

	//enable receive mailbox0~5
	ECanaRegs.CANME.all = 0;

	tempECanReg.CANME.all = ECanaRegs.CANME.all;
	tempECanReg.CANME.bit.ME0 = 1;
	tempECanReg.CANME.bit.ME1 = 1;
	tempECanReg.CANME.bit.ME2 = 1;
	tempECanReg.CANME.bit.ME3 = 1;
	tempECanReg.CANME.bit.ME4 = 1;
	tempECanReg.CANME.bit.ME5 = 1;
	ECanaRegs.CANME.all = tempECanReg.CANME.all;
	
	iLocalStatusChanged = 0; 						

	iPoutTotala = 0;
	iPoutTotalb = 0;
	iPoutTotalc = 0;
	iSoutTotala = 0;
	iSoutTotalb = 0;
	iSoutTotalc = 0;
	
//zqf 060615--������Ͳ�ͬ��������
	iKCapAdj=4096;
}



/* ***********************************************************
�ļ����ƣ�	void Class_CAN::vCAN_Agent( )
����������	CANͨ���������ڴ˽��������ա����͡������жϵ��ӳ���ĵ���
�㷨˵����	none

��ڲ�����	iStartTimeCount
			objSystem.m_st_wFlagSystem
			objMonInterface.m_st_wUPSModeRun
			ECanaRegs
			K180sBased40msCnst
			ParaNumCNST
			uCanDelay
			objTimerDrv.u16ClkUs_0
			iFrameIDLow
			m_i16Pouta
			m_i16Poutb
			m_i16Poutc
			m_i16Souta
			m_i16Soutb
			m_i16Soutc
			K42msCnst
			
			
���ڲ�����	iStartTimeCount
			sCanFlag
			ECanaRegs
			sParaData
			uFrameNo
			uCanDelay
			iParaRunFlag
			iSoutTotala
			iSoutTotalb
			iSoutTotalc
			iPoutTotala
			iPoutTotalb
			iPoutTotalc
			iPoutTotalMax
			iSoutTotalMax
			iPSoutTotalMax
			iNumInv
			iNumOn
			iVersionTxFlagCounter
			iCommErrJudgePeriod
			
			
�ֲ�������	
			i
			
�������ã�
			vCANLocalData( )
			vCAN_Rx()
			vCANCalculation( )
			vCANVersion_Tx()
			vCANHigh_Tx()
			vCANLow_Tx( )
			vCAN_Com_Err()
			
*********************************************************** */
	
void SysCanParallelHandle::vCAN_Agent( )
{
	UINT16	i;
//	struct	ECAN_REGS	tempECanReg;

	//cb 030808  CAN ���Ԥ�澯����ع���̨���ݴ�
	if(objXfer.m_st_XferResult.bInvKM==0)
	{
//				objMonitor.uInvRealInfo.bBattWarn=0;
//				objMonitor.uInvRealInfo.bBattInv=0;
		
	}			
	//cb_H 030414 5������ʱ����Ϊ3����
	if ( ( iStartTimeCount < K180sBased40msCnst ) && ( objSystem.m_st_wFlagSystem.bTimer40ms == 1 ) )
	//cb_E 030414
	{
		iStartTimeCount ++;		//�ӿ�����ʼ��ʱ����ʱ3���Ӻ�ֹͣ��ʱ
	}
	
	if ( ECanaRegs.CANES.bit.BO != 0 ) 
	{
		EALLOW;
		ECanaRegs.CANMC.bit.ABO = 1;
		EDIS;
	}
/*
	//20060419 ����ϵͳ����ģʽ�£��ɿ�����䣻
	if ( objSystem.m_st_wFlagSystem.bUPSTestMode == 1 )//����ģʽ?2
	{
		S2U( sCanFlag ) = 0;
		
		if ( objSwitches.m_st_FlagBreaker.bInv == 0 )
		{
			iNumInv = 0;
		}
		else
		{
			iNumInv = 1;
		}

		if ( objSwitches.m_st_FlagBreaker.bOn == 0 )
		{
			iNumOn = 0;
		}
		else
		{
			iNumOn = 1;
		}

		if ( objSwitches.m_st_FlagBreaker.bQ5 == 1 )
		{
			iPoutTotala = m_i16Pouta;
			iPoutTotalb = m_i16Poutb;
			iPoutTotalc = m_i16Poutc;

			iSoutTotala = m_i16Souta;
			iSoutTotalb = m_i16Soutb;
			iSoutTotalc = m_i16Soutc;
		}
		else
		{
			iPoutTotala = 0;
			iPoutTotalb = 0;
			iPoutTotalc = 0;

			iSoutTotala = 0;
			iSoutTotalb = 0;
			iSoutTotalc = 0;
		}

		iCapacitySum_InvOn = m_i16wGrantKVA;
	
		return;
	}
	
	//20060419 end
*/

	
	//zqf 061225--������·����̨��ά��
	//������ �����źŸ���̬ ��UPSTestMode
	if ( objMonInterface.m_st_wUPSModeRun.bConfig != 3 || objParallel.m_st_wParaMaint.bParaSignalMask == 1 || objSystem.m_st_wFlagSystem.bUPSTestMode == 1 )
	{
		S2U( sCanFlag ) = 0;
		
		if ( objSwitches.m_st_FlagBreaker.bInv == 0 )
		{
			iNumInv = 0;
			iCapacitySum_InvOn = 0;
		}
		else
		{
			iNumInv = 1;
			iCapacitySum_InvOn = objMonInterface.m_i16GrantKVA;					//��֤UPSTestMode�����ɿ���
		}

		if ( objSwitches.m_st_FlagBreaker.bOn == 0 )
		{
			iNumOn = 0;
		}
		else
		{
			iNumOn = 1;
		}

		//������·����״̬������ɲ���״̬�л�����������̬ʱ�����״̬����ˢ������(������Ե���)
		if ( objSwitches.m_st_FlagBreaker.bBpOn == 1 )						//��·����
		{
			iNumBp = 1;
		}
		else
		{
			iNumBp = 0;
		}

		if ( objSwitches.m_st_FlagBreaker.bQ5 == 1 )
		{
			iPoutTotala = objOutput.m_i16Pouta;
			iPoutTotalb = objOutput.m_i16Poutb;
			iPoutTotalc = objOutput.m_i16Poutc;

			iSoutTotala = objOutput.m_i16Souta;
			iSoutTotalb = objOutput.m_i16Soutb;
			iSoutTotalc = objOutput.m_i16Soutc;
		}
		else
		{
			iPoutTotala = 0;
			iPoutTotalb = 0;
			iPoutTotalc = 0;

			iSoutTotala = 0;
			iSoutTotalb = 0;
			iSoutTotalc = 0;
		}

		return;
	}
	
	/*  ������ϵͳ��ֻ��һ̨UPS����ʱ������������ڷ���״̬���򱾻��ķ�������λ��ʼ��Ϊ1��
		��CAN���߻ᱨ�����Ӷ�����CANͨ���޷��������С���ʱ�ѱ����������͹��ʼ���俪��
		״̬��Ϊ����ϵͳ�����������ܹ��ʡ�
	*/
	
	//�ӵ������߹��ɵ��������
	if ( ( sCanFlag.bAlone == 1 ) && ( sCanFlag.bUPSSingleOline == 0 ) )
	{
		sCanFlag.bAlone = 0;

		//��������־20061020
		ECanaRegs.CANES.all = ECanaRegs.CANES.all;
						
		for(i=1;i<=ParaNumCNST-1;i++)		//���ԭ������������
		{
			sParaData[i].m_i16Pouta = 0;
			sParaData[i].m_i16Poutb = 0;
			sParaData[i].m_i16Poutc = 0;
			sParaData[i].m_i16Souta = 0;
			sParaData[i].m_i16Soutb = 0;
			sParaData[i].m_i16Soutc = 0;
			sParaData[i].uSystemFlag = 0;
		}
	
		//cb 030904 ����bErr��־���˷�BE_ALONE ��BROADCAST֮���л�ʱ�Ĺ���©����
//wzh20051212
		//S2U ( sCanFlag ) &= CBIT3;
//wzh
		sCanFlag.bCANTxPrdStatus = 1;
		iFrameIDLow = 1;					//��1��֡��ʼ
	}
	
	//��ȡ��������
	vCANLocalData( );
	
	// ���ݽ��պ���
	vCAN_Rx();

	//�������ϵͳ��������
	if ( sCanFlag.bUPSSingleOline == 1 )
	{
		//vCAN_Be_Alone( );
		vCANCalculation( );
		sCanFlag.bAlone = 1;
	}

	//������յ�������
	if ( sCanFlag.bReceived == 1 )
	{
		vCANCalculation( );
		sCanFlag.bReceived = 0;
	}

	if ( ECanaRegs.CANTRS.bit.TRS10 == 0 )//�����������10Ϊ��

	{
	
		//���ͱ�������汾��
		if ( sCanFlag.bVersion == 1 )
		{
			vCANVersion_Tx();
			sCanFlag.bVersion = 0;
			iVersionTxFlagCounter = 0;
		}
	
		//�����������״̬�ı䣬���͸����ȼ���Ϣ֡
		else if (iLocalStatusChanged == 1) 
		{
			vCANHigh_Tx();
			iLocalStatusChanged = 0;
		}
	
		
		else  
		{
			//���͵����ȼ���Ϣ֡
			if ((iFrameIDLow <= 3) && (iFrameIDLow >= 1))
			{
				vCANLow_Tx( );
			}
		}
		
	}
	// ���ͺ�����ͨ�Ź����ж�
	if( sCanFlag.bCommErrJudgePeriod == 2 )
	{
		//iCANTxPrdStatus = 0;
		vCAN_Com_Err();//ͨ�Ź��ϴ���
		sCanFlag.bCommErrJudgePeriod = 0;
	}

				
	// ��������42ms��ʱ
	if ( sCanFlag.bCANTxPrdStatus == 1 )
	{
		sCanFlag.bCANTxPrdStatus = 0;		//�ÿ�ʼ��ʱ��־
		uCanDelay = objTimerDrv.m_u16ClkUs_0;	//��¼��ʼ��ʱʱ��
		return;
	}

	if ( ( ( ( objTimerDrv.m_u16ClkUs_0 + CLKUSPRDCNST ) < ( uCanDelay + K42msCnst ) ) && ( objTimerDrv.m_u16ClkUs_0 < uCanDelay ) )
		|| ( ( objTimerDrv.m_u16ClkUs_0 < ( uCanDelay + K42msCnst ) ) && ( objTimerDrv.m_u16ClkUs_0 > uCanDelay ) ) )
	{
		return;			//�ȴ���ʱ42ms��û�е�������
	}
	
	sCanFlag.bCANTxPrdStatus = 1;    //��ʱ42msʱ�䵽,���ñ�־λ
	iFrameIDLow = 1;
	sCanFlag.bCommErrJudgePeriod += 1;
	iVersionTxFlagCounter += 1;

	if ( iVersionTxFlagCounter == 120 )
	{
		sCanFlag.bVersion = 1;
	}
	
	
}



/*************************************************************
�ļ����ƣ�	vCan_Rx( )
����������	����ϵͳ�У���ѯ���������Ƿ����µ����ݡ���������ú���
			vCANDataRemove(),���յ��������ݷŵ���ʱbuffers�У���ͨ��
			vCANDataStore()�����ݽ��з����ŵ���ӦUPS�洢��Ԫ�С�
			
�㷨˵����	none

��ڲ�����	CANRxFlag
				ECanaRegs.CANRMP
				ECanaRegs.CANRML
				
���ڲ�����	
			
�ֲ�������	vMailBoxFlag
				vCANReceived
�������ã�vCANDataRemove( )
				vCANDataStore( )

*************************************************************/
void SysCanParallelHandle::vCAN_Rx( )
{
	char vMailBoxFlag;

	if ( ECanaRegs.CANRMP.all & CANRxFlag )
	{
		
		//�鿴����5�Ƿ���յ�����
		if ( ECanaRegs.CANRMP.bit.RMP5 == 1 )
		{
			vMailBoxFlag = 5;
			vCANDataRemove( vMailBoxFlag );	//�������Ƶ���ʱbuffer
			//ECanaRegs.CANRMP.bit.RMP5 = 1;	//�ͷ�����5�������½����µ�����
			ECanaRegs.CANRMP.all = 0x00000020;//�ͷ�����5�������½����µ�����20061008
			vCANDataStore( );	//�����ݷŵ���Ӧ��buffers
		}
		
		//�鿴����4�Ƿ���յ�����
		if ( ECanaRegs.CANRMP.bit.RMP4 == 1 )
		{
			vMailBoxFlag = 4;
			vCANDataRemove( vMailBoxFlag );	//�������Ƶ���ʱbuffer
			//ECanaRegs.CANRMP.bit.RMP4 = 1;	//�ͷ�����4�������½����µ�����
			ECanaRegs.CANRMP.all = 0x00000010;	//�ͷ�����4�������½����µ�����20061008
			vCANDataStore( );	//�����ݷŵ���Ӧ��buffers
		}
		
		//�鿴����3�Ƿ���յ�����
		if ( ECanaRegs.CANRMP.bit.RMP3 == 1 )
		{
			vMailBoxFlag = 3;
			vCANDataRemove( vMailBoxFlag );	//�������Ƶ���ʱbuffer
			//ECanaRegs.CANRMP.bit.RMP3 = 1;	//�ͷ�����3�������½����µ�����
			ECanaRegs.CANRMP.all = 0x00000008;	//�ͷ�����3�������½����µ�����20061008
			vCANDataStore( );	//�����ݷŵ���Ӧ��buffers
		}
		
		//�鿴����2�Ƿ���յ�����
		if ( ECanaRegs.CANRMP.bit.RMP2 == 1 )
		{
			vMailBoxFlag = 2;
			vCANDataRemove( vMailBoxFlag );	//�������Ƶ���ʱbuffer
			//ECanaRegs.CANRMP.bit.RMP2= 1;	//�ͷ�����2�������½����µ�����
			ECanaRegs.CANRMP.all = 0x00000004;	//�ͷ�����2�������½����µ�����20061008
			vCANDataStore( );	//�����ݷŵ���Ӧ��buffers
		}

		//�鿴����1�Ƿ���յ�����
		if ( ECanaRegs.CANRMP.bit.RMP1 == 1 )
		{
			vMailBoxFlag = 1;
			vCANDataRemove( vMailBoxFlag );	//�������Ƶ���ʱbuffer
			//ECanaRegs.CANRMP.bit.RMP1= 1;	//�ͷ�����1�������½����µ�����
			ECanaRegs.CANRMP.all = 0x00000002;	//�ͷ�����1�������½����µ�����20061008
			vCANDataStore( );	//�����ݷŵ���Ӧ��buffers
		}

		//�鿴����0�Ƿ���յ�����
		if ( ECanaRegs.CANRMP.bit.RMP0== 1 )
		{
			vMailBoxFlag = 0;
			vCANDataRemove( vMailBoxFlag );	//�������Ƶ���ʱbuffer
			//ECanaRegs.CANRMP.bit.RMP0= 1;	//�ͷ�����0�������½����µ�����
			ECanaRegs.CANRMP.all = 0x00000001;	//�ͷ�����0�������½����µ�����20061008
			vCANDataStore( );	//�����ݷŵ���Ӧ��buffers
		}
	}

	//��������־
	ECanaRegs.CANRML.all |=0x0000003F;
	
}
/*************************************************************
�ļ����ƣ�	vCanDataRemove( UINT16 i )
����������	���յ������ݷŵ���ʱbuffer
�㷨˵����	none

��ڲ�����	
				ECanaMboxes
		
���ڲ�����	iCANTempReg

			
�ֲ�������	i
�������ã�	none


*************************************************************/
void SysCanParallelHandle::vCANDataRemove ( UINT16 i )
{

	switch ( i )
	{
		case 5:
			//����������5�е�ID���������ݷŵ���ʱ�Ĵ���
			iCANTempReg.iMessageID =ECanaMboxes.MBOX5.MSGID.bit.STDMSGID;//���ID
			iCANTempReg.iTempBox0 = ECanaMboxes.MBOX5.MDL.word.LOW_WORD;//��ŵ�һ���ֵ�����
			iCANTempReg.iTempBox1 = ECanaMboxes.MBOX5.MDL.word.HI_WORD;//��ŵڶ����ֵ�����
			iCANTempReg.iTempBox2 = ECanaMboxes.MBOX5.MDH.word.LOW_WORD;//��ŵ������ֵ�����
			iCANTempReg.iTempBox3 = ECanaMboxes.MBOX5.MDH.word.HI_WORD;//��ŵ��ĸ��ֵ�����
		break;

		case 4:
			//����������4�е�ID���������ݷŵ���ʱ�Ĵ���
			iCANTempReg.iMessageID = ECanaMboxes.MBOX4.MSGID.bit.STDMSGID;//���ID
			iCANTempReg.iTempBox0 = ECanaMboxes.MBOX4.MDL.word.LOW_WORD;//��ŵ�һ���ֵ�����
			iCANTempReg.iTempBox1 = ECanaMboxes.MBOX4.MDL.word.HI_WORD;//��ŵڶ����ֵ�����
			iCANTempReg.iTempBox2 = ECanaMboxes.MBOX4.MDH.word.LOW_WORD;//��ŵ������ֵ�����
			iCANTempReg.iTempBox3 = ECanaMboxes.MBOX4.MDH.word.HI_WORD;//��ŵ��ĸ��ֵ�����
		break;
		
		case 3:
			//����������3�е�ID���������ݷŵ���ʱ�Ĵ���
			iCANTempReg.iMessageID = ECanaMboxes.MBOX3.MSGID.bit.STDMSGID;//���ID
			iCANTempReg.iTempBox0 = ECanaMboxes.MBOX3.MDL.word.LOW_WORD;//��ŵ�һ���ֵ�����
			iCANTempReg.iTempBox1 = ECanaMboxes.MBOX3.MDL.word.HI_WORD;//��ŵڶ����ֵ�����
			iCANTempReg.iTempBox2 = ECanaMboxes.MBOX3.MDH.word.LOW_WORD;//��ŵ������ֵ�����
			iCANTempReg.iTempBox3 = ECanaMboxes.MBOX3.MDH.word.HI_WORD;//��ŵ��ĸ��ֵ�����
		break;
			
		case 2:
			//����������2�е�ID���������ݷŵ���ʱ�Ĵ���
			iCANTempReg.iMessageID = ECanaMboxes.MBOX2.MSGID.bit.STDMSGID;//���ID
			iCANTempReg.iTempBox0 = ECanaMboxes.MBOX2.MDL.word.LOW_WORD;//��ŵ�һ���ֵ�����
			iCANTempReg.iTempBox1 = ECanaMboxes.MBOX2.MDL.word.HI_WORD;//��ŵڶ����ֵ�����
			iCANTempReg.iTempBox2 = ECanaMboxes.MBOX2.MDH.word.LOW_WORD;//��ŵ������ֵ�����
			iCANTempReg.iTempBox3 = ECanaMboxes.MBOX2.MDH.word.HI_WORD;//��ŵ��ĸ��ֵ�����
		break;
			
		case 1:
			//����������1�е�ID���������ݷŵ���ʱ�Ĵ���
			iCANTempReg.iMessageID = ECanaMboxes.MBOX1.MSGID.bit.STDMSGID;//���ID
			iCANTempReg.iTempBox0 = ECanaMboxes.MBOX1.MDL.word.LOW_WORD;//��ŵ�һ���ֵ�����
			iCANTempReg.iTempBox1 = ECanaMboxes.MBOX1.MDL.word.HI_WORD;//��ŵڶ����ֵ�����
			iCANTempReg.iTempBox2 = ECanaMboxes.MBOX1.MDH.word.LOW_WORD;//��ŵ������ֵ�����
			iCANTempReg.iTempBox3 = ECanaMboxes.MBOX1.MDH.word.HI_WORD;//��ŵ��ĸ��ֵ�����
		break;
		
		case 0:
			//����������0�е�ID���������ݷŵ���ʱ�Ĵ���
			iCANTempReg.iMessageID = ECanaMboxes.MBOX0.MSGID.bit.STDMSGID;//���ID
			iCANTempReg.iTempBox0 = ECanaMboxes.MBOX0.MDL.word.LOW_WORD;//��ŵ�һ���ֵ�����
			iCANTempReg.iTempBox1 = ECanaMboxes.MBOX0.MDL.word.HI_WORD;//��ŵڶ����ֵ�����
			iCANTempReg.iTempBox2 = ECanaMboxes.MBOX0.MDH.word.LOW_WORD;//��ŵ������ֵ�����
			iCANTempReg.iTempBox3 = ECanaMboxes.MBOX0.MDH.word.HI_WORD;//��ŵ��ĸ��ֵ�����
		break;
			
		default:
		break;
	}
}
/*************************************************************
�ļ����ƣ�	vCanDataStore( )
����������	����ʱbuffer�е������������жϲ��ŵ���ӦUPS���ݴ�����Ԫ��
�㷨˵����	none

��ڲ�����	iCANTempReg
				MessageID_Low
				MessageID_High
				MessageID_Version
				VersionNumber
				


���ڲ�����	uMACIDFlag
			iParaRunFlag
			sParaData
	
			
�ֲ�������	iMsssagePRIID
			iMACID
			iMessageOldVersion
�������ã�	none

*************************************************************/
void SysCanParallelHandle::vCANDataStore( )
{
	UINT16 iMessagePRIID;
	UINT16 iMACID;
	UINT16 iMessageOldVersion;

	//��ȡID���е���Ϣ���ֵ
	iMessagePRIID = (iCANTempReg.iMessageID & 0x03C0)>>6;

	//��ȡID���е�UPS IDֵ
	iMACID = (iCANTempReg.iMessageID & 0x003F);
	//��ȡ����IDֵ
	iMessageOldVersion = (iCANTempReg.iMessageID & 0x03FF);


	if ( 	iMACID <= 6 )
	{
	
		//�����ȼ���Ϣ֡����
		if (iMessagePRIID == MessageID_Low)
		{
			sCanFlag.bReceived = 1;

			//�ж�UPS ID
			if (iMACID ==0)
			{
				uMACIDFlag.sMACIDFlagBits.bMACID0 = 1;  //�ж���·�Ƿ�����
			}
						
			if (iMACID ==1)
			{
				uMACIDFlag.sMACIDFlagBits.bMACID1 = 1;	//UPS���߱�־
				iParaRunFlag |= 0x0001; //����Ӧ��UPS����λ��1
			}
		
			if (iMACID ==2)
			{
				uMACIDFlag.sMACIDFlagBits.bMACID2 = 1;	//UPS���߱�־
				iParaRunFlag |= 0x0002;	//����Ӧ��UPS����λ��1
			}

			if (iMACID ==3)
			{
				uMACIDFlag.sMACIDFlagBits.bMACID3 = 1;	//UPS���߱�־
				iParaRunFlag |= 0x0004;	//����Ӧ��UPS����λ��1
			}

			if (iMACID ==4)
			{
				uMACIDFlag.sMACIDFlagBits.bMACID4 = 1;	//UPS���߱�־
				iParaRunFlag |= 0x0008;	//����Ӧ��UPS����λ��1
			}

			if (iMACID ==5)
			{
				uMACIDFlag.sMACIDFlagBits.bMACID5 = 1;	//UPS���߱�־
				iParaRunFlag |= 0x0010;	//����Ӧ��UPS����λ��1
			}

			if (iMACID ==6)
			{
				uMACIDFlag.sMACIDFlagBits.bMACID6 = 1;	//UPS���߱�־
				iParaRunFlag |= 0x0020;	//����Ӧ��UPS����λ��1
			}

			//���ݴ洢
			//�Ƿ�Ϊ�����ȼ����ݵ�һ֡
			if ((iCANTempReg.iTempBox0 & 0x00FF)==1)
			{
				sParaData[iMACID].m_i16Pouta = iCANTempReg.iTempBox1;
				sParaData[iMACID].m_i16Poutb = iCANTempReg.iTempBox2;
				sParaData[iMACID].m_i16Poutc = iCANTempReg.iTempBox3;
			}

			//�Ƿ�Ϊ�����ȼ����ݵ���֡
			if ((iCANTempReg.iTempBox0 & 0x00FF)==2)
			{
				sParaData[iMACID].m_i16Souta = iCANTempReg.iTempBox1;
				sParaData[iMACID].m_i16Soutb = iCANTempReg.iTempBox2;
				sParaData[iMACID].m_i16Soutc = iCANTempReg.iTempBox3;
			}				 

			//�Ƿ�Ϊ�����ȼ����ݵڶ�֡
			if ((iCANTempReg.iTempBox0 & 0x00FF)==3)
			{
				sParaData[iMACID].uSystemFlag = iCANTempReg.iTempBox1;
				sParaData[iMACID].m_i16ModelKVA = iCANTempReg.iTempBox2;
				sParaData[iMACID].m_i16wGrantKVA = iCANTempReg.iTempBox3;
			}
		}
		
		//�����ȼ���Ϣ֡����
		if (iMessagePRIID == MessageID_High)
		{
			sCanFlag.bReceived = 1;
			sParaData[iMACID].uSystemFlag = iCANTempReg.iTempBox1;
		}			

		//�汾��Ϣ֡����
		if ( iMessagePRIID == MessageID_Version )
		{
		
			if  ( iCANTempReg.iTempBox0 != VersionNumber ) // �ж��Ƿ���ڱ����汾��
			{
				 sCanFlag.bVersionErr = 1;
			}
					
		}
	}

		//�Ƿ�Ϊ�ϰ汾Э���е���Ϣ֡
	if ( iMessageOldVersion == 0x41)
	{
		sCanFlag.bVersionErr = 1;
	}
}

/* ***********************************************************
�ļ����ƣ�	vCanCalculation( )
����������	���㲢��ϵͳ����,��������ϵͳ�Ĺ��ʼ���͹���״̬��־�ж�
�㷨˵����	none

��ڲ�����	ParaNumCNST
			sParaData
			m_i16ModelKVA
			m_i16wSerialNumSet_0
			iParaWarnFlag
			objMonInterface.m_st_wUPSModeRun
			iNumUPS
			m_i16BasicNum
			m_i16RedunNum
���ڲ�����	
			iNumBp
			iNumInv
			iNumOn
			iNumUPS
			iNumBattWarn
			iNumBattInv
			iParaWarnFlag
			iCapacitySumBasic
			iCapacitySum_InvOn
			iCapacitySum_Invmod
			iMaxCapacityInv
			iMaxCapacityOn
			InvMasterNum_20
			iPoutTotala
			iPoutTotalb
			iPoutTotalc
			iSoutTotala
			iSoutTotalb
			iSoutTotalc
			iPoutTotalMax
			iSoutTotalMax
			iPSoutTotalMax
			sCanFlag

�ֲ�������	i
			j
			temp
			INT32temp
			uCanDataBuff1
�������ã�	none
*********************************************************** */
void SysCanParallelHandle::vCANCalculation( )
{
	INT16		i,j,temp;				//��ʱ����
	INT32 	INT32temp;
	UINT16 		uCanDataBuff1;


	j = 0;
	for(i = 0;i < ParaNumCNST-1;i++)
	{
		iCapacityOn[i] = 0;
	}
//cb_H 030403 ������·����̨��
	iNumBp = 0;
//cb_E 030403
	iNumInv = 0;
	iNumOn = 0;
	iNumUPS = 0;
	iNumBattWarn = 0;
	iNumBattInv = 0;
	iParaWarnFlag = 0;
	iCapacitySumBasic = 0;
	iCapacitySum_InvOn = 0;
	iCapacitySum_Invmod = 0;

//YWF_H20051029��ͬ��������
	iMaxCapacityInv = 0;
	iMaxCapacityOn = 0;
	iInvMasterPriorINT16er = 0;
//YWF_E20051029

//wzh 20060802 ��ع���
	iSumOfInvONCapacity = 0;
	iSumOfBattInvCapacity = 0;
	iSumOfBattWarCapacity = 0;
//wzh end
	
//zqf 060615--������Ͳ�ͬ��������
	iKCapSum_Invmod = 0;		
		
    
//�ж���·����Ϣ
	sCanFlag.bAutoBp = (sParaData[0].uSystemFlag & SBIT7);//��������һλTBD	
	
	/* ���㲢��ϵͳ����ܿ���̨�����ܹ���̨�� */
	for(i=1;i<=ParaNumCNST-1;i++)
	{    
		if ( ( sParaData[i].uSystemFlag & SBIT0 ) != 0 )
		{
			iNumInv ++;					//��俪����̨��
		}
		if ( ( sParaData[i].uSystemFlag & SBIT1 ) != 0 )
		{
			iNumOn ++;					//��乩����̨��
		}
		if ( ( sParaData[i].uSystemFlag & SBIT2 ) != 0 )
		{
			iNumBattWarn ++;			//����ϵͳ��������澯��̨��
		}
		if ( ( sParaData[i].uSystemFlag & SBIT3 ) != 0 )
		{
			iNumBattInv ++;				//����ϵͳ�����乩����̨��
		}
		if ( ( sParaData[i].uSystemFlag & SBIT9 ) != 0 )
		{
			iNumUPS ++;					//����ϵͳʵ���ϵ���̨��
		}

//wzh 20060802 ���㴦����乩��״̬�µĸ�̨������GrandKVA֮��
		if((sParaData[i].uSystemFlag & SBIT1)!=0)
		{
			iSumOfInvONCapacity += sParaData[i].m_i16wGrantKVA;
		}
//wzh end

//wzh 20060802 ���㴦�ڵ�����״̬�µĸ�̨������GrandKVA֮��;
		if((sParaData[i].uSystemFlag & SBIT3)!=0)
		{
			iSumOfBattInvCapacity += sParaData[i].m_i16wGrantKVA;
		}
//wzh end

//wzh 20060802 ���㴦�ڵ��Ԥ�澯״̬�µĸ�̨������GrandKVA֮��;
		if((sParaData[i].uSystemFlag & SBIT2)!=0)
		{
			iSumOfBattWarCapacity += sParaData[i].m_i16wGrantKVA;
		}
//wzh end

//wzh20060802 

//cb_H 030403 ������·�ϵ�̨��
		if ( sParaData[i].uSystemFlag & SBIT8 )
		{
			iNumBp ++;					//����ϵͳ��·������̨��
		}
//cb_E 030403				

		if ( ( sParaData[i].uSystemFlag & SBIT0 ) != 0 )
		{
			iCapacitySum_InvOn = iCapacitySum_InvOn + sParaData[i].m_i16wGrantKVA;

//YWF_H20051029��ͬ��������
			if (iMaxCapacityInv < sParaData[i].m_i16wGrantKVA)
			{
				iMaxCapacityInv = sParaData[i].m_i16wGrantKVA;	
			}
//YWF_E20051029
		}

		if ( ( sParaData[i].uSystemFlag & SBIT1 ) != 0 )
		{
			iCapacitySum_Invmod = iCapacitySum_Invmod + sParaData[i].m_i16wGrantKVA;

//YWF_H20051031��ͬ��������
			if (iMaxCapacityOn < sParaData[i].m_i16wGrantKVA)
			{
				iMaxCapacityOn = sParaData[i].m_i16wGrantKVA;	
			}
//YWF_E20051031
//zqf 060615--������͵Ĳ�ͬ��������
			iKCapSum_Invmod += (((INT32)sParaData[i].m_i16wGrantKVA)<<12)/(sParaData[i].m_i16ModelKVA);
//zqf 060615--������͵Ĳ�ͬ��������
		}

		if ( ( sParaData[i].uSystemFlag & SBIT9 ) != 0 ) //����ϵͳʵ���ϵ�
		{
			if ((sParaData[i].m_i16wGrantKVA > objMonInterface.m_i16GrantKVA)
				||((sParaData[i].m_i16wGrantKVA == objMonInterface.m_i16GrantKVA) && (i < objMonInterface.m_i16wSerialNumSet_0)))
			{
				iInvMasterPriorINT16er++;
			}

			iCapacityOn[j] = sParaData[i].m_i16wGrantKVA;
			j++;
			
		}
						
		/* �ϳɲ���ϵͳ�ĸ澯��־ */
		uCanDataBuff1 = ( sParaData[i].uSystemFlag >> 4 ) & 0xF;
		iParaWarnFlag = iParaWarnFlag | uCanDataBuff1;
	}

//zqf 060615--������͵Ĳ�ͬ��������
	if ((iKCapSum_Invmod !=0)&&(iNumOn>=2))
		iKCapAdj = (((INT32)objMonInterface.m_i16GrantKVA<<12) / objMonInterface.m_i16ModelKVA * iNumOn * 4096) / iKCapSum_Invmod;
	else
		iKCapAdj = 4096;
//zqf 060615--������͵Ĳ�ͬ��������

//YWF_H20051125��ͬ�������ಢ��			
//			objInvAlg.m_i16wInvMasterPrior_0 = iInvMasterPriorINT16er;  //

	for (i = 0;i < iNumUPS-1;i++)
	{
		for (j = i+1;j < iNumUPS;j++)
		{
			if (iCapacityOn[i] > iCapacityOn[j])
			{
				temp = iCapacityOn[i];
				iCapacityOn[i] = iCapacityOn[j];
				iCapacityOn[j] = temp;
			}
		}
	}

	for (j = 0;j < objMonInterface.m_i16BasicNum;j++)
	{
		iCapacitySumBasic = iCapacitySumBasic + iCapacityOn[j];
	}
//YWF_E20051125

//wzh 20060606 �����ܵ���·��ֵ��Χ��־

	objBypass.m_st_wBpAmpRangeFlag.bCANWide = 0;
	for (j = 0; j<iNumUPS;j++)
	{
		if( (sParaData[j+1].uSystemFlag & SBIT12 ) != 0)
		{
			objBypass.m_st_wBpAmpRangeFlag.bCANWide = 1;
			break;
		}
		
	}

//wzh end



	/* ���㲢��ϵͳ�������й����ʺ����ڹ��� */
	if ( objMonInterface.m_st_wUPSModeRun.bCabinet != 2 )	//����ϵͳ������·��
	{
		iPoutTotala = 0;
		iPoutTotalb = 0;
		iPoutTotalc = 0;
		iSoutTotala = 0;
		iSoutTotalb = 0;
		iSoutTotalc = 0;

		//���㲢��ϵͳ����ʵ�ʹ���
		for(i=1;i<=ParaNumCNST-1;i++)
		{
			INT32temp = ((INT32)sParaData[i].m_i16Pouta*sParaData[i].m_i16wGrantKVA*10)>>10;
			INT32temp = (INT32temp*K1div3Cnst)>>12;     // *1/3
			iPoutTotala = iPoutTotala + (INT16)(INT32temp); 
			
			INT32temp = ((INT32)sParaData[i].m_i16Poutb*sParaData[i].m_i16wGrantKVA*10)>>10;
			INT32temp = (INT32temp*K1div3Cnst)>>12;	// *1/3
			iPoutTotalb = iPoutTotalb + (INT16)(INT32temp);
			
			INT32temp = ((INT32)sParaData[i].m_i16Poutc*sParaData[i].m_i16wGrantKVA*10)>>10;
			INT32temp = (INT32temp*K1div3Cnst)>>12;	// *1/3
			iPoutTotalc = iPoutTotalc + (INT16)(INT32temp);
			
			INT32temp = ((INT32)sParaData[i].m_i16Souta*sParaData[i].m_i16wGrantKVA*10)>>10;
			INT32temp = (INT32temp*K1div3Cnst)>>12;	// *1/3
			iSoutTotala = iSoutTotala + (INT16)(INT32temp); 
			
			INT32temp = ((INT32)sParaData[i].m_i16Soutb*sParaData[i].m_i16wGrantKVA*10)>>10;
			INT32temp = (INT32temp*K1div3Cnst)>>12;	// *1/3
			iSoutTotalb = iSoutTotalb + (INT16)(INT32temp);
			
			INT32temp = ((INT32)sParaData[i].m_i16Soutc*sParaData[i].m_i16wGrantKVA*10)>>10;
			INT32temp = (INT32temp*K1div3Cnst)>>12;	// *1/3
			iSoutTotalc = iSoutTotalc + (INT16)(INT32temp);
	
		}

		//���㲢��ϵͳ���ฺ���е����ֵ
		iPoutTotalMax = MAX2(iPoutTotala,iPoutTotalb);
		iPoutTotalMax = MAX2(iPoutTotalMax,iPoutTotalc);

		iSoutTotalMax = MAX2(iSoutTotala,iSoutTotalb);
		iSoutTotalMax = MAX2(iSoutTotalMax,iSoutTotalc);

		iPSoutTotalMax = MAX2(iSoutTotalMax,iPoutTotalMax);

		//����ϵͳ���ر���ֵ
		INT32temp = (INT32)iPoutTotala*objSystem.m_i161DivGrantKVA;			//6.10
		iPoutTotala = (INT16)((INT32temp*K1div10Cnst)>>12);			
	
		INT32temp = (INT32)iPoutTotalb*objSystem.m_i161DivGrantKVA;
		iPoutTotalb = (INT16)((INT32temp*K1div10Cnst)>>12);

		INT32temp = (INT32)iPoutTotalc*objSystem.m_i161DivGrantKVA;
		iPoutTotalc = (INT16)((INT32temp*K1div10Cnst)>>12);
	
		INT32temp = (INT32)iSoutTotala*objSystem.m_i161DivGrantKVA;
		iSoutTotala = (INT16)((INT32temp*K1div10Cnst)>>12);
	
		INT32temp = (INT32)iSoutTotalb*objSystem.m_i161DivGrantKVA;
		iSoutTotalb = (INT16)((INT32temp*K1div10Cnst)>>12);

		INT32temp = (INT32)iSoutTotalc*objSystem.m_i161DivGrantKVA;
		iSoutTotalc = (INT16)((INT32temp*K1div10Cnst)>>12);

	}       
}

/*************************************************************
�ļ����ƣ�	vCANLocalData( )
����������	�����������ݷŵ���ӦUPS���ݴ�����Ԫ��
�㷨˵����	none

��ڲ�����	
			objSwitches.m_st_FlagBreaker
			m_i16Pouta
			m_i16Poutb
			m_i16Poutc
			m_i16Souta
			m_i16Soutb
			m_i16Soutc
			m_i16ModelKVA
			sParaData
			
���ڲ�����	
			sParaData
			uInvRealInfo
			iLocalStatusChanged
			
�ֲ�������	
			uLocalStatus
			uCanDataBuff1
�������ã�	none
*************************************************************/		
void SysCanParallelHandle::vCANLocalData( )
{
	UINT16		uLocalStatus;
	UINT16 		uCanDataBuff1;

	if ( objSwitches.m_st_FlagBreaker.bQ5 == 1 )
	{
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Pouta = objOutput.m_i16Pouta;
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Poutb = objOutput.m_i16Poutb;
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Poutc = objOutput.m_i16Poutc;
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Souta = objOutput.m_i16Souta;
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Soutb = objOutput.m_i16Soutb;
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Soutc = objOutput.m_i16Soutc;
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16ModelKVA = objMonInterface.m_i16ModelKVA;
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16wGrantKVA = objMonInterface.m_i16GrantKVA;			
	}
	else
	{
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Pouta = 0x0000;
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Poutb = 0x0000;
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Poutc = 0x0000;
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Souta = 0x0000;
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Soutb = 0x0000;
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Soutc = 0x0000;
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16ModelKVA = objMonInterface.m_i16ModelKVA;
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16wGrantKVA = objMonInterface.m_i16GrantKVA;
	}
	
	uLocalStatus = sParaData[objMonInterface.m_i16wSerialNumSet_0].uSystemFlag;
	sParaData[objMonInterface.m_i16wSerialNumSet_0].uSystemFlag = 0;
	if ( objSwitches.m_st_FlagBreaker.bInv == 1 )			//��俪��
	{
		sParaData[objMonInterface.m_i16wSerialNumSet_0].uSystemFlag |= SBIT0;
	}
				
	if ( objSwitches.m_st_FlagBreaker.bOn == 1 )			//��乩��
	{
		sParaData[objMonInterface.m_i16wSerialNumSet_0].uSystemFlag |= SBIT1;
	}
					
	//cb_H 030403  ������·�����ж�
	if ( objSwitches.m_st_FlagBreaker.bBpOn == 1 )		//��·����
	{
		sParaData[objMonInterface.m_i16wSerialNumSet_0].uSystemFlag |= SBIT8;
	}
	//cb_E 030403

	//wzh 20060606 ������·��ֵ��Χ��־
	if (objBypass.m_st_wBpAmpRangeFlag.bLocalWide == 1)
	{
		sParaData[objMonInterface.m_i16wSerialNumSet_0].uSystemFlag |= SBIT12;	//�������߱�־	
	}
	// wzh end

	/* �ϳɱ����澯��Ϣ */
	uCanDataBuff1 = ( S2U(objMonitor.uInvRealInfo) << 2 ) & 0xFC;
	sParaData[objMonInterface.m_i16wSerialNumSet_0].uSystemFlag |= uCanDataBuff1;

	sParaData[objMonInterface.m_i16wSerialNumSet_0].uSystemFlag |= SBIT9;	//�������߱�־

	uCanDataBuff1 = (UINT16) objSystem.m_st_wFlagStatus.bVbp;
	sParaData[objMonInterface.m_i16wSerialNumSet_0].uSystemFlag |= ( uCanDataBuff1 << 10 );
	
	if (sParaData[objMonInterface.m_i16wSerialNumSet_0].uSystemFlag != uLocalStatus)
	{
		iLocalStatusChanged = 1;
	}
	//cb_H 030412 Э���������屾��ʵʱ��Ϣ�Ĺ�����Ϣ
	objMonitor.uInvRealInfo.bSevereFail = 0;
	objMonitor.uInvRealInfo.bNormalFail = 0;
	objMonitor.uInvRealInfo.bSevereWarn = 0;
	objMonitor.uInvRealInfo.bNormalWarn = 0;				

}


/*************************************************************
�ļ����ƣ�	vCANLow_Tx( )
����������	�������ĵ����ȼ������������Ϲ㲥
�㷨˵����	none

��ڲ�����	
			ECanaRegs
			EcanaMboxes
			MessageID_Low
			m_i16wSerialNumSet_0
			iFrameIDLow
			sParaData
���ڲ���:	
			ECanaRegs
			ECanaMboxes
�ֲ�����:	temp0
			tempECanReg
			tempECanMboxReg
��������:	none

*************************************************************/
void SysCanParallelHandle::vCANLow_Tx( )
{
	INT16	temp0;
	struct	ECAN_REGS	tempECanReg;
	struct  	ECAN_MBOXES	tempECanMboxReg;

	//������ͳɹ���־λ
	tempECanReg.CANTA.all = ECanaRegs.CANTA.all;
	tempECanReg.CANTA.bit.TA10 = 1;
	ECanaRegs.CANTA.all = tempECanReg.CANTA.all;

	//������ͱ����Ա�־λ
	tempECanReg.CANAA.all = ECanaRegs.CANAA.all;
	tempECanReg.CANAA.bit.AA10 = 1;
	ECanaRegs.CANAA.all = tempECanReg.CANAA.all;

	//��ֹ����10����
	tempECanReg.CANME.all = ECanaRegs.CANME.all;
	tempECanReg.CANME.bit.ME10 = 0;
	ECanaRegs.CANME.all = tempECanReg.CANME.all;
	
	//����10 ID ����
	tempECanMboxReg.MBOX10.MSGID.all =  ECanaMboxes.MBOX10.MSGID.all;
	tempECanMboxReg.MBOX10.MSGID.bit.IDE = 0; //stardart identifier
	tempECanMboxReg.MBOX10.MSGID.bit.AME =0;//dont care
	tempECanMboxReg.MBOX10.MSGID.bit.AAM = 0;//normal transmit mode
	tempECanMboxReg.MBOX10.MSGID.bit.STDMSGID = (MessageID_Low<<6);//д��Ϣ�����ID
	temp0 = (objMonInterface.m_i16wSerialNumSet_0 & 0x07);
	tempECanMboxReg.MBOX10.MSGID.bit.STDMSGID |= temp0;	//дUPS ID
	tempECanMboxReg.MBOX10.MSGID.bit.EXTMSGID_H = 0;
	tempECanMboxReg.MBOX10.MSGID.bit.EXTMSGID_L = 0;
	ECanaMboxes.MBOX10.MSGID.all = tempECanMboxReg.MBOX10.MSGID.all;
	
	//ECanaMboxes.MBOX10.MSGCTRL.all = ECanaMboxes.MBOX10.MSGCTRL.all;
	ECanaMboxes.MBOX10.MSGCTRL.all = 0x00000008;//д���ݳ���
	
	if (iFrameIDLow == 1)
	{
		//д����
		ECanaMboxes.MBOX10.MDL.word.LOW_WORD= iFrameIDLow;  
		ECanaMboxes.MBOX10.MDL.word.HI_WORD = sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Pouta;
		ECanaMboxes.MBOX10.MDH.word.LOW_WORD = sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Poutb;
		ECanaMboxes.MBOX10.MDH.word.HI_WORD = sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Poutc;
		//CAN_MDER = 0x00EF;

		//����������
		tempECanReg.CANME.all = ECanaRegs.CANME.all;
		tempECanReg.CANME.bit.ME10 = 1;
		ECanaRegs.CANME.all = tempECanReg.CANME.all;
		
		//��������
		tempECanReg.CANTRS.all = ECanaRegs.CANTRS.all;
		tempECanReg.CANTRS.bit.TRS10 = 1;
		ECanaRegs.CANTRS.all = tempECanReg.CANTRS.all;

		iFrameIDLow++;
		return;
	}
	
	if (iFrameIDLow == 2)
	{
		//д����
		ECanaMboxes.MBOX10.MDL.word.LOW_WORD =iFrameIDLow;
		ECanaMboxes.MBOX10.MDL.word.HI_WORD = sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Souta;
		ECanaMboxes.MBOX10.MDH.word.LOW_WORD = sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Soutb;
		ECanaMboxes.MBOX10.MDH.word.HI_WORD = sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Soutc;
		//CAN_MDER = 0x00EF;
		
		//����������
		tempECanReg.CANME.all = ECanaRegs.CANME.all;
		tempECanReg.CANME.bit.ME10 = 1;
		ECanaRegs.CANME.all = tempECanReg.CANME.all;
		
		//��������
		tempECanReg.CANTRS.all = ECanaRegs.CANTRS.all;
		tempECanReg.CANTRS.bit.TRS10 = 1;
		ECanaRegs.CANTRS.all = tempECanReg.CANTRS.all;
		
		iFrameIDLow++;
		return;
	}
																																																																																																																																																																																																						
	if (iFrameIDLow == 3)
	{
		//д����
		ECanaMboxes.MBOX10.MDL.word.LOW_WORD =iFrameIDLow;
		ECanaMboxes.MBOX10.MDL.word.HI_WORD = sParaData[objMonInterface.m_i16wSerialNumSet_0].uSystemFlag; 
		ECanaMboxes.MBOX10.MDH.word.LOW_WORD = sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16ModelKVA;
		ECanaMboxes.MBOX10.MDH.word.HI_WORD = sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16wGrantKVA;
		//CAN_MDER = 0x00EF;
		
		//����������
		tempECanReg.CANME.all = ECanaRegs.CANME.all;
		tempECanReg.CANME.bit.ME10 = 1;
		ECanaRegs.CANME.all = tempECanReg.CANME.all;
		
		//��������
		tempECanReg.CANTRS.all = ECanaRegs.CANTRS.all;
		tempECanReg.CANTRS.bit.TRS10 = 1;
		ECanaRegs.CANTRS.all = tempECanReg.CANTRS.all;
		
		iFrameIDLow = 0;
	}
		
}
/*************************************************************
�ļ����ƣ�	vCANVersion_Tx( )
����������	��������CAN Э��汾���������Ϲ㲥
�㷨˵����	none

��ڲ�����
			ECanaRegs
			ECanaMboxes
			MessageID_Version
			m_i16wSerialNumSet_0
			VersionNumber
���ڲ�����	
			ECanaRegs
			ECanaMboxes
�ֲ�������	temp0
			tempECanReg
			tempECanMboxReg
�������ã�	none

*************************************************************/				
void SysCanParallelHandle::vCANVersion_Tx( )
{
	INT16 temp0;
	struct	ECAN_REGS	tempECanReg;
	struct  	ECAN_MBOXES	tempECanMboxReg;

	//������ͳɹ���־λ
	tempECanReg.CANTA.all = ECanaRegs.CANTA.all;
	tempECanReg.CANTA.bit.TA10 = 1;
	ECanaRegs.CANTA.all = tempECanReg.CANTA.all;

	//������ͱ����Ա�־λ
	tempECanReg.CANAA.all = ECanaRegs.CANAA.all;
	tempECanReg.CANAA.bit.AA10 = 1;
	ECanaRegs.CANAA.all = tempECanReg.CANAA.all;

	//��ֹ����10����
	tempECanReg.CANME.all = ECanaRegs.CANME.all;
	tempECanReg.CANME.bit.ME10 = 0;
	ECanaRegs.CANME.all = tempECanReg.CANME.all;

	//����10 ID ����
	tempECanMboxReg.MBOX10.MSGID.all =  ECanaMboxes.MBOX10.MSGID.all;
	tempECanMboxReg.MBOX10.MSGID.bit.IDE = 0; //stardart identifier
	tempECanMboxReg.MBOX10.MSGID.bit.AME =0;//dont care
	tempECanMboxReg.MBOX10.MSGID.bit.AAM = 0;//normal transmit mode
	tempECanMboxReg.MBOX10.MSGID.bit.STDMSGID = (MessageID_Version<<6);//д��Ϣ�����ID
	temp0 = (objMonInterface.m_i16wSerialNumSet_0 & 0x07);
	tempECanMboxReg.MBOX10.MSGID.bit.STDMSGID |= temp0;	//дUPS ID
	tempECanMboxReg.MBOX10.MSGID.bit.EXTMSGID_H = 0;
	tempECanMboxReg.MBOX10.MSGID.bit.EXTMSGID_L = 0;
	ECanaMboxes.MBOX10.MSGID.all = tempECanMboxReg.MBOX10.MSGID.all;
	
	ECanaMboxes.MBOX10.MSGCTRL.all = 0x00000002;//д���ݳ���

	//д����	
	ECanaMboxes.MBOX10.MDL.word.LOW_WORD = VersionNumber;
	//CAN_MBX5B = sParaData[m_i16wSerialNumSet_0].uSystemFlag; 
	
	//����������
	tempECanReg.CANME.all = ECanaRegs.CANME.all;
	tempECanReg.CANME.bit.ME10 = 1;
	ECanaRegs.CANME.all = tempECanReg.CANME.all;
		
	//��������
	tempECanReg.CANTRS.all = ECanaRegs.CANTRS.all;
	tempECanReg.CANTRS.bit.TRS10 = 1;
	ECanaRegs.CANTRS.all = tempECanReg.CANTRS.all;
}																																																																																																																																																																																																
/*************************************************************
�ļ����ƣ�	vCANHigh_Tx( )
����������	�������ĸ����ȼ������������Ϲ㲥
�㷨˵����	none

��ڲ�����
			ECanaRegs
			ECanaMboxes
			MessageID_High
			m_i16wSerialNumSet_0
			iFrameIDHigh
			
���ڲ�����	
			ECanaRegs
			ECanaMboxes
�ֲ�������	temp0
			tempECanReg
			tempECanMboxReg
�������ã�	none

*************************************************************/																																																																																																																																										
void SysCanParallelHandle::vCANHigh_Tx( )
{

	INT16 temp0;
	struct	ECAN_REGS	tempECanReg;
	struct  	ECAN_MBOXES	tempECanMboxReg;

	iFrameIDHigh = 1;

	//������ͳɹ���־λ
	tempECanReg.CANTA.all = ECanaRegs.CANTA.all;
	tempECanReg.CANTA.bit.TA10 = 1;
	ECanaRegs.CANTA.all = tempECanReg.CANTA.all;

	//������ͱ����Ա�־λ
	tempECanReg.CANAA.all = ECanaRegs.CANAA.all;
	tempECanReg.CANAA.bit.AA10 = 1;
	ECanaRegs.CANAA.all = tempECanReg.CANAA.all;

	//��ֹ����10����
	tempECanReg.CANME.all = ECanaRegs.CANME.all;
	tempECanReg.CANME.bit.ME10 = 0;
	ECanaRegs.CANME.all = tempECanReg.CANME.all;

	//����10 ID ����
	tempECanMboxReg.MBOX10.MSGID.all =  ECanaMboxes.MBOX10.MSGID.all;
	tempECanMboxReg.MBOX10.MSGID.bit.IDE = 0; //stardart identifier
	tempECanMboxReg.MBOX10.MSGID.bit.AME =0;//dont care
	tempECanMboxReg.MBOX10.MSGID.bit.AAM = 0;//normal transmit mode
	tempECanMboxReg.MBOX10.MSGID.bit.STDMSGID = (MessageID_High<<6);//д��Ϣ�����ID
	temp0 = (objMonInterface.m_i16wSerialNumSet_0 & 0x07);
	tempECanMboxReg.MBOX10.MSGID.bit.STDMSGID |= temp0;	//дUPS ID
	tempECanMboxReg.MBOX10.MSGID.bit.EXTMSGID_H = 0;
	tempECanMboxReg.MBOX10.MSGID.bit.EXTMSGID_L = 0;
	ECanaMboxes.MBOX10.MSGID.all = tempECanMboxReg.MBOX10.MSGID.all;
	
	ECanaMboxes.MBOX10.MSGCTRL.all = 0x00000004;//д���ݳ���

	//д����
	ECanaMboxes.MBOX10.MDL.word.LOW_WORD = iFrameIDHigh;
	ECanaMboxes.MBOX10.MDL.word.HI_WORD = sParaData[objMonInterface.m_i16wSerialNumSet_0].uSystemFlag; 
	
	//����������
	tempECanReg.CANME.all = ECanaRegs.CANME.all;
	tempECanReg.CANME.bit.ME10 = 1;
	ECanaRegs.CANME.all = tempECanReg.CANME.all;
		
	//��������
	tempECanReg.CANTRS.all = ECanaRegs.CANTRS.all;
	tempECanReg.CANTRS.bit.TRS10 = 1;
	ECanaRegs.CANTRS.all = tempECanReg.CANTRS.all;
	
}

/*************************************************************
�ļ����ƣ�	vCAN_Com_Err( )
����������	����ϵͳͨѶ�����ж�
�㷨˵����	none

��ڲ�����	
			uMACIDFlag
			m_i16wSerialNumSet_0
			sCanFlag
			m_i16BasicNum
			m_i16RedunNum
			objMonInterface.m_st_wUPSModeRun
			
���ڲ���:
			sCanFlag
			iSpanCanErr
			sParaData
			uMACIDFlag
�ֲ�������	i
				iUPSOnlineNum
�������ã�	none

*************************************************************/																																																																																																																																										
void SysCanParallelHandle::vCAN_Com_Err( )
{
	UINT16 iUPSOnlineNum; 
	UINT16 i;

	//�Ƿ����������뱾����ͬ��ID��
	if ( uMACIDFlag.iMACIDFlagWord & (1<<(objMonInterface.m_i16wSerialNumSet_0)))
	{
		iUPSOnlineNum = 0;
	}
	else
	{
		iUPSOnlineNum = 1;
	}
	
	//���������ϵ�����̨��
	if (uMACIDFlag.sMACIDFlagBits.bMACID1 == 1)
	{
		iUPSOnlineNum++;
	}
	
	if (uMACIDFlag.sMACIDFlagBits.bMACID2 == 1)
	{
		iUPSOnlineNum++;
	}
	
	if (uMACIDFlag.sMACIDFlagBits.bMACID3 == 1)
	{
		iUPSOnlineNum++;
	}
	
	if (uMACIDFlag.sMACIDFlagBits.bMACID4 == 1)
	{
		iUPSOnlineNum++;
	}
	
	if (uMACIDFlag.sMACIDFlagBits.bMACID5 == 1)
	{
		iUPSOnlineNum++;
	}
	
	if (uMACIDFlag.sMACIDFlagBits.bMACID6 == 1)
	{
		iUPSOnlineNum++;
	}

	//�ж��Ƿ񵥻�����
	if (iUPSOnlineNum == 1 )
	{
		sCanFlag.bUPSSingleOline = 1;
	}
	else
	{
		sCanFlag.bUPSSingleOline = 0;
	}

	//�����µ�UPS����,�汾��Ϣ֡���ͱ�־��1
	if (iUPSOnlineNum >( objMonInterface.m_i16BasicNum + objMonInterface.m_i16RedunNum ) )
	{
		sCanFlag.bVersion = 1;
	}

	
	if (( iUPSOnlineNum != ( objMonInterface.m_i16BasicNum + objMonInterface.m_i16RedunNum ) )
		||( ( objMonInterface.m_st_wUPSModeRun.bCabinet == 2 ) && ( sCanFlag.bAutoBp == 0 ) ))
		//ʵ��̨������������̨���������ó��Զ���·����·��δ����
	{
		iSpanCanErr ++;
		if (iSpanCanErr >=5)
		{
			iSpanCanErr = 5;
			sCanFlag.bErr = 1;
			for (i=1; i<=ParaNumCNST-1; i++)
			{
				//��������Ҫ���20061008
				if(i == objMonInterface.m_i16wSerialNumSet_0)
				{
					sMACIDErrCounter[i] = 0;//�Ա�����������0
					continue;
				}
				
				//�Ե����UPS��������20061008
				if (uMACIDFlag.iMACIDFlagWord & ( 1 <<i ))
				{
					sMACIDErrCounter[i] = 0;
				}
				else if ((++sMACIDErrCounter[i]) >= 5 )
				{
					sParaData[i].m_i16Pouta = 0;
					sParaData[i].m_i16Poutb = 0;
					sParaData[i].m_i16Poutc = 0;
					sParaData[i].m_i16Souta = 0;
					sParaData[i].m_i16Soutb = 0;
					sParaData[i].m_i16Soutc = 0;
					//sParaData[i].m_i16ModelKVA = 0;
					//sParaData[i].m_i16wGrantKVA = 0;
					sParaData[i].uSystemFlag = 0;
					sMACIDErrCounter[i]=5;
				}
			
				/*if ( (  uMACIDFlag.iMACIDFlagWord & ( 1 <<i ) ) !=( ( uMACIDFlag.iMACIDFlagWord & ( 1 <<(i+6) ) ) >>6)) 
				{
					sParaData[i].m_i16Pouta = 0;
					sParaData[i].m_i16Poutb = 0;
					sParaData[i].m_i16Poutc = 0;
					sParaData[i].m_i16Souta = 0;
					sParaData[i].m_i16Soutb = 0;
					sParaData[i].m_i16Soutc = 0;
					//sParaData[i].m_i16ModelKVA = 0;
					//sParaData[i].m_i16wGrantKVA = 0;
					sParaData[i].uSystemFlag = 0;
				}
				*/
			}	
			
		}
	}
	else
	{
		iSpanCanErr = 0;
		sCanFlag.bErr = 0;

		//��������������
		for (i=1; i<=ParaNumCNST-1; i++)
		{
			sMACIDErrCounter[i] = 0;//20061008
		}	

		//���ݸ�UPS���߱�־
		uMACIDFlag.sMACIDFlagBits.bMACID1Backup  = uMACIDFlag.sMACIDFlagBits.bMACID1; 
		uMACIDFlag.sMACIDFlagBits.bMACID2Backup  = uMACIDFlag.sMACIDFlagBits.bMACID2; 
		uMACIDFlag.sMACIDFlagBits.bMACID3Backup  = uMACIDFlag.sMACIDFlagBits.bMACID3; 
		uMACIDFlag.sMACIDFlagBits.bMACID4Backup  = uMACIDFlag.sMACIDFlagBits.bMACID4; 
		uMACIDFlag.sMACIDFlagBits.bMACID5Backup  = uMACIDFlag.sMACIDFlagBits.bMACID5; 
		uMACIDFlag.sMACIDFlagBits.bMACID6Backup  = uMACIDFlag.sMACIDFlagBits.bMACID6;
	}

	
	uMACIDFlag.iMACIDFlagWord  &= 0xFF80;	//��������ݱ�־λ
}	

	




