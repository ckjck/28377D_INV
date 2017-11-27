/*==========================================================*/ 	
/* Title		:	PowCANScheduler.h									*/ 	
/* Description	: 	Class_PowCANScheduler definition					*/ 	
/* Date			:	Apr.2008								*/ 	
/*==========================================================*/

#ifndef		POWCANSCHEDULER_H 
             
#define     POWCANSCHEDULER_H

//CANͨ����س���
#define				BroadcastIdCNST		15
#define				CANRxFlag			0x0000003F
#define				VersionNumber		0x0020
#define				MessageID_Version	0x000A
#define				MessageID_High		0x000B
#define				MessageID_Low		0x000C

//By wxw20090714 
//���͵��ȳ���
#define				SEND_DQCURRENT		1
#define				SEND_PLOAD			2
#define				SEND_SLOAD			3
#define				SEND_SYSTEMSTATE	5
#define				SEND_ILFILTER		7
#define				SEND_IINVADQCURRENT	8
//#define				SEND_IINVADQCURRENT	4

#define				SEND_IINVBDQCURRENT	9
#define				SEND_IINVCDQCURRENT	10


//Load���߷�ֵ����
#define				LOAD_NOT_ONLINE_MAX_CNT	42

//const	UINT16	BroadcastIdCNST = 15;			//�㲥֡��ID
//const	UINT16 	CANRxFlag = 0x0000003F;	//�ж��Ƿ��յ��µ�����
//const	UINT16 	VersionNumber = 0x0020;		//����汾��

//const	UINT16 	MessageID_Version = 0x000A;		//����������Ϣ֡ID
//const	UINT16 	MessageID_High = 0x000B;		//�����ȼ���Ϣ֡ID
//const	UINT16 	MessageID_Low = 0x000C;		//�����ȼ���Ϣ֡ID
const	UINT16	UPSID_BIT_NUM = 6;					//UPS IDռ�õ�IDλ��
const	UINT16	MessageType_BIT_NUM = 4;			//��Ϣ��������ID��ռ�õ�λ��

const	UINT16	MessageType_MASK = 0x03C0;		//��Ϣ��������ID�е�λ��
const	UINT16	UPS_NOT_ONLINE_MAX_CNT = 2;		//ups����4ms������ʱ������Ϊ��ups�����ߣ�����UPS�����ж�
const	UINT16	POW_CAN_PRD_TABL_SIZE = 3;		//power CAN��ʱ����ص������ı��С
const	UINT16	UPS_NOT_ONLINE_MAX_CNT_0 = 60;		//ups�����������ڲ�����ʱ������ͨѶ���ϵ��ж�



	typedef void (Class_PowCANScheduler::*TTs_PrdTmrCbFunc)(void);

	// This structure defines the contents of a periodic timer. The periodic
	// scheduler table consists of one or more of these periodic timer definitions.
	typedef struct
	{
	 	UINT16			m_u16SetTickCnt; // The num of int ticks(Timer int) to expire timer.
	 	UINT16			m_u16TickCntr;   // Interrupt tick counter.
		TTs_PrdTmrCbFunc		pCbFunc;    // A pointer to a table of callback(s).
	} TTs_PrdTmrDef;
 
 	typedef struct 
	{
		INT16 	m_i16IINVAP;
		INT16 	m_i16IINVAQ;
		INT16	m_i16IINVBP;
		INT16	m_i16IINVBQ;
		INT16	m_i16IINVCP;
		INT16	m_i16IINVCQ;

		INT16	m_i16DCurr;
	}DQ_OUT_CURR_T;
	
	 //ͨ��������msg���ճɹ���־
	typedef struct
	{
		UINT16 bMACID1		:1; //UPS1 ID   1:��ʾͨ���������յ���UPS��msg��0:û���յ�
		UINT16 bMACID2		:1;	//UPS2 ID
		UINT16 bMACID3		:1;	//UPS3 ID
		UINT16 bMACID4		:1;	//UPS4 ID
		UINT16 bMACID5		:1;	//UPS5 ID
		UINT16 bMACID6		:1;	//UPS6 ID
		UINT16 bMACID7		:1;	//UPS7 ID
		UINT16 bRev7To15	:9;	//20051117		
	}MSG_RECV_FLAG_T;

	typedef union
	{
		UINT16 			m_u16All;
		MSG_RECV_FLAG_T tBits; 	
	}MSG_RECV_FLAG_U;
	
	//��UPS���߱�־�Լ���ϵͳͨ�Ź��ϱ�־
	typedef struct
	{
		UINT16 bMACID1		:1; //UPS1 ID   1:��ʾ��UPS���� 0:������
		UINT16 bMACID2		:1;	//UPS2 ID
		UINT16 bMACID3		:1;	//UPS3 ID
		UINT16 bMACID4		:1;	//UPS4 ID
		UINT16 bMACID5		:1;	//UPS5 ID
		UINT16 bMACID6		:1;	//UPS6 ID
		UINT16 bMACID7		:1;	//UPS7 ID
		UINT16 bRev7To13	:7;	//
		UINT16 bSysCommerr  :1; //ϵͳͨ�Ź���
		UINT16 bRev15		:1;
		
	}UPS_ONLINE_FLAG_T;

	typedef union
	{
		UINT16			 	m_u16All;
		UPS_ONLINE_FLAG_T 	tBits;
	}UPS_ONLINE_FLAG_U;

	typedef struct
	{
		UPS_ONLINE_FLAG_U uOnlineFlag;
		UINT16			  m_u16NotOnlineCnt[ParaNumCNST];
	}UPS_ONLINE_INFO_T;

	typedef struct
	{
		INT16		m_i16IlFiltera;					//����A��ֱ�� 
		INT16		m_i16IlFilterb;					//����B��ֱ�� 
		INT16		m_i16IlFilterc;					//����C��ֱ�� 
	}IL_Filter_DATA_T;

	//power can������Ϣ���ʵ�Ψһ���
	struct Power_CAN_Para_Info_Entry
	{
		UPS_ONLINE_INFO_T UpsOnlineInfo;
		MSG_RECV_FLAG_U MsgRecvFalg;
		DQ_OUT_CURR_T ParaDQCurrEntry[ParaNumCNST];
		IL_Filter_DATA_T IlFilter[ParaNumCNST];
	};
	
	//By wxw20090714 Adaptor15k~20K��ͨ��PowerCan���ݸ�����Ϣ������Ϊ��ض�����
	typedef struct
	{
		INT16		m_i16Pouta;					//����A�๦�� 
		INT16		m_i16Poutb;					//����B�๦��
		INT16		m_i16Poutc;					//����C�๦��

		INT16		m_i16Souta;					//����A������
		INT16		m_i16Soutb;					//����B������
		INT16		m_i16Soutc;					//����C������
	}LOAD_DATA_T;

	typedef struct
	{
		UINT16 bMACID1		:1; //UPS1 ID   1:��ʾ��UPS�ĸ����ѷ��� 0:δ����
		UINT16 bMACID2		:1;	//UPS2 ID
		UINT16 bMACID3		:1;	//UPS3 ID
		UINT16 bMACID4		:1;	//UPS4 ID
		UINT16 bMACID5		:1;	//UPS5 ID
		UINT16 bMACID6		:1;	//UPS6 ID
		UINT16 bMACID7		:1;	//UPS7 ID
		UINT16 Rev7to15		:9;
	}DQ_CURRENT_VALID_T;

	typedef union
	{
		DQ_CURRENT_VALID_T		tBits;
		UINT16					m_u16All;
	}DQ_CURRENT_VALID_U;

	typedef struct
	{
		UINT16 bInv		:1; //���������λ 1:Inv�������� 0:δ������
		UINT16 bOn		:1; //���������λ 1:Inv�ڹ���	0:δ����
		UINT16 Rev2to15 :14;	
	}INV_FLAG_T;

	typedef union
	{
		INV_FLAG_T				tbits;
		UINT16					m_u16all;
	}INV_FLAG_U;
	
	typedef struct
	{
		UINT16 bChargerWorkP		:1; 
		UINT16 bChargerWorkN		:1;
		UINT16 bChgBostP		:1; 
		UINT16 bBattDischg		:1; 
		UINT16 bBattPreWarn		:1; 
		UINT16 bBattSelfTest		:1; 	
		UINT16 Rev5to15 	:10;	
	}Batt_FLAG_T;

	typedef union
	{
		Batt_FLAG_T				tbits;
		UINT16					m_u16all;
	}Batt_FLAG_U;

	typedef struct
	{
		INV_FLAG_U				InvFlag;
		Batt_FLAG_U				BattFlag;
	}SYS_FLAG_U;
	struct Power_CAN_Para_Load_Entry
	{
		DQ_CURRENT_VALID_U		DqCurrentValid;			//�ܸ��ر�־
		LOAD_DATA_T				ParaLoadData[ParaNumCNST];
		SYS_FLAG_U				SysFlag[ParaNumCNST];
	};

	struct Battery_Status
	{
		UINT16 bChargerWorkP	:1; 
		UINT16 bChargerWorkN	:1;
		UINT16 bChgBostP		:1; 
		UINT16 bBattDischg		:1; 
		UINT16 bBattPreWarn		:1; 
		UINT16 bBattSelfTest	:1; 
		UINT16 Rev5to15 		:10;	
	};

	class Class_PowCANScheduler
	{
		public:
			INT16 m_i16_IoutAvg_0;
			INT16 m_i16_IoutAvgP_0;
			INT16 m_i16_IoutAvgQ_0;
			INT16 m_i16_IlAvgDc_0;
			
			INT16 m_i16ChgOnTotalNum;			//��Charger����̨��
			INT16 m_i16ChgPBoostTotalNum;		//��Charger����̨��
			INT16 m_i16BattDischgTotalNum;		//��طŵ�̨��
			INT16 m_i16BattPreWarnTotalNum;		//��طŵ�Ԥ�澯̨��
			INT16 m_i16ChgPBoostTotal;
			INT16 m_i16BattPreWarnTotal;	
			INT16 m_i16ChgOnMinID;
			INT16 m_i16BattSelfTestTotalNum;
			INT16 m_i16BattSelfTestTotal;
			INT16 m_i16PowerCanOnlineNum;						//Power CAN����̨��
			INT16 m_i16PowerCanTotalNum;						//���벢�������л���Power CAN����̨��

			volatile struct  Power_CAN_Para_Info_Entry  PowCanParaInfoEntry;
			volatile struct Power_CAN_Para_Load_Entry PowerCANParaLoadEntry;
			volatile struct Battery_Status m_st_BatteryStatus;
			//By wxw20090714 PowerCan���ݸ��ش�С���ֶ��������������:
			//
			INT16 m_i16PoutTotalMax;
			INT16 m_i16SoutTotalMax;
			INT16 m_i16PoutaTotal;
			INT16 m_i16PoutbTotal;
			INT16 m_i16PoutcTotal;
			INT16 m_i16SoutaTotal;
			INT16 m_i16SoutbTotal;
			INT16 m_i16SoutcTotal;
			//By wxw20090803 ����̨��
			INT16	m_i16InvTotalNum;
			INT16	m_i16OnTotalNum;
			INT16		m_i16PowerCanIaPM_0[6];
			INT16		m_i16PowerCanIaQM_0[6];
			INT16		m_i16PowerCanIbPM_0[6];
			INT16		m_i16PowerCanIbQM_0[6];
			INT16		m_i16PowerCanIcPM_0[6];
			INT16		m_i16PowerCanIcQM_0[6];

			INT16		m_i16PowerCanDcurr_0[6];


		private:						
			
			volatile	struct	ECAN_REGS	*pECanReg;
			volatile 	struct 	ECAN_MBOXES *pECanMboxes;
			volatile	struct	ECAN_REGS	tempECanReg;
			
			volatile	struct  MBOX		tempMbox;
			TTs_PrdTmrDef	 PrdSchdlrTbl[POW_CAN_PRD_TABL_SIZE];
			
			INT16		m_i16PowCanErrCnt;							//�ϵ������ʱ3���ӱ�ͨѶ����
			INT16		m_i16DCurrSend;									//�������͵�D�����
			INT16		m_i16QCurrSend;									//�������͵�Q�����	
			INT16		m_i16ZCurrSend;									//�������͵�Q�����	
			INT16		m_i16IoutSend;									//�������͵�Q�����
			
			INT16		m_i16PoutaSend;
			INT16		m_i16PoutbSend;
			INT16		m_i16PoutcSend;
			INT16		m_i16SoutaSend;
			INT16		m_i16SoutbSend;
			INT16		m_i16SoutcSend;

			UINT16		i16ScheduleID;
			UINT16		i16SendScheduleCnt;
			UINT16		m_i16InvState;
			UINT16		m_i16BattState;

			INT16		m_i16IabcShedule;

			INT16		m_i16InvaPSend;
			INT16		m_i16InvaQSend;
			INT16		m_i16InvbPSend;
			INT16		m_i16InvbQSend;
			INT16		m_i16InvcPSend;
			INT16		m_i16InvcQSend;
			
		public:
			//Power Can��غ�������
			void	App_PowCANInit(void); 
			void 	App_Int_PrdTimer_Route(void);
			void	App_Int_Can_Recv(void);
			void		Dat_Int_Can_Send(void);
			void	App_Int_Can_ComErr(void);
			//By wxw20090714 PowerCan���ݸ��ش�С��ĺ���
			UINT16 App_Int_Can_SendSchedule(void);
			void	Dat_Can_LoadCalculation(void);
			void 	App_Can_ILFilter(void);
			void	App_Can_IInvDQCal(void);
			void	Alg_Int_MainInterfaceVar(void);
		friend class	Class_PowCANProtocol;
		friend class	Class_Debug;
	};
	
#endif

//===========================================================================
// End of file.
//===========================================================================

