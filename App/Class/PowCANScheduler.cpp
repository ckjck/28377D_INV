/*==========================================================*/
/* Title		:	PowCANScheduler.cpp											*/
/* Description	: 	Class_PowCANScheduler realization 								*/
/* Date		:	Apr.2008														*/
/*==========================================================*/
#include "PowCANScheduler.h"


/*************************************************************
�������ƣ�  void App_PowCANInit()
����������  ��ʼ����ر���
�㷨˵����
��ڲ�����
���ڲ�����
�������ڣ�	070919
�޸����ڣ�	070919
*************************************************************/
void	Class_PowCANScheduler::App_PowCANInit(void)
{
	INT16	i;

	//power can �������ñ��ʼ��
		PrdSchdlrTbl[0].m_u16SetTickCnt = K166usCnst;    //ģ�������ӣ����յ�ģ���������࣬ÿ1���жϼ��һ�����䲢��������
		PrdSchdlrTbl[0].m_u16TickCntr = 0;
		PrdSchdlrTbl[0].pCbFunc = &Class_PowCANScheduler::App_Int_Can_Recv;

		PrdSchdlrTbl[1].m_u16SetTickCnt = K2msCnst;         //ģ����������12��Ϊ����CAN���߸����ʹ���������е�ģ��û�з��ͣ���Ϊ2ms����һ֡����
		PrdSchdlrTbl[1].m_u16TickCntr = 0;
		PrdSchdlrTbl[1].pCbFunc =&Class_PowCANScheduler::Dat_Int_Can_Send;

		PrdSchdlrTbl[2].m_u16SetTickCnt = (K1msCnst<<1);    //�ж�UPS���ߵ�ʱ��ά�ֲ���
		PrdSchdlrTbl[2].m_u16TickCntr = 0;
		PrdSchdlrTbl[2].pCbFunc = &Class_PowCANScheduler::App_Int_Can_ComErr;

		//Power CAN ���޹����ݽṹ��ʼ��
		for (i = 0; i < ParaNumCNST; i++)
		{
			PowCanParaInfoEntry.ParaDQCurrEntry[i].m_i16IINVAP = 0;
			PowCanParaInfoEntry.ParaDQCurrEntry[i].m_i16IINVAQ = 0;
			PowCanParaInfoEntry.ParaDQCurrEntry[i].m_i16IINVBP = 0;
			PowCanParaInfoEntry.ParaDQCurrEntry[i].m_i16IINVBQ = 0;
			PowCanParaInfoEntry.ParaDQCurrEntry[i].m_i16IINVCP= 0;
			PowCanParaInfoEntry.ParaDQCurrEntry[i].m_i16IINVCQ = 0;
			PowCanParaInfoEntry.UpsOnlineInfo.m_u16NotOnlineCnt[i] = 0;
		}

		PowCanParaInfoEntry.MsgRecvFalg.m_u16All = 0;
		PowCanParaInfoEntry.UpsOnlineInfo.uOnlineFlag.m_u16All = 0;

		//By wxw20090730 PowerCan��Load���ݱ��ʼ��
		for (i = 0; i < ParaNumCNST; i++)
		{
			PowerCANParaLoadEntry.ParaLoadData[i].m_i16Pouta= 0;
			PowerCANParaLoadEntry.ParaLoadData[i].m_i16Poutb= 0;
			PowerCANParaLoadEntry.ParaLoadData[i].m_i16Poutc= 0;
			PowerCANParaLoadEntry.ParaLoadData[i].m_i16Souta= 0;
			PowerCANParaLoadEntry.ParaLoadData[i].m_i16Soutb= 0;
			PowerCANParaLoadEntry.ParaLoadData[i].m_i16Soutc= 0;
		}

		i16SendScheduleCnt = 0;
		m_i16InvState = 0;
		m_i16BattState = 0;
		PowerCANParaLoadEntry.DqCurrentValid.m_u16All = 0;
}
/* ***********************************************************
�������ƣ�	void App_Int_PrdTimer_Route( )
����������	��������ups���������ֵ
�㷨˵����
��ڲ�����
���ڲ�����
��������:	070919
�޸�����:	070919
*********************************************************** */
#pragma CODE_SECTION(".Epwm1Funcs")

void	Class_PowCANScheduler::App_Int_PrdTimer_Route(void)
{
	INT16	 i;


	//������ �����źŸ���̬ ��UPSTestMode
//	if ( sUPSModeRun.bConfig != 3 || sParaMaint.bParaSignalMask == 1 || sFlagSystem.bUPSTestMode == 1 )
//	{
//		return;
//	}

//  ECanbRegs.CANES.bit.BO == 1,����CAN�����ϴ���һ���쳣������(��CANTEC�ﵽ256����ֵʱ)����ʱֻ��ͨ�����CANMC�Ĵ����е�CCRλ����ABOλ(Auto Bus Open)���趨��
//  ���յ�128*11������λ���˳����뿪���߹ر�״̬�����Ҵ��������(CANTEC)λ�����

	//����busoff���ϻָ�
	if ( ECanbRegs.CANES.bit.BO != 0 )
	{
		EALLOW;
		ECanbRegs.CANMC.bit.ABO = 1;
		EDIS;
	}


	/*
		By wxw20090714 PowerCanģ��ÿ2ms����һ�η��ͣ�
		�����ش�СԼһ����Ƶ���ڸ���һ�Σ���˶���50Hzϵͳÿ40���������ڷ�һ�θ��ش�С����40*2 = 80ms
    */
	for (i = 0; i < POW_CAN_PRD_TABL_SIZE; i++)
	{
		if (++PrdSchdlrTbl[i].m_u16TickCntr >= PrdSchdlrTbl[i].m_u16SetTickCnt)
		{
			PrdSchdlrTbl[i].m_u16TickCntr = 0;
			switch (i)
			{
				case 0:
					App_Int_Can_Recv();
					break;
				case 1:
					//By wxw20090724 30Kģ��PowerCanֻ���������������ϵͳ��������ŵ���
					//20KPowerCan���贫�ݸ��ش�С�������һֱ����
					if((objMonInterface.m_st_wUPSModeRun.bConfig == 3) && (objParallel.m_st_wParaMaint.bParaSignalMask != 1))
					{
						//By wxw20090714 ��ͨ�����͵��Ⱥ���App_Int_Can_SendSchedule
						//�õ���ǰ�跢�͵�֡����,�ٷ�����
						i16ScheduleID = App_Int_Can_SendSchedule();
						Dat_Int_Can_Send();
					}
					break;
				case 2:
					if((objMonInterface.m_st_wUPSModeRun.bConfig == 3)
					&& (objParallel.m_st_wParaMaint.bParaSignalMask != 1)
					&& (objSystem.m_st_wFlagSystem.bAfterParamDly == 1)
					&& (objDigitalIO.m_st_HardwareSignal.bRecOff == 0))
					{
						App_Int_Can_ComErr();
					}
					else
					{
						PowCanParaInfoEntry.MsgRecvFalg.m_u16All = 0;
						PowCanParaInfoEntry.UpsOnlineInfo.uOnlineFlag.m_u16All = 0;
					}
					break;
				default:
					break;

			}
		}
	}
}

/* ***********************************************************
�������ƣ�	void Int_Can_Recv( )
����������	��������ups��������޹�����ֵ
�㷨˵����
��ڲ�����
���ڲ�����
��������:	070919
�޸�����:	070919
*********************************************************** */
#pragma CODE_SECTION(".Epwm1Funcs")

void Class_PowCANScheduler::App_Int_Can_Recv(void)
{
	volatile 	struct MBOX *pMBox;
	volatile	struct	ECAN_REGS	*pECanReg;

	pECanReg = &ECanbRegs;

	//��ȡ����9����
	pMBox = &(ECanbMboxes.MBOX9);
	objPowCANPro.Dat_Int_MBox_Read(pMBox, BIT9);

	//��ȡ����8����
	pMBox = &(ECanbMboxes.MBOX8);
	objPowCANPro.Dat_Int_MBox_Read(pMBox, BIT8);

	//��ȡ����7����
	pMBox = &(ECanbMboxes.MBOX7);
	objPowCANPro.Dat_Int_MBox_Read(pMBox, BIT7);

	//��ȡ����6����
	pMBox = &(ECanbMboxes.MBOX6);
	objPowCANPro.Dat_Int_MBox_Read(pMBox, BIT6);

	//�������ͳɹ������±�������
	//*****��̨��Ϊ1ʱ��D��Q��Z�����������Ϣ���󣬾�����������̨�����ƣ�����ʹ��ʱע�⡣
	if (pECanReg->CANTRS.bit.TRS11 == 0)
	{
		PowCanParaInfoEntry.ParaDQCurrEntry[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16IINVAP = m_i16InvaPSend;
		PowCanParaInfoEntry.ParaDQCurrEntry[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16IINVAQ = m_i16InvaQSend;
		PowCanParaInfoEntry.ParaDQCurrEntry[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16IINVBP = m_i16InvbPSend;
		PowCanParaInfoEntry.ParaDQCurrEntry[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16IINVBQ = m_i16InvbQSend;
		PowCanParaInfoEntry.ParaDQCurrEntry[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16IINVCP = m_i16InvcPSend;
		PowCanParaInfoEntry.ParaDQCurrEntry[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16IINVCQ = m_i16InvcQSend;

		PowCanParaInfoEntry.ParaDQCurrEntry[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16DCurr = m_i16DCurrSend;

	}
}
/* ***********************************************************
�������ƣ�	void Int_Can_Send( )
����������	�㲥��ups���������ֵ
�㷨˵����
��ڲ�����
���ڲ�����
��������:	070919
�޸�����:	070919
*********************************************************** */
#pragma CODE_SECTION(".Epwm1Funcs")

void Class_PowCANScheduler::Dat_Int_Can_Send(void)
{
	UINT16	i16TempID;

	pECanReg = &ECanbRegs;
	pECanMboxes = &ECanbMboxes;
	i16TempID = objDigitalIO.m_st_wHardwareNewSignal.bInvID;

	PowCanParaInfoEntry.MsgRecvFalg.m_u16All |= ( 1 << objDigitalIO.m_st_wHardwareNewSignal.bInvID);
	//���ͳɹ�
	if (pECanReg->CANTRS.bit.TRS11 == 0)
	{
		//������ͳɹ���־λ
		tempECanReg.CANTA.all = pECanReg->CANTA.all;
		tempECanReg.CANTA.bit.TA11 = 1;
		pECanReg->CANTA.all = tempECanReg.CANTA.all;

		//������ͱ����Ա�־λ
		tempECanReg.CANAA.all = pECanReg->CANAA.all;
		tempECanReg.CANAA.bit.AA11 = 1;
		pECanReg->CANAA.all = tempECanReg.CANAA.all;

		//��ֹ����11����
		tempECanReg.CANME.all = pECanReg->CANME.all;
		tempECanReg.CANME.bit.ME11 = 0;
		pECanReg->CANME.all = tempECanReg.CANME.all;

		//����11 ID ����
		tempMbox.MSGID.all =  pECanMboxes->MBOX11.MSGID.all;
		tempMbox.MSGID.bit.IDE = 0; 				//stardart identifier
		tempMbox.MSGID.bit.AME = 0;					//dont care
		tempMbox.MSGID.bit.AAM = 0;					//normal transmit mode

		switch(i16ScheduleID)
		{
		 	case SEND_IINVADQCURRENT:
				if(objXfer.m_st_XferResult.bInvKM == 1)
				{
					tempMbox.MSGID.bit.STDMSGID = (SEND_IINVADQCURRENT << 6);//д��Ϣ�����ID
					PowerCANParaLoadEntry.DqCurrentValid.m_u16All |=( 1 << (i16TempID));
				}
				else
				{
					tempMbox.MSGID.bit.STDMSGID = (MESSAGEID_INVALID_CURRENT<< 6);//д��Ч������Ϣ�����ID
					//PowerCANParaLoadEntry.DqCurrentValid.m_u16All = 0;
					PowerCANParaLoadEntry.DqCurrentValid.m_u16All &= (~(1 << i16TempID));
				}
				//m_u16UpsIdxTemp = iSerialNumSet_0 & 0x07;//дUPS ID
				tempMbox.MSGID.bit.STDMSGID |=i16TempID;// objDigitalIO.m_st_wHardwareNewSignal.bInvID;
				tempMbox.MSGID.bit.EXTMSGID_H = 0;//��չID
				tempMbox.MSGID.bit.EXTMSGID_L = 0;//��չID

				pECanMboxes->MBOX11.MSGID.all = tempMbox.MSGID.all;
				pECanMboxes->MBOX11.MSGCTRL.all = 0x00000006;//д���ݳ���



			//д����

					pECanMboxes->MBOX11.MDL.word.LOW_WORD= objInverter.m_i16IinvaP_0;
					pECanMboxes->MBOX11.MDL.word.HI_WORD = objInverter.m_i16IinvaQ_0;
					pECanMboxes->MBOX11.MDH.word.LOW_WORD = objInverter.m_unIinvMdFilter_0.half.hword;

					m_i16InvaPSend = objInverter.m_i16IinvaP_0;
					m_i16InvaQSend = objInverter.m_i16IinvaQ_0;
					m_i16DCurrSend = objInverter.m_unIinvMdFilter_0.half.hword;

				break;


			case SEND_IINVBDQCURRENT:
				if(objXfer.m_st_XferResult.bInvKM == 1)
				{
					tempMbox.MSGID.bit.STDMSGID = (SEND_IINVBDQCURRENT << 6);//д��Ϣ�����ID
					PowerCANParaLoadEntry.DqCurrentValid.m_u16All |=( 1 << (i16TempID));
				}
				else
				{
					tempMbox.MSGID.bit.STDMSGID = (MESSAGEID_INVALID_CURRENT<< 6);//д��Ч������Ϣ�����ID
					//PowerCANParaLoadEntry.DqCurrentValid.m_u16All = 0;
					PowerCANParaLoadEntry.DqCurrentValid.m_u16All &= (~(1 << i16TempID));
				}
				//m_u16UpsIdxTemp = iSerialNumSet_0 & 0x07;//дUPS ID
				tempMbox.MSGID.bit.STDMSGID |=i16TempID;// objDigitalIO.m_st_wHardwareNewSignal.bInvID;
				tempMbox.MSGID.bit.EXTMSGID_H = 0;//��չID
				tempMbox.MSGID.bit.EXTMSGID_L = 0;//��չID

				pECanMboxes->MBOX11.MSGID.all = tempMbox.MSGID.all;
				pECanMboxes->MBOX11.MSGCTRL.all = 0x00000006;//д���ݳ���



			//д����

					pECanMboxes->MBOX11.MDL.word.LOW_WORD= objInverter.m_i16IinvbP_0;
					pECanMboxes->MBOX11.MDL.word.HI_WORD = objInverter.m_i16IinvbQ_0;
					pECanMboxes->MBOX11.MDH.word.LOW_WORD = objInverter.m_unIinvMdFilter_0.half.hword;

					objPowCANSched.m_i16InvbPSend = objInverter.m_i16IinvbP_0;
					objPowCANSched.m_i16InvbQSend = objInverter.m_i16IinvbQ_0;
					objPowCANSched.m_i16DCurrSend = objInverter.m_unIinvMdFilter_0.half.hword;

				break;


			case SEND_IINVCDQCURRENT:
				if(objXfer.m_st_XferResult.bInvKM == 1)
				{
					tempMbox.MSGID.bit.STDMSGID = (SEND_IINVCDQCURRENT << 6);//д��Ϣ�����ID
					PowerCANParaLoadEntry.DqCurrentValid.m_u16All |=( 1 << (i16TempID));
				}
				else
				{
					tempMbox.MSGID.bit.STDMSGID = (MESSAGEID_INVALID_CURRENT<< 6);//д��Ч������Ϣ�����ID
					//PowerCANParaLoadEntry.DqCurrentValid.m_u16All = 0;
					PowerCANParaLoadEntry.DqCurrentValid.m_u16All &= (~(1 << i16TempID));
				}
				//m_u16UpsIdxTemp = iSerialNumSet_0 & 0x07;//дUPS ID
				tempMbox.MSGID.bit.STDMSGID |=i16TempID;// objDigitalIO.m_st_wHardwareNewSignal.bInvID;
				tempMbox.MSGID.bit.EXTMSGID_H = 0;//��չID
				tempMbox.MSGID.bit.EXTMSGID_L = 0;//��չID

				pECanMboxes->MBOX11.MSGID.all = tempMbox.MSGID.all;
				pECanMboxes->MBOX11.MSGCTRL.all = 0x00000006;//д���ݳ���



			//д����

					pECanMboxes->MBOX11.MDL.word.LOW_WORD= objInverter.m_i16IinvcP_0;
					pECanMboxes->MBOX11.MDL.word.HI_WORD = objInverter.m_i16IinvcQ_0;
					pECanMboxes->MBOX11.MDH.word.LOW_WORD = objInverter.m_unIinvMdFilter_0.half.hword;

					objPowCANSched.m_i16InvcPSend = objInverter.m_i16IinvcP_0;
					objPowCANSched.m_i16InvcQSend = objInverter.m_i16IinvcQ_0;
					objPowCANSched.m_i16DCurrSend = objInverter.m_unIinvMdFilter_0.half.hword;

				break;


			case SEND_PLOAD:
				tempMbox.MSGID.bit.STDMSGID = (MESSAGEID_PLOAD << 6);//д��Ϣ�����ID
				//m_u16UpsIdxTemp = iSerialNumSet_0 & 0x07;//дUPS ID
				tempMbox.MSGID.bit.STDMSGID |=i16TempID;// objDigitalIO.m_st_wHardwareNewSignal.bInvID;
				tempMbox.MSGID.bit.EXTMSGID_H = 0;//��չID
				tempMbox.MSGID.bit.EXTMSGID_L = 0;//��չID

				pECanMboxes->MBOX11.MSGID.all = tempMbox.MSGID.all;
				pECanMboxes->MBOX11.MSGCTRL.all = 0x00000006;//д���ݳ���

				pECanMboxes->MBOX11.MDL.word.LOW_WORD = m_i16PoutaSend;
				pECanMboxes->MBOX11.MDL.word.HI_WORD = m_i16PoutbSend;
				pECanMboxes->MBOX11.MDH.word.LOW_WORD = m_i16PoutcSend;
				break;


			case SEND_SLOAD:
				tempMbox.MSGID.bit.STDMSGID = (MESSAGEID_SLOAD << 6);//д��Ϣ�����ID
				//m_u16UpsIdxTemp = iSerialNumSet_0 & 0x07;//дUPS ID
				tempMbox.MSGID.bit.STDMSGID |=i16TempID;// objDigitalIO.m_st_wHardwareNewSignal.bInvID;
				tempMbox.MSGID.bit.EXTMSGID_H = 0;//��չID
				tempMbox.MSGID.bit.EXTMSGID_L = 0;//��չID

				pECanMboxes->MBOX11.MSGID.all = tempMbox.MSGID.all;
				pECanMboxes->MBOX11.MSGCTRL.all = 0x00000006;//д���ݳ���


			//д����
				pECanMboxes->MBOX11.MDL.word.LOW_WORD = m_i16SoutaSend;
				pECanMboxes->MBOX11.MDL.word.HI_WORD = m_i16SoutbSend;
				pECanMboxes->MBOX11.MDH.word.LOW_WORD = m_i16SoutcSend;
				break;

			case SEND_SYSTEMSTATE:
				tempMbox.MSGID.bit.STDMSGID = (MESSAGEID_STATE << 6);//д��Ϣ�����ID
				//m_u16UpsIdxTemp = iSerialNumSet_0 & 0x07;//дUPS ID
				tempMbox.MSGID.bit.STDMSGID |=i16TempID;// objDigitalIO.m_st_wHardwareNewSignal.bInvID;
				tempMbox.MSGID.bit.EXTMSGID_H = 0;//��չID
				tempMbox.MSGID.bit.EXTMSGID_L = 0;//��չID

				pECanMboxes->MBOX11.MSGID.all = tempMbox.MSGID.all;
				pECanMboxes->MBOX11.MSGCTRL.all = 0x00000004;//д���ݳ���

				pECanMboxes->MBOX11.MDL.word.LOW_WORD = m_i16InvState;
				pECanMboxes->MBOX11.MDL.word.HI_WORD =  m_i16BattState;
				break;


			case SEND_ILFILTER:
				if(objXfer.m_st_XferResult.bInvKM == 1)
				{
					tempMbox.MSGID.bit.STDMSGID = (MESSAGEID_ILFILTER << 6);//д��Ϣ�����ID
					PowerCANParaLoadEntry.DqCurrentValid.m_u16All |=( 1 << (i16TempID));
				}
				else
				{
					tempMbox.MSGID.bit.STDMSGID = (MESSAGEID_INVALID_CURRENT<< 6);//д��Ч������Ϣ�����ID
					//PowerCANParaLoadEntry.DqCurrentValid.m_u16All = 0;
					PowerCANParaLoadEntry.DqCurrentValid.m_u16All &= (~(1 << i16TempID));
				}
				//m_u16UpsIdxTemp = iSerialNumSet_0 & 0x07;//дUPS ID
				tempMbox.MSGID.bit.STDMSGID |=i16TempID;// objDigitalIO.m_st_wHardwareNewSignal.bInvID;
				tempMbox.MSGID.bit.EXTMSGID_H = 0;//��չID
				tempMbox.MSGID.bit.EXTMSGID_L = 0;//��չID

				pECanMboxes->MBOX11.MSGID.all = tempMbox.MSGID.all;
				pECanMboxes->MBOX11.MSGCTRL.all = 0x00000006;//д���ݳ���

				pECanMboxes->MBOX11.MDL.word.LOW_WORD = objInverter.m_i16IlaFilter2_0;
				pECanMboxes->MBOX11.MDL.word.HI_WORD =  objInverter.m_i16IlbFilter2_0;
				pECanMboxes->MBOX11.MDH.word.LOW_WORD = objInverter.m_i16IlcFilter2_0;
				PowCanParaInfoEntry.IlFilter[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16IlFiltera = objInverter.m_i16IlaFilter2_0;
				PowCanParaInfoEntry.IlFilter[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16IlFilterb = objInverter.m_i16IlbFilter2_0;
				PowCanParaInfoEntry.IlFilter[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16IlFilterc = objInverter.m_i16IlcFilter2_0;
				break;

			default:
				break;
		}

		//����������
		tempECanReg.CANME.all = pECanReg->CANME.all;
		tempECanReg.CANME.bit.ME11 = 1;
		pECanReg->CANME.all = tempECanReg.CANME.all;

		//��������
		tempECanReg.CANTRS.all = pECanReg->CANTRS.all;
		tempECanReg.CANTRS.bit.TRS11 = 1;
		pECanReg->CANTRS.all = tempECanReg.CANTRS.all;
	}
}

/* ***********************************************************
�������ƣ�	void Int_Can_ComErr( )
����������	power ����ͨ�Ź����жϣ�������ȼ�����can
				��ͬά������ͨ�Ź��ϱ�־
�㷨˵����
��ڲ�����
���ڲ�����
��������:	070919
�޸�����:	070919
*********************************************************** */
#pragma CODE_SECTION(".Epwm1Funcs")

void Class_PowCANScheduler::App_Int_Can_ComErr(void)
{
	UINT16	 i;

	INT16 i16UpsOnlineNum = 0;
	static UINT16 s_u16ComErrCnt2ms = 0;

	for (i = 0;  i < ParaNumCNST;  i++)
	{
		//ups�����ж�
		if ((PowCanParaInfoEntry.MsgRecvFalg.m_u16All & (1 << i))  !=  0)
		{
			PowCanParaInfoEntry.UpsOnlineInfo.uOnlineFlag.m_u16All |= (1 << i);
			PowCanParaInfoEntry.UpsOnlineInfo.m_u16NotOnlineCnt[i] = 0;
			i16UpsOnlineNum++;
		}

		//ups�����жϣ���������û���յ�ĳups����Ϣ�����ø�ups״̬Ϊ����
		else
		{
			if (++PowCanParaInfoEntry.UpsOnlineInfo.m_u16NotOnlineCnt[i]  >=  UPS_NOT_ONLINE_MAX_CNT)
			{
				PowCanParaInfoEntry.UpsOnlineInfo.uOnlineFlag.m_u16All &= (~(1 << i));
				PowCanParaInfoEntry.UpsOnlineInfo.m_u16NotOnlineCnt[i] = UPS_NOT_ONLINE_MAX_CNT;

			}
		}
	}

	//�屾�����н��ձ�־
	PowCanParaInfoEntry.MsgRecvFalg.m_u16All = 0;

	//ʵ��̨������������̨���������ó��Զ���·����·��δ����
//	if (( i16UpsOnlineNum != (objMonInterface.m_st_wParallelData.iNumOn + 1) )

	//���о�: �ղ�����·ģ�����Ϣ����ΪͨѶ����
	//By wxw20090715 Adapter15K~20K���ʵ�ʲ���̨��С�ڻ���̨������ͨѶ����
	if(i16UpsOnlineNum < (objMonInterface.m_i16BasicNum + objMonInterface.m_i16RedunNum))
	{
		//����120ms����������̨��,��ͨѶ����
		if (++s_u16ComErrCnt2ms >= UPS_NOT_ONLINE_MAX_CNT_0)
		{
			s_u16ComErrCnt2ms = UPS_NOT_ONLINE_MAX_CNT_0;
			PowCanParaInfoEntry.UpsOnlineInfo.uOnlineFlag.tBits.bSysCommerr = 1;
		}
	}
	else
	{
		PowCanParaInfoEntry.UpsOnlineInfo.uOnlineFlag.tBits.bSysCommerr = 0;
		s_u16ComErrCnt2ms = 0;
	}
}

/* ***********************************************************
�������ƣ�	UINT16 App_Int_Can_SendSchedule( void)
����������	power �������͵��Ⱥ���
�㷨˵����
��ڲ�����
���ڲ�����	SEND_DQCURRENT:��ǰ�贫DQ����֡
			SEND_PLOAD	  :��ǰ�贫�й�������Ϣ֡
			SEND_SLOAD	  :��ǰ�贫���ڹ�����Ϣ֡
��������:	090714
�޸�����:	090714
*********************************************************** */
#pragma CODE_SECTION(".Epwm1Funcs")
UINT16 Class_PowCANScheduler::App_Int_Can_SendSchedule(void)
{
	UINT16 i16SendId;

	/*
		By wxw20090714 PowerCanģ��ÿ1ms����һ�η��ͣ�
		�����ش�СԼһ����Ƶ���ڸ���һ�Σ���˶���50Hzϵͳÿ20���������ڷ�һ�θ��ش�С
		����60Hzϵͳ16.7���������ڷ���һ�θ��ش�С��ȡ���߼�Ĵ�ֵ
	*/
	switch(i16SendScheduleCnt)
	{
		case 5:
				i16SendId = SEND_SYSTEMSTATE;
				break;
		case 15:
				i16SendId = SEND_PLOAD;
				break;
		case 25:
				i16SendId = SEND_SLOAD;
				break;
		case 35:
		        i16SendId = SEND_ILFILTER;
		        break;
		default:
				m_i16IabcShedule++;
				if(m_i16IabcShedule>2)
				{
					m_i16IabcShedule = 0;
				}
				if(m_i16IabcShedule == 0)
				{
					i16SendId = SEND_IINVADQCURRENT;
				}
				else if(m_i16IabcShedule == 1)
				{
					i16SendId = SEND_IINVBDQCURRENT;
				}
				else if(m_i16IabcShedule == 2)
				{
					i16SendId = SEND_IINVCDQCURRENT;
				}
				break;
	}

	if( i16SendScheduleCnt <39)
	{
		i16SendScheduleCnt++;
	}
	else
	{
		i16SendScheduleCnt = 0;
	}
	return (i16SendId);
	//return(SEND_PLOAD);
}

/* ***********************************************************
�������ƣ�	void Dat_Can_LoadCalculation( void)
����������	power �������ؼ���
�㷨˵����	Adaptor15k~20K���Ӽ��㲢�����ش�С
��ڲ�����
���ڲ�����
��������:	090714
�޸�����:	090714
*********************************************************** */
void Class_PowCANScheduler::Dat_Can_LoadCalculation(void)
{
	INT16 i;
	INT16 PoutaTotal = 0;		//A�๦�ʺ�
	INT16 SoutaTotal = 0;		//A�����ڹ��ʺ�
	INT16 PoutbTotal = 0;		//B�๦�ʺ�
	INT16 SoutbTotal = 0;		//B�����ڹ��ʺ�
	INT16 PoutcTotal = 0;		//C�๦�ʺ�
	INT16 SoutcTotal = 0;		//C�����ڹ��ʺ�

	INT16 InvNum = 0;				//Inv����̨��
	INT16 OnNum  = 0;				//Inv����̨��

	INT16 ChgOnNum  = 0;			//Charger����̨��
	INT16 ChgPBoostNum  = 0;		//��Charger����̨��
	INT16 BattDischgNum  = 0;		//��طŵ�Ԥ�澯̨��
	INT16 BattPreWarnNum  = 0;		//��طŵ�Ԥ�澯̨��
	INT16 BattSelfTestNum  = 0;		//��طŵ�Ԥ�澯̨��

	INT16 ChgOnMinID  = 0;

	//��������ʱ�������͵��������
	if(( objXfer.m_st_wXferCmd.bSwitchStatus == 1 )||(objXfer.m_st_XferResult.bInvKM == 1))
	{
		m_i16PoutaSend = objInverter.m_i16Pinva;
		m_i16PoutbSend = objInverter.m_i16Pinvb;
		m_i16PoutcSend = objInverter.m_i16Pinvc;

		m_i16SoutaSend = objInverter.m_i16Sinva;
		m_i16SoutbSend = objInverter.m_i16Sinvb;
		m_i16SoutcSend = objInverter.m_i16Sinvc;
	}
	else if(( objXfer.m_st_wXferCmd.bSwitchStatus == 0 )&&(objXfer.m_st_XferResult.bBpSTS == 1))
	{
		m_i16PoutaSend = objOutput.m_i16Pouta;
		m_i16PoutbSend = objOutput.m_i16Poutb;
		m_i16PoutcSend = objOutput.m_i16Poutc;

		m_i16SoutaSend = objOutput.m_i16Souta;
		m_i16SoutbSend = objOutput.m_i16Soutb;
		m_i16SoutcSend = objOutput.m_i16Soutc;
	}
	else
	{
		m_i16PoutaSend = 1;
		m_i16PoutbSend = 1;
		m_i16PoutcSend = 1;
		m_i16SoutaSend = 1;
		m_i16SoutbSend = 1;
		m_i16SoutcSend = 1;
	}
	//�ӹ�״̬��־
	m_i16InvState = (objSwitches.m_st_FlagBreaker.bInv + (objSwitches.m_st_FlagBreaker.bOn<<1));
	m_i16BattState = S2U(m_st_BatteryStatus);

	//���±�������
	PowerCANParaLoadEntry.ParaLoadData[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16Pouta = m_i16PoutaSend;
	PowerCANParaLoadEntry.ParaLoadData[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16Poutb = m_i16PoutbSend;;
	PowerCANParaLoadEntry.ParaLoadData[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16Poutc = m_i16PoutcSend;;


	PowerCANParaLoadEntry.ParaLoadData[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16Souta = m_i16SoutaSend;
	PowerCANParaLoadEntry.ParaLoadData[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16Soutb = m_i16SoutbSend;
	PowerCANParaLoadEntry.ParaLoadData[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16Soutc = m_i16SoutcSend;
	PowerCANParaLoadEntry.SysFlag[objDigitalIO.m_st_wHardwareNewSignal.bInvID].InvFlag.m_u16all	= m_i16InvState;
	PowerCANParaLoadEntry.SysFlag[objDigitalIO.m_st_wHardwareNewSignal.bInvID].BattFlag.m_u16all= m_i16BattState;

	for (i = 1; i < ParaNumCNST; i++)
	{
		if ((PowCanParaInfoEntry.UpsOnlineInfo.uOnlineFlag.m_u16All & (1 << i)) != 0)
		{
			//�������й�����
			PoutaTotal += PowerCANParaLoadEntry.ParaLoadData[i].m_i16Pouta;
			PoutbTotal += PowerCANParaLoadEntry.ParaLoadData[i].m_i16Poutb;
			PoutcTotal += PowerCANParaLoadEntry.ParaLoadData[i].m_i16Poutc;

			//���������ڹ���
			SoutaTotal += PowerCANParaLoadEntry.ParaLoadData[i].m_i16Souta;
			SoutbTotal += PowerCANParaLoadEntry.ParaLoadData[i].m_i16Soutb;
			SoutcTotal += PowerCANParaLoadEntry.ParaLoadData[i].m_i16Soutc;
			if(PowerCANParaLoadEntry.SysFlag[i].InvFlag.tbits.bInv == 1)
			{
				InvNum++;
			}
			if(PowerCANParaLoadEntry.SysFlag[i].InvFlag.tbits.bOn == 1)
			{
				OnNum++;
			}


			//���õ�������״̬�ӹ�
			if((PowerCANParaLoadEntry.SysFlag[i].BattFlag.tbits.bChargerWorkP == 1)
			||(PowerCANParaLoadEntry.SysFlag[i].BattFlag.tbits.bChargerWorkN == 1))
			{
				ChgOnNum++;

				if(ChgOnNum == 1)
				{
					ChgOnMinID = i;
				}
			}

			if(PowerCANParaLoadEntry.SysFlag[i].BattFlag.tbits.bChgBostP == 1)
			{
				ChgPBoostNum++;
			}

			if(PowerCANParaLoadEntry.SysFlag[i].BattFlag.tbits.bBattDischg == 1)
			{
				BattDischgNum++;
			}

			if(PowerCANParaLoadEntry.SysFlag[i].BattFlag.tbits.bBattPreWarn == 1)
			{
				BattPreWarnNum++;
			}

			if(PowerCANParaLoadEntry.SysFlag[i].BattFlag.tbits.bBattSelfTest == 1)
			{
				BattSelfTestNum++;
			}
		}
	}
	m_i16PoutaTotal = PoutaTotal;
	m_i16PoutbTotal = PoutbTotal;
	m_i16PoutcTotal = PoutcTotal;

	m_i16SoutaTotal = SoutaTotal;
	m_i16SoutbTotal = SoutbTotal;
	m_i16SoutcTotal = SoutcTotal;

	m_i16PoutTotalMax = MAX2(PoutaTotal,PoutbTotal);
	m_i16PoutTotalMax = MAX2(m_i16PoutTotalMax,PoutcTotal);

	m_i16SoutTotalMax = MAX2(SoutaTotal,SoutbTotal);
	m_i16SoutTotalMax = MAX2(m_i16SoutTotalMax,SoutcTotal);


	m_i16InvTotalNum = InvNum;
	m_i16OnTotalNum = OnNum;

	if(objMonInterface.m_st_wUPSModeRun2.bBattShare == 1)
	{
			m_i16ChgOnTotalNum = ChgOnNum;
			m_i16ChgPBoostTotalNum = ChgPBoostNum;
			m_i16BattDischgTotalNum = BattDischgNum;
			m_i16BattPreWarnTotalNum = BattPreWarnNum;
			m_i16BattSelfTestTotalNum = BattSelfTestNum;
			m_i16ChgOnMinID = ChgOnMinID;

			if(m_i16ChgPBoostTotalNum != 0)
			{
				m_i16ChgPBoostTotal = 1;
			}
			else
			{
				m_i16ChgPBoostTotal = 0;
			}

			if(m_i16BattDischgTotalNum != 0)
			{
				m_i16ChgOnTotalNum = 0;
				m_i16ChgPBoostTotal = 0;
				m_i16ChgOnMinID = 0;

				if(m_i16BattDischgTotalNum == m_i16BattPreWarnTotalNum)
				{
					m_i16BattPreWarnTotal = 1;
				}
				else
				{
					m_i16BattPreWarnTotal = 0;
				}
			}
			else
			{
				m_i16BattPreWarnTotal = 0;
			}

			if(m_i16BattSelfTestTotalNum != 0)
			{
					m_i16BattSelfTestTotal = 1;
			}
			else
			{
					m_i16BattSelfTestTotal = 0;
			}
	}
	else
	{
			m_i16ChgOnTotalNum = 0;
			m_i16ChgPBoostTotalNum = 0;
			m_i16BattDischgTotalNum = 0;
			m_i16BattPreWarnTotalNum = 0;
			m_i16ChgOnMinID = 0;
			m_i16ChgPBoostTotal = 0;
			m_i16BattPreWarnTotal = 0;
			m_i16BattSelfTestTotalNum = 0;
			m_i16BattSelfTestTotal = 0;

	}
}

/* ***********************************************************
�������ƣ�	void App_Can_ILFilter( void)
����������
�㷨˵����
��ڲ�����
���ڲ�����
��������:	090714
�޸�����:	090714
*********************************************************** */
void Class_PowCANScheduler::App_Can_ILFilter(void)
{
	INT16 i;
	INT16	i16DqValidNum = 0;
	INT32	i32ILaFilterSum = 0;
	INT32	i32ILbFilterSum = 0;
	INT32	i32ILcFilterSum = 0;

	for (i = 1; i < ParaNumCNST; i++)
	{
		if ((PowCanParaInfoEntry.UpsOnlineInfo.uOnlineFlag.m_u16All & (1 << i)) != 0)
		{
			if((PowerCANParaLoadEntry.DqCurrentValid.m_u16All& (1 << i)) != 0)
			{

				i32ILaFilterSum += PowCanParaInfoEntry.IlFilter[i].m_i16IlFiltera;
				i32ILbFilterSum += PowCanParaInfoEntry.IlFilter[i].m_i16IlFilterb;
				i32ILcFilterSum += PowCanParaInfoEntry.IlFilter[i].m_i16IlFilterc;

				i16DqValidNum++;
			}
		}
	}
	if(i16DqValidNum != 0)
	{
		objInvAlg.m_i16IlaFilterAvg_0 = i32ILaFilterSum / i16DqValidNum;
		objInvAlg.m_i16IlbFilterAvg_0 = i32ILbFilterSum / i16DqValidNum;
		objInvAlg.m_i16IlcFilterAvg_0 = i32ILcFilterSum / i16DqValidNum;
	}
	else
	{
		objInvAlg.m_i16IlaFilterAvg_0 = 0;
		objInvAlg.m_i16IlbFilterAvg_0 = 0;
		objInvAlg.m_i16IlcFilterAvg_0 = 0;
	}
	objInvAlg.m_i16IlaFilterShare_0 = PowCanParaInfoEntry.IlFilter[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16IlFiltera;
	objInvAlg.m_i16IlbFilterShare_0 = PowCanParaInfoEntry.IlFilter[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16IlFilterb;
	objInvAlg.m_i16IlcFilterShare_0 = PowCanParaInfoEntry.IlFilter[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16IlFilterc;
}

/* ***********************************************************
�������ƣ�	void App_Can_IInvDQCal( void)
����������
�㷨˵����
��ڲ�����
���ڲ�����
��������:	090714
�޸�����:	090714
*********************************************************** */
void Class_PowCANScheduler::App_Can_IInvDQCal(void)
{
	INT16 i;
	INT16	i16DqValidNum = 0;
	INT32	i32IaPSum = 0;
	INT32	i32IaQSum = 0;
	INT32	i32IbPSum = 0;
	INT32	i32IbQSum = 0;
	INT32	i32IcPSum = 0;
	INT32	i32IcQSum = 0;
	INT32	i32IdSum=0;
	INT16	i16UpsOnlineNum = 0;


	for (i = 1; i < ParaNumCNST; i++)
	{
		if ((PowCanParaInfoEntry.UpsOnlineInfo.uOnlineFlag.m_u16All & (1 << i)) != 0)
		{
			if((PowerCANParaLoadEntry.DqCurrentValid.m_u16All& (1 << i)) != 0)
			{

				i32IaPSum += m_i16PowerCanIaPM_0[i];
				i32IaQSum += m_i16PowerCanIaQM_0[i];
				i32IbPSum += m_i16PowerCanIbPM_0[i];
				i32IbQSum += m_i16PowerCanIbQM_0[i];
				i32IcPSum += m_i16PowerCanIcPM_0[i];
				i32IcQSum += m_i16PowerCanIcQM_0[i];

				i32IdSum  += m_i16PowerCanDcurr_0[i];

				i16DqValidNum++;
			}
			i16UpsOnlineNum++;
		}
	}


	m_i16PowerCanOnlineNum = i16UpsOnlineNum;
	m_i16PowerCanTotalNum = i16DqValidNum;

	if(i16DqValidNum != 0)
	{
		objInvAlg.m_i16IlaPAvg_0 = i32IaPSum / i16DqValidNum;
		objInvAlg.m_i16IlaQAvg_0 = i32IaQSum / i16DqValidNum;
		objInvAlg.m_i16IlbPAvg_0 = i32IbPSum / i16DqValidNum;
		objInvAlg.m_i16IlbQAvg_0 = i32IbQSum / i16DqValidNum;
		objInvAlg.m_i16IlcPAvg_0 = i32IcPSum / i16DqValidNum;
		objInvAlg.m_i16IlcQAvg_0 = i32IcQSum / i16DqValidNum;
		objInvAlg.m_i16IdAvg_0	 = i32IdSum / i16DqValidNum;
	}
	else
	{
		objInvAlg.m_i16IlaPAvg_0 = 0;
		objInvAlg.m_i16IlaQAvg_0 = 0;
		objInvAlg.m_i16IlbPAvg_0 = 0;
		objInvAlg.m_i16IlbQAvg_0 = 0;
		objInvAlg.m_i16IlcPAvg_0 = 0;
		objInvAlg.m_i16IlcQAvg_0 = 0;

		objInvAlg.m_i16IdAvg_0 = 0;
	}
	objInvAlg.m_i16IlaPShare_0 = m_i16PowerCanIaPM_0[objDigitalIO.m_st_wHardwareNewSignal.bInvID];
	objInvAlg.m_i16IlaQShare_0 = m_i16PowerCanIaQM_0[objDigitalIO.m_st_wHardwareNewSignal.bInvID];
	objInvAlg.m_i16IlbPShare_0 = m_i16PowerCanIbPM_0[objDigitalIO.m_st_wHardwareNewSignal.bInvID];
	objInvAlg.m_i16IlbQShare_0 = m_i16PowerCanIbQM_0[objDigitalIO.m_st_wHardwareNewSignal.bInvID];
	objInvAlg.m_i16IlcPShare_0 = m_i16PowerCanIcPM_0[objDigitalIO.m_st_wHardwareNewSignal.bInvID];
	objInvAlg.m_i16IlcQShare_0 = m_i16PowerCanIcQM_0[objDigitalIO.m_st_wHardwareNewSignal.bInvID];
}

/* ***********************************************************
�������ƣ�	void Alg_Int_MainInterfaceVar( void)
����������
�㷨˵����
��ڲ�����
���ڲ�����
��������:	090714
�޸�����:	090714
*********************************************************** */
#pragma CODE_SECTION(".Epwm1Funcs")

void Class_PowCANScheduler::Alg_Int_MainInterfaceVar(void)
{
	INT16	i;

	for (i = 1; i < ParaNumCNST; i++)
	{

		m_i16PowerCanIaPM_0[i] = PowCanParaInfoEntry.ParaDQCurrEntry[i].m_i16IINVAP;
		m_i16PowerCanIaQM_0[i] = PowCanParaInfoEntry.ParaDQCurrEntry[i].m_i16IINVAQ;
		m_i16PowerCanIbPM_0[i] = PowCanParaInfoEntry.ParaDQCurrEntry[i].m_i16IINVBP;
		m_i16PowerCanIbQM_0[i] = PowCanParaInfoEntry.ParaDQCurrEntry[i].m_i16IINVBQ;
		m_i16PowerCanIcPM_0[i] = PowCanParaInfoEntry.ParaDQCurrEntry[i].m_i16IINVCP;
		m_i16PowerCanIcQM_0[i] = PowCanParaInfoEntry.ParaDQCurrEntry[i].m_i16IINVCQ;

		m_i16PowerCanDcurr_0[i] = PowCanParaInfoEntry.ParaDQCurrEntry[i].m_i16DCurr;
	}
}
