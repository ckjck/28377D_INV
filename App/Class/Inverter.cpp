/*==========================================================*/ 	
/* Title		:	Inverter.cpp													*/ 	
/* Description	: 	Class_Inverter definition										*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#include "Inverter.h"

/************************************************************************************
函数名：	App_InvVarInit()	
功能描述：	Inverter类变量初始化		
函数调用：	
***********************************************************************************/
	 void	Class_Inverter::App_InvVarInit(void)
	{
		//Output Rating		
		m_i16VrefSetMd=5792;
		
		m_i16KVinva_0 = 4119;         //定标系数
		m_i16KVinvb_0 = 4119;         //定标系数
		m_i16KVinvc_0 = 4119;         //定标系数
		
		m_i16KIinva_0 = 4055;
		m_i16KIinvb_0 = 4055;
		m_i16KIinvc_0 = 4055;		
		
		m_i16KIl_0 = 15207;
	
		//采样系数设置
		m_i16KIlFilter_0 = 2672;
		m_i16KVinvdc_0 = 4509;
		//温度授权率
		m_i16TempKWGrant = 1138;
		m_i16TempKVAGrant = 1024;
		m_i16SystemPF = 1138;
		
		m_i16PointcntInvM_0 = K50HzPointCntCnst;	

		m_i16PointcntInvMDtheta = K50HzPointCntCnst;
	}
/************************************************************************************
函数名：App_InvParamConfig()
************************************************************************************/
	void	Class_Inverter::App_InvParamConfig(void)
	{														
		//为提高精度，电感电流定标系数人为放大3倍，使用时再人为缩小1/3
		switch(objMonInterface.m_st_wUPSModeRun.bVoltOut)
		{
			case 0:												//400v
				m_i16KVinvNormal = 4119;                      //定标系数
				m_i16KIl_0 = 15207;			
				m_i16KIinvNormal = 4055;
				m_i16KIlFilter_0 = 2672;
				objMonInterface.m_i16VoltDispScale = 2200;
				break;
			case 1:												//380v
				m_i16KVinvNormal = 4119;                      //定标系数
				m_i16KIl_0 = 15207;			
				m_i16KIinvNormal = 4055;
				m_i16KIlFilter_0 = 2672;
				objMonInterface.m_i16VoltDispScale = 2200;
				break;
			case 2:												//415v
				m_i16KVinvNormal = 4119;                      //定标系数
				m_i16KIl_0 = 15207;			
				m_i16KIinvNormal = 4055;
				m_i16KIlFilter_0 = 2672;
				objMonInterface.m_i16VoltDispScale= 2200;
				break;
		
			default:
				break;		
		}
		
		
		//逆变电压定标系数校正
		m_i16KVinva_0 = (((INT32)objMonInterface.m_i16KsVinvaSet)*m_i16KVinvNormal)>>12;
		m_i16KVinvb_0 = (((INT32)objMonInterface.m_i16KsVinvbSet)*m_i16KVinvNormal)>>12;
		m_i16KVinvc_0 = (((INT32)objMonInterface.m_i16KsVinvcSet)*m_i16KVinvNormal)>>12;
		
		//逆变电流定标系数校正--Nxb Ext产品屏蔽逆变电流校正			
		m_i16KIinva_0 = (((INT32)objMonInterface.m_i16KsIinvaSet)*m_i16KIinvNormal)>>12;
		m_i16KIinvb_0 = (((INT32)objMonInterface.m_i16KsIinvbSet)*m_i16KIinvNormal)>>12;
		m_i16KIinvc_0 = (((INT32)objMonInterface.m_i16KsIinvcSet)*m_i16KIinvNormal)>>12;
//		m_i16KIinva_0 = objOutput.m_i16KIouta_0;				      
//		m_i16KIinvb_0 = objOutput.m_i16KIoutb_0;				
//		m_i16KIinvc_0 = objOutput.m_i16KIoutc_0;				

		
	}

/************************************************************************************
函数名：	Alg_InvFault()
功能描述：	逆变故障判断模块
函数调用：	无			
************************************************************************************/
	void	Class_Inverter::Alg_InvFault(void)
	{
		 if ( ( objInverter.m_st_wFlagInv.bInv == 3 ) && ( m_st_FlagVinvABC.bVinvRMS == 1 ) )//逆变器启动完毕且逆变电压异常
		 {
		 	//电压检测值低于660V， 或已检测出母线异常（正负低、DC-OV等）
			if (  (objDCBus.m_i16Vdc_0 < VdcLowCnst )
			     || (objSystem.m_st_wFlagFault.bDCAbnormal == 1)||(objSystem.m_st_wFlagFault.bDCOV == 1))
			
			{
				objInvCtrl.m_st_wFlagInvOnoff.bManualRecoverOff = 1;					//置可人为恢复故障关机位
				objSystem.m_st_wFlagPrompt.bDCAbnormalOff = 1;							//置母线电压异常关机
				objDebug.m_st_wDebug.bDCAbnormalOff4 = 1;
			}
			else
			{
				objSystem.m_st_wFlagFault.bInvFault = 1;								//置逆变故障
				objInvCtrl.m_st_wFlagInvOnoff.bInvFaultOff = 1;							//置逆变故障关机
				objSystem.m_st_wFlagStatus.bInvFault = 1;								//置逆变故障状态
				objDebug.m_st_wDebug.bInvFault1 = 1;
			}
		  }

		
		if (( objInvAlg.m_st_wIntMainFlag.bVinvLoadFast == 1) && (objXfer.m_st_wXferCmd.bSwitchStatus == 1) 
			&& (objSwitches.m_st_FlagBreaker.bInvKMOpen == 0) && (objSwitches.m_st_FlagBreaker.bFuse == 1))
		{
		  	objSystem.m_st_wFlagFault.bInvFault = 1;						   			//置逆变故障
		   	objInvCtrl.m_st_wFlagInvOnoff.bInvFaultOff = 1;					   			//置逆变故障关机
	   		objSystem.m_st_wFlagStatus.bInvFault = 1;						   			//置逆变故障状态
			objDebug.m_st_wDebug.bInvFault2 = 1;
		}
	}
	
/************************************************************************************
函数名：	Dat_VinvFlag()			
功能描述：	逆变标志处理模块
函数调用：	无
************************************************************************************/
	void	Class_Inverter::Dat_VinvFlag(void)
	{
		INT16	i16MainAX,i16MainBX,i16MainCX,i16MainDX,i16MainEX,i16MainFX;
		INT16	i16VinvFaultCnt;
		if(objSystem.m_st_wFlagSystem.bTimer14ms == 0) 
			return;

		if((objInverter.m_st_wFlagInv.bInv != 3)||(objDigitalIO.m_un_ExOutAReg.BIT.bINVKM == 0)) 
		{
			m_st_FlagVinvABC.bVinvRMS = 0;
			m_st_FlagVinvABC.bVoutAbnormal = 0;
			m_i16VinvFault1CntPer14ms = 0;
			m_i16VinvFault2CntPer14ms = 0;
			m_i16VoutAbnormalCntPer14ms = 0;
			return;	
		}	

		i16MainCX = MAX2(m_i16VinvaPrms,MAX2(m_i16VinvbPrms,m_i16VinvcPrms));	//Max V
		i16MainDX = MIN2(m_i16VinvaPrms,MIN2(m_i16VinvbPrms,m_i16VinvcPrms));	//Min V
		i16MainEX = MAX2(m_i16IinvaPrms,MAX2(m_i16IinvbPrms,m_i16IinvcPrms));	//Max I		

		//自老化模式下，逆变器故障判断基准取旁路模值
		if(objSystem.m_st_wFlagSystem.bSelfAgingAction == 1)
		{
			i16MainFX = (((INT32)(objBypass.m_unVbpMMean_0.half.hword))*724)>>12;//Md/1.414/4=Prms
		}
		else
		{
			i16MainFX = (((INT32)objInvAlg.m_i16wVrefRunMd_0)*724)>>12;				//Md/1.414/4=Prms
		}
	
		if(objSystem.m_st_wFlagSystem.bSelfAgingAction == 1)
		{
			i16MainBX = (((INT32)i16MainFX)*3072)>>12;						//75% dnlim	
			i16MainAX = (((INT32)i16MainFX)*4710)>>12;						//115% uplim
			i16VinvFaultCnt = 10;
		}
		else
		{
			i16MainBX = (((INT32)i16MainFX)*3277)>>12;						//80% dnlim	
			i16MainAX = (((INT32)i16MainFX)*4506)>>12;						//110% uplim
			i16VinvFaultCnt = 3;
		}
		//自老化模式下，逆变器故障不要求屏蔽冲击电流
		if(((i16MainCX>i16MainAX) || (i16MainDX<i16MainBX)) && ((i16MainEX<717)||(objSystem.m_st_wFlagSystem.bSelfAgingAction == 1)) )
		{	
			if(m_i16VinvFault1CntPer14ms<10)
			{
				m_i16VinvFault1CntPer14ms++;
			}

		}
		//zxf20050412当有效值超过130%时，勿需屏蔽冲击电流,
		else if(i16MainCX > 1331)
		{	
			if(m_i16VinvFault1CntPer14ms < 10)
			{
				m_i16VinvFault1CntPer14ms++;
			}
		}
		else
		{	
			if(m_i16VinvFault1CntPer14ms > 0)
			{
				m_i16VinvFault1CntPer14ms--;
			}
		}

		if(m_i16VinvFault1CntPer14ms >= i16VinvFaultCnt)
		{
			m_st_FlagVinvABC.bVinvRMS = 1;
		}

		//长期逆变故障缩减为0.5秒
		//PARADIGM中电池不是直接挂在母线上，不存在长期不能稳压的情况。
		if((8*i16MainCX > 9*i16MainFX) || (8*i16MainFX > 9*i16MainDX) )
		{
			m_i16VinvFault2CntPer14ms++; 
		}
		else
		{
			m_i16VinvFault2CntPer14ms -= 8; 
		}
		UPDNLMT16(m_i16VinvFault2CntPer14ms,40,0);

		if(m_i16VinvFault2CntPer14ms >= 36)					// 0.5s
		{
			m_st_FlagVinvABC.bVinvRMS = 1;
		}
			
		//zqf 060717--增加输出电压异常的有效值判据
		if(0)//objSystem.m_st_wFlagSystem.bSelfAgingAction == 0)
       {
		{
		    i16MainAX = (((INT32)i16MainFX)*4915)>>12;						//120% uplim
			i16MainBX = (((INT32)i16MainFX)*2867)>>12;						//70% dnlim	
			i16MainCX = MAX2(objOutput.m_i16VoutaPrms,MAX2(objOutput.m_i16VoutbPrms,objOutput.m_i16VoutcPrms));	//Max   Vout
		    i16MainDX = MIN2(objOutput.m_i16VoutaPrms,MIN2(objOutput.m_i16VoutbPrms,objOutput.m_i16VoutcPrms)); //Min   Vout
			
			if((i16MainCX > i16MainAX) ||( i16MainDX < i16MainBX))
			{
				m_i16VoutAbnormalCntPer14ms++;
				if(m_i16VoutAbnormalCntPer14ms >= i16VinvFaultCnt)
				{
					m_st_FlagVinvABC.bVoutAbnormal=1;
				}
			}
			else
			{
				m_i16VoutAbnormalCntPer14ms--;
				if(m_i16VoutAbnormalCntPer14ms<=0)
			{
					m_st_FlagVinvABC.bVoutAbnormal=0;
				}
			}
			UPDNLMT16(m_i16VoutAbnormalCntPer14ms,66,0);
		}
//		else
		{
			m_i16VoutAbnormalCntPer14ms = 0;
			m_st_FlagVinvABC.bVoutAbnormal = 0;
		}
		}

	}

/************************************************************************************
//函数名称：    Dat_Int_InvPreDeal()
************************************************************************************/

#pragma CODE_SECTION(".Epwm1Funcs") 			

void 	Class_Inverter::Dat_Int_InvPreDeal(void)
{	
	INT16	i16IntAx_0,i16IntAlpha_0,i16IntBeta_0;
	INT16	i16IntBx_0;
	//读取采样值
	m_i16Ila_0 = objADDrv.m_i16ADC_Ila_0;
	m_i16Ilb_0 = objADDrv.m_i16ADC_Ilb_0;
	m_i16Ilc_0 = objADDrv.m_i16ADC_Ilc_0; 
	
	m_i16Iinva_0 = objADDrv.m_i16ADC_Iinva_0; 
	m_i16Iinvb_0 = objADDrv.m_i16ADC_Iinvb_0; 
	m_i16Iinvc_0 = objADDrv.m_i16ADC_Iinvc_0; 
	
	m_i16Vinva_0 = objADDrv.m_i16ADC_Vinva_0;
	m_i16Vinvb_0 = objADDrv.m_i16ADC_Vinvb_0;
	m_i16Vinvc_0 = objADDrv.m_i16ADC_Vinvc_0;
	 
	m_i16VinvaDc_0 = objADDrv.m_i16ADC_VinvaDc_0;
	m_i16VinvbDc_0= objADDrv.m_i16ADC_VinvbDc_0;
	m_i16VinvcDc_0 = objADDrv.m_i16ADC_VinvcDc_0;
 
	 
	m_i16IlaFilter_0 = objADDrv.m_i16ADC_IlaFilter_0;
	m_i16IlbFilter_0 = objADDrv.m_i16ADC_IlbFilter_0;
	m_i16IlcFilter_0 = objADDrv.m_i16ADC_IlcFilter_0;
	
	// 电感电流计算，定标为7.9
	m_i16Ila_0 = (((INT32)m_i16Ila_0)*m_i16KIl_0)>>9;
	m_i16Ilb_0 = (((INT32)m_i16Ilb_0)*m_i16KIl_0)>>9;
	m_i16Ilc_0 = (((INT32)m_i16Ilc_0)*m_i16KIl_0)>>9;
    
    // 逆变电流计算，定标为7.9                             
	m_i16Iinva_0 = -(((INT32)m_i16Iinva_0)*m_i16KIinva_0)>>9; 
	m_i16Iinvb_0 = -(((INT32)m_i16Iinvb_0)*m_i16KIinvb_0)>>9; 
	m_i16Iinvc_0 = -(((INT32)m_i16Iinvc_0)*m_i16KIinvc_0)>>9; 

	// 逆变电压直流分量计算，定标为5.11
	m_i16VinvaDc_0 = (((INT32)m_i16VinvaDc_0)*m_i16KVinvdc_0)>>11;
	m_i16VinvbDc_0 = (((INT32)m_i16VinvbDc_0)*m_i16KVinvdc_0)>>11;
	m_i16VinvcDc_0 = (((INT32)m_i16VinvcDc_0)*m_i16KVinvdc_0)>>11;
	
    // 逆变相电压计算，定标为6.10
	m_i16Vinva_0 = ((((INT32)m_i16Vinva_0)*m_i16KVinva_0)>>10)+ objInvAlg.m_i16VdcaCon_0 + m_i16VbusDiff_0;
	m_i16Vinvb_0 = ((((INT32)m_i16Vinvb_0)*m_i16KVinvb_0)>>10)+ objInvAlg.m_i16VdcbCon_0 + m_i16VbusDiff_0;
	m_i16Vinvc_0 = ((((INT32)m_i16Vinvc_0)*m_i16KVinvc_0)>>10)+ objInvAlg.m_i16VdccCon_0 + m_i16VbusDiff_0;
    //电感电流直流分量计算，定标为4.12
	m_i16IlaFilter_0 = (((INT32)m_i16IlaFilter_0)*m_i16KIlFilter_0)>>12;
	m_i16IlbFilter_0 = (((INT32)m_i16IlbFilter_0)*m_i16KIlFilter_0)>>12;
	m_i16IlcFilter_0 = (((INT32)m_i16IlcFilter_0)*m_i16KIlFilter_0)>>12;
	
	m_unIlaFilter_0.dword = FILTER(8, m_i16IlaFilter_0, m_unIlaFilter_0.dword);
	m_unIlbFilter_0.dword = FILTER(8, m_i16IlbFilter_0, m_unIlbFilter_0.dword);
	m_unIlcFilter_0.dword = FILTER(8, m_i16IlcFilter_0, m_unIlcFilter_0.dword);
	
	UPDNLMT32(m_unIlaFilter_0.dword,0x05000000,-0x05000000);				//限幅8A
	UPDNLMT32(m_unIlbFilter_0.dword,0x05000000,-0x05000000);				//限幅8A
	UPDNLMT32(m_unIlcFilter_0.dword,0x05000000,-0x05000000);				//限幅8A

	
//	m_i16VaNoDc_0 = m_i16Vinva_0 - objInvAlg.m_i16VdcaCon_0 - m_i16VbusDiff_0;
//	m_i16VbNoDc_0 = m_i16Vinvb_0 - objInvAlg.m_i16VdcaCon_0 - m_i16VbusDiff_0;
//	m_i16VcNoDc_0 = m_i16Vinvc_0 - objInvAlg.m_i16VdccCon_0 - m_i16VbusDiff_0;
	
	if((objDCBus.m_i16VbusN_0 > 4328)
	&&(objDCBus.m_i16VbusN_0 > objDCBus.m_i16VbusP_0 + 50))
	{
		i16IntBx_0 = objDCBus.m_i16VbusN_0 - objDCBus.m_i16VbusP_0;
	}
	else if((objDCBus.m_i16VbusP_0 > 4328)
		&&(objDCBus.m_i16VbusP_0 > objDCBus.m_i16VbusN_0 + 50))
	{
		i16IntBx_0 = objDCBus.m_i16VbusN_0 - objDCBus.m_i16VbusP_0;
	}
	else
	{
		i16IntBx_0 = 0;
	}
	m_i16VbusDiff_0 = (((INT32)i16IntBx_0) * 450)>>10;
	UPDNLMT16(m_i16VbusDiff_0,150,-150);	
	
	/*i16IntBx_0 = objDCBus.m_i16VbusN_0 - objDCBus.m_i16VbusP_0;
	m_unVbusSubFilter_0.dword = FILTER(4, i16IntBx_0, m_unVbusSubFilter_0.dword);
	i16IntCx_0 = (((INT32)m_unVbusSubFilter_0.half.hword) * 100)>>10;
	UPDNLMT16(i16IntCx_0,50,-50);
	m_i16VbusDiff_0 = i16IntCx_0;*/
	
	//objDebug.m_i16DebugVar2_0 = i16IntCx_0;																

	
		// 电感电流3/2变换
		i16IntAlpha_0 = (((INT32)m_i16Ila_0*2-m_i16Ilb_0-m_i16Ilc_0)*K1div3Cnst)>>12;
		i16IntBeta_0 = (((INT32)m_i16Ilb_0-m_i16Ilc_0)*KSqrt3invCnst)>>12;
		m_i16IlMz_0 = (((INT32)m_i16Ila_0+m_i16Ilb_0+m_i16Ilc_0)*KSqrt2Div3Cnst)>>12;
		
		m_i16IlMd_0 = (((INT32)i16IntBeta_0)*objInvAlg.m_i16SinQ0Ref_0+((INT32)i16IntAlpha_0)*objInvAlg.m_i16CosQ0Ref_0)>>12;
		m_i16IlMq_0 = (((INT32)i16IntBeta_0)*objInvAlg.m_i16CosQ0Ref_0-((INT32)i16IntAlpha_0)*objInvAlg.m_i16SinQ0Ref_0)>>12;	

	    // 逆变电压3/2变换，矢量模与相位处理
		i16IntAlpha_0 = (((INT32)m_i16Vinva_0*2-m_i16Vinvb_0-m_i16Vinvc_0)*K1div3Cnst)>>12;
		i16IntBeta_0 = (((INT32)m_i16Vinvb_0-m_i16Vinvc_0)*KSqrt3invCnst)>>12;
	      
		m_i16VinvMd_0 = (((INT32)i16IntBeta_0)*objInvAlg.m_i16SinQ0Ref_0+(INT32)i16IntAlpha_0*objInvAlg.m_i16CosQ0Ref_0)>>12;
		m_i16VinvMq_0 = (((INT32)i16IntBeta_0)*objInvAlg.m_i16CosQ0Ref_0-(INT32)i16IntAlpha_0*objInvAlg.m_i16SinQ0Ref_0)>>12;
		m_i16VinvMz_0 = (((INT32)m_i16Vinva_0+m_i16Vinvb_0+m_i16Vinvc_0)*KSqrt2Div3Cnst)>>12;

		i16IntAx_0 = (((INT32)i16IntAlpha_0)*i16IntAlpha_0+((INT32)i16IntBeta_0)*i16IntBeta_0)>>16;
		UPDNLMT16(i16IntAx_0,1023,1);
		m_i16VinvM_0 = sqrtTab[i16IntAx_0];
		m_i16SinQInv_0 = (((INT32)i16IntBeta_0)<<12)/m_i16VinvM_0;
		m_i16CosQInv_0 = (((INT32)i16IntAlpha_0)<<12)/m_i16VinvM_0;
			
	    // 逆变电流3/2变换
		i16IntAlpha_0 = ((((INT32)m_i16Iinva_0)*2-m_i16Iinvb_0-m_i16Iinvc_0)*K1div3Cnst)>>12;
		i16IntBeta_0 = (((INT32)m_i16Iinvb_0-m_i16Iinvc_0)*KSqrt3invCnst)>>12;
		
		m_i16IinvMd_0 = (((INT32)i16IntBeta_0)*objInvAlg.m_i16SinQ0Ref_0+(INT32)i16IntAlpha_0*objInvAlg.m_i16CosQ0Ref_0)>>12;
		m_i16IinvMq_0 = (((INT32)i16IntBeta_0)*objInvAlg.m_i16CosQ0Ref_0-(INT32)i16IntAlpha_0*objInvAlg.m_i16SinQ0Ref_0)>>12;
		m_i16IinvMz_0 = (((INT32)m_i16Iinva_0+m_i16Iinvb_0+m_i16Iinvc_0)*KSqrt2Div3Cnst)>>12;

		m_i16IinvMd_1 = m_i16IinvMd_0;
		m_i16IinvMq_1 = m_i16IinvMq_0;
		m_i16IinvMz_1 = m_i16IinvMz_0;
		
		UPDNLMT16(m_i16IinvMd_1,13148,-13148);
		UPDNLMT16(m_i16IinvMq_1,13148,-13148);
		UPDNLMT16(m_i16IinvMz_1,13148,-13148);
		// 用于环流控制逆变电流3/2变换
		m_i16Iinva2_0 = m_i16Iinva_0 + (m_unIlaFilter_0.half.hword>>1);			//电感电流只检测了一半
		m_i16Iinvb2_0 = m_i16Iinvb_0 + (m_unIlbFilter_0.half.hword>>1);			//电感电流只检测了一半
		m_i16Iinvc2_0 = m_i16Iinvc_0 + (m_unIlcFilter_0.half.hword>>1);	

		m_i16IlaFilter2_0 = (m_unIlaFilter_0.half.hword>>1);
		m_i16IlbFilter2_0 = (m_unIlbFilter_0.half.hword>>1);
		m_i16IlcFilter2_0 = (m_unIlcFilter_0.half.hword>>1);
		
		/*i16IntAlpha_0 = (((INT32)m_i16Iinva2_0*2-m_i16Iinvb2_0-m_i16Iinvc2_0)*K1div3Cnst)>>12;
		i16IntBeta_0 = (((INT32)m_i16Iinvb2_0-m_i16Iinvc2_0)*KSqrt3invCnst)>>12;
		m_i16IinvMz2_0 = (((INT32)m_i16Iinva2_0+m_i16Iinvb2_0+m_i16Iinvc2_0)*KSqrt2Div3Cnst)>>12;
		m_i16IinvMz2_0 += i16IntCx_0;


		m_i16IinvMd2_0 = (((INT32)i16IntBeta_0)*objInvAlg.m_i16SinQ0Ref_0+(INT32)i16IntAlpha_0*objInvAlg.m_i16CosQ0Ref_0)>>12;
		m_i16IinvMq2_0 = (((INT32)i16IntBeta_0)*objInvAlg.m_i16CosQ0Ref_0-(INT32)i16IntAlpha_0*objInvAlg.m_i16SinQ0Ref_0)>>12;*/

		m_i16IinvMd2_0 = m_i16IinvMd_1;
		//m_i16IinvMq2_0 = m_i16IinvMq_1;
		//m_i16IinvMz2_0 = m_i16IinvMz_1+i16IntCx_0; 
		m_unIinvMdFilter_0.dword += ((((INT32)m_i16IinvMd2_0)<<16) - m_unIinvMdFilter_0.dword)>>7;		//用于通过CAN在并机间传输
		//m_unIinvMqFilter_0.dword += ((((INT32)m_i16IinvMq2_0)<<16) - m_unIinvMqFilter_0.dword)>>7;		//用于通过CAN在并机间传输
		//m_unIinvMzFilter_0.dword += ((((INT32)m_i16IinvMz2_0)<<16) - m_unIinvMzFilter_0.dword);				//用于通过CAN在并机间传输*/
	
	
}

/************************************************************************************
//函数名称：    Dat_VinvRMS()
************************************************************************************/
void 	Class_Inverter::Dat_VinvRMS(void)
{
	INT32 	i32temp;
	INT16 	i16temp;
	//逆变相电压有效值计算
	i32temp = m_unVinvaSumM_0.dword/m_i16PointcntInvM_0;
	m_i16VinvaPrms = isqrt(i32temp);
	m_unVinvaRms.dword += ((((INT32)m_i16VinvaPrms)<<16)-m_unVinvaRms.dword)>>K30msFilterCnst;
	
	i32temp = m_unVinvbSumM_0.dword/m_i16PointcntInvM_0;
	m_i16VinvbPrms = isqrt(i32temp);
	m_unVinvbRms.dword += ((((INT32)m_i16VinvbPrms)<<16)-m_unVinvbRms.dword)>>K30msFilterCnst;
	
	i32temp = m_unVinvcSumM_0.dword/m_i16PointcntInvM_0;
	m_i16VinvcPrms = isqrt(i32temp);
	m_unVinvcRms.dword += ((((INT32)m_i16VinvcPrms)<<16)-m_unVinvcRms.dword)>>K30msFilterCnst;

	i32temp = (INT32)(m_unDthetaRefBase_0.half.hword);
	m_unDthetaRefMean_0.dword += ((i32temp<<16) -m_unDthetaRefMean_0.dword)>>K30msFilterCnst;

	i32temp = ((INT32)(m_unDthetaRefMean_0.half.hword))*KFMeanCalCnst;
	m_i16FinvMean = i32temp>>11;

	//逆变线电压有效值计算
	i32temp = m_unVinvabSumM_0.dword/m_i16PointcntInvM_0;
	m_i16VinvabPrms = isqrt(i32temp);
//	m_unVinvabRms.dword += ((((INT32)m_i16VinvabPrms)<<16)-m_unVinvabRms.dword)>>K30msFilterCnst;

	i32temp = m_unVinvbcSumM_0.dword/m_i16PointcntInvM_0;
	m_i16VinvbcPrms = isqrt(i32temp);
//	m_unVinvbcRms.dword += ((((INT32)m_i16VinvbcPrms)<<16)-m_unVinvbcRms.dword)>>K30msFilterCnst;
	
	i32temp = m_unVinvcaSumM_0.dword/m_i16PointcntInvM_0;
	m_i16VinvcaPrms = isqrt(i32temp);
//	m_unVinvcaRms.dword += ((((INT32)m_i16VinvcaPrms)<<16)-m_unVinvcaRms.dword)>>K30msFilterCnst;
					
	i16temp = (((INT32)360)<<10)/objInvAlg.m_i16DthetaRefInst_0;
	//什么玩意
	if(((((INT32)360)<<10) - (((INT32)i16temp) * objInvAlg.m_i16DthetaRefInst_0))>=(objInvAlg.m_i16DthetaRefInst_0>>1))
	{
		i16temp++;
	}
	UPDNLMT16(i16temp,K66HzPointCntCnst,K45HzPointCntCnst);
	m_i16PointcntInvMDtheta = i16temp;
}

/************************************************************************************
//函数名称：    Dat_IinvRMS()       等效于sqrt((Iinv*Iinv)>>4)
************************************************************************************/
void 	Class_Inverter::Dat_IinvRMS(void)
{
	INT32	i32temp;
	//逆变电流有效值计算
	i32temp = m_unIinvaSumM_0.dword/m_i16PointcntInvM_0;
	m_i16IinvaPrms = isqrt(i32temp);
	m_unIinvaRms.dword += ((((INT32)m_i16IinvaPrms)<<16)-m_unIinvaRms.dword)>>K30msFilterCnst;
	
	i32temp = m_unIinvbSumM_0.dword/m_i16PointcntInvM_0;
	m_i16IinvbPrms = isqrt(i32temp);
	m_unIinvbRms.dword += ((((INT32)m_i16IinvbPrms)<<16)-m_unIinvbRms.dword)>>K30msFilterCnst;
	
	i32temp = m_unIinvcSumM_0.dword/m_i16PointcntInvM_0;
	m_i16IinvcPrms = isqrt(i32temp);
	m_unIinvcRms.dword += ((((INT32)m_i16IinvcPrms)<<16)-m_unIinvcRms.dword)>>K30msFilterCnst;

	//电感电流有效值计算
	i32temp = m_unIindaSumM_0.dword/m_i16PointcntInvM_0;
	m_i16IindaPrms = isqrt(i32temp);
	m_unIindaRms.dword += ((((INT32)m_i16IindaPrms)<<16)-m_unIindaRms.dword)>>K30msFilterCnst;
	
	i32temp = m_unIindbSumM_0.dword/m_i16PointcntInvM_0;
	m_i16IindbPrms = isqrt(i32temp);
	m_unIindbRms.dword += ((((INT32)m_i16IindbPrms)<<16)-m_unIindbRms.dword)>>K30msFilterCnst;
	
	i32temp = m_unIindcSumM_0.dword/m_i16PointcntInvM_0;
	m_i16IindcPrms = isqrt(i32temp);
	m_unIindcRms.dword += ((((INT32)m_i16IindcPrms)<<16)-m_unIindcRms.dword)>>K30msFilterCnst;

	//发送给监控的逆变电流、电感电流按授权率折算    实际逆变电流放大10倍上传

	
	m_i16IinvaforShow = (((INT32)m_unIinvaRms.half.hword)*objSystem.m_i161DivGrantRate)>>10;        
	m_i16IinvbforShow = (((INT32)m_unIinvbRms.half.hword)*objSystem.m_i161DivGrantRate)>>10;
	m_i16IinvcforShow = (((INT32)m_unIinvcRms.half.hword)*objSystem.m_i161DivGrantRate)>>10;
		
	m_i16IlaforShow = (((INT32)m_unIindaRms.half.hword)*objSystem.m_i161DivGrantRate)>>10;
	m_i16IlbforShow = (((INT32)m_unIindbRms.half.hword)*objSystem.m_i161DivGrantRate)>>10;
	m_i16IlcforShow = (((INT32)m_unIindcRms.half.hword)*objSystem.m_i161DivGrantRate)>>10;
	
}
/************************************************************************************
//函数名称：    Dat_TempPowerGrant()
************************************************************************************/
void 	Class_Inverter::Dat_TempPowerGrant()
{
	if(objSystem.m_st_wFlagSystem.bTimer1ms == 1)
	{
		if(m_i16wEnvirTemp >= 1078)			//环境温度0-30度输出功率不降额
		{
			m_i16TempGrant1Cnt++;
			if(m_i16TempGrant1Cnt >=100)			//100ms确认
			{
				if(objDigitalIO.m_st_wLocalNewSignal.bOUT_PF == 1)
				{
					m_i16SystemPF = 1138;		//PF = 0.9 
				}	
				else
				{
					m_i16SystemPF = 1280;		//PF = 0.8 
				}
				m_i16TempKVAGrant = 1024;			//KVA
				m_i16TempKWGrant = (((INT32)m_i16TempKVAGrant) *  m_i16SystemPF)>>10;
				m_i16TempGrant1Cnt = 100;
				m_i16TempGrant8Cnt = 0;
				m_i16TempGrant9Cnt = 0;
			}
		}
		else if(m_i16wEnvirTemp <= 1068)		//环境温度30-35度
		{
			m_i16TempGrant9Cnt ++;
			if(m_i16TempGrant9Cnt >=100)						//100ms确认
			{
				if(objDigitalIO.m_st_wLocalNewSignal.bOUT_PF == 1)
				{
					m_i16SystemPF = 1280;			//PF = 0.8 
				}
				else
				{
					m_i16SystemPF = 1462;			//PF = 0.7 
				}	
				m_i16TempKVAGrant = 1024;			//KVA
				m_i16TempKWGrant = (((INT32)m_i16TempKVAGrant) *  m_i16SystemPF)>>10;
				m_i16TempGrant9Cnt = 100;
				m_i16TempGrant1Cnt = 0;
				m_i16TempGrant8Cnt = 0;
			}
			
		}		
	}

	m_i16SystemPF = 1024;			    //PF = 1
	m_i16TempKVAGrant = 1024;			//KVA
	m_i16TempKWGrant = (((INT32)m_i16TempKVAGrant) *  m_i16SystemPF)>>10;
	m_i16TempGrant9Cnt = 100;
	m_i16TempGrant1Cnt = 0;
	m_i16TempGrant8Cnt = 0;

}


/************************************************************************************
//函数名称：    Dat_PinvRMS()
************************************************************************************/
void 	Class_Inverter::Dat_PinvRMS(void)
{
	INT32	i32temp;
	INT16	i16_temp1,i16_temp2;
	
		i32temp = m_unPinvaAccM_0.dword/m_i16PointcntInvM_0;

		m_i16Pinva =i32temp>>10;									//功率因数为1
	//	m_i16Pinva = ((i32temp>>10)*1138)>>10;					//(1/0.9=1138/1024)
		m_i16Sinva = (((INT32)m_i16VinvaPrms) * m_i16IinvaPrms)>>10;		//逆变A相总输出容量

		i32temp = m_unPinvbAccM_0.dword/m_i16PointcntInvM_0;

		m_i16Pinvb = i32temp>>10;
	//	m_i16Pinvb = ((i32temp>>10)*1138)>>10;					//(1/0.9=1138/1024)
		m_i16Sinvb = (((INT32)m_i16VinvbPrms) * m_i16IinvbPrms)>>10;		//逆变B相总输出容量

		i32temp = m_unPinvcAccM_0.dword/m_i16PointcntInvM_0;

		m_i16Pinvc = i32temp>>10;
	//	m_i16Pinvc = ((i32temp>>10)*1138)>>10;					//(1/0.9=1138/1024)
		m_i16Sinvc = (((INT32)m_i16VinvcPrms) * m_i16IinvcPrms)>>10;		//逆变C相总输出容量

	//自老化下的输出负载率显示处理(自老化时输出功率因数为1)
	if(objSystem.m_st_wFlagSystem.bSelfAgingAction == 1)
	{
		m_i16SinvaforShow = (((INT32)m_unIinvaRms.half.hword) * m_unVinvaRms.half.hword)*11/5>>10;
		m_i16SinvbforShow = (((INT32)m_unIinvbRms.half.hword) * m_unVinvbRms.half.hword)*11/5>>10;
		m_i16SinvcforShow = (((INT32)m_unIinvcRms.half.hword) * m_unVinvcRms.half.hword)*11/5>>10;

		m_i16PinvaforShow = m_i16SinvaforShow;
		m_i16PinvbforShow = m_i16SinvbforShow;
		m_i16PinvcforShow = m_i16SinvcforShow;
	}
	
	//功率按授权率折算  实际功率放大一百倍上传
	else
	{
		m_i16PinvaforShow = (((INT32)m_i16Pinva) * ((objSystem.m_i161DivGrantRate*11)/5))>>10;          //1024为满载功率
		m_i16SinvaforShow = (((INT32)m_i16Sinva) * ((objSystem.m_i161DivGrantRate*11)/5))>>10;
		
		m_i16PinvbforShow = (((INT32)m_i16Pinvb) * ((objSystem.m_i161DivGrantRate*11)/5))>>10;
		m_i16SinvbforShow = (((INT32)m_i16Sinvb) * ((objSystem.m_i161DivGrantRate*11)/5))>>10;
			
		m_i16PinvcforShow = (((INT32)m_i16Pinvc )* ((objSystem.m_i161DivGrantRate*11)/5))>>10;
		m_i16SinvcforShow = (((INT32)m_i16Sinvc) * ((objSystem.m_i161DivGrantRate*11)/5))>>10;	
	}



	m_i16PFinva = ((INT32)m_i16Pinva*100)/m_i16Sinva;
	m_i16PFinvb = ((INT32)m_i16Pinvb*100)/m_i16Sinvb;
	m_i16PFinvc = ((INT32)m_i16Pinvc*100)/m_i16Sinvc;
	UPDNLMT16(m_i16PFinva,100,0);
	UPDNLMT16(m_i16PFinvb,100,0);
	UPDNLMT16(m_i16PFinvc,100,0);

	
	i16_temp1 = ((INT32)m_i16Pinva*1000)>>10;			
	i16_temp1 = (((INT32)i16_temp1)*m_i16SystemPF)>>10;		//PF = 1
	i16_temp2 = (((INT32)m_i16Sinva)*1000)>>10;			
	m_i16LoadRatea =  MAX2(i16_temp1,i16_temp2);

	i16_temp1 = (((INT32)m_i16Pinvb)*1000)>>10;			
	i16_temp1 = (((INT32)i16_temp1)*m_i16SystemPF)>>10;		//PF = 1
	i16_temp2 = (((INT32)m_i16Sinvb)*1000)>>10;			
	m_i16LoadRateb =  MAX2(i16_temp1,i16_temp2);
		
	i16_temp1 = (((INT32)m_i16Pinvc)*1000)>>10;			
	i16_temp1 = (((INT32)i16_temp1)*m_i16SystemPF)>>10;		//PF = 1
	i16_temp2 = (((INT32)m_i16Sinvc)*1000)>>10;			
	m_i16LoadRatec =  MAX2(i16_temp1,i16_temp2);

	if(m_i16LoadRatea <=60 )
	{
		m_i16LoadRatea = (((INT32)m_i16LoadRatea) * 819)>>10;
	}
	if(m_i16LoadRateb <=60 )
	{
		m_i16LoadRateb = (((INT32)m_i16LoadRateb) * 819)>>10;
	}
	if(m_i16LoadRatec <=60 )
	{
		m_i16LoadRatec = (((INT32)m_i16LoadRatec) * 819)>>10;
	}

	if(objSystem.m_st_wFlagSystem.bSelfAgingAction == 1)
	{
		m_i16LoadRatea = m_unIinvaRms.half.hword;
		m_i16LoadRateb = m_unIinvbRms.half.hword;
		m_i16LoadRatec = m_unIinvcRms.half.hword; 
	}
	
	
	//电感电流过载阀值设置为额定电流的110%，符合容性载降额，由热实验决定    
//	m_i16SLinva = (((INT32)m_i16IindaPrms )* 930)>>10;		
//	m_i16SLinvb = (((INT32)m_i16IindbPrms )* 930)>>10;		
//	m_i16SLinvc = (((INT32)m_i16IindcPrms )* 930)>>10;		

}

/************************************************************************************
//函数名称：    Dat_Int_InvCal()
//功能描述:     逆变矢量角基本步长、电压、电流、功率及电感电流周期平方和计算
//算法说明:     1、逆变矢量角瞬时步长在宏IMAC_PHASE_LOCK中已求得，这里对其滤波得到逆变矢量角基本步长
//				2、分别将A、B、C相的电压、电流、功率的平方累加
//			    3、相位过零时将平方和备份，且将其清零
************************************************************************************/
 
#pragma CODE_SECTION(".Epwm1Funcs") 			

void 	Class_Inverter::Dat_Int_InvCal(void)
{
	INT16 i16IntAx_0;
	// 逆变矢量角基本步长计算
	m_unDthetaRefBase_0.dword = FILTER(8,objInvAlg.m_i16DthetaRefInst_0,m_unDthetaRefBase_0.dword);
	
	m_unVinvMMean_0.dword = FILTER(7,m_i16VinvM_0,m_unVinvMMean_0.dword);
	m_i16PointcntInv_0++;
	
	// 逆变相电压平方和累加12.20格式
	m_unVinvaSum_0.dword += ((((INT32)m_i16Vinva_0)*m_i16Vinva_0)>>4);
	m_unVinvbSum_0.dword += ((((INT32)m_i16Vinvb_0)*m_i16Vinvb_0)>>4);
	m_unVinvcSum_0.dword += ((((INT32)m_i16Vinvc_0)*m_i16Vinvc_0)>>4);
	
	// 逆变线电压平方和累加12.20格式
	m_i16Vinvab_0 = m_i16Vinva_0 - m_i16Vinvb_0;
	m_i16Vinvbc_0 = m_i16Vinvb_0 - m_i16Vinvc_0;
	m_i16Vinvca_0 = m_i16Vinvc_0 - m_i16Vinva_0;
	m_unVinvabSum_0.dword += ((((INT32)m_i16Vinvab_0)*m_i16Vinvab_0)>>4);
	m_unVinvbcSum_0.dword += ((((INT32)m_i16Vinvbc_0)*m_i16Vinvbc_0)>>4);
	m_unVinvcaSum_0.dword += ((((INT32)m_i16Vinvca_0)*m_i16Vinvca_0)>>4);

	// 逆变电流平方和累加12.20格式
	m_unIinvaSum_0.dword += ((((INT32)m_i16Iinva2_0)*m_i16Iinva2_0)>>4);
	m_unIinvbSum_0.dword += ((((INT32)m_i16Iinvb2_0)*m_i16Iinvb2_0)>>4);
	m_unIinvcSum_0.dword += ((((INT32)m_i16Iinvc2_0)*m_i16Iinvc2_0)>>4);
	
	// 逆变电感电流平方和累加12.20格式
	m_unIindaSum_0.dword += ((((INT32)m_i16Ila_0)*m_i16Ila_0)>>4);
	m_unIindbSum_0.dword += ((((INT32)m_i16Ilb_0)*m_i16Ilb_0)>>4);
	m_unIindcSum_0.dword += ((((INT32)m_i16Ilc_0)*m_i16Ilc_0)>>4);

	// 逆变瞬时功率和累加12.20格式
	m_unPinvaAcc_0.dword += ((((INT32)m_i16Vinva_0)*m_i16Iinva2_0)>>4);
	m_unPinvbAcc_0.dword += ((((INT32)m_i16Vinvb_0)*m_i16Iinvb2_0)>>4);
	m_unPinvcAcc_0.dword += ((((INT32)m_i16Vinvc_0)*m_i16Iinvc2_0)>>4);

	//逆变电流瞬时值比较
  	if((labs((INT32)m_i16Iinva_0<<6)-((INT32)m_i16IinvaPrms)*m_u16IinvaCrest_0) >= 0)	
  	{
		m_i16IinvaCrestAck_0++;
  	}
	if((labs((INT32)m_i16Iinvb_0<<6)-((INT32)m_i16IinvbPrms)*m_u16IinvbCrest_0) >= 0)	
	{
		m_i16IinvbCrestAck_0++;
	}
	if((labs((INT32)m_i16Iinvc_0<<6)-((INT32)m_i16IinvcPrms)*m_u16IinvcCrest_0) >= 0)	
	{
		m_i16IinvcCrestAck_0++;
	}
	

	if((objInvAlg.m_st_wIntMainFlag.bVpwmCrossZero == 1) ||( m_i16PointcntInv_0 >= K40HzPointCntCnst))  
	{															//逆变过零或超过25ms
		m_i16PointcntInvM_0 = m_i16PointcntInv_0;
		m_i16PointcntInv_0 = 0;

		m_unVinvaSumM_0 = m_unVinvaSum_0;
		m_unVinvbSumM_0 = m_unVinvbSum_0;
		m_unVinvcSumM_0 = m_unVinvcSum_0;
		
		m_unVinvabSumM_0 = m_unVinvabSum_0;
		m_unVinvbcSumM_0 = m_unVinvbcSum_0;
		m_unVinvcaSumM_0 = m_unVinvcaSum_0;

		m_unIinvaSumM_0 = m_unIinvaSum_0;
		m_unIinvbSumM_0 = m_unIinvbSum_0;
		m_unIinvcSumM_0 = m_unIinvcSum_0;
		
		m_unIindaSumM_0 = m_unIindaSum_0;
		m_unIindbSumM_0 = m_unIindbSum_0;
		m_unIindcSumM_0 = m_unIindcSum_0;

		if(m_unPinvaAcc_0.half.hword < 0) 
		{
			m_unPinvaAccM_0.dword = 0;
		}
		else
		{
			m_unPinvaAccM_0 = m_unPinvaAcc_0;
		}
		
		if(m_unPinvbAcc_0.half.hword < 0) 
		{
			m_unPinvbAccM_0.dword = 0;
		}
		else
		{
			m_unPinvbAccM_0 = m_unPinvbAcc_0;
		}
		if(m_unPinvcAcc_0.half.hword < 0)
		{
			m_unPinvcAccM_0.dword = 0;
		}
		else
		{
			m_unPinvcAccM_0 = m_unPinvcAcc_0;
		}
		
		m_unVinvaSum_0.dword = 0;
		m_unVinvbSum_0.dword = 0;
		m_unVinvcSum_0.dword = 0;
		
		m_unVinvabSum_0.dword = 0;
		m_unVinvbcSum_0.dword = 0;
		m_unVinvcaSum_0.dword = 0;

		m_unIinvaSum_0.dword = 0;
		m_unIinvbSum_0.dword = 0;
		m_unIinvcSum_0.dword = 0;

		m_unIindaSum_0.dword = 0;
		m_unIindbSum_0.dword = 0;
		m_unIindcSum_0.dword = 0;

		m_unPinvaAcc_0.dword = 0;
		m_unPinvbAcc_0.dword = 0;
		m_unPinvcAcc_0.dword = 0;

		 // 逆变峰值比调整
		if(m_i16IinvaCrestAck_0 >= 2)
		{
			if(++m_u16IinvaCrest_0 > 1280)
			{
				m_u16IinvaCrest_0 = 1280;
			}
		}
		else
		{
			if(--m_u16IinvaCrest_0 < 256)
			{
				m_u16IinvaCrest_0 = 256;
			}
		}
		
		if(m_i16IinvbCrestAck_0 >= 2)
		{
			if(++m_u16IinvbCrest_0 > 1280)
			{
				m_u16IinvbCrest_0 = 1280;
			}
		}
		else
		{
			if(--m_u16IinvbCrest_0<256)
			{
				m_u16IinvbCrest_0 = 256;
			}
		}
		
		if(m_i16IinvcCrestAck_0 >= 2)
		{
			if(++m_u16IinvcCrest_0 > 1280)
			{
				m_u16IinvcCrest_0 = 1280;
			}
		}
		else
		{
			if(--m_u16IinvcCrest_0<256)
			{
				m_u16IinvcCrest_0 = 256;
			}
		}

		m_i16IinvaCrestAck_0 = 0;
		m_i16IinvbCrestAck_0 = 0;
		m_i16IinvcCrestAck_0 = 0;
	
	}   

	// 计算相位偏移，用于快速检测
	i16IntAx_0 = objInvAlg.m_unThetaRef_0.half.hword+330;
	if(i16IntAx_0 >= 360)
	{
		i16IntAx_0 -= 360;
	}
	objInvAlg.m_i16CosQ120Ref_0 = sintab[i16IntAx_0];

	i16IntAx_0 = objInvAlg.m_unThetaRef_0.half.hword+210;
	if(i16IntAx_0 >= 360) 
	{
		i16IntAx_0 -= 360;
	}
	objInvAlg.m_i16CosQ240Ref_0 = sintab[i16IntAx_0];
}



/************************************************************************************
//函数名称：    Alg_Int_InvOverCurrentChk()
//功能描述:   逆变过流快检
//算法说明:    
************************************************************************************/
 
//#pragma CODE_SECTION(".Epwm1Funcs") 			

void 	Class_Inverter::Alg_Int_InvOverCurrentChk(void)
{
//现在CT放在旁路功率线上，当逆变供电时，如果输出电流过大则认为主旁共通
//加入逆变侧标志，防止逆变间断切旁路误判
	if((objXfer.m_st_XferResult.bBpSTS==0)&&(objXfer.m_st_wXferCmd.bSwitchStatus == 1)&&(objXfer.m_st_XferResult.bInvKM == 1)&&(objXfer.m_i16KMOnCntPer14ms >= 3)) 
	{
		if((abs(objOutput.m_i16Iouta_0) > 409)||(abs(objOutput.m_i16Ioutb_0) > 409)||(abs(objOutput.m_i16Ioutc_0) > 409)) 	
		{
			m_i16_InvOverCurrentChkCnt++;
			if(m_i16_InvOverCurrentChkCnt>K2msCnst)
			{
				m_i16_InvOverCurrentChkCnt=K2msCnst;
				objInvAlg.m_st_wIntMainFlag1_0.bBpInvOverCurrent = 1;//主旁共通大电流标志
				objInvAlg.m_st_wIntMainFlag1_0.bBpInvOverCurrentReason = 0;
				objInvAlg.m_st_wIntMainFlag2_0.bBpSTSShortPulseDoing = 1;
				m_i16BpSTSShortPWMShutCnt_0 = 20;
			}
		}
		else
		{
			m_i16_InvOverCurrentChkCnt--;	
			if(m_i16_InvOverCurrentChkCnt<0)
			{
				m_i16_InvOverCurrentChkCnt=0;
				//objInvAlg.m_st_wIntMainFlag1_0.bBpInvOverCurrent = 0;
			}
		}
	}
	else if(objXfer.m_st_XferResult.bInvKM == 0)
	{
		if((abs(objInverter.m_i16Iinva_0) > 8192) ||(abs(objInverter.m_i16Iinvb_0) > 8192)||(abs(objInverter.m_i16Iinvc_0) > 8192))
		{
			m_i16_InvOverCurrentChkCnt1++;
			m_i16_InvOverCurrentChkCnt1++;
			if(m_i16_InvOverCurrentChkCnt1>K13msCnst)
			{
				m_i16_InvOverCurrentChkCnt1=K13msCnst;
				objInvAlg.m_st_wIntMainFlag1_0.bBpInvOverCurrent = 1;//主旁共通大电流标志
				objInvAlg.m_st_wIntMainFlag1_0.bBpInvOverCurrentReason = 1;
			}
		}
		else
		{
			m_i16_InvOverCurrentChkCnt1--;	
			if(m_i16_InvOverCurrentChkCnt1<0)
			{
				m_i16_InvOverCurrentChkCnt1=0;
				objInvAlg.m_st_wIntMainFlag1_0.bBpInvOverCurrent = 0;
			}
		}	
	}
	else
	{
		m_i16_InvaOverCurrentChkCnt=0;
		m_i16_InvbOverCurrentChkCnt=0;
		m_i16_InvcOverCurrentChkCnt=0;
		m_i16_InvOverCurrentChkCnt1 = 0;
		objInvAlg.m_st_wIntMainFlag1_0.bBpInvOverCurrent = 0;//scr not short
	}

	if((objInvAlg.m_st_wIntMainFlag2_0.bBpSTSShortPulseDoing == 1)&&(m_i16BpSTSShortPWMShutCnt_0 >=0))
	{
		objPWMDrv.Drv_PWMInactive();
		m_i16BpSTSShortPWMShutCnt_0--;
		if(m_i16BpSTSShortPWMShutCnt_0<=0)
		{
			m_i16BpSTSShortPWMShutCnt_0 = 0;
			objInvAlg.m_st_wIntMainFlag2_0.bBpSTSShortPulseDoing = 0;
		}
	}
}


/************************************************************************************
//函数名称：    Alg_IinvUnbalanced()
//功能描述:   逆变过流快检
//算法说明:    
************************************************************************************/
void Class_Inverter::Alg_IinvUnbalanced(void)
{
	
		m_st_FlagVinvABC.bInvCurrUnbalanced= 0;
		m_st_FlagVinvABC.bInvCurrUnbalancedWarn= 0;
	
}

/************************************************************************************
//函数名称：    Dat_Int_IinvDFTCal()
//功能描述:   逆变电流基波分析
//算法说明:    
************************************************************************************/
#pragma CODE_SECTION(".Epwm1Funcs")

void Class_Inverter::Dat_Int_IinvDFTCal()
{
	if((objXfer.m_st_wXferCmd.bSwitchStatus == 1)&&(objXfer.m_st_XferResult.bInvKM == 1))
	{
		m_i32IinvaPSum_0 += ((((INT32)m_i16Iinva_0) * objInvAlg.m_i16CosQ0RefBak_0)>>2);
		m_i32IinvaQSum_0 += ((((INT32)m_i16Iinva_0) * objInvAlg.m_i16SinQ0RefBak_0)>>2);
		m_i32IinvbPSum_0 += ((((INT32)m_i16Iinvb_0) * objInvAlg.m_i16CosQ0RefBak_0)>>2);
		m_i32IinvbQSum_0 += ((((INT32)m_i16Iinvb_0) * objInvAlg.m_i16SinQ0RefBak_0)>>2);
		m_i32IinvcPSum_0 += ((((INT32)m_i16Iinvc_0) * objInvAlg.m_i16CosQ0RefBak_0)>>2);
		m_i32IinvcQSum_0 += ((((INT32)m_i16Iinvc_0 )* objInvAlg.m_i16SinQ0RefBak_0)>>2);
	}
	else
	{
		
		m_i32IinvaPSum_0 = 0;
		m_i32IinvaQSum_0 = 0;
		m_i32IinvbPSum_0 = 0;
		m_i32IinvbQSum_0 = 0;
		m_i32IinvcPSum_0 = 0;
		m_i32IinvcQSum_0 = 0;
	}
	if((objInvAlg.m_st_wIntMainFlag.bVpwmCrossZero == 1) ||( m_i16PointcntInv_0 >= K40HzPointCntCnst))
	{
		m_i32IinvaPSumM_0 = m_i32IinvaPSum_0;
		m_i32IinvaQSumM_0 = m_i32IinvaQSum_0;
		m_i32IinvbPSumM_0 = m_i32IinvbPSum_0;
		m_i32IinvbQSumM_0 = m_i32IinvbQSum_0;
		m_i32IinvcPSumM_0 = m_i32IinvcPSum_0;
		m_i32IinvcQSumM_0 = m_i32IinvcQSum_0;

		m_i32IinvaPSum_0 = 0;
		m_i32IinvaQSum_0 = 0;
		m_i32IinvbPSum_0 = 0;
		m_i32IinvbQSum_0 = 0;
		m_i32IinvcPSum_0 = 0;
		m_i32IinvcQSum_0 = 0;
	}
}


/************************************************************************************
//函数名称：    Dat_InvPQRMS()
//功能描述:   逆变电流谐波有功、无功分量计算
//算法说明:    
************************************************************************************/
void Class_Inverter::Dat_IinvPQRMS(void)
{
	if((objXfer.m_st_wXferCmd.bSwitchStatus == 1)&&(objXfer.m_st_XferResult.bInvKM == 1))
	{
		m_i16IinvaP_0 = (m_i32IinvaPSumM_0 / m_i16PointcntInvM_0)>>9;
		m_i16IinvaQ_0 = (m_i32IinvaQSumM_0 / m_i16PointcntInvM_0)>>9;
		m_i16IinvbP_0 = (m_i32IinvbPSumM_0 / m_i16PointcntInvM_0)>>9;
		m_i16IinvbQ_0 = (m_i32IinvbQSumM_0 / m_i16PointcntInvM_0)>>9;
		m_i16IinvcP_0 = (m_i32IinvcPSumM_0 / m_i16PointcntInvM_0)>>9;
		m_i16IinvcQ_0 = (m_i32IinvcQSumM_0 / m_i16PointcntInvM_0)>>9;
	}
	else
	{
		m_i16IinvaP_0 = 0;
		m_i16IinvaQ_0 = 0;
		m_i16IinvbP_0 = 0;
		m_i16IinvbQ_0 = 0;
		m_i16IinvcP_0 = 0;
		m_i16IinvcQ_0 = 0;
	}
}


//===========================================================================
// End of file.
//===========================================================================
