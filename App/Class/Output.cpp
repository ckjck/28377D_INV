/*==========================================================*/ 	
/* Title		:	Output.cpp													*/ 	
/* Description	: 	Class_Output definition											*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#include "Output.h"


/************************************************************************************
函数名：	App_OutVarInit()	
功能描述：	Output类变量初始化		
函数调用：	
***********************************************************************************/
	 void	Class_Output::App_OutVarInit(void)
	{
		m_i16PointcntOutM_0 = K50HzPointCntCnst;
		m_unDthetaOutMean_0.half.hword = K50HzDthetaCnst;
		
		m_i16wPFoutLeada = 1024;
		m_i16wPFoutLeadb = 1024;
		m_i16wPFoutLeadc = 1024;
		
		m_i16KVouta_0 = 4119;         //定标系数
		m_i16KVoutb_0 = 4119;         //定标系数
		m_i16KVoutc_0 = 4119;         //定标系数
	    
//		m_i16KIouta_0 = 6876;
//		m_i16KIoutb_0 = 6876;
//		m_i16KIoutc_0 = 6876;
		
	}
/************************************************************************************
函数名：App_OutParamConfig()
************************************************************************************/
	void	Class_Output::App_OutParamConfig(void)
	{
		//采样系数设置
		switch (objMonInterface.m_st_wUPSModeRun.bVoltOut)
		{
			case 0:																								//400v
				m_i16KVoutNormal = 4119;    //定标系数
				m_i16KIoutNormal = 4055;																		// 正样检测板更改
				break;				            													
			case 1:																									//380v
				m_i16KVoutNormal = 4119;    //定标系数
				m_i16KIoutNormal = 4055;
				break;                    													
			case 2:																									//415v
				m_i16KVoutNormal = 4119;    //定标系数
				m_i16KIoutNormal = 4055;
				break;
			default:
				break;							
			}
		
		//输出电压定标系数校正
		m_i16KVouta_0 = (((INT32)objMonInterface.m_i16KsVoutaSet)*m_i16KVoutNormal)>>12;
		m_i16KVoutb_0 = (((INT32)objMonInterface.m_i16KsVoutbSet)*m_i16KVoutNormal)>>12;
		m_i16KVoutc_0 = (((INT32)objMonInterface.m_i16KsVoutcSet)*m_i16KVoutNormal)>>12;

		//输出电流定标系数校正
		m_i16KIouta_0 = (((INT32)objMonInterface.m_i16KsIoutaSet)*m_i16KIoutNormal)>>12;
		m_i16KIoutb_0 = (((INT32)objMonInterface.m_i16KsIoutbSet)*m_i16KIoutNormal)>>12;
		m_i16KIoutc_0 = (((INT32)objMonInterface.m_i16KsIoutcSet)*m_i16KIoutNormal)>>12;

	}

/************************************************************************************
函数名：	vVout_Abnormal()
功能描述：	输出电压异常判断模块
函数调用：	无			
************************************************************************************/
	void	Class_Output::Alg_VoutAbnormal(void)
	{	
		 //有效值判据
		 //061013--“输出电压异常”告警去掉对母线的判断
		 if ( ( objInverter.m_st_wFlagInv.bInv == 3 ) && ( objInverter.m_st_FlagVinvABC.bVoutAbnormal == 1 ) )	//逆变器启动完毕且输出电压异常
		 {
		 	  	objSystem.m_st_wFlagPrompt.bVoutAbnormal=1;					//置输出电压异常状态
		 		objDebug.m_st_wDebug.bVoutAbnormal1 = 1;
		 }
		//瞬时值判据
		else if(( objInvAlg.m_st_wIntMainFlag.bVoutAbnormal == 1) && (objSwitches.m_st_FlagBreaker.bInvKMOpen == 0) 
				&& (objSwitches.m_st_FlagBreaker.bFuse == 1))
		{
		  	objSystem.m_st_wFlagPrompt.bVoutAbnormal=1;						//置输出电压异常状态
			objDebug.m_st_wDebug.bVoutAbnormal2 = 1;
		}
		else if(objInvAlg.m_st_wIntMainFlag1_0.bVoutFastChkAbnomal==1)
		{
		  	objSystem.m_st_wFlagPrompt.bVoutAbnormal=1;						//置输出电压异常状态
			objDebug.m_st_wDebug.bVoutAbnormal2 = 1;

		}
		else
		{
		  	objSystem.m_st_wFlagPrompt.bVoutAbnormal=0;						//清输出电压异常状态
		}
		
		//061013--“输出电压异常”故障旁路范围选择为[70%,m_i16VbpUplimit]，并进行2%内缩
		if( (objSystem.m_st_wFlagPrompt.bVoutAbnormal==1) && (objBypass.m_st_FlagVbpTotal.bBpRange<3)
			&& ( objBypass.m_i16VbpaPrms <= (objBypass.m_i16VbpUplimit-20) ) && ( objBypass.m_i16VbpaPrms >= 737 )	
			&& ( objBypass.m_i16VbpbPrms <= (objBypass.m_i16VbpUplimit-20) ) && ( objBypass.m_i16VbpbPrms >= 737 )
			&& ( objBypass.m_i16VbpcPrms <= (objBypass.m_i16VbpUplimit-20) ) && ( objBypass.m_i16VbpcPrms >= 737 ) )	
		{
			objSystem.m_st_wFlagFault.bVoutAbnormal=1;
		}
		else								
		{
			objSystem.m_st_wFlagFault.bVoutAbnormal=0;
		}
	}
	
/************************************************************************************
函数名：	Alg_Int_VoutAbnormal()
功能描述：	采用输出电压波形比较，超过[70%,120%]累计15ms后将“输出电压异常”告警，
            旁路在保护范围内，系统切旁路工作，且可自起机；否则，逆变坚持工作	
函数调用：	无
************************************************************************************/
	
	//#pragma CODE_SECTION(".Epwm1Funcs") 			

	void	Class_Output::Alg_Int_VoutAbnormal(void)
	{
		INT16 i16IntAx_0,i16IntBx_0,i16IntCx_0,i16IntDx_0;
		
		if((objXfer.m_st_XferResult.bInvKM == 1) && (objSystem.m_st_wFlagSystem.bSelfAgingAction == 0))
		{	
			m_i16VoutFastAbnormalStartCnt_0++;
			if(m_i16VoutFastAbnormalStartCnt_0 >= (K7msCnst >> 1)) 
			{
				UPDNLMT16(m_i16VoutFastAbnormalStartCnt_0,K7msCnst,0);
				//Phase A
				
				i16IntAx_0 = (INT32)objInvAlg.m_i16CosQ0Ref_0*m_i16wVoutFastRef3>>12;	//70%
				
				i16IntBx_0 = abs(i16IntAx_0);

				i16IntCx_0 = (INT32)objInvAlg.m_i16CosQ0Ref_0*m_i16wVoutFastRef4>>12;	//120%
				
				i16IntDx_0 = abs(i16IntCx_0);
				if(((i16IntBx_0 >= 186) && ((abs(m_i16Vouta_0)<i16IntBx_0) || ((INT32)m_i16Vouta_0*i16IntAx_0<0)))	
					|| ((i16IntDx_0 >= 186) && ((abs(m_i16Vouta_0)>i16IntDx_0)||((INT32)m_i16Vouta_0*i16IntCx_0<0))))
				{           
					m_i16VoutaFastAbnormalCnt_0++;
				}	        
				else
				{
					m_i16VoutaFastAbnormalCnt_0--;
					
				}
				UPDNLMT16(m_i16VoutaFastAbnormalCnt_0,K15msCnst,0);

				
				
					//Phase B
					i16IntAx_0 = (INT32)objInvAlg.m_i16CosQ120Ref_0*m_i16wVoutFastRef3>>12;	//70%
					i16IntBx_0 = abs(i16IntAx_0);
					i16IntCx_0 = (INT32)objInvAlg.m_i16CosQ120Ref_0*m_i16wVoutFastRef4>>12;	//120%
					i16IntDx_0 = abs(i16IntCx_0);
			   		if(((i16IntBx_0 >= 186) && ((abs(m_i16Voutb_0)<i16IntBx_0)||((INT32)m_i16Voutb_0*i16IntAx_0<0))	
						|| ((i16IntDx_0 >= 186) && ((abs(m_i16Voutb_0)>i16IntDx_0)||((INT32)m_i16Voutb_0*i16IntCx_0<0)))))
					{
						m_i16VoutbFastAbnormalCnt_0++;
					}	
					else
					{
						m_i16VoutbFastAbnormalCnt_0--;
					
					}
					UPDNLMT16(m_i16VoutbFastAbnormalCnt_0,K15msCnst,0);
					
					//Phase C
					i16IntAx_0 = (INT32)objInvAlg.m_i16CosQ240Ref_0*m_i16wVoutFastRef3>>12;	//70%
					i16IntBx_0 = abs(i16IntAx_0);
					i16IntCx_0 = (INT32)objInvAlg.m_i16CosQ240Ref_0*m_i16wVoutFastRef4>>12;	//120%
					i16IntDx_0 = abs(i16IntCx_0);
					if(((i16IntBx_0>=186)&&((abs(m_i16Voutc_0)<i16IntBx_0)||((INT32)m_i16Voutc_0*i16IntAx_0<0)))	
						||((i16IntDx_0>=186)&&((abs(m_i16Voutc_0)>i16IntDx_0)||((INT32)m_i16Voutc_0*i16IntCx_0<0))))
					{
						
						m_i16VoutcFastAbnormalCnt_0++;
					}	
					else
					{
						m_i16VoutcFastAbnormalCnt_0--;
						
					}
					UPDNLMT16(m_i16VoutcFastAbnormalCnt_0,K15msCnst,0);
					
				
				if((m_i16VoutaFastAbnormalCnt_0>(K15msCnst>>1)) || (m_i16VoutbFastAbnormalCnt_0>(K15msCnst>>1)) 
					|| (m_i16VoutcFastAbnormalCnt_0>(K15msCnst>>1)))
				{	
					objInvAlg.m_st_wIntMainFlag.bVoutAbnormal = 1;
					
				}
				else							
				{	
					objInvAlg.m_st_wIntMainFlag.bVoutAbnormal = 0;
				}
			}
		}
		else								
		{	
			m_i16VoutaFastAbnormalCnt_0 = 0;
			m_i16VoutbFastAbnormalCnt_0 = 0;
			m_i16VoutcFastAbnormalCnt_0 = 0;
			objInvAlg.m_st_wIntMainFlag.bVoutAbnormal = 0; 
			m_i16VoutFastAbnormalStartCnt_0 = 0; 
		}		
	}

/************************************************************************************
函数名：	Alg_Int_VoutFastChk2()
功能描述：	类似旁路dsp实现的旁路接管的条件,实时监测输出电压如果三相均<110v，确认17ms报输出异常
            	
函数调用：	无
************************************************************************************/
	//#pragma CODE_SECTION(".Epwm1Funcs") 			

	void	Class_Output::Alg_Int_VoutFastChk2(void)
	{

		if((objMonInterface.m_st_wParallelData.iNumOn == 1)&&( objInverter.m_st_wFlagInv.bInv == 3 )&&((objInverter.m_st_wFlagInv.bPhaseLock==0)||(objBypass.m_st_FlagVbpTotal.bBpRange !=0)))

		{
			if((abs(objOutput.m_i16Vouta_0) < 2048) && (abs(objOutput.m_i16Voutb_0) < 2048) && (abs(objOutput.m_i16Voutc_0) < 2048))						//2048(220*1.414*0.3=110V)	
			{
				m_i16VoutFastchkCnt_0++;
			}	
			else
			{
				m_i16VoutFastchkCnt_0--;
			}

			UPDNLMT16(m_i16VoutFastchkCnt_0,((K20msCnst+K10msCnst) >> 1),0);
                     if(m_i16VoutFastchkCnt_0>(K17msCnst >> 1))
                     {
			   objInvAlg.m_st_wIntMainFlag1_0.bVoutFastChkAbnomal=1;

//			   GpioDataRegs.GPBSET.bit.GPIO34=1;					//MASTER 

                     }
			else
			{
			objInvAlg.m_st_wIntMainFlag1_0.bVoutFastChkAbnomal=0;
			}
			
		}
		else
		{
			m_i16VoutFastchkCnt_0=0;
			objInvAlg.m_st_wIntMainFlag1_0.bVoutFastChkAbnomal=0;

//			GpioDataRegs.GPBCLEAR.bit.GPIO34=1;					//SLAVE     

		}

	}

	
/************************************************************************************
函数名：	Alg_Int_VoutFastChk()
功能描述：	输出电压异常快速检测	
函数调用：	无
************************************************************************************/
	
//#pragma CODE_SECTION(".Epwm1Funcs") 			
	
	void	Class_Output::Alg_Int_VoutFastChk(void)
	{	
		INT16 i16IntAx_0,i16IntBx_0,i16IntCx_0;

		objInvAlg.m_st_wIntMainFlag.bVinvLoadFast = 0;			//清逆变器故障快速检测异常标志 BIT7
		objInvAlg.m_st_wIntMainFlag.bVbpFastChk = 0;				//清旁路快速检测进行中标志 BIT9
		objInvAlg.m_st_wIntMainFlag.bVinvFastChk = 0;			//清逆变快速检测进行中标志 BIT10
		objInvAlg.m_st_wIntMainFlag.bVinvFast = 0;				//清逆变快速检测异常标志 BIT12
		objInvAlg.m_st_wIntMainFlag.bVbpFast = 0;				//清旁路快速检测异常标志 BIT13
		i16IntCx_0 = m_i16wVoutFastRef1;					//电压快检的参考波幅值默认为旁路超保护设置值
		
		//10S旁路快检标志
		if((objXfer.m_st_XferResult.bBpSTS == 1) && (objSystem.m_st_wFlagSystem.bSelfAgingAction == 0)
			&& (objBypass.m_st_FlagVbpTotal.bBps10Fast == 1)&&(objBypass.m_st_FlagVbpTotal.bBps4FaultDelay ==1))	//旁路STS导通,非自老化,有10S旁路快检标志
		{
			objInvAlg.m_st_wIntMainFlag.bVbpFastChk = 1;
		}
		
		//正常情况下的电压快检
		if((objInverter.m_st_wFlagInv.bInv==3) && (objSwitches.m_st_FlagBreaker.bQ5==1)&&(objSystem.m_st_wFlagSystem.bSelfAgingAction==0)
			&& (objXfer.m_st_wXferCmd.bSwitchMode!=2))		//主机开启完毕，Q5合且非自老化且switchmode!=2
		{
			//工况一：逆变电压快检
			if(objXfer.m_st_XferResult.bInvKM == 1) 		
			{    
				m_i16VinvFastchkStartCnt++;
				if(m_i16VinvFastchkStartCnt >= (K7msCnst>>1)) 
				{      
					m_i16VinvFastchkStartCnt = (K7msCnst>>1);
					objInvAlg.m_st_wIntMainFlag.bVinvFastChk = 1;
					i16IntCx_0 = m_i16wVoutFastRef2;			//逆变电压快检的参考波幅值固定为-40%
			     }
			} 
			
			//工况二：单机ECO或热备份主机ECO旁路电压快检
			else if(((objMonInterface.m_st_wUPSModeRun.bConfig == 0)&&(objMonInterface.m_st_wUPSModeRun.bMode == 1))		
				||((objMonInterface.m_st_wUPSModeRun.bConfig == 2)&&(objMonInterface.m_st_wUPSModeRun.bMasterECO == 1)))
			{
				/*if(m_i16SpanVbpFastAct_0 < 20)
				{
					if(m_i16ClkSVbpFastchk_0 != objTimerDrv.m_u16ClkS_0)
					{
						m_i16SpanVbpFastAct_0++;
					}
					if(!((objInverter.m_st_wFlagInv.bPhaseLock == 1) && (m_i16SpanVbpFastAct_0 > 5)))
					{
						m_i16ClkSVbpFastchk_0 = objTimerDrv.m_u16ClkS_0;
						return;
					}	
				}*/
				objInvAlg.m_st_wIntMainFlag.bVbpFastChk = 1;
			}
			else
			{
				objInvAlg.m_st_wIntMainFlag.bVoutFastA = 0;
				objInvAlg.m_st_wIntMainFlag.bVoutFastB = 0;
				objInvAlg.m_st_wIntMainFlag.bVoutFastC = 0;
				objInvAlg.m_st_wIntMainFlag.bVinvLoadFast = 0;
				objInvAlg.m_st_wIntMainFlag1_0.bVinvLoadFastA = 0;
				objInvAlg.m_st_wIntMainFlag1_0.bVinvLoadFastB = 0;
				objInvAlg.m_st_wIntMainFlag1_0.bVinvLoadFastC = 0;
				//m_i16SpanVbpFastAct_0  =0;
				m_i16VoutaFastchkCnt_0 = 0;
				m_i16VoutbFastchkCnt_0 = 0;
				m_i16VoutcFastchkCnt_0 = 0;
				m_i16VoutaFtFastchkCnt_0 = 0;
				m_i16VoutbFtFastchkCnt_0 = 0;
				m_i16VoutcFtFastchkCnt_0 = 0; 
				m_i16VinvFastchkStartCnt = 0;
				
				m_i16ClkSVbpFastchk_0 = objTimerDrv.m_u16ClkS_0;
				return;
			}
		if((m_i16VinvFastchkStartCnt == (K7msCnst>>1))
			||((objInverter.m_st_wFlagInv.bInv == 3) && (objXfer.m_st_XferResult.bInvKM == 0)))
		{
			//A相

			
			i16IntAx_0 = ((INT32)objInvAlg.m_i16CosQ0Ref_0)*i16IntCx_0>>12;	//X/1024*4096*SQRT(2)*0.9*COSQ/4096=XCOSQ*1.25*8/8192=10XCOSQ/8192                	// 输出频率基准及有效值计算	
			
			
			i16IntBx_0 = abs(i16IntAx_0);
			if((abs(m_i16Vouta_0)<(i16IntBx_0-186))||((i16IntBx_0>=186)&&((INT32)m_i16Vouta_0*i16IntAx_0<0)))	
			{
				m_i16VoutaFastchkCnt_0++;
				if(abs(objInverter.m_i16Iinva_0) < 2000)
				{
					m_i16VoutaFtFastchkCnt_0++;
				}
				else
				{
					m_i16VoutaFtFastchkCnt_0--;	
				}	
			}	
			else
			{
				m_i16VoutaFastchkCnt_0--;
				m_i16VoutaFtFastchkCnt_0--;
			}
			UPDNLMT16(m_i16VoutaFastchkCnt_0,K7msCnst,0);
			UPDNLMT16(m_i16VoutaFtFastchkCnt_0,K7msCnst,0);
				
			if(m_i16VoutaFastchkCnt_0 >(K6msCnst>>1)) 	
			{	
				objInvAlg.m_st_wIntMainFlag.bVoutFastA = 1;				//用于冲击转旁路
			}
			else						
			{	
				objInvAlg.m_st_wIntMainFlag.bVoutFastA = 0;
			}
			if((m_i16VoutaFastchkCnt_0 > (K5msCnst>>1) )&& (m_i16VoutaFtFastchkCnt_0 >( K4msCnst>>1))) 
			{
				objInvAlg.m_st_wIntMainFlag1_0.bVinvLoadFastA = 1;		//用于逆变器故障
			}
			else						
			{	
				objInvAlg.m_st_wIntMainFlag1_0.bVinvLoadFastA = 0;
			}
				
			
			
				//B相
				i16IntAx_0 = ((INT32)objInvAlg.m_i16CosQ120Ref_0)*i16IntCx_0>>12;	//X/1024*4096*SQRT(2)*0.9*COSQ/4096=XCOSQ*1.25*8/8192=10XCOSQ/8192                	// 输出频率基准及有效值计算	
				i16IntBx_0 = abs(i16IntAx_0);
				if((abs(m_i16Voutb_0)<(i16IntBx_0-186))||((i16IntBx_0>=186)&&((INT32)m_i16Voutb_0*i16IntAx_0<0)))	
				{
					m_i16VoutbFastchkCnt_0++;
					if(abs(objInverter.m_i16Iinvb_0)<2000)
					{
						m_i16VoutbFtFastchkCnt_0++;
					}
					else
					{
						m_i16VoutbFtFastchkCnt_0--;
					}	
				}	
				else
				{
					m_i16VoutbFastchkCnt_0--;
					m_i16VoutbFtFastchkCnt_0--;
				}
				UPDNLMT16(m_i16VoutbFastchkCnt_0,K7msCnst,0);
				UPDNLMT16(m_i16VoutbFtFastchkCnt_0,K7msCnst,0);
					
				if(m_i16VoutbFastchkCnt_0 > (K6msCnst>>1)) 	
				{	
					objInvAlg.m_st_wIntMainFlag.bVoutFastB = 1;
				}
				else						
				{	
					objInvAlg.m_st_wIntMainFlag.bVoutFastB = 0;
				}
				if((m_i16VoutbFastchkCnt_0 > (K5msCnst>>1)) &&( m_i16VoutbFtFastchkCnt_0 > ( K4msCnst>>1))) 
				{	
					objInvAlg.m_st_wIntMainFlag1_0.bVinvLoadFastB = 1;
				}
				else						
				{	
					objInvAlg.m_st_wIntMainFlag1_0.bVinvLoadFastB = 0;	
				}
					
				//C相
				i16IntAx_0 = ((INT32)objInvAlg.m_i16CosQ240Ref_0)*i16IntCx_0>>12;	//X/1024*4096*SQRT(2)*0.9*COSQ/4096=XCOSQ*1.25*8/8192=10XCOSQ/8192                	// 输出频率基准及有效值计算
				i16IntBx_0 = abs(i16IntAx_0);
				if((abs(m_i16Voutc_0)<(i16IntBx_0-186))||((i16IntBx_0>=186)&&((INT32)m_i16Voutc_0*i16IntAx_0<0)))	
				{
					m_i16VoutcFastchkCnt_0++;
					if(abs(objInverter.m_i16Iinvc_0) < 2000)
					{
						m_i16VoutcFtFastchkCnt_0++;
					}
					else
					{
						m_i16VoutcFtFastchkCnt_0--;	
					}	
				}	
				else
				{
					m_i16VoutcFastchkCnt_0--;
					m_i16VoutcFtFastchkCnt_0--;
				}
				UPDNLMT16(m_i16VoutcFastchkCnt_0,K7msCnst,0);
				UPDNLMT16(m_i16VoutcFtFastchkCnt_0,K7msCnst,0);
					
				if(m_i16VoutcFastchkCnt_0 > (K6msCnst>>1)) 	
				{	
					objInvAlg.m_st_wIntMainFlag.bVoutFastC = 1;
				}
				else						
				{	
					objInvAlg.m_st_wIntMainFlag.bVoutFastC = 0;
				}
				if((m_i16VoutcFastchkCnt_0 > (K5msCnst>>1)) &&( m_i16VoutcFtFastchkCnt_0 > ( K4msCnst>>1)))	
				{	
					objInvAlg.m_st_wIntMainFlag1_0.bVinvLoadFastC = 1;
				}
				else						
				{	
					objInvAlg.m_st_wIntMainFlag1_0.bVinvLoadFastC = 0;
				}
			
			//标志综合
			if((objInvAlg.m_st_wIntMainFlag.bVoutFastA == 1)||(objInvAlg.m_st_wIntMainFlag.bVoutFastB==1)||(objInvAlg.m_st_wIntMainFlag.bVoutFastC==1))
			{
				if(objInvAlg.m_st_wIntMainFlag.bVinvFastChk == 1) 
				{
					objInvAlg.m_st_wIntMainFlag.bVinvFast = 1;
				}
				else							
				{
					objInvAlg.m_st_wIntMainFlag.bVbpFast = 1;	 
				}
			}
			
			if(((objInvAlg.m_st_wIntMainFlag1_0.bVinvLoadFastA == 1) || (objInvAlg.m_st_wIntMainFlag1_0.bVinvLoadFastB==1)
				||(objInvAlg.m_st_wIntMainFlag1_0.bVinvLoadFastC == 1)) && (objInvAlg.m_st_wIntMainFlag.bVinvFastChk==1))
			{
				objInvAlg.m_st_wIntMainFlag.bVinvLoadFast = 1;
			}
				
			m_i16ClkSVbpFastchk_0 = objTimerDrv.m_u16ClkS_0;	
		}		
		
		else
		{
			objInvAlg.m_st_wIntMainFlag.bVoutFastA = 0;
			objInvAlg.m_st_wIntMainFlag.bVoutFastB = 0;
			objInvAlg.m_st_wIntMainFlag.bVoutFastC = 0;
			objInvAlg.m_st_wIntMainFlag.bVinvLoadFast = 0;
			objInvAlg.m_st_wIntMainFlag1_0.bVinvLoadFastA = 0;
			objInvAlg.m_st_wIntMainFlag1_0.bVinvLoadFastB = 0;
			objInvAlg.m_st_wIntMainFlag1_0.bVinvLoadFastC = 0;
			//m_i16SpanVbpFastAct_0 = 0;
			m_i16VoutaFastchkCnt_0 = 0;
			m_i16VoutbFastchkCnt_0 = 0;
			m_i16VoutcFastchkCnt_0 = 0;
			m_i16VoutaFtFastchkCnt_0 = 0;
			m_i16VoutbFtFastchkCnt_0 = 0;
			m_i16VoutcFtFastchkCnt_0 = 0;
		
			m_i16ClkSVbpFastchk_0 = objTimerDrv.m_u16ClkS_0;
		}
      	}
	else
	{
		m_i16VinvFastchkStartCnt = 0;
	}
	}
	
/************************************************************************************
//函数名称：    Dat_Int_OutCal()
//功能描述:     输出电压、电流、功率周期平方和计算，输出电流峰值比计算
//算法说明:     1、滤波后的输出电压经3/2变换后求得正、余弦值，与上一开关周期的相
//		        应值求的相位差，再经滤波后得到输出矢量角基本步长
//			    2、分别将A、B、C相的电压平方累加；输出电流瞬时值比较
//			    3、点数到达时将平方和备份，且将其清零；输出电流峰值比调整
************************************************************************************/

#pragma CODE_SECTION(".Epwm1Funcs") 			

void 	Class_Output::Dat_Int_OutCal(void)
{


	// 输出相电压平方和累加12.20格式
	m_unVoutaSum_0.dword += (((INT32)m_i16Vouta_0*m_i16Vouta_0)>>4);	 
	m_unVoutbSum_0.dword += (((INT32)m_i16Voutb_0*m_i16Voutb_0)>>4);
	m_unVoutcSum_0.dword += (((INT32)m_i16Voutc_0*m_i16Voutc_0)>>4);
	

	
	m_i16PointcntOut_0++;
	if(m_i16PointcntOut_0 >= m_i16PointcntOutM_0)				 // 输出过零
	{
		m_unVoutaSumM_0 = m_unVoutaSum_0;     
		m_unVoutbSumM_0 = m_unVoutbSum_0;
		m_unVoutcSumM_0 = m_unVoutcSum_0;


		m_i16PointcntOut_0 = 0;

		m_unVoutaSum_0.dword = 0;
		m_unVoutbSum_0.dword = 0;
		m_unVoutcSum_0.dword = 0;
		
	}
	
}

/************************************************************************************
//函数名称：    Dat_Int_OutPreDeal()
************************************************************************************/

#pragma CODE_SECTION(".Epwm1Funcs") 			

void 	Class_Output::Dat_Int_OutPreDeal(void)
{
	INT16	i16IntAx_0,i16IntAlpha_0,i16IntBeta_0;
	//读取采样值
	m_i16Vouta_0 = objADDrv.m_i16ADC_Vouta_0; 
	m_i16Voutb_0 = objADDrv.m_i16ADC_Voutb_0;
	m_i16Voutc_0 = objADDrv.m_i16ADC_Voutc_0; 


	// 输出相电压计算，定标为6.10
	m_i16Vouta_0 = ((INT32)m_i16Vouta_0*m_i16KVouta_0)>>10;
	m_i16Voutb_0 = ((INT32)m_i16Voutb_0*m_i16KVoutb_0)>>10;
	m_i16Voutc_0 = ((INT32)m_i16Voutc_0*m_i16KVoutc_0)>>10;

   

		// 输出电压3/2变换，矢量模与相位处理
		i16IntAlpha_0 = (((INT32)m_i16Vouta_0*2-m_i16Voutb_0-m_i16Voutc_0)*K1div3Cnst)>>12;
		i16IntBeta_0 = (((INT32)m_i16Voutb_0-m_i16Voutc_0)*KSqrt3invCnst)>>12;

		i16IntAx_0 = (((INT32)i16IntAlpha_0)*i16IntAlpha_0+((INT32)i16IntBeta_0)*i16IntBeta_0)>>16;
		UPDNLMT16(i16IntAx_0,1023,1);
		m_i16VoutM_0 = sqrtTab[i16IntAx_0];
		m_i16SinQOut_0 = (((INT32)i16IntBeta_0)<<12)/m_i16VoutM_0;
		m_i16CosQOut_0 = (((INT32)i16IntAlpha_0)<<12)/m_i16VoutM_0;
	
	
}

/************************************************************************************
//函数名称：    Dat_VoutRMS()
************************************************************************************/
void 	Class_Output::Dat_VoutRMS(void)
{
	INT32	i32temp;
	//输出相电压有效值计算
	i32temp = m_unVoutaSumM_0.dword/m_i16PointcntOutM_0;
	m_i16VoutaPrms = isqrt(i32temp);
	m_unVoutaRms.dword += (((INT32)m_i16VoutaPrms<<16)-m_unVoutaRms.dword)>>K30msFilterCnst;
	
	i32temp = m_unVoutbSumM_0.dword/m_i16PointcntOutM_0;
	m_i16VoutbPrms = isqrt(i32temp);
	m_unVoutbRms.dword += (((INT32)m_i16VoutbPrms<<16)-m_unVoutbRms.dword)>>K30msFilterCnst;
	
	i32temp = m_unVoutcSumM_0.dword/m_i16PointcntOutM_0;
	m_i16VoutcPrms = isqrt(i32temp);
	m_unVoutcRms.dword += (((INT32)m_i16VoutcPrms<<16)-m_unVoutcRms.dword)>>K30msFilterCnst;
	
	i32temp = (INT32)(abs(m_unDthetaOutBase_0.half.hword));
	m_unDthetaOutMean_0.dword += ((i32temp<<16) -m_unDthetaOutMean_0.dword)>>K30msFilterCnst;
    
	i32temp = ((INT32)(m_unDthetaOutMean_0.half.hword))*KFMeanCalCnst;
	m_i16FoutMean = i32temp>>11;

	if((m_i16FoutMean >= 3000) &&( m_i16FoutMean <= 8000))
	{
		i32temp=((INT32)5000)*K50HzPointCntCnst;
		m_i16PointcntOutM_0 = i32temp/m_i16FoutMean;
	}	
	else
	{
		m_i16PointcntOutM_0 =K30HzPointCntCnst;	
	}
					
}

/************************************************************************************
//函数名称：    Dat_IoutRMS()
************************************************************************************/
void 	Class_Output::Dat_IoutRMS(void)
{
	INT32	i32temp;
	i32temp = m_unIoutaSumM_0.dword/m_i16PointcntOutM_0;
	m_i16IoutaPrms = isqrt(i32temp);
	m_unIoutaRms.dword += (((INT32)m_i16IoutaPrms<<16)-m_unIoutaRms.dword)>>K30msFilterCnst;
	
	i32temp = m_unIoutbSumM_0.dword/m_i16PointcntOutM_0;
	m_i16IoutbPrms = isqrt(i32temp);
	m_unIoutbRms.dword += (((INT32)m_i16IoutbPrms<<16)-m_unIoutbRms.dword)>>K30msFilterCnst;
	
	i32temp = m_unIoutcSumM_0.dword/m_i16PointcntOutM_0;
	m_i16IoutcPrms = isqrt(i32temp);
	m_unIoutcRms.dword += (((INT32)m_i16IoutcPrms<<16)-m_unIoutcRms.dword)>>K30msFilterCnst;

	//发送给监控的输出电流按授权率折算
	
	m_i16IoutaforShow = (((INT32)m_unIoutaRms.half.hword)*objSystem.m_i161DivGrantRate)>>10;
	m_i16IoutbforShow = (((INT32)m_unIoutbRms.half.hword)*objSystem.m_i161DivGrantRate)>>10;
	m_i16IoutcforShow = (((INT32)m_unIoutcRms.half.hword)*objSystem.m_i161DivGrantRate)>>10;
	
				
}
/************************************************************************************
//函数名称：    Dat_PoutRMS()
************************************************************************************/
void 	Class_Output::Dat_PoutRMS(void)
{
		INT16	i16_temp1,i16_temp2;
		INT32	i32temp;
		
	
		i32temp = m_unPoutaAccM_0.dword/m_i16PointcntOutM_0;	
		m_i16Pouta = i32temp>>10;												
		m_i16Souta = ((INT32)m_i16VoutaPrms * m_i16IoutaPrms)>>10;	
	
		i32temp = m_unPoutbAccM_0.dword/m_i16PointcntOutM_0;
		m_i16Poutb = i32temp>>10;										
		m_i16Soutb = ((INT32)m_i16VoutbPrms * m_i16IoutbPrms)>>10;	


		i32temp = m_unPoutcAccM_0.dword/m_i16PointcntOutM_0;		
		m_i16Poutc =i32temp>>10;						
		m_i16Soutc = ((INT32)m_i16VoutcPrms * m_i16IoutcPrms)>>10;


	m_i16Pouta = (((INT32)m_i16Pouta) * objSystem.m_i161DivGrantRate)>>10;
	m_i16Souta = (((INT32)m_i16Souta) * objSystem.m_i161DivGrantRate)>>10;
	
	m_i16Poutb = (((INT32)m_i16Poutb) * objSystem.m_i161DivGrantRate)>>10;
	m_i16Soutb = (((INT32)m_i16Soutb) * objSystem.m_i161DivGrantRate)>>10;
		
	m_i16Poutc = (((INT32)m_i16Poutc )* objSystem.m_i161DivGrantRate)>>10;
	m_i16Soutc = (((INT32)m_i16Soutc) * objSystem.m_i161DivGrantRate)>>10;

	
	//输出功率因数，负载小时可能需容错	
	m_i16PFouta = ((INT32)m_i16Pouta*100)/m_i16Souta;
	m_i16PFoutb = ((INT32)m_i16Poutb*100)/m_i16Soutb;
	m_i16PFoutc = ((INT32)m_i16Poutc*100)/m_i16Soutc;
	UPDNLMT16(m_i16PFouta,100,0);
	UPDNLMT16(m_i16PFoutb,100,0);
	UPDNLMT16(m_i16PFoutc,100,0);

	i16_temp1 = ((INT32)m_i16Pouta*1000)>>10;			//30KW为满载
	i16_temp1 = ((INT32)i16_temp1 *	objInverter.m_i16SystemPF)>>10;		//Adapter15K~20K的功率因数为0.9,4096/4551=0.9
	i16_temp2 = ((INT32)m_i16Souta*1000)>>10;			//30KVA为满载
	m_i16LoadRatea =  MAX2(i16_temp1,i16_temp2);

	i16_temp1 = ((INT32)m_i16Poutb*1000)>>10;			//30KW为满载
	i16_temp1 = ((INT32)i16_temp1 *	objInverter.m_i16SystemPF)>>10;		//Adapter15K~20K的功率因数为0.9,4096/4551=0.9
	i16_temp2 = ((INT32)m_i16Soutb*1000)>>10;			//30KVA为满载
	m_i16LoadRateb =  MAX2(i16_temp1,i16_temp2);
		
	i16_temp1 = ((INT32)m_i16Poutc*1000)>>10;			//30KW为满载
	i16_temp1 = ((INT32)i16_temp1 *	objInverter.m_i16SystemPF)>>10;		//Adapter15K~20K的功率因数为0.9,4096/4551=0.9
	i16_temp2 = ((INT32)m_i16Soutc*1000)>>10;			//30KVA为满载
	m_i16LoadRatec =  MAX2(i16_temp1,i16_temp2);	

	if(m_i16LoadRatea <=60 )
	{
		m_i16LoadRatea = ((INT32)m_i16LoadRatea * 819)>>10;
	}
	if(m_i16LoadRateb <=60 )
	{
		m_i16LoadRateb = ((INT32)m_i16LoadRateb * 819)>>10;
	}
	if(m_i16LoadRatec <=60 )
	{
		m_i16LoadRatec = ((INT32)m_i16LoadRatec * 819)>>10;
	}


	//自老化下的输出负载率仅从逆变电流获得，以克服旁路电压的影响
	if(objSystem.m_st_wFlagSystem.bSelfAgingAction == 1)
	{
		m_i16LoadRatea = objInverter.m_unIinvaRms.half.hword;
		m_i16LoadRateb = objInverter.m_unIinvbRms.half.hword;
		m_i16LoadRatec = objInverter.m_unIinvcRms.half.hword;
	}	

}
/************************************************************************************
//函数名称：    Alg_Int_ShortCircuit()
//利用瞬时电流判断短路
************************************************************************************/
//#pragma CODE_SECTION(".Epwm1Funcs")
/*void Class_Output::Alg_Int_ShortCircuit(void)
{
	//旁路供电或者逆变供电才检测，均不供电不检测，防止并机主旁共通时误报
	if((objXfer.m_st_XferResult.bBpSTS == 1)||(objXfer.m_st_XferResult.bInvKM == 1))
	{
		m_i16ShortPointCnt++;
		if(abs(objOutput.m_i16Iouta_0 )> 0x2800)
		{
			m_i16IoutaShortCnt++;
		}
		if(abs(objOutput.m_i16Ioutb_0) > 0x2800)
		{
			m_i16IoutbShortCnt++;
		}
		if(abs(objOutput.m_i16Ioutc_0) > 0x2800)
		{
			m_i16IoutcShortCnt++;
		}
		if(m_i16ShortPointCnt >= 44)
		{
			if(m_i16IoutaShortCnt>=30)
			{
				m_i16IaShortFlag =((m_i16IaShortFlag&0x0007) << 1) + 1;
			}
			else
			{
				m_i16IaShortFlag = ((m_i16IaShortFlag&0x0007) << 1) + 0;
			}
			if(m_i16IoutbShortCnt>=30)
			{
				m_i16IbShortFlag =((m_i16IbShortFlag&0x0007) << 1) + 1;
			}
			else
			{
				m_i16IbShortFlag = ((m_i16IbShortFlag&0x0007) << 1) + 0;
			}
			if(m_i16IoutcShortCnt>=30)
			{
				m_i16IcShortFlag =((m_i16IcShortFlag&0x0007) << 1) + 1;
			}
			else
			{
				m_i16IcShortFlag = ((m_i16IcShortFlag&0x0007) << 1) + 0;
			}
			m_i16ShortPointCnt = 0;
			m_i16IoutaShortCnt = 0;
			m_i16IoutbShortCnt = 0;
			m_i16IoutcShortCnt = 0;
		}

		if((m_i16IaShortFlag  == 0x000f) || (m_i16IbShortFlag  == 0x000f) || (m_i16IcShortFlag  == 0x000f))
		{
			m_st_OutFlag.bShortCircuit = 1;
		}
	}
	else
	{
		m_i16IoutaShortCnt = 0;
		m_i16IoutbShortCnt = 0;
		m_i16IoutcShortCnt = 0;
		m_i16IaShortFlag = 0;
		m_i16IbShortFlag = 0;
		m_i16IcShortFlag = 0;
	}
}*/
/************************************************************************************
//函数名称：    Alg_Int_VOutRangeFastChk()
//利用瞬时模值和瞬时步旁路是否超保护
**********************************************************************************/
//#pragma CODE_SECTION(".Epwm1Funcs")

void Class_Output::Alg_Int_VOutRangeFastChk(void)
{
	
		if((m_i16DthetaOutInst_0 > objBypass.m_i16DthetaBpUplimitFast)||(m_i16DthetaOutInst_0 < objBypass.m_i16DthetaBpDnlimitFast))
		{
			m_i16DthetaoutFastChkCnt_0++;
			if(m_i16DthetaoutFastChkCnt_0>=16)
			{
				m_i16DthetaoutFastChkCnt_0 = 16;
				m_st_OutFlag.bDthetaOutFast = 1;
			}
		}
		else
		{
			m_i16DthetaoutFastChkCnt_0 --;
			if(m_i16DthetaoutFastChkCnt_0 <= 0)
			{
				m_i16DthetaoutFastChkCnt_0 = 0;
				m_st_OutFlag.bDthetaOutFast = 0;
			}
		}

		if((m_i16VoutM_0 > objBypass.m_i16VbpMUplimitFast)||((m_i16VoutM_0 < objBypass.m_i16VbpMDnlimitFast)))	//瞬时模值超过超跟踪范围，5%的回差
		{
			m_i16VoutMFastChkCnt_0++;
			if(m_i16VoutMFastChkCnt_0>=16)
			{
				m_i16VoutMFastChkCnt_0 = 16;
				m_st_OutFlag.bVoutMFast= 1;
			}
		}
		else
		{
			m_i16VoutMFastChkCnt_0 --;
			if(m_i16VoutMFastChkCnt_0 <= 0)
			{
				m_i16VoutMFastChkCnt_0 = 0;
				m_st_OutFlag.bVoutMFast = 0;
			}
		}

	
	
	m_st_OutFlag.bVoutFast = m_st_OutFlag.bDthetaOutFast|m_st_OutFlag.bVoutMFast;
}
//===========================================================================
// End of file.
//===========================================================================
