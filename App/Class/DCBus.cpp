/*==========================================================*/ 	
/* Title		:	DCBus.cpp													*/ 	
/* Description	: 	Class_DCBus definition											*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#include "DCBus.h"

/************************************************************************************
函数名：	App_DCBusVarInit()	
功能描述：	DCBus类变量初始化		
函数调用：	
***********************************************************************************/
inline	 void	Class_DCBus::App_DCBusVarInit(void)
	{
		m_i16KVdcbus_0 = 5064;            //定标系数
		m_i16KVbusPAdjust_0 = 4096; 
		m_i16KVbusNAdjust_0 = 4096;
		m_i16VdcOverCnt_0 = 0;
	}
/************************************************************************************
函数名：App_DCBusParamConfig()
************************************************************************************/
inline	void	Class_DCBus::App_DCBusParamConfig(void)
	{
		//采样系数设置
		m_i16KVdcbus_0 = 5064;              //定标系数
		m_i16KVbusPAdjust_0 = 4096; 
		m_i16KVbusNAdjust_0 = 4096;
	}

/************************************************************************************
函数名：	vDC_Fault()
功能描述：	母线故障判断模块
函数调用：	无			
************************************************************************************/
	void	Class_DCBus::Alg_DCBusFault(void)
	{
		//母线电压低关机，在旁路正常的情况下，自起机，若旁路异常，直接关机。
		objSystem.m_st_wFlagPrompt.bDCAbnormalOff = 0;										//清直流电压异常故障位
		
		if (  m_i16Vdc_0 < VdcDnlimCnst  )											//正负电压检测值低于600V
		{
			objSystem.m_st_wFlagFault.bDCAbnormal = 1;										//置直流母线电压异常故障

			if ( objInverter.m_st_wFlagInv.bInv != 0 )										//逆变器开启
			{
				objSystem.m_st_wFlagPrompt.bDCAbnormalOff = 1;								//置母线电压低关机
				objDebug.m_st_wDebug.bDCAbnormalOff1 = 1;
			}
		}
		else
		{
			if ( objSystem.m_st_wFlagSystem.bTimer40ms == 1 )								//40ms定时到
			{
				if (  m_i16Vdc_0 < VdcLowCnst )									//正负电压检测值低于660V
				{
					m_i16VdcLowCntPer40ms ++;											//母线低于330V延时计数器递增
					if ( m_i16VdcLowCntPer40ms >= 250 )								//10s
					{
						m_i16VdcLowCntPer40ms = 250;
						objSystem.m_st_wFlagFault.bDCAbnormal = 1;							//置直流母线电压异常故障
						if ( objInverter.m_st_wFlagInv.bInv != 0 )							//限幅于250
						{
							objSystem.m_st_wFlagPrompt.bDCAbnormalOff = 1;					//置母线电压低关机
							objDebug.m_st_wDebug.bDCAbnormalOff2 = 1;
						}
					}
					else if(m_i16VdcLowCntPer40ms < 100)
					{
						objSystem.m_st_wFlagFault.bDCAbnormal = 0;							//清直流母线电压异常故障
					}
				}
				else
				{
					m_i16VdcLowCntPer40ms --;											//母线低于330V延时计数器递减
					if ( m_i16VdcLowCntPer40ms <= 0 )
					{
						m_i16VdcLowCntPer40ms = 0;									//限幅于零
						objSystem.m_st_wFlagFault.bDCAbnormal = 0;							//清直流母线电压异常故障
					}
				}
			}
		}		

		if ( objDigitalIO.m_st_HardwareSignal.bDcOvp == 1 )								//直流过压 0有效
		{
			objSystem.m_st_wFlagFault.bDCOV = 1;											//置直流过压故障
			objInvCtrl.m_st_wFlagInvOnoff.bManualRecoverOff = 1;							//置可人为恢复故障关机位 靠DC-OV信号自身锁存

			if ( objInverter.m_st_wFlagInv.bInv != 0 )										//逆变器开启
			{
				objSystem.m_st_wFlagPrompt.bDCAbnormalOff = 1;								//置母线电压异常关机
			}
		}
		else
		{
			objSystem.m_st_wFlagFault.bDCOV = 0;	
		}	 

		//整流器关闭 (包括主路和电池) 时立即关闭逆变器，加快母线判断速度?
		if((objDigitalIO.m_st_HardwareSignal.bRecOff == 1) && (objInverter.m_st_wFlagInv.bInv != 0))
		{
			objSystem.m_st_wFlagPrompt.bDCAbnormalOff = 1;								//置母线电压低关机
			if(objDigitalIO.m_st_wLocalNewSignal.bRECOvTemp == 1)
			{
				objInvCtrl.m_st_wFlagInvOnoff.bAutoRecoverOff = 1;
			}
			else
			{
				objInvCtrl.m_st_wFlagInvOnoff.bManualRecoverOff = 1;						//置可人为恢复故瞎鼗?
			}	
			objDebug.m_st_wDebug.bDCAbnormalOff3 = 1;
		}

		//旁路宽范围异常，直流母线异常直接关机，否则关机后自起机
		if ((objSystem.m_st_wFlagPrompt.bDCAbnormalOff == 1) && (objBypass.m_st_FlagVbpTotalWide.bBpRange == 3))
		{
			objInvCtrl.m_st_wFlagInvOnoff.bManualRecoverOff = 1;						//置可人为恢复故障关机位
		}
			
	}

/************************************************************************************
//函数名称：    Alg_Int_DCOVProtect()
//功能描述:     只有当旁路异常才进行此处理 
//	            判断母线电压的瞬时值，当电压电压>920V时,封锁逆变器脉冲1ms，后开放逆变脉冲
//	            当电压电压<880V标志位清零
***********************************************************************************/
 
#pragma CODE_SECTION(".Epwm1Funcs") 			

void 	Class_DCBus::Alg_Int_DCOVProtect(void)
{
    if((objInverter.m_st_wFlagInv.bInv!=0) && (objSystem.m_st_wFlagPrompt.bPow15vFault == 0))     			 // 逆变器关闭/辅助电源故障下，不处理该模块
   {
	objInvAlg.m_st_wIntMainFlag1_0.bDcOvTotalBak = objInvAlg.m_st_wIntMainFlag1_0.bDcOvTotal;  			// BIT12==1代表母线有过压

        if((objBypass.m_st_FlagVbpTotal.bBpRange == 3) && (objMonInterface.m_st_wUPSModeRun.bConfig == 3))      // 旁路异常标志/并机模式下,进行母线电压高封脉冲动作
        {
            // 判断母线是否过压
            if(m_i16VdcSmp_0 < 4652)                                          // 920*4096/810=4652
            {
            		if(m_i16VdcSmp_0 <= 4449) 
            		{
				objInvAlg.m_st_wIntMainFlag1_0.bDcOvTotal = 0;     // 880V*4096/810=4449
            		}
            }
            else 
            {
			objInvAlg.m_st_wIntMainFlag1_0.bDcOvTotal = 1;
            }
        }
        else 
        {
		objInvAlg.m_st_wIntMainFlag1_0.bDcOvTotal = 0;
        }
        
        // DCOV_FASTCHK_END
        if((objInvAlg.m_st_wIntMainFlag1_0.bDcOvTotalBak == 0) && ( objInvAlg.m_st_wIntMainFlag1_0.bDcOvTotal == 1))
        {
           	 m_i16DCOVPWMShutCnt_0 = 5;
            	objInvAlg.m_st_wIntMainFlag1_0.bDcOvPluseOffDoing = 1;
        }


		//三单机型的快关逆变器,GpioDataRegs.GPADAT.bit.GPIO28为硬件逐波限流信号
		
		
		
        if(objInvAlg.m_st_wIntMainFlag1_0.bDcOvPluseOffDoing == 1)
        {
            if(m_i16DCOVPWMShutCnt_0 > 0)
            {
                m_i16DCOVPWMShutCnt_0--;
                objPWMDrv.Drv_PWMInactive();	   // PWM输出强制为高杼?
            }
            else
            {
                m_i16DCOVPWMShutCnt_0 = 0;
                objInvAlg.m_st_wIntMainFlag1_0.bDcOvPluseOffDoing = 0;
                objPWMDrv.Drv_PWMActive();	   // PWM输出
            }
        }
	}
	if(objInverter.m_st_wFlagInv.bInv ==0)
	{
		m_i16VdcOverCnt_0 = 0;
		objSystem.m_st_wFlagPrompt.bDCAbnormalOff1 = 0;
		m_i16DcOverDelayCnt_0 --;
		if(m_i16DcOverDelayCnt_0 <= 0)
		{
			objLoad.m_st_wFlagLoad.bShortCircuit = 0;
			m_i16DcOverDelayCnt_0 = 0;
		}	
	}
}

/************************************************************************************
//函数名称：    Dat_DCBusRMS()
************************************************************************************/
void 	Class_DCBus::Dat_DCBusRMS(void)
{
	//直流母线电压滤波,滤波常数折衷考虑为30ms 
	m_unVdcSmp_0.dword += ( (((INT32)m_i16VdcSmp_0)<<16)-m_unVdcSmp_0.dword)>>K30msFilterCnst;
	m_i16Vdc_0 = m_unVdcSmp_0.half.hword;
				
}

/************************************************************************************
//函数名称：    Dat_Int_DCBusPreDeal()
************************************************************************************/
 
#pragma CODE_SECTION(".Epwm1Funcs") 			

void 	Class_DCBus::Dat_Int_DCBusPreDeal(void)
{
	
	 //读取采样值
	 m_i16VbusP_0 = objADDrv.m_i16ADC_VbusP_0;
	 m_i16VbusN_0 = objADDrv.m_i16ADC_VbusN_0;
	 
	 // 母线电压计算，定标分别为4.12
	m_i16VbusP_0 = (((INT32)m_i16VbusP_0)*m_i16KVdcbus_0)>>12;	
	m_i16VbusP_0 = (((INT32)m_i16VbusP_0)*m_i16KVbusPAdjust_0)>>12;
	
	m_i16VbusN_0 = (((INT32)m_i16VbusN_0)*m_i16KVdcbus_0)>>12;       
	m_i16VbusN_0 = (((INT32)m_i16VbusN_0)*m_i16KVbusNAdjust_0)>>12;
	 
	 m_i16VdcSmp_0 = (m_i16VbusP_0+m_i16VbusN_0)>>1;	 	
	// 母线电压和旁路反灌电压计算，定标分别为5.11和6.10
//	m_i16VdcSmp_0 = (((INT32)m_i16VdcSmp_0)*m_i16KVdcbus_0)>>11;
}


//===========================================================================
// End of file.
//===========================================================================
