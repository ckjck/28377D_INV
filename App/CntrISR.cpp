/*=======================================================*/
/* Filename��	cntr.c                                   */
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
    	PieCtrlRegs.PIEACK.all |= 0xFFFF;				//PIE����,��ACK��־   Acknowledge this interrupt to receive more interrupts from group 3


	objInvAlg.Alg_Int_InvSyncRecord();
//	asm(" setc OVM");//???

	if((objSystem.m_st_wFlagSystem.bIoTestMode == 0) && (objSystem.m_st_wFlagSystem.bServiceMode == 0))
	{
			//asm(" RPT #99 || NOP");
	     	//asm(" RPT #99 || NOP");
	     	//asm(" RPT #99 || NOP");
	     	//asm(" RPT #49 || NOP");


        // ����ģʽ���������ϻ�ģʽ
       	objADDrv.Drv_AD_SOC();      // ������һ�ֲ���
		objInvAlg.Alg_Int_CarrierSync();                        //�ز�ͬ������  ����
//		objLBS.Dat_Int_LBSCapDeal();							//debugonly
		objInvAlg.Alg_Int_InvSyncPulseRec();

		objInvAlg.Alg_Int_MainInterfaceVar();
				//�̵���off�ӳ�ģ��
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

		objDigitalIO.Dat_Int_MainInterfaceOut(); 				// ������ӿ�1��������ݴ���)
    	objTimerDrv.Drv_Int_ClockMaintain();              		// ʱ��ά��
    	objMonInterface.App_Int_TimeLabel();

		objADDrv.Drv_Int_Sampling1Data();						//��һ�ֲ��������ȡ
		objGPIODrv.Drv_Int_ADCh2Cs();							// ѡ��ڶ��β���ͨ��

		//-----Int_Measure1_Scale,Int_Measure1_Deal-----
		objInverter.Dat_Int_InvPreDeal();
		objBypass.Dat_Int_BpPreDeal();
//		objParallel.Dat_Int_ParaPreDeal();
		objDCBus.Dat_Int_DCBusPreDeal();
   		//----------------------------------------------
        if(objSystem.m_st_wFlagSystem.bSelfAgingAction==1)
        {
            objSelfAgingAlg.Alg_Int_SelfAging();  				// ���ϻ�ģʽ
	    }
	    else
	    {
	    	objInvAlg.Alg_Int_VoltDCReg();

	        objPowCANSched.App_Int_PrdTimer_Route();			//����PowerCAN

	        if(objInverter.m_st_wFlagInv.bInv!=0)	   			// ���������
	        {

    	        		objInvAlg.Alg_Int_CurrShareReg();     			// ����������

    		    		objInvAlg.Alg_Int_PhaseLock();        			// ������ڿ�����

    		     		objInvAlg.Alg_Int_InvSyncPulseGen();			//��Ƶͬ�����巢��

    		    		objInvAlg.Alg_Int_VoltReg();          			// ��ѹ��������

    		    		objInvAlg.Alg_Int_CurrReg();          			// ������������

				                // �������ڣ�ע�⽫�ظ�����ϵ����0
				                //             SPWM��������
				                //objInvAlg.m_i16wUirD_0 = objInvAlg.m_i16wVrefRunMd_0;
								//objInvAlg.m_i16wUirQ_0 = 0;
				                //objInvAlg.m_i16wUirZ_0 = 0;
				                //objInvAlg.m_i16wKRptConLmt_0 = 0;
				         		//objDCBus.m_i16VbusP_0 = 3742;
				              	//objDCBus.m_i16VbusN_0 = 3742;

		              objInvAlg.Alg_Int_SPWM();            				// SPWM����

			    objPWMDrv.Drv_PWMActive();							//PWM���
	        }
    	    else
    	    {
    	    	objInvAlg.Alg_Int_InvOffReset();					// �ػ�״̬��λ����

    	    	objPWMDrv.Drv_PWMInactive();						//��ֹPWM���
    	    }

    	}

    	objADDrv.Drv_AD_SOC();        				            // �����ڶ��ֲ���

//      	objInvAlg.Alg_Int_RptCtrl();                // �ظ�������

		//objSPIDrv.Drv_SPIAct();								//SPI����������� �Ż��ж�ʱ��20170901


		        //-----Int_Measure2_Scale,Int_Measure2_Deal-----



        //----------------------------------------------
        objInverter.Dat_Int_InvCal();                   // ���Ƶ�ʻ�׼����Чֵ����
        objBypass.Dat_Int_BpCal();                      // ��·Ƶ�ʻ�׼����Чֵ����

		objADDrv.Drv_Int_Sampling2Data();					//�ڶ��ֲ��������ȡ
		objGPIODrv.Drv_Int_ADCh1Cs();	     				// ѡ���һ�β���ͨ��
		objOutput.Dat_Int_OutPreDeal();
		objOutput.Dat_Int_OutCal();                     // ���Ƶ�ʻ�׼����Чֵ����
//      objSwitches.Dat_Int_VBackFeedCal();
        objInvAlg.Alg_Int_MasterComm();             // CAN������������ģ��

        objInverter.Dat_Int_IinvDFTCal();
//      objSwitches.Alg_Int_BpSTSShort();               // ��·��բ�ܶ�·˲ʱֵ���
        objInvAlg.Alg_Int_LockStatus();             // ��λ����״̬��� �Ż��ж�ʱ��20170901



	//Ϊ�˽�ʡ�ж�ʱ��,�����жϳ����������֧����
	switch (i16_IntCnt_0)
	{
		case (0):
		{

			objDCBus.Alg_Int_DCOVProtect(); 			 	// ĸ�߹�ѹ���� RAMRUN
			//objOutput.Alg_Int_VoutFastChk();		     	// �����ѹ���ټ�� NOTRAMRUN
			break;
		}
		case (1):
		{
//            objInverter.Alg_Int_InvOverCurrentChk();	//��䲻���������������	//����������,��ֹ���Թ�ͨը��   NOTRAMRUN
			objSwitches.Alg_Int_STSDiffFastChk(); 		 	// ����쳣���ټ��     RAMRUN
//			if(((objMonInterface.m_st_wUPSModeRun.bConfig == 0) && (objMonInterface.m_st_wUPSModeRun.bMode == 1))
//					||((objMonInterface.m_st_wUPSModeRun.bConfig == 2) && (objMonInterface.m_st_wUPSModeRun.bMasterECO == 1)))
//			{
//				objOutput.Alg_Int_VOutRangeFastChk();   //NOTRAMRUN
//			}
//			break;
		}
		case (2):
		{
//			objOutput.Alg_Int_VoutFastChk2();               //�����ѹ������20ms NOTRAMRUN
			//objOutput.Alg_Int_ShortCircuit();
//			objOutput.Alg_Int_VoutAbnormal();				// �����ѹ�쳣��� NOTRAMRUN
			//objLBS.Alg_Int_LBSDetect(); 			 		// LBS�źż��  NOTRAMRUN
			objSwitches.Alg_Int_INVKMFastChk();				//����̵�����·���    RAMRUN
			break;
		}
		default:
		{
			break;
		}
	}
        objDigitalIO.Dat_Int_MainInterfaceIn();		            // ������ӿ�2���������ݴ���
		objPowCANSched.Alg_Int_MainInterfaceVar();
    	objDebug.App_Int_WaveRecord();					// ���μ�¼
    }

    // ά��ģʽ�͹�װģʽ
    else
    {
    	objDigitalIO.Dat_Int_MainInterfaceOut();		// ������ӿ�1��������ݴ���
    	objTimerDrv.Drv_Int_ClockMaintain();            // ʱ��ά��
    	objDigitalIO.Dat_Int_MainInterfaceIn();			// ������ӿ�2���������ݴ���
    	EALLOW;
    	EPwm6Regs.TZCLR.bit.OST = 1;                    //����Ƶͬ�����巢��
        EDIS;
    	objInvAlg.Alg_Int_InvSyncPulseGen();			//��Ƶͬ�����巢��
		objInvAlg.Alg_Int_InvSyncPulseRec();            //��Ƶͬ���������
		objParallel.m_st_wParaSignalBak.bMasterOnline = 1;
    	objInvAlg.Alg_Int_CarrierSync();
		objInverter.m_st_wFlagInv.bPhaseTrace = 0;		//����
		objInvAlg.Alg_Int_PhaseLock();                 	// ������ڿ�����
        objInvAlg.m_i16wUirD_0 = 5792;
	    objInvAlg.m_i16wUirQ_0 = 0;
	    objInvAlg.m_i16wUirZ_0 = 0;
	    objInvAlg.m_i16wKRptConLmt_0 = 0;
	    objDCBus.m_i16VbusP_0 = 3742;
	    objDCBus.m_i16VbusN_0 = 3742;
	    objInvAlg.Alg_Int_SPWM(); // SPWM����

	    objSPIDrv.Drv_SPIAct();
		objDebug.App_Int_WaveRecord();					// ���μ�¼

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
