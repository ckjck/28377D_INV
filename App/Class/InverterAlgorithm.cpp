/*==========================================================*/
/* Title		:	InverterAlgorithm.cpp											*/
/* Description	: 	Class_InverterAlgorithm definition								*/
/* Date		:	Apr.2008													*/
/*==========================================================*/
#include "InverterAlgorithm.h"

/************************************************************************************
函数名：Alg_Int_MainInterfaceVar()
************************************************************************************/
inline	void	Class_InverterAlgorithm::Alg_Int_MainInterfaceVar(void)
	{
		m_i16wVrefActMd_0 = m_i16wVrefRunMd_0;

	}
/************************************************************************************
函数名：App_InvAlgParamConfig()
************************************************************************************/
	void	Class_InverterAlgorithm::App_InvAlgParamConfig(void)
	{
		//输出频率设置映射
		if(objMonInterface.m_st_wUPSModeRun.bFreqOut==0)				//50Hz
		{
			switch(objMonInterface.m_i16FbpTraceSet)
			{
				case 0:							//+-2Hz
					m_i16FreqSetMax_0 = 1603;		//52.2Hz		360*52.2*1024/fpwm
					m_i16FreqSetMin_0 = 1468;		//47.8Hz
					break;
				case 1:							//+-1Hz
					m_i16FreqSetMax_0 = 1573;		//51.2Hz
					m_i16FreqSetMin_0 = 1499;		//48.8Hz
					break;
				case 2:							//+-3Hz
					m_i16FreqSetMax_0 = 1634;		//53.2Hz
					m_i16FreqSetMin_0 = 1438;		//46.8Hz
					break;
				case 3:							//+-0.5Hz
					m_i16FreqSetMax_0 = 2077;		//50.7Hz
					m_i16FreqSetMin_0 = 1557;		//49.3Hz
					break;
				default:
					break;
			}
		}
		else									//60Hz
		{
			switch(objMonInterface.m_i16FbpTraceSet)
			{
				case 0:							//+-2Hz
					m_i16FreqSetMax_0 = 1911;		//62.2Hz
					m_i16FreqSetMin_0 = 1775;		//57.8Hz
					break;
				case 1:							//+-1Hz
					m_i16FreqSetMax_0 = 1880;		//61.2Hz
					m_i16FreqSetMin_0 = 1806;		//58.8Hz
					break;
				case 2:							//+-3Hz
					m_i16FreqSetMax_0 = 1942;		//63.2Hz
					m_i16FreqSetMin_0 = 1774;		//56.8Hz
					break;
				case 3:							//+-0.5Hz
					m_i16FreqSetMax_0 = 1865;		//60.7Hz
					m_i16FreqSetMin_0 = 1821;		//59.3Hz
					break;
				default:
					break;
			}
		}

		//输出频率跟踪速率映射
		if((objMonInterface.m_st_wUPSModeRun.bConfig==3)||(objMonInterface.m_st_wUPSModeRun.bLBS !=0))
		{
			//m_i16ThetaStepBp_0 = (KThetaStepBpCnst*2)+5;		// 并机跟踪速率0.2Hz/s
			//m_i16DthetaBpPLLLmt_0 = 1835;						// 0.175Hz(360*0.175*1024*2^8/fpwm) 跟踪旁路时锁相部分输出频率限幅
			m_i16ThetaStepBp_0 = objMonInterface.m_i16FreqSlewRate*KThetaStepBpCnst;// 单机跟踪速率
			m_i16DthetaBpPLLLmt_0 = 590*(1+objMonInterface.m_i16FreqSlewRate);		// (0.05+iwFreqSlewRate/2)<<8
			UPDNLMT16(m_i16DthetaBpPLLLmt_0,11010,1573);		// max=1.05Hz(360*1.05*1024*2^8/fpwm)
															// min=0.15Hz(360*0.15*1024*2^8/fpwm)
		}
		else
		{
			m_i16ThetaStepBp_0 = objMonInterface.m_i16FreqSlewRate*KThetaStepBpCnst;// 单机跟踪速率
			m_i16DthetaBpPLLLmt_0 = 590*(1+objMonInterface.m_i16FreqSlewRate);		// (0.05+iwFreqSlewRate/2)<<8
			UPDNLMT16(m_i16DthetaBpPLLLmt_0,11010,1573);		// max=1.05Hz(360*1.05*1024*2^8/fpwm)
															// min=0.15Hz(360*0.15*1024*2^8/fpwm)
		}
		
		//SPWM发波控制系数设置
		switch(objMonInterface.m_st_wUPSModeRun.bVoltOut)
			{
				case 0:											//400V
					m_i16KSPWM = KSPWMCnst2;
					break;
				case 1:											//380v
					m_i16KSPWM = KSPWMCnst1;	
					break;
				case 2:											//415v
					m_i16KSPWM = KSPWMCnst3;
					break;

				default:
					break;
	}
}

/************************************************************************************
函数名：	Alg_Int_LockStatus()
功能描述：	相位跟踪精度判断，根据逆变电压与旁路电压（或输出电压）的相位
			  关系实时判断其相位锁定状态，为输出切换的模式选择提供依据
函数调用：	无
************************************************************************************/

	#pragma CODE_SECTION(".Epwm1Funcs")

	void Class_InverterAlgorithm::Alg_Int_LockStatus(void)
	{
		INT16   i16IntAx_0;
		INT16	i16IntBx_0;
		
		i16IntAx_0 = abs(m_i16DphaseInst_0>>3);									//相差取绝对值，克服旁路为方波时正、	负相差互相抵消的情况
		m_unDphaseMean_0.dword = FILTER(6, i16IntAx_0, m_unDphaseMean_0.dword);	//滤波常数  2**7=128 相当于1个工频周期

		m_i16DphaseBPInst_0 = (((INT32)objBypass.m_i16SinQBp_0)*objInverter.m_i16CosQInv_0-((INT32)objInverter.m_i16SinQInv_0)*objBypass.m_i16CosQBp_0)>>14;//计算逆变与旁路之间相位差
		i16IntAx_0 = abs(m_i16DphaseBPInst_0);
		m_unDphaseBPMean_0.dword = FILTER(6, i16IntAx_0, m_unDphaseBPMean_0.dword);

		m_i16CosDphaseBP_0 = (((INT32)objBypass.m_i16CosQBp_0)*objInverter.m_i16CosQInv_0+((INT32)objInverter.m_i16SinQInv_0)*objBypass.m_i16SinQBp_0)>>14;

		if(objInverter.m_st_wFlagInv.bPhaseTrace >= 2)
		{
			i16IntBx_0 = 100;
		}
		else
		{
			i16IntBx_0 = 498;
		}
		if(objSystem.m_st_wFlagSystem.bSelfAgingAction!=1)
		{	
			if((m_st_wIntMainFlag.bVpwmCrossZero == 1) && (objInverter.m_st_wFlagInv.bInv == 3))
			{
				//判断与旁路的锁相标志
				if ((objInverter.m_st_wFlagInv.bPhaseTrace == 1)||((objInverter.m_st_wFlagInv.bPhaseTrace >= 2) && (objXfer.m_st_wXferCmd.bSwitchStatus == 1)))
				{
					if(((m_i16CosDphase_0 > 512) && (m_unDphaseMean_0.half.hword < 108))//512/2**10=0.5, 6/180*PI*2**10=108
					&&(abs(objBypass.m_unDthetaBpBase_0.half.hword-m_unPLL1DthetaRefStd_0.half.hword) < KDthetaDiffCnst))
					{
						if(m_i16BpPhaseLockCnt_0 < 500)	
						{
							m_i16BpPhaseLockCnt_0 ++;
						}
					}
					else
					{
						if(m_i16BpPhaseLockCnt_0 > 0)
						{		
							m_i16BpPhaseLockCnt_0 --;
						}
					}
					
					if(m_i16BpPhaseLockCnt_0 >= 500)
					{
						m_st_wIntMainFlag1_0.bBpPhaseLock = 1;
					}
					else if(m_i16BpPhaseLockCnt_0 <= i16IntBx_0)
					{
						m_st_wIntMainFlag1_0.bBpPhaseLock = 0;
					}
				}
				else
				{
					m_i16BpPhaseLockCnt_0 = 0;
					m_st_wIntMainFlag1_0.bBpPhaseLock = 0;
				}			
    	
				//从机跟踪工频同步或旁路的锁相标志
				if (((objInverter.m_st_wFlagInv.bPLL2PhaseTrace == 1) && (m_i16DphaseInvSync_0 < 214))||				//3/180*PI*2**12=214
					((objInverter.m_st_wFlagInv.bPLL2PhaseTrace == 0) && ((m_i16CosDphase_0 > 512) && (m_unDphaseMean_0.half.hword  < 108)) ))	//512/2**10=0.5, 6/180*PI*2**10=108
				{
					if(m_i16SlaverPhaseLock_0 < 500)
					{
						m_i16SlaverPhaseLock_0 ++;
					}
				}
				else
				{
					if(m_i16SlaverPhaseLock_0 > 0)
					{
						m_i16SlaverPhaseLock_0--;
					}
				}

				if(m_i16SlaverPhaseLock_0 >= 500)
				{
					m_st_wIntMainFlag1_0.bSlaverPhaseLock = 1;
				}
				else if(m_i16SlaverPhaseLock_0 <= 498)
				{
					m_st_wIntMainFlag1_0.bSlaverPhaseLock = 0;
				}

				//锁相标志的综合
				if (objParallel.m_st_wParaSignalBak.bMasterOnline == 1)				//主机
				{
					m_st_wIntMainFlag.bPhaseLock = m_st_wIntMainFlag1_0.bBpPhaseLock;
				}
				else																//从机
				{
					m_st_wIntMainFlag.bPhaseLock = m_st_wIntMainFlag1_0.bSlaverPhaseLock;
				}
			}
			else if(objInverter.m_st_wFlagInv.bInv != 3)
			{
				m_i16BpPhaseLockCnt_0 = 0;
				m_i16SlaverPhaseLock_0 = 0;
				m_st_wIntMainFlag1_0.bBpPhaseLock = 0;
				m_st_wIntMainFlag1_0.bSlaverPhaseLock = 0;
				m_st_wIntMainFlag.bPhaseLock = 0;
			}
		}
	}




/************************************************************************************
函数名：	Alg_Int_MasterComm()
功能描述：	本模块实现并机之间通讯主机的竞争
算法说明： 	并机中的各机根据本机编号在MASTER_COMM总线上发送一定脉宽的高电平，
			并侦听脉冲宽度，若在本机范围内经两次确认后置本机为主机，否则为从机。
函数调用：	无
************************************************************************************/

#pragma CODE_SECTION(".Epwm1Funcs");

	inline	void	Class_InverterAlgorithm::Alg_Int_MasterComm(void)
	{
		m_st_wIntMainFlag.bCANMaster = 1;
	}

/************************************************************************************
函数名称：    App_InvAlgVarInit()
功能描述：    InvAlg类变量初始化模块（供主程序调用）
***********************************************************************************/
void 	Class_InverterAlgorithm::App_InvAlgVarInit(void)
{
	//输出跟踪速率控制
	m_i16ThetaStepBp_0 = KThetaStepBpCnst;

	//m_i16KpVolt_0 = 2293;
	m_i16KpVolt_0 = 1638;
	//m_i16KiVolt_0 = 380;
	m_i16KiVolt_0 = 760;
	m_i16KpCurr_0 = 330;
	m_i16KpVoltDyn_0=4300;							//外环动态比例系数
	m_i16wKRptConLmt_0 =80;

	m_i16FreqSetMax_0 = K54HzDthetaCnst;	             // 54Hz
	m_i16FreqSetMin_0 = K46HzDthetaCnst;	             // 46Hz
	m_i16KSPWM	= KSPWMCnst1;

	m_i16Zc_0 = 0;
	m_i16Zl_0 = 0;

	m_i16RptCmpnD_0 = 0;
	m_i16RptCmpnQ_0 = 0;
	m_i16RptSite_0 = 0;
	m_i16RptSiteSav_0 = 0;
	objInverter.m_i16PointcntInvM_0 = K50HzPointCntCnst;
	m_i16KRptCon_0 = 80;
	m_i16invKM_CNT_0 = 0;

	m_i16DthetaRefInst_0 = K50HzDthetaCnst;
	m_unDthetaRefStd_0.half.hword = K50HzDthetaCnst;
	m_unDthetaRefStd_0.half.lword = 0;

	m_i16PLL1DthetaRefInst_0 = K50HzDthetaCnst;
	m_unPLL1DthetaRefStd_0.half.hword = K50HzDthetaCnst;
	m_unPLL1DthetaRefStd_0.half.lword = 0;

	m_i16PLL2CroseZeroFlag_0 = 0;

	m_i16KpACShare = 128;
	m_i16KiACShare = 20;

}
/************************************************************************************
//函数名称：    Alg_Int_InvOffReset()
//功能描述：    逆变器关闭时对一些参数赋值，防止再次开机时出现超调
************************************************************************************/

//#pragma CODE_SECTION(".Epwm1Funcs")

void 	Class_InverterAlgorithm::Alg_Int_InvOffReset(void)
{
	m_i16PulseOffCount++;

	m_st_wIntMainFlag.bVpwmCrossZero = 0;								// 逆变过零

	if(objMonInterface.m_st_wUPSModeRun.bFreqOut == 0)				     // 50Hz体制
	{
		m_i16DthetaRefInst_0 = K50HzDthetaCnst;
		m_unDthetaRefStd_0.half.hword = K50HzDthetaCnst;
		m_unDthetaRefStd_0.half.lword = 0;

		m_i16PLL1DthetaRefInst_0 = K50HzDthetaCnst;
		m_unPLL1DthetaRefStd_0.half.hword = K50HzDthetaCnst;
		m_unPLL1DthetaRefStd_0.half.lword = 0;
	}
	else																 // 60Hz体制
	{
		m_i16DthetaRefInst_0 = K60HzDthetaCnst;
		m_unDthetaRefStd_0.half.hword = K60HzDthetaCnst;
		m_unDthetaRefStd_0.half.lword = 0;

		m_i16PLL1DthetaRefInst_0 = K60HzDthetaCnst;
		m_unPLL1DthetaRefStd_0.half.hword = K60HzDthetaCnst;
		m_unPLL1DthetaRefStd_0.half.lword = 0;

	}

	m_unIntgDVr_0.dword = 0;
	m_unIntgQVr_0.dword = 0;
	m_unIntgZVr_0.dword = 0;
	m_unVrefIRegOut_0.dword = 0;
	m_unThetaIRegOut_0.dword = 0;
	m_unCirZIRegOut_0.dword = 0;


	unIarefFilter_0.dword = 0;
	unIbrefFilter_0.dword = 0;
	unIcrefFilter_0.dword = 0;

	m_i16wVrefActMd_0 = 0;

	m_unThetaRef_0.dword = 0;
	objPowCANSched.m_i16PowerCanTotalNum = 0;

	if(m_i16PulseOffCount > 3)
	{
		m_i16PulseOffCount = 3;
		EPwm1Regs.CMPA.bit.CMPA = 0;						//A相管脉冲
		EPwm1Regs.CMPB.bit.CMPB = 0;									//B相管脉冲
		EPwm2Regs.CMPA.bit.CMPA = 0;						//C相管脉冲

		GpioDataRegs.GPACLEAR.bit.GPIO17=1;					//A相正半周
		GpioDataRegs.GPACLEAR.bit.GPIO18=1;					//B相正半周
		GpioDataRegs.GPACLEAR.bit.GPIO19=1;					//C相正半周
	}

	m_unVshareMdFilt_0.dword= 0;
	m_unVshareMdFilt2_0.dword = 0;
	m_unVerrMdFilt_0.dword = 0;
	m_unVerrMqFilt_0.dword = 0;
	m_unVerrMzFilt_0.dword = 0;

}

/************************************************************************************
//函数名称：    Alg_Int_RptCtrl()
//功能描述：    利用重复控制原理，克服电压畸变
************************************************************************************/

#pragma CODE_SECTION(".Epwm1Funcs")

void 	Class_InverterAlgorithm::Alg_Int_RptCtrl(void)
{


	UINT16	i;

	INT16 i16IntCx_0;
	//debugonly INT16	i16IntAx_0,i16IntBx_0;
	//debugonly INT16	i16IntCx_0,i16IntDx_0,i16IntEx_0;
	/*i16IntAx_0 = (((INT32)objInverter.m_i16VaNoDc_0*2-objInverter.m_i16VbNoDc_0-objInverter.m_i16VcNoDc_0)*K1div3Cnst)>>12;
	i16IntBx_0 = (((INT32)objInverter.m_i16VbNoDc_0-objInverter.m_i16VcNoDc_0)*KSqrt3invCnst)>>12;
	      
	i16IntCx_0 = ((INT32)i16IntBx_0*objInvAlg.m_i16SinQ0Ref_0+(INT32)i16IntAx_0*objInvAlg.m_i16CosQ0Ref_0)>>12;
	i16IntDx_0 = ((INT32)i16IntBx_0*objInvAlg.m_i16CosQ0Ref_0-(INT32)i16IntAx_0*objInvAlg.m_i16SinQ0Ref_0)>>12;
	i16IntEx_0 = (((INT32)objInverter.m_i16VaNoDc_0+objInverter.m_i16VbNoDc_0+objInverter.m_i16VcNoDc_0)*KSqrt2Div3Cnst)>>12;*/
	m_unVshareMdFilt2_0.dword = FILTER(7, m_i16VshareMd_0, m_unVshareMdFilt2_0.dword); 
//  	m_unVshareMdFilt_0.dword = FILTER(7, m_unVshareMdFilt2_0.half.hword, m_unVshareMdFilt_0.dword);    // 重复控制均流调节量滤波
	m_i16VerrMd_0 = m_i16wVrefRunMd_0+m_unVshareMdFilt2_0.half.hword-objInverter.m_i16VinvMd_0;
    UPDNLMT16(m_i16VerrMd_0,1000,-1000);

	m_i16VerrMq_0 = m_i16VshareMq_0 - objInverter.m_i16VinvMq_0;
	UPDNLMT16(m_i16VerrMq_0,1000,-1000);

	//m_unVshareMzFilt2_0.dword = FILTER(7, m_i16VrefActMz_0, m_unVshareMzFilt2_0.dword); 
   // m_unVshareMzFilt_0.dword = FILTER(7, m_unVshareMzFilt2_0.half.hword, m_unVshareMzFilt_0.dword);    // 重复控制均流调节量滤波
    m_i16VerrMz_0 = m_i16VrefActMz_0-objInverter.m_i16VinvMz_0;
    UPDNLMT16(m_i16VerrMz_0,1000,-1000);

    m_unVerrMdFilt_0.dword = FILTER(1, m_i16VerrMd_0, m_unVerrMdFilt_0.dword);    		// 重复控制误差滤波
    m_unVerrMqFilt_0.dword = FILTER(1, m_i16VerrMq_0, m_unVerrMqFilt_0.dword);
    m_unVerrMzFilt_0.dword = FILTER(1, m_i16VerrMz_0, m_unVerrMzFilt_0.dword);
    m_i16VerrMd_0 = m_unVerrMdFilt_0.half.hword;
    m_i16VerrMq_0 = m_unVerrMqFilt_0.half.hword;
    m_i16VerrMz_0 = m_unVerrMzFilt_0.half.hword;

	if(abs(m_i16VerrMd_0) >= 1000)
	{
		m_i16VerrMd_0 = 0;
	}
	if(abs(m_i16VerrMq_0) >= 1000)
	{
		m_i16VerrMq_0 = 0;
	}
	if(abs(m_i16VerrMz_0) >= 1000)
	{
		m_i16VerrMz_0 = 0;
	}

	if(objInvCtrl.m_i16InvSSCntPer40ms >= 75)
	{
		if(m_i16RptClearCnt_0 >= (K45HzPointCntCnst-1))
		{
			/*m_unPointcntInvMF_0.dword = FILTER(8, objInverter.m_i16PointcntInvM_0, m_unPointcntInvMF_0.dword);
	        i16IntCx_0 = m_unPointcntInvMF_0.half.lword>>1;
	        m_unPointcntInvMFLF_0.dword = FILTER(10, i16IntCx_0, m_unPointcntInvMFLF_0.dword);

			m_unRptTemp_0.dword = m_unPointcntInvMFLF_0.half.hword;
			m_unRptTemp_0.dword = (m_unRptTemp_0.dword<<1)+0x8000;
			i16IntCx_0 = m_unPointcntInvMF_0.half.hword+m_unRptTemp_0.half.hword;// 重复控制点数四舍五入
			*/
			i16IntCx_0 = objInverter.m_i16PointcntInvMDtheta;

			m_i16RptSiteSav_0 = m_i16RptSite_0-6;
			if(m_i16RptSiteSav_0 < 0)
			{
				m_i16RptSiteSav_0 = i16IntCx_0+m_i16RptSiteSav_0;
			}

			// 重复控制积分计算
			i16RptIntD_0[m_i16RptSiteSav_0] = ((((INT32)i16RptIntD_0[m_i16RptSiteSav_0])*4014)>>12)+m_i16VerrMd_0;
			i16RptIntQ_0[m_i16RptSiteSav_0] = ((((INT32)i16RptIntQ_0[m_i16RptSiteSav_0])*4014)>>12)+m_i16VerrMq_0;
			i16RptIntZ_0[m_i16RptSiteSav_0] = ((((INT32)i16RptIntZ_0[m_i16RptSiteSav_0])*4014)>>12)+m_i16VerrMz_0;
			UPDNLMT16(i16RptIntD_0[m_i16RptSiteSav_0],0x6000,-0x6000);
			UPDNLMT16(i16RptIntQ_0[m_i16RptSiteSav_0],0x6000,-0x6000);
			UPDNLMT16(i16RptIntZ_0[m_i16RptSiteSav_0],0x6000,-0x6000);

			
			if ( (m_i16RptSiteSav_0 >= (i16IntCx_0-1)) && (m_i16RptSiteSav_0 >= K66HzPointCntCnst))
			{
				for (i=(m_i16RptSiteSav_0+1); i<(K45HzPointCntCnst-1); i++)
				{
					i16RptIntD_0[i] = i16RptIntD_0[m_i16RptSiteSav_0];
					i16RptIntQ_0[i] = i16RptIntQ_0[m_i16RptSiteSav_0];
					i16RptIntZ_0[i] = i16RptIntZ_0[m_i16RptSiteSav_0];
				}
			}

			// 重复控制补偿量计算
			m_i16RptCmpnD_0 = i16RptIntD_0[m_i16RptSite_0];
			m_i16RptCmpnQ_0 = i16RptIntQ_0[m_i16RptSite_0];
			m_i16RptCmpnZ_0 = i16RptIntZ_0[m_i16RptSite_0];

			// 重复控制指针维护
			m_i16RptSite_0++;
			if(m_i16RptSite_0 > K45HzPointCntCnst)
			{
				m_i16RptSite_0 = K45HzPointCntCnst;
			}
			//if(m_st_wIntMainFlag.bVpwmCrossZero == 1)

			if(m_i16RptSite_0 >= i16IntCx_0)
			{
				m_i16RptSite_0 = 0;
			}
		}
	}
	else
	{
		 m_i16RptCmpnD_0 = 0;
		 m_i16RptCmpnQ_0 = 0;
		  m_i16RptCmpnZ_0 = 0;

		 m_i16RptSite_0 = 0;
		 m_i16RptSiteSav_0 = 0;
//		 m_unVshareMdFilt_0.dword = 0;
//		 m_unVshareMzFilt_0.dword = 0;

//		 i16TempIntAx_0 = K45HzPointCntCnst-1;
		 m_i16RptClearCnt_0 = 0;
	}

	if (m_i16RptClearCnt_0 < (K45HzPointCntCnst-1))
	{
		i16RptIntD_0[m_i16RptClearCnt_0] = 0;
		i16RptIntQ_0[m_i16RptClearCnt_0] = 0;
		i16RptIntZ_0[m_i16RptClearCnt_0] = 0;	
		m_i16RptClearCnt_0++;
	} 
}

/************************************************************************************
//函数名称：    Alg_Int_CurrShareReg()
//功能描述：    均流调节环。
//算法说明:     用环流D轴分量调节逆变电压给定矢量D轴分量，用环流Q轴分量调节逆变电压角度瞬时步长
************************************************************************************/

#pragma CODE_SECTION(".Epwm1Funcs")

void 	Class_InverterAlgorithm::Alg_Int_CurrShareReg(void)
{
	INT16	i16IaReg_0,i16IbReg_0,i16IcReg_0;
	INT16	i16IntAlpha_0,i16IntBeta_0;

	// 并机模式、非（并机信号隔离态和调测模式）
	if((objMonInterface.m_st_wUPSModeRun.bConfig == 3) && ( objParallel.m_st_wParaMaint.bParaSignalMask != 1 )
		&& (objSystem.m_st_wFlagSystem.bUPSTestMode != 1) && (objXfer.m_st_XferResult.bInvKM == 1)
	  	&& (objPowCANSched.m_i16PowerCanTotalNum > 1))
	{
		
		//交流均流
		i16IaReg_0 = (((INT32)m_i16IlaPShareReg_0) * m_i16CosQ0Ref_0 + ((INT32)m_i16IlaQShareReg_0) * m_i16SinQ0Ref_0)>>12;
		i16IbReg_0 = (((INT32)m_i16IlbPShareReg_0) * m_i16CosQ0Ref_0 + ((INT32) m_i16IlbQShareReg_0) * m_i16SinQ0Ref_0)>>12;
		i16IcReg_0 = (((INT32)m_i16IlcPShareReg_0) * m_i16CosQ0Ref_0 + ((INT32)m_i16IlcQShareReg_0) * m_i16SinQ0Ref_0)>>12;

		//加入直流均流
		//i16IaReg_0 += m_i16IlaRegOut_0;
		//i16IbReg_0 += m_i16IlbRegOut_0;
		//i16IcReg_0 += m_i16IlcRegOut_0;

		 // 逆变电流3/2变换，矢量模与相位处理
		i16IntAlpha_0 = (((INT32)i16IaReg_0*2-i16IbReg_0-i16IcReg_0)*K1div3Cnst)>>12;
		i16IntBeta_0 = (((INT32)i16IbReg_0-i16IcReg_0)*KSqrt3invCnst)>>12;
	      
		m_i16VshareMd_0 = (((INT32)i16IntBeta_0)*m_i16SinQ0Ref_0+((INT32)i16IntAlpha_0)*m_i16CosQ0Ref_0)>>12;
		m_i16VshareMq_0 = (((INT32)i16IntBeta_0)*m_i16CosQ0Ref_0-((INT32)i16IntAlpha_0)*m_i16SinQ0Ref_0)>>12;
		m_i16VshareMz_0 = (((INT32)i16IaReg_0+i16IbReg_0+i16IcReg_0)*KSqrt2Div3Cnst)>>12;
		
		m_unIntgIinvdReg_0.dword = FILTER(2, objInverter.m_i16IinvMd_1, m_unIntgIinvdReg_0.dword);
		m_i16VrefDroop_0 = (((INT32)m_unIntgIinvdReg_0.half.hword)* 123)>>12;			//加入下垂
		m_i16VrefCmpn_0 =  (((INT32)objInvAlg.m_i16IdAvg_0)  * 123)>>12;				//加入补偿
		m_i16VshareMd_0 = m_i16VshareMd_0 - m_i16VrefDroop_0 + m_i16VrefCmpn_0;

        UPDNLMT16(m_i16VshareMd_0,300,-300);
        UPDNLMT16(m_i16VshareMq_0,300,-300);
        UPDNLMT16(m_i16VshareMz_0,300,-300);
		
		m_i16wVrefActMd_0 += m_i16VshareMd_0;
		m_i16VrefActMz_0 = m_i16VshareMz_0;
	}
	else
	{
		m_i16VshareMd_0 = 0;
		m_i16VshareMq_0 = 0;
		m_i16VshareMz_0 = 0;
	}

}

/************************************************************************************
//函数名称：    Alg_Int_PhaseLock()
//功能描述：    调整逆变给定矢量角瞬时步长实现逆变相位向输出或旁路输出相位靠齐，达到锁定目的
//算法说明:     先根据旁路/输出矢量角平均步长确定给定矢量角标准步长（对应于中心频率），再将
//			    逆变与旁路/输出的相位差P调节后加到给定矢量角标准步长得到给定矢量角瞬时步长。
//			    如果本振，控制瞬时步长逐步递增/递减到标准步长
************************************************************************************/

#pragma CODE_SECTION(".Epwm1Funcs")
void 	Class_InverterAlgorithm::Alg_Int_PhaseLock(void)
{
	INT16 	i16IntAx_0,i16IntBx_0,i16IntCx_0;
	INT32	i32IntGx_0,i32temp1_0,i32temp2_0,i32temp3_0,i32temp4_0;
	UINT32	u32Temp1_0;

	m_i16CosQ0RefBak_0 = m_i16CosQ0Ref_0;
	m_i16SinQ0RefBak_0 = m_i16SinQ0Ref_0;


	if ((objLBS.m_i32TLBS_0 <= InvSyncCap45HzCnt) && (objLBS.m_i32TLBS_0 >= InvSyncCap66HzCnt))
	{	
		if (objLBS.m_st_LBSStatus.bCAPLBSFlag == 1)	
		{
			m_i16DthetaLBSBase_0 = (KDthetaInvSyncBaseCnst/(objLBS.m_i32TLBS_0>>2)); 	//360*1024*10^8/fpwm/cnt
			m_unDthetaLBSBaseFlt_0.dword = FILTER(1, m_i16DthetaLBSBase_0, m_unDthetaLBSBaseFlt_0.dword);
		}
	}
	else
	{
		m_i16DthetaLBSBase_0 = 0;
		m_unDthetaLBSBaseFlt_0.dword  = 0;
	}
	
	switch(objInverter.m_st_wFlagInv.bPhaseTrace)
	{
		case 0:															//本振

			if(objMonInterface.m_st_wUPSModeRun.bFreqOut == 0)
			{
				i16IntAx_0 = K50HzDthetaCnst;	 							//50Hz
			}
			else
			{
				i16IntAx_0 = K60HzDthetaCnst;								//60Hz
			}

			// 调频
			i32IntGx_0 = ((INT32)i16IntAx_0<<16)-m_unPLL1DthetaRefStd_0.dword;
			UPDNLMT32(i32IntGx_0,100,-100);
			m_unPLL1DthetaRefStd_0.dword += i32IntGx_0;
			m_i16PLL1DthetaRefInst_0 = m_unPLL1DthetaRefStd_0.half.hword;

			break;

		case 1:						             // 跟旁路

			// 调频
			i32IntGx_0 = objBypass.m_unDthetaBpBase_0.dword;
			UPDNLMT32(i32IntGx_0,(((INT32)m_i16FreqSetMax_0)<<16),(((INT32)m_i16FreqSetMin_0)<<16));
			i32IntGx_0 -= m_unPLL1DthetaRefStd_0.dword;
			UPDNLMT32(i32IntGx_0,m_i16ThetaStepBp_0,-m_i16ThetaStepBp_0);
			m_unPLL1DthetaRefStd_0.dword += i32IntGx_0;
			

			

				i16IntCx_0 = m_unDphaseFilter_0.half.hword;			// 备份上次相位误差e(n-1)
	            		// 虚拟锁相，求得瞬时相差的正弦值与余弦值
				m_i16DphaseInst_0 = (((INT32)objBypass.m_i16SinQBp_0)*m_i16CosQ0Ref1_0-((INT32)m_i16SinQ0Ref1_0)*objBypass.m_i16CosQBp_0)>>11;	// 计算相位差e(n),3.13格式
				m_i16CosDphase_0 = (((INT32)objBypass.m_i16CosQBp_0)*m_i16CosQ0Ref1_0+((INT32)m_i16SinQ0Ref1_0)*objBypass.m_i16SinQBp_0)>>14;	// 计算相位差e(n),6.10格式

				m_unDphaseFilter_0.dword=FILTER(6, m_i16DphaseInst_0, m_unDphaseFilter_0.dword);

				// 锁相环节优化,增量式PI控制锁相部分的SlewRate
				i32IntGx_0 = ((INT32)m_unDphaseFilter_0.half.hword)*K0BpPLLCnst-((INT32)i16IntCx_0)*K1BpPLLCnst;									// K0*e(n)-K1*e(n-1)

				// 锁相环节的加入与退出处理
				if((objParallel.m_st_wParaSignalBak.bMasterOnline ==1) && (objParallel.m_st_wParaSignalF.bSystemUnlockF==1))      // 逆变主机，并且系统未锁定
				{
					i16IntAx_0 = abs(objBypass.m_unDthetaBpBase_0.half.hword-m_unPLL1DthetaRefStd_0.half.hword);
					if(i16IntAx_0 < KDthetaDiffCnst)
					{
						m_i16TraceBpflag_0 = 1;				// 对应0.1Hz
					}
					else if(i16IntAx_0 > 2*KDthetaDiffCnst)
					{
						m_i16TraceBpflag_0 = 0;				// 对应0.2Hz
					}
					if(m_i16TraceBpflag_0 == 0)				// 回差处理，逆变旁路频率差小于0.1Hz时加入，大于0.2Hz时退出
					{
						i32IntGx_0 = -m_unDthetaBpPLL_0.dword;				// 退出锁相时，锁相输出频率缓慢衰减至0
					}
				}

				// 锁相环节的频率SlewRate控制与积分
				UPDNLMT32(i32IntGx_0,m_i16ThetaStepBp_0*2,-m_i16ThetaStepBp_0*2);

				m_unDthetaBpPLL_0.dword += i32IntGx_0;

				// 锁相环节输出频率的限幅
				UPDNLMT32(m_unDthetaBpPLL_0.dword,(((INT32)m_i16DthetaBpPLLLmt_0)<<7),-(((INT32)m_i16DthetaBpPLLLmt_0)<<7));

				// 锁频输出频率+锁相输出频率
				m_i16PLL1DthetaRefInst_0 = (m_unPLL1DthetaRefStd_0.dword+m_unDthetaBpPLL_0.dword)>>16;
			
			
			break;

		case 2:						             // 跟工频同步信号
    		       m_unPLL1ThetaRef_0.dword = m_unThetaRef_0.dword;				//借用PLL2的输出结果
			m_unPLL1DthetaRefStd_0.dword = m_unDthetaRefStd_0.dword;
			m_i16PLL1DthetaRefInst_0 = m_i16DthetaRefInst_0;
			break;

		case 3:						             // 跟LBS
            // 调频
			if (objLBS.m_st_LBSStatus.bCAPLBSFlag == 1)
			{
				//锁频
				i16IntAx_0 = m_unDthetaLBSBaseFlt_0.half.hword;
				UPDNLMT16(i16IntAx_0,m_i16FreqSetMax_0,m_i16FreqSetMin_0);

			
				i32IntGx_0 = (((INT32)i16IntAx_0)<<16)-m_unPLL1DthetaRefStd_0.dword;
				UPDNLMT32(i32IntGx_0,18000,-18000);								//0.1Hz/s =>38     38*2*200=15200
				m_unPLL1DthetaRefStd_0.dword += i32IntGx_0;
			}
			
            // 调相
			if(abs(m_unPLL1DthetaRefStd_0.half.hword - m_unDthetaLBSBaseFlt_0.half.hword) < KDthetaDiff2Cnst)
			{
				if ((objLBS.m_st_LBSStatus.bCAPLBSFlag == 1) || (m_i16CAPSyncFlag_0 == 1))
				{
					//工频同步锁相角度差的计算
					
						u32Temp1_0 = objLBS.m_u32TLBSClk_0 + (objLBS.m_i32TLBS_0>>2);
						
					i32temp1_0 = m_u32TInvSyncClk_0 - u32Temp1_0 + 32000;
					//补偿并机下垂的角度
					if(objMonInterface.m_st_wUPSModeRun.bFreqOut==0)
					{
						i32temp1_0 =	i32temp1_0 - ((((INT32)m_unThetaIcirChgFlt_0.half.hword)*5556)>>10);	//100M/50Hz/360/2^10(角度下垂量的定标值) * 2^10
					}
					else
					{
						i32temp1_0 =	i32temp1_0 - ((((INT32)m_unThetaIcirChgFlt_0.half.hword)*4630)>>10);			//100M/60Hz/360/2^10(角度下垂量的定标值) * 2^10
					}
					if (i32temp1_0 > (objLBS.m_i32TLBS_0 >> 1))
					{
						i32temp1_0 = i32temp1_0-objLBS.m_i32TLBS_0;
					}
					if (i32temp1_0 < -(objLBS.m_i32TLBS_0 >> 1))
					{
						i32temp1_0 = objLBS.m_i32TLBS_0 + i32temp1_0;
					}
					UPDNLMT32(i32temp1_0,InvSyncCap45HzCntDiv2,-InvSyncCap45HzCntDiv2);

					if(objMonInterface.m_st_wUPSModeRun.bFreqOut==0)
					{
						m_i16DphaseLBSSync_0 = (i32temp1_0*0x34)>>12;				// 50Hz (2*pi)/2000000*2^12*2^12
					}
					else
					{
						m_i16DphaseLBSSync_0 = (i32temp1_0*0x3f)>>12;				// 60Hz (2*pi)/1666667*2^12*2^12
					}
					UPDNLMT32(m_i16DphaseLBSSync_0,214,-214);					// 3度,4.12f
				
					//锁相
					i16IntBx_0 = ((INT32)m_i16DphaseLBSSync_0)*0x5;
					UPDNLMT16(i16IntBx_0,1500,-1500);
					
					m_i16PLL1DthetaInstOneInt_0 = i16IntBx_0/objInverter.m_i16PointcntInvM_0;	//锁相对应的每个开关周期的平均角度增量
			
            			}	
            		}
			else
			{
				m_i16PLL1DthetaInstOneInt_0 = 0;
			}
			m_i16PLL1DthetaRefInst_0 = m_unPLL1DthetaRefStd_0.half.hword + m_i16PLL1DthetaInstOneInt_0;
			break;

		default:
			break;

	}

	if (objInverter.m_st_wFlagInv.bPhaseTrace != 1)
	{
		m_unDthetaBpPLL_0.dword = 0;
	}

	switch(objInverter.m_st_wFlagInv.bPLL2PhaseTrace)
	{
		case 0:						             						// 跟旁路
			//m_unThetaRef_0.dword = m_unPLL1ThetaRef_0.dword;				//借用PLL1的输出结果
			//m_unDthetaRefStd_0.dword = m_unPLL1DthetaRefStd_0.dword;
			//m_i16DthetaRefInst_0 = m_i16PLL1DthetaRefInst_0;
			//break;

		case 1:
			if (m_i16CAPSyncFlag_0 == 1)
			{// 跟工频同步信号
				//工频同步信号频率的计算

				m_i16DthetaInvSyncBase_0 = (KDthetaInvSyncBaseCnst/(m_i32TInvSync_0>>2)); 	//360*1024*10^8/fpwm/cnt

				//锁频
				i16IntAx_0 = m_i16DthetaInvSyncBase_0;
				UPDNLMT16(i16IntAx_0,m_i16FreqSetMax_0,m_i16FreqSetMin_0);


				i32IntGx_0 = (((INT32)i16IntAx_0)<<16)-m_unDthetaRefStd_0.dword;
				UPDNLMT32(i32IntGx_0,300000L,-300000L);								//0.1Hz/s =>38     38*2*200=15200
				m_unDthetaRefStd_0.dword += i32IntGx_0;
			}


			if ((m_i16PLL2CroseZeroFlag_0 == 1) || (m_i16CAPSyncFlag_0 == 1))
			{
				//工频同步锁相角度差的计算
				i32temp1_0 = m_u32TInvSyncPLL2Clk_0 - m_u32TInvSyncClk_0 + 100;        //100/2000000*360=0.018度,补偿硬件延时???2017/10/06
				
				if (i32temp1_0 > (m_i32TInvSync_0 >> 1))
				{
					i32temp1_0 = i32temp1_0-m_i32TInvSync_0;
				}
				if (i32temp1_0 < -(m_i32TInvSync_0 >> 1))
				{
					i32temp1_0 = m_i32TInvSync_0 + i32temp1_0;
				}
				UPDNLMT32(i32temp1_0,InvSyncCap45HzCntDiv2,-InvSyncCap45HzCntDiv2);

				m_i16DphaseInvSync_0 = (i32temp1_0*0x34)>>12;				// 50Hz (2*pi)/2000000*2^12*2^12
				UPDNLMT32(m_i16DphaseInvSync_0,1050,-1050);				// 4.9度,4.12f

				//锁相
				i16IntBx_0 = ((INT32)m_i16DphaseInvSync_0)*0x0E;
				UPDNLMT16(i16IntBx_0,16000,-16000);
				m_i16DthetaRefInstOneInt_0 = i16IntBx_0/objInverter.m_i16PointcntInvM_0;	//锁相对应的每个开关周期的平均角度增量
				m_i16DthetaRefInstRem_0 = i16IntBx_0 - m_i16DthetaRefInstOneInt_0*objInverter.m_i16PointcntInvM_0;
																	//
				m_i16DthetaRefInst_0 = m_unDthetaRefStd_0.half.hword + m_i16DthetaRefInstOneInt_0;
				m_i16DthetaRefInstCnt_0 = 0;
				
			}
			else
			{
				m_i16DthetaRefInstCnt_0++;
				if (m_i16DthetaRefInstCnt_0 <= m_i16DthetaRefInstRem_0)
				{
					m_i16DthetaRefInst_0 = m_unDthetaRefStd_0.half.hword + m_i16DthetaRefInstOneInt_0 + 1;
				}
				else
				{
					m_i16DthetaRefInstCnt_0 = m_i16DthetaRefInstRem_0;
					m_i16DthetaRefInst_0 = m_unDthetaRefStd_0.half.hword+m_i16DthetaRefInstOneInt_0;
				}
			}
			break;

		default:
			break;

	}
	
	//计算PLL1的角度
	UPDNLMT16(m_i16PLL1DthetaRefInst_0,m_i16FreqSetMax_0,m_i16FreqSetMin_0);

	m_unPLL1ThetaRefOld_0.dword = m_unPLL1ThetaRef_0.dword;	//保存上一拍的角度值
	m_unPLL1ThetaRef_0.dword += (((INT32)m_i16PLL1DthetaRefInst_0)<<6);
	if(m_unPLL1ThetaRef_0.dword >= 23592960)                 				// 360度
	{
		m_unPLL1ThetaRef_0.dword -= 23592960;
	}
	
	//计算PLL2的角度
	UPDNLMT16(m_i16DthetaRefInst_0,m_i16FreqSetMax_0,m_i16FreqSetMin_0);

	m_i16TinvUsSum_0++;
	m_unThetaRefOld_0.dword = m_unThetaRef_0.dword;
	m_st_wIntMainFlag.bVpwmCrossZero = 0;				 // 清过零标志
	m_unThetaRef_0.dword += (((INT32)m_i16DthetaRefInst_0)<<6);
	if(m_unThetaRef_0.dword >= 23592960)                 // 360度
	{
		m_unThetaRef_0.dword -= 23592960;		         // (360*65536)
		m_st_wIntMainFlag.bVpwmCrossZero = 1;		     // 置过零标志
		m_i16TinvUsSumM_0 = m_i16TinvUsSum_0;
		m_i16TinvUsSum_0 = 0;
	}

	//连续50个中断周期旁路间断切逆变时没有抓到过零点则强行切换
	//如果不是旁路间断切逆变状态(例如维持旁路，维持逆变等待)，则清标志
	if(objXfer.m_st_wXferCmd.bWaitingZeroCross == 0)
	{
		m_i16WaitingZeroCrossCnt_0 = 0;
		m_st_wIntMainFlag2_0.bWaitingZeroCrossDelayok = 0;
	}
	if((m_unThetaRef_0.dword <= 196608L) || (m_unThetaRef_0.dword >= 23396352L))
	{
		if(m_st_wIntMainFlag2_0.bBptoInvZero == 0)
		{
			if(objXfer.m_st_wXferCmd.bWaitingZeroCross == 1)
			{
				m_i16WaitingZeroCrossCnt_0++;
				if(m_i16WaitingZeroCrossCnt_0 >= 50)
				{
					m_i16WaitingZeroCrossCnt_0 = 50;
					m_st_wIntMainFlag2_0.bWaitingZeroCrossDelayok = 1;
				}
				else
				{
					m_st_wIntMainFlag2_0.bWaitingZeroCrossDelayok = 0;
				}
			}	
		}
		m_st_wIntMainFlag2_0.bBptoInvZero =  1;
	}
	else
	{
		m_st_wIntMainFlag2_0.bBptoInvZero =  0;
	}

	if(objMonInterface.m_st_wUPSModeRun.bConfig == 3)
	{
		if(m_unThetaRef_0.dword < 11763712)				// 180度(实际考虑四舍五入为179.5度 359<<15)
		{
			if(m_st_wIntMainFlag.bSinPositive == 0)
			{
				m_st_wIntMainFlag.bSinPositive = 1;

			}
		}
		else
		{
			if(m_st_wIntMainFlag.bSinPositive == 1)
			{
				m_st_wIntMainFlag.bSinPositive = 0;

			}
		}
	}
	
	//PLL2 0°对应ECap4Regs.TSCTR的差值处理
	m_i16PLL2CroseZeroFlag_0 = 0;

	if ((m_unThetaRef_0.dword < Theta90Cnt) && (m_unThetaRefOld_0.dword > Theta270Cnt))
	{
		m_u32TInvSyncPLL2Clk_0 = m_u32ECap4RegsTSCTR_0;

		i32temp4_0 = ((INT32)Theta360Cnt - m_unThetaRefOld_0.dword)>>8;
		i32temp2_0 = (m_unThetaRef_0.dword + Theta360Cnt - m_unThetaRefOld_0.dword)>>8;
		i32temp3_0 = m_u32TInvSyncPLL2Clk_0 - m_u32TInvSyncPLL2ClkOld_0;
		i32temp1_0 = (i32temp3_0 * i32temp4_0)/ i32temp2_0;
		m_u32TInvSyncPLL2Clk_0 = m_u32TInvSyncPLL2ClkOld_0 + i32temp1_0;

		m_i16PLL2CroseZeroFlag_0 = 1;
	}
	m_u32TInvSyncPLL2ClkOld_0 = m_u32ECap4RegsTSCTR_0;

	//并机调试时需屏蔽
//	iThetaIcirChg_0 = 0;

	
		m_unThetaRef2_0.dword = m_unThetaRef_0.dword + 23592960 + (((INT32)m_i16ThetaIcirChg_0)<<6);	//角度下垂量
	

	if(m_unThetaRef2_0.dword >= 23592960)					// 360度
	{
		m_unThetaRef2_0.dword -= 23592960;				// (360*65536)
	}
	if(m_unThetaRef2_0.dword >= 23592960)					// 360度
	{
		m_unThetaRef2_0.dword -= 23592960;				// (360*65536)
	}

	//下垂前角度的正余弦计算，用于锁相
    // 正弦值插值操作
	m_i16SinQ0Ref_0 = sintab[m_unPLL1ThetaRef_0.half.hword];
	i16IntBx_0 = m_unPLL1ThetaRef_0.half.hword+1;
	if(i16IntBx_0 >= 360)
	{
		i16IntBx_0 -= 360;
	}
	i16IntBx_0 = sintab[i16IntBx_0];
	i16IntCx_0 = abs(i16IntBx_0-m_i16SinQ0Ref_0);
	i16IntCx_0 = (((INT32)i16IntCx_0)*m_unPLL1ThetaRef_0.half.lword)>>16;
	if(i16IntBx_0 > m_i16SinQ0Ref_0)
	{
		m_i16SinQ0Ref_0 += i16IntCx_0;
	}
	else
	{
		m_i16SinQ0Ref_0 -= i16IntCx_0;
	}
	Theta = i16IntCx_0;

    // 余弦值插值操作
   	 i16IntAx_0 = m_unPLL1ThetaRef_0.half.hword+90;
   	 if(i16IntAx_0 >= 360)
   	 {
		i16IntAx_0 -= 360;
    	}
	m_i16CosQ0Ref_0 = sintab[i16IntAx_0];
	i16IntBx_0 = i16IntAx_0+1;
	if(i16IntBx_0 >= 360)
	{
		i16IntBx_0 -= 360;
	}
	i16IntBx_0 = sintab[i16IntBx_0];
	i16IntCx_0 = abs(i16IntBx_0-m_i16CosQ0Ref_0);
	i16IntCx_0 = (((INT32)i16IntCx_0)*m_unPLL1ThetaRef_0.half.lword)>>16;
	if(i16IntBx_0 > m_i16CosQ0Ref_0)
	{
		m_i16CosQ0Ref_0 += i16IntCx_0;
	}
	else
	{
		m_i16CosQ0Ref_0 -= i16IntCx_0;
	}
	m_i16SinQ0Ref1_0 = m_i16SinQ0Ref_0;
	m_i16CosQ0Ref1_0 = m_i16CosQ0Ref_0;

	//下垂后角度的正余弦计算，用于变换
	// 正弦值插值操作
	m_i16SinQ0Ref_0 = sintab[m_unThetaRef2_0.half.hword];
	i16IntBx_0 = m_unThetaRef2_0.half.hword+1;
	if(i16IntBx_0 >= 360)
	{
		i16IntBx_0 -= 360;
	}
	i16IntBx_0 = sintab[i16IntBx_0];
	i16IntCx_0 = abs(i16IntBx_0-m_i16SinQ0Ref_0);
	i16IntCx_0 = (((INT32)i16IntCx_0)*m_unThetaRef2_0.half.lword)>>16;
	if(i16IntBx_0 > m_i16SinQ0Ref_0)
	{
		m_i16SinQ0Ref_0 += i16IntCx_0;
	}
	else
	{
		m_i16SinQ0Ref_0 -= i16IntCx_0;
	}

    // 余弦值插值操作
   	 i16IntAx_0 = m_unThetaRef2_0.half.hword+90;
    	if(i16IntAx_0 >= 360)
    	{
		i16IntAx_0 -= 360;
    	}
	m_i16CosQ0Ref_0 = sintab[i16IntAx_0];
	i16IntBx_0 = i16IntAx_0+1;
	if(i16IntBx_0 >= 360)
	{
		i16IntBx_0 -= 360;
	}
	i16IntBx_0 = sintab[i16IntBx_0];
	i16IntCx_0 = abs(i16IntBx_0-m_i16CosQ0Ref_0);
	i16IntCx_0 = (((INT32)i16IntCx_0)*m_unThetaRef2_0.half.lword)>>16;
	if(i16IntBx_0 > m_i16CosQ0Ref_0)
	{
		m_i16CosQ0Ref_0 += i16IntCx_0;
	}
	else
	{
		m_i16CosQ0Ref_0 -= i16IntCx_0;
	}

}

/************************************************************************************
//函数名称：    Alg_Int_VoltReg()
//功能描述：    电压环实现。
//算法说明:     PI调节器实现，调节后进行电流的解耦与前馈处理得到参考电流
************************************************************************************/

#pragma CODE_SECTION(".Epwm1Funcs")

void 	Class_InverterAlgorithm::Alg_Int_VoltReg(void)
{
	INT16	i16IntAx_0,i16IntBx_0;
	INT32	i32IntGx_0,i32temp_0;

	m_i16VerrMd_0 = m_i16wVrefActMd_0-objInverter.m_i16VinvMd_0;
	UPDNLMT16(m_i16VerrMd_0,1000,-1000);

	m_i16IrefLimit_0 = 0x6666;
	i32IntGx_0=(((INT32)m_i16IrefLimit_0)<<16);

	i16IntAx_0 = m_i16KpVolt_0;
	i16IntBx_0 = m_i16KiVolt_0;
	if(abs(m_i16VerrMd_0) > 200)
	{
     		i16IntAx_0 = m_i16KpVoltDyn_0;
		i16IntBx_0 = m_i16KiVolt_0<<2;
	}

	// D轴电压调节
//	m_i16IrefMd_0 = m_unIntgDVr_0.half.hword+((((INT32)m_i16VerrMd_0)*i16IntAx_0)>>12)+objInverter.m_i16IinvMd_0-objParallel.m_i16IcirMdAvg_0;
	i32temp_0 = m_unIntgDVr_0.half.hword+((((INT32)m_i16VerrMd_0)*i16IntAx_0)>>12)+objInverter.m_i16IinvMd_1;
	UPDNLMT32(i32temp_0,m_i16IrefLimit_0,-m_i16IrefLimit_0);
	m_i16IrefMd_0 = i32temp_0;
	m_unIntgDVr_0.dword +=( (INT32)m_i16VerrMd_0)*i16IntBx_0;
	UPDNLMT32(m_unIntgDVr_0.dword,i32IntGx_0,-i32IntGx_0);

	// Q轴电压调节
	m_i16VerrMq_0 = m_i16VshareMq_0 -objInverter.m_i16VinvMq_0;
	UPDNLMT16(m_i16VerrMq_0,1000,-1000);
//	m_i16IrefMq_0 = m_unIntgQVr_0.half.hword+((((INT32)m_i16VerrMq_0)*i16IntAx_0)>>12)+objInverter.m_i16IinvMq_0-objParallel.m_i16IcirMqAvg_0;
	i32temp_0 = m_unIntgQVr_0.half.hword+((((INT32)m_i16VerrMq_0)*i16IntAx_0)>>12)+objInverter.m_i16IinvMq_1;
	UPDNLMT32(i32temp_0,0x4000,-0x4000);
	m_i16IrefMq_0 = i32temp_0;
	m_unIntgQVr_0.dword += ((INT32)m_i16VerrMq_0)*i16IntBx_0;
	UPDNLMT32(m_unIntgQVr_0.dword,0x20000000,-0x20000000);
	

	//0轴电压调节
	m_i16VerrMz_0 = m_i16VrefActMz_0-objInverter.m_i16VinvMz_0;
	UPDNLMT16(m_i16VerrMz_0,1000,-1000);
	i32temp_0 = m_unIntgZVr_0.half.hword+((((INT32)m_i16VerrMz_0)*i16IntAx_0)>>12)+objInverter.m_i16IinvMz_1;
	UPDNLMT32(i32temp_0,0x4000,-0x4000);
	m_i16IrefMz_0 = i32temp_0;
	m_unIntgZVr_0.dword += ((INT32)m_i16VerrMz_0)*i16IntBx_0;
	UPDNLMT32(m_unIntgZVr_0.dword,0x2000000,-0x2000000);
}
/************************************************************************************
//函数名称：    Alg_Int_VoltDCReg()
//功能描述：    电压直流分量环实现。
//算法说明:     PI调节器实现，调节后进行电流的解耦与前馈处理得到参考电流
************************************************************************************/

#pragma CODE_SECTION(".Epwm1Funcs")

void 	Class_InverterAlgorithm::Alg_Int_VoltDCReg(void)
{
	// 逆变电压直流分量控制
    if (objInvCtrl.m_i16InvSSCntPer40ms<=175)								//逆变开启7S不调节，输出滤波电容有残压
	{
		m_unVinvaDcFilter_0.dword = 0;
		m_unVinvbDcFilter_0.dword = 0;
		m_unVinvcDcFilter_0.dword = 0;
		m_i16VdcaCon_0 = 0;
		m_i16VdcbCon_0 = 0;
		m_i16VdccCon_0 = 0;

	}
//else if(objInvCtrl.m_i16InvSSCntPer40ms<1250)							// 逆变电压起来后50s内进行直流分量调节
	{
		if(abs(objInverter.m_i16VinvaDc_0)<200)
		{
			objInverter.m_i16VinvaDc_0 = 0;      	// 逆变电压直流分量小于200mV后不进行积分

		}
		m_unVinvaDcFilter_0.dword = ((INT32)objInverter.m_i16VinvaDc_0)*4+m_unVinvaDcFilter_0.dword;
		UPDNLMT32(m_unVinvaDcFilter_0.dword,0x12345678,-0x12345678);	// 4660,-4660
		m_i16VdcaCon_0 = m_unVinvaDcFilter_0.half.hword>>6;				//72,-72,3.9V

		if(abs(objInverter.m_i16VinvbDc_0)<200)
		{
			objInverter.m_i16VinvbDc_0 = 0;         // 逆变电压直流分量小于200mV后不进行积分
		}
		m_unVinvbDcFilter_0.dword = ((INT32)objInverter.m_i16VinvbDc_0)*4+m_unVinvbDcFilter_0.dword;
		UPDNLMT32(m_unVinvbDcFilter_0.dword,0x12345678,-0x12345678);
		m_i16VdcbCon_0 = m_unVinvbDcFilter_0.half.hword>>6;				//72,-72,3.9V

		if(abs(objInverter.m_i16VinvcDc_0)<200)
		{
			objInverter.m_i16VinvcDc_0 = 0;         // 逆变电压直流分量小于200mV后不进行积分
		}
		m_unVinvcDcFilter_0.dword = ((INT32)objInverter.m_i16VinvcDc_0)*4+m_unVinvcDcFilter_0.dword;
		UPDNLMT32(m_unVinvcDcFilter_0.dword,0x12345678,-0x12345678);
		m_i16VdccCon_0 = m_unVinvcDcFilter_0.half.hword>>6;				//72,-72,3.9V
	}
}

/************************************************************************************
//函数名称：    Alg_Int_CurrReg()
//功能描述：    电流环功能实现。
//算法说明: 	电流参考与电感电流之差进行P调节并限幅得到UIR_D,UIR_Q
************************************************************************************/

#pragma CODE_SECTION(".Epwm1Funcs")

void 	Class_InverterAlgorithm::Alg_Int_CurrReg(void)
{
	INT16	i16IntAx_0;

	// D轴电流调节
	i16IntAx_0 = m_i16IrefMd_0-objInverter.m_i16IlMd_0;
	m_i16wUirD_0 = (((INT32)i16IntAx_0)*m_i16KpCurr_0)>>12;
	m_i16wUirD_0 += -((((INT32)objInverter.m_i16IlMq_0)*m_i16Zl_0)>>12)+m_i16wVrefActMd_0;
	UPDNLMT16(m_i16wUirD_0,0x2000,-0x2000);

    // Q轴电流调节
	i16IntAx_0 = m_i16IrefMq_0-objInverter.m_i16IlMq_0;
	m_i16wUirQ_0 = (((INT32)i16IntAx_0)*m_i16KpCurr_0)>>12;
	m_i16wUirQ_0 += ((((INT32)objInverter.m_i16IlMd_0)*m_i16Zl_0)>>12);
	UPDNLMT16(m_i16wUirQ_0,0x2000,-0x2000);

	// 0轴电流调节
	i16IntAx_0 = m_i16IrefMz_0-objInverter.m_i16IlMz_0;
	m_i16wUirZ_0=(((INT32)i16IntAx_0)*m_i16KpCurr_0)>>12;				// P调节
	UPDNLMT16(m_i16wUirZ_0,0x2000,-0x2000);
}

/************************************************************************************
//函数名称：    Alg_Int_SPWM()
//功能描述：    SPWM发波模块
************************************************************************************/

#pragma CODE_SECTION(".Epwm1Funcs")

void 	Class_InverterAlgorithm::Alg_Int_SPWM(void)
{
	INT16	i16IntAx_0,i16IntBx_0,i16IntCx_0,i16IntDx_0;
	INT16	i16IntAlpha_0,i16IntBeta_0;
//	INT16	i16VaAct_0,i16VbAct_0,i16VcAct_0;

	m_i16PulseOffCount = 0;
	m_i16KRptCon_0 = m_i16wKRptConLmt_0; 					// 重复控制系数调整

	m_i16wUirD_0 = m_i16wUirD_0+((((INT32)m_i16RptCmpnD_0)*m_i16KRptCon_0)>>12);
	m_i16wUirQ_0 = m_i16wUirQ_0+((((INT32)m_i16RptCmpnQ_0)*m_i16KRptCon_0)>>12);
	m_i16wUirZ_0 = m_i16wUirZ_0+((((INT32)m_i16RptCmpnZ_0)*m_i16KRptCon_0)>>12);

	//母线电压前馈
	i16IntDx_0 = objDCBus.m_i16VbusP_0;
	UPDNLMT16(i16IntDx_0,4999,3000);
	i16IntDx_0 -= 3000;
	m_i161DivVbusP_0 = iVbusInvTab[i16IntDx_0];				//4096*4096/(VbusP定标后的值 405V＝4096)

	i16IntDx_0 = objDCBus.m_i16VbusN_0;
	UPDNLMT16(i16IntDx_0,4999,3000);
	i16IntDx_0 -= 3000;
	m_i161DivVbusN_0 = iVbusInvTab[i16IntDx_0];				//4096*4096/(VbusN定标后的值 405V＝4096)

	// 旋转/静止变换
	i16IntAlpha_0 = (((INT32)m_i16wUirD_0) * m_i16CosQ0Ref_0 - (((INT32)m_i16wUirQ_0)*m_i16SinQ0Ref_0)) >> 12;
	i16IntBeta_0 = (((INT32)m_i16wUirD_0) * m_i16SinQ0Ref_0 + (((INT32)m_i16wUirQ_0)*m_i16CosQ0Ref_0)) >> 12;

	// 2/3变换
	m_i16VaAct_0 = i16IntAlpha_0+((((INT32)m_i16wUirZ_0) * K1DivSqrt2Cnst) >> 12);
	m_i16VbAct_0 = (-((INT32)i16IntAlpha_0) * K1Div2Cnst + (((INT32)i16IntBeta_0) * KSqrt3Div2Cnst)+(((INT32)m_i16wUirZ_0)*K1DivSqrt2Cnst)) >> 12;
	m_i16VcAct_0 = (-((INT32)i16IntAlpha_0) * K1Div2Cnst -(((INT32)i16IntBeta_0)  *KSqrt3Div2Cnst)+(((INT32)m_i16wUirZ_0)*K1DivSqrt2Cnst)) >> 12;

	m_i16VaAct_0 += m_i16IlaRegOut_0;
	m_i16VbAct_0 += m_i16IlbRegOut_0;
	m_i16VcAct_0 += m_i16IlcRegOut_0;
	//wxwei:去掉死区补偿，防止有差并机时三次谐波环流
	/*
	// 旋转/静止变换
	i16IntAlpha_0 = (((INT32)m_i16IrefMd_0) * m_i16CosQ0Ref_0 - (((INT32)m_i16IrefMq_0)*m_i16SinQ0Ref_0)) >> 12;
	i16IntBeta_0 = (((INT32)m_i16IrefMd_0) * m_i16SinQ0Ref_0 + (((INT32)m_i16IrefMq_0)*m_i16CosQ0Ref_0)) >> 12;

	// 2/3变换
	m_i16Iaref_0 = i16IntAlpha_0+((((INT32)m_i16IrefMz_0) * K1DivSqrt2Cnst) >> 12);
	m_i16Ibref_0 = (-((INT32)i16IntAlpha_0) * K1Div2Cnst + (((INT32)i16IntBeta_0) * KSqrt3Div2Cnst)+(((INT32)m_i16IrefMz_0)*K1DivSqrt2Cnst)) >> 12;
	m_i16Icref_0 = (-((INT32)i16IntAlpha_0) * K1Div2Cnst -(((INT32)i16IntBeta_0)  *KSqrt3Div2Cnst)+(((INT32)m_i16IrefMz_0)*K1DivSqrt2Cnst)) >> 12;

	UPDNLMT16(m_i16Iaref_0,1600,-1600);
	UPDNLMT16(m_i16Ibref_0,1600,-1600);
	UPDNLMT16(m_i16Icref_0,1600,-1600);

	unIarefFilter_0.dword += (((long)m_i16Iaref_0<<11) - unIarefFilter_0.dword)>>2;
	unIbrefFilter_0.dword += (((long)m_i16Ibref_0<<11) - unIbrefFilter_0.dword)>>2;
	unIcrefFilter_0.dword += (((long)m_i16Icref_0<<11) - unIcrefFilter_0.dword)>>2;

	m_i16PWMaDeadCmp_0 = unIarefFilter_0.half.hword;
	m_i16PWMbDeadCmp_0 = unIbrefFilter_0.half.hword;
	m_i16PWMcDeadCmp_0 = unIcrefFilter_0.half.hword;

	UPDNLMT16(m_i16PWMaDeadCmp_0,50,-50);
	UPDNLMT16(m_i16PWMbDeadCmp_0,50,-50);
	UPDNLMT16(m_i16PWMcDeadCmp_0,50,-50);*/
	
	m_i16PWMaDeadCmp_0 = 0;
	m_i16PWMbDeadCmp_0 = 0;
	m_i16PWMcDeadCmp_0 = 0;

	// 计算比较值
	i16IntAx_0 = (((INT32)(abs(m_i16VaAct_0)))* m_i16KSPWM) >> 12;
	i16IntBx_0 = (((INT32)(abs(m_i16VbAct_0)))* m_i16KSPWM) >> 12;
	i16IntCx_0 = (((INT32)(abs(m_i16VcAct_0)))* m_i16KSPWM) >> 12;

	i16IntAx_0 += 8;										//发波补偿360ns(硬件上升下降延时损失)
	i16IntBx_0 += 8;										//发波补偿360ns(硬件上升下降延时损失)
	i16IntCx_0 += 8;										//发波补偿360ns(硬件上升下降延时损失)

	if (m_i16VaAct_0 > 0)
	{
		GpioDataRegs.GPACLEAR.bit.GPIO17 = 1;				//A相正半周
		i16IntAx_0 = ((((INT32)i16IntAx_0) * m_i161DivVbusP_0) >> 12) + m_i16PWMaDeadCmp_0;
	}
	else
	{
		GpioDataRegs.GPASET.bit.GPIO17 = 1;				//A相负半周
		i16IntAx_0 = ((((INT32)i16IntAx_0) * m_i161DivVbusN_0) >> 12) - m_i16PWMaDeadCmp_0;
	}

	if (m_i16VbAct_0 > 0)
	{
        GpioDataRegs.GPACLEAR.bit.GPIO18 = 1;//B相正半周
		i16IntBx_0 = ((((INT32)i16IntBx_0) * m_i161DivVbusP_0) >> 12) + m_i16PWMbDeadCmp_0;
	}
	else
	{
        GpioDataRegs.GPASET.bit.GPIO18 = 1;				//B相负半周
		i16IntBx_0 = ((((INT32)i16IntBx_0) * m_i161DivVbusN_0) >> 12) - m_i16PWMbDeadCmp_0;
	}

	if (m_i16VcAct_0 > 0)
	{
		GpioDataRegs.GPACLEAR.bit.GPIO19 = 1;				//C相正半周
		i16IntCx_0 = ((((INT32)i16IntCx_0) * m_i161DivVbusP_0) >> 12) + m_i16PWMcDeadCmp_0;
	}
	else
	{
		GpioDataRegs.GPASET.bit.GPIO19 = 1;				//C相负半周
		i16IntCx_0 = ((((INT32)i16IntCx_0) * m_i161DivVbusN_0) >> 12) - m_i16PWMcDeadCmp_0;
	}

	UPDNLMT16(i16IntAx_0,KSwPrdCnst-20,20);
	UPDNLMT16(i16IntBx_0,KSwPrdCnst-20,20);
	UPDNLMT16(i16IntCx_0,KSwPrdCnst-20,20);

	// 三相发波
	EPwm1Regs.CMPA.bit.CMPA = i16IntAx_0;				//A相S1管脉冲
	EPwm1Regs.CMPB.bit.CMPB	= i16IntBx_0;				//B相S1管脉冲
	EPwm2Regs.CMPA.bit.CMPA = i16IntCx_0;				//C相S1管脉冲

	}

/************************************************************************************
//函数名称：    Alg_Int_CarrierSync()
//功能描述：	主从机之间的载波同步(Cap1)
************************************************************************************/

#pragma CODE_SECTION(".Epwm1Funcs")

void Class_InverterAlgorithm::Alg_Int_CarrierSync(void)
{
	INT16	i16IntAx_0;
    // 载波同步处理
	GpioDataRegs.GPASET.bit.GPIO16 = 1;							// zxj 20060920改用IO16发载波同步信号  改用CANS收发器 20170826

	m_u16T4Period_0 = KIntPrdCnst;
	if(objParallel.m_st_wParaMaint.bParaSignalMask !=1)				// 并机信号隔离态下跳转
	{
    	if(objParallel.m_st_wParaSignalBak.bMasterOnline == 1)		// 主机发送同步脉冲
		{
    	  	 	 m_i16NumCarrier_0--;
    		   	 if(m_i16NumCarrier_0 <= 0)
    	   		 {
    	    			m_i16NumCarrier_0 = 25;
				    GpioDataRegs.GPACLEAR.bit.GPIO16 = 1;					 // zxj 20060920改用IO16发载波同步信号
			     }
		}
		else if(ECap1Regs.ECFLG.bit.CEVT1 == 1)	  				 	// 从机接收同步脉冲
		{
			i16IntAx_0 = ECap1Regs.CAP1 - 146;		                //补偿 CAN收发器传输延时0.4us+进中断到发载波同步信号时间1.06us；并进行实际微调20170828				
			if(i16IntAx_0>KIntPrdCnst)
			{
				i16IntAx_0 -= (KIntPrdCnst<<1);
			}
			m_u16T4Period_0 = (i16IntAx_0>>1)+KIntPrdCnst;
			
			UPDNLMT16(m_u16T4Period_0,KIntPrdCnst+10,KIntPrdCnst-10);	//30时钟周期的限幅有待商豁，之前机器为5KHz开关频率
			ECap1Regs.ECCLR.all = 0xFFFF;          					// 清所有标志
			ECap1Regs.ECCTL2.bit.REARM = 1;		   					// 重置捕获寄存器
		}
	}

	m_u16T1Period_0 = m_u16T4Period_0>>1;				// 处理发波频率与控制频率的2倍频关系

	EPwm1Regs.TBPRD = m_u16T1Period_0;
	EPwm2Regs.TBPRD = m_u16T1Period_0;	

}

/******************************************************************************************/
//函数名称：    Alg_Int_InvSyncPulseGen
//功能描述：    工频同步脉冲发送模块
/*******************************************************************************************/
#pragma CODE_SECTION(".Epwm1Funcs")

void	Class_InverterAlgorithm:: Alg_Int_InvSyncPulseGen()
{
	static	INT16	s_i16Sync1st_0 = 0;							//第一次调整EPwm6Regs.TBPRD后，更改为1
	static	INT16	s_i16SyncTimeRecordCnt_0 = 0;				//工频同步180°时EPwm6Regs.TBCTR已记录的计数器
	static	INT16	s_i16Timer6T1Cnt_0 = 0;						//前一拍的Timer6的计数值
	INT16	i16Timer6T3Cnt_0;									//后一拍的Timer6的计数值
	INT16	i16InvSyncPrdDiv2_0;								//0.5工频周期对应的计数增量
  	  INT32	i32temp1_0, i32temp2_0,i32temp3_0,i32temp4_0;

	//记录0°时的时刻点,差值处理
	if ((m_unPLL1ThetaRef_0.dword < Theta90Cnt) && (m_unPLL1ThetaRefOld_0.dword > Theta270Cnt) && (s_i16SyncTimeRecordCnt_0 >= 1))
	{
		i16Timer6T3Cnt_0 = m_i16EPwm6RegsTBCTR_0;
		if (i16Timer6T3Cnt_0 < s_i16Timer6T1Cnt_0)
		{
			i16Timer6T3Cnt_0 = i16Timer6T3Cnt_0 + EPwm6Regs.TBPRD;
		}

		i32temp4_0 = ((INT32)Theta360Cnt) - m_unPLL1ThetaRefOld_0.dword;
		i32temp2_0 = m_unPLL1ThetaRef_0.dword + Theta360Cnt - m_unPLL1ThetaRefOld_0.dword;
		i32temp3_0 = i16Timer6T3Cnt_0 - s_i16Timer6T1Cnt_0;
		i32temp1_0 = (i32temp3_0 * i32temp4_0)/ i32temp2_0;
		m_i16Theta0Timer6Cnt_0 = s_i16Timer6T1Cnt_0 + i32temp1_0;
	}

	//记录180°时的时刻点
	//根据0°到180°之间EPwm6Regs.TBCTR的增量调整EPwm6Regs.TBPRD；
	//EPwm6Regs.TBPRD = (180°的TBCTR） ＋ 0.5工频周期对应的EPwm6Regs.TBCTR的计数增量
	//只在0°和180°的EPwm6Regs.TBCTR的值都有记录时才会调整EPwm6Regs.TBPRD，由计数器s_i16SyncTimeRecordCnt_0保证
	//在第一次调整EPwm6Regs.TBPRD中，对iTheta180Timer6Cnt_0限制在〔5000，25000〕之间，这样一次可调整到位
	if ((m_unPLL1ThetaRefOld_0.dword < Theta180Cnt) && (m_unPLL1ThetaRef_0.dword >= Theta180Cnt))
	{
		s_i16SyncTimeRecordCnt_0++;
		if (s_i16SyncTimeRecordCnt_0 >= 2)
		{
			s_i16SyncTimeRecordCnt_0 = 2;
		}

		i16Timer6T3Cnt_0 = m_i16EPwm6RegsTBCTR_0;
		if (i16Timer6T3Cnt_0 < s_i16Timer6T1Cnt_0)
		{
			i16Timer6T3Cnt_0 = i16Timer6T3Cnt_0 + EPwm6Regs.TBPRD;
		}

		i32temp4_0 = ((INT32)Theta180Cnt) - m_unPLL1ThetaRefOld_0.dword;
		i32temp2_0 = m_unPLL1ThetaRef_0.dword - m_unPLL1ThetaRefOld_0.dword;
		i32temp3_0 = i16Timer6T3Cnt_0 - s_i16Timer6T1Cnt_0;
		i32temp1_0 = (i32temp3_0 * i32temp4_0)/ i32temp2_0;
		m_i16Theta180Timer6Cnt_0 = s_i16Timer6T1Cnt_0 + i32temp1_0;

		if (((s_i16SyncTimeRecordCnt_0 >= 2) && (m_i16Theta180Timer6Cnt_0 > 5000) && (m_i16Theta180Timer6Cnt_0 < 25000))
			|| (s_i16Sync1st_0 != 0))
		{
			s_i16Sync1st_0 = 1;

			if (m_i16Theta180Timer6Cnt_0 < m_i16Theta0Timer6Cnt_0)
			{
				i16InvSyncPrdDiv2_0 = m_i16Theta180Timer6Cnt_0 - m_i16Theta0Timer6Cnt_0 + EPwm6Regs.TBPRD;
			}
			else
			{
				i16InvSyncPrdDiv2_0 = m_i16Theta180Timer6Cnt_0 - m_i16Theta0Timer6Cnt_0;
			}

			UPDNLMT16(i16InvSyncPrdDiv2_0,InvSyncPrd40HzDiv2,InvSyncPrd70HzDiv2);

			EPwm6Regs.TBPRD = m_i16Theta180Timer6Cnt_0 + i16InvSyncPrdDiv2_0;
		}
	}

	s_i16Timer6T1Cnt_0 = m_i16EPwm6RegsTBCTR_0;					//保存上一拍的计数值
}


/******************************************************************************************/
//函数名称：    Alg_Int_InvSyncRecord
//功能描述：    在进中断时记录ECap4Regs.TSCTR和EPwm6Regs.TBCTR的值
/*******************************************************************************************/
#pragma CODE_SECTION(".Epwm1Funcs")

void	Class_InverterAlgorithm::Alg_Int_InvSyncRecord()
{
		m_i16EPwm6RegsTBCTR_0 = EPwm6Regs.TBCTR;
		m_u32ECap4RegsTSCTR_0 = ECap4Regs.TSCTR;
		objTimerDrv.m_u32ClkIntStart_0 = CpuTimer0Regs.TIM.all;
}

/******************************************************************************************/
//函数名称：    Alg_Int_InvSyncPulseRec
//功能描述：    工频同步脉冲接收模块
/*******************************************************************************************/

#pragma CODE_SECTION(".Epwm1Funcs")

void	Class_InverterAlgorithm:: Alg_Int_InvSyncPulseRec()
{
    
//shuzhou 工频同步捕获
	m_i16CAPSyncFlag_0 = 0;
	m_u16InvSyncCnt_0++;
	if(ECap4Regs.ECFLG.bit.CEVT1==1)
	{
		m_u32TInvSyncClkOld_0 = m_u32TInvSyncClk_0;
		m_u32TInvSyncClk_0 = ECap4Regs.CAP1;
		ECap4Regs.ECCLR.all = 0xFFFF;			 	// 清所有标志
		m_i32TInvSync_0 = m_u32TInvSyncClk_0 - m_u32TInvSyncClkOld_0;
		UPDNLMT32(m_i32TInvSync_0,InvSyncCap45HzCnt,InvSyncCap66HzCnt);

		m_u16InvSyncCnt_0 = 0;
		m_i16CAPSyncFlag_0 = 1;
	}
	else if (m_u16InvSyncCnt_0 >= 300)
	{
		m_u16InvSyncCnt_0 = 300;
		m_i32TInvSync_0 = 0;
		m_u32TInvSyncClk_0 = ECap4Regs.TSCTR;
	}
	

}


/******************************************************************************************/
//函数名称：    Alg_ILFilterReg
//功能描述：   直流环流均流     主机不调节
/******************************************************************************************/

void	Class_InverterAlgorithm:: Alg_ILFilterReg(void)
{
	if((objMonInterface.m_st_wUPSModeRun.bConfig == 3) && ( objParallel.m_st_wParaMaint.bParaSignalMask != 1 )
		&& (objSystem.m_st_wFlagSystem.bUPSTestMode != 1) && (objXfer.m_st_XferResult.bInvKM == 1)
	  	&& (objPowCANSched.m_i16PowerCanTotalNum > 1)&&(objParallel.m_st_wParaSignalBak.bMasterOnline == 0))
	{
		if( objSystem.m_st_wFlagSystem.bTimer1ms == 1 )
		{
			m_i16IlaFilterCir_0 = m_i16IlaFilterAvg_0 - m_i16IlaFilterShare_0;
			m_unIntgIlaFilter_0.dword += ((INT32)m_i16IlaFilterCir_0 )* 20;
			UPDNLMT32(m_unIntgIlaFilter_0.dword,0x500000,-0x500000);    //80，-80
			m_i16IlaRegOut_0 = m_unIntgIlaFilter_0.half.hword;

			m_i16IlbFilterCir_0 = m_i16IlbFilterAvg_0 - m_i16IlbFilterShare_0;
			m_unIntgIlbFilter_0.dword += ((INT32)m_i16IlbFilterCir_0 )* 20;
			UPDNLMT32(m_unIntgIlbFilter_0.dword,0x500000,-0x500000);    //80，-80
			m_i16IlbRegOut_0 = m_unIntgIlbFilter_0.half.hword;

			m_i16IlcFilterCir_0 = m_i16IlcFilterAvg_0 - m_i16IlcFilterShare_0;
			m_unIntgIlcFilter_0.dword += ((INT32)m_i16IlcFilterCir_0) * 20;
			UPDNLMT32(m_unIntgIlcFilter_0.dword,0x500000,-0x500000);    //80，-80
			m_i16IlcRegOut_0 = m_unIntgIlcFilter_0.half.hword;
		}
	}
	else
	{
		m_unIntgIlaFilter_0.dword = 0;
		m_unIntgIlbFilter_0.dword = 0;
		m_unIntgIlcFilter_0.dword = 0;

		m_i16IlaRegOut_0 = 0;
		m_i16IlbRegOut_0 = 0;
		m_i16IlcRegOut_0 = 0;
	}
}	


/******************************************************************************************/
//函数名称：    Alg_InvACShareReg
//功能描述：   输出电流交流均流环
/******************************************************************************************/

void	Class_InverterAlgorithm:: Alg_InvACShareReg(void)
{
	INT16 i16Ax_0;

	if((objMonInterface.m_st_wUPSModeRun.bConfig == 3) && ( objParallel.m_st_wParaMaint.bParaSignalMask != 1 )
		&& (objSystem.m_st_wFlagSystem.bUPSTestMode != 1) && (objXfer.m_st_XferResult.bInvKM == 1)
	  	&& (objPowCANSched.m_i16PowerCanTotalNum > 1))
	{
		if( objSystem.m_st_wFlagSystem.bTimer1ms == 1)
		{

			if((objDCBus.m_i16VbusP_0 > 4400)||(objDCBus.m_i16VbusN_0 > 4400))
			{
				m_i16IntgClrDelayCnt_0++;
				if(m_i16IntgClrDelayCnt_0>=3)
				{
					m_i16IntgClrDelayCnt_0 = 3;
					m_i16KpACShare = 500;
					m_i16KiACShare = 160;
					m_i16IntgClrFlag_0 = 0;
				}
			}
			else
			{
				m_i16IntgClrDelayCnt_0--;
				if(m_i16IntgClrDelayCnt_0<=0)
				{
					m_i16IntgClrDelayCnt_0 = 0;
					m_i16KpACShare = 128;
					m_i16KiACShare = 20;
					m_i16IntgClrFlag_0 = 1;
				}	
			}
			//A相有功均流
			i16Ax_0 = m_i16IlaPAvg_0 - m_i16IlaPShare_0;
			UPDNLMT16(i16Ax_0,1000,-1000);
			m_i16IlaPShareReg_0 = m_unIntgIlaPReg_0.half.hword + ((((INT32)i16Ax_0) * m_i16KpACShare)>>12);
			if((objParallel.m_st_wParaSignalBak.bMasterOnline == 1)&&(m_i16IntgClrFlag_0 == 1))
			{
				
				if(m_unIntgIlaPReg_0.dword > 50)
				{
					m_unIntgIlaPReg_0.dword -=50;
				}
				else if(m_unIntgIlaPReg_0.dword < -50)
				{
					m_unIntgIlaPReg_0.dword +=50;
				}
				else
				{
					m_unIntgIlaPReg_0.dword =0;
				}	
			}
			else
			{
				if(abs(i16Ax_0) > 20)		//20=0.5%*4096
				{
					m_unIntgIlaPReg_0.dword += ((INT32)i16Ax_0) * m_i16KiACShare;
				}
				UPDNLMT32(m_unIntgIlaPReg_0.dword,(500L)<<16,-(500L)<<16);		//0x0033=51 51=(2.875/230)*4096 (2.875*2/230=2.5%)  220--->5.5V  230--->5.75V  240--->6V
			}	
			UPDNLMT16(m_i16IlaPShareReg_0,500,-500);

			//A相无功均流
			i16Ax_0 = m_i16IlaQAvg_0 - m_i16IlaQShare_0;
			UPDNLMT16(i16Ax_0,1000,-1000);
			m_i16IlaQShareReg_0 = m_unIntgIlaQReg_0.half.hword + ((((INT32)i16Ax_0) * m_i16KpACShare)>>12);	
			if((objParallel.m_st_wParaSignalBak.bMasterOnline == 1)&&(m_i16IntgClrFlag_0 == 1))
			{
				
				if(m_unIntgIlaQReg_0.dword > 50)
				{
					m_unIntgIlaQReg_0.dword -=50;
				}
				else if(m_unIntgIlaQReg_0.dword < -50)
				{
					m_unIntgIlaQReg_0.dword +=50;
				}
				else
				{
					m_unIntgIlaQReg_0.dword =0;
				}	
			}
			else
			{
				if(abs(i16Ax_0) > 20)		//20=0.5%*4096
				{
					m_unIntgIlaQReg_0.dword += ((INT32)i16Ax_0) * m_i16KiACShare;
				}
				UPDNLMT32(m_unIntgIlaQReg_0.dword,(500L)<<16,-(500L)<<16);		//0x0033=51 51=(2.875/230)*4096 (2.875*2/230=2.5%)  220--->5.5V  230--->5.75V  240--->6V
			}
			UPDNLMT16(m_i16IlaQShareReg_0,500,-500);

			//B相有功均流
			i16Ax_0 = m_i16IlbPAvg_0 - m_i16IlbPShare_0;
			UPDNLMT16(i16Ax_0,1000,-1000);
			m_i16IlbPShareReg_0 = m_unIntgIlbPReg_0.half.hword + ((((INT32)i16Ax_0) * m_i16KpACShare)>>12);
			if((objParallel.m_st_wParaSignalBak.bMasterOnline == 1)&&(m_i16IntgClrFlag_0 == 1))
			{
				
				if(m_unIntgIlbPReg_0.dword > 50)
				{
					m_unIntgIlbPReg_0.dword -=50;
				}
				else if(m_unIntgIlbPReg_0.dword < -50)
				{
					m_unIntgIlbPReg_0.dword +=50;
				}
				else
				{
					m_unIntgIlbPReg_0.dword =0;
				}	
			}
			else
			{
				if(abs(i16Ax_0) > 20)		//20=0.5%*4096
				{
					m_unIntgIlbPReg_0.dword += ((INT32)i16Ax_0) * m_i16KiACShare;
				}
				UPDNLMT32(m_unIntgIlbPReg_0.dword,(500L)<<16,-(500L)<<16);		//0x0033=51 51=(2.875/230)*4096 (2.875*2/230=2.5%)  220--->5.5V  230--->5.75V  240--->6V
			}	
			UPDNLMT16(m_i16IlbPShareReg_0,500,-500);

			//B相无功均流
			i16Ax_0 = m_i16IlbQAvg_0 - m_i16IlbQShare_0;
			UPDNLMT16(i16Ax_0,1000,-1000);
			m_i16IlbQShareReg_0 = m_unIntgIlbQReg_0.half.hword + ((((INT32)i16Ax_0) * m_i16KpACShare)>>12);
			if((objParallel.m_st_wParaSignalBak.bMasterOnline == 1)&&(m_i16IntgClrFlag_0 == 1))
			{
				
				if(m_unIntgIlbQReg_0.dword > 50)
				{
					m_unIntgIlbQReg_0.dword -=50;
				}
				else if(m_unIntgIlbQReg_0.dword < -50)
				{
					m_unIntgIlbQReg_0.dword +=50;
				}
				else
				{
					m_unIntgIlbQReg_0.dword =0;
				}	
			}
			else
			{
				if(abs(i16Ax_0) > 20)		//20=0.5%*4096
				{
					m_unIntgIlbQReg_0.dword += ((INT32)i16Ax_0) * m_i16KiACShare;
				}
				UPDNLMT32(m_unIntgIlbQReg_0.dword,(500L)<<16,-(500L)<<16);		//0x0033=51 51=(2.875/230)*4096 (2.875*2/230=2.5%)  220--->5.5V  230--->5.75V  240--->6V
			}	
			UPDNLMT16(m_i16IlbQShareReg_0,500,-500);

			//C相有功均流
			i16Ax_0 = m_i16IlcPAvg_0 - m_i16IlcPShare_0;
			UPDNLMT16(i16Ax_0,1000,-1000);
			m_i16IlcPShareReg_0 = m_unIntgIlcPReg_0.half.hword + ((((INT32)i16Ax_0) * m_i16KpACShare)>>12);
			if((objParallel.m_st_wParaSignalBak.bMasterOnline == 1)&&(m_i16IntgClrFlag_0 == 1))
			{
				
				if(m_unIntgIlcPReg_0.dword > 50)
				{
					m_unIntgIlcPReg_0.dword -=50;
				}
				else if(m_unIntgIlcPReg_0.dword < -50)
				{
					m_unIntgIlcPReg_0.dword +=50;
				}
				else
				{
					m_unIntgIlcPReg_0.dword =0;
				}	
			}
			else
			{
				if(abs(i16Ax_0) > 20)		//20=0.5%*4096
				{
					m_unIntgIlcPReg_0.dword += ((INT32)i16Ax_0) * m_i16KiACShare;
				}
				UPDNLMT32(m_unIntgIlcPReg_0.dword,(500L)<<16,-(500L)<<16);		//0x0033=51 51=(2.875/230)*4096 (2.875*2/230=2.5%)  220--->5.5V  230--->5.75V  240--->6V
			}
			UPDNLMT16(m_i16IlcPShareReg_0,500,-500);

			//C相无功均流
			i16Ax_0 = m_i16IlcQAvg_0 - m_i16IlcQShare_0;
			UPDNLMT16(i16Ax_0,1000,-1000);
			m_i16IlcQShareReg_0 = m_unIntgIlcQReg_0.half.hword + ((((INT32)i16Ax_0) * m_i16KpACShare)>>12);
			if((objParallel.m_st_wParaSignalBak.bMasterOnline == 1)&&(m_i16IntgClrFlag_0 == 1))
			{
				
				if(m_unIntgIlcQReg_0.dword > 50)
				{
					m_unIntgIlcQReg_0.dword -=50;
				}
				else if(m_unIntgIlcQReg_0.dword < -50)
				{
					m_unIntgIlcQReg_0.dword +=50;
				}
				else
				{
					m_unIntgIlcQReg_0.dword =0;
				}	
			}
			else
			{
				if(abs(i16Ax_0) > 20)		//20=0.5%*4096
				{
					m_unIntgIlcQReg_0.dword += ((INT32)i16Ax_0) * m_i16KiACShare;
					UPDNLMT32(m_unIntgIlcQReg_0.dword,(500L)<<16,-(500L)<<16);		//0x0033=51 51=(2.875/230)*4096 (2.875*2/230=2.5%)  220--->5.5V  230--->5.75V  240--->6V
				}
			}	
			UPDNLMT16(m_i16IlcQShareReg_0,500,-500);
		}
	}
	else
	{
		m_i16IlaPShareReg_0 = 0;
		m_unIntgIlaPReg_0.dword = 0;
		m_i16IlaQShareReg_0 = 0;
		m_unIntgIlaQReg_0.dword = 0;

		m_i16IlbPShareReg_0 = 0;
		m_unIntgIlbPReg_0.dword = 0;
		m_i16IlbQShareReg_0 = 0;
		m_unIntgIlbQReg_0.dword = 0;

		m_i16IlcPShareReg_0 = 0;
		m_unIntgIlcPReg_0.dword = 0;
		m_i16IlcQShareReg_0 = 0;
		m_unIntgIlcQReg_0.dword = 0;
	}
}	

//===========================================================================
// End of file.
//===========================================================================
