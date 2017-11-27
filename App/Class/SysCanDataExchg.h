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

//COV��Ϣ״̬
enum COV_MSG_ST
{
	NON_COV_MSG = 0,
	HAVE_COV_MSG = 1
};

//������Ϣ��������״̬
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
	//��ģ�鹦��ģ������ʱ���ͼ�����,Ŀǰ����Ϊ500ms
	//�˹���(����ֵ)����䴫����ģ�������,�����й��ʺ����ڹ���
	_TIMER_CNT_T u16PeriodPwrCnt;

	//�����Ҫ�����������������涨��
	//�����¶ȶ�ʱ���ͼ�����,Ŀǰ����Ϊ200ms
//	_TIMER_CNT_T u16PeriodEnviTempCnt;
	//���������Ҫ������TBD
	
}_PERIOD_TIME_CNT_T;

typedef struct
{
	INT16 i16XmitActive		:8;	//ͬ����Ϣ����ʹ�ܱ�־
	INT16 i16XmitIdx			:8;	//��ǰ������Ϣservice code
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

//һ���¼���ʽ
typedef struct
{
//	UINT32 u32Second;			//�¼�����ʱ�̵�����, Unix��Ԫ
	UINT16 u16Second;			//�룬���18��Сʱ
	UINT16 u16Millisecond;		//�¼�����ʱ�̵ĺ�����
	UINT16 u16ExEventID;		//Event Log extended ID 
								// bit15 event status : 0---over; 1---start
								//bit14~0 event ID

}EVENT_LOG_INFO_T;

typedef struct
{
	//�¼�COV��־
	UINT16 bCovFlag:1;
	//��ǰ�¼���Ч
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
	//--1.����տ�״̬��0���Ͽ���1���պ�RM����������տ�״̬
//	{180,0,0,1,0},

	//--2.����տ�״̬��0���Ͽ���1���պ�RM����������տ�״̬
	//�պ�ID=181
//	{181,0,0,1,1},
	//--3.ά����·�տ�״̬��0���Ͽ���1���պ�
	//--.�Ͽ�ID=171
	{171,0,1,1,0},
	//--4.�պ�ID=172
	{172,0,1,1,0},
	
	//--3.��·�տ�״̬
	//�Ͽ�ID=178
//	{178,0,2,1,0},

	//�պ�ID=179
//	{179,0,2,1,1},
	
	//--5.��ת�տ�״̬RM��֧��3λ
	//--=0�ر�
//	{173,0,3,7,0},
	//--=1����״̬
//	{174,0,3,7,1},
	//--=2����
//	{175,0,3,7,2},
	//--=3��·
//	{176,0,3,7,3},
	//--=4ά��
//	{177,0,3,7,4},

	//--6.��乩��״̬
	//-=0 ��������,=1��·����,=2��乩��,=3�����ֹ

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
	//--�澯��1
	//--1.�������ͬ��
	{116,0,0,1,1},
	//--2.���������
	{117,0,1,1,1},
	//--3.���������
//	{119,0,2,1,1},
	//--4.�����������
	{127,0,3,1,1},
	//--5.�������س�ʱ
	{129,0,4,1,1},
	//--6.���������ѹ����
//	{131,0,5,1,1},
	//--7.�����ѹ�쳣
	{216,0,6,1,1},
	//--8.���ֱ����������
//	{143,0,7,1,1},
	//--9.���������
//	{132,0,8,1,1},
	//--10.ĸ���쳣�ػ�
	{137,0,9,1,1},
	//--11.ֱ��ĸ�߹�ѹ
	{140,0,10,1,1},
	//--12.����й���
//	{118,0,11,1,1},
	//--13.���ȹ���,���������ȹ����ۺϵ���ز�澯��,�˴�������
	{120,0,12,1,1},
	//--14.��侧բ�ܹ���
	{121,0,13,1,1},
	//--15.�����˿��
	{125,0,14,1,1},
	//--16.������Դ2����
	{126,0,15,1,1},

	//�澯��2
	//--23.�û���������
	{124,1,0,1,1},
	//LBS����
	{144,1,1,3,1},
	//LBS�쳣
	{208,1,1,3,2},	

	//�澯��3
	//--16.��·�����������
//	{114,2,0,3,2},
	//--17.��·�����������
//	{115,2,0,3,1},
	//--18.��·����
//	{113,2,2,1,1},
	//--19.���س��ת��·
	{134,2,3,1,1},
	//--20.��·�л���������
	{135,2,4,1,1},
	//--21.��·����
//	{142,2,5,1,1},
	//--22.��·��բ�ܹ���/1.����/2.��·/3.��·
	//--���ֻ��ע��·
	{122,2,6,3,3},
	//--��·�쳣�ػ�
	{130,2,8,1,1},
	//--���������
	{139,2,9,1,1},
	//--������������
	{136,2,10,1,1},
	//--���������ӹ���
	{141,2,11,1,1},
	//--����ͨѶ����
	{88,2,12,1,1},
	//--����ϵͳ����
	{128,2,13,1,1},
	//--�ٻ�����ת��·
	{138,2,14,1,1}
	
};



CONST EVENT_MSG_T BypStateEventIdObj[]=
{
	//BCB �պ�=1
	{187,0,2,3,1},
	//BCB�Ͽ�=2
	{188,0,2,3,2},

	//--EPO--
	{153,0,1,1,1},

	//--ά����·�տ�״̬��0���Ͽ���1���պ�
	//--.�Ͽ�ID=171
	{171,0,7,1,0},
	//--�պ�ID=172
	{172,0,7,1,1},

	//CLEAR--Fault clear 
	{158,2,0,0,0},

	//SET--Fault clear 
	{158,2,0,0,0}
};

#define BYP_STATE_EVENT_OBJ_SIZE ((sizeof(BypStateEventIdObj))/(sizeof(EVENT_MSG_T)))

CONST EVENT_MSG_T BypAlarmEventIdObj[]=
{
	//-1.��·��������
	{142,0,0,1,1},
	//-2.��·��բ�ܹ���
	//=1����
	{122,0,1,3,1},
	//=3��·
	{122,0,1,3,3},
	//-3.���ȹ���,�ɼ�ز��������/���/��·�ۺϲ���,�˴����ϱ�
	{120,0,3,1,1},
	//-4.��ؽӵع���,�ɼ�ز���������Ƿ��ϱ����ۺ�
	{150,0,4,1,1},
	//-5.��ط������쳣
	{201,0,5,1,1},
	//-6.��·ģ�����,Ŀǰû�д�ID,��Ϊ400
	{400,0,6,1,1},
	//-������Դ1����,�Ƿ�Ӧ���ɼ�����ۺ�
	{110,0,7,1,1},
	//-������Դ2����
	{126,0,8,1,1},
	//-��·���,��·������
	{114,0,9,3,1},
	//-��·���,��·������
	{115,0,9,3,3},
	//-��·����
	{133,0,11,1,1},
	//��һ����ϵͳ�������
	{128,0,12,1,1},
	//���ܼ䲢��ͨѶ����
	{87,0,13,1,1},
	//�ڻ���բ�ܹ���
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

//ͬ��֡����
typedef struct
{
	//�Ƿ����������־=1���ټ���
	UINT16 usMonitorNodeIdFlag;
	//����������
	UINT16 usMonitorCnt ;
}_SYNC_FRAME_MONITOR_T;



//-------------------------------------------------------------------------------
//������Ϣ

typedef VOID (SysCanDataExchg::*CTRL_MSG_RECV_T)(VOID);

//������Ϣ��ش���ṹ��
typedef struct
{
	UINT16 bSrvcCode:	7;
	UINT16 bLength	:	4;
	UINT16 			:	5;
	CTRL_MSG_RECV_T MsgRecvFcb;
}CAN_CTRL_MSG_T;

//--20->23�����Լ��������
#define CTRL_MSG_OBJ_SIZE 23//--20


typedef VOID(SysCanDataExchg::*EVENT_LOG_FCB)(INT16 i16Arg);


class SysCanDataExchg
{
public:
//	SysCanDataExchg(VOID);
//	~SysCanDataExchg(VOID);

	//��ʼ������
	VOID SysCanDataExchgIni(VOID);
	VOID InterfaceDataProc(VOID);

	//��ȡ���ݽ�������ָ��
	LOCAL SysCanDataExchg *GetDataExchgPtr(VOID);

	//TBD
	//COV����
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
	

	//Sync��Ϣ����
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
	
	
	//ģ�ⶨʱ���ͼ���������
	_PERIOD_TIME_CNT_T m_PeriodCnt;
	
	UINT16 m_u16SyncFlag;
	UCHAR	m_ucSyncCfgReqFlag;

	//Analog receive msg
	ANALOG_RECV_OBJ_T m_AnalogRecvMsgObj;
	
	//Cfg msg
	CFG_INFO_T m_CfgMsgObj;
	UINT16	m_ReqSetFlag;
	UINT16 m_CfgMsgIdxBak;

	//ͬ��֡����msg
	_SYNC_FRAME_MONITOR_T m_SyncFrameMonitor;

	//��ǰ���ȡ�Ĺ��ϼ�¼��,
	//Ŀǰֻ�������3��
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
	
	//����������Ϣ
	UINT16 m_u16FlashupdateMsg;
	UINT16 m_u16FlashupdateSecnodMsg;
};



#endif 

