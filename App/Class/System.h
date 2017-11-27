/*==========================================================*/ 	
/* Title		:	System.h														*/ 	
/* Description	: 	Class_System definition											*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#ifndef 	SYSTEM_H

#define		SYSTEM_H

	struct structFlagStatus
	{	
		UINT16	bVbp					:2;					//BIT01-00	��·״̬ 0������1�����л���Χ3����������Χ		
		UINT16	bInv					:2;					//BIT03-02	���״̬ 0���ر�1��������3���������		
		UINT16	bLock					:1;					//BIT04		�������״̬	0����ͬ��1��ͬ��		
		UINT16	bUserOffBak				:1;					//BIT05		�û��ػ�����ݣ����ڽ��2ms����0:��1:��		
		UINT16	bSupply					:2;					//BIT07-06	ϵͳ����״̬00��������01��·����10��乩��	
		UINT16	bEPO					:1;					//BIT08		EPO״̬	0��	1��(״̬)		
		UINT16	bInvFault				:1;					//BIT09		���������	0��	1��(״̬)		
		UINT16	bMainFeedBack			:1;					//BIT10		IGBT����	0��	1��(״̬)		
		UINT16	bOutDisable				:1;					//BIT11		�����ֹ	0��	1��(״̬)		
		UINT16	bIntSwitch				:1;					//BIT12		����л���ʾ0��	1��(��ʾ)		
		UINT16	bSingleRiskOff			:1;					//BIT13		�����ػ�������ʾ0��	1��(��ʾ)		
		UINT16	bParaRiskOff			:1;					//BIT14		�����ػ�������ʾ	0��	1��(��ʾ)		
		UINT16	bPow15vFaultBak			:1;					//BIT15	 	POW_OK�źű��� 		0��	1��			
	};
	
	struct	structFlagPrompt
	{	
		UINT16	bDCAbnormalOff			:1;					//BIT00		 ĸ�ߵ�ѹ�쳣�ػ�  		0��	1��		
		UINT16	bSwitchTimesClamp		:1;					//BIT01		 ��Сʱ�����л�������	0��	1��		
		UINT16	bImpactSwitch			:1;					//BIT02		 �������������·   	0��	1��		
		UINT16	bReqSwitch				:1;					//BIT03		 ϵͳ����ת��·���ڻ����س����0��1��		
		UINT16	bParaShareFault			:1;					//BIT04		 ������������ 	0��	1��		
		UINT16	bDCAbnormalOff1			:1;					//BIT05		
		UINT16	bParaCableFault			:1;					//BIT06		 ���������ӹ��� 	0��	1��		
		UINT16	bInvOnEnable			:1;					//BIT07		 ��������������־  	0��	1��		
		UINT16	bUnitCapPrompt			:1;					//BIT08		 ϵͳ�����л�, ���ش��ڵ������� 	0��	1��		
		UINT16	bTotalCapPrompt0		:1;					//BIT09		 ϵͳ���ش��ڵ�ǰ��ЧUPS������(��ʾ) 	0��1��		
		UINT16	bTotalCapPrompt			:1;					//BIT10		 ϵͳ���ش��ڵ�ǰ��ЧUPS������(����ʱ)	0��1��		
		UINT16	bParaOvload				:1;					//BIT11		 0��1��		
		UINT16	bShortCircuit			:1;					//BIT12		 ��������·��״̬��0��1��		
		UINT16	bVoutAbnormal			:1;					//BIT13		 �����ѹ�쳣�澯	0��	1��			
		UINT16	bPow24vFaultConf			:1;					//BIT14		 ȷ�Ϻ��POWOKFAULT��PowFault2���� 	0��	1��			
		UINT16	bPow15vFault				:1;					//BIT15		 POW_OK���� 	0��	1��		
	};	
	
	struct	structFlagFault
	{
		UINT16	bInvFault				:1;					//BIT00		���������	0��	1��		
		UINT16	bInvCurrentUnbalanced	:1;					//BIT01		������������������ƽ�� 0�� 1��					
		UINT16	bBpCurrentUnbalanced	:1;					//BIT02		��������·���������ƽ�� 0�� 1��
		UINT16	bFanFault				:1;					//BIT03		���ȹ���	0��	1��		
		UINT16	bDCOV					:1;					//BIT04		ĸ�߹�ѹ	0��	1��		
		UINT16	bVoutAbnormal			:1;					//BIT05		�����ѹ�쳣0��	1��		
		UINT16	bBpOverCurrent			:1;					//BIT06		��·����	0��	1��		
		UINT16	bInvDcOffset			:1;					//BIT07		���ֱ��������	0��	1��		
		UINT16	bOperFault				:1;					//BIT08		�û���������	0��	1��		
		UINT16	bOvload					:1;					//BIT09		�������		0��	1��	
		UINT16	bInvOvloadTimeout		:1;					//BIT10		����������س�ʱ0��	1��		
		UINT16	bBypFeedBackFault		:1;					//BIT11		IGBT����		0��	1��		
		UINT16	bAuxPS2Fault			:1;					//BIT12		������Դ����	
		UINT16	bDCAbnormal				:1;					//BIT13		ĸ���쳣		0��	1��		
		UINT16	bBpInvtSeq				:1;					//BIT14		��·����		0��	1��		
		UINT16	bBpFailOff				:1;					//BIT15		��·�쳣�ػ�	0��	1��	
	};
	
	struct	structFlagSystem
	{
		UINT16 	bAfterParamDly			:1;					//BIT00				0-��1-�������ú���ʱ����				
		UINT16 	bKeyModuleDly			:1;					//BIT01				0-��1-����ģ������ǰ��ʱ���		
		UINT16 	bParamSetReq			:1;					//BIT02				1-�����������	
		UINT16 	bUPSTestMode			:1;					//BIT03				1-����ģʽ����λ
		UINT16 	bParamFresh				:1;					//BIT04				1-�������ø��±�־			
		UINT16	bNewFaultArise			:1;					//BIT05				1-�¹��ϳ��ֱ�־				
		UINT16	bRecordSaveRequest		:1;					//BIT06				1-���ϼ�¼�ϴ������־			
		UINT16	bSelfAgingAction		:1;					//BIT07				1-���ϻ�ģʽ����λ��ֻ���ϵ�����ʱ��Ч			
		UINT16 	bRAMError				:1;					//BIT08				�ڴ��Լ��		
		UINT16 	bServiceMode			:1;					//BIT09				ά�޼��ģʽ		
		UINT16 	bWDStart				:1;					//BIT10				��ʼι����־			
		UINT16	bTimer1ms				:1;					//BIT11				1ms��ʱ��־		ÿ1ms��λһ��,������ģ���ѯ.�ñ�־�ڽ���ʱ���.		
		UINT16	bSelfAgingSet			:1;					//BIT12				1-���ϻ�ģʽ����λ�����Ÿ�SCIģ��			
		UINT16 	bIoTestMode				:1;					//BIT13 				DSP��װģʽ				
		UINT16 	bTimer14ms				:1;					//BIT14  			14ms��ʱ��־ ÿ14ms��λһ��,������ģ���ѯ.�ñ�־�ڽ���ʱ���.		
		UINT16 	bTimer40ms				:1;					//BIT15  			40ms��ʱ��־ ÿ40ms��λһ��,������ģ���ѯ.�ñ�־�ڽ���ʱ���.		
	};
	class Class_System
	{
	public:
		volatile struct structFlagStatus	m_st_wFlagStatus,m_st_wFlagStatusBak;	//ϵͳ״̬, �������
		volatile struct	 structFlagPrompt	m_st_wFlagPrompt,m_st_wFlagPromptBak;	//�������״̬
		volatile struct	 structFlagFault		m_st_wFlagFault,m_st_wFlagFaultBak;		//����״̬���������
		volatile struct	 structFlagSystem	m_st_wFlagSystem;					//ϵͳ״̬��־
				
		INT16		m_i16wNumMainloop;											//��ѭ�����0-9ѭ��, ���ڷ�ʱ����
		                                  								
		INT16		m_i161DivGrantKVA;											//iGrantKVA�ĵ���,���ڽ�CAN �㷨�еĳ��������Ϊ�˷�
		INT16		m_i161DivGrantRate;											//��Ȩ�ʵĵ���
//		INT16		m_i16GrantKVABak; 											//iGrantKVA�����ı���
        UINT16		m_u16TimerBig1ms;                                   								
		UINT16		m_u16DSPRevision;    
		INT16		m_i16wFlagCPLDVerErr;
		                                 								
		INT16		m_i16wAfterParamCntPer40ms;									//�������ú���ʱ, ��40msΪ��λ
		INT16		m_i16wKeyModuleCntPer40ms;									//����ģ������ǰ����, ��40msΪ��λ
		INT16		m_i16wWDStartCntPer40ms;									//Ӳ������ʱ������
		
		INT16		m_i16PoutaTotal;					//����A��������й�����(������·)
		INT16		m_i16PoutbTotal;					//����B��������й�����(������·)
		INT16		m_i16PoutcTotal;					//����C��������й�����(������·)
		INT16		m_i16SoutaTotal;					//����A����������ڹ���(������·)
		INT16		m_i16SoutbTotal;					//����B����������ڹ���(������·)
		INT16		m_i16SoutcTotal;					//����C����������ڹ���(������·)

		INT16		m_i16RackPoutaTotal;			//���ܼ䲢������--- A��������й�����(������·)
		INT16		m_i16RackPoutbTotal;			//���ܼ䲢������--- B��������й�����(������·)
		INT16		m_i16RackPoutcTotal;			//���ܼ䲢������--- C��������й�����(������·)
		INT16		m_i16RackSoutaTotal;			//���ܼ䲢������--- A����������ڹ���(������·)
		INT16		m_i16RackSoutbTotal;			//���ܼ䲢������--- B����������ڹ���(������·)
		INT16		m_i16RackSoutcTotal;			//���ܼ䲢������--- C����������ڹ���(������·)


		INT16		m_i16PoutTotalMax;
		INT16		m_i16SoutTotalMax;
		INT16		m_i16RackPoutTotalMax;
		INT16		m_i16RackSoutTotalMax;
		  
		INT16		m_i16CPLDVerD1;											//CPLD�汾��
		INT16		m_i16CPLDVerD2;				//CPLD�汾��								//CPLD�汾��
		INT16		m_i16CPLDVerD3;	                                								
	private: 
		INT16		m_i16FaultLedCnt;
		INT16		m_i16NormalLedCnt;
		
		INT16		m_i16FanFaultCntPer1ms;									//���ȹ���ȷ�ϼ�����								
		INT16		m_i16PowerFaultCntPer40ms;
		INT16 		m_i16PowerOKWrongCntPerMloop;								//POW_OK ����ȷ�ϼ�����	
		INT16		m_i16ModuleOkCnt;										//ModuleReady�ź�ȷ�ϼ�����
		INT16		m_i16ModuleNotOkCnt;
	public:
		void		App_SysVarInit(void);	
		void		App_ModelOnline(void);
		void		App_SysParamConfig(void);
			    	
		void		App_DSPIDCheck(void);		
		void		App_CPLD_Ver(void); 
		 void     	App_Module_Ok(void);
		 void	App_Module_LED(void);
		void		Alg_SysFault(void);		
		void		Alg_OperFault(void);
	
		
	};
	
#endif	
//===========================================================================
// End of file.
//===========================================================================
