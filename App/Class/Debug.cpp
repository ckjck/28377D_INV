/*==========================================================*/	
/* Title		:	Debug.cpp								*/	
/* Description	:	Class_Debug	definition					*/	
/* Date			:	Apr.2008								*/	
/*==========================================================*/
#include "Debug.h"


/************************************************************************************
��������	App_DebugVarInit()	
����������	Debug�������ʼ��		
�������ã�	
***********************************************************************************/
	 void	Class_Debug::App_DebugVarInit(void)
	{
			//zqf 060524--���ʾ�������ֱ�����ʼ��
		unSCIDebugFlag_0.BIT.Sending = 0;
		unSCIDebugFlag_0.BIT.HighorLow = 0;
		unSCIDebugFlag_0.BIT.Saving	= 1;
		unSCIDebugFlag_0.BIT.ConditionSaveEn=1;
		m_i16SumCheck_0	= 0;
		m_i16SCISendSignalCnt_0	= 0;
		m_i16SCISendPointCnt_0 = 0;
		m_i16SCISendDelay_0	= 0;
		m_i16SCISaveDelay_0	= 0;
		m_i32StopDelayCnt_0	= 0;
		pSCISavePoint_0	= iSCIDataBuffer_0;
		pSCISendPoint_0	= iSCIDataBuffer_0;
		m_i16Trop_0	= 0;
	}
/************************************************************************
��������:	App_Int_WaveRecord()	
��������:	ʵʱ���μ�¼����¼����Ϊ������ѹ�������ѹ����·��ѹ��
			�����������������ĸ�ߺͣ���¼����256*16=4k
���ڲ���ָ�	0x0a0--ֹͣ��������
				0x0b0--�������ݴ���
				0x0c0--���¿�ʼ���ݱ���
************************************************************************/  
 
#pragma CODE_SECTION(".Epwm1Funcs")			

void	Class_Debug::App_Int_WaveRecord( void )
{
	INT16	i16temp_0;
	
	//���ϴ��������ݱ��棬�����������Ը�����Ҫ����
	if(objInverter.m_st_wFlagInv.bInv!=0)
	{
		m_i32StopDelayCnt_0++;
//		if (m_i32StopDelayCnt_0>10)	m_i16Trop_0	= 1;
		if (m_i32StopDelayCnt_0>=350)										//ͻ���غ���ʱ900���ж����ں�ֹͣ��¼
		{
			m_i32StopDelayCnt_0	= 350;
			if (unSCIDebugFlag_0.BIT.ConditionSaveEn ==	1)
			{
				m_i16Trop_0	= 0;
				unSCIDebugFlag_0.BIT.Saving	= 0;
				unSCIDebugFlag_0.BIT.ConditionSaveEn = 0;					//������¼������
				pSCISendPoint_0	= pSCISavePoint_0;							//ȷ���������ݵĵ�ַ���Ӿɵ���
			}
		}
	}
		
	if(ScibRegs.SCIRXST.bit.RXRDY == 1)
	 {
		//SCIAͨ�Ŵ��������ݱ��桢���ݴ��͡����±�������
		i16temp_0 =	ScibRegs.SCIRXBUF.bit.SAR;

		switch ( i16temp_0 )
		{
			case 0x0a0:														//ֹͣ��������
					unSCIDebugFlag_0.BIT.Saving=0;
					pSCISendPoint_0=pSCISavePoint_0;						//ȷ���������ݵĵ�ַ���Ӿɵ���
					break;
			case 0x0b0:														//�������ݴ���
				if(unSCIDebugFlag_0.BIT.Saving	== 0)
				{
					unSCIDebugFlag_0.BIT.Sending = 1;		
					unSCIDebugFlag_0.BIT.HighorLow = 0;						//�ȴ������ݵĵ�λ�ֽ�
					m_i16SCISendDelay_0	= 0;
					m_i16SCISendPointCnt_0 = 0;
					m_i16SCISendSignalCnt_0	= 0;
					m_i16SumCheck_0	= 0;
				}
				break;
			case 0x0c0:														//���¿�ʼ��������
				if((unSCIDebugFlag_0.BIT.Saving	== 0)&&(unSCIDebugFlag_0.BIT.Sending ==	0))
				{
					pSCISavePoint_0	= iSCIDataBuffer_0;						//��ͷ��ʼ����
					unSCIDebugFlag_0.BIT.Saving	= 1;
					m_i32StopDelayCnt_0	= 0;
					m_i16Trop_0	= 0;
				}
				break;
			case 0x0d0:
				unSCIDebugFlag_0.BIT.ConditionSaveEn = 1;					//������¼����
				m_i32StopDelayCnt_0	= 0;
				m_i16Trop_0	= 0;
				break;
			case 0x0e0:
				unSCIDebugFlag_0.BIT.ConditionSaveEn = 0;					//������¼������
				m_i32StopDelayCnt_0	= 0;
				m_i16Trop_0	= 0;
				break;
			default:
				if(i16temp_0<50)
				{
					objMonInterface.m_i16_DegVar1 = i16temp_0;
				}
				break;
		} 
	 }

	m_i16SCISendDelay_0++;													//������ʱ
	if(m_i16SCISendDelay_0 > 40)  
	{
		m_i16SCISendDelay_0	 =0;
	}

	if((m_i16SCISendSignalCnt_0	== 4) && (unSCIDebugFlag_0.BIT.Sending == 0))
	{
		m_i16SCISendSignalCnt_0=0;
		ScibRegs.SCITXBUF.bit.TXDT=(m_i16SumCheck_0 & 0xFF);							//���ͼ���
	}

	if((unSCIDebugFlag_0.BIT.Sending ==	1)&&(m_i16SCISendDelay_0 ==	40))
	{

		if(unSCIDebugFlag_0.BIT.HighorLow == 0)								//�������ݵĵ�λ�ֽ�
		{
				ScibRegs.SCITXBUF.bit.TXDT =	((*(pSCISendPoint_0+(m_i16SCISendSignalCnt_0<<9))) & 0x0FF);
				m_i16SumCheck_0	+= ((*(pSCISendPoint_0+(m_i16SCISendSignalCnt_0<<9))) & 0x0FF);
				unSCIDebugFlag_0.BIT.HighorLow = 1;							 //�������ݵĸ�λ�ֽ�
		}
		else
		{
				ScibRegs.SCITXBUF.bit.TXDT =	(((*(pSCISendPoint_0+(m_i16SCISendSignalCnt_0<<9)))>>8) & 0x0FF);
				m_i16SumCheck_0	+= (((*(pSCISendPoint_0+(m_i16SCISendSignalCnt_0<<9)))>>8) & 0x0FF);

				unSCIDebugFlag_0.BIT.HighorLow=0;							//������һ���ֽ�����

				pSCISendPoint_0++;
				if(pSCISendPoint_0 >= iSCIDataBuffer_0+512)
				{
					pSCISendPoint_0=iSCIDataBuffer_0;
				}

				m_i16SCISendPointCnt_0++;
				if(m_i16SCISendPointCnt_0 >= 512)
				{
					m_i16SCISendPointCnt_0 = 0;
					pSCISendPoint_0	= pSCISavePoint_0;						//��һ���źŵ���ʼλ��
					m_i16SCISendSignalCnt_0++;
					if(m_i16SCISendSignalCnt_0 >= 4)
					{
						unSCIDebugFlag_0.BIT.Sending = 0;					//��������
					}
				}
		}

	 }

	if(unSCIDebugFlag_0.BIT.Saving == 1)									//ѭ����������
	{
		m_i16SCISaveDelay_0++;
//		if(m_i16SCISaveDelay_0 > 700)
//		if(objInvAlg.m_i16PLL2CroseZeroFlag_0 == 1)
		{
			m_i16SCISaveDelay_0	= 0;
            objDebug.App_DebugInf();
			pSCISavePoint_0++;
			if(pSCISavePoint_0 >= iSCIDataBuffer_0+512)
			{
				pSCISavePoint_0	= iSCIDataBuffer_0;							//�ռ䱣����֮���ͷ�ٱ���
			}
		 }
		
	}
}

void    Class_Debug::App_DebugInf(void)
{
switch(objMonInterface.m_i16_DegVar1)
    {
    case 0:
        {
        *pSCISavePoint_0        =   objInvAlg.m_i16SinQ0Ref_0;
        *(pSCISavePoint_0+512)  =   objInverter.m_i16Vinva_0;
        *(pSCISavePoint_0+1024) =   objInverter.m_i16Vinvb_0;
        *(pSCISavePoint_0+1536) =   objInverter.m_i16Vinvc_0;
        break;
        }
    case 1:
        {
        *pSCISavePoint_0        =   objDCBus.m_i16VbusP_0;
        *(pSCISavePoint_0+512)  =   objBypass.m_i16Vbpa_0;
        *(pSCISavePoint_0+1024) =   objBypass.m_i16Vbpb_0;
        *(pSCISavePoint_0+1536) =   objBypass.m_i16Vbpc_0;
        break;
        }
    case 2:
        {
        *pSCISavePoint_0        =   objInverter.m_i16Ila_0;
        *(pSCISavePoint_0+512)  =   objInverter.m_i16Ilb_0;
        *(pSCISavePoint_0+1024) =   objInverter.m_i16Ilc_0;
        *(pSCISavePoint_0+1536) =   objInverter.m_i16Vinva_0;
        break;
        }
    case 3:
        {
        *pSCISavePoint_0        =   objADDrv.m_i16ADC_Vinvb_0;
        *(pSCISavePoint_0+512)  =   objADDrv.m_i16ADC_Ila_0;
        *(pSCISavePoint_0+1024) =   objADDrv.m_i16ADC_Ilb_0;
        *(pSCISavePoint_0+1536) =   objADDrv.m_i16ADC_Ilc_0;
        break;
        }
    case 4:
        {
        *pSCISavePoint_0        =   objInverter.m_i16Vinva_0;
        *(pSCISavePoint_0+512)  =   objInverter.m_i16Vinvb_0;
        *(pSCISavePoint_0+1024) =   objInverter.m_i16Vinvc_0;
        *(pSCISavePoint_0+1536) =   objOutput.m_i16Vouta_0;
        break;
        }
    case 5:
        {
        *pSCISavePoint_0        =   objOutput.m_i16Vouta_0;
        *(pSCISavePoint_0+512)  =   objOutput.m_i16Voutb_0;
        *(pSCISavePoint_0+1024) =   objOutput.m_i16Voutc_0;
        *(pSCISavePoint_0+1536) =   objInverter.m_i16Vinva_0;
        break;
        }
    case 6:
        {
        *pSCISavePoint_0        =   objInverter.m_i16Vinva_0;
        *(pSCISavePoint_0+512)  =   objInverter.m_i16Ila_0;
        *(pSCISavePoint_0+1024) =   objInverter.m_i16Iinva_0;
        *(pSCISavePoint_0+1536) =   objOutput.m_i16Vouta_0;
        break;
        }
    case 7:
        {
        *pSCISavePoint_0        =   objDCBus.m_i16VbusP_0;
        *(pSCISavePoint_0+512)  =   objDCBus.m_i16VbusN_0;
        *(pSCISavePoint_0+1024) =   objInvAlg.m_i16SinQ0Ref_0;
        *(pSCISavePoint_0+1536) =   objInvAlg.m_i16SinQ0Ref1_0;
        break;
        }
    case 8:           
        {
        *pSCISavePoint_0        =   objInvAlg.m_i16SinQ0Ref1_0;
        *(pSCISavePoint_0+512)  =   objInverter.m_i16Vinva_0;
        *(pSCISavePoint_0+1024) =   objInverter.m_i16Vinvb_0;
        *(pSCISavePoint_0+1536) =   objInverter.m_i16Vinvc_0;
        break;
        }
    case 9:
        {
        *pSCISavePoint_0        =   objInverter.m_i16Vinva_0;
        *(pSCISavePoint_0+512)  =   objBypass.m_i16Vbpa_0;
        *(pSCISavePoint_0+1024) =   objBypass.m_i16Vbpb_0;
        *(pSCISavePoint_0+1536) =   objBypass.m_i16Vbpc_0;
        break;
        }
    case 10:   
        {
        *pSCISavePoint_0        =   objInverter.m_i16Vinva_0;
        *(pSCISavePoint_0+512)  =   objInverter.m_i16VinvaDc_0;
        *(pSCISavePoint_0+1024) =   objInverter.m_i16VinvbDc_0;
        *(pSCISavePoint_0+1536) =   objInverter.m_i16VinvcDc_0;
        break;
        }
    case 11:   
        {
        *pSCISavePoint_0        =   objInverter.m_i16Vinva_0;
        *(pSCISavePoint_0+512)  =   objInverter.m_i16IlaFilter_0;
        *(pSCISavePoint_0+1024) =   objInverter.m_i16IlbFilter_0;
        *(pSCISavePoint_0+1536) =   objInverter.m_i16IlcFilter_0;
        break;
        }
    case 12:  
        {
        *pSCISavePoint_0        =   objInverter.m_i16Iinva_0;
        *(pSCISavePoint_0+512)  =   objInverter.m_i16IinvaP_0;
        *(pSCISavePoint_0+1024) =   objInverter.m_i16IinvaQ_0;
        *(pSCISavePoint_0+1536) =   objInverter.m_i16Vinva_0;
        break;
        }
    case 13:
        {
        *pSCISavePoint_0        =   objInverter.m_i16Iinvb_0;
        *(pSCISavePoint_0+512)  =   objInverter.m_i16IinvbP_0;
        *(pSCISavePoint_0+1024) =   objInverter.m_i16IinvbQ_0;
        *(pSCISavePoint_0+1536) =   objInverter.m_i16Vinvb_0;
        break;
        }
    case 14:
        {
        *pSCISavePoint_0        =   objInverter.m_i16Iinvc_0;
        *(pSCISavePoint_0+512)  =   objInverter.m_i16IinvcP_0;
        *(pSCISavePoint_0+1024) =   objInverter.m_i16IinvcQ_0;
        *(pSCISavePoint_0+1536) =   objInverter.m_i16Vinvc_0;
        break;
        }
    case 15:   
        {
        *pSCISavePoint_0        =   objInverter.m_i16Vinva_0;
        *(pSCISavePoint_0+512)  =   objInvAlg.m_i16wUirD_0;
        *(pSCISavePoint_0+1024) =   objInvAlg.m_i16wUirQ_0;
        *(pSCISavePoint_0+1536) =   objInvAlg.m_i16wUirZ_0;
        break;
        }
    case 16:   
        {
        *pSCISavePoint_0        =   objInverter.m_i16Vinva_0;
        *(pSCISavePoint_0+512)  =   objInvAlg.m_i16IrefMd_0;
        *(pSCISavePoint_0+1024) =   objInvAlg.m_i16IrefMq_0;
        *(pSCISavePoint_0+1536) =   objInvAlg.m_i16IrefMz_0;
        break;
        }
    case 17:
        {
        *pSCISavePoint_0        =   objBypass.m_i16Vbpa_0;
        *(pSCISavePoint_0+512)  =   objBypass.m_i16Vbpb_0;
        *(pSCISavePoint_0+1024) =   objBypass.m_i16Vbpc_0;
        *(pSCISavePoint_0+1536) =   objInverter.m_i16Vinva_0;
        break;
        }
    case 18:
        {
        *pSCISavePoint_0        =   objInverter.m_i16Vinva_0;
        *(pSCISavePoint_0+512)  =   objInvAlg.m_i16IlaPAvg_0;
        *(pSCISavePoint_0+1024) =   objInvAlg.m_i16IlaPShare_0;
        *(pSCISavePoint_0+1536) =   objInvAlg.m_i16IlaPShareReg_0;
        break;
        }
    case 19:
        {
        *pSCISavePoint_0        =   objInverter.m_i16Vinvb_0;
        *(pSCISavePoint_0+512)  =   objInvAlg.m_i16IlbPAvg_0;
        *(pSCISavePoint_0+1024) =   objInvAlg.m_i16IlbPShare_0;
        *(pSCISavePoint_0+1536) =   objInvAlg.m_i16IlbPShareReg_0;
        break;
        }
    case 20: 
        {
        *pSCISavePoint_0        =   objInverter.m_i16Vinvc_0;
        *(pSCISavePoint_0+512)  =   objInvAlg.m_i16IlcPAvg_0;
        *(pSCISavePoint_0+1024) =   objInvAlg.m_i16IlcPShare_0;
        *(pSCISavePoint_0+1536) =   objInvAlg.m_i16IlcPShareReg_0;
        break;
        }
    case 21:
        {
        *pSCISavePoint_0        =   objInverter.m_i16Vinva_0;
        *(pSCISavePoint_0+512)  =   objInvAlg.m_i16IlaQAvg_0;
        *(pSCISavePoint_0+1024) =   objInvAlg.m_i16IlaQShare_0;
        *(pSCISavePoint_0+1536) =   objInvAlg.m_i16IlaQShareReg_0;
        break;
        }
    case 22: 
        {
        *pSCISavePoint_0        =   objInverter.m_i16Vinvb_0;
        *(pSCISavePoint_0+512)  =   objInvAlg.m_i16IlbQAvg_0;
        *(pSCISavePoint_0+1024) =   objInvAlg.m_i16IlbQShare_0;
        *(pSCISavePoint_0+1536) =   objInvAlg.m_i16IlcQShareReg_0;
        break;
        }
    case 23:  
        {
        *pSCISavePoint_0        =   objInverter.m_i16Vinvc_0;
        *(pSCISavePoint_0+512)  =   objInvAlg.m_i16IlcQAvg_0;
        *(pSCISavePoint_0+1024) =   objInvAlg.m_i16IlcQShare_0;
        *(pSCISavePoint_0+1536) =   objInvAlg.m_i16IlcQShareReg_0;
        break;
        }
    case 24:  
        {
        *pSCISavePoint_0        =   objInverter.m_i16Vinva_0;
        *(pSCISavePoint_0+512)  =   objInvAlg.m_i16IlaFilterAvg_0;
        *(pSCISavePoint_0+1024) =   objInvAlg.m_i16IlaFilterShare_0;
        *(pSCISavePoint_0+1536) =   objInvAlg.m_i16IlaRegOut_0;
        break;
        }
    case 25:
        {
        *pSCISavePoint_0        =   objInverter.m_i16Vinvb_0;
        *(pSCISavePoint_0+512)  =   objInvAlg.m_i16IlbFilterAvg_0;
        *(pSCISavePoint_0+1024) =   objInvAlg.m_i16IlbFilterShare_0;
        *(pSCISavePoint_0+1536) =   objInvAlg.m_i16IlbRegOut_0;
        break;
        }
    case 26:
        {
        *pSCISavePoint_0        =   objInverter.m_i16Vinvc_0;
        *(pSCISavePoint_0+512)  =   objInvAlg.m_i16IlcFilterAvg_0;
        *(pSCISavePoint_0+1024) =   objInvAlg.m_i16IlcFilterShare_0;
        *(pSCISavePoint_0+1536) =   objInvAlg.m_i16IlcRegOut_0;
        break;
        }
    case 27:
        {
        *pSCISavePoint_0        =   objInverter.m_i16Vinva_0;
        *(pSCISavePoint_0+512)  =   objDCBus.m_i16VbusP_0;
        *(pSCISavePoint_0+1024) =   objDCBus.m_i16VbusN_0;
        *(pSCISavePoint_0+1536) =   objInverter.m_i16VbusDiff_0;
        break;
        }
    case 28: 
        {
        *pSCISavePoint_0        =   objInverter.m_i16Vinva_0;
        *(pSCISavePoint_0+512)  =   objInverter.m_i16VinvaDc_0;;
        *(pSCISavePoint_0+1024) =   objInvAlg.m_i16VdcaCon_0;
        *(pSCISavePoint_0+1536) =   objInverter.m_i16Ila_0;
        break;
        }
    case 29:
        {
        *pSCISavePoint_0        =   objInverter.m_i16Vinvb_0;
        *(pSCISavePoint_0+512)  =   objInverter.m_i16VinvbDc_0;;
        *(pSCISavePoint_0+1024) =   objInvAlg.m_i16VdcbCon_0;
        *(pSCISavePoint_0+1536) =   objInverter.m_i16Ilb_0;
        break;
        }
    case 30:
        {
        *pSCISavePoint_0        =   objInverter.m_i16Vinvc_0;
        *(pSCISavePoint_0+512)  =   objInverter.m_i16VinvcDc_0;
        *(pSCISavePoint_0+1024) =   objInvAlg.m_i16VdccCon_0;
        *(pSCISavePoint_0+1536) =   objInverter.m_i16Ilc_0;
        break;
        }  
    case 31:
        {
        *pSCISavePoint_0        =   objInverter.m_i16VinvMd_0;
        *(pSCISavePoint_0+512)   =   objInvAlg.m_unIntgDVr_0.half.hword;
        *(pSCISavePoint_0+1024)  =   objInvAlg.m_i16IrefMd_0;
        *(pSCISavePoint_0+1536)  =   objInvAlg.m_i16wUirD_0;
        break;
        }
    case 32:
        {
        *pSCISavePoint_0        =   objInverter.m_i16VinvMq_0;
        *(pSCISavePoint_0+512)   =   objInvAlg.m_unIntgQVr_0.half.hword;
        *(pSCISavePoint_0+1024)  =   objInvAlg.m_i16IrefMq_0;
        *(pSCISavePoint_0+1536)  =   objInvAlg.m_i16wUirQ_0;
        break;
        }
    case 33:
        {
        *pSCISavePoint_0        =   objInverter.m_i16VinvMz_0;
        *(pSCISavePoint_0+512)   =   objInvAlg.m_unIntgZVr_0.half.hword;
        *(pSCISavePoint_0+1024)  =   objInvAlg.m_i16IrefMz_0;
        *(pSCISavePoint_0+1536)  =   objInvAlg.m_i16wUirZ_0;
        break;
        }
    case 34:
        {
        *pSCISavePoint_0        =   objInverter.m_i16VinvMd_0;
        *(pSCISavePoint_0+512)  =   objInvAlg.m_i16VaAct_0;
        *(pSCISavePoint_0+1024) =   objInvAlg.m_i16VbAct_0;
        *(pSCISavePoint_0+1536) =   objInvAlg.m_i16VcAct_0;
        break;
        }
    case 35:
        {
        *pSCISavePoint_0        =   objInvAlg.m_i16VshareMd_0;
        *(pSCISavePoint_0+512)  =   objInvAlg.m_i16VshareMq_0;
        *(pSCISavePoint_0+1024) =   objInvAlg.m_i16VshareMz_0;
        *(pSCISavePoint_0+1536) =   objInverter.m_i16VinvMd_0;
        break;
        }
    case 36:
        {
        *pSCISavePoint_0        =   objInvAlg.m_i16VshareMd_0;
        *(pSCISavePoint_0+512)  =   objInvAlg.m_i16VrefDroop_0;
        *(pSCISavePoint_0+1024) =   objInvAlg.m_i16VrefCmpn_0;
        *(pSCISavePoint_0+1536) =   objInverter.m_i16VinvMd_0;
        break;
        }
    case 37:
        {
        *pSCISavePoint_0        =   objPowCANSched.PowCanParaInfoEntry.ParaDQCurrEntry[objMonInterface.m_i16_DegVar2].m_i16IINVAP;
        *(pSCISavePoint_0+512)  =   objPowCANSched.PowCanParaInfoEntry.ParaDQCurrEntry[objMonInterface.m_i16_DegVar2].m_i16IINVBP;
        *(pSCISavePoint_0+1024) =   objPowCANSched.PowCanParaInfoEntry.ParaDQCurrEntry[objMonInterface.m_i16_DegVar2].m_i16IINVCP;
        *(pSCISavePoint_0+1536) =   objInverter.m_i16Vinva_0;
        break;
        }
    case 38:
        {
        *pSCISavePoint_0        =   objPowCANSched.PowCanParaInfoEntry.ParaDQCurrEntry[objMonInterface.m_i16_DegVar2].m_i16IINVAQ;
        *(pSCISavePoint_0+512)  =   objPowCANSched.PowCanParaInfoEntry.ParaDQCurrEntry[objMonInterface.m_i16_DegVar2].m_i16IINVBQ;
        *(pSCISavePoint_0+1024) =   objPowCANSched.PowCanParaInfoEntry.ParaDQCurrEntry[objMonInterface.m_i16_DegVar2].m_i16IINVCQ;
        *(pSCISavePoint_0+1536) =   objInverter.m_i16Vinva_0;
        break;
        }
    case 39:
        {
        *pSCISavePoint_0        =   objPowCANSched.PowCanParaInfoEntry.IlFilter[objMonInterface.m_i16_DegVar2].m_i16IlFiltera;
        *(pSCISavePoint_0+512)  =   objPowCANSched.PowCanParaInfoEntry.IlFilter[objMonInterface.m_i16_DegVar2].m_i16IlFilterb;
        *(pSCISavePoint_0+1024) =   objPowCANSched.PowCanParaInfoEntry.IlFilter[objMonInterface.m_i16_DegVar2].m_i16IlFilterc;
        *(pSCISavePoint_0+1536) =   objInverter.m_i16Vinva_0;
        break;
        }
    
    default:
        {
        *pSCISavePoint_0        =   objInverter.m_i16Iinva_0;
        *(pSCISavePoint_0+512)  =   objInverter.m_i16Iinvb_0;
        *(pSCISavePoint_0+1024) =   objInverter.m_i16Iinvc_0;
        *(pSCISavePoint_0+1536) =   objInverter.m_i16Vinva_0;
        break;
        }
    }

}
//===========================================================================
// End of file.
//===========================================================================
