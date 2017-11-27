/*==========================================================*/ 	
/* Title		:	Switches.h													*/ 	
/* Description	: 	Class_Switches definition										*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#ifndef 	SWITCHES_H

#define		SWITCHES_H

	struct	structFlagBreaker
	{
		UINT16	bQ5					:1;				//BIT00		����տ�״̬ 0��	1��		
		UINT16	bBp					:1;				//BIT01		��·����״̬ 0�� 1��		
		UINT16	bInv				:1;				//BIT02		���������״̬ 0��	1��		
		UINT16	bOn					:1;				//BIT03		��������� 0��	1��		
		UINT16	bQ3bp				:1;				//BIT04		ά����·�տ� 0��	1��		
		UINT16	bReqBpSTSShort		:1;				//BIT05		�Ƿ��ڻ���·��բ�ܶ�·���� 0����	1�ڻ�
		UINT16	bFuse				:1;				//BIT06		�����˿ 0��	1��		
		UINT16	bBackFeed			:1;				//BIT07     ��·�����־���͸���ذ壬�Լ����Դ������ϼ�¼					
		UINT16	bBpSTS				:1;				//BIT08		��·��բ������ 0��	1��		
		UINT16	bInvKM				:1;				//BIT09		��侧բ������ 0��	1��		
		UINT16	bBpOn				:1;				//BIT10		��·�������־	0��	1��		
		UINT16	bKM5FClose			:1;				//BIT11		208V���Ӵ����ɿ���ͨ��־������ȷ�����ɸ���ѭ����				0��				1��		
		UINT16	bBpSTSOpen			:1;				//BIT12		��·��բ�ܿ�·���� 0�޹���	1��·		
		UINT16	bBpSTSShort			:1;				//BIT13		��·��բ�ܶ�·���� 0�޹���	1��·		
		UINT16	bInvKMOpen			:1;				//BIT14		��侧բ�ܿ�·���� 0�޹���	1��·		
		UINT16	bInvKMShort			:1;				//BIT15		��侧բ�ܶ�· 0�޹���	1��·		
	};			
	class Class_Switches
	{
	public:
		volatile struct	structFlagBreaker	m_st_FlagBreaker,m_st_wFlagBreakerBak;	//�տ�״̬, �������
//		volatile struct	structFlagTrueBreaker	sFlagTrueBreaker;			//�տ�״̬, �������
		INT16		m_i16VbackFeed_0; 						//��·���ౣ����ص�ѹ�����
//		INT16		m_i16KbackFeed_0;						//��·���ౣ����ص�ѹ����ϵ��
		UNLONG		m_unVbackFeedRms;						//��·���ౣ����ѹ��Чֵ(���˲�β��)
		
	private:	
		UNLONG		m_unVbackFeedSum_0;						//��·���ౣ����ص�ѹƽ���ۼӺ�,�ж��ۼ���
		UNLONG		m_unVbackFeedSumM_0;					//��·���ౣ����ص�ѹƽ�����ں�,����������Чֵ��
		INT16		m_i16VbackFeedPrms;						//��·���ౣ����ѹ��ʱ��Чֵ
		  
		UNLONG		m_unIinvaFilter_0;   			        //�������˲�ֵ
		UNLONG		m_unIinvbFilter_0;                      //�������˲�ֵ
		UNLONG		m_unIinvcFilter_0;                      //�������˲�ֵ
		           
		INT16		m_i16BpSTSBackwardFaultCnt_0;			//��·��բ�ܶ�·ȷ�ϼ���
		INT16		m_i16BpSTSForwardFaultCntPer40ms;		//��·��բ�ܶ�·ȷ�ϼ��� FEED FORWARD
		INT16		m_i16BpSTSOpenCntPer40ms;				//��·��բ�ܿ�·ȷ�ϼ���
		INT16 		m_i16FeedbackVcomCntPer14ms;			//��·���ౣ��
		INT16		m_i16InvSTSFaultCntPer40ms;				//��侧բ�ܶ�·ȷ�ϼ���
		INT16		m_i16InvSTSFault1CntPer14ms;
		INT16		m_i16InvSTSOpenCntPer40ms;				//��侧բ�ܿ�·ȷ�ϼ���
		INT16		m_i16SCRDiffFastChkCnt_0;				//��������ѹѹ����ټ�������	
		INT16		m_i16InvKMFastChkCnt;
		INT16		m_i16InvSTSFaultDisableCntPer40ms; //��·��բ�ܶ�·��ֹ������---�������ѹȫ��ʱ,��ͨ���������ѹ����·��բ�ܹ���
		INT16		m_i16STSShortCntPer40ms;			//��·��·ȷ�ϼ�����
		
	public:
		inline	void		App_SwitchesVarInit(void);
//		inline	void		App_SwitchesParamConfig(void);
		        	
//		void 		Dat_Int_VBackFeedCal(void);
//		void		Dat_VBackFeedRMS(void);
		        	
		void		Dat_BreakerStatus(void);
		        	
//		void		Alg_Int_BpSTSShort(void);
		void		Alg_Int_STSDiffFastChk(void);
		void		Alg_Int_INVKMFastChk(void);
		void		Alg_STSShortFault(void);
		void		Alg_STSOpenFault(void);
		
	};

#endif
//===========================================================================
// End of file.
//===========================================================================
