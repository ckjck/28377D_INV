/*==========================================================*/
/* Title		:	Debug.h									*/
/* Description	:	Class_Debug	definition					*/
/* Date		:	Apr.2008									*/
/*==========================================================*/
 #ifndef		DEBUG_H

 #define		DEBUG_H

struct	structDebug
	{
		UINT16	bBpSTSShort1				:1;					//BIT00	旁路晶闸管短路判据1
		UINT16	bBpSTSShort2				:1;					//BIT01	旁路晶闸管短路判据2
		UINT16	bBpSTSShort3				:1;					//BIT02	旁路晶闸管短路判据3（反灌保护）
		UINT16	bInvKMShort1				:1;					//BIT03	逆变晶闸管短路判据1
		UINT16	bInvKMShort2				:1;					//BIT04	逆变晶闸管短路判据2
		UINT16	bBpSTSOpen1					:1;					//BIT05	旁路晶闸管开路判据1
		UINT16	bInvKMOpen1					:1;					//BIT06	逆变晶闸管开路判据1
		UINT16	bSTSOpenFast				:1;					//BIT07	晶闸管开路快检判据
		UINT16	bDCAbnormalOff1				:1;					//BIT08	母线异常关机判据1
		UINT16	bDCAbnormalOff2				:1;					//BIT09	母线异常关机判据2
		UINT16	bDCAbnormalOff3				:1;					//BIT10	母线异常关机判据3
		UINT16	bDCAbnormalOff4				:1;					//BIT11	母线异常关机判据4
		UINT16	bInvFault1					:1;					//BIT12	逆变器故障判据1（有效值）
		UINT16	bInvFault2					:1;					//BIT13	逆变器故障判据2（快检）
		UINT16	bVoutAbnormal1				:1;					//BIT14	输出电压异常判据1（有效值）
		UINT16	bVoutAbnormal2				:1;					//BIT15	输出电压异常判据2（快检）
	};


class Class_Debug
	{
	public:
		volatile struct	structDebug		m_st_wDebug;			//调试变量

		INT16		m_i16wDAchannel_0;							//DA转换通道
		INT16		m_i16wInvDAOut0_0;							//10位DA输出数据0（低10位）
		INT16		m_i16wInvDAOut1_0;							//10位DA输出数据1（低10位）
		INT16		m_i16wInvDAOut2_0;							//10位DA输出数据2（低10位）
		INT16		m_i16wInvDAOut3_0;							//10位DA输出数据3（低10位）


	private:													//软件示波器
		INT16		*pSCISavePoint_0;							//波形数据存储指针
		INT16		*pSCISendPoint_0;							//波形数据发送指针
		INT16		m_i16SCISendPointCnt_0;						//数据发送点数计数器
		INT16		m_i16SCISendSignalCnt_0;					//数据发送信号计数器
		INT16		m_i16SCISendDelay_0;						//发送间隔延时计数器
		INT16		m_i16SCISaveDelay_0;						//数据保存间隔延时计数器
		INT16		m_i16SumCheck_0;							//检验和
		INT32		m_i32StopDelayCnt_0;
		INT16		m_i16Trop_0;
		union SCIDebugFlag
		{
			UINT16	all;
			struct
			{
				UINT16 Sending			:1;						//1: 数据传送中，0：非数据传送中
				UINT16 HighorLow		:1;						//1：传出数据的高位字节。1：传输数据的低位字节
				UINT16 Saving			:1;						//1：开始存储数据，0：不能存储数据
				UINT16 ConditionSaveEn	:1;						//1:条件使能保存 0：不使能条件保存 1：使能条件保存
			}BIT;
		}unSCIDebugFlag_0;

	public:
		void	App_DebugVarInit(void);
		void	App_Int_WaveRecord(	void );
		void	App_DebugInf(void);

	};

	#pragma	DATA_SECTION(".WaveRecord")
	INT16 iSCIDataBuffer_0[2048];								//数据存储数组	RAMFLASH, 2k*16
#endif

//===========================================================================
// End of file.
//===========================================================================
