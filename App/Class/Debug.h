/*==========================================================*/
/* Title		:	Debug.h									*/
/* Description	:	Class_Debug	definition					*/
/* Date		:	Apr.2008									*/
/*==========================================================*/
 #ifndef		DEBUG_H

 #define		DEBUG_H

struct	structDebug
	{
		UINT16	bBpSTSShort1				:1;					//BIT00	��·��բ�ܶ�·�о�1
		UINT16	bBpSTSShort2				:1;					//BIT01	��·��բ�ܶ�·�о�2
		UINT16	bBpSTSShort3				:1;					//BIT02	��·��բ�ܶ�·�о�3�����ౣ����
		UINT16	bInvKMShort1				:1;					//BIT03	��侧բ�ܶ�·�о�1
		UINT16	bInvKMShort2				:1;					//BIT04	��侧բ�ܶ�·�о�2
		UINT16	bBpSTSOpen1					:1;					//BIT05	��·��բ�ܿ�·�о�1
		UINT16	bInvKMOpen1					:1;					//BIT06	��侧բ�ܿ�·�о�1
		UINT16	bSTSOpenFast				:1;					//BIT07	��բ�ܿ�·����о�
		UINT16	bDCAbnormalOff1				:1;					//BIT08	ĸ���쳣�ػ��о�1
		UINT16	bDCAbnormalOff2				:1;					//BIT09	ĸ���쳣�ػ��о�2
		UINT16	bDCAbnormalOff3				:1;					//BIT10	ĸ���쳣�ػ��о�3
		UINT16	bDCAbnormalOff4				:1;					//BIT11	ĸ���쳣�ػ��о�4
		UINT16	bInvFault1					:1;					//BIT12	����������о�1����Чֵ��
		UINT16	bInvFault2					:1;					//BIT13	����������о�2����죩
		UINT16	bVoutAbnormal1				:1;					//BIT14	�����ѹ�쳣�о�1����Чֵ��
		UINT16	bVoutAbnormal2				:1;					//BIT15	�����ѹ�쳣�о�2����죩
	};


class Class_Debug
	{
	public:
		volatile struct	structDebug		m_st_wDebug;			//���Ա���

		INT16		m_i16wDAchannel_0;							//DAת��ͨ��
		INT16		m_i16wInvDAOut0_0;							//10λDA�������0����10λ��
		INT16		m_i16wInvDAOut1_0;							//10λDA�������1����10λ��
		INT16		m_i16wInvDAOut2_0;							//10λDA�������2����10λ��
		INT16		m_i16wInvDAOut3_0;							//10λDA�������3����10λ��


	private:													//���ʾ����
		INT16		*pSCISavePoint_0;							//�������ݴ洢ָ��
		INT16		*pSCISendPoint_0;							//�������ݷ���ָ��
		INT16		m_i16SCISendPointCnt_0;						//���ݷ��͵���������
		INT16		m_i16SCISendSignalCnt_0;					//���ݷ����źż�����
		INT16		m_i16SCISendDelay_0;						//���ͼ����ʱ������
		INT16		m_i16SCISaveDelay_0;						//���ݱ�������ʱ������
		INT16		m_i16SumCheck_0;							//�����
		INT32		m_i32StopDelayCnt_0;
		INT16		m_i16Trop_0;
		union SCIDebugFlag
		{
			UINT16	all;
			struct
			{
				UINT16 Sending			:1;						//1: ���ݴ����У�0�������ݴ�����
				UINT16 HighorLow		:1;						//1���������ݵĸ�λ�ֽڡ�1���������ݵĵ�λ�ֽ�
				UINT16 Saving			:1;						//1����ʼ�洢���ݣ�0�����ܴ洢����
				UINT16 ConditionSaveEn	:1;						//1:����ʹ�ܱ��� 0����ʹ���������� 1��ʹ����������
			}BIT;
		}unSCIDebugFlag_0;

	public:
		void	App_DebugVarInit(void);
		void	App_Int_WaveRecord(	void );
		void	App_DebugInf(void);

	};

	#pragma	DATA_SECTION(".WaveRecord")
	INT16 iSCIDataBuffer_0[2048];								//���ݴ洢����	RAMFLASH, 2k*16
#endif

//===========================================================================
// End of file.
//===========================================================================
