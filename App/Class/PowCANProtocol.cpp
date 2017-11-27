/*==========================================================*/
/* Title		:	PowCANProtocol.cpp						*/
/* Description	: 	Class_PowCANProtocol realization 		*/
/* Date			:	Apr.2008								*/
/*==========================================================*/
#include "PowCANProtocol.h"



/* ***********************************************************
�������ƣ�	void Int_MBox_Read( )
����������	��ȡ�������ݲ���ŵ���Ӧ��������
�㷨˵����	
��ڲ�����	
���ڲ�����	
��������:	070919 
�޸�����:	070919
*********************************************************** */ 
#pragma CODE_SECTION(".Epwm1Funcs")  

void Class_PowCANProtocol::Dat_Int_MBox_Read(volatile struct MBOX *pMBox, UINT32 u32MBoxIdx)
{
	INT16 FrameID;
	//�鿴�����Ƿ���յ�����
	if ((ECanbRegs.CANRMP.all & u32MBoxIdx) == u32MBoxIdx)
	{

		//�ж�UPS����
		m_u16UpsIdx = pMBox->MSGID.bit.STDMSGID;
		FrameID = (m_u16UpsIdx & MESSAGEID_MASK)>>6;
		m_u16UpsIdx &= UPSID_MASK;
		if((FrameID == MESSAGEID_IINVADQCURRENT)||(FrameID == MESSAGEID_IINVBDQCURRENT)||(FrameID == MESSAGEID_IINVCDQCURRENT)||(FrameID== MESSAGEID_INVALID_CURRENT)||(FrameID == MESSAGEID_ILFILTER))	//D,Q������Ϣ֡
		{
			//�������
			if(FrameID == MESSAGEID_IINVADQCURRENT)
			{
				objPowCANSched.PowCanParaInfoEntry.ParaDQCurrEntry[m_u16UpsIdx].m_i16IINVAP = pMBox->MDL.word.LOW_WORD;
				objPowCANSched.PowCanParaInfoEntry.ParaDQCurrEntry[m_u16UpsIdx].m_i16IINVAQ = pMBox->MDL.word.HI_WORD;

				objPowCANSched.PowCanParaInfoEntry.ParaDQCurrEntry[m_u16UpsIdx].m_i16DCurr = pMBox->MDH.word.LOW_WORD;
			}
			else if(FrameID == MESSAGEID_IINVBDQCURRENT)
			{
				objPowCANSched.PowCanParaInfoEntry.ParaDQCurrEntry[m_u16UpsIdx].m_i16IINVBP = pMBox->MDL.word.LOW_WORD;
				objPowCANSched.PowCanParaInfoEntry.ParaDQCurrEntry[m_u16UpsIdx].m_i16IINVBQ = pMBox->MDL.word.HI_WORD;
				
				objPowCANSched.PowCanParaInfoEntry.ParaDQCurrEntry[m_u16UpsIdx].m_i16DCurr = pMBox->MDH.word.LOW_WORD;
			}
			else if(FrameID == MESSAGEID_IINVCDQCURRENT)
			{
				objPowCANSched.PowCanParaInfoEntry.ParaDQCurrEntry[m_u16UpsIdx].m_i16IINVCP = pMBox->MDL.word.LOW_WORD;
				objPowCANSched.PowCanParaInfoEntry.ParaDQCurrEntry[m_u16UpsIdx].m_i16IINVCQ = pMBox->MDL.word.HI_WORD;
				
				objPowCANSched.PowCanParaInfoEntry.ParaDQCurrEntry[m_u16UpsIdx].m_i16DCurr = pMBox->MDH.word.LOW_WORD;
			}
			else if(FrameID == MESSAGEID_ILFILTER)
			{
				objPowCANSched.PowCanParaInfoEntry.IlFilter[m_u16UpsIdx].m_i16IlFiltera= pMBox->MDL.word.LOW_WORD;
				objPowCANSched.PowCanParaInfoEntry.IlFilter[m_u16UpsIdx].m_i16IlFilterb = pMBox->MDL.word.HI_WORD;
				objPowCANSched.PowCanParaInfoEntry.IlFilter[m_u16UpsIdx].m_i16IlFilterc = pMBox->MDH.word.LOW_WORD;
			}

			//By wxw20090627 Adapter15k~20K����·��
			if(FrameID== MESSAGEID_INVALID_CURRENT)
			{
				objPowCANSched.PowerCANParaLoadEntry.DqCurrentValid.m_u16All &= (~(1 << m_u16UpsIdx));
			}
			else
			{
				objPowCANSched.PowerCANParaLoadEntry.DqCurrentValid.m_u16All |= ( 1 << (m_u16UpsIdx));
			}
			if(objXfer.m_st_XferResult.bInvKM == 0)
			{
				objPowCANSched.PowerCANParaLoadEntry.DqCurrentValid.m_u16All = 0;
			}
		}
		else if(FrameID== MESSAGEID_PLOAD)	//�����й�������Ϣ֡
		{
			//�������
			objPowCANSched.PowerCANParaLoadEntry.ParaLoadData[m_u16UpsIdx].m_i16Pouta = pMBox->MDL.word.LOW_WORD;
			objPowCANSched.PowerCANParaLoadEntry.ParaLoadData[m_u16UpsIdx].m_i16Poutb = pMBox->MDL.word.HI_WORD;
			objPowCANSched.PowerCANParaLoadEntry.ParaLoadData[m_u16UpsIdx].m_i16Poutc = pMBox->MDH.word.LOW_WORD;
		}
		else if(FrameID== MESSAGEID_SLOAD)	//�������ڹ�����Ϣ֡
		{
			//�������
			objPowCANSched.PowerCANParaLoadEntry.ParaLoadData[m_u16UpsIdx].m_i16Souta = pMBox->MDL.word.LOW_WORD;
			objPowCANSched.PowerCANParaLoadEntry.ParaLoadData[m_u16UpsIdx].m_i16Soutb = pMBox->MDL.word.HI_WORD;
			objPowCANSched.PowerCANParaLoadEntry.ParaLoadData[m_u16UpsIdx].m_i16Soutc = pMBox->MDH.word.LOW_WORD;
		}
		else if(FrameID== MESSAGEID_STATE)	//��ؼ����״̬֡
		{
			//�������
			objPowCANSched.PowerCANParaLoadEntry.SysFlag[m_u16UpsIdx].InvFlag.m_u16all= pMBox->MDL.word.LOW_WORD;
			objPowCANSched.PowerCANParaLoadEntry.SysFlag[m_u16UpsIdx].BattFlag.m_u16all= pMBox->MDL.word.HI_WORD;
		}
		
		//����ӦUPS������ɱ�־
		objPowCANSched.PowCanParaInfoEntry.MsgRecvFalg.m_u16All |= ( 1 << (m_u16UpsIdx));
		//�ͷ����䣬�����½����µ�����
		ECanbRegs.CANRMP.all = u32MBoxIdx;          //���ܻᱻ��һ������д���Ƕ���һ�������ֵ������OPC�Ĵ���δʹ��
	}
}
