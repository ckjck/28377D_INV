/************************************************************************************
函数名：	App_SelfAgingAlgVarInit()	
功能描述：	SelfAgingAlgorithm类变量初始化		
函数调用：	
***********************************************************************************/
	 void	Class_SelfAgingAlgorithm::App_SelfAgingAlgVarInit(void)
	{
		m_i16AgingIindMdTgt = 1160;								//自老化D轴给定为20%.
		m_i16AgingRefIQ_0 = 356;								//40uF电容，2.8A电流
		m_i16KSPWM = KSPWMCnst1;
	}
/************************************************************************************
函数名：App_SelfAgingAlgParamConfig()
************************************************************************************/
	void	Class_SelfAgingAlgorithm::App_SelfAgingAlgParamConfig(void)
	{
		//上电参数设置结束前，可确认自老化模式的设置；补充回馈电流至电感电流模值的映射。
		INT16 i16Temp;
		
		//后台设置为（１２０－２０）：１００代表１０ｋｗ，旁路电压为２２０Ｖ(1024)，电流给定为(59909*100)/1024=5792;
		/*i32Temp1 = 59310;
		i16Temp = objMonInterface.m_i16SelfAgingCurrRate - 20;
		UPDNLMT16(i16Temp,100,0);
		i16Temp1 = objBypass.m_unVbpaRms.half.hword;
		UPDNLMT16(i16Temp1,1200,900);
		m_i16AgingIindMdTgt = (i32Temp1*i16Temp/i16Temp1);
		UPDNLMT16(m_i16AgingIindMdTgt,6000,0);*/

		i16Temp = objMonInterface.m_i16SelfAgingCurrRate - 20;
		UPDNLMT16(i16Temp,110,10);
		m_i16AgingIindMdTgt = i16Temp*58;	  			// 回馈电流比例映射:5792/100
			
		if((objSystem.m_st_wFlagSystem.bAfterParamDly == 0) && ( objMonInterface.m_st_wUPSModeRun.bMode == 0))
		{
			if (objMonInterface.m_st_wUPSModeRun2.bInvRunMode == 1)
			{
				objSystem.m_st_wFlagSystem.bSelfAgingAction = 1;
			}
			else
			{
				objSystem.m_st_wFlagSystem.bSelfAgingAction = 0;
			}
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
//函数名称：    Alg_Int_AgingPhaseLock()
//功能描述：	自老化锁相，跟踪旁路电压相角。并判断是否锁上。
//算法说明：	普通的PI跟踪算法
***********************************************************************************/
void 	Class_SelfAgingAlgorithm::Alg_Int_AgingPhaseLock(void)
{
	INT16	i16IntAx_0,i16IntBx_0,i16IntCx_0,i16IntDx_0,i16IntEx_0;

	if(objBypass.m_st_FlagVbpTotal.bBpTrace == 0)				// 旁路不可跟踪
	{
		m_unAgingIoutTheta_0.dword = 0;
		m_unAgingSumPLL_0.dword  = 0;
		m_i16AgingPLLCnt_0 = 0;
		objInvAlg.m_st_wIntMainFlag.bPhaseLock = 0;				// 清相位锁定标志
	}
	else
	{															// 计算相位误差
		i16IntBx_0 = objBypass.m_i16SinQBp_0;
		i16IntCx_0 = objBypass.m_i16CosQBp_0;
		i16IntAx_0 = ((INT32)i16IntBx_0*m_i16AgingCos0_0-(INT32)i16IntCx_0*m_i16AgingSin0_0)>>14;	// 计算相位差正弦值,6.10
		m_i16AgingSinDphase_0 = i16IntAx_0;
		UPDNLMT16(i16IntAx_0,500,-500);							// 限制在正负30度,1024为1,512为0.5,即30度
		m_i16AgingCosDphase_0 = ((INT32)i16IntCx_0*m_i16AgingCos0_0+(INT32)i16IntBx_0*m_i16AgingSin0_0)>>14;// 计算相位差余弦值,6.10
		
		// PI调节器
		i16IntDx_0 = 11000;
		i16IntEx_0 = m_unAgingSumPLL_0.half.hword+(((INT32)i16IntAx_0*i16IntDx_0)>>12);
		UPDNLMT16(i16IntEx_0,1267,-1267);							// 1024-->1度 1267/1024/360*8000=27.5Hz 55+-27.5Hz
	
		// 积分量更新
		m_unAgingSumPLL_0.dword += (INT32)i16IntAx_0*1408;
		UPDNLMT32(m_unAgingSumPLL_0.dword,0x4F30000,-0x4F30000);
		
		// 判断当前相角是否过零
		m_unAgingIoutTheta_0.dword += (((INT32)i16IntEx_0+K55HzDthetaCnst)<<6);
		if(m_unAgingIoutTheta_0.dword>=23592960)
		{
			m_unAgingIoutTheta_0.dword -= 23592960;
		}
		
		// 求取sinQ，cosQ
		m_i16AgingSin0_0 = sintab[m_unAgingIoutTheta_0.half.hword];
		i16IntAx_0 = m_unAgingIoutTheta_0.half.hword+90;
    	if(i16IntAx_0>=360)
    	{
			i16IntAx_0 -= 360;
    	}
		m_i16AgingCos0_0 = sintab[i16IntAx_0];
		
		// 判断相位是否锁定,9.8°
		if((m_i16AgingCosDphase_0 < 512) || (abs(m_i16AgingSinDphase_0) > 174))
		{
			m_i16AgingPLLCnt_0--;
			if(m_i16AgingPLLCnt_0 <= 0)
			{
				m_i16AgingPLLCnt_0 = 0;
				objInvAlg.m_st_wIntMainFlag.bPhaseLock = 0;
				objInvAlg.m_st_wIntMainFlag1_0.bBpPhaseLock = 0;
			}
		}
		else
		{
			m_i16AgingPLLCnt_0++;
			if(m_i16AgingPLLCnt_0 >= 1500)
			{
				m_i16AgingPLLCnt_0 = 1500;
				objInvAlg.m_st_wIntMainFlag.bPhaseLock = 1;
				objInvAlg.m_st_wIntMainFlag1_0.bBpPhaseLock = 1;
			}
		}
	}
}

/************************************************************************************
//函数名称：    Alg_Int_AgingRefGen()
//功能描述：	参考电压，电流生成
//算法说明：    根据电压模式或电流模式，几个开关周期增加或减少参考电压(或参考电流)得到目标值
***********************************************************************************/
void 	Class_SelfAgingAlgorithm::Alg_Int_AgingRefGen(void)
{
	INT16	i16IntAx_0;
	
	if((objXfer.m_st_wXferCmd.bSwitchStatus == 1) && (objSwitches.m_st_FlagBreaker.bInvKM == 1))	// 逆变供电且逆变输出INVKM可靠导通
	{
		i16IntAx_0 = m_i16AgingIindMdTgt-m_i16AgingRefID_0;
		if(i16IntAx_0 != 0)
		{
			m_i16AgingCurRefCnt_0++;
			if(m_i16AgingCurRefCnt_0 >= 6)								// 每6个开关周期调整一次
			{ 
				m_i16AgingCurRefCnt_0 = 0;
				if(i16IntAx_0 < 0)
				{
					m_i16AgingRefID_0--;
				}
				else
				{
					m_i16AgingRefID_0++;
				}
			}
		}
		m_i16AgingKpCur_0 = 250;
		m_i16AgingKiCur_0 = 40;
		m_i16AgingBpModeRef_0 = objBypass.m_unVbpMMean_0.half.hword;
	}
	else														// 旁路侧或者逆变供电且逆变输出INVKM不导通
	{
		i16IntAx_0 = objBypass.m_unVbpMMean_0.half.hword-m_i16AgingBpModeRef_0;
		if(i16IntAx_0 != 0)
		{
			m_i16AgingVolRefCnt_0++;
			if(m_i16AgingVolRefCnt_0 >= 3)								// 每3个开关周期调整一次
			{
				m_i16AgingVolRefCnt_0 = 0;
				if(i16IntAx_0 < 0)
				{
					m_i16AgingBpModeRef_0--;
				}
				else
				{
					m_i16AgingBpModeRef_0++;
				}
			}
		}
		// 变量复位，为电流模式作准备
		m_i16AgingKpCur_0 = 0;
		m_i16AgingKiCur_0 = 0;
		m_unAgingSumID_0.dword = 0;
		m_unAgingSumIQ_0.dword = 0;
		m_unAgingSumIZ_0.dword = 0;
	}
}

/************************************************************************************
//函数名称：    Alg_Int_AgingPreDeal()
//功能描述：	逆变电流坐标变换
//算法说明：    三二变换旋转变换
***********************************************************************************/
void 	Class_SelfAgingAlgorithm::Alg_Int_AgingPreDeal(void)
{
	INT16	i16IntAlpha_0,i16IntBeta_0;
	i16IntAlpha_0 = (((INT32)objInverter.m_i16Ila_0*2-objInverter.m_i16Ilb_0-objInverter.m_i16Ilc_0)*K1div3Cnst)>>12;
	i16IntBeta_0 = (((INT32)objInverter.m_i16Ilb_0-objInverter.m_i16Ilc_0)*KSqrt3invCnst)>>12;
	m_i16AgingIZ_0 = (((INT32)objInverter.m_i16Ila_0+objInverter.m_i16Ilb_0+objInverter.m_i16Ilc_0)*KSqrt2Div3Cnst)>>12;

	m_i16AgingID_0 = ((INT32)i16IntBeta_0*m_i16AgingSin0_0+(INT32)i16IntAlpha_0*m_i16AgingCos0_0)>>12;
	m_i16AgingIQ_0 = ((INT32)i16IntBeta_0*m_i16AgingCos0_0-(INT32)i16IntAlpha_0*m_i16AgingSin0_0)>>12;	
}

/************************************************************************************
//函数名称：    Alg_Int_AgingCurrReg()
//功能描述：	电流环调节器
//算法说明：    普通的增量PI调节器
***********************************************************************************/
void 	Class_SelfAgingAlgorithm::Alg_Int_AgingCurrReg(void)
{
	INT16	i16IntAx_0;
  i16IntAx_0 = m_i16AgingRefID_0-m_i16AgingID_0;							// 误差
	UPDNLMT16(i16IntAx_0,6000,-6000);	
	m_i16AgingPwmD_0 = m_unAgingSumID_0.half.hword+(((INT32)i16IntAx_0*m_i16AgingKpCur_0)>>12)+m_i16AgingBpModeRef_0;
	UPDNLMT16(m_i16AgingPwmD_0,8000,-8000);
	m_unAgingSumID_0.dword += (INT32)i16IntAx_0*m_i16AgingKiCur_0;
	UPDNLMT32(m_unAgingSumID_0.dword,0x17700000,-0x17700000);
	
    i16IntAx_0 = m_i16AgingRefIQ_0-m_i16AgingIQ_0;							// 误差
	UPDNLMT16(i16IntAx_0,3000,-3000);
	m_i16AgingPwmQ_0 = m_unAgingSumIQ_0.half.hword+(((INT32)i16IntAx_0*m_i16AgingKpCur_0)>>12);
	UPDNLMT16(m_i16AgingPwmQ_0,4096,-4096);
	m_unAgingSumIQ_0.dword += (INT32)i16IntAx_0*m_i16AgingKiCur_0;
	UPDNLMT32(m_unAgingSumIQ_0.dword,0x8000000,-0x8000000);
	
	i16IntAx_0 = -m_i16AgingIZ_0;
	UPDNLMT16(i16IntAx_0,1000,-1000);
	m_i16AgingPwmZ_0 = m_unAgingSumIZ_0.half.hword+(((INT32)i16IntAx_0*m_i16AgingKpCur_0)>>12);
	UPDNLMT16(m_i16AgingPwmZ_0,1500,-1500);
	UPDNLMT16(i16IntAx_0,300,-300);
	m_unAgingSumIZ_0.dword += (INT32)i16IntAx_0*2;
	UPDNLMT32(m_unAgingSumIZ_0.dword,0x5DC000,-0x5DC000);
}

/************************************************************************************
//函数名称：    Alg_Int_AgingSPWM()
//功能描述：	PWM发波
***********************************************************************************/
void 	Class_SelfAgingAlgorithm::Alg_Int_AgingSPWM(void)
{
	INT16	i16IntAx_0,i16IntBx_0,i16IntCx_0,i16IntDx_0;
	INT16	i16IntAlpha_0,i16IntBeta_0;

	m_i16wUirD_0 = m_i16AgingPwmD_0;
	m_i16wUirQ_0 = m_i16AgingPwmQ_0;
	m_i16wUirZ_0 = m_i16AgingPwmZ_0;

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
	i16IntAlpha_0 = (((INT32)m_i16wUirD_0) * m_i16AgingCos0_0 - (((INT32)m_i16wUirQ_0)*m_i16AgingSin0_0)) >> 12;
	i16IntBeta_0 = (((INT32)m_i16wUirD_0) * m_i16AgingSin0_0 + (((INT32)m_i16wUirQ_0)*m_i16AgingCos0_0)) >> 12;

	// 2/3变换
	m_i16VaAct_0 = i16IntAlpha_0+((((INT32)m_i16wUirZ_0) * K1DivSqrt2Cnst) >> 12);
	m_i16VbAct_0 = (-((INT32)i16IntAlpha_0) * K1Div2Cnst + (((INT32)i16IntBeta_0) * KSqrt3Div2Cnst)+(((INT32)m_i16wUirZ_0)*K1DivSqrt2Cnst)) >> 12;
	m_i16VcAct_0 = (-((INT32)i16IntAlpha_0) * K1Div2Cnst -(((INT32)i16IntBeta_0)  *KSqrt3Div2Cnst)+(((INT32)m_i16wUirZ_0)*K1DivSqrt2Cnst)) >> 12;


	// 旋转/静止变换
	i16IntAlpha_0 = (((INT32)m_i16AgingRefID_0) * m_i16AgingCos0_0 - (((INT32)m_i16AgingRefIQ_0)*m_i16AgingSin0_0)) >> 12;
	i16IntBeta_0 = (((INT32)m_i16AgingRefID_0) * m_i16AgingSin0_0 + (((INT32)m_i16AgingRefIQ_0)*m_i16AgingCos0_0)) >> 12;

	// 2/3变换
	m_i16Iaref_0 = i16IntAlpha_0;
	m_i16Ibref_0 = (-((INT32)i16IntAlpha_0) * K1Div2Cnst + (((INT32)i16IntBeta_0) * KSqrt3Div2Cnst)) >> 12;
	m_i16Icref_0 = (-((INT32)i16IntAlpha_0) * K1Div2Cnst -(((INT32)i16IntBeta_0)  *KSqrt3Div2Cnst)) >> 12;

	UPDNLMT16(m_i16Iaref_0,1600,-1600);
	UPDNLMT16(m_i16Iaref_0,1600,-1600);
	UPDNLMT16(m_i16Iaref_0,1600,-1600);

	unIarefFilter_0.dword += (((long)m_i16Iaref_0<<11) - unIarefFilter_0.dword)>>2;
	unIbrefFilter_0.dword += (((long)m_i16Ibref_0<<11) - unIbrefFilter_0.dword)>>2;
	unIcrefFilter_0.dword += (((long)m_i16Icref_0<<11) - unIcrefFilter_0.dword)>>2;

	m_i16PWMaDeadCmp_0 = unIarefFilter_0.half.hword;
	m_i16PWMbDeadCmp_0 = unIbrefFilter_0.half.hword;
	m_i16PWMcDeadCmp_0 = unIcrefFilter_0.half.hword;
	
	UPDNLMT16(m_i16PWMaDeadCmp_0,50,-50);
	UPDNLMT16(m_i16PWMbDeadCmp_0,50,-50);
	UPDNLMT16(m_i16PWMcDeadCmp_0,50,-50);

	// 计算比较值
	i16IntAx_0 = ((INT32)(abs(m_i16VaAct_0))* m_i16KSPWM) >> 12;
	i16IntBx_0 = ((INT32)(abs(m_i16VbAct_0))* m_i16KSPWM) >> 12;
	i16IntCx_0 = ((INT32)(abs(m_i16VcAct_0))* m_i16KSPWM) >> 12;
	
	i16IntAx_0 += 8;										//发波补偿360ns(硬件上升下降延时损失)
	i16IntBx_0 += 8;										//发波补偿360ns(硬件上升下降延时损失)
	i16IntCx_0 += 8;										//发波补偿360ns(硬件上升下降延时损失)

	if (m_i16VaAct_0 > 0)
	{
		GpioDataRegs.GPACLEAR.bit.GPIO17 = 1;				//A相正半周
		i16IntAx_0 = (((INT32)i16IntAx_0 * m_i161DivVbusP_0) >> 12) + m_i16PWMaDeadCmp_0;
	}
	else
	{
		GpioDataRegs.GPASET.bit.GPIO17 = 1;				//A相负半周
		i16IntAx_0 = (((INT32)i16IntAx_0 * m_i161DivVbusN_0) >> 12) - m_i16PWMaDeadCmp_0;
	}

	if (m_i16VbAct_0 > 0)
	{
		GpioDataRegs.GPACLEAR.bit.GPIO18 = 1;				//B相正半周
		i16IntBx_0 = (((INT32)i16IntBx_0 * m_i161DivVbusP_0) >> 12) + m_i16PWMbDeadCmp_0;
	}
	else
	{
		GpioDataRegs.GPASET.bit.GPIO18 = 1;				//B相负半周
		i16IntBx_0 = (((INT32)i16IntBx_0 * m_i161DivVbusN_0) >> 12) - m_i16PWMbDeadCmp_0;
	}

	if (m_i16VcAct_0 > 0)
	{
		GpioDataRegs.GPACLEAR.bit.GPIO19 = 1;				//C相正半周
		i16IntCx_0 = (((INT32)i16IntCx_0 * m_i161DivVbusP_0) >> 12) + m_i16PWMcDeadCmp_0;
	}
	else
	{
		GpioDataRegs.GPASET.bit.GPIO19 = 1;				//C相负半周
		i16IntCx_0 = (((INT32)i16IntCx_0 * m_i161DivVbusN_0) >> 12) - m_i16PWMcDeadCmp_0;
	}

	UPDNLMT16(i16IntAx_0,KSwPrdCnst-20,20);
	UPDNLMT16(i16IntBx_0,KSwPrdCnst-20,20);
	UPDNLMT16(i16IntCx_0,KSwPrdCnst-20,20);

	// 三相发波
	m_i16PulseOffCount = 0;
	EPwm2Regs.CMPA.bit.CMPA = i16IntAx_0;				//A相S1管脉冲
	EPwm3Regs.CMPA.bit.CMPA = i16IntBx_0;				//B相S1管脉冲
	EPwm4Regs.CMPA.bit.CMPA = i16IntCx_0;				//C相S1管脉冲
}

/************************************************************************************
//函数名称：    Alg_Int_AgingOffReset()
//功能描述：	关机状态复位处理
***********************************************************************************/
void 	Class_SelfAgingAlgorithm::Alg_Int_AgingOffReset(void)
{
    m_i16AgingBpModeRef_0 = 100;
	m_i16AgingRefID_0 = 500;
	
	m_i16AgingKpCur_0 = 0;
	m_i16AgingKiCur_0 = 0;
	m_unAgingSumID_0.dword = 0;
	m_unAgingSumIQ_0.dword = 0;
	m_unAgingSumIZ_0.dword = 0;
	
	unIarefFilter_0.dword = 0;
	unIbrefFilter_0.dword = 0;
	unIcrefFilter_0.dword = 0;
	
	m_i16PulseOffCount++;
	if(m_i16PulseOffCount > 3)                                                
	{                                                                   
		m_i16PulseOffCount = 3;                                             
		EPwm2Regs.CMPA.bit.CMPA = 0;						//A相管脉冲
		EPwm3Regs.CMPA.bit.CMPA = 0;						//B相管脉冲
		EPwm4Regs.CMPA.bit.CMPA = 0;						//C相管脉冲
	                                                                    
		GpioDataRegs.GPACLEAR.bit.GPIO17=1;					//A相正半周     
		GpioDataRegs.GPACLEAR.bit.GPIO18=1;					//B相正半周     
		GpioDataRegs.GPACLEAR.bit.GPIO19=1;					//C相正半周     
	}      
}

/*==========================================================*/ 	
/* Title		:	SelfAgingAlgorithm.cpp											*/ 	
/* Description	: 	Class_SelfAgingAlgorithm definition								*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/	
#include "SelfAgingAlgorithm.h"

/************************************************************************************
//函数名称：    Alg_Int_SelfAging()
//功能描述：	实现自老化功能
//算法说明：    系统设为自老化模式时，实现自老化功能
//入口参数：    objInvCtrl.m_st_wFlagInv 自老化模式标志位
//出口参数：    无
***********************************************************************************/
//#pragma CODE_SECTION(".Epwm1Funcs")

void 	Class_SelfAgingAlgorithm::Alg_Int_SelfAging(void)
{
	Alg_Int_AgingPhaseLock();
		
	if(objInverter.m_st_wFlagInv.bInv != 0)		         		// 逆变器开启
	{
		Alg_Int_AgingRefGen();            						// 参考电压，电流生成
    
		Alg_Int_AgingPreDeal();                					// 逆变电流坐标变换
    
		Alg_Int_AgingCurrReg();                  				// 电流环控制器
    
		Alg_Int_AgingSPWM();                      				// SPWM发波
		
		objPWMDrv.Drv_PWMActive();								//PWM输出
	}
    else 
    {
   		Alg_Int_AgingOffReset();								// 关机状态复位处理
   		
   		objPWMDrv.Drv_PWMInactive();							//禁止PWM输出
    }
}

//===========================================================================
// End of file.
//===========================================================================
