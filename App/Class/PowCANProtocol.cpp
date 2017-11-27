/*==========================================================*/
/* Title		:	PowCANProtocol.cpp						*/
/* Description	: 	Class_PowCANProtocol realization 		*/
/* Date			:	Apr.2008								*/
/*==========================================================*/
#include "PowCANProtocol.h"



/* ***********************************************************
函数名称：	void Int_MBox_Read( )
功能描述：	读取邮箱内容并存放到相应的数据区
算法说明：	
入口参数：	
出口参数：	
初稿日期:	070919 
修改日期:	070919
*********************************************************** */ 
#pragma CODE_SECTION(".Epwm1Funcs")  

void Class_PowCANProtocol::Dat_Int_MBox_Read(volatile struct MBOX *pMBox, UINT32 u32MBoxIdx)
{
	INT16 FrameID;
	//查看邮箱是否接收到数据
	if ((ECanbRegs.CANRMP.all & u32MBoxIdx) == u32MBoxIdx)
	{

		//判断UPS机号
		m_u16UpsIdx = pMBox->MSGID.bit.STDMSGID;
		FrameID = (m_u16UpsIdx & MESSAGEID_MASK)>>6;
		m_u16UpsIdx &= UPSID_MASK;
		if((FrameID == MESSAGEID_IINVADQCURRENT)||(FrameID == MESSAGEID_IINVBDQCURRENT)||(FrameID == MESSAGEID_IINVCDQCURRENT)||(FrameID== MESSAGEID_INVALID_CURRENT)||(FrameID == MESSAGEID_ILFILTER))	//D,Q电流信息帧
		{
			//存放数据
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

			//By wxw20090627 Adapter15k~20K无旁路柜
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
		else if(FrameID== MESSAGEID_PLOAD)	//本机有功功率信息帧
		{
			//存放数据
			objPowCANSched.PowerCANParaLoadEntry.ParaLoadData[m_u16UpsIdx].m_i16Pouta = pMBox->MDL.word.LOW_WORD;
			objPowCANSched.PowerCANParaLoadEntry.ParaLoadData[m_u16UpsIdx].m_i16Poutb = pMBox->MDL.word.HI_WORD;
			objPowCANSched.PowerCANParaLoadEntry.ParaLoadData[m_u16UpsIdx].m_i16Poutc = pMBox->MDH.word.LOW_WORD;
		}
		else if(FrameID== MESSAGEID_SLOAD)	//本机视在功率信息帧
		{
			//存放数据
			objPowCANSched.PowerCANParaLoadEntry.ParaLoadData[m_u16UpsIdx].m_i16Souta = pMBox->MDL.word.LOW_WORD;
			objPowCANSched.PowerCANParaLoadEntry.ParaLoadData[m_u16UpsIdx].m_i16Soutb = pMBox->MDL.word.HI_WORD;
			objPowCANSched.PowerCANParaLoadEntry.ParaLoadData[m_u16UpsIdx].m_i16Soutc = pMBox->MDH.word.LOW_WORD;
		}
		else if(FrameID== MESSAGEID_STATE)	//电池及逆变状态帧
		{
			//存放数据
			objPowCANSched.PowerCANParaLoadEntry.SysFlag[m_u16UpsIdx].InvFlag.m_u16all= pMBox->MDL.word.LOW_WORD;
			objPowCANSched.PowerCANParaLoadEntry.SysFlag[m_u16UpsIdx].BattFlag.m_u16all= pMBox->MDL.word.HI_WORD;
		}
		
		//置相应UPS接收完成标志
		objPowCANSched.PowCanParaInfoEntry.MsgRecvFalg.m_u16All |= ( 1 << (m_u16UpsIdx));
		//释放邮箱，可重新接收新的数据
		ECanbRegs.CANRMP.all = u32MBoxIdx;          //可能会被下一个数据写覆盖读回一个错误的值，不过OPC寄存器未使能
	}
}
