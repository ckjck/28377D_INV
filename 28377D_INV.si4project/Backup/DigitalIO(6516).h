/*==========================================================*/ 	
/* Title		:	DigitalIO.h								*/ 	
/* Description	: 	Class_DigitalIO definition				*/ 	
/* Date			:	Apr.2008								*/ 	
/*==========================================================*/
#ifndef		DIGITALIO_H
       
#define     DIGITALIO_H




		struct	structLocalSignal
		{
			UINT16	bBpSTS					:1;					//BIT00		��·����		
			UINT16	bInvKM					:1;					//BIT01		�������					
			UINT16	bModuleReadyOk			:1;					//BIT02				
			UINT16	bBpSTSBak				:1;					//BIT03		��·�����źű���		
			UINT16	bModelOnline			:1;					//BIT04		 ����·ģ�����ڿ���SCR����������Ȩ����		     		
			UINT16	bNormalInd				:1;					//BIT05		ָʾģ����ϻ�δ׼����
			UINT16	bFaultInd				:1;					//BIT06		ָʾģ������		
			UINT16	bRev07					:1;					//BIT07		����		
			UINT16	bBPSF					:1;					//BIT08
			UINT16	bDSPFirmWareOK			:1;					//BIT09	DSP��Ӳ��ƥ����
			UINT16	bCarrierSyn				:1;					//BIT10	�ز�ͬ��
			UINT16	bHardWD					:1;					//BIT11 Ӳ��ι��
			UINT16	bLed					:2;					//BIT12-13	ָʾ��
			UINT16	bInvOffToCPLD			:1;					//BIT14	�����CPLD�Ĺػ�����
			UINT16	bBPFeedAction			:1;					//BIT15	��·�����ѿ�											
		};				
		struct	structLocalNewSignal
		{
			UINT16	bRev0					:1;					//BIT00
			UINT16	bRECFault				:2;					//BIT01-02
			UINT16	bEXT_OUT_STATUS			:1;					//BIT03
			UINT16	bCAB_Q3BP_STATUS		:1;					//BIT04
			UINT16	bQ3BP					:1;					//BIT05			
			UINT16	bINVOnMod1				:1;					//BIT06	bINVOnMod1			
			UINT16	bINVOnMod2				:1;					//BIT07	bINVOnMod2			
			UINT16	bINVOnMod3				:1;					//BIT08	bINVOnMod3			
			UINT16	bINVOnMod4				:1;					//BIT09	bINVOnMod4			
			UINT16	bINVOnMod5				:1;					//BIT10	bINVOnMod5						
			UINT16	bVoutAbnomalFromBP		:1;					//BIT11 bVoutAbnomalFromBP
			UINT16	bIMC_Enable				:1;					//BIT12	bIMC_Enable
			UINT16	bRev2					:1;					//BIT13	
			UINT16	bRECOvTemp				:1;					//BIT14
			UINT16	bOUT_PF					:1;					//BIT15
		};

		struct	structHardwareSignal
		{
			UINT16	bIGBTOCP				:1;					//BIT0		bIGBTOCP		
			UINT16	bRev					:1;					//BIT1		Q2	  	
	  		UINT16	bQ3BP					:1;					//BIT2		Q3BP
	  		UINT16	bQ5						:1;		  			//BIT3		Q5			
	  		UINT16	Rev1					:2;					//BIT4-5		
			UINT16	bFuse					:1;					//BIT6		bFuse					
			UINT16	bRev2					:1;					//BIT7				
			UINT16	bInvtIoTestMode			:1;					//BIT8		bInvtIoTestMode		
			UINT16	bInvtServiceMode		:1;					//BIT9		bInvtServiceMode		
			UINT16	bRev3					:1;					//BIT10				
			UINT16	bRecOff					:1;					//BIT11		bRecOff(From Rectifier)		
			UINT16	bRev4					:1;					//BIT12				
			UINT16	bDcOvp					:1;					//BIT13		bInvtDcOvp		
			UINT16	bFanFault				:1;					//BIT14		bFanFault		
			UINT16	bInvAutoOnEn			:1;					//BIT15							
		};
		struct	structHardwareNewSignal
		{
			UINT16	bRev					:1;					//BIT0		bModuleType	��·ģ��������ģ��	
			UINT16	bModuleReady			:1;					//BIT1		bInvtModuleReady	ģ���Ƿ�׼����
			UINT16	bDSPFirmWareOK			:1;					//BIT2		bDSPFirmWareOK		DSP���Ӳ���Ƿ�ƥ��	
			UINT16	bBPBackFeedDetect		:1;					//BIT3		bBPBackFeedDetect	��·�������ź�
			UINT16	bInvID					:4;					//BIT7-4		bInvID						ģ��ID����
			UINT16	bPower24vFault			:1;					//BIT8
			UINT16	bBPBackFeedDetect_F		:1;					//BIT09		ϵͳ����·�����־
			UINT16	bInvHeatSinkOVTemp		:1;					//BIT10
			UINT16	Rev1511					:5;					//BIT15-11
		};

		
		union ExOutAReg
		{
			UINT16	 		all;
		struct
			{
			UINT16	bPWMEN					:1; //BIT00 		
			UINT16	bINVKM					:1; //BIT01 			
			UINT16	bINVSCR 				:1; //BIT02 		
			UINT16	bRecRelaxCommand		:1; //BIT03 	
			UINT16	bInvOn					:1; //BIT04 	
			UINT16	bInvRunStatus			:3; //BIT05-07
			UINT16	bPWMENbak				:1;	//BIT08
			UINT16	bREV					:5; //BIT09-13				
			UINT16	bFrameID				:2; //BIT14-15		

			}BIT;
		};	
						
			
		union ExOutBReg
		{
			UINT16	 		all;
			struct
			{
			UINT16	bMACID					:8;	//BIT00-07			                                                        
			UINT16	bREV1					:3;	//BIT08-10		bREV1-3		
			UINT16	bIMC_Enable				:1;	//BIT11		bIMC_Enable	��ɢ�ź������־
			UINT16	bLBSStatus				:2;	//BIT12-13	LBS����λ		                           
			UINT16	bFrameID				:2;	//BIT14-15			

			}BIT;
		};	


		union ExOutCReg
		{
			UINT16	 		all;
			struct
			{
			UINT16	bINV_OVERLOAD			:1;	//BIT00		bINV_OVERLOAD			
			UINT16	bSYSTEM_UNLOCK			:1;	//BIT01		bSYSTEM_UNLOCK			
			UINT16	bINV_ON_STANDBY			:1;	//BIT02		bINV_ON_STANDBY			
			UINT16	bINV_BP_STATUS			:1;	//BIT03		bINV_BP_STATUS
			UINT16	bMASTER_ONLINE			:1;	//bit04
			UINT16	bModuleOnline			:1;	//BIT05		bModelOnline
			UINT16	bREV					:7;	//BIT06-12
			UINT16	bBPSCR					:1;	//BIT13		
			UINT16	bFrameID				:2;	//BIT14-15			

			}BIT;
		};	

		
		union ExInAReg
		{
			UINT16								all;
			struct
			{
				UINT16	bPower15VFault			:1; // BIT00	1�޹���
				UINT16	bIGBTOvTemp 			:1;	// BIT01	1�޹���
				UINT16	bVBusOVP				:1;	// BIT02	1�޹���
				UINT16	bEPO					:1; // BIT03	1�޹���
				UINT16	bInvFuseFault			:1;	// BIT04	0�޹���
				UINT16	bModuleReady			:1; // BIT05	1׼������
				UINT16	bInvHeatSinkOVTemp		:1; // BIT06	1�޹���
				UINT16	bRecOff					:1; // BIT07 0��ʾ��������
				UINT16	bQ5						:1; // BIT08	1 Q5�պ�		
				UINT16	bQ3Bp					:1; // BIT09 bQ3Bp ά�޿տ����������ź�
				UINT16	bBpFBDetect				:1;	// BIT10
				UINT16	bInvAutoOnEn			:1;	// BIT11 				
				UINT16	bBPSCRFromBp			:1;	// BIT12	��״̬����·��������BPSCR Status,�����·δ��ͨʱ���������·���磬��·��բ�ܿ�·����
				UINT16	bRev					:1; // BIT13
				UINT16	bFrameID				:2; // BIT14~15
				
			}									BIT;
		};											//SPI�����һ֡
							
		
		union ExInBReg
		{
			UINT16								all;
			struct
			{
				UINT16	bMPUFunc				:2; 				// BIT00-01
				UINT16	bMPUProductVer			:4; 				// BIT02-05
				UINT16	bMPUDone				:1; 				// BIT06
				UINT16	bRev					:7;					// BIT07-13 	
				UINT16	bFrameID				:2;					// BIT14-15
			}									BIT;
		};															//SPI����ڶ�֡


		union ExInCReg
		{
			UINT16								all;
			struct
			{
				UINT16	bMACID_F				:8; 				//BIT00-07		bBPS-3																
				UINT16	bREV1					:6; 				//BIT08-13		bREV1-3 									   
				UINT16	bFrameID				:2; 				//BIT14-15

			}									BIT;
		};															//SPI�������֡

		
		union ExInDReg
		{
			UINT16								all;
			struct
			{
				UINT16	bInv_OverLoad_F			:1; 				// BIT00	������(��ɢ�����ź�����)	 	,1��ʾ ����	
				UINT16	bSystem_Unlock_F		:1;					// BIT01	ϵͳ����·�������������־λ(��ɢ�����ź�����)�� 0 ��ʾ��·����	
				UINT16	bInv_ON_Standby_F		:1;					// BIT02	���ͬ�������ź�(��ɢ�����ź�����)	��0��ʾ��俪�����	
				UINT16	bInv_BP_Status_F		:1; 				// BIT03	�����·����״̬��־λ(��ɢ�����ź�����)	��1��ʾ���๩��	
				UINT16	bMaster_Online_F		:1;					// BIT04 ���������������߱�־	0:������ 1��������
				UINT16	bLeader_Rack_F			:1; 				// BIT05	����������ź�
				UINT16	bRev					:6; 				// BIT06-11
				UINT16	bBP_BackFeed_Detect_F	:1; 				// BIT12
				UINT16	bBPSCR_F				:1; 				// BIT13
				UINT16	bFrameID				:2; 				// BIT14~15

			}									BIT;
		};															//SPI�������֡


class Class_DigitalIO
	{
	public:
		volatile struct		structLocalSignal		m_st_wLocalSignal;		//��ӦEXT_OUT D C  
		volatile struct		structLocalNewSignal	m_st_wLocalNewSignal;
		volatile struct		structHardwareSignal	m_st_HardwareSignal; 	//Ӳ�������źż���ȷ�ϻ���
		volatile struct		structHardwareNewSignal	m_st_wHardwareNewSignal;//
	
		union		ExOutAReg			m_un_ExOutAReg;			//SPI��һ���������
		union		ExOutBReg			m_un_ExOutBReg;			//SPI�ڶ����������
		union		ExOutCReg			m_un_ExOutCReg;			//SPI�������������
		union		IPC_CPU1toCPU2		m_un_CPU1toCPU2;
		union		IPC_CPU2toCPU1		m_un_CPU2toCPU1;
		
		union		ExInAReg			m_un_ExInAReg;			//SPI��һ����������
		union		ExInBReg			m_un_ExInBReg;			//SPI�ڶ�����������
		union		ExInCReg			m_un_ExInCReg;			//SPI��������������
		union		ExInDReg			m_un_ExInDReg;			//SPI��������������
		union		ExInAReg			m_un_ExInARegConf;		//SPI��һ����������ȷ��
		
		                                                             	
		void	Dat_Int_SetExInReg(UINT16	u16_InAReg);			//SPI��������֡ʶ��
	private:                                                            								                                                                        			
		INT16		m_i16InvIDOkFlag;
		UINT16		m_u16InvID;
		UINT16		m_u16InvIDOld;	
		INT16		m_i16IoConfCntPer1ms[22];			//ȷ�ϼ�����
		UINT16		m_u16ComIndSatConfCntPer1ms;
		INT16		m_i16InvOcpConfCnt_0;				//�ж���INV��OCP�����ź�ȷ�ϼ�����
		INT16		m_i16BpBackFeedFIoConfCntPer1ms;	//ϵͳ��·����ȷ�ϼ�����
		INT16		m_i16IMCCtrlConfCnt;	//���ܼ���ɢ�źſ���ȷ�ϼ�����
		UINT16		m_u16InvSCROffCnt;
		UINT16		m_u16InvRunStatusCnt;
		

	public:
		void		App_DigitalIOVarInit(void);
//		void		Dat_IoDeal(void);
		        	
		void 		Dat_Int_MainInterfaceOut(void);
		void 		Dat_Int_MainInterfaceIn(void);
		void		Dat_DigitalInput(void);
		void		Dat_DigitalOutput(void);
		INT16		Dat_InvIDOk(void);
		void		App_IMCCtrl(void);	

	};

#endif
		
	

//===========================================================================
// End of file.
//===========================================================================
