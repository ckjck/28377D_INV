/***************************************************************************************
* 
* Filename: 		SysCanApp.h
* Contributor:
* Author: 		Wang Zhihua	
* Date: 			3/17/2008
* Copyright 		2008 ENPC Corporation, All Rights Reserved
*
* Description: 	
* 
* 
* Version
* Last rev by:		Wang Zhihua
* Last rev date:	3/17/2008
* Last rev desc:	
****************************************************************************************/
#ifndef SYS_CAN_DATA_EXCHG_H
#define SYS_CAN_DATA_EXCHG_H

//---------------------------------------------------------------------------
//Head files
//#include "Define.h"
#include "SysCanApp.h"
//#include "SysCanDefine.h"

#define XMIT_INFO_MAX_LEN 100

//COV消息状态
enum COV_MSG_ST
{
	NON_COV_MSG = 0,
	HAVE_COV_MSG = 1
};

//批量信息请求设置状态
enum REQ_SET_MSG_ST
{
	NON_REQ_MSG = 0,
	HAVE_REQ_MSG = 1
};

enum SYNC_MSG_ST
{
	NON_SYNC_MSG = 0,
	HAVE_SYNC_MSG = 1
};

enum FAULT_MSG_ST
{
	NON_FAULT_MSG = 0,
	HAVE_FAULT_MSG = 1
};

enum CFG_MSG_ST
{
	NON_CFG_MSG = 0,
	HAVE_CFG_MSG = 1
};

enum COV_MSG_XMIT_ST
{
	COV_XMIT_FAIL = 0,
	COV_XMIT_OK = 1
};

enum FRAG_MSG_XMIT_RESULT
{
	FRAG_XMIT_FAIL = 0,
	FRAG_XMIT_OK = 1
};

enum MSG_XMIT_DIRECTION_ST
{
	RECV_DIRECTION = 0,
	XMIT_DIRECTION = 1
};

typedef struct
{
	UINT16	u16ActiveCnt;
	UINT16	i16XmitIdx;
}_TIMER_CNT_T;

typedef struct
{
	//本模块功率模拟量定时发送计数器,目前定义为500ms
	//此功率(定标值)由逆变传给本模块的整流,包括有功率和视在功率
	_TIMER_CNT_T u16PeriodPwrCnt;

	//如果需要增加类似量可在下面定义
	//环境温度定时发送计数器,目前定义为200ms
//	_TIMER_CNT_T u16PeriodEnviTempCnt;
	//下面根据需要可扩充TBD
	
}_PERIOD_TIME_CNT_T;

typedef struct
{
	INT16 i16XmitActive		:8;	//同步信息发送使能标志
	INT16 i16XmitIdx			:8;	//当前发送信息service code
}SYNC_MSG_XMIT_T;

typedef struct
{
	UINT16 bSrvcCod	:7;	
	UINT16 bCovFlag	:1;
	UINT16 bMsgLen	:4;
	UINT16 			:4;
	UINT16 *pData;
}COV_MSG_OBJ_T;


typedef struct
{
	COV_MSG_OBJ_T CovObj[2];
	SYNC_MSG_XMIT_T SyncInfo;
	_INV_SW1_T Msg1; 
	_BYP_SW1_T Msg2;
}STATE_COV_SYNC_OBJ_T;



typedef struct
{
	COV_MSG_OBJ_T CovObj[2];
	SYNC_MSG_XMIT_T SyncInfo;
	_INV_WARN1_T Msg1; 
	_BYP_WARN1_T Msg2;
}ALRM_COV_SYNC_OBJ_T;

//
typedef struct
{
	COV_MSG_OBJ_T CovObj[PARALLEL_STATE_MSG_OBJ_SIZE];
	SYNC_MSG_XMIT_T SyncInfo;
	_PARALLEL_SW1_T Msg1; 
}PARALLEL_STATE_COV_SYNC_OBJ_T;


typedef struct
{
	UINT16 bSrvcCod	:7;
	UINT16 bMsgLen	:4;
	UINT16 bDirection	:1;	// 1: Xmit; 0:receive 
	UINT16 				:4;
	UINT16 *pData;
	//_REC_ANALOG_T DataBuff;
}ANALOG_MSG_OBJ_T;


typedef struct
{
	ANALOG_MSG_OBJ_T MsgObj[INV_ANALOG_MSG_OBJ_SIZE];
	_INV_ANALOG_T DataBuff;
	SYNC_MSG_XMIT_T SyncInfo;
}ANALOG_SYNC_OBJ_T;

typedef struct
{
	ANALOG_MSG_OBJ_T MsgObj[BYP_ANALOG_MSG_OBJ_SIZE];
	_BYP_ANALOG_T DataBuff;
	SYNC_MSG_XMIT_T SyncInfo;
}BYP_ANALOG_SYNC_OBJ_T;


typedef struct
{
	ANALOG_MSG_OBJ_T MsgObj[INV_ANALOG_RECV_MSG_SIZE];
	_INV_RECV_ANALOG_T DataBuff;
}ANALOG_RECV_OBJ_T;

typedef struct
{
	UINT16 bSrvcCod	:7;
	UINT16 bMsgLen	:7;
	UINT16 bFragMsg	:1;
	UINT16 bCompleted	:1;
	UINT16 bMsgDirect	:1;
	UINT16 bUpdate		:1;
	UINT16 *pData;
}CFG_MSG_T;

//一条事件格式
typedef struct
{
//	UINT32 u32Second;			//事件发生时刻的秒数, Unix纪元
	UINT16 u16Second;			//秒，最大18个小时
	UINT16 u16Millisecond;		//事件发生时刻的毫秒数
	UINT16 u16ExEventID;		//Event Log extended ID 
								// bit15 event status : 0---over; 1---start
								//bit14~0 event ID

}EVENT_LOG_INFO_T;

typedef struct
{
	//事件COV标志
	UINT16 bCovFlag:1;
	//当前事件有效
	UINT16 bXmitActive:1;
	UINT16 			:14;
	
	UINT16 *pData;
}COV_EVENT_OBJ_T;


typedef struct
{
	UINT16 bFromMon 	:8;
	UINT16 bToMon		:8;
}FIRST_CFG_FLAG_T;

typedef  struct
{
	CFG_MSG_T FromMon[INV_CFG_FROM_MON_OBJ_SIZE];
	CFG_MSG_T ToMon[INV_CFG_TO_MON_OBJ_SIZE];
	FIRST_CFG_FLAG_T FirstCfgFlag;
}CFG_INFO_T;

typedef struct
{
	UINT16 uEventID;
	UINT16 bEventInWord: 	2;
	UINT16 bOffSet:			4;
	UINT16 		  :			10;
	UINT16 uMaskVal;
	UINT16 uSetVal;
}EVENT_MSG_T;


CONST EVENT_MSG_T StateEventIdObj[]=
{
	//--1.输出空开状态，0－断开，1－闭合RM不考虑输入空开状态
//	{180,0,0,1,0},

	//--2.输出空开状态，0－断开，1－闭合RM不考虑输入空开状态
	//闭合ID=181
//	{181,0,0,1,1},
	//--3.维修旁路空开状态，0－断开，1－闭合
	//--.断开ID=171
	{171,0,1,1,0},
	//--4.闭合ID=172
	{172,0,1,1,0},
	
	//--3.旁路空开状态
	//断开ID=178
//	{178,0,2,1,0},

	//闭合ID=179
//	{179,0,2,1,1},
	
	//--5.旋转空开状态RM不支持3位
	//--=0关闭
//	{173,0,3,7,0},
	//--=1测试状态
//	{174,0,3,7,1},
	//--=2正常
//	{175,0,3,7,2},
	//--=3旁路
//	{176,0,3,7,3},
	//--=4维修
//	{177,0,3,7,4},

	//--6.逆变供电状态
	//-=0 均不供电,=1旁路供电,=2逆变供电,=3输出禁止

	//EPO
//	{153,1,4,1,1},

	//-----------------------------------
	//Fault clear
//	{158,2,0,0,0},
	//INV Seting
	{196,2,0,0,0}
};

CONST EVENT_MSG_T AlarmEventIdObj[]=
{
	//--告警字1
	//--1.逆变器不同步
	{116,0,0,1,1},
	//--2.逆变器故障
	{117,0,1,1,1},
	//--3.逆变器过温
//	{119,0,2,1,1},
	//--4.单机输出过载
	{127,0,3,1,1},
	//--5.单机过载超时
	{129,0,4,1,1},
	//--6.交流输出电压故障
//	{131,0,5,1,1},
	//--7.输出电压异常
	{216,0,6,1,1},
	//--8.输出直流分量过大
//	{143,0,7,1,1},
	//--9.逆变器过流
//	{132,0,8,1,1},
	//--10.母线异常关机
	{137,0,9,1,1},
	//--11.直流母线过压
	{140,0,10,1,1},
	//--12.逆变电感过温
//	{118,0,11,1,1},
	//--13.风扇故障,与整流风扇故障综合到监控侧告警量,此处不产生
	{120,0,12,1,1},
	//--14.逆变晶闸管故障
	{121,0,13,1,1},
	//--15.输出熔丝断
	{125,0,14,1,1},
	//--16.辅助电源2掉电
	{126,0,15,1,1},

	//告警字2
	//--23.用户操作错误
	{124,1,0,1,1},
	//LBS激活
	{144,1,1,3,1},
	//LBS异常
	{208,1,1,3,2},	

	//告警字3
	//--16.旁路情况－超跟踪
//	{114,2,0,3,2},
	//--17.旁路情况－超保护
//	{115,2,0,3,1},
	//--18.旁路相序反
//	{113,2,2,1,1},
	//--19.负载冲击转旁路
	{134,2,3,1,1},
	//--20.旁路切换次数限制
	{135,2,4,1,1},
	//--21.旁路过流
//	{142,2,5,1,1},
	//--22.旁路晶闸管故障/1.反灌/2.开路/3.短路
	//--逆变只关注短路
	{122,2,6,3,3},
	//--旁路异常关机
	{130,2,8,1,1},
	//--并机板故障
	{139,2,9,1,1},
	//--并机均流故障
	{136,2,10,1,1},
	//--并机线连接故障
	{141,2,11,1,1},
	//--并机通讯故障
	{88,2,12,1,1},
	//--并机系统过载
	{128,2,13,1,1},
	//--临机请求转旁路
	{138,2,14,1,1}
	
};



CONST EVENT_MSG_T BypStateEventIdObj[]=
{
	//BCB 闭合=1
	{187,0,2,3,1},
	//BCB断开=2
	{188,0,2,3,2},

	//--EPO--
	{153,0,1,1,1},

	//--维修旁路空开状态，0－断开，1－闭合
	//--.断开ID=171
	{171,0,7,1,0},
	//--闭合ID=172
	{172,0,7,1,1},

	//CLEAR--Fault clear 
	{158,2,0,0,0},

	//SET--Fault clear 
	{158,2,0,0,0}
};

#define BYP_STATE_EVENT_OBJ_SIZE ((sizeof(BypStateEventIdObj))/(sizeof(EVENT_MSG_T)))

CONST EVENT_MSG_T BypAlarmEventIdObj[]=
{
	//-1.旁路过流故障
	{142,0,0,1,1},
	//-2.旁路晶闸管故障
	//=1反灌
	{122,0,1,3,1},
	//=3短路
	{122,0,1,3,3},
	//-3.风扇故障,由监控侧根据整流/逆变/旁路综合产生,此处不上报
	{120,0,3,1,1},
	//-4.电池接地故障,由监控侧根据整流是否上报来综合
	{150,0,4,1,1},
	//-5.电池房环境异常
	{201,0,5,1,1},
	//-6.旁路模块过温,目前没有此ID,设为400
	{400,0,6,1,1},
	//-辅助电源1掉电,是否应该由监控来综合
	{110,0,7,1,1},
	//-辅助电源2掉电
	{126,0,8,1,1},
	//-旁路情况,旁路超跟踪
	{114,0,9,3,1},
	//-旁路情况,旁路超保护
	{115,0,9,3,3},
	//-旁路相序反
	{133,0,11,1,1},
	//单一机架系统输出过载
	{128,0,12,1,1},
	//机架间并机通讯故障
	{87,0,13,1,1},
	//邻机晶闸管故障
	{320,0,14,1,1}
};
#define BYP_ALARM_EVENT_OBJ_SIZE ((sizeof(BypAlarmEventIdObj))/(sizeof(EVENT_MSG_T)))


#define INV_STATE_EVENT_OBJ_SIZE (sizeof(StateEventIdObj)/sizeof(EVENT_MSG_T))
typedef struct
{
	COV_EVENT_OBJ_T CovObj[INV_STATE_EVENT_OBJ_SIZE];
	EVENT_LOG_INFO_T EventLog[INV_STATE_EVENT_OBJ_SIZE];	
}STATE_EVENT_LOG_OBJ_T;

typedef struct
{
	COV_EVENT_OBJ_T CovObj[BYP_STATE_EVENT_OBJ_SIZE];
	EVENT_LOG_INFO_T EventLog[BYP_STATE_EVENT_OBJ_SIZE];	
}BYP_STATE_EVENT_LOG_OBJ_T;

#define INV_ALARM_EVENT_OBJ_SIZE (sizeof(AlarmEventIdObj)/sizeof(EVENT_MSG_T))
typedef struct
{
	COV_EVENT_OBJ_T CovObj[INV_ALARM_EVENT_OBJ_SIZE];
	EVENT_LOG_INFO_T EventLog[INV_ALARM_EVENT_OBJ_SIZE];
	
}ALARM_EVENT_LOG_OBJ_T;	

typedef struct
{
	COV_EVENT_OBJ_T CovObj[BYP_ALARM_EVENT_OBJ_SIZE];
	EVENT_LOG_INFO_T EventLog[BYP_ALARM_EVENT_OBJ_SIZE];
	
}BYP_ALARM_EVENT_LOG_OBJ_T;


class SysCanDataExchg;
//EXTERN UINT16 uInvAnalogLen[INV_ANALOG_MSG_OBJ_SIZE];

//同步帧监听
typedef struct
{
	//是否继续监听标志=1不再监听
	UINT16 usMonitorNodeIdFlag;
	//监听计数器
	UINT16 usMonitorCnt ;
}_SYNC_FRAME_MONITOR_T;



//-------------------------------------------------------------------------------
//控制信息

typedef VOID (SysCanDataExchg::*CTRL_MSG_RECV_T)(VOID);

//控制信息相关处理结构体
typedef struct
{
	UINT16 bSrvcCode:	7;
	UINT16 bLength	:	4;
	UINT16 			:	5;
	CTRL_MSG_RECV_T MsgRecvFcb;
}CAN_CTRL_MSG_T;

//--20->23增加自检相关命令
#define CTRL_MSG_OBJ_SIZE 23//--20


typedef VOID(SysCanDataExchg::*EVENT_LOG_FCB)(INT16 i16Arg);


class SysCanDataExchg
{
public:
//	SysCanDataExchg(VOID);
//	~SysCanDataExchg(VOID);

	//初始化数据
	VOID SysCanDataExchgIni(VOID);
	VOID InterfaceDataProc(VOID);

	//获取数据交换对象指针
	LOCAL SysCanDataExchg *GetDataExchgPtr(VOID);

	//TBD
	//COV函数
	INT16 GetStateCovMsg(OUT CAN_APDU_T *pApdu,UCHAR modul);
	VOID StateCovMsgXmitState(IN INT16 i16XmitState);
	VOID StateXmitMsgGen(UINT16 u16Idx);
	INT16 GetAlrmCovMsg(OUT CAN_APDU_T *pApdu,UCHAR modul);
	VOID AlrmCovMsgXmitState(IN INT16 i16XmitState);
	VOID AlrmXmitMsgGen(UINT16 u16Idx);
	INT16 GetEventLogMsg(OUT CAN_APDU_T *pApdu,UCHAR modul);
	INT16 ReqSetMsg(OUT CAN_APDU_T *pApdu);

	VOID AlarmEventXmitMsgGen(UINT16 EventIndex,UCHAR modul);
	VOID StateEventXmitMsgGen(UINT16 EventIndex,UCHAR modul);

	INT16 GetParallelStateMsg(OUT CAN_APDU_T *pApdu);
	VOID ParallelStateXmitMsgGen(UINT16 u16Idx);
	

	//Sync信息函数
	INT16 GetStateSyncMsg(OUT CAN_APDU_T *pApdu,UCHAR modul);
	VOID StateSyncMsgXmitState(IN INT16 i16XmitState);
	INT16 GetAlrmSyncMsg(OUT CAN_APDU_T *pApdu,UCHAR modul);
	VOID AlrmSyncMsgXmitState(IN INT16 i16XmitState);
	INT16 GetAnalogSyncMsg(OUT CAN_APDU_T *pApdu,UCHAR modul);
	VOID AnalogSyncMsgXmitState(IN INT16 i16XmiyState);

	INT16 PeriodAnalogSyncMsg(OUT CAN_APDU_T *pApdu);

	VOID MonitorSyncMsgProc( VOID);
	

	//Cfg msg
	INT16 CfgMsgRecvProc(IN CAN_APDU_T *pApdu);
	INT16 StateMsgRecvProc(IN CAN_APDU_T *pApdu);
	INT16 SyncMsgProc(IN CAN_APDU_T *pApdu);
	INT16 CfgMsgFromMonProc(IN CAN_APDU_T *pApdu, IN UINT16 u16Idx);
	INT16 CfgMsgToMonProc(IN CAN_APDU_T *pApdu, IN UINT16 u16Idx);
	INT16 SyncTimeMsgProc( IN CAN_APDU_T *pApdu);

	//ALARM msg
	INT16 AlrmMsgRecvProc(IN CAN_APDU_T *pApdu);

	//Ctrl msg
	INT16 CtrlMsgRecvProc(IN CAN_APDU_T *pApdu);
	VOID PostBattConvFltChg(VOID);
	VOID NegBattConvFltChg(VOID);
	VOID PostBattConvEqualChg(VOID);
	VOID NegBattConvEqualChg(VOID);
	VOID BattSelfChk20Percent(VOID);
	VOID BattSelfChkDeep(VOID);
	VOID BattSelfChkEnd(VOID);
	VOID BattEodTurnOff(VOID);
	VOID BattTrip(VOID);
	VOID OutputEnableCmd(VOID);
	VOID OutputDisableCmd(VOID);
	VOID BattGndPtcCmd(VOID);
	
	VOID FaultClrCmd(VOID);
	VOID MaualStartupCmd(VOID);
	VOID MaualShutCmd(VOID);
	VOID EpoCmd(VOID);
	VOID AbnormalShutConfirmCmd(VOID);
	VOID AbnormalShutCancelCmd(VOID);
	VOID IndirectSwitchCmd(VOID);

	VOID RecFlashUpdate(VOID);
	VOID InvFlashUpdate(VOID);
	VOID BypFlashUpdate(VOID);
	
	VOID BattPreWarn(VOID);
	VOID BattPreWarnCancel(VOID);

	//FRAG process
	INT16 FragXmitResult(CAN_APDU_T *pApdu, INT16 i16Result);

	
	INT16 FragMsgPacked(CAN_APDU_T *pApdu);
	INT16 CfgFragMsgPacked(CAN_APDU_T *pApdu);

	// Analog receive msg
	INT16 AnalogMsgRecvProc(IN CAN_APDU_T *pApdu);

	INT16 GetFaultData(OUT CAN_APDU_T *pApdu);


	//VOID BattVoltMsgRecvFcb(VOID);
	//VOID TotalPwrMsgRecvFcb(VOID);
	
private:
	INT16 RecDiscreteMsgRecvProc(IN CAN_APDU_T *pApdu);
	VOID AnalogRecvMsgObjsInit(VOID);
	VOID CtrlMsgObjsInit(VOID);
	VOID CovSyncMsgObjInit(VOID);
	VOID CfgMsgObjsInit(VOID);

	VOID AnalogXmitMsgGen(UINT16 u16Idx,UCHAR state);
	//VOID AnalogXmitMsgGen(UINT16 u16Idx);

	VOID PeriodPowerXmitMsgGen(UINT16 u16Idx);

	VOID CfgMsgRqPacked(CAN_APDU_T *pApdu, INT16 i16Idx);
	VOID CfgMsgRsPacked(CAN_APDU_T *pApdu, INT16 i16Idx);

	INT16 GetResendSysSetFlag(void);
	void RestartResendTimer(void);
	void EndResendTimer(void);

	VOID GenStateEvent(VOID);
	VOID GenAlarmEvent(VOID);

	VOID GenBypStateEvent(VOID);
	VOID GenBypAlarmEvent(VOID);
	
	INT16 FaultFragMsgPacked(CAN_APDU_T *pApdu);

	INT16 ParallelMsgRecvProc(IN CAN_APDU_T *pApdu);
	VOID MaintParallelData(VOID);

	INT16 BypMaintMsgRecvProc(IN CAN_APDU_T *pApdu);

	INT16 RackStateMsgRecvProc(IN CAN_APDU_T *pApdu);

	INT16 GetSyncMonitorCnt(void);

	INT16 MonRealMsgRecvProc(IN CAN_APDU_T *pApdu);
	
	 INT16 m_ResendReqSetTimer;
		
	
	LOCAL SysCanDataExchg *m_pDataExchgPtr;

	//COV and Sync msg
	STATE_COV_SYNC_OBJ_T m_StateMsgObj;
	_INV_SW1_T	 m_StateMsgBak ;
	_BYP_SW1_T   m_BypStateMsgBak ;
	ALRM_COV_SYNC_OBJ_T m_AlrmMsgObj;
	_INV_WARN1_T	 m_AlarmMsgBak;
	_BYP_WARN1_T	m_BypAlarmMsgBak;
	ANALOG_SYNC_OBJ_T m_AnalogSyncMsgObj;
	BYP_ANALOG_SYNC_OBJ_T m_BypAnalogSyncMsgObj;

	

	//parallel state msg
	PARALLEL_STATE_COV_SYNC_OBJ_T m_ParallelStateMsgObj;
	_PARALLEL_SW1_T m_ParallelStateMsgBak;

	//State Event COV msg
	STATE_EVENT_LOG_OBJ_T m_StateEventObj;
	ALARM_EVENT_LOG_OBJ_T m_AlarmEventObj;

	BYP_STATE_EVENT_LOG_OBJ_T m_BypStateEventObj;
	BYP_ALARM_EVENT_LOG_OBJ_T m_BypAlarmEventObj;
	
	
	//模拟定时发送计数器定义
	_PERIOD_TIME_CNT_T m_PeriodCnt;
	
	UINT16 m_u16SyncFlag;
	UCHAR	m_ucSyncCfgReqFlag;

	//Analog receive msg
	ANALOG_RECV_OBJ_T m_AnalogRecvMsgObj;
	
	//Cfg msg
	CFG_INFO_T m_CfgMsgObj;
	UINT16	m_ReqSetFlag;
	UINT16 m_CfgMsgIdxBak;

	//同步帧监听msg
	_SYNC_FRAME_MONITOR_T m_SyncFrameMonitor;

	//当前需读取的故障记录号,
	//目前只读最近的3条
	INT16 m_ReadFaultNo;

	UCHAR m_BatchSetEvent;
	UCHAR m_ucFaultClearFlag;
	//Ctrl msg
	CAN_CTRL_MSG_T m_CtrlMsgObj[CTRL_MSG_OBJ_SIZE];
	
	struct 
	{
		CAN_APDU_T CanApdu;
		WORD_TO_2BYTES_T DataBuff[XMIT_INFO_MAX_LEN];
	}m_XmitMsgBuff;
	
	//升级命令信息
	UINT16 m_u16FlashupdateMsg;
	UINT16 m_u16FlashupdateSecnodMsg;
};



#endif 

