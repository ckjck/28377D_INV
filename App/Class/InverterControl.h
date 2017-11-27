/*==========================================================*/ 	
/* Title		:	InverterControl.h												*/ 	
/* Description	: 	Class_InverterControl definition									*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#ifndef		INVERTERCONTROL_H
       
#define     INVERTERCONTROL_H

struct	structFlagInvOnoff
	{
		UINT16	bMonitorOn					:1;				//BIT00		��ؿ���    	1 ����0 ��		
		UINT16	bFaultAutoOn				:1;				//BIT01		���ϻָ��Կ���	1 ����  (����Ȼָ�)0 ��		
		UINT16	bSwitchAutoOn				:1;				//BIT02		����л��Կ���  1 ����	0 ��		
//cb 030724 �û��ػ���Ϊ���л�����ػ���bUserOffֻ�л���bUserOffAct�Źػ�		
//ע�����������λ���彻����ԭBIT4����BIT3�� ��BIT4��Ϊ�ػ�����!
		UINT16	bUserOff					:1;				//BIT03		�û��ػ��л�	1 �ػ��л�	0 ��		
		UINT16	bUserOffAct					:1;				//BIT04		�û��ػ�����	1 �ػ�	0 ��		
		UINT16	bEPO						:1;				//BIT05		�����ػ�		1 �ػ�	0 ��		
		UINT16	bInvFaultOff				:1;				//BIT06		�����Ϲػ�	1 �ػ�	0 ��		
		UINT16	bIGBTOCPOff					:1;				//BIT07		IGBT�����ػ�	1 �ػ�	0 ��		
		UINT16	bInstantOff					:1;				//BIT08		�����ػ�		1 �ػ�	0 ��		
		UINT16	bOutDisableOff				:1;				//BIT09		��ֹ̬�ػ�  	1 �ػ�	0 ��		
		UINT16	bBpFailOff					:1;				//BIT10		��·�쳣�ػ�	1 �ػ�  (������ʱ�䵽��·�쳣 ��·�������쳣 ���غ���·5�������쳣 ���д�������·�쳣)0 ��
		UINT16	bAutoRecoverOff				:1;				//BIT11		�����лָ����Ϲػ� 	1 �ػ�	0 ��		
		UINT16	bManualRecoverOff			:1;				//BIT12		����Ϊ�ָ����Ϲػ� 	1 �ػ�	0 ��		
		UINT16	bNeverRecoverOff			:1;				//BIT13		���ɻָ����Ϲػ�  	1 �ػ�	0 ��		
		UINT16	bSwitchAutoOff				:1;				//BIT14		����л��Թػ�	 	1 �ػ�	0 ��		
		UINT16	bFinalOnOff					:1;				//BIT15		�ܿ��ػ�  			1 ����	0 �ػ� 		
	};	
	
	class Class_InverterControl
	{
	public:
		// ���±����������ִ��д���������ڴӹ��ϣ���أ��л���ģ���ռ����ֿ��ػ����
		volatile struct	structFlagInvOnoff	m_st_wFlagInvOnoff;						//��俪�ػ�״̬
		volatile struct	structFlagInvOnoff	m_st_FlagInvOnoffBak;					//��俪�ػ�״̬
		
		INT16		m_i16InvSSCntPer40ms;											//�������������ʱ, ��40msΪ��λ
                                                									
		INT16		m_i16FlagOffReason;												//���ػ�ԭ��	 	
																					//			BIT03--00	�ػ�ԭ��(�ֵȼ�,�ɸ���)
																					//	0000--����״̬
																					//	0001--�л��ػ�
																					//	0010--���Իָ����Ϲػ�
																					//	0011--�����ػ������Ϊ �ָ����Ϲػ�
																					//	0100--�����������ػ� ������ػ�,��ֹ̬�ػ�����	
																					//           ·�����ػ�
																					//	0101--���ɻָ����Ϲػ�
																					//

	private:
		INT16		m_i16wVrefTgtMd;													//��ѹĿ��ʸ��d�����	
		INT16		m_i16AutoOnDlyCntPer40ms;
		INT16		m_i16SoftStartCount;				//test
		INT16		m_i16MasterOnlineConfCnt;										//����������ʱ������
		INT16		m_i16ModelReadyCnt;
		INT16		m_i16AmpLockBpCnt;				//��ֵ������ʱ������				
		INT16		m_i16AmpLockTgtCnt;				//��ֵ������ʱ������				

	public:
		inline	void		App_InvCtrlVarInit(void);
		void		App_InvCtrl(void);
			    	
	private:   
		void		App_InvOffReason(void); 	
		void		App_InvOnoffAgent(void);
		void		App_InvOnoffAction(void);
		void		App_InvMaster(void);
		void		App_InvTrace(void);
		void		App_InvSoftstart(void);
		void		App_InvStandby(void);
                	
//		void		App_InvOffReason(void);
                	
		void		App_TraceSignal( void );
		void		App_TraceControl( void );
		void		App_TraceJudge( void );
		
	};

#endif
//===========================================================================
// End of file.
//===========================================================================
