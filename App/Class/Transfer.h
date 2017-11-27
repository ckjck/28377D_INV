/*==========================================================*/ 	
/* Title		:	Transfer.h													*/ 	
/* Description	: 	Class_Transfer definition										*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
 #ifndef	TRANSFER_H
            
 #define    TRANSFER_H


//DSP�쳣����·��
#define 	DSPCrazyA	*(volatile UINT16 *)0xAFFC		//DSP CRAZY TEST BIT A
#define 	DSPCrazyB	*(volatile UINT16 *)0xAFFD		//DSP CRAZY TEST BIT B
#define 	DSPCrazyC	*(volatile UINT16 *)0xAFFE		//DSP CRAZY TEST BIT C
#define 	DSPCrazyD	*(volatile UINT16 *)0xAFFF		//DSP CRAZY TEST BIT D
                                                      		
	struct	structXferCmd
	{
		UINT16	bSwitchStatus				:1;				//BIT00		SWITCH_STATUS  0:��·��1:����								
		UINT16	bRev0301					:3;		
		UINT16	bSwitchMode					:2;				//BIT05-04	SWITCH_MODE	0:���� 1:����� 2:���	
		UINT16	bRev0607					:2;				//BIT07-06
		UINT16	bSwitchDesire				:1;				//BIT08		SWITCH_DESIRE	0:��·��1:����							
		UINT16	bIGBTOCPBak					:1;				//BIT09		 �����Ӳ�����������ź� 0:�޹���1:�й���
		UINT16	bWaitingZeroCross			:1;				//BIT10		��·��������л�ʱ�ȴ���������ӳٱ�־ 0:��ʱδ��ʼ�����ѽ��� 1:�ӳ���
		UINT16	bSwOverlapDelay				:1;				//BIT11		208V�ص��л���־ 0���ص���ʱ��	1�ص���ʱ��			
		UINT16	bSwitchInterval				:1;				//BIT12		0:���л������	1:�л������
		UINT16	bRev13						:2;				//BIT14-13
		UINT16	bOvloadSwitch				:1;				//BIT15		����95���������� 0:�ǹ��ػ���1:���ػ���
	};				


	struct	structXferResult
	{
		UINT16	bInvKM						:1;				//BIT00		���STS	0:���� 1:����
		UINT16	bBpSTS						:1;				//BIT01		��·STS	0:���� 1:����	
		UINT16	bInvKMBak					:1;				//BIT02		���STS����bInvKMBak 0:���� 1:����
		UINT16	bEcoInvMdUpFlag				:1;				//BIT03	ECOģʽ����䲻�������·ʱ��Чֵ��߱�־
																		//0:��������1:������
																
		UINT16	bEcoInvMdUpOk				:1;				//BIT04	ECOģʽ����䲻�������·ʱ��Чֵ��߽�����־
																		//0:δ���1:�����
		UINT16	bRev1105					:7;				//BIT11-05
		UINT16	bInvSideDelay				:2;				//BIT13-12	���5������ʱ��־	
																		//00���಻����ʱ
																		//10��������ʱ,��δ��ʼ
																		//11��������ʱ,�ѿ�ʼ;��ʱ����ת��Ϊ00
		UINT16	bBpSideDelay				:2;				//BIT15-14	��·5������ʱ��־		
																		//00��·�಻����ʱ
																		//10��·������ʱ,��δ��ʼ
																		//11��·������ʱ,�ѿ�ʼ; ��ʱ����ת��Ϊ00		
	};
	
	
	class Class_Transfer
	{
	public:	
			
		volatile struct	structXferCmd		m_st_wXferCmd,m_st_XferCmdBack;			//�л������γ�
		volatile struct	structXferResult	m_st_XferResult;						//�л����

		INT16		m_i16BPDelayCnt;	
	    INT16		m_i16InvFreeRunBasedOnBpFreqCount;
		INT16		m_i16FlagInvFreeRunBasedOnBpFreq;
		INT16		m_i16wFlagInvFreeRunBasedOnBpFreqTraceMinus;	
		            
		INT16		m_i16wBptoinvTimes;			     	                   			//������·��������,
		                                                                   			
		INT16	 	m_i16wBptoinvSwCntPer40ms;	     	                   			//��·������л��ȴ���ʱ  
	  	INT16	m_i16BpSTSOffDelay;											//��·��բ�ܶϿ���ʱ						                                           			
	  	INT16	m_i16BpSTSOffCnt;											//��·��բ�ܶϿ�������						                                           			

		INT16	m_i16KMOffDelayFlag;
		INT16	m_i16KMOffDelay;
		INT16	m_i16KMOnCntPer14ms;
	private:                                                               			
		UINT16 		m_u16ClkUsIntSw;			     	                   			//����л���ʱ���, 12ms	            	                                                       			
		UINT16 		m_u16ClkSBpside;			     	                   			//��������·��ʱ���, 5����ʱ��	            	                                                       			
		UINT16 		m_u16ClkSInvside;			     	                   			//ECO������ʱ���, 5����ʱ��	                                            				 
		INT16		m_i16SystemOvloadCntPerMloop;									//�ڻ�����ȷ�ϼ�����--ÿ��Сʱ����(CLK_H)�������趨����		                                                                    		
		INT16 		m_i16XferBpID;													//����·����
		//0:��������   1:Q5�� 2:Q3BP�� 3:������ر� 4:����������ʱ��  5:�����������·  6:�ڻ����س��ת��· 7:ECO                                              					                                                 					                                                              		
		INT16		m_i16XferBpIDBack;												//����·����                        
		INT16 		m_i16XferBpIDArray[10];											//��ǿ�����

	public:
		void 		App_XferOut(void);
			    	
	private:    	
		void		App_XferCommand(void);
		void		App_XferProcess(void);
		void		App_XferExecute(void);
	
	};
	
	
#endif	




//===========================================================================
// End of file.
//===========================================================================
