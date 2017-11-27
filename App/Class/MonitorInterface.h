/*==========================================================*/
/* Title		:	MonitorInterface.h												*/
/* Description	: 	Class_MonitorInterface definition									*/
/* Date		:	Apr.2008														*/
/*==========================================================*/
#ifndef      MONITORINTERFACE_H

#define      MONITORINTERFACE_H
//Head files
//#include "SysCanApp.h"
//版本信息常数，以下常数表示软件版本为V100B000D000

#define	ProtocolVerVCnst		100
#define	ProtocolVerBCnst		0x1
#define	ProtocolVerDCnst		0x0

#define	DspVerVCnst				100
#define	DspVerBCnst				0
#define	DspVerDCnst				2

#define	CpldVerVCnst			0x0
#define	CpldVerBCnst			0x6
#define	CpldVerDCnst			0x0

#define Voltage80CNST	373				//373=1024*80/2
#define Current5CNST	51				//51=1024*5/100
#define Sout5CNST		51				//51=1024*5/100


struct structMonitorOrder
	{
		UINT16	bOn							:1;			//BIT00		Inverter On  0:无 1:有
		UINT16	bOff						:1;			//BIT01		Inverter Off 0:无 1:有
		UINT16	bEPO						:1;			//BIT02		EPO          0:无 1:有
		UINT16	bRev03						:1;			//BIT03		(BpShut预留）
		UINT16	bReset						:1;			//BIT04		Fault Reset  0无
		UINT16	bIntSwitchConf			:1;			//BIT05		间断切换确认 0:无 1:有
		UINT16	bRiskOffConf				:2;			//BIT07-06	风险关机确认 0:无1:确认2:取消
		UINT16	bRev1108					:4;			//BIT11-08
		UINT16	bInstantOff					:1;			//BIT12		立即关机,如电池关机点 0:无 1:有
		UINT16	bOutDisable					:1;			//BIT13		输出禁止命令   0:无 1:有
		UINT16	bOutEnable					:1;			//BIT14		输出允许命令   0:无 1:有
		UINT16	bFlashUpdate				:1;			//BIT15		FlashUpdate命令0:无 1:有
	};
	struct 	structUPSModeRun
	{
 		UINT16	bConfig						:2;			//BIT01-00	UPS组态
																	//00 单机
																	//01 热备份从机
																	//10 热备份主机
																	//11 并机
		UINT16	bMode						:1;			//BIT02		UPS模式 	0: 在线 1: ECO 	(设置)
		UINT16	bID208						:1;			//BIT03	   	208V体系标识0:400体系 1:208体系
		UINT16	bCabinet					:2;			//BIT05－04 旁路柜形式  0: 无1: 维修2: 自动
		UINT16	bPowerFactor				:1;			//BIT06	  	输出功率因数选择 0:0.9 1:1
		UINT16	bLBS						:2;			//BIT08－07  LBS设置 0:LBS禁止1:LBS从机2:LBS主机
		UINT16	bFeedback					:1;			//BIT9		 BACK_FEED选件设置 0:无旁路CIRCUIT BREAKER 1:有旁路CIRCUIT BREAKER
		UINT16	bIntSwMode					:1;			//BIT10		 间断切换原则 0:最小间断时间1:最小负载直流饱和
		UINT16	bFreqOut					:1;			//BIT11  	 输出频率  0:50HZ 1:60HZ
		UINT16	bVoltOut					:3;			//BIT14－12  电压体制
														//000 400V  体制
														//001 380V  体制
														//010 415V  体制
														//011 208V  体制
														//100 200V  体制
														//101	480V  体制
		UINT16	bMasterECO					:1;			//BIT15		热备份主机当前模式 0: 在线 1: ECO
	};

	struct 	structUPSModeRun2
	{
 		UINT16	bInvRunMode					:2;			//BIT01	逆变器工作模式设置0:正常 1:自老化 2:调测
		UINT16	bBattShare					:1;			//共用电池组1:共用；0:不共用
		UINT16	bRev1501					:13;		//BIT15－03
	};
//-------------------------------------------------------------------------------

/*****************************************************************************
名称：逆变所有上传量
类别：状态，告警，模拟量，版本信息
********************************************************************************/
	typedef struct
	{

		_INV_SW1_T 				m_st_InvBreakerStatus;					//逆变器上传状态量
		_INV_WARN1_T			m_st_InvWarn;							//逆变上传告警量
		_INV_ANALOG_T			m_st_InvAnalog;							//逆变上传模拟量
		_MODULE_BASE_INFO_T    	m_st_ModuleVerInfo;						//模块版本信息

	}_INV_SEND_MONITOR_T;

/*****************************************************************************
名称：旁路所有上传量
类别：状态，告警，模拟量，版本信息
********************************************************************************/
	typedef struct
	{

		_BYP_SW1_T 				m_st_BypBreakerStatus;					//旁路器上传状态量
		_BYP_WARN1_T			m_st_BypWarn;							//旁路上传告警量
		_BYP_ANALOG_T			m_st_BypAnalog;							//旁路上传模拟量
		//_MODULE_BASE_INFO_T    		m_st_BypVerInfo;						//模块版本信息

	}_BYP_SEND_MONITOR_T;

//---------------------------------------

//机架内并机数据交互集合
	typedef struct
	{
		_PARALLEL_SW1_T  		ParallelState[ParaNumCNST];

		INT16		iNumInv;					//本机架内并机系统逆变开机总台数
		INT16		iNumOn;						//本机架内并机系统逆变供电总台数
		INT16		iNumBp;						//本机架内并机系统旁路供电台数
		INT16		iNumUPS;					//本机架内并机系统实际上电总台数

		INT16		iTotalNumInv;                          //整个机架间并机系统所有逆变开机总台数
		INT16		iTotalNumOn;                           //整个机架间并机系统所有供电开机总台数


		//以下是通信本机传给监控的数据
		INT16		iNumBattWarn;				//并机系统电池容量告警总台数
		INT16		iNumBattInv;				//并机系统电池逆变供电总台数
		INT16		iParaWarnFlag;				//并机系统告警故障综合标志
											//bit0:	1-并机系统有严重故障
											//bit1:	1-并机系统有一般故障
											//bit2:	1-并机系统有严重告警
											//bit3:	1-并机系统有一般告警

		INT16		iParaRunFlag;				//并机系统UPS上电标志，单机或从机上传0xFF
											//bit0:	Reserved
											//bit1:	1-1#机开机
											//bit2:	1-2#机开机
											//bit3:	1-3#机开机
											//bit4:	1-4#机开机
											//bit5:	1-5#机开机
											//bit6:	1-6#机开机
											//bit7-15:	Reserved

	}_PARALLEL_EXCHG_DATA_T;

/*****************************************************************************
名称：监控设置量
类别：
********************************************************************************/
	typedef struct
	{
		_SYS_DSP_SET_INFO_T			m_st_SysDSPSettings;					//系统设置量
		_INV_SET_INFO_T				m_st_InvSetInfo;						//监控校正设置
		_CTRL_INFO_T				m_st_SysCtrCmd;							//监控命令 设置
		_INV_RECV_ANALOG_T			m_st_InvRecvAnalog;
	}_INV_RECEIVE_MONITOR_T;


	//模块ID错误标志位
	 typedef struct
	{
		//检测到的ID号超范围,DSP逻辑置,CAN使用
		UINT16	bUsIdNumExpire 		:1;
		//监测到总线上有重复的ID,CAN置位,整流逻辑要使用
		UINT16	bUsIdNumOverlap		:1;
		UINT16 	bReserved				:14;

	}_ID_ERROR_TYPE_T;
//-----------------------------------------------------------------------------


	class Class_MonitorInterface
	{
	public:

		volatile struct structMonitorOrder		m_st_wMonitorOrder;				//监控命令
		volatile struct structUPSModeRun		m_st_wUPSModeRun;				//UPS运行模式
		volatile struct structUPSModeRun2		m_st_wUPSModeRun2;				//UPS运行模式

		_INV_SEND_MONITOR_T			m_st_InvSendMonitor; 			//逆变上传监控信息
		_BYP_SEND_MONITOR_T			m_st_BypSendMonitor;			//旁路上传监控信息
		_INV_RECEIVE_MONITOR_T			m_st_wInvReceiveMonitor;		//逆变接受监控信息
//		_MODULE_BASE_INFO_T 			m_stCfgInfoCheck_D;		//基本配置信息查询

		_REC_DISCRETE_SW1_T 			m_st_DiscreteState[ParaNumCNST];
		//Parallel state data
		_PARALLEL_EXCHG_DATA_T			m_st_wParallelData;		//机架内并机间传递数据

		//Byp maint state data
		_BYPMAINT_SW1_T				m_st_wBypBreakData;	//旁路传过来的状态信息

		//Rack parallel sw msg
		_RACK_RECV_STATE_T				m_st_wRackStateMsg;

		_RT_CLK_T						 m_SyncTime;				//监控同步的时间，DSP维护ms值

		_ID_ERROR_TYPE_T				m_st_IdErrorFlag; 		//模块ID错误标志位

		_MON_REAL_MSG_T				m_st_MonRealMsg;			//来自监控的信息

		INT16 		m_i16BptoinvTimesSet;										//过载旁路切逆变次数设置/可通讯设置
		INT16		m_i16IntSwTimeSet;											//输出间断切换时间设置
																				//0:最小切换时间 1:40ms2:60ms 3:80ms 4:100ms
		INT16 		m_i16FbpTraceSet;											//旁路频率跟踪范围设置	/可通讯设置
						                                  						//  0: +-2Hz1: +-1Hz  2: +-3Hz 3:+-0.5Hz
		INT16 		m_i16VbpUplimitSet;											//旁路保护范围上限设置/可通讯设置   0:+10%  1:+15%
		INT16 		m_i16VbpDnlimitSet;											//旁路保护范围下限设置/可通讯设置
			                                   			            			//   0:-20%  1:-10%   2: -30%  3: -40%
		INT16 		m_i16FbpLimitSet;											//旁路频率保护范围设置/可通讯设置
		INT16		m_i16VoltDispScale;						        					                      //0: +-10%  1: +-20%   2: +-5%  3: +-2.5%

		INT16		m_i16FreqSlewRate;											//跟踪速率：0.1-3Hz/s ===>1-30
		INT16 		m_i16VrefSetRms;											//逆变额定有效值(用户)

//---------------------------------------------------------
		UINT16 		m_u16ParamFresh;											//校正参数更新标志，按位(0-14)对应下列校正参数
		INT16		m_i16KsVoutaSet;
		INT16		m_i16KsVoutbSet;
		INT16		m_i16KsVoutcSet;
		INT16		m_i16KsIoutaSet;
		INT16		m_i16KsIoutbSet;
		INT16		m_i16KsIoutcSet;
		INT16		m_i16KsVinvaSet;
		INT16		m_i16KsVinvbSet;
		INT16		m_i16KsVinvcSet;
		INT16		m_i16KsIinvaSet;
		INT16		m_i16KsIinvbSet;
		INT16		m_i16KsIinvcSet;

		INT16		m_i16KsVbpaSet;
		INT16		m_i16KsVbpbSet;
		INT16		m_i16KsVbpcSet;
//----------------------------------------------------------------
		INT16		m_i16KsVdcpSet;
		INT16		m_i16KsVdcnSet;

		INT16		m_i16SelfAgingCurrRate;										//自老化功能相关变量--回馈电流比例设置
		INT16		m_i16wSerialNumSet_0;										//机器编号,由通讯设置
		INT16		m_i16BasicNum;												//并机基本台数
		INT16		m_i16RedunNum;												//并机冗余台数
		INT16		m_i16AlterPeriodSet;										//热备份主机：主从供电交替时间
		INT16		m_i16ModelKVA;												//机型容量
		INT16		m_i16GrantKVA;												//授权容量
		//INT16		m_i16ModelKW;

		UINT16		m_u16TimeLabel1_0;
		UINT16		m_u16TimeLabel2_0;
		UINT16		m_u16TimeLabel3_0;
																				//来自监控的同步时间


		INT16		m_i16TimeSynCommand_0;										//时间同步命令
		UINT16		m_u16HistoryRecord[256];									//历史记录缓冲区
		UINT16 		ModuleNum;			//逆变软件中包含的软件数量 150K仅逆变 20K为逆变+旁路

		/*调试参数*/
		UINT16		m_i16_DegVar1;//2017.06.10
		UINT16		m_i16_DegVar2;
		UINT16		m_i16_DegVar3;

		//控制容量自检命令有效
		UINT16		m_u16BatDeepSelfRecved;

	private:
		UINT16		*pDSPVar1Adr;												//诊断变量的地址1指针
		UINT16		*pDSPVar2Adr;												//诊断变量的地址2指针
		UINT16		*pDSPVar3Adr;												//诊断变量的地址3指针

		INT16		m_i16SupplyStatusCnt;
		INT16		m_i16SystemInvOnCnt;
		INT16		m_i16OutputDelayCntPerMloop;
		INT16		m_i16ParaSignalExitMaskDelayCntPerMloop;
		INT16		m_i16FaultResetCntPerMloop;
		INT16		m_i16PromptCntPer40ms;										//提示确认延时

	public:
		void		App_MonitorVarInit(void);
		void		App_MonitorDeal(void);
		void		App_Int_TimeLabel(void);

	private:
		void		App_MonInterfaceWrite(void);
		void		Byp_App_MonInterfaceWrite(void);
		void		App_MonitorOrder(void);
		void		App_SendtoMonitor(void);
		void		App_MonitorParam(void);

		void        App_MonInterfaceRead(void);
		VOID 		App_ParallelLocalData(VOID );

	};

	#pragma DATA_SECTION(".FaultRecord")
	UINT16	u16FaultRecordRead[128];			//故障记录数据读缓冲区
#endif

//===========================================================================
// End of file.
//===========================================================================
