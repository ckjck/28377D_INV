/*==========================================================*/ 	
/* Title		:	PowCANScheduler.h									*/ 	
/* Description	: 	Class_PowCANScheduler definition					*/ 	
/* Date			:	Apr.2008								*/ 	
/*==========================================================*/

#ifndef		POWCANSCHEDULER_H 
             
#define     POWCANSCHEDULER_H

//CAN通信相关常数
#define				BroadcastIdCNST		15
#define				CANRxFlag			0x0000003F
#define				VersionNumber		0x0020
#define				MessageID_Version	0x000A
#define				MessageID_High		0x000B
#define				MessageID_Low		0x000C

//By wxw20090714 
//发送调度常数
#define				SEND_DQCURRENT		1
#define				SEND_PLOAD			2
#define				SEND_SLOAD			3
#define				SEND_SYSTEMSTATE	5
#define				SEND_ILFILTER		7
#define				SEND_IINVADQCURRENT	8
//#define				SEND_IINVADQCURRENT	4

#define				SEND_IINVBDQCURRENT	9
#define				SEND_IINVCDQCURRENT	10


//Load离线阀值常数
#define				LOAD_NOT_ONLINE_MAX_CNT	42

//const	UINT16	BroadcastIdCNST = 15;			//广播帧的ID
//const	UINT16 	CANRxFlag = 0x0000003F;	//判断是否收到新的数据
//const	UINT16 	VersionNumber = 0x0020;		//软件版本号

//const	UINT16 	MessageID_Version = 0x000A;		//本版配置信息帧ID
//const	UINT16 	MessageID_High = 0x000B;		//高优先级信息帧ID
//const	UINT16 	MessageID_Low = 0x000C;		//低优先级信息帧ID
const	UINT16	UPSID_BIT_NUM = 6;					//UPS ID占用的ID位数
const	UINT16	MessageType_BIT_NUM = 4;			//信息类型在邋ID中占用的位数

const	UINT16	MessageType_MASK = 0x03C0;		//信息类型在邋ID中的位置
const	UINT16	UPS_NOT_ONLINE_MAX_CNT = 2;		//ups连续4ms不在线时，则认为该ups不在线，用于UPS离线判断
const	UINT16	POW_CAN_PRD_TABL_SIZE = 3;		//power CAN定时处理回调函数的表大小
const	UINT16	UPS_NOT_ONLINE_MAX_CNT_0 = 60;		//ups连续两个周期不在线时，用于通讯故障的判断



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
	
	 //通信周期内msg接收成功标志
	typedef struct
	{
		UINT16 bMACID1		:1; //UPS1 ID   1:表示通信周期内收到该UPS的msg，0:没有收到
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
	
	//各UPS在线标志以及总系统通信故障标志
	typedef struct
	{
		UINT16 bMACID1		:1; //UPS1 ID   1:表示该UPS在线 0:不在线
		UINT16 bMACID2		:1;	//UPS2 ID
		UINT16 bMACID3		:1;	//UPS3 ID
		UINT16 bMACID4		:1;	//UPS4 ID
		UINT16 bMACID5		:1;	//UPS5 ID
		UINT16 bMACID6		:1;	//UPS6 ID
		UINT16 bMACID7		:1;	//UPS7 ID
		UINT16 bRev7To13	:7;	//
		UINT16 bSysCommerr  :1; //系统通信故障
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
		INT16		m_i16IlFiltera;					//单机A相直流 
		INT16		m_i16IlFilterb;					//单机B相直流 
		INT16		m_i16IlFilterc;					//单机C相直流 
	}IL_Filter_DATA_T;

	//power can并机信息访问的唯一入口
	struct Power_CAN_Para_Info_Entry
	{
		UPS_ONLINE_INFO_T UpsOnlineInfo;
		MSG_RECV_FLAG_U MsgRecvFalg;
		DQ_OUT_CURR_T ParaDQCurrEntry[ParaNumCNST];
		IL_Filter_DATA_T IlFilter[ParaNumCNST];
	};
	
	//By wxw20090714 Adaptor15k~20K需通过PowerCan传递负载信息，以下为相关定义量
	typedef struct
	{
		INT16		m_i16Pouta;					//单机A相功率 
		INT16		m_i16Poutb;					//单机B相功率
		INT16		m_i16Poutc;					//单机C相功率

		INT16		m_i16Souta;					//单机A相容量
		INT16		m_i16Soutb;					//单机B相容量
		INT16		m_i16Soutc;					//单机C相容量
	}LOAD_DATA_T;

	typedef struct
	{
		UINT16 bMACID1		:1; //UPS1 ID   1:表示该UPS的负载已发送 0:未发送
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
		UINT16 bInv		:1; //逆变器开启位 1:Inv已启动完 0:未启动完
		UINT16 bOn		:1; //逆变器供电位 1:Inv在供电	0:未供电
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
		DQ_CURRENT_VALID_U		DqCurrentValid;			//总负载标志
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
			
			INT16 m_i16ChgOnTotalNum;			//正Charger开启台数
			INT16 m_i16ChgPBoostTotalNum;		//正Charger均充台数
			INT16 m_i16BattDischgTotalNum;		//电池放电台数
			INT16 m_i16BattPreWarnTotalNum;		//电池放电预告警台数
			INT16 m_i16ChgPBoostTotal;
			INT16 m_i16BattPreWarnTotal;	
			INT16 m_i16ChgOnMinID;
			INT16 m_i16BattSelfTestTotalNum;
			INT16 m_i16BattSelfTestTotal;
			INT16 m_i16PowerCanOnlineNum;						//Power CAN在线台数
			INT16 m_i16PowerCanTotalNum;						//参与并联的所有机架Power CAN在线台数

			volatile struct  Power_CAN_Para_Info_Entry  PowCanParaInfoEntry;
			volatile struct Power_CAN_Para_Load_Entry PowerCANParaLoadEntry;
			volatile struct Battery_Status m_st_BatteryStatus;
			//By wxw20090714 PowerCan传递负载大小不分对外输出三个变量:
			//
			INT16 m_i16PoutTotalMax;
			INT16 m_i16SoutTotalMax;
			INT16 m_i16PoutaTotal;
			INT16 m_i16PoutbTotal;
			INT16 m_i16PoutcTotal;
			INT16 m_i16SoutaTotal;
			INT16 m_i16SoutbTotal;
			INT16 m_i16SoutcTotal;
			//By wxw20090803 供电台数
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
			
			INT16		m_i16PowCanErrCnt;							//上电故障延时3分钟报通讯故障
			INT16		m_i16DCurrSend;									//本机发送的D轴电流
			INT16		m_i16QCurrSend;									//本机发送的Q轴电流	
			INT16		m_i16ZCurrSend;									//本机发送的Q轴电流	
			INT16		m_i16IoutSend;									//本机发送的Q轴电流
			
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
			//Power Can相关函数声明
			void	App_PowCANInit(void); 
			void 	App_Int_PrdTimer_Route(void);
			void	App_Int_Can_Recv(void);
			void		Dat_Int_Can_Send(void);
			void	App_Int_Can_ComErr(void);
			//By wxw20090714 PowerCan传递负载大小需的函数
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

