/*=======================================================*/
/* Title:  	main.c                                       */
/* Description: Main Program			                 */
/* Author: 	Zhuo Qingfeng, Zhang Xuejie					 */
/* Date:	Nov.2005									 */
/*=======================================================*/	


extern UINT16 Epwm1FuncsLoadStart;
extern UINT16 Epwm1FuncsLoadEnd;
extern UINT16 Epwm1FuncsRunStart;



	int	main(void)

	{	
		objWatchDogDrv.Drv_DisableDog();			               
		
		//----------vSys_Init()--------------
//		asm(" setc OVM");
		objSysDrv.Drv_InitSysCtrl();
		objSysDrv.Drv_InitPieCtrl();
		objSysDrv.Drv_InitPieVectTable();
		
    	IER |= (M_INT3|M_INT9);	// Enable CPU INT1 INT3 INT9
    	
    	objRAMDrv.Drv_MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
    	objFlashDrv.Drv_InitFlash();
    	
    	objPWMDrv.Drv_PWMInit();
    	objADDrv.Drv_ADInit();
    	objGPIODrv.Drv_GPIOInit();
    	objSPIDrv.Drv_SPIInit();
    	objCAPDrv.Drv_CAPInit();
    	objI2CDrv.Drv_I2CInit();
    	objSCIDrv.Drv_SCIInit();
    	objCANDrv.Drv_CANInit();
	    objTimerDrv.Drv_CPUTimerInit();
		//-----------------------------------
	
		objRAMDrv.Drv_MemCheck();
		
		objRAMDrv.Drv_MemCopy(&Epwm1FuncsLoadStart, &Epwm1FuncsLoadEnd, &Epwm1FuncsRunStart);

		

		//--------vVar_Init()---------------
		objSystem.App_SysVarInit();  
		objPowCANSched.App_PowCANInit();
		objDCBus.App_DCBusVarInit();
		objBypass.App_BpVarInit();
		objInverter.App_InvVarInit();
		objOutput.App_OutVarInit();
//		objParallel.App_ParaVarInit();
		objInvCtrl.App_InvCtrlVarInit();
		objSwitches.App_SwitchesVarInit();
		objDigitalIO.App_DigitalIOVarInit();
		objMonInterface.App_MonitorVarInit();
		objFaultRecord.App_FaultRecordVarInit();
		objEEPROM.Dat_EEPROMInit();
		objInvAlg.App_InvAlgVarInit();
		objSelfAgingAlg.App_SelfAgingAlgVarInit();
		objDebug.App_DebugVarInit();

		//CAN相关对象初始化
		ProtocolObj.SysCanProtocolIni();
		SysCanAppObj.SysCanInitial();
		DataExchgObj.SysCanDataExchgIni();
		
		//----------------------------------
		
		objADDrv.Drv_ADAdjust();
		
		objFlashDrv.Drv_Encrypt();

		
		EINT;  // Enable Global interrupt INTM
    	ERTM;  // Enable Global realtime interrupt DBGM		
		
		objSystem.App_DSPIDCheck();  			

		INT16 i,k,m;
		for(i=k=m=0;i<3;i++)
		{
			objTimerDrv.Drv_usDelay(500);  	
					
//			if(objDigitalIO.m_st_wSPIDataIn2_0.bInvtIoTestMode==0)	j++;
		
			if(GpioDataRegs.GPDDAT.bit.GPIO99 == 0)	k++;
			
			if(objDigitalIO.m_un_ExInBReg.BIT.bMPUProductVer == 0)	m++;
		}

		if(k >= 2)
		{
			objSystem.m_st_wFlagSystem.bServiceMode=1;
		} 
		//逆变软件与CPLD软件版本不匹配，报“用户操作错误”，
		if(m >= 2)  
		{
		 	 objSystem.m_i16wFlagCPLDVerErr = 1;
		 }
		
		//逆变版本显示
		objSystem.App_CPLD_Ver();
		
			
		if(objSystem.m_u16DSPRevision >= 3)
		{
			objWatchDogDrv.Drv_EnableDog();			// Enable the watchdog
			
		}

		//主循环起始点	
		while(1)
		{	

		GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1;
            objSPIDrv.Drv_SPIAct();								//SPI输入输出处理 优化中断时间20170901
			objDigitalIO.App_IMCCtrl();
			if(objSystem.m_u16DSPRevision >= 3)
			{
				objWatchDogDrv.Drv_KickDog();
			}

			if(objSystem.m_st_wFlagSystem.bServiceMode == 1)
			{
				
				objSystem.m_st_wFlagSystem.bWDStart = 1;
                objTimerDrv.Drv_SysTimer();
				objService.App_ServiceMode();
				//objSPIDrv.Drv_SPIAct();
				continue;
			}
			
			
			objTimerDrv.Drv_SysTimer();
			objDigitalIO.Dat_DigitalInput();
			//objSystem.App_Module_LED();							//模块LED灯显示
			
			if (objDigitalIO.Dat_InvIDOk() == 1)
			{
			
				//System CAN 
				SysCanAppObj.MainRoutie();	        	
 				objMonInterface.App_MonitorDeal();
				objEEPROM.App_EEPROMAction();
             	
				//--------vIo_Deal()------------------
//				objDigitalIO.Dat_DigitalInput();
				objSwitches.Dat_BreakerStatus();
				objDigitalIO.Dat_DigitalOutput();
				//--------------------------------------
           	
				//--------vRMS_Power()------------------
				
				
				objSystem.m_i16wNumMainloop++;
				if(objSystem.m_i16wNumMainloop>6) 
				{
					objSystem.m_i16wNumMainloop=0;
				}
				switch(objSystem.m_i16wNumMainloop)
				{
					case 0:
						objBypass.Dat_BpRMSPower();
//						objSwitches.Dat_VBackFeedRMS();
						break;	
	        	
					case 1:
						objInverter.Dat_VinvRMS();
						break;	
						
					case 2:
						objInverter.Dat_IinvRMS();
						break;	
						
					case 3:
						objOutput.Dat_VoutRMS();
						break;
						
					case 4:
					    objInverter.Dat_TempPowerGrant();
					 	//objOutput.Dat_IoutRMS();
						//Bywxw20090601 Adapter20k增加旁路电流授权值的计算。每7个主循环刷新一次标志
						//objBypass.Dat_IbpFlag();
						break;
						
					case 5:
						
						objInverter.Dat_PinvRMS();
						break;
						
					case 6:
					    objDCBus.Dat_DCBusRMS();
						//objOutput.Dat_PoutRMS();
						break;
					
					default:
						break;
						
					}
				//--------------------------------------
   

				
				objLoad.Dat_LoadDeal();
            	//By wxw20090601 Adapter20k增加旁路过流关断功能
        		//objBypass.Dat_TempCurrentGrant();
				//objBypass.App_OverCurrentDelay();
				if(objMonInterface.m_st_wUPSModeRun.bConfig == 3)
				{
					objPowCANSched.Dat_Can_LoadCalculation();
					objPowCANSched.App_Can_ILFilter();
					objInvAlg.Alg_ILFilterReg();
					objInverter.Dat_IinvPQRMS();
					objPowCANSched.App_Can_IInvDQCal();
					objInvAlg.Alg_InvACShareReg();
				}	
				
				
				//-----------Fault_Seek()---------------
				objSwitches.Alg_STSOpenFault();
				//objSwitches.Alg_STSShortFault();
				objBypass.Alg_BpSeqFault();
			 	objBypass.Alg_BpBackFeedFault();
				//By wxw20090601 Adapter20K增加过流报警模块
				//短路模块在中断CntrISR中调用
				//objBypass.Alg_OvcurrFault();
				objSystem.Alg_SysFault();
            	
				//objBypass.Alg_BpFault();
				objDCBus.Alg_DCBusFault();
				objInverter.Alg_InvFault();		
				objOutput.Alg_VoutAbnormal();		
				objSystem.Alg_OperFault();
				objParallel.Alg_ParaStatus();
				objParallel.Alg_ParaFault();
				
				

				//--------------------------------------
            	
				if(objSystem.m_st_wFlagSystem.bParamSetReq == 1)	
				{
					continue;
				}


				objSwitches.Alg_STSShortFault();
					
					if(objSystem.m_st_wFlagSystem.bAfterParamDly != 1)
					{
						if(objSystem.m_st_wFlagSystem.bTimer40ms == 1)
						{	
							objSystem.m_i16wAfterParamCntPer40ms++;
						}
						if(objSystem.m_i16wAfterParamCntPer40ms >= 50)		
						{
							objSystem.m_st_wFlagSystem.bAfterParamDly = 1;
						}
					
					continue;
					}
         	
//PHASE1:				
				//---------Analog_Flag()------------
				objBypass.Dat_VbpFlag();
				objInverter.Dat_VinvFlag();
				//----------------------------------			
            	
//				objLBS.Alg_LBSSignal();         //debugonly
            	
				if(objSystem.m_st_wFlagSystem.bKeyModuleDly != 1)
				{
					if(objSystem.m_st_wFlagSystem.bTimer40ms == 1)	
					{
						objSystem.m_i16wKeyModuleCntPer40ms++;
					}
					if(objSystem.m_i16wKeyModuleCntPer40ms >= 75)		
					{	
						objSystem.m_st_wFlagSystem.bKeyModuleDly = 1;
					}
					continue;
				}
				objSystem.App_Module_Ok();				//moduleready信号形成
            	
				objSystem.App_ModelOnline();  				//modelonline信号发送
				//PHASE2:   	
				objInvCtrl.App_InvCtrl();
				
				objXfer.App_XferOut();
				objFaultRecord.App_FaultRecord();			//严重故障记录，注意调用位置
            	
				if(objSystem.m_st_wFlagSystem.bWDStart != 1)			//延时3s喂狗
				{
					if(objSystem.m_st_wFlagSystem.bTimer40ms == 1)	
					{
						objSystem.m_i16wWDStartCntPer40ms++;
					}
					if(objSystem.m_i16wWDStartCntPer40ms >= 75)		
					{
						objSystem.m_st_wFlagSystem.bWDStart = 1;
					}
				}  
			}
			
		}
}
       
    
    
    
	
//===========================================================================
// End of file.
//===========================================================================
