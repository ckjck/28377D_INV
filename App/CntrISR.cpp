/*=======================================================*/
/* Filename：	cntr.c                                   */
/* Description: EPWM1 Period Interrupt ISR               */
/* Author: 	Zhuo Qingfeng, Zhang Xuejie					 */
/* Date:	Nov.2005									 */
/*=======================================================*/

extern UINT16 Epwm1FuncsLoadStart;
extern UINT16 Epwm1FuncsLoadEnd;
extern UINT16 Epwm1FuncsRunStart;


#pragma CODE_SECTION(".Epwm1Funcs")

// EPWM1 Period Interrupt ISR
interrupt void EPWM1_INT_ISR(void)
{
    //GpioDataRegs.GPBSET.bit.GPIO34 = 1;
	INT16	i16IntCount_0;
	volatile	INT16   i16IntTemp_0;

    	EPwm1Regs.ETCLR.bit.INT = 1;                    // Clear INT flag for this timer
    	PieCtrlRegs.PIEACK.all |= 0xFFFF;				//PIE处理,清ACK标志   Acknowledge this interrupt to receive more interrupts from group 3


	objInvAlg.Alg_Int_InvSyncRecord();
//	asm(" setc OVM");//???

	if((objSystem.m_st_wFlagSystem.bIoTestMode == 0) && (objSystem.m_st_wFlagSystem.bServiceMode == 0))
	{
			//asm(" RPT #99 || NOP");
	     	//asm(" RPT #99 || NOP");
	     	//asm(" RPT #99 || NOP");
	     	//asm(" RPT #49 || NOP");


        // 正常模式，包括自老化模式
       	objADDrv.Drv_AD_SOC();      // 启动第一轮采样
		objInvAlg.Alg_Int_CarrierSync();                        //载波同步发送  接收
//		objLBS.Dat_Int_LBSCapDeal();							//debugonly
		objInvAlg.Alg_Int_InvSyncPulseRec();

		objInvAlg.Alg_Int_MainInterfaceVar();
				//继电器off延迟模块
        if((objParallel.m_st_wParaSignalBak.bInvBpStatus==1)&&(objParallel.m_st_wParaSignal.bInvBpStatus==0))
        {
            objXfer.m_i16KMOffDelayFlag =1;
        }
        if(objXfer.m_i16KMOffDelayFlag==1)
        {
            objXfer.m_i16KMOffDelay++;
            if(objXfer.m_i16KMOffDelay >=K15msCnst)
            {
                objXfer.m_i16KMOffDelay=0;
                objXfer.m_i16KMOffDelayFlag =0;
            }

        }

		objDigitalIO.Dat_Int_MainInterfaceOut(); 				// 主程序接口1（输出数据处理)
    	objTimerDrv.Drv_Int_ClockMaintain();              		// 时钟维护
    	objMonInterface.App_Int_TimeLabel();

		objADDrv.Drv_Int_Sampling1Data();						//第一轮采样结果读取
		objGPIODrv.Drv_Int_ADCh2Cs();							// 选择第二次采样通道

		//-----Int_Measure1_Scale,Int_Measure1_Deal-----
		objInverter.Dat_Int_InvPreDeal();
		objBypass.Dat_Int_BpPreDeal();
//		objParallel.Dat_Int_ParaPreDeal();
		objDCBus.Dat_Int_DCBusPreDeal();
   		//----------------------------------------------
        if(objSystem.m_st_wFlagSystem.bSelfAgingAction==1)
        {
            objSelfAgingAlg.Alg_Int_SelfAging();  				// 自老化模式
	    }
	    else
	    {
	    	objInvAlg.Alg_Int_VoltDCReg();

	        objPowCANSched.App_Int_PrdTimer_Route();			//并机PowerCAN

	        if(objInverter.m_st_wFlagInv.bInv!=0)	   			// 逆变器开启
	        {

    	        		objInvAlg.Alg_Int_CurrShareReg();     			// 均流控制器

    		    		objInvAlg.Alg_Int_PhaseLock();        			// 锁相调节控制器

    		     		objInvAlg.Alg_Int_InvSyncPulseGen();			//工频同步脉冲发送

    		    		objInvAlg.Alg_Int_VoltReg();          			// 电压环控制器

    		    		objInvAlg.Alg_Int_CurrReg();          			// 电流环控制器

				                // 开环调节－注意将重复控制系数置0
				                //             SPWM开环发波
				                //objInvAlg.m_i16wUirD_0 = objInvAlg.m_i16wVrefRunMd_0;
								//objInvAlg.m_i16wUirQ_0 = 0;
				                //objInvAlg.m_i16wUirZ_0 = 0;
				                //objInvAlg.m_i16wKRptConLmt_0 = 0;
				         		//objDCBus.m_i16VbusP_0 = 3742;
				              	//objDCBus.m_i16VbusN_0 = 3742;

		              objInvAlg.Alg_Int_SPWM();            				// SPWM发波

			    objPWMDrv.Drv_PWMActive();							//PWM输出
	        }
    	    else
    	    {
    	    	objInvAlg.Alg_Int_InvOffReset();					// 关机状态复位处理

    	    	objPWMDrv.Drv_PWMInactive();						//禁止PWM输出
    	    }

    	}

    	objADDrv.Drv_AD_SOC();        				            // 启动第二轮采样

//      	objInvAlg.Alg_Int_RptCtrl();                // 重复控制器

		//objSPIDrv.Drv_SPIAct();								//SPI输入输出处理 优化中断时间20170901


		        //-----Int_Measure2_Scale,Int_Measure2_Deal-----



        //----------------------------------------------
        objInverter.Dat_Int_InvCal();                   // 逆变频率基准及有效值计算
        objBypass.Dat_Int_BpCal();                      // 旁路频率基准及有效值计算

		objADDrv.Drv_Int_Sampling2Data();					//第二轮采样结果读取
		objGPIODrv.Drv_Int_ADCh1Cs();	     				// 选择第一次采样通道
		objOutput.Dat_Int_OutPreDeal();
		objOutput.Dat_Int_OutCal();                     // 输出频率基准及有效值计算
//      objSwitches.Dat_Int_VBackFeedCal();
        objInvAlg.Alg_Int_MasterComm();             // CAN主机竞争辅助模块

        objInverter.Dat_Int_IinvDFTCal();
//      objSwitches.Alg_Int_BpSTSShort();               // 旁路晶闸管短路瞬时值检测
        objInvAlg.Alg_Int_LockStatus();             // 相位锁定状态检测 优化中断时间20170901



	//为了节省中断时间,部分中断程序分三个分支进行
	switch (i16_IntCnt_0)
	{
		case (0):
		{

			objDCBus.Alg_Int_DCOVProtect(); 			 	// 母线过压保护 RAMRUN
			//objOutput.Alg_Int_VoutFastChk();		     	// 输出电压快速检测 NOTRAMRUN
			break;
		}
		case (1):
		{
//            objInverter.Alg_Int_InvOverCurrentChk();	//逆变不检输出电流故屏蔽	//逆变大电流快检,防止主旁共通炸机   NOTRAMRUN
			objSwitches.Alg_Int_STSDiffFastChk(); 		 	// 输出异常快速检测     RAMRUN
//			if(((objMonInterface.m_st_wUPSModeRun.bConfig == 0) && (objMonInterface.m_st_wUPSModeRun.bMode == 1))
//					||((objMonInterface.m_st_wUPSModeRun.bConfig == 2) && (objMonInterface.m_st_wUPSModeRun.bMasterECO == 1)))
//			{
//				objOutput.Alg_Int_VOutRangeFastChk();   //NOTRAMRUN
//			}
//			break;
		}
		case (2):
		{
//			objOutput.Alg_Int_VoutFastChk2();               //输出电压掉电快检20ms NOTRAMRUN
			//objOutput.Alg_Int_ShortCircuit();
//			objOutput.Alg_Int_VoutAbnormal();				// 输出电压异常检测 NOTRAMRUN
			//objLBS.Alg_Int_LBSDetect(); 			 		// LBS信号检测  NOTRAMRUN
			objSwitches.Alg_Int_INVKMFastChk();				//输出继电器开路快检    RAMRUN
			break;
		}
		default:
		{
			break;
		}
	}
        objDigitalIO.Dat_Int_MainInterfaceIn();		            // 主程序接口2（输入数据处理）
		objPowCANSched.Alg_Int_MainInterfaceVar();
    	objDebug.App_Int_WaveRecord();					// 波形记录
    }

    // 维修模式和工装模式
    else
    {
    	objDigitalIO.Dat_Int_MainInterfaceOut();		// 主程序接口1（输出数据处理）
    	objTimerDrv.Drv_Int_ClockMaintain();            // 时钟维护
    	objDigitalIO.Dat_Int_MainInterfaceIn();			// 主程序接口2（输入数据处理）
    	EALLOW;
    	EPwm6Regs.TZCLR.bit.OST = 1;                    //允许工频同步脉冲发送
        EDIS;
    	objInvAlg.Alg_Int_InvSyncPulseGen();			//工频同步脉冲发送
		objInvAlg.Alg_Int_InvSyncPulseRec();            //工频同步脉冲接收
		objParallel.m_st_wParaSignalBak.bMasterOnline = 1;
    	objInvAlg.Alg_Int_CarrierSync();
		objInverter.m_st_wFlagInv.bPhaseTrace = 0;		//本振
		objInvAlg.Alg_Int_PhaseLock();                 	// 锁相调节控制器
        objInvAlg.m_i16wUirD_0 = 5792;
	    objInvAlg.m_i16wUirQ_0 = 0;
	    objInvAlg.m_i16wUirZ_0 = 0;
	    objInvAlg.m_i16wKRptConLmt_0 = 0;
	    objDCBus.m_i16VbusP_0 = 3742;
	    objDCBus.m_i16VbusN_0 = 3742;
	    objInvAlg.Alg_Int_SPWM(); // SPWM发波

	    objSPIDrv.Drv_SPIAct();
		objDebug.App_Int_WaveRecord();					// 波形记录

	}

	i16_IntCnt_0 ++;										//IntCnt_0 [0,1,2]
	if ((i16_IntCnt_0 > 2)||(i16_IntCnt_0<0))
	{
		i16_IntCnt_0 = 0;
	}

    //GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;

}

//===========================================================================
// End of file.
//===========================================================================
