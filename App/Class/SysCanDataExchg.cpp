/***************************************************************************************
* 
* Filename: 		SysCanHal.cpp
* Contributor:
* Author: 		Wang Zhihua	
* Date: 			4/11/2008
* Copyright 		2008 ENPC Corporation, All Rights Reserved
*
* Description: 	
* 
* Version			0
* Last rev by:		Wang Zhihua
* Last rev date:	4/11/2008
* Last rev desc:	
****************************************************************************************/

//---------------------------------------------------------------
//Include header
#include "SysCanDataExchg.h"
#include "MonitorInterface.h"
//#include "Publics.h"
//#include "..\App\Publics.h"

//--------------------------------------------------------------------------------
//Private Enumerated and macro Definitions
#define STATE_COV_MSG_MASK 0x01
#define ALRM_COV_MSG_MASK 0x02
#define EVENT_COV_MSG_MASK 0x04
#define EVENT_COV_MSG_XMIT_MASK 0x08

//TBD
#define LOCAL_SYS_CAN_ID MAC_ID_MOD1_INV
#define BATT_GRP	1

#define SYNC_MSG_SRVC_CODE 0x01
#define PARALLEL_STATE_MSG_SRVC_CODE 0x05
#define BYPMAINT_STATE_MSG_SRVC_CODE 0x06
#define REC_DISCRETE_MSG_SRVC_CODE 0x07
#define MON_REALTIME_MSG_SRVC_CODE 0x08
#define RACK_STATE_MSG_SRVC_CODE 0x09
#define SET_PARA_SUPPORT 1
#define SET_PARA_NOT_SUPPORT 0	

//������Ϣ�е�ʱ��ͬ�����õ�service code
#define TIME_SYNC_SRVE_CODE 0x01

//��������sevice code����غ궨��
//���FLASH UPDATE
#define CTRL_INV_FLASHUPDATE_SRVE_CODE 0x21

//�ط�δ�յ�Ӧ���֡��ʱ
//--500ms
#define RESEND_TIMER_CNT	500

//��ʱ500ms����ģ�鹦����Ϣ����һ�θ���ģ�������
#define PERIOD_SEND_POWER	500

//ͬ��֡������ʱ2000ms
#define SYNC_MONITOR_MAX_CNT  2000


//�궨��
//ͬ��֡������
#define SYNC_FRAME_MONITORING		0
//ͬ��֡�������,���Կ�ʼͨѶ
#define SYNC_FRAME_MONITORED		1
//��⵽����������ͬID�ڵ�,��������
#define SYNC_FRAME_MONITOR_ABORD 0xff

//Test Macro
//#define TEST_MODE


//-------------------------------------------------------------------------------
//Private Structure and Const variable defintion
UINT16 uInvAnalogLen[INV_ANALOG_MSG_OBJ_SIZE];
UINT16 uBypAnalogLen[BYP_ANALOG_MSG_OBJ_SIZE];

//--------------------------------------------------------------------------------
//public variable

//���͡����մ�����ָ������
typedef VOID (SysCanDataExchg::*_MSG_XMIT_FCB)(VOID);
typedef VOID (SysCanDataExchg::*_MSG_RECV_FCB)(VOID);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ģ��������ṹ��
typedef struct
{
	UINT16 u16ServiceCode;
	_MSG_RECV_FCB pMsgRecvFcb;
}_ANALOG_RECV_MSG_T;


CONST ANALOG_MSG_OBJ_T CanAnlogRecvMsgObjs[] = 
{
	//������й�����
	{0x1F, 6, RECV_DIRECTION, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvRecvAnalog.OutActPwr},
	//��������ڹ���
	{0x23, 6, RECV_DIRECTION, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvRecvAnalog.OutApparentPwr},
	//�����¶�
	{0x22, 2,RECV_DIRECTION,(UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvRecvAnalog.u16EnvirTemp},

	//���ܼ䲢��ϵͳ�ܵ��й�����
	{0x32, 6,RECV_DIRECTION,(UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvRecvAnalog.RackOutActPwr},
	//���ܼ䲢��ϵͳ�ܵ����ڹ���
	{0x33, 6,RECV_DIRECTION,(UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvRecvAnalog.RackOutApparentPwr},
	
};

#define ANALOG_RECV_OBJ_SIZE (sizeof(CanAnlogRecvMsgObjs) / sizeof(ANALOG_MSG_OBJ_T))


//~~~~~~~~~~~~~~~~~~~~~~
//������Ϣ

//������Ϣ����
CONST CAN_CTRL_MSG_T CanCtrlMsgObjs[] =
{
	
	//������ת����
	{0x01, 0x00, &SysCanDataExchg::PostBattConvFltChg},
	//������ת����
	{0x02, 0x00, &SysCanDataExchg::NegBattConvFltChg},
	//������ת����
	{0x03, 0x00, &SysCanDataExchg::PostBattConvEqualChg},
	//������ת����
	{0x04, 0x00, &SysCanDataExchg::NegBattConvEqualChg},
	//��ص�ѹ�͹ػ�����
	{0x08, 0x02, &SysCanDataExchg::BattEodTurnOff},
	//���BCB����
//	{0x09, 0x02, &SysCanDataExchg::BattTrip},
	//�����������
	{0x0A, 0x02, &SysCanDataExchg::OutputEnableCmd},
	//�����ֹ����
	{0x0B, 0x02, &SysCanDataExchg::OutputDisableCmd},
	//��ؽӵر�������
	{0x0C, 0x02, &SysCanDataExchg::BattGndPtcCmd},

	//20%�Լ�ŵ�����
	{0x05, 0x02, &SysCanDataExchg::BattSelfChk20Percent},
	//����Լ�ŵ�����
	{0x06, 0x02, &SysCanDataExchg::BattSelfChkDeep},
	//��طŵ��������
	{0x07, 0x02, &SysCanDataExchg::BattSelfChkEnd},

	//ϵͳ��������
	//������������
	{0x10, 0, &SysCanDataExchg::FaultClrCmd},
	//�ֶ���������
	{0x11, 0, &SysCanDataExchg::MaualStartupCmd},
	//�ֶ��ػ�������
	{0x12, 0, &SysCanDataExchg::MaualShutCmd},
	//�����ػ�����
	{0x13, 0, &SysCanDataExchg::EpoCmd},
	//�쳣�ػ�ȷ������
	{0x14, 0, &SysCanDataExchg::AbnormalShutConfirmCmd},
	//�쳣�ػ�ȡ��
	{0x15, 0, &SysCanDataExchg::AbnormalShutCancelCmd},
	//����л�����
	{0x16, 0, &SysCanDataExchg::IndirectSwitchCmd},
	

	//FlashUpdate
	//����Flashupdate
	{0x20, 0, &SysCanDataExchg::RecFlashUpdate},
	//���Flashupdate
	{0x21, 0, &SysCanDataExchg::InvFlashUpdate},
	//��·Flashupdate
	{0x22, 0, &SysCanDataExchg::BypFlashUpdate},

	//���Ԥ�澯����	
	{0x23, 0, &SysCanDataExchg::BattPreWarn},

	//���Ԥ�澲ȡ��
	{0x24, 0, &SysCanDataExchg::BattPreWarnCancel}

		
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`
//ģ������Ϣ//TBD ����ָ��δ����
//��ͬ��֡�ϴ�
CONST ANALOG_MSG_OBJ_T CanAnlogMsgObjs[]=
{

	//������й�����
	{0x10, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.OutActPwr},
	//��������ڹ���
	{0x11, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.OutApparentPwr},
	//�������������
	{0x12, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.OutFactor},
	
	//�������ѹ
	{0x13, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.OutPhaseVlt},
	//���������
	{0x14, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.OutPHaseCurr},
	//������ѹ
	{0x15, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.InvPhaseVlt},
	
	//��������
	{0x16, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.InvPhaseCurr},
	//����е���
	{0x17, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.InductCurr},

	//��·���ѹ
	{0x1C, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.BypPhaseVlt},
	
	//������ذٷֱ�
	{0x18, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.OutLoadRate},
	//�����ֵ��
	{0x19, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.OutPeakRate},
	//���Ƶ��
	{0x1A, 2, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.OutFreq},
	//��·���Ƶ��
	{0x1D, 2, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.BypFreq},
	
	//�����Ա���
	{0x1B, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.DbgVar}
	
};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`
//��·ģ������Ϣ//TBD ����ָ��δ����
//���й��ʺͻ����¶�Ҫ��ʱ�㲥
CONST ANALOG_MSG_OBJ_T BypCanAnlogMsgObjs[]=
{
	//��·���ѹ
	{0x2C, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.BypPhaseVlt},
	//������ѹ
	{0x27, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.BypOutPhaseVlt},
	//��·�������
	{0x2E, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.BypPhaseCurr},
	//�������������
	{0x24, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.OutFactor},
	//��·Ƶ��
	{0x2D, 2, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.BypFreq},
	//���Ƶ��
	{0x28, 2, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.BypOutFreq},

	//����й�����
	{0x2F, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.OutActPwr},
	//������ڹ���
	{0x23, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.OutApparentPwr},

	//������ذٷֱ�
	{0x25, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.OutLoadRate},
	//�����ֵ��
	{0x26, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.OutPeakRate},

	//���ӻ���ϵͳ�й�/���ڹ���
	//����й�����
	{0x32, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.RackOutActPwr},
	//������ڹ���
	{0x33, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.RackOutApparentPwr},
	
	//����¶�---��������ڲ����ⲿ�¶�,���Գ��ȴ�2-->4
//	{0x20, 4, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.BatteryInTemp},
	//�����¶�
//	{0x22, 2, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.EnvirTemp},
	//��·���Ա���
//	{0x21, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.BypDebugVari}
//ȱ��﮵����ģ��Ϣ
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`
//CFG msg objs //TBD ����ָ��δ����
CONST CFG_MSG_T CfgFromMonObjs[] = 
{
	//System dsp
	//ģ���������
	{0x10, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16CapType},
	//ϵͳ��ѹ�ȼ�
	{0x11, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16SysVltGrade},

//mask the REC parameter
#if (0)
	//����Ƶ������
	{0x12, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16InFreq},
	//���ܷ����ģʽ����
	{0x13, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16GenMod},
	//���ܷ�����޹�������
	{0x14, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16GenPwrLtd},
	//���ܷ����������ʱʱ������
	{0x15, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16GenStartDelay},
#endif

	//ϵͳ���ܺ�����
	{0x16, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16RMIdx},
	//����ϵͳ����������
	{0x17, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16RMNum},
	//��������ģ��������
	{0x18, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BasicModNum},
	//��������ģ��������
	{0x19, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16ReModNum},
	//LBSģʽ
	{0x1A, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16LBSMode},
	//ECO ģʽTBD
	{0x1B, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16RunMod},
	//���������ģʽ����
	{0x1C, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16InvRunMode},
	//���Ƶ������
	{0x1D, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutFreq},
	//���ϻ���������ٷֱ�����
	{0x1E, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16AgingOutCurrRate},
	//�������л�ʱ������
	{0x1F, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutSwitchTime},
	//���Ƶ�ʸ�����������
	{0x20, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutFreqTrackRate},
	//��������������������
	{0x21, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16InvOutFactor},
	//�����ѹ΢��ϵ������
	{0x22, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutVltMicroAdj},

	//��·�л���������
	{0x23, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypSwitchTime},
	//��·Ƶ�ʸ��ٷ�Χ����
	{0x24, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypFreqTrackLtd},
	//��·��ѹ������Χ����(����)
	{0x25, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypVltUpLtd},
	//��·��ѹ������Χ����(����)
	{0x26, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypVltDownLtd},
	//��·Ƶ�ʱ�����Χ����
	{0x27, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypFreqLtd},
	//��·���ౣ���ö�·��TBD
	{0x28, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypProtectBrk},
	//��������������
	{0x29, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutputPhase},

	//2017.06.10
	//�����Բ���1
	{0x2b, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16DegVar1},
	//�����Բ���2
	{0x2c, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16DegVar2},
	//�����Բ���3
	{0x2d, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16DegVar3},

	//���������
	{0x35, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16ReRMNum},
	//��������---����/����/�����ȱ���
	{0x36, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16UpsConfigType},
	//share bat	
	{0x3C, 6, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BatShare},	
	//�����Ա���1��ַ
	{0x69, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16DebugVar1Addr},
	//�����Ա���2��ַ
	{0x6A, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16DebugVar2Addr},
	//�����Ա���3��ַ
	{0x6B, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16DebugVar3Addr},
	
		

	
//-----------ϵͳ���õ������������ַ������------------------------------------------------

	//INV set
	//the follow parameter need store to EEPROM
	//���A���ѹУ��ϵ��
	{0x5A, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_a},
	//���B���ѹУ��ϵ��
	{0x5B, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_b},
	//���C���ѹУ��ϵ��
	{0x5C, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_c},
	//���A�����У��ϵ��
	{0x5D, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_a},
	//���B�����У��ϵ��
	{0x5E, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_b},
	//���C�����У��ϵ��
	{0x5F, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_c},
	//���A���ѹУ��ϵ��
	{0x60, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_a},
	//���B���ѹУ��ϵ��
	{0x61, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_b},
	//���C���ѹУ��ϵ��
	{0x62, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_c},
	//���A�����У��ϵ��
	{0x63, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_a},
	//���B�����У��ϵ��
	{0x64, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_b},
	//���C�����У��ϵ��
	{0x65, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_c},
	//��·A���ѹУ��ϵ��
	{0x66, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_a},
	//��·B���ѹУ��ϵ��
	{0x67, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_b},
	//��·C���ѹУ��ϵ��
	{0x68, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_c}
	

	
};

CONST CFG_MSG_T CfgToMonObjs[] = 
{
	//������Ϣ��ѯTBD liuj
	//INV config requirement
	{0x71, INV_SET_INFO_SIZE, FRAG_MSG, FALSE, XMIT_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam},
	//Э��汾��ѯ
	{0x73, 6, NONFRAG_MSG, FALSE, XMIT_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_ModuleVerInfo.ProtocolVer},
	//�̼��汾��ѯ
	{0x74, 6, NONFRAG_MSG, FALSE, XMIT_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_ModuleVerInfo.DspFwVer},
	//CPLD�汾��ѯ
	{0x75, 6, NONFRAG_MSG, FALSE, XMIT_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_ModuleVerInfo.CpldFwVer},
	//ģ�����кŲ�ѯTBD
	{0x76, 6, NONFRAG_MSG, FALSE, XMIT_DIRECTION, FALSE, NULL}

};


CONST UINT16 uInvSetMsgBitSet[]=
{
	//����A���ѹУ��ϵ��
	1,
	//B
	2,
	4,
	8,
	0x10,
	0x20,
	0x40,
	0x80,
	0x100,
	0x200,
	0x400,
	0x800,
	0x1000,
	0x2000,
	0x4000,
	0x8000
};


#define EVENT_SET 0x8000
#define EVENT_CLEAR 0x7fff

//��̬��Ա������ʼ��
SysCanDataExchg* SysCanDataExchg::m_pDataExchgPtr = NULL;

//--------------------------------------------------------------------------------
//private  function

//--------------------------------------------------------------------------------
//function definiton


#if (0)
//״̬COV

COV_MSG_OBJ_T  StateCovObjs[] = 
{
	0x02, 0, SysCanDataExchg::StateCovMsg1Fcb
};

#define STATE_COV_MSG_OBJ_SIZE (sizeof(StateCovObjs) / sizeof(COV_MSG_OBJ_T))

COV_MSG_OBJ_T AlrmCovObjs[] = 
{
	0x05, 0, SysCanDataExchg::AlrmCovMsg1Fcb
};

#define ALRM_COV_MSG_OBJ_SZIE (sizeof(AlrmCovObjs) / sizeof(COV_MSG_OBJ_T))



#endif

#if (0)
/**********************************************************************
GetDataPoolPtr-----get a pointer of class GblDataPool 

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
SysCanDataExchg::SysCanDataExchg(VOID)
{
	//INT16 i;
	
	// ---------------------------------------------------------------
	// 1 control msg objs init
	CtrlMsgObjsInit();	

	//---------------------------------------------------------------
	// 2 Cov and Sync msg objs init
	CovSyncMsgObjInit();

	// 3 Configure msg objs init
	CfgMsgObjsInit();

	// 4 Analog receive msg objs init
	AnalogRecvMsgObjsInit();

}
SysCanDataExchg::~SysCanDataExchg(VOID)
{

}
#endif

//�����ʼ��
VOID SysCanDataExchg::SysCanDataExchgIni(VOID)
{
	//INT16 i;
	
	// ---------------------------------------------------------------
	// 1 control msg objs init
	CtrlMsgObjsInit();	

	//---------------------------------------------------------------
	// 2 Cov and Sync msg objs init
	CovSyncMsgObjInit();

	// 3 Configure msg objs init
	CfgMsgObjsInit();

	// 4 Analog receive msg objs init
	AnalogRecvMsgObjsInit();

	
//    for(UINT16 i = 0;i < INV_ANALOG_MSG_OBJ_SIZE;i++)
//    {
//        uInvAnalogLen[i] = (sizeof(_PHASE_INFO_T));
//    }
	
	uInvAnalogLen[0] = (sizeof(_PHASE_INFO_T));
	uInvAnalogLen[1] = (sizeof(_PHASE_INFO_T));
	uInvAnalogLen[2] = (sizeof(_PHASE_INFO_T));

	uInvAnalogLen[3] = (sizeof(_PHASE_INFO_T));
	uInvAnalogLen[4] = (sizeof(_PHASE_INFO_T));
	uInvAnalogLen[5] = (sizeof(_PHASE_INFO_T));

	uInvAnalogLen[6] = (sizeof(_PHASE_INFO_T));
	uInvAnalogLen[7] = (sizeof(_PHASE_INFO_T));
	uInvAnalogLen[8] = (sizeof(_PHASE_INFO_T));

	uInvAnalogLen[9] = (sizeof(_PHASE_INFO_T));
	uInvAnalogLen[10] = (sizeof(_PHASE_INFO_T));
	
	uInvAnalogLen[11] = (sizeof(_SINGLE_INFO_T));
	uInvAnalogLen[12] = (sizeof(_SINGLE_INFO_T));
	
	uInvAnalogLen[13] = (sizeof(_DBG_INFO_T));
	
//	for(UINT16 i = 0;i < BYP_ANALOG_MSG_OBJ_SIZE;i++)
//	{
//	    uBypAnalogLen[i] = (sizeof(_PHASE_INFO_T));
//	}
	
	uBypAnalogLen[0] = (sizeof(_PHASE_INFO_T));
	uBypAnalogLen[1] = (sizeof(_PHASE_INFO_T));
	uBypAnalogLen[2] = (sizeof(_PHASE_INFO_T));
	uBypAnalogLen[3] = (sizeof(_PHASE_INFO_T));
	
	uBypAnalogLen[4] = (sizeof(_SINGLE_INFO_T));
	uBypAnalogLen[5] = (sizeof(_SINGLE_INFO_T));
	
	uBypAnalogLen[6] = (sizeof(_PHASE_INFO_T));
	uBypAnalogLen[7] = (sizeof(_PHASE_INFO_T));
	uBypAnalogLen[8] = (sizeof(_PHASE_INFO_T));
	uBypAnalogLen[9] = (sizeof(_PHASE_INFO_T));

	uBypAnalogLen[10] = (sizeof(_PHASE_INFO_T));
	uBypAnalogLen[11] = (sizeof(_PHASE_INFO_T));

//	uBypAnalogLen[12] = (sizeof(_BATT_TEMP_INFO_T));
//	uBypAnalogLen[13] = (sizeof(_SINGLE_INFO_T));
//	uBypAnalogLen[14] = (sizeof(_DBG_INFO_T));

}

/**********************************************************************
CtrlMsgObjsInit-----control msg objs init

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
VOID SysCanDataExchg::CtrlMsgObjsInit(VOID)
{
	INT16 i;
	for (i = 0; i < (sizeof(CanCtrlMsgObjs) / sizeof(CAN_CTRL_MSG_T)); i++)
	{
		m_CtrlMsgObj[i] = CanCtrlMsgObjs[i];
	}
	
	m_ucFaultClearFlag = FALSE;
}

/**********************************************************************
CovSyncMsgObjInit-----COV and SYNC msg objs init

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
VOID SysCanDataExchg::CovSyncMsgObjInit(VOID)
{
	INT16 i;
	
	// 1 Sate Msg
	m_StateMsgObj.Msg1 = objMonInterface.m_st_InvSendMonitor.m_st_InvBreakerStatus;
	m_StateMsgBak = objMonInterface.m_st_InvSendMonitor.m_st_InvBreakerStatus;

	m_StateMsgObj.CovObj[0].bCovFlag = FALSE;
	m_StateMsgObj.CovObj[0].bMsgLen = 4;
	m_StateMsgObj.CovObj[0].bSrvcCod = 0x03;
	m_StateMsgObj.CovObj[0].pData = (UINT16 *)&(m_StateMsgObj.Msg1);

	m_StateMsgObj.SyncInfo.i16XmitActive = FALSE;
	m_StateMsgObj.SyncInfo.i16XmitIdx = 0;

		//bypass  msg
	m_StateMsgObj.CovObj[1].bCovFlag = FALSE;
	m_StateMsgObj.CovObj[1].bMsgLen = 2;
	m_StateMsgObj.CovObj[1].bSrvcCod = 0x04;
	m_StateMsgObj.CovObj[1].pData = (UINT16 *)&(m_StateMsgObj.Msg2);
	// 2 Alrm Msg
	m_AlrmMsgObj.Msg1 = objMonInterface.m_st_InvSendMonitor.m_st_InvWarn;
	m_AlarmMsgBak = objMonInterface.m_st_InvSendMonitor.m_st_InvWarn;
	
	m_AlrmMsgObj.CovObj[0].bCovFlag = FALSE;
	m_AlrmMsgObj.CovObj[0].bMsgLen = 6;
	m_AlrmMsgObj.CovObj[0].bSrvcCod = 0x06;
	m_AlrmMsgObj.CovObj[0].pData = (UINT16 *)&(m_AlrmMsgObj.Msg1);

	m_AlrmMsgObj.SyncInfo.i16XmitActive = FALSE;
	m_AlrmMsgObj.SyncInfo.i16XmitIdx = 0;

	//bypass msg
	m_AlrmMsgObj.CovObj[1].bCovFlag = FALSE;
	m_AlrmMsgObj.CovObj[1].bMsgLen = 2;
	m_AlrmMsgObj.CovObj[1].bSrvcCod = 0x07;
	m_AlrmMsgObj.CovObj[1].pData = (UINT16 *)&(m_AlrmMsgObj.Msg2);
	// 3 Analog msg
	for (i = 0; i < INV_ANALOG_MSG_OBJ_SIZE; i++)
	{
		m_AnalogSyncMsgObj.MsgObj[i] = CanAnlogMsgObjs[i];
	}
	//bypass Analog msg
	
	for (i = 0; i < BYP_ANALOG_MSG_OBJ_SIZE; i++)
	{
		m_BypAnalogSyncMsgObj.MsgObj[i] = BypCanAnlogMsgObjs[i];
	}

	// 4.State Event msg
	for (i=0;i<INV_STATE_EVENT_OBJ_SIZE ;i++)
	{
		m_StateEventObj.CovObj[i].pData= (UINT16 *)(&m_StateEventObj.EventLog);
		m_StateEventObj.CovObj[i].bXmitActive= FALSE;	//�¼�����ʹ��
		m_StateEventObj.CovObj[i].bCovFlag = FALSE;		//�¼�COV��־
	}

	// 5.Alarm Event msg
	for (i=0;i<INV_ALARM_EVENT_OBJ_SIZE ;i++)
	{
		m_AlarmEventObj.CovObj[i].pData= (UINT16 *)(&m_AlarmEventObj.EventLog);
		m_AlarmEventObj.CovObj[i].bXmitActive = FALSE;
		m_AlarmEventObj.CovObj[i].bCovFlag = FALSE;
	}

	m_AnalogSyncMsgObj.SyncInfo.i16XmitActive = FALSE;
	m_AnalogSyncMsgObj.SyncInfo.i16XmitIdx = 0;

	// Parallel state msg TBD 
	UINT16 u16Idx = objMonInterface.m_i16wSerialNumSet_0-0x10;
	m_ParallelStateMsgObj.Msg1 = objMonInterface.m_st_wParallelData.ParallelState[u16Idx];
	m_ParallelStateMsgBak = objMonInterface.m_st_wParallelData.ParallelState[u16Idx];

	m_ParallelStateMsgObj.CovObj[0].bCovFlag = FALSE;
	m_ParallelStateMsgObj.CovObj[0].bMsgLen = 2;
	m_ParallelStateMsgObj.CovObj[0].bSrvcCod = 0x05;
	m_ParallelStateMsgObj.CovObj[0].pData = (UINT16 *)&(m_ParallelStateMsgObj.Msg1);

	m_ParallelStateMsgObj.SyncInfo.i16XmitActive = FALSE;
	m_ParallelStateMsgObj.SyncInfo.i16XmitIdx = 0;

	//

	//ͬ��֡�����Ϣ��ʼ��
	m_SyncFrameMonitor.usMonitorNodeIdFlag = SYNC_FRAME_MONITORING;
	m_SyncFrameMonitor.usMonitorCnt = 0;
	

	
	// 4 Sync Flag
	//ͬ��ģ����
	m_u16SyncFlag = FALSE;
	//ͬ����������
	m_ucSyncCfgReqFlag = FALSE;
	
	m_ResendReqSetTimer = 50;

	

}


/**********************************************************************
CfgMsgObjsInit-----Cfg msg objs init

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
VOID SysCanDataExchg::CfgMsgObjsInit(VOID)
{
	INT16 i;

	for (i = 0; i < INV_CFG_FROM_MON_OBJ_SIZE; i++)
	{
		m_CfgMsgObj.FromMon[i] = CfgFromMonObjs[i];
	}

	for (i = 0; i < INV_CFG_TO_MON_OBJ_SIZE; i++)
	{
		m_CfgMsgObj.ToMon[i] = CfgToMonObjs[i];
	}

	m_CfgMsgObj.FirstCfgFlag.bFromMon = TRUE;
	m_CfgMsgObj.FirstCfgFlag.bToMon = TRUE;
	m_ReqSetFlag = TRUE;
	//�������õĵ�һ������SRVS CODE
	m_CfgMsgIdxBak = 0;
	m_BatchSetEvent = 0;

}

/**********************************************************************
AnalogRecvMsgObjsInit-----analog recv msg objs init

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
VOID SysCanDataExchg::AnalogRecvMsgObjsInit(VOID)
{

	INT16 i;

	for (i = 0; i < (ANALOG_RECV_OBJ_SIZE); i++)
	{
		m_AnalogRecvMsgObj.MsgObj[i] = CanAnlogRecvMsgObjs[i];
	}

}

//CANͨѶ��DSP�߼��������ݴ���
//����1.COV��־������2.�¼����� 3.�¼���EEPROM
/**********************************************************************
InterfaceDataProc-----�ӿ����ݴ���

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
VOID SysCanDataExchg::InterfaceDataProc(VOID)
{
//	INT16 i16NewMsg;
//	UINT16 i;
	
	//�ϵ�������δ���
	if ((objMonInterface.m_st_InvSendMonitor.m_st_InvBreakerStatus.Word1_U.tWord1Bits.bParamSetReq)
		&& (0 == objMonInterface.m_st_IdErrorFlag.bUsIdNumExpire) )
	{
		//����δ��ɣ�����COV����
		//�������¼�����
		if (1 == m_BatchSetEvent)
		{
			//INV on setting
			if (TRUE == m_CfgMsgObj.FirstCfgFlag.bFromMon)
			{
				m_StateMsgObj.CovObj[0].bCovFlag = TRUE;
				
				m_StateEventObj.CovObj[INV_STATE_EVENT_OBJ_SIZE-1].bCovFlag = TRUE;
				m_StateEventObj.CovObj[INV_STATE_EVENT_OBJ_SIZE-1].bXmitActive = TRUE;
				m_StateEventObj.EventLog[INV_STATE_EVENT_OBJ_SIZE-1].u16Second = objMonInterface.m_u16TimeLabel2_0;
				m_StateEventObj.EventLog[INV_STATE_EVENT_OBJ_SIZE-1].u16Millisecond = objMonInterface.m_u16TimeLabel1_0;
				
				//set
				m_StateEventObj.EventLog[INV_STATE_EVENT_OBJ_SIZE-1].u16ExEventID = ((StateEventIdObj[INV_STATE_EVENT_OBJ_SIZE-1].uEventID) | (EVENT_SET)) ;

				m_BatchSetEvent = 2;
			}

			//��·����Ҫ���κ�����
		}
		
		
		else
		{
			;//do nothing
		}

	}
	else
	{
		//--1.���״̬COV�ӹ�
		m_StateMsgObj.Msg1 = objMonInterface.m_st_InvSendMonitor.m_st_InvBreakerStatus;
		
		if ( (m_StateMsgObj.Msg1.Word1_U.u16Word1All != m_StateMsgBak.Word1_U.u16Word1All)
			|| (m_StateMsgObj.Msg1.Word2_U.u16Word2All != m_StateMsgBak.Word2_U.u16Word2All) )
		{
			m_StateMsgObj.CovObj[MODULE_INV].bCovFlag = TRUE;
		}
		
		else
		{
			m_StateMsgObj.CovObj[MODULE_INV].bCovFlag = FALSE;
		}
		//��·״̬COV�ӹ�
		m_StateMsgObj.Msg2 = objMonInterface.m_st_BypSendMonitor.m_st_BypBreakerStatus;
		if ( (m_StateMsgObj.Msg2.Word1_U.u16Word1All != m_BypStateMsgBak.Word1_U.u16Word1All))
		{
			m_StateMsgObj.CovObj[MODULE_BYP].bCovFlag = TRUE;
		}
		
		else
		{
			m_StateMsgObj.CovObj[MODULE_BYP].bCovFlag = FALSE;
		}		

		//--2.���澯COV�ӹ�
		m_AlrmMsgObj.Msg1 = objMonInterface.m_st_InvSendMonitor.m_st_InvWarn;

		if ( (m_AlrmMsgObj.Msg1.Word1_U.u16Word1All != m_AlarmMsgBak.Word1_U.u16Word1All)
			|| (m_AlrmMsgObj.Msg1.Word2_U.u16Word2All != m_AlarmMsgBak.Word2_U.u16Word2All) 
			|| (m_AlrmMsgObj.Msg1.Word3_U.u16Word3All != m_AlarmMsgBak.Word3_U.u16Word3All) )
		{
			m_AlrmMsgObj.CovObj[MODULE_INV].bCovFlag = TRUE;
		}

		else
		{
			m_AlrmMsgObj.CovObj[MODULE_INV].bCovFlag = FALSE;
		}

		//--2.��·�澯COV�ӹ�
		//for test
		m_AlrmMsgObj.Msg2 = objMonInterface.m_st_BypSendMonitor.m_st_BypWarn;

		if (m_AlrmMsgObj.Msg2.Word1_U.u16Word1All != m_BypAlarmMsgBak.Word1_U.u16Word1All)
			
		{
			m_AlrmMsgObj.CovObj[MODULE_BYP].bCovFlag = TRUE;
		}

		else
		{
			m_AlrmMsgObj.CovObj[MODULE_BYP].bCovFlag = FALSE;
		}


		//--3.״̬�¼�COV
		if (2 == m_BatchSetEvent)
		{
			if (FALSE == m_CfgMsgObj.FirstCfgFlag.bFromMon)
			{
				m_StateMsgObj.CovObj[0].bCovFlag = TRUE;
				
				m_StateEventObj.CovObj[INV_STATE_EVENT_OBJ_SIZE-1].bCovFlag = TRUE;
				m_StateEventObj.CovObj[INV_STATE_EVENT_OBJ_SIZE-1].bXmitActive = TRUE;
				m_StateEventObj.EventLog[INV_STATE_EVENT_OBJ_SIZE-1].u16Second = objMonInterface.m_u16TimeLabel2_0;
				m_StateEventObj.EventLog[INV_STATE_EVENT_OBJ_SIZE-1].u16Millisecond = objMonInterface.m_u16TimeLabel1_0;
				
				m_StateEventObj.EventLog[INV_STATE_EVENT_OBJ_SIZE-1].u16ExEventID = ((StateEventIdObj[INV_STATE_EVENT_OBJ_SIZE-1].uEventID) & (EVENT_CLEAR)) ;
				m_BatchSetEvent = 0xff;
			}
		}

		else
		{
			;//do nothing
		}

		//����״̬�¼�
		GenStateEvent();

		//--4.�澯�¼�COV
		GenAlarmEvent();

		//--5.����״̬COV TBD,�������¼���
		
		m_ParallelStateMsgObj.Msg1 = objMonInterface.m_st_wParallelData.ParallelState[ objMonInterface.m_i16wSerialNumSet_0-0x10];


		if (m_ParallelStateMsgObj.Msg1.Word1_U.u16Word1All != m_ParallelStateMsgBak.Word1_U.u16Word1All)
			
		{
			m_ParallelStateMsgObj.CovObj[0].bCovFlag = TRUE;
			m_ParallelStateMsgBak = objMonInterface.m_st_wParallelData.ParallelState[ objMonInterface.m_i16wSerialNumSet_0-0x10];
		}

		else
		{
			m_ParallelStateMsgObj.CovObj[0].bCovFlag = FALSE;
		}
		
		//��·״̬�¼�
		GenBypStateEvent();
		//��·�澯�¼�
		GenBypAlarmEvent();
		
		
	}

}

/**********************************************************************
GenStateEvent-----

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
VOID SysCanDataExchg::GenStateEvent(VOID)
{
	UINT16 i;
	
	if (m_StateMsgObj.CovObj[0].bCovFlag == TRUE)
	{
		for (i=0; i<(sizeof(StateEventIdObj)/sizeof(EVENT_MSG_T));i++)
		{
			//��һ��״̬��
			if (StateEventIdObj[i].bEventInWord == 0)
			{

				if (( (m_StateMsgObj.Msg1.Word1_U.u16Word1All>>StateEventIdObj[i].bOffSet) & (StateEventIdObj[i].uMaskVal))
					!= (((m_StateMsgBak.Word1_U.u16Word1All)>>StateEventIdObj[i].bOffSet )& (StateEventIdObj[i].uMaskVal)))
				{
					if (((m_StateMsgObj.Msg1.Word1_U.u16Word1All>>StateEventIdObj[i].bOffSet) & (StateEventIdObj[i].uMaskVal))
						== (StateEventIdObj[i].uSetVal) )
					//set event
					{
						m_StateEventObj.EventLog[i].u16ExEventID = ((StateEventIdObj[i].uEventID) | (EVENT_SET)) ;
						m_StateEventObj.CovObj[i].bCovFlag = TRUE;
						m_StateEventObj.CovObj[i].bXmitActive = TRUE;
						m_StateEventObj.EventLog[i].u16Second = objMonInterface.m_u16TimeLabel2_0;
						m_StateEventObj.EventLog[i].u16Millisecond = objMonInterface.m_u16TimeLabel1_0;
					}
					//clear event
					else if (((m_StateMsgBak.Word1_U.u16Word1All>>StateEventIdObj[i].bOffSet) & (StateEventIdObj[i].uMaskVal))
						== (StateEventIdObj[i].uSetVal))
					{
						m_StateEventObj.EventLog[i].u16ExEventID = ((StateEventIdObj[i].uEventID) & (EVENT_CLEAR)) ;
						m_StateEventObj.CovObj[i].bCovFlag = TRUE;
						m_StateEventObj.CovObj[i].bXmitActive = TRUE;
						m_StateEventObj.EventLog[i].u16Second = objMonInterface.m_u16TimeLabel2_0;
						m_StateEventObj.EventLog[i].u16Millisecond = objMonInterface.m_u16TimeLabel1_0;
					}
					
					else
					{
						
					}

					//TBD ���¼�д��EEPROM��ػ���������liuj
					
				}
			}

			//�ڶ���״̬��
			else if (StateEventIdObj[i].bEventInWord == 1)
			{
				if ( ((m_StateMsgObj.Msg1.Word2_U.u16Word2All>>StateEventIdObj[i].bOffSet) & (StateEventIdObj[i].uMaskVal))
					!= (((m_StateMsgBak.Word2_U.u16Word2All)>>StateEventIdObj[i].bOffSet )& (StateEventIdObj[i].uMaskVal)))
				{
					if (((m_StateMsgObj.Msg1.Word2_U.u16Word2All>>StateEventIdObj[i].bOffSet) & (StateEventIdObj[i].uMaskVal))
						== (StateEventIdObj[i].uSetVal) )
					//set event
					{
						m_StateEventObj.EventLog[i].u16ExEventID = ((StateEventIdObj[i].uEventID) | (EVENT_SET)) ;
						m_StateEventObj.CovObj[i].bCovFlag = TRUE;
						m_StateEventObj.CovObj[i].bXmitActive = TRUE;
						m_StateEventObj.EventLog[i].u16Second = objMonInterface.m_u16TimeLabel2_0;
						m_StateEventObj.EventLog[i].u16Millisecond = objMonInterface.m_u16TimeLabel1_0;
					}
					//clear event
					else if (((m_StateMsgBak.Word2_U.u16Word2All>>StateEventIdObj[i].bOffSet) & (StateEventIdObj[i].uMaskVal))
						== (StateEventIdObj[i].uSetVal) )
					{
						m_StateEventObj.EventLog[i].u16ExEventID = ((StateEventIdObj[i].uEventID) & (EVENT_CLEAR)) ;
						m_StateEventObj.CovObj[i].bCovFlag = TRUE;
						m_StateEventObj.CovObj[i].bXmitActive = TRUE;
						m_StateEventObj.EventLog[i].u16Second = objMonInterface.m_u16TimeLabel2_0;
						m_StateEventObj.EventLog[i].u16Millisecond = objMonInterface.m_u16TimeLabel1_0;
					}
					
					else
					{
						
					}

					//TBD ���¼�д��EEPROM��ػ���������liuj
					
				}
			}			
			else
			{
				;//do nothing
			}
		}

		m_StateMsgBak = objMonInterface.m_st_InvSendMonitor.m_st_InvBreakerStatus;
		
	}

	
	
}

/**********************************************************************
GenAlarmEvent-----

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
VOID SysCanDataExchg::GenAlarmEvent(VOID)
{
	UINT16 i;
	
	
	if (m_AlrmMsgObj.CovObj[MODULE_INV].bCovFlag == TRUE)
	{
		for (i=0;i<(sizeof(AlarmEventIdObj)/sizeof(EVENT_MSG_T));i++)
		{
			//�澯��1
			if (AlarmEventIdObj[i].bEventInWord == 0)
			{
				if ( ((m_AlrmMsgObj.Msg1.Word1_U.u16Word1All >>(AlarmEventIdObj[i].bOffSet)) & AlarmEventIdObj[i].uMaskVal)
					!= (((m_AlarmMsgBak.Word1_U.u16Word1All)>>(AlarmEventIdObj[i].bOffSet)) & AlarmEventIdObj[i].uMaskVal) )
				{
					//set
					if (((m_AlrmMsgObj.Msg1.Word1_U.u16Word1All>>AlarmEventIdObj[i].bOffSet) & (AlarmEventIdObj[i].uMaskVal))
						== (AlarmEventIdObj[i].uSetVal) )
					{
						m_AlarmEventObj.EventLog[i].u16ExEventID = ((AlarmEventIdObj[i].uEventID) | (EVENT_SET)) ;
						m_AlarmEventObj.CovObj[i].bCovFlag = TRUE;
						m_AlarmEventObj.CovObj[i].bXmitActive = TRUE;
						m_AlarmEventObj.EventLog[i].u16Second = objMonInterface.m_u16TimeLabel2_0;
						m_AlarmEventObj.EventLog[i].u16Millisecond = objMonInterface.m_u16TimeLabel1_0;
					}
					//clear
					else if (((m_AlarmMsgBak.Word1_U.u16Word1All>>AlarmEventIdObj[i].bOffSet) & (AlarmEventIdObj[i].uMaskVal))
						== (AlarmEventIdObj[i].uSetVal) )
					{
						m_AlarmEventObj.EventLog[i].u16ExEventID = ((AlarmEventIdObj[i].uEventID) & (EVENT_CLEAR)) ;
						m_AlarmEventObj.CovObj[i].bCovFlag = TRUE;
						m_AlarmEventObj.CovObj[i].bXmitActive = TRUE;
						m_AlarmEventObj.EventLog[i].u16Second = objMonInterface.m_u16TimeLabel2_0;
						m_AlarmEventObj.EventLog[i].u16Millisecond = objMonInterface.m_u16TimeLabel1_0;
					}
					
					else
					{
						
					}
				}
			}
			
			//�澯��2
			else if (AlarmEventIdObj[i].bEventInWord == 1)
			{
				if ( ((m_AlrmMsgObj.Msg1.Word2_U.u16Word2All >>(AlarmEventIdObj[i].bOffSet)) & AlarmEventIdObj[i].uMaskVal)
					!= (((m_AlarmMsgBak.Word2_U.u16Word2All)>>(AlarmEventIdObj[i].bOffSet)) & AlarmEventIdObj[i].uMaskVal) )
				{
					//set
					if (((m_AlrmMsgObj.Msg1.Word2_U.u16Word2All>>AlarmEventIdObj[i].bOffSet) & (AlarmEventIdObj[i].uMaskVal))
						== (AlarmEventIdObj[i].uSetVal) )
					{
						m_AlarmEventObj.EventLog[i].u16ExEventID = ((AlarmEventIdObj[i].uEventID) | (EVENT_SET)) ;
						m_AlarmEventObj.CovObj[i].bCovFlag = TRUE;
						m_AlarmEventObj.CovObj[i].bXmitActive = TRUE;
						m_AlarmEventObj.EventLog[i].u16Second = objMonInterface.m_u16TimeLabel2_0;
						m_AlarmEventObj.EventLog[i].u16Millisecond = objMonInterface.m_u16TimeLabel1_0;
					}
					//clear
					else if (((m_AlarmMsgBak.Word2_U.u16Word2All>>AlarmEventIdObj[i].bOffSet) & (AlarmEventIdObj[i].uMaskVal))
						== (AlarmEventIdObj[i].uSetVal) )
					{
						m_AlarmEventObj.EventLog[i].u16ExEventID = ((AlarmEventIdObj[i].uEventID) & (EVENT_CLEAR)) ;
						m_AlarmEventObj.CovObj[i].bCovFlag = TRUE;
						m_AlarmEventObj.CovObj[i].bXmitActive = TRUE;
						m_AlarmEventObj.EventLog[i].u16Second = objMonInterface.m_u16TimeLabel2_0;
						m_AlarmEventObj.EventLog[i].u16Millisecond = objMonInterface.m_u16TimeLabel1_0;
					}
					
					else
					{
						
					}
				}
			}

			//�澯��3
			else if (AlarmEventIdObj[i].bEventInWord == 2)
			{
				if ( ((m_AlrmMsgObj.Msg1.Word3_U.u16Word3All >>(AlarmEventIdObj[i].bOffSet)) & AlarmEventIdObj[i].uMaskVal)
					!= (((m_AlarmMsgBak.Word3_U.u16Word3All)>>(AlarmEventIdObj[i].bOffSet)) & AlarmEventIdObj[i].uMaskVal) )
				{
					//set
					if (((m_AlrmMsgObj.Msg1.Word3_U.u16Word3All>>AlarmEventIdObj[i].bOffSet) & (AlarmEventIdObj[i].uMaskVal))
						== (AlarmEventIdObj[i].uSetVal) )
					{
						m_AlarmEventObj.EventLog[i].u16ExEventID = ((AlarmEventIdObj[i].uEventID) | (EVENT_SET)) ;
						m_AlarmEventObj.CovObj[i].bCovFlag = TRUE;
						m_AlarmEventObj.CovObj[i].bXmitActive = TRUE;
						m_AlarmEventObj.EventLog[i].u16Second = objMonInterface.m_u16TimeLabel2_0;
						m_AlarmEventObj.EventLog[i].u16Millisecond = objMonInterface.m_u16TimeLabel1_0;
					}
					//clear
					else if (((m_AlarmMsgBak.Word3_U.u16Word3All>>AlarmEventIdObj[i].bOffSet) & (AlarmEventIdObj[i].uMaskVal))
						== (AlarmEventIdObj[i].uSetVal) )
					{
						m_AlarmEventObj.EventLog[i].u16ExEventID = ((AlarmEventIdObj[i].uEventID) & (EVENT_CLEAR)) ;
						m_AlarmEventObj.CovObj[i].bCovFlag = TRUE;
						m_AlarmEventObj.CovObj[i].bXmitActive = TRUE;
						m_AlarmEventObj.EventLog[i].u16Second = objMonInterface.m_u16TimeLabel2_0;
						m_AlarmEventObj.EventLog[i].u16Millisecond = objMonInterface.m_u16TimeLabel1_0;
					}
					
					else
					{
						
					}
				}
			}
			else
			{
				;//do nothing
			}
			
			//TBD ���¼�д��EEPROM��ػ���������liuj
						
		}

		m_AlarmMsgBak = objMonInterface.m_st_InvSendMonitor.m_st_InvWarn;
	}
}


/**********************************************************************
GetDataPoolPtr-----get a pointer of class GblDataPool 

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
SysCanDataExchg* SysCanDataExchg::GetDataExchgPtr(VOID)
{
	if (NULL == m_pDataExchgPtr)
	{
		m_pDataExchgPtr = new SysCanDataExchg();
	}

	return m_pDataExchgPtr;
}

/**********************************************************************
GetStateCovMsg-----��ȡ����״̬������Ϣ

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
INT16 SysCanDataExchg::GetStateCovMsg(OUT CAN_APDU_T *pApdu,UCHAR modul)
{
	INT16 i16NewMsg = NON_COV_MSG;


	//��ʼ����δ���,����COV����
	if (objMonInterface.m_st_InvSendMonitor.m_st_InvBreakerStatus.Word1_U.tWord1Bits.bParamSetReq == 0)
	{
			if(modul < MODULE_NUM)
			{
				if (HAVE_COV_MSG == m_StateMsgObj.CovObj[modul].bCovFlag)
				{
					StateXmitMsgGen(modul);
					i16NewMsg = HAVE_COV_MSG;
				}
			}	
		
	}
	
	if (i16NewMsg == HAVE_COV_MSG)
	{
		*pApdu = m_XmitMsgBuff.CanApdu;

	}
	
	return i16NewMsg;

}

//----------�������״̬������TBD-----------------
/**********************************************************************
GetParallelStateMsg-----��ȡ�����ڲ���״̬������Ϣ

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
INT16 SysCanDataExchg::GetParallelStateMsg(OUT CAN_APDU_T *pApdu)
{
	INT16 i16NewMsg = NON_COV_MSG;
	INT16 i;

	//��ʼ����δ���,����COV����
	if (objMonInterface.m_st_InvSendMonitor.m_st_InvBreakerStatus.Word1_U.tWord1Bits.bParamSetReq == 0)
	{
		for ( i = 0; i < PARALLEL_STATE_MSG_OBJ_SIZE; i++)
		{
			if (HAVE_COV_MSG == m_ParallelStateMsgObj.CovObj[i].bCovFlag)
			{
				ParallelStateXmitMsgGen(i);
				i16NewMsg = HAVE_COV_MSG;

				break;
			}
		
			//ά����Ӧ����
			MaintParallelData();
		}
	}
	
	if (i16NewMsg == HAVE_COV_MSG)
	{
		*pApdu = m_XmitMsgBuff.CanApdu;

	}
	
	return i16NewMsg;

}

/********************************************************************************
MonRealMsgRecvProc-----���Լ�ص�ʵʱ���ݽ��մ���,��ض�ʱ1S�·�
                     	
Parameters:            pApdu:���յ�������֡
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanDataExchg::MonRealMsgRecvProc(IN CAN_APDU_T *pApdu)
{
	INT16 i16NewMsg = NON_SYNC_MSG;
	
	if (pApdu->u16SourceMacId == MAC_ID_MON)
		
	{
		//��������
		objMonInterface.m_st_MonRealMsg.Word1_U.u16Word1All = pApdu->pDataPtr[0].u16Word;	

	}

	return i16NewMsg;
}

/**********************************************************************
ParallelStateXmitMsgGen-----�����ڲ���״̬��Ϣ�ӹ�

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
VOID SysCanDataExchg::ParallelStateXmitMsgGen(UINT16 u16Idx)
{
	UINT16 i;
	UINT16 *pTemp = (UINT16 *)(m_XmitMsgBuff.DataBuff);
	m_XmitMsgBuff.CanApdu.u16ByteLength = m_ParallelStateMsgObj.CovObj[u16Idx].bMsgLen;
	m_XmitMsgBuff.CanApdu.u16ServiceCode = m_ParallelStateMsgObj.CovObj[u16Idx].bSrvcCod;
	m_ParallelStateMsgObj.CovObj[u16Idx].bCovFlag = NON_COV_MSG;
	m_XmitMsgBuff.CanApdu.u16Fragment = NONFRAG_MSG;
	m_XmitMsgBuff.CanApdu.u16MsgClass = CAN_STATE_MSG_CLASS;
	m_XmitMsgBuff.CanApdu.u16RsRq = RS_MSG;//RQ_MSG;
	m_XmitMsgBuff.CanApdu.u16DestinationMacId = MAC_ID_BROADCAST;
	m_XmitMsgBuff.CanApdu.u16SourceMacId = objMonInterface.m_i16wSerialNumSet_0;//LOCAL_SYS_CAN_ID;

	for (i = 0; i < (PARALLEL_SW1_LENGTH / 2); i++)
	{
		*pTemp++ = m_ParallelStateMsgObj.CovObj[u16Idx].pData[i];
	}

	m_XmitMsgBuff.CanApdu.pDataPtr = m_XmitMsgBuff.DataBuff;

}

//--------------------------------------------------

/**********************************************************************
ReqSetMsg-----����������Ϣ����

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
INT16 SysCanDataExchg::ReqSetMsg(OUT CAN_APDU_T *pApdu)
{
	UINT16 i16NewMsg = NON_REQ_MSG;

//mask only  for test
//#if (0)
	//�յ�ͬ��֡���������������Ϣ��δ�յ���صĻ�Ӧ����
	//�����ط�������
	//�����������ñ�־��Ч
	if (TRUE == m_ReqSetFlag)
	{
		if (m_ucSyncCfgReqFlag == TRUE)
		{
			m_ucSyncCfgReqFlag = FALSE;
			//��������������
			pApdu->u16ByteLength = 0;
			pApdu->u16ServiceCode = 0x10;      		//��һ������:������������
			pApdu->u16Fragment = NONFRAG_MSG;
			pApdu->u16MsgClass = CAN_CFG_MSG_CLASS;
			pApdu->u16RsRq = RQ_MSG;
			pApdu->u16DestinationMacId = MAC_ID_MON;
			pApdu->u16SourceMacId = objMonInterface.m_i16wSerialNumSet_0;//LOCAL_SYS_CAN_ID;
			
			i16NewMsg = HAVE_REQ_MSG;
			
		}

		
		
	}

	//�Ѿ���ʼ����,��û�����궨ʱ�����ط�
	else
	{
		//��û������
		if (TRUE ==m_CfgMsgObj.FirstCfgFlag.bFromMon)
		{
			//��ʱ��500MS
			if (GetResendSysSetFlag() == TRUE)
			{
				if (m_CfgMsgIdxBak > INV_SYS_CFG_FROM_MON_OBJ_SIZE-1)
				{
					m_CfgMsgIdxBak = 0;
				}
				CfgMsgRqPacked(pApdu, (m_CfgMsgIdxBak));
				i16NewMsg = HAVE_REQ_MSG;
			}
		}
	}
	
		
//#endif	
	
	return i16NewMsg;

}

/**********************************************************************
GetResendSysSetFlag-----��ȡ�ط���־

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
INT16 SysCanDataExchg::GetResendSysSetFlag(void)
{
	INT16 Ret=FALSE;
	
	//--1ms��ʱ��
	if (objSystem.m_st_wFlagSystem.bTimer1ms == 1)
	{
		//50ms��ʱ��׼���ط�
		if (++m_ResendReqSetTimer > RESEND_TIMER_CNT)
			Ret = TRUE;
		
	}
	
	return Ret;
}

/**********************************************************************
EndResendTimer-----ֹͣ�ط���ʱ

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
void SysCanDataExchg::EndResendTimer(void)
{
	m_ResendReqSetTimer = RESEND_TIMER_CNT;
}

/**********************************************************************
RestartResendTimer-----���������ط���ʱ

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
void SysCanDataExchg::RestartResendTimer(void)
{
	m_ResendReqSetTimer = 0;
}


/**********************************************************************
StateCovMsgXmitState-----����״̬����״̬

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
VOID SysCanDataExchg::StateCovMsgXmitState(IN INT16 i16XmitState)
{
	
}


/**********************************************************************
StateCovMsg1Fcb-----״̬��Ϣ�ӹ�

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: u16IDx :ģ������� ��仹����·
**********************************************************************/
VOID SysCanDataExchg::StateXmitMsgGen(UINT16 u16Idx)
{
	UINT16 i;
	UINT16 *pTemp = (UINT16 *)(m_XmitMsgBuff.DataBuff);
	m_XmitMsgBuff.CanApdu.u16ByteLength = m_StateMsgObj.CovObj[u16Idx].bMsgLen;
	m_XmitMsgBuff.CanApdu.u16ServiceCode = m_StateMsgObj.CovObj[u16Idx].bSrvcCod;
	m_StateMsgObj.CovObj[u16Idx].bCovFlag = NON_COV_MSG;
	m_XmitMsgBuff.CanApdu.u16Fragment = NONFRAG_MSG;
	m_XmitMsgBuff.CanApdu.u16MsgClass = CAN_STATE_MSG_CLASS;
	m_XmitMsgBuff.CanApdu.u16RsRq = RS_MSG;//RQ_MSG;
	m_XmitMsgBuff.CanApdu.u16DestinationMacId = MAC_ID_MON;
	
	if(u16Idx == MODULE_INV)//���
	{
		m_XmitMsgBuff.CanApdu.u16SourceMacId = objMonInterface.m_i16wSerialNumSet_0;//LOCAL_SYS_CAN_ID;
		for (i = 0; i < (INV_SW1_LENGTH / 2); i++)
		{
			*pTemp++ = m_StateMsgObj.CovObj[MODULE_INV].pData[i];
		}
	}
	if(u16Idx == MODULE_BYP)//��·
	{
		m_XmitMsgBuff.CanApdu.u16SourceMacId=MAC_ID_BYP;
		for (i = 0; i < (BYP_SW1_LENGTH / 2); i++)
		{
			*pTemp++ = m_StateMsgObj.CovObj[MODULE_BYP].pData[i];
		}
	}


	m_XmitMsgBuff.CanApdu.pDataPtr = m_XmitMsgBuff.DataBuff;

}


/**********************************************************************
GetAlrmCovMsg-----��ȡ�����澯������Ϣ

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
INT16 SysCanDataExchg::GetAlrmCovMsg(OUT CAN_APDU_T *pApdu,UCHAR modul)
{
	INT16 i16NewMsg = NON_COV_MSG;

	//��ʼ����δ���,����COV����
	if (objMonInterface.m_st_InvSendMonitor.m_st_InvBreakerStatus.Word1_U.tWord1Bits.bParamSetReq == 0)
	{
			if (HAVE_COV_MSG == m_AlrmMsgObj.CovObj[modul].bCovFlag)
			{
				AlrmXmitMsgGen(modul);
				i16NewMsg = HAVE_COV_MSG;
			}
			
	}
	
	if (i16NewMsg == HAVE_COV_MSG)
	{
		*pApdu = m_XmitMsgBuff.CanApdu;
	}
	
	return i16NewMsg;

}

/**********************************************************************
AlrmCovMsgXmitState-----�����澯����״̬

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
VOID SysCanDataExchg::AlrmCovMsgXmitState(IN INT16 i16XmitState)
{


}

/**********************************************************************
StateCovMsg1Fcb-----�澯��Ϣ�ӹ�

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
VOID SysCanDataExchg::AlrmXmitMsgGen(UINT16 u16Idx)
{
	UINT16 i;
	UINT16 *pTemp = (UINT16 *)(m_XmitMsgBuff.DataBuff);
	m_XmitMsgBuff.CanApdu.u16ByteLength = m_AlrmMsgObj.CovObj[u16Idx].bMsgLen;
	m_XmitMsgBuff.CanApdu.u16ServiceCode = m_AlrmMsgObj.CovObj[u16Idx].bSrvcCod;
	m_AlrmMsgObj.CovObj[u16Idx].bCovFlag = NON_COV_MSG;
	m_XmitMsgBuff.CanApdu.u16Fragment = NONFRAG_MSG;
	m_XmitMsgBuff.CanApdu.u16MsgClass = CAN_ALRM_MSG_CLASS;
	m_XmitMsgBuff.CanApdu.u16RsRq = RS_MSG;//RQ_MSG;
	m_XmitMsgBuff.CanApdu.u16DestinationMacId = MAC_ID_MON;

	if(u16Idx == MODULE_INV)//���
	{
		m_XmitMsgBuff.CanApdu.u16SourceMacId = objMonInterface.m_i16wSerialNumSet_0;//LOCAL_SYS_CAN_ID;
		for (i = 0; i < (INV_ALRM1_LENGTH / 2); i++)
		{
			*pTemp++ = m_AlrmMsgObj.CovObj[MODULE_INV].pData[i];
		}
	}
	else		//��·
	{
		m_XmitMsgBuff.CanApdu.u16SourceMacId = MAC_ID_BYP;
		for (i = 0; i < (BYP_ALRM1_LENGTH / 2); i++)
		{
			*pTemp++ = m_AlrmMsgObj.CovObj[MODULE_BYP].pData[i];
		}
	}


	m_XmitMsgBuff.CanApdu.pDataPtr = m_XmitMsgBuff.DataBuff;

}


/**********************************************************************
GetEventLogMsg-----��ȡ�¼���¼COV��Ϣ

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
INT16 SysCanDataExchg::GetEventLogMsg(OUT CAN_APDU_T *pApdu,UCHAR modul)
{
	UINT16 i;
	
	INT16 i16NewMsg = NON_COV_MSG;

	//��ʼ����δ���,����COV����
	if (objMonInterface.m_st_InvSendMonitor.m_st_InvBreakerStatus.Word1_U.tWord1Bits.bParamSetReq == 0)
	{
		if(modul == MODULE_INV)
		{
		//�澯�¼�COV
		for (i=0;i<INV_ALARM_EVENT_OBJ_SIZE;i++)
		{
			if (m_AlarmEventObj.CovObj[i].bCovFlag == TRUE)
			{
				AlarmEventXmitMsgGen(i,modul);
				i16NewMsg = HAVE_COV_MSG;
				break;
			}
		}

		//״̬�¼�COV
		if (i16NewMsg == NON_COV_MSG)
		{
			for (i=0;i<INV_STATE_EVENT_OBJ_SIZE;i++)
			{
				if (m_StateEventObj.CovObj[i].bCovFlag == TRUE)
				{
					StateEventXmitMsgGen(i,modul);
					i16NewMsg = HAVE_COV_MSG;
					break;
				}
				
			}
		}
		}

		
		if(modul == MODULE_BYP)
		{
		//��·�澯�¼�
		for (i=0;i<BYP_ALARM_EVENT_OBJ_SIZE;i++)
		{
			if (m_BypAlarmEventObj.CovObj[i].bCovFlag == TRUE)
			{
				AlarmEventXmitMsgGen(i,modul);
				i16NewMsg = HAVE_COV_MSG;
				break;
			}
		}

		//״̬�¼�COV
		if (i16NewMsg == NON_COV_MSG)
		{
			for (i=0;i<BYP_STATE_EVENT_OBJ_SIZE;i++)
			{
				if (m_BypStateEventObj.CovObj[i].bCovFlag == TRUE)
				{
					StateEventXmitMsgGen(i,modul);
					i16NewMsg = HAVE_COV_MSG;
					break;
				}
				
			}
		}
		}

	}

	//���������¼�
	else
	{
		if ((m_StateEventObj.CovObj[INV_STATE_EVENT_OBJ_SIZE-1].bCovFlag == TRUE)
			&&(modul == MODULE_INV))
		{
			StateEventXmitMsgGen(INV_STATE_EVENT_OBJ_SIZE-1,MODULE_INV);
			i16NewMsg = HAVE_COV_MSG;
		}

		if ((m_BypStateEventObj.CovObj[BYP_STATE_EVENT_OBJ_SIZE-1].bCovFlag == TRUE)
			&&(modul == MODULE_BYP))
		{
			StateEventXmitMsgGen(BYP_STATE_EVENT_OBJ_SIZE-1,MODULE_BYP);
			i16NewMsg = HAVE_COV_MSG;
		}
	}
	
	if (i16NewMsg == HAVE_COV_MSG)
	{
		*pApdu = m_XmitMsgBuff.CanApdu;
	}
	
	
	return i16NewMsg;

}

/**********************************************************************
EventLogXmitMsgGen-----

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
VOID SysCanDataExchg::AlarmEventXmitMsgGen(UINT16 EventIndex,UCHAR modul)
{
	

	UINT16 *pTemp = (UINT16 *)(m_XmitMsgBuff.DataBuff);
	
	m_XmitMsgBuff.CanApdu.u16ByteLength = 6;			//6BYTES ��Ч����
	m_XmitMsgBuff.CanApdu.u16ServiceCode = 0x04;
	m_XmitMsgBuff.CanApdu.u16Fragment = NONFRAG_MSG;
	m_XmitMsgBuff.CanApdu.u16MsgClass = CAN_ALRM_MSG_CLASS;
	m_XmitMsgBuff.CanApdu.u16RsRq = RS_MSG;//RQ_MSG;
	m_XmitMsgBuff.CanApdu.u16DestinationMacId = MAC_ID_MON;


	if(modul == MODULE_INV)
	{
		m_AlarmEventObj.CovObj[EventIndex].bCovFlag = FALSE;
		m_XmitMsgBuff.CanApdu.u16SourceMacId = objMonInterface.m_i16wSerialNumSet_0;//LOCAL_SYS_CAN_ID;		
		*pTemp++ = m_AlarmEventObj.EventLog[EventIndex].u16ExEventID;
		*pTemp++ = m_AlarmEventObj.EventLog[EventIndex].u16Second;
		*pTemp++ = m_AlarmEventObj.EventLog[EventIndex].u16Millisecond;
	}
	if(modul == MODULE_BYP)
	{
		m_BypAlarmEventObj.CovObj[EventIndex].bCovFlag = FALSE;
		m_XmitMsgBuff.CanApdu.u16SourceMacId = MAC_ID_BYP;//bypass	
		*pTemp++ = m_BypAlarmEventObj.EventLog[EventIndex].u16ExEventID;
		*pTemp++ = m_BypAlarmEventObj.EventLog[EventIndex].u16Second;
		*pTemp++ = m_BypAlarmEventObj.EventLog[EventIndex].u16Millisecond;		
	}

	m_XmitMsgBuff.CanApdu.pDataPtr = m_XmitMsgBuff.DataBuff;
		
	
}

/**********************************************************************
StateEventXmitMsgGen-----

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
VOID SysCanDataExchg::StateEventXmitMsgGen(UINT16 EventIndex,UCHAR modul)
{
	
	UINT16 *pTemp = (UINT16 *)(m_XmitMsgBuff.DataBuff);
	
	m_XmitMsgBuff.CanApdu.u16ByteLength = 6;			//6BYTES ��Ч����
	m_XmitMsgBuff.CanApdu.u16ServiceCode = 0x04;
	m_XmitMsgBuff.CanApdu.u16Fragment = NONFRAG_MSG;
	m_XmitMsgBuff.CanApdu.u16MsgClass = CAN_ALRM_MSG_CLASS;
	m_XmitMsgBuff.CanApdu.u16RsRq = RS_MSG;//RQ_MSG;
	m_XmitMsgBuff.CanApdu.u16DestinationMacId = MAC_ID_MON;

	if(modul == MODULE_INV)
	{
		m_StateEventObj.CovObj[EventIndex].bCovFlag = FALSE;
		m_XmitMsgBuff.CanApdu.u16SourceMacId = objMonInterface.m_i16wSerialNumSet_0;//LOCAL_SYS_CAN_ID;		
		*pTemp++ = m_StateEventObj.EventLog[EventIndex].u16ExEventID;
		*pTemp++ = m_StateEventObj.EventLog[EventIndex].u16Second;
		*pTemp++ = m_StateEventObj.EventLog[EventIndex].u16Millisecond;
	}
	if(modul == MODULE_BYP)
	{
		m_BypStateEventObj.CovObj[EventIndex].bCovFlag = FALSE;
		m_XmitMsgBuff.CanApdu.u16SourceMacId = MAC_ID_BYP;//bypass	
		*pTemp++ = m_BypStateEventObj.EventLog[EventIndex].u16ExEventID;
		*pTemp++ = m_BypStateEventObj.EventLog[EventIndex].u16Second;
		*pTemp++ = m_BypStateEventObj.EventLog[EventIndex].u16Millisecond;	
	}
	


	m_XmitMsgBuff.CanApdu.pDataPtr = m_XmitMsgBuff.DataBuff;
		

}

/**********************************************************************
GetStateSyncMsg-----��ȡͬ��״̬��Ϣ

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
INT16 SysCanDataExchg::GetStateSyncMsg(OUT CAN_APDU_T *pApdu,UCHAR modul)
{
	INT16 i16NewMsg = NON_SYNC_MSG;

	//��ʼ����δ���,����COV����
	if (objMonInterface.m_st_InvSendMonitor.m_st_InvBreakerStatus.Word1_U.tWord1Bits.bParamSetReq == 0)
	{
		if (TRUE == m_StateMsgObj.SyncInfo.i16XmitActive)
		{
			if (m_StateMsgObj.SyncInfo.i16XmitIdx < STATE_MSG_OBJ_SIZE) 
			{
				StateXmitMsgGen(modul);
				m_StateMsgObj.SyncInfo.i16XmitIdx++;
				i16NewMsg = HAVE_SYNC_MSG;
			}
			else
			{
				m_StateMsgObj.SyncInfo.i16XmitIdx = 0;
				m_StateMsgObj.SyncInfo.i16XmitActive = FALSE;	
			}

		}

		//�����ڲ���״̬����SYNC
		else if ((TRUE == m_ParallelStateMsgObj.SyncInfo.i16XmitActive)
				&&(modul == MODULE_INV))
		{
			if (m_ParallelStateMsgObj.SyncInfo.i16XmitIdx < PARALLEL_STATE_MSG_OBJ_SIZE ) 
			{
				ParallelStateXmitMsgGen(m_ParallelStateMsgObj.SyncInfo.i16XmitIdx);
				m_ParallelStateMsgObj.SyncInfo.i16XmitIdx++;
				i16NewMsg = HAVE_SYNC_MSG;

				//ά����Ӧ����
				MaintParallelData();
			}

			else
			{
				m_ParallelStateMsgObj.SyncInfo.i16XmitIdx = 0;
				m_ParallelStateMsgObj.SyncInfo.i16XmitActive = FALSE;
			}
		}
	}

	if (i16NewMsg == HAVE_COV_MSG)
	{
		*pApdu = m_XmitMsgBuff.CanApdu;
	}

	return i16NewMsg;
}

/**********************************************************************
StateSyncMsgXmitState-----ͬ��״̬��Ϣ����״̬

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
VOID SysCanDataExchg::StateSyncMsgXmitState(IN INT16 i16XmitState)
{

}


/**********************************************************************
GetAlrmSyncMsg-----��ȡͬ���澯��Ϣ

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
INT16 SysCanDataExchg::GetAlrmSyncMsg(OUT CAN_APDU_T *pApdu,UCHAR modul)
{
	INT16 i16NewMsg = NON_SYNC_MSG;

	//��ʼ����δ���,����COV����
	if (objMonInterface.m_st_InvSendMonitor.m_st_InvBreakerStatus.Word1_U.tWord1Bits.bParamSetReq == 0)
	{
		if (TRUE == m_AlrmMsgObj.SyncInfo.i16XmitActive)
		{
			if (m_AlrmMsgObj.SyncInfo.i16XmitIdx < STATE_MSG_OBJ_SIZE) 
			{
				AlrmXmitMsgGen(modul);
				m_AlrmMsgObj.SyncInfo.i16XmitIdx++;
				i16NewMsg = HAVE_SYNC_MSG;
			}

			else
			{
				m_AlrmMsgObj.SyncInfo.i16XmitIdx = 0;
				m_AlrmMsgObj.SyncInfo.i16XmitActive = FALSE;
			}
			
		}
	}

	if (i16NewMsg == HAVE_COV_MSG)
	{
		*pApdu = m_XmitMsgBuff.CanApdu;
	}

	return i16NewMsg;
}

/**********************************************************************
AlrmSyncMsgXmitState-----ͬ���澯��Ϣ����״̬

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
VOID SysCanDataExchg::AlrmSyncMsgXmitState(IN INT16 i16XmitState)
{


}

/**********************************************************************
GetAnalogSyncMsg-----��ȡͬ��ģ������Ϣ

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
INT16 SysCanDataExchg::GetAnalogSyncMsg(OUT CAN_APDU_T *pApdu,UCHAR modul)
{
//	INT16 i;
	INT16 i16NewMsg = NON_SYNC_MSG;

	//��ʼ����δ���,����COV����
	if (objMonInterface.m_st_InvSendMonitor.m_st_InvBreakerStatus.Word1_U.tWord1Bits.bParamSetReq == 0)
	{
		if (TRUE == m_AnalogSyncMsgObj.SyncInfo.i16XmitActive)
		{
			//��BYP_ANALOG_MSG_OBJ_SIZE<INV_ANALOG_MSG_OBJ_SIZE��Ӧ���½ṹ���������λ�öԵ�
			if(m_AnalogSyncMsgObj.SyncInfo.i16XmitIdx < INV_ANALOG_MSG_OBJ_SIZE)
			{
				if (modul==MODULE_INV)
				{
					AnalogXmitMsgGen(m_AnalogSyncMsgObj.SyncInfo.i16XmitIdx,MODULE_INV);
					i16NewMsg = HAVE_SYNC_MSG;
				}
				else if(modul==MODULE_BYP)
				{
					if(m_AnalogSyncMsgObj.SyncInfo.i16XmitIdx < BYP_ANALOG_MSG_OBJ_SIZE)
					{
						AnalogXmitMsgGen(m_AnalogSyncMsgObj.SyncInfo.i16XmitIdx,MODULE_BYP);
						i16NewMsg = HAVE_SYNC_MSG;
					}
					m_AnalogSyncMsgObj.SyncInfo.i16XmitIdx++;

				}
				else //���������ô���
				{
					m_AnalogSyncMsgObj.SyncInfo.i16XmitIdx=0;
				}

			}
			else
			{
				m_AnalogSyncMsgObj.SyncInfo.i16XmitIdx = 0;
				m_AnalogSyncMsgObj.SyncInfo.i16XmitActive = FALSE;
				m_u16SyncFlag = FALSE;
			}
		}
	}

	if (i16NewMsg == HAVE_COV_MSG)
	{
		*pApdu = m_XmitMsgBuff.CanApdu;
	}

	return i16NewMsg;

}


/**********************************************************************
PeriodAnalogSyncMsg-----��ʱ���ݱ�ģ��������Ϣ���͸�����

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
INT16 SysCanDataExchg::PeriodAnalogSyncMsg(OUT CAN_APDU_T *pApdu)
{
	INT16 i16NewMsg = NON_SYNC_MSG;
	
	//��ʼ����δ���,����COV����
	if (objMonInterface.m_st_InvSendMonitor.m_st_InvBreakerStatus.Word1_U.tWord1Bits.bParamSetReq == 0)
	{
		//��ʱ����
		//--1ms��ʱ��
		if (objSystem.m_st_wFlagSystem.bTimer1ms == 1)
		{
			//������Ϣ��ʱ
			if ((++m_PeriodCnt.u16PeriodPwrCnt.u16ActiveCnt) >= PERIOD_SEND_POWER)
			{
				m_PeriodCnt.u16PeriodPwrCnt.u16ActiveCnt = PERIOD_SEND_POWER;
			}
			
		}

		if (m_PeriodCnt.u16PeriodPwrCnt.u16ActiveCnt == PERIOD_SEND_POWER)
		{
			if (m_PeriodCnt.u16PeriodPwrCnt.i16XmitIdx < 4 ) 
			{
				PeriodPowerXmitMsgGen(m_PeriodCnt.u16PeriodPwrCnt.i16XmitIdx);
				m_PeriodCnt.u16PeriodPwrCnt.i16XmitIdx++;
				i16NewMsg = HAVE_SYNC_MSG;
			}

			else
			{
				m_PeriodCnt.u16PeriodPwrCnt.i16XmitIdx = 0;
				m_PeriodCnt.u16PeriodPwrCnt.u16ActiveCnt = 0;
			}
		}

	}

	if (i16NewMsg == HAVE_COV_MSG)
	{
		*pApdu = m_XmitMsgBuff.CanApdu;
	}

	return i16NewMsg;
	
}

/**********************************************************************
PeriodPowerXmitMsgGen-----��ʱ���͹�����Ϣ׼��

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
VOID SysCanDataExchg::PeriodPowerXmitMsgGen(UINT16 u16Idx)
{
	UINT16 *pTemp = (UINT16 *)(m_XmitMsgBuff.DataBuff);
	
	switch (u16Idx)
	{
		//�й�����
		case 0:
			m_XmitMsgBuff.CanApdu.u16ByteLength = 6;
			m_XmitMsgBuff.CanApdu.u16ServiceCode = 0x10;
			m_XmitMsgBuff.CanApdu.u16Fragment = NONFRAG_MSG;
			m_XmitMsgBuff.CanApdu.u16MsgClass = CAN_ANALOG_MSG_CLASS;
			m_XmitMsgBuff.CanApdu.u16RsRq = RS_MSG;
			m_XmitMsgBuff.CanApdu.u16DestinationMacId = 0x10+objMonInterface.m_i16wSerialNumSet_0;
			m_XmitMsgBuff.CanApdu.u16SourceMacId = objMonInterface.m_i16wSerialNumSet_0;

			*pTemp++ = objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.OutActPwr.u16Phase_a;
			*pTemp++ = objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.OutActPwr.u16Phase_b;
			*pTemp++ = objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.OutActPwr.u16Phase_c;
			
			m_XmitMsgBuff.CanApdu.pDataPtr = m_XmitMsgBuff.DataBuff;
			break;
		//���ڹ���
		case 1:
			m_XmitMsgBuff.CanApdu.u16ByteLength = 6;
			m_XmitMsgBuff.CanApdu.u16ServiceCode = 0x11;
			m_XmitMsgBuff.CanApdu.u16Fragment = NONFRAG_MSG;
			m_XmitMsgBuff.CanApdu.u16MsgClass = CAN_ANALOG_MSG_CLASS;
			m_XmitMsgBuff.CanApdu.u16RsRq = RS_MSG;
			m_XmitMsgBuff.CanApdu.u16DestinationMacId = 0x10+objMonInterface.m_i16wSerialNumSet_0;
			m_XmitMsgBuff.CanApdu.u16SourceMacId = objMonInterface.m_i16wSerialNumSet_0;

			*pTemp++ = objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.OutApparentPwr.u16Phase_a;
			*pTemp++ = objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.OutApparentPwr.u16Phase_b;
			*pTemp++ = objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.OutApparentPwr.u16Phase_c;
			
			m_XmitMsgBuff.CanApdu.pDataPtr = m_XmitMsgBuff.DataBuff;
			break;
			
		//��·����
		case 2:
			m_XmitMsgBuff.CanApdu.u16ByteLength = 6;
			m_XmitMsgBuff.CanApdu.u16ServiceCode = 0x1E;
			m_XmitMsgBuff.CanApdu.u16Fragment = NONFRAG_MSG;
			m_XmitMsgBuff.CanApdu.u16MsgClass = CAN_ANALOG_MSG_CLASS;
			m_XmitMsgBuff.CanApdu.u16RsRq = RS_MSG;
			m_XmitMsgBuff.CanApdu.u16DestinationMacId = 0x10+objMonInterface.m_i16wSerialNumSet_0;
			m_XmitMsgBuff.CanApdu.u16SourceMacId = objMonInterface.m_i16wSerialNumSet_0;

			*pTemp++ = objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.BypPhaseCurr.u16Phase_a;
			*pTemp++ = objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.BypPhaseCurr.u16Phase_b;
			*pTemp++ = objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.BypPhaseCurr.u16Phase_c;
			
			m_XmitMsgBuff.CanApdu.pDataPtr = m_XmitMsgBuff.DataBuff;
			break;	

		//��С�����ID
		case 3:

			m_XmitMsgBuff.CanApdu.u16ByteLength = 6;
			m_XmitMsgBuff.CanApdu.u16ServiceCode = 0x34;
			m_XmitMsgBuff.CanApdu.u16Fragment = NONFRAG_MSG;
			m_XmitMsgBuff.CanApdu.u16MsgClass = CAN_ANALOG_MSG_CLASS;
			m_XmitMsgBuff.CanApdu.u16RsRq = RS_MSG;
			m_XmitMsgBuff.CanApdu.u16DestinationMacId = 0x10+objMonInterface.m_i16wSerialNumSet_0;
			m_XmitMsgBuff.CanApdu.u16SourceMacId = objMonInterface.m_i16wSerialNumSet_0;

			//�ѿ�����С�����ID

			*pTemp++ = objPowCANSched.m_i16ChgOnMinID;
			*pTemp++ = objPowCANSched.m_i16ChgPBoostTotal;//ǿ��ת��������
			*pTemp++ = 0;//Ԥ������
			
			m_XmitMsgBuff.CanApdu.pDataPtr = m_XmitMsgBuff.DataBuff;
			break;	
		default:

			break;
	}
}


/**********************************************************************
AnalogSyncMsgXmitState-----ͬ��ģ������Ϣ����״̬

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
VOID SysCanDataExchg::AnalogSyncMsgXmitState(IN INT16 i16XmiyState)
{
	

}

/**********************************************************************
AnalogXmitMsgGen-----ģ������Ϣ�ӹ�

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
VOID SysCanDataExchg::AnalogXmitMsgGen(UINT16 u16Idx,UCHAR modul)
{
	UINT16 i;
	UINT16 *pTemp = (UINT16 *)(m_XmitMsgBuff.DataBuff);
	//m_AlrmMsgObj.CovObj[u16Idx].bCovFlag == NON_COV_MSG;
	m_XmitMsgBuff.CanApdu.u16Fragment = NONFRAG_MSG;
	m_XmitMsgBuff.CanApdu.u16MsgClass = CAN_ANALOG_MSG_CLASS;
	m_XmitMsgBuff.CanApdu.u16RsRq = RS_MSG;//RQ_MSG;
	m_XmitMsgBuff.CanApdu.u16DestinationMacId = MAC_ID_MON;
	if(modul == MODULE_INV)
	{
		m_XmitMsgBuff.CanApdu.u16ByteLength = m_AnalogSyncMsgObj.MsgObj[u16Idx].bMsgLen;
		m_XmitMsgBuff.CanApdu.u16ServiceCode = m_AnalogSyncMsgObj.MsgObj[u16Idx].bSrvcCod;
		m_XmitMsgBuff.CanApdu.u16SourceMacId = objMonInterface.m_i16wSerialNumSet_0;//LOCAL_SYS_CAN_ID;
		for (i = 0; i < (uInvAnalogLen[u16Idx]); i++)
		{
			*pTemp++ = m_AnalogSyncMsgObj.MsgObj[u16Idx].pData[i];
		}		
	}
	else
	{
		m_XmitMsgBuff.CanApdu.u16ByteLength = m_BypAnalogSyncMsgObj.MsgObj[u16Idx].bMsgLen;
		m_XmitMsgBuff.CanApdu.u16ServiceCode = m_BypAnalogSyncMsgObj.MsgObj[u16Idx].bSrvcCod;
		m_XmitMsgBuff.CanApdu.u16SourceMacId = MAC_ID_BYP;
		for (i = 0; i < (uBypAnalogLen[u16Idx]); i++)
		{
			*pTemp++ = m_BypAnalogSyncMsgObj.MsgObj[u16Idx].pData[i];
		}
	}
	m_XmitMsgBuff.CanApdu.pDataPtr = m_XmitMsgBuff.DataBuff;

}

/**********************************************************************
BattVoltMsgRecvFcb-----���յ�ظ��䡢�����ѹ

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
#if (0)
VOID SysCanDataExchg::BattVoltMsgRecvFcb(VOID)
{
	objMonInterface.m_st_RecAnalog_D.FltVlt.u16PostVlt = ;
	objMonInterface.m_st_RecAnalog_D.FltVlt.u16NegVlt = ;
	

}



/**********************************************************************
TotalPwrMsgRecvFcb-----�����ܹ�����Ϣ

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
VOID SysCanDataExchg::TotalPwrMsgRecvFcb(VOID)
{


}
#endif

/********************************************************************************
CfgMsgRecvProc-----����֡���մ���
                     	
Parameters:            pApdu:���յ�������֡
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanDataExchg::CfgMsgRecvProc(CAN_APDU_T *pApdu)
{
	INT16 i;
	INT16 i16NewMsg = NON_CFG_MSG;

	if (TIME_SYNC_SRVE_CODE == pApdu->u16ServiceCode)
	{
		i16NewMsg = SyncTimeMsgProc(pApdu);

		//ʱ��ͬ��������Ӧ
		return NON_CFG_MSG;
	}

	//�������������Ϣ��DSP��������������صĻ�Ӧ����
	for (i = 0; i < INV_CFG_FROM_MON_OBJ_SIZE; i++)
	{
		if (pApdu->u16ServiceCode == m_CfgMsgObj.FromMon[i].bSrvcCod)
		{
			i16NewMsg = CfgMsgFromMonProc(pApdu, i);
			return i16NewMsg;
		}
	}

	//��Ӧ��ز������������Ϣ�Ĳ�ѯ
	//����ؿ��Բ�ѯ��ǰ�����ò���ֵ
	for (i =0; i < INV_CFG_TO_MON_OBJ_SIZE; i++)
	{
		if (pApdu->u16ServiceCode == m_CfgMsgObj.ToMon[i].bSrvcCod)
		{
			i16NewMsg = CfgMsgToMonProc(pApdu, i);
			return i16NewMsg;
		}
	}
	
	//TBD
	return i16NewMsg;
}

/********************************************************************************
SyncMsgProc-----ͬ��֡����
                     	
Parameters:            pApdu:���յ�������֡
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanDataExchg::SyncMsgProc( IN CAN_APDU_T *pApdu)
{
//	INT16 i;
	INT16 i16NewMsg = NON_SYNC_MSG;
	
	if (MAC_ID_MON == pApdu->u16SourceMacId)
	{
		if (m_SyncFrameMonitor.usMonitorCnt == 0)
		{
			//�����ϵ��ʼ����ͬ��֡��ʱ��
			m_SyncFrameMonitor.usMonitorCnt = 1;
		}

		//��ʱ���Ѿ�����,�������ID���ͨ��,���Ӧͬ��֡

		//���ÿ��1S�·�һ��ͬ����Ϣ���󣬱�ģ�鿪ʼ����
		if ((m_SyncFrameMonitor.usMonitorNodeIdFlag == SYNC_FRAME_MONITORED)
			&& (1 != objMonInterface.m_st_IdErrorFlag.bUsIdNumExpire))
		{
			m_u16SyncFlag = TRUE;

			//�ñ�־
			m_StateMsgObj.SyncInfo.i16XmitActive = TRUE;
			m_StateMsgObj.SyncInfo.i16XmitIdx = 0;
			m_AlrmMsgObj.SyncInfo.i16XmitActive = TRUE;
			m_AlrmMsgObj.SyncInfo.i16XmitIdx = 0;
			m_AnalogSyncMsgObj.SyncInfo.i16XmitActive = TRUE;
			m_AnalogSyncMsgObj.SyncInfo.i16XmitIdx = 0;

			//�û����ڲ���״̬SYNC��־
			m_ParallelStateMsgObj.SyncInfo.i16XmitActive = TRUE;
			m_ParallelStateMsgObj.SyncInfo.i16XmitIdx = 0;

			m_ucSyncCfgReqFlag = TRUE;
			//ͬ��֡��Ӧ���
			pApdu->u16ServiceCode = 0x01;
			pApdu->u16Fragment = NONFRAG_MSG;
			pApdu->u16MsgClass = CAN_STATE_MSG_CLASS;
			pApdu->u16RsRq = RQ_MSG;//RS_MSG;
			pApdu->u16DestinationMacId = MAC_ID_BROADCAST;
			pApdu->u16SourceMacId = objMonInterface.m_i16wSerialNumSet_0;//LOCAL_NODE_ID;
		
		
			pApdu->u16ByteLength = 0;

			i16NewMsg = HAVE_SYNC_MSG;
		}
		
	}

	//����ͬ��֡
	else
	{
		//�ҵ�ID����ͬ�Ľڵ�,�����ٷ��κ���Ϣ
		if (pApdu->u16SourceMacId  ==  objMonInterface.m_i16wSerialNumSet_0)
		{
			//���˳�������־=0���ڼ���,=1���ټ�����ʼͨѶ,=0xff��������
			m_SyncFrameMonitor.usMonitorNodeIdFlag = SYNC_FRAME_MONITOR_ABORD;
			//ID�ų�ͻ,�ñ�־���ݸ��߼�ʹ��
			objMonInterface.m_st_IdErrorFlag.bUsIdNumOverlap = 1;
			//���ټ���
			m_SyncFrameMonitor.usMonitorCnt = SYNC_MONITOR_MAX_CNT;
		}

		//û��������ͬ�ڵ��ID
		else
		{
			//do nothing
		}
	}

	return i16NewMsg;
}

/********************************************************************************
MonitorSyncMsgProc-----ͬ��֡��ʼ��������
                     	
Parameters:            
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanDataExchg::MonitorSyncMsgProc( VOID)
{
	//�����������û������ͬ�Ľڵ�
	if (m_SyncFrameMonitor.usMonitorNodeIdFlag != SYNC_FRAME_MONITOR_ABORD)
	{
		//���ڼ���������
		if (SYNC_FRAME_MONITORED != m_SyncFrameMonitor.usMonitorNodeIdFlag )
		{
			//ÿ����ѭ��ά��������
			if (GetSyncMonitorCnt())
			{
				m_SyncFrameMonitor.usMonitorNodeIdFlag = SYNC_FRAME_MONITORED;

			}

			//������ʱδ��,��������
			else
			{
				//do nothing
			}
		}
	}

	//������������ͬ��ID�ڵ�
	else
	{
		//do nothing
	}

	if (m_SyncFrameMonitor.usMonitorNodeIdFlag == SYNC_FRAME_MONITORED)
	{
		
		//���������п�ʼ
		if ((m_BatchSetEvent == 0)
			&& (objMonInterface.m_SyncTime.u16HaveSyncClkFlag == 1))
		{
			m_BatchSetEvent = 1;
		}
	}
	
}

/**********************************************************************
GetSyncMonitorCnt-----��ȡ��ʱ������

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
INT16 SysCanDataExchg::GetSyncMonitorCnt(void)
{
	INT16 Ret=FALSE;
	
	//--1ms��ʱ��
	if (objSystem.m_st_wFlagSystem.bTimer1ms == 1)
	{
		//5ms��ʱ��
		//�����������������
		if (m_SyncFrameMonitor.usMonitorCnt)
		{
			if (++m_SyncFrameMonitor.usMonitorCnt >= SYNC_MONITOR_MAX_CNT)
			{
				m_SyncFrameMonitor.usMonitorCnt = SYNC_MONITOR_MAX_CNT;
				Ret = TRUE;
			}
		}

		else
		{
			//do nothing
		}
			
		
	}
	
	return Ret;
}

/********************************************************************************
SyncTimeMsgProc-----ʱ��ͬ������
                     	
Parameters:            pApdu:���յ�������֡
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanDataExchg::SyncTimeMsgProc( IN CAN_APDU_T *pApdu)
{
//	INT16 i;
	INT16 i16NewMsg = NON_CFG_MSG;
	
	if (MAC_ID_MON == pApdu->u16SourceMacId)
	{
		//��ͬ��ʱ�䱣�浽InterfaceMon�ӿ��ж����ʱ�������
		objMonInterface.m_SyncTime.u16TimeS_L= pApdu->pDataPtr[0].u16Word;
		objMonInterface.m_SyncTime.u16TimeS_H = pApdu->pDataPtr[1].u16Word;

		//ͬʱ��ʱ��ms��0���Ա����ʱ�俪ʼ���¼�ʱ
		//���Ҫ��ȷ��us��ɽ��˼���´�����ȡ�����ã�����0����
		objMonInterface.m_SyncTime.u16TimeMS = pApdu->pDataPtr[2].u16Word;
		//objMonInterface.m_SyncTime.u16TimeMS = 0;
		if (0 == objMonInterface.m_SyncTime.u16HaveSyncClkFlag)
		{
			objMonInterface.m_SyncTime.u16HaveSyncClkFlag = 1;
		}

		//֪ͨ
		objMonInterface.m_i16TimeSynCommand_0 = 1;
	}

	return i16NewMsg;
}


/********************************************************************************
CfgMsgFromMon-----������Ϣ���ô���
                     	
Parameters:            pApdu:���յ�������֡
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanDataExchg::CfgMsgFromMonProc(IN CAN_APDU_T *pApdu, IN UINT16 u16Idx)
{
	INT16 i16NewMsg = NON_CFG_MSG;

	INT16 i16NewCfgFromMon=0;
	UINT16 u16SrvCodeBak ;
	
	//������Ϣ���Լ�ص�Ӧ���������������Ϣ
	if (RS_MSG == pApdu->u16RsRq)
	{
		EndResendTimer();
		
		//���Լ������������Ϣ����
		if (TRUE == m_CfgMsgObj.FirstCfgFlag.bFromMon)
		{
			//��֤���õ�������
			if (m_CfgMsgIdxBak != u16Idx)
			{
				//���в���δ�����������һ��������������
				//δ����,�ط�

				CfgMsgRqPacked(pApdu, (m_CfgMsgIdxBak));
				/*pApdu->u16ByteLength = 0;
				pApdu->u16ServiceCode = m_CfgMsgObj.FromMon[u16Idx + 1].bSrvcCod;
				pApdu->u16Fragment = NONFRAG_MSG;
				pApdu->u16MsgClass = CAN_CFG_MSG_CLASS;
				pApdu->u16RsRq = RQ_MSG;
				pApdu->u16DestinationMacId = MAC_ID_MON;
				pApdu->u16SourceMacId = objMonInterface.m_i16wSerialNumSet_0;//LOCAL_SYS_CAN_ID;
				//pApdu ->pDataPtr[0] = *(m_CfgMsgObj.FromMon[u16Idx + 1].pData);
				//pApdu->pDataPtr[1] = SET_PARA_SUPPORT;
				*/
				i16NewMsg = HAVE_CFG_MSG;
				
			}
			else
			{
				m_CfgMsgIdxBak++;
				m_CfgMsgObj.FromMon[u16Idx].bCompleted = TRUE;
				m_ReqSetFlag = FALSE;

				if (SET_PARA_SUPPORT == pApdu->pDataPtr[(pApdu->u16ByteLength) /2 -1].u16Word)
				{
					if ((pApdu->u16ByteLength) == 4)
					{
						*(m_CfgMsgObj.FromMon[u16Idx].pData) = pApdu->pDataPtr[0].u16Word;
						
						i16NewCfgFromMon=1;
						u16SrvCodeBak = pApdu->u16ServiceCode;
					}
					else
					{
						//�ж��Ƿ�Ϊ��ͬ�����
						//pApdu->pDataPtr[0].u16WordΪ������
						*(m_CfgMsgObj.FromMon[u16Idx].pData) = pApdu->pDataPtr[1].u16Word;
						
						i16NewCfgFromMon=1;
						u16SrvCodeBak = pApdu->u16ServiceCode;
					}

					//LIUJ �������������
					if ((INV_SYS_CFG_FROM_MON_OBJ_SIZE -1) == u16Idx)
					{
						m_CfgMsgObj.FirstCfgFlag.bFromMon = FALSE;
						objSystem.m_st_wFlagSystem.bParamSetReq = 0;
						i16NewMsg = NON_CFG_MSG;
					}

					//���в���δ�����������һ��������������
					else
					{

						CfgMsgRqPacked(pApdu, (u16Idx +1));
						/*pApdu->u16ByteLength = 0;
						pApdu->u16ServiceCode = m_CfgMsgObj.FromMon[u16Idx + 1].bSrvcCod;
						pApdu->u16Fragment = NONFRAG_MSG;
						pApdu->u16MsgClass = CAN_CFG_MSG_CLASS;
						pApdu->u16RsRq = RQ_MSG;
						pApdu->u16DestinationMacId = MAC_ID_MON;
						pApdu->u16SourceMacId = objMonInterface.m_i16wSerialNumSet_0;//LOCAL_SYS_CAN_ID;
						//pApdu ->pDataPtr[0] = *(m_CfgMsgObj.FromMon[u16Idx + 1].pData);
						//pApdu->pDataPtr[1] = SET_PARA_SUPPORT;
						*/
						i16NewMsg = HAVE_CFG_MSG;
					}
				}

				else
				{
					//add not support process function
					i16NewMsg = NON_CFG_MSG;
				}
			}
		}

		//��������������ò��������������ʱ��DSP�������Ӧ��
		else
		{
			//֧�ִ�������ȡ�ô�����
			if (SET_PARA_SUPPORT == pApdu->pDataPtr[(pApdu->u16ByteLength) /2 -1].u16Word)
			{
				if ((pApdu->u16ByteLength) == 4)
				{
					*(m_CfgMsgObj.FromMon[u16Idx].pData) = pApdu->pDataPtr[0].u16Word;
					
					i16NewCfgFromMon=1;
					u16SrvCodeBak = pApdu->u16ServiceCode;
				}
				else
				{
					//�ж��Ƿ�Ϊ��ͬ�����
					//pApdu->pDataPtr[0].u16WordΪ������
					*(m_CfgMsgObj.FromMon[u16Idx].pData) = pApdu->pDataPtr[1].u16Word;
					i16NewCfgFromMon=1;
					u16SrvCodeBak = pApdu->u16ServiceCode;
				}
			}

			//����֧����񣬾�����Ӧ
			i16NewMsg = NON_CFG_MSG;
		}

		//������µĲ������ã��ø��±�־
		if (i16NewCfgFromMon == 1) 
		{
			//ϵͳ���ò���
			if (u16SrvCodeBak < 0x40||(u16SrvCodeBak == 0x6f))
			{
				objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.u16SetParamFresh = TRUE;
				objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh2 = TRUE;
			}
			//������ò���
			else
			{
				if ((0x69 == u16SrvCodeBak) || (0x6A == u16SrvCodeBak) || (0x6B == u16SrvCodeBak))
				{
					objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh |= uInvSetMsgBitSet[15];
				}
				else
				{
					objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh |= uInvSetMsgBitSet[u16Idx-28];
				}
				//TBD дEEPROM����
				
			}
		}

	}

	//TBD liuj ���������ȡ������Ϣ�Ĵ���20080524
	else 
	{
#if (0)
		//��������Ҫ��ȡ����������Ϣ,��Ϊ�ֶ�֡�ϴ�
		if (0x70 == pApdu->u16ServiceCode )
		{

		}
#endif		
//		else
//		{
		
			//������ϢӦ��֡���
			CfgMsgRsPacked(pApdu, u16Idx );
			
			//��ػ�ȡ��Ϣ��׼����ϢӦ������
			i16NewMsg = HAVE_CFG_MSG;
	
//		}
		#if (0)
		//��֧��
		else
		{
			//add not support process function
			i16NewMsg = NON_CFG_MSG;
		}

		#endif
	
	}

	return i16NewMsg;

}

/********************************************************************************
GetFaultData-----������Ϣ�ϴ�����׼��
                     	
Parameters:            pApdu:���յ�������֡
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanDataExchg::GetFaultData(OUT CAN_APDU_T *pApdu)
{
	INT16 i16NewMsg = NON_CFG_MSG;
	
	//���ϼ�¼������������׼����
	if (objEEPROM.m_i16FaultReadReady)
	{
		pApdu->u16ByteLength = 256;
		pApdu->u16DestinationMacId = MAC_ID_MON;
		pApdu->u16Fragment = FRAG_MSG;
		pApdu->u16MsgClass = CAN_ALRM_MSG_CLASS;
		pApdu->u16RsRq = RS_MSG;
		pApdu->u16ServiceCode = m_ReadFaultNo;
		pApdu->u16SourceMacId = objMonInterface.m_i16wSerialNumSet_0;//LOCAL_SYS_CAN_ID;

		objEEPROM.m_i16FaultReadReady = 0;

		i16NewMsg = HAVE_CFG_MSG;
	}

	return i16NewMsg;
	
}

/********************************************************************************
CfgMsgToMonProc-----������Ϣ��ѯ����,�������
                     	
Parameters:            pApdu:���յ�������֡
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanDataExchg::CfgMsgToMonProc(IN CAN_APDU_T *pApdu, IN UINT16 u16Idx)
{
	UINT16 i;
	INT16 i16NewMsg = NON_CFG_MSG;

	//���������Ϣ��ѯ
	if (RQ_MSG == (pApdu->u16RsRq))
	{
		if (TRUE == m_CfgMsgObj.FirstCfgFlag.bToMon)
		{
			m_CfgMsgObj.ToMon[u16Idx].bCompleted =  TRUE;

			//����:������Ϣ��ѯ�Ƿ�Ӧ�����ۺ�ʱ�����Բ飬����
			//��־m_CfgMsgObj.ToMon[i].bCompleted��Ӧ�������ã�û������?TBD liuj 20060524
			for (i = 0; i < INV_CFG_TO_MON_OBJ_SIZE; i++)
			{
				if (FALSE == m_CfgMsgObj.ToMon[i].bCompleted)
				{
					break;
				}
			}

			if ( INV_CFG_TO_MON_OBJ_SIZE == i) 
			{
				CfgMsgRqPacked(pApdu, 0);
				//pApdu ->pDataPtr[0] = *(m_CfgMsgObj.FromMon[0].pData);
				//pApdu->pDataPtr[1] = SET_PARA_SUPPORT;
				i16NewMsg = HAVE_CFG_MSG;
				m_CfgMsgObj.FirstCfgFlag.bToMon = FALSE;
			}
		}
		
		pApdu->u16ByteLength = m_CfgMsgObj.ToMon[u16Idx].bMsgLen;
		pApdu->u16ServiceCode = m_CfgMsgObj.ToMon[u16Idx].bSrvcCod;
		pApdu->u16Fragment = m_CfgMsgObj.ToMon[u16Idx].bFragMsg;
		pApdu->u16MsgClass = CAN_CFG_MSG_CLASS;
		pApdu->u16RsRq = RS_MSG;
		pApdu->u16DestinationMacId = MAC_ID_MON;
		pApdu->u16SourceMacId = objMonInterface.m_i16wSerialNumSet_0;//LOCAL_SYS_CAN_ID;

		if (NONFRAG_MSG == pApdu->u16Fragment)
		{
			for (i = 0; i < (pApdu->u16ByteLength / 2); i++)
			{
#ifdef TEST_MODE
				pApdu->pDataPtr[i].u16Word = 0x01;
#else
				pApdu->pDataPtr[i].u16Word = m_CfgMsgObj.ToMon[u16Idx].pData[i];
#endif
			}	
		}
		
		i16NewMsg = HAVE_CFG_MSG;
	}

	else
	{
		i16NewMsg = NON_CFG_MSG;
	}

	return i16NewMsg;
}


/********************************************************************************
CfgMsgRqPacked-----������Ϣ����֡���
                     	
Parameters:            
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanDataExchg::CfgMsgRqPacked(CAN_APDU_T *pApdu, INT16 i16Idx)
{
	pApdu->u16ByteLength = 0;
	pApdu->u16ServiceCode = m_CfgMsgObj.FromMon[i16Idx].bSrvcCod;
	pApdu->u16Fragment = NONFRAG_MSG;
	pApdu->u16MsgClass = CAN_CFG_MSG_CLASS;
	pApdu->u16RsRq = RQ_MSG;
	pApdu->u16DestinationMacId = MAC_ID_MON;
	pApdu->u16SourceMacId = objMonInterface.m_i16wSerialNumSet_0;//LOCAL_SYS_CAN_ID;

	//���¿�ʼ��ʱ�Ƿ���Ҫ�ط�
	RestartResendTimer();
}


VOID SysCanDataExchg::CfgMsgRsPacked(CAN_APDU_T *pApdu, INT16 i16Idx)
{
	UINT i;
	
	pApdu->u16ServiceCode = m_CfgMsgObj.FromMon[i16Idx].bSrvcCod;
	pApdu->u16Fragment = NONFRAG_MSG;
	pApdu->u16MsgClass = CAN_CFG_MSG_CLASS;
	pApdu->u16RsRq = RS_MSG;
	pApdu->u16DestinationMacId = MAC_ID_MON;
	pApdu->u16SourceMacId = objMonInterface.m_i16wSerialNumSet_0;//LOCAL_SYS_CAN_ID;
	
	
	pApdu->u16ByteLength = m_CfgMsgObj.FromMon[i16Idx].bMsgLen;;
	

	for (i=0; i<(pApdu->u16ByteLength/2); i++)
	{
		pApdu->pDataPtr[i].u16Word = m_CfgMsgObj.FromMon[i16Idx].pData[i];
	}

}

/********************************************************************************
CtrlMsgToMonProc-----������Ϣ��ѯ����
                     	
Parameters:            pApdu:���յ�������֡
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanDataExchg::CtrlMsgRecvProc(IN CAN_APDU_T *pApdu)
{
	INT16 i;

	for (i = 0; i < (sizeof(CanCtrlMsgObjs) / sizeof(CAN_CTRL_MSG_T)); i++)
	{
		if (m_CtrlMsgObj[i].bSrvcCode == pApdu->u16ServiceCode)
		{
			if (CTRL_INV_FLASHUPDATE_SRVE_CODE == pApdu->u16ServiceCode)
			{
				m_u16FlashupdateMsg = pApdu->pDataPtr[0].u16Word;
				m_u16FlashupdateSecnodMsg = pApdu->pDataPtr[1].u16Word;
			}
			if (NULL != m_CtrlMsgObj[i].MsgRecvFcb)
			{
				(this->*m_CtrlMsgObj[i].MsgRecvFcb)();
			}

			break;
		}
	}

	if (i == (sizeof(CanCtrlMsgObjs) / sizeof(CAN_CTRL_MSG_T)))
	{
		//wrong msg process!
		#if (0)
		//����һ������֡���Զ����servcode = 0xaa
		if (pApdu->u16ServiceCode = 0xaa)
		{
			//pDataPtr[0]Ϊ��Ҫ��ѯ��servcode
			pApdu->pDataPtr[0] = 
			UINT16 *pAnswer = &objMonInterface.m_st_CtrlCmd_D ;
			
		}
		#endif
	}
	
	//TBD
	return 0;
}


/********************************************************************************
AlrmMsgRecvProc-----��ȡ�������ݴ���
                     	
Parameters:            pApdu:���յ�������֡
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanDataExchg::AlrmMsgRecvProc(IN CAN_APDU_T *pApdu)
{
//	INT16 i;
//	INT16 ret = HAVE_FAULT_MSG;
	
	//���ϼ�¼Ŀǰֻ��������
	#define FAULT1_SRV_CODE 0x01
	#define FAULT2_SRV_CODE 0x02
	#define FAULT3_SRV_CODE 0x03
	
	m_ReadFaultNo = 0;
	
	switch (pApdu->u16ServiceCode)
	{
		case FAULT1_SRV_CODE:
			objEEPROM.m_i16FaultReadComand = 1;
			m_ReadFaultNo =1;
			objEEPROM.App_FaultReadTaskInit(m_ReadFaultNo);
			break;
			
		case FAULT2_SRV_CODE:
			objEEPROM.m_i16FaultReadComand = 1;
			m_ReadFaultNo =2;
			objEEPROM.App_FaultReadTaskInit(m_ReadFaultNo);
			break;
			
		case FAULT3_SRV_CODE:
			objEEPROM.m_i16FaultReadComand = 1;
			m_ReadFaultNo =3;
			objEEPROM.App_FaultReadTaskInit(m_ReadFaultNo);
			break;
		default:
			m_ReadFaultNo =0;
			//ret = NON_FAULT_MSG;
			break;
	}

	
	return 0;
}

/********************************************************************************
StateMsgRecvProc-----״̬��Ϣ�е�ͬ��֡��Ϣ����
                     	
Parameters:            pApdu:���յ�������֡
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanDataExchg::StateMsgRecvProc(IN CAN_APDU_T *pApdu)
{

	INT16 ret = NON_SYNC_MSG;
	
	//ͬ��֡�����ÿ��1S����ģ�鷢����Ϣ
	if (SYNC_MSG_SRVC_CODE == pApdu->u16ServiceCode )
	{
		ret = SyncMsgProc(pApdu);
	}

	//�����ڲ������ݽ��մ���
	else if (PARALLEL_STATE_MSG_SRVC_CODE == pApdu->u16ServiceCode)
	{
		ret = ParallelMsgRecvProc(pApdu);
	}

	//��������·�����ݽ��մ���
	else if (BYPMAINT_STATE_MSG_SRVC_CODE == pApdu->u16ServiceCode)
	{
		ret = BypMaintMsgRecvProc(pApdu);
	}

	//���ÿ��1S�´���ʵʱ��Ϣ
	else if (MON_REALTIME_MSG_SRVC_CODE == pApdu->u16ServiceCode)
	{
		ret = MonRealMsgRecvProc(pApdu);
	}

	//���ܼ�״̬��Ϣ����
	else if (RACK_STATE_MSG_SRVC_CODE == pApdu->u16ServiceCode)
	{
		ret = RackStateMsgRecvProc(pApdu);
	}

	//����������������ɢ�ź���Ϣ,Ŀǰֻ��������·�����ź�
	else if ((REC_DISCRETE_MSG_SRVC_CODE == pApdu->u16ServiceCode)
		    &&(MODULE_NUM == 2))//���20K��ͬʱ�߱�������·�Ĺ���
	{
		ret = RecDiscreteMsgRecvProc(pApdu);
	}
	
	else
	{
		//do nothing
	}

	
	//TBD
	return ret;
}

/********************************************************************************
BypMaintMsgRecvProc-----��������·�����ݽ��մ���
                     	
Parameters:            pApdu:���յ�������֡
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanDataExchg::BypMaintMsgRecvProc(IN CAN_APDU_T *pApdu)
{
	INT16 i16NewMsg = NON_SYNC_MSG;
	
	if (pApdu->u16SourceMacId == MAC_ID_BYP)
	{
		
		//��������
		objMonInterface.m_st_wBypBreakData.Word1_U.u16Word1All = pApdu->pDataPtr[0].u16Word;

	}

	return i16NewMsg;
}


/********************************************************************************
RackStateMsgRecvProc-----����������·���ܼ�״̬��Ϣ���մ���
                     	
Parameters:            pApdu:���յ�������֡
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanDataExchg::RackStateMsgRecvProc(IN CAN_APDU_T *pApdu)
{
	INT16 i16NewMsg = NON_SYNC_MSG;
	
	if (pApdu->u16SourceMacId == MAC_ID_BYP)
	{
		
		//��������
		objMonInterface.m_st_wRackStateMsg.Word1_U.u16Word1All = pApdu->pDataPtr[0].u16Word;
		objMonInterface.m_st_wRackStateMsg.Word2_U.u16Word2All = pApdu->pDataPtr[1].u16Word;


	}

	return i16NewMsg;
}

/********************************************************************************
ParallelMsgRecvProc-----�����ڲ������ݽ��մ���
                     	
Parameters:            pApdu:���յ�������֡
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanDataExchg::ParallelMsgRecvProc(IN CAN_APDU_T *pApdu)
{
	INT16 i16NewMsg = NON_SYNC_MSG;
	
	if ((pApdu->u16SourceMacId >= MAC_ID_MOD1_INV)
		&& (pApdu->u16SourceMacId <= MAC_ID_MOD10_INV) )
	{
		UINT16 u16Idx = pApdu->u16SourceMacId-MAC_ID_MOD1_INV;

		//��������
		objMonInterface.m_st_wParallelData.ParallelState[u16Idx].Word1_U.u16Word1All
			= pApdu->pDataPtr[0].u16Word;

		//ά����Ӧ����
		MaintParallelData();
		

	}

	return i16NewMsg;
}

/********************************************************************************
MaintParallelData-----�����ڲ�������ά������
                     	
Parameters:           
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanDataExchg::MaintParallelData(VOID)
{
	INT16 iNumInv = 0;
	INT16 iNumOn = 0;
	INT16 iNumUPS = 0;
	INT16 iNumBattWarn = 0;
	INT16 iNumBattInv = 0;
	INT16 iParaWarnFlag = 0;
	INT16 iNumBp = 0;
//	INT16 iCapacitySumBasic = 0;
//	INT16 iCapacitySum_InvOn = 0;
//	INT16 iCapacitySum_Invmod = 0;
	
	//���㲢��ϵͳ����ܿ���̨�����ܹ���̨�� 
	for(UINT16 i=0;i<ParaNumCNST;i++)
	{    
		if ( objMonInterface.m_st_wParallelData.ParallelState[i].Word1_U.tWord1Bits.bInvOn != 0 )
		{
			iNumInv++;					//��俪����̨��
		}
		if ( ( objMonInterface.m_st_wParallelData.ParallelState[i].Word1_U.tWord1Bits.bInvPowerOn ) != 0 )
		{
			iNumOn ++;					//��乩����̨��
		}
		if ( ( objMonInterface.m_st_wParallelData.ParallelState[i].Word1_U.tWord1Bits.bBattCapAlarm  ) != 0 )
		{
			iNumBattWarn ++;			//����ϵͳ��������澯��̨��
		}
		if ( ( objMonInterface.m_st_wParallelData.ParallelState[i].Word1_U.tWord1Bits.bBattInvPower ) != 0 )
		{
			iNumBattInv ++;				//����ϵͳ�����乩����̨��
		}
		if ( ( objMonInterface.m_st_wParallelData.ParallelState[i].Word1_U.tWord1Bits.bSingleOnLine ) != 0 )
		{
			iNumUPS ++;					//����ϵͳʵ���ϵ���̨��
		}

		
		//��·�ϵ�̨��
		if ( objMonInterface.m_st_wParallelData.ParallelState[i].Word1_U.tWord1Bits.bBypPowerOn )
		{
			iNumBp ++;					//����ϵͳ��·������̨��
		}

		// ���㴦����乩��״̬�µĸ�̨������GrandKVA֮��
		//TBD,�������ǲ�ͬ����ʱ�账��
	#if (0)
		if((objMonInterface.m_st_wParallelData.ParallelState[i].Word1_U.tWord1Bits.bInvPowerOn !=0)
		{
			iSumOfInvONCapacity += sParaData[i].m_i16wGrantKVA;
		}
	

		// ���㴦�ڵ�����״̬�µĸ�̨������GrandKVA֮��;
		if((sParaData[i].uSystemFlag & SBIT3)!=0)
		{
			iSumOfBattInvCapacity += sParaData[i].m_i16wGrantKVA;
		}


		// ���㴦�ڵ��Ԥ�澯״̬�µĸ�̨������GrandKVA֮��;
		if((sParaData[i].uSystemFlag & SBIT2)!=0)
		{
			iSumOfBattWarCapacity += sParaData[i].m_i16wGrantKVA;
		}
	
			

		if ( ( sParaData[i].uSystemFlag & SBIT0 ) != 0 )
		{
			iCapacitySum_InvOn = iCapacitySum_InvOn + sParaData[i].m_i16wGrantKVA;

			//��ͬ��������
			if (iMaxCapacityInv < sParaData[i].m_i16wGrantKVA)
			{
				iMaxCapacityInv = sParaData[i].m_i16wGrantKVA;	
			}

		}

		if ( ( sParaData[i].uSystemFlag & SBIT1 ) != 0 )
		{
			iCapacitySum_Invmod = iCapacitySum_Invmod + sParaData[i].m_i16wGrantKVA;

			//��ͬ��������
			if (iMaxCapacityOn < sParaData[i].m_i16wGrantKVA)
			{
				iMaxCapacityOn = sParaData[i].m_i16wGrantKVA;	
			}

			//������͵Ĳ�ͬ��������
			iKCapSum_Invmod += (((INT32)sParaData[i].m_i16wGrantKVA)<<12)/(sParaData[i].m_i16ModelKVA);

		}

		//����ϵͳʵ���ϵ�
		if ( ( objMonInterface.m_st_wParallelData.ParallelState[i].Word1_U.tWord1Bits.bSingleOnLine) != 0 ) 
		{
			if ((sParaData[i].m_i16wGrantKVA > objMonInterface.m_i16GrantKVA)
				||((sParaData[i].m_i16wGrantKVA == objMonInterface.m_i16GrantKVA) && (i < objMonInterface.m_i16wSerialNumSet_0)))
			{
				iInvMasterPriorINT16er++;
			}

			iCapacityOn[j] = sParaData[i].m_i16wGrantKVA;
			j++;
			
		}
	#endif
						
		/* �ϳɲ���ϵͳ�ĸ澯��־ */
		UINT16 uTmpBuff = ( objMonInterface.m_st_wParallelData.ParallelState[i].Word1_U.u16Word1All >> 4 ) & 0xF;
		iParaWarnFlag = iParaWarnFlag | uTmpBuff;
		
	}

	//���µ�ǰֵ
	//�������ڵ���俪��̨��
	objMonInterface.m_st_wParallelData.iNumInv = objPowCANSched.m_i16InvTotalNum;
	//�����ܲ���ϵͳ ��俪��̨��--�����������ܵ�̨��
	objMonInterface.m_st_wParallelData.iTotalNumInv = objPowCANSched.m_i16InvTotalNum;
	//�������ڵ���乩��̨��
	objMonInterface.m_st_wParallelData.iNumOn = objPowCANSched.m_i16OnTotalNum;
	//�����ܲ�����乩��̨��--�����������ܵ�̨��
	objMonInterface.m_st_wParallelData.iTotalNumOn = objPowCANSched.m_i16OnTotalNum;
	objMonInterface.m_st_wParallelData.iNumBattWarn = iNumBattWarn;
	objMonInterface.m_st_wParallelData.iNumBattInv = iNumBattInv;
	objMonInterface.m_st_wParallelData.iNumUPS = iNumUPS;
	objMonInterface.m_st_wParallelData.iNumBp = iNumBp;
	
	objMonInterface.m_st_wParallelData.iParaWarnFlag = iParaWarnFlag;
	
}

/********************************************************************************
AnalogMsgRecvProc-----ģ�������մ���
                     	
Parameters:            pApdu:���յ�������֡
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanDataExchg::AnalogMsgRecvProc(IN CAN_APDU_T *pApdu)
{
	INT16 i, j;

	
	for (i = 0; i < (sizeof (m_AnalogRecvMsgObj.MsgObj) / sizeof (ANALOG_MSG_OBJ_T)); i++)
	{
			
		if (m_AnalogRecvMsgObj.MsgObj[i].bSrvcCod == pApdu->u16ServiceCode)
		{
			for (j = 0; j < ((pApdu->u16ByteLength ) / 2); j++)
			{
				m_AnalogRecvMsgObj.MsgObj[i].pData[j] = pApdu->pDataPtr[j].u16Word;
			} 
	
		}

	}

	return 0;
	
}

/********************************************************************************
FragXmitResult-----�ֶ�֡���ͽ������
                     	
Parameters:            pApdu:���յ�������֡
				 i16Result:���ͽ��
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanDataExchg::FragXmitResult(CAN_APDU_T *pApdu, INT16 i16Result)
{
	switch(pApdu->u16MsgClass)
	{
		case CAN_CFG_MSG_CLASS:
			break;

		case CAN_ALRM_MSG_CLASS:
			//TBD
			//�¼���¼�͹��ϵ�
			break;

		default:
			break;

	}
	
	//TBD
	return 0;
}

/********************************************************************************
FragMsgPacked-----�ֶ�֡���
                     	
Parameters:            pApdu:���յ�������֡
				 
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanDataExchg::FragMsgPacked(CAN_APDU_T *pApdu)
{
	
	switch(pApdu->u16MsgClass)
	{
		case CAN_CFG_MSG_CLASS:
			CfgFragMsgPacked(pApdu);
			break;

		case CAN_ALRM_MSG_CLASS:
			//���ϵ�
			FaultFragMsgPacked(pApdu);
			break;

		default:
			break;

	}

	//TBD
	return 0;
}

/********************************************************************************
FragMsgPacked-----�ֶ�֡���
                     	
Parameters:            pApdu:���յ�������֡
				 
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanDataExchg::CfgFragMsgPacked(CAN_APDU_T *pApdu)
{
	INT16 i, j, ret = 0;

	for (i = 0; i < INV_CFG_TO_MON_OBJ_SIZE; i++)
	{
		if (m_CfgMsgObj.ToMon[i].bSrvcCod == pApdu->u16ServiceCode)
		{
			for (j = 0; j < (pApdu->u16ByteLength / 2); j++)
			{
				pApdu->pDataPtr[j].u16Word = m_CfgMsgObj.ToMon[i].pData[j];
			}

		}
	}

	if (i == INV_CFG_TO_MON_OBJ_SIZE)
	{
		//
	}
	
	//TBD
	return ret;

}

/********************************************************************************
FaultFragMsgPacked-----�ֶ�֡���
                     	
Parameters:            pApdu:���յ�������֡
				 
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanDataExchg::FaultFragMsgPacked(CAN_APDU_T *pApdu)
{
	INT16 i,  ret = 0;

	
	for (i = 0; i< (pApdu->u16ByteLength / 2); i++)
	{
		pApdu->pDataPtr[i].u16Word =u16FaultRecordRead[i];// objMonInterface.m_u16_FaultRecordRead[i];
	}

	return ret;

}

//mask REC ctrl
//#if (0)


/********************************************************************************
BattSelfChk20Percent-----20%�ŵ��Լ�
                     	
Parameters:           
				 
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanDataExchg::BattSelfChk20Percent(VOID)
{
	objMonInterface.m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16SelfChk20PercentCmd = 1;
}

/********************************************************************************
BattSelfChkDeep-----��ȷŵ��Լ�
                     	
Parameters:           
				 
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanDataExchg::BattSelfChkDeep(VOID)
{
	objMonInterface.m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16SelfChkDeepCmd = 1;
}

/********************************************************************************
BattSelfChkEnd-----�ŵ��Լ����
                     	
Parameters:           
				 
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanDataExchg::BattSelfChkEnd(VOID)
{
	objMonInterface.m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16SelfChkEndCmd = 1;
}

//#endif

/********************************************************************************
BattEodTurnOff-----��ص�ѹ�͹ػ�
                     	
Parameters:           
				 
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanDataExchg::BattEodTurnOff(VOID)
{
	objMonInterface.m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16EodCmd = 1;
}

/********************************************************************************
BattTrip-----���trip
                     	
Parameters:           
				 
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanDataExchg::BattTrip(VOID)
{
	objMonInterface.m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16BattTripCmd = 1;
}

/********************************************************************************
OutputEnalbeCmd-----���ʹ��
                     	
Parameters:           
				 
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanDataExchg::OutputEnableCmd(VOID)
{
	objMonInterface.m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16OutputEnableCmd = 1;
}

/********************************************************************************
OutputDisalbeCmd-----�����ֹ
                     	
Parameters:           
				 
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanDataExchg::OutputDisableCmd(VOID)
{
	objMonInterface.m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16OutputDisableCmd = 1;
}

/********************************************************************************
BattGndPtcCmd-----��ؽӵر���
                     	
Parameters:           
				 
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanDataExchg::BattGndPtcCmd(VOID)
{
	objMonInterface.m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16BattGndPtcCmd = 1;
}


/********************************************************************************
FaultClrCmd-----�������
                     	
Parameters:           
				 
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanDataExchg::FaultClrCmd(VOID)
{
	objMonInterface.m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16FaultClrCmd = 1;
	//read for Fault clear Event set
	if (FALSE == m_ucFaultClearFlag )
	{
		m_ucFaultClearFlag = TRUE;
	}
}

/********************************************************************************
MaualStartupCmd-----�ֶ���������
                     	
Parameters:           
				 
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanDataExchg::MaualStartupCmd(VOID)
{
	objMonInterface.m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16ManualInvOnCmd = 1;
}

/********************************************************************************
MaualShutCmd-----�ֶ��ػ�����
                     	
Parameters:           
				 
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanDataExchg::MaualShutCmd(VOID)
{
	objMonInterface.m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16ManulaInvOffCmd = 1;
}

/********************************************************************************
EpoCmd-----EPO����
                     	
Parameters:           
				 
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanDataExchg::EpoCmd(VOID)
{
	objMonInterface.m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16EpoCmd = 1;
}

/********************************************************************************
AbnormalShutConfirmCmd-----�쳣�ػ�ȷ������
                     	
Parameters:           
				 
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanDataExchg::AbnormalShutConfirmCmd(VOID)
{
	objMonInterface.m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16AbnrmlShutConfirmCmd = 1;
}

/********************************************************************************
AbnormalShutCancelCmd-----�쳣�ػ�ȡ������
                     	
Parameters:           
				 
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanDataExchg::AbnormalShutCancelCmd(VOID)
{
	objMonInterface.m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16AbnrmllShutCancelCmd = 1;
}

/********************************************************************************
IndirectSwitchCmd-----����л�����
                     	
Parameters:           
				 
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanDataExchg::IndirectSwitchCmd(VOID)
{
	objMonInterface.m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16IndirectSwitchCmd = 1;
}

/********************************************************************************
RecFlashUpdate-----����Flash update����
                     	
Parameters:           
				 
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanDataExchg::RecFlashUpdate(VOID)
{
	objMonInterface.m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16RecFwUpdateCmd = 1;
}

/********************************************************************************
InvFlashUpdate-----���Flash update����
                     	
Parameters:           
				 
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanDataExchg::InvFlashUpdate(VOID)
{
	if ((m_u16FlashupdateMsg>>(objMonInterface.m_i16wSerialNumSet_0-0x10+1))&0x01)
	{
		if (0x4a25 == m_u16FlashupdateSecnodMsg)
		{
			objMonInterface.m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16InvFwUpdateCmd = 1;

			m_u16FlashupdateSecnodMsg = 0;

		}
		
	}
}

/********************************************************************************
BypFlashUpdate-----��·Flash update����
                     	
Parameters:           
				 
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanDataExchg::BypFlashUpdate(VOID)
{
	objMonInterface.m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16BypFwUpdateCmd = 1;
}


VOID SysCanDataExchg::GenBypStateEvent(VOID)
{
	unsigned char i;
		//--3.״̬�¼�COV
		if (m_StateMsgObj.CovObj[MODULE_BYP].bCovFlag == TRUE)
		{
			
			
			for (i=0;i<(sizeof(BypStateEventIdObj)/sizeof(EVENT_MSG_T));i++)
			{
				//��һ��״̬��
				if (BypStateEventIdObj[i].bEventInWord == 0)
				{
					//�б仯,������¼
					if ( ((m_StateMsgObj.Msg2.Word1_U.u16Word1All>>BypStateEventIdObj[i].bOffSet) & (BypStateEventIdObj[i].uMaskVal))
						!= (((m_BypStateMsgBak.Word1_U.u16Word1All)>>BypStateEventIdObj[i].bOffSet )& (BypStateEventIdObj[i].uMaskVal)))
					{
						if (((m_StateMsgObj.Msg2.Word1_U.u16Word1All>>BypStateEventIdObj[i].bOffSet) & (BypStateEventIdObj[i].uMaskVal))
							== (BypStateEventIdObj[i].uSetVal) )
						//set event
						{
							m_BypStateEventObj.EventLog[i].u16ExEventID = ((BypStateEventIdObj[i].uEventID) | (EVENT_SET)) ;
							m_BypStateEventObj.CovObj[i].bCovFlag = TRUE;
							m_BypStateEventObj.CovObj[i].bXmitActive = TRUE;
							m_BypStateEventObj.EventLog[i].u16Second = objMonInterface.m_u16TimeLabel2_0;
							m_BypStateEventObj.EventLog[i].u16Millisecond = objMonInterface.m_u16TimeLabel1_0;
						}

						//clear event
						else if (((m_BypStateMsgBak.Word1_U.u16Word1All>>BypStateEventIdObj[i].bOffSet) & (BypStateEventIdObj[i].uMaskVal))
							== (BypStateEventIdObj[i].uSetVal) )
						{
							m_BypStateEventObj.EventLog[i].u16ExEventID = ((BypStateEventIdObj[i].uEventID) & (EVENT_CLEAR)) ;
							m_BypStateEventObj.CovObj[i].bCovFlag = TRUE;
							m_BypStateEventObj.CovObj[i].bXmitActive = TRUE;
							m_BypStateEventObj.EventLog[i].u16Second = objMonInterface.m_u16TimeLabel2_0;
							m_BypStateEventObj.EventLog[i].u16Millisecond = objMonInterface.m_u16TimeLabel1_0;
						}
						
						else
						{
							
						}

						//TBD ���¼�д��EEPROM��ػ���������liuj
						
					}
				}

				else
				{
					//do nothing
				}
			}
		

			m_BypStateMsgBak = objMonInterface.m_st_BypSendMonitor.m_st_BypBreakerStatus;
		
		}

		//FAULT CLEAR
		//Fault clear
		if (TRUE == m_ucFaultClearFlag )
		{
			m_ucFaultClearFlag = FALSE;
			//m_StateMsgObj.CovObj[0].bCovFlag = TRUE;

			//֮ǰ���й�������¼�,�Ȳ���һ�����������ʧ�¼�
			if (m_BypStateEventObj.CovObj[BYP_STATE_EVENT_OBJ_SIZE-1].bXmitActive == TRUE)
			{
				m_BypStateEventObj.CovObj[BYP_STATE_EVENT_OBJ_SIZE-1].bXmitActive = FALSE;
				m_BypStateEventObj.CovObj[BYP_STATE_EVENT_OBJ_SIZE-2].bCovFlag = TRUE;
				m_BypStateEventObj.CovObj[BYP_STATE_EVENT_OBJ_SIZE-2].bXmitActive = TRUE;
				m_BypStateEventObj.EventLog[BYP_STATE_EVENT_OBJ_SIZE-2].u16Second = objMonInterface.m_u16TimeLabel2_0;
				m_BypStateEventObj.EventLog[BYP_STATE_EVENT_OBJ_SIZE-2].u16Millisecond = objMonInterface.m_u16TimeLabel1_0;
				m_BypStateEventObj.EventLog[BYP_STATE_EVENT_OBJ_SIZE-2].u16ExEventID = ((BypStateEventIdObj[BYP_STATE_EVENT_OBJ_SIZE-2].uEventID) & (EVENT_CLEAR)) ;
			}

			//��������¼�����
			m_BypStateEventObj.CovObj[BYP_STATE_EVENT_OBJ_SIZE-1].bCovFlag = TRUE;
			m_BypStateEventObj.CovObj[BYP_STATE_EVENT_OBJ_SIZE-1].bXmitActive = TRUE;
			m_BypStateEventObj.EventLog[BYP_STATE_EVENT_OBJ_SIZE-1].u16Second = objMonInterface.m_u16TimeLabel2_0;
			m_BypStateEventObj.EventLog[BYP_STATE_EVENT_OBJ_SIZE-1].u16Millisecond = objMonInterface.m_u16TimeLabel1_0;
			m_BypStateEventObj.EventLog[BYP_STATE_EVENT_OBJ_SIZE-1].u16ExEventID = ((BypStateEventIdObj[BYP_STATE_EVENT_OBJ_SIZE-1].uEventID) | (EVENT_SET)) ;
		}
}

VOID SysCanDataExchg::GenBypAlarmEvent(VOID)
{
		unsigned char i;
		//--4.�澯�¼�COV
		if (m_AlrmMsgObj.CovObj[MODULE_BYP].bCovFlag == TRUE)
		{
			for (i=0;i<(sizeof(BypAlarmEventIdObj)/sizeof(EVENT_MSG_T));i++)
			{
				//�б仯,�����¼�
				//�澯��1
				if (BypAlarmEventIdObj[i].bEventInWord == 0)
				{
					if ( ((m_AlrmMsgObj.Msg2.Word1_U.u16Word1All >>(BypAlarmEventIdObj[i].bOffSet)) & BypAlarmEventIdObj[i].uMaskVal)
						!= (((m_BypAlarmMsgBak.Word1_U.u16Word1All)>>(BypAlarmEventIdObj[i].bOffSet)) & BypAlarmEventIdObj[i].uMaskVal) )
					{
						//set
						if (((m_AlrmMsgObj.Msg2.Word1_U.u16Word1All>>BypAlarmEventIdObj[i].bOffSet) & (BypAlarmEventIdObj[i].uMaskVal))
							== (BypAlarmEventIdObj[i].uSetVal) )
						{
							m_BypAlarmEventObj.EventLog[i].u16ExEventID = ((BypAlarmEventIdObj[i].uEventID) | (EVENT_SET)) ;
							m_BypAlarmEventObj.CovObj[i].bCovFlag = TRUE;
							m_BypAlarmEventObj.CovObj[i].bXmitActive = TRUE;
							m_BypAlarmEventObj.EventLog[i].u16Second = objMonInterface.m_u16TimeLabel2_0;
							m_BypAlarmEventObj.EventLog[i].u16Millisecond = objMonInterface.m_u16TimeLabel1_0;
						}
						//clear
						else if (((m_BypAlarmMsgBak.Word1_U.u16Word1All>>BypAlarmEventIdObj[i].bOffSet) & (BypAlarmEventIdObj[i].uMaskVal))
							== (BypAlarmEventIdObj[i].uSetVal) )
						{
							m_BypAlarmEventObj.EventLog[i].u16ExEventID = ((BypAlarmEventIdObj[i].uEventID) & (EVENT_CLEAR)) ;
							m_BypAlarmEventObj.CovObj[i].bCovFlag = TRUE;
							m_BypAlarmEventObj.CovObj[i].bXmitActive = TRUE;
							m_BypAlarmEventObj.EventLog[i].u16Second = objMonInterface.m_u16TimeLabel2_0;
							m_BypAlarmEventObj.EventLog[i].u16Millisecond = objMonInterface.m_u16TimeLabel1_0;
						}
						
						else
						{
							
						}
					}
				}
				

				//�����澯
				else
				{

				}
				
				
			}

			//TBD ���¼�д��EEPROM��ػ���������liuj

			m_BypAlarmMsgBak = objMonInterface.m_st_BypSendMonitor.m_st_BypWarn;
			
		}
}

/********************************************************************************
RecDiscreteMsgRecvProc-----������������ɢ�ź����ݽ��մ���
                     	
Parameters:            pApdu:���յ�������֡
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanDataExchg::RecDiscreteMsgRecvProc(IN CAN_APDU_T *pApdu)
{
	INT16 i16NewMsg = NON_SYNC_MSG;
	
	if ((pApdu->u16SourceMacId >= MAC_ID_MOD1_REC)
		&& (pApdu->u16SourceMacId <= (ParaNumCNST+MAC_ID_MOD1_REC)) )
	{
		UINT16 u16Idx = pApdu->u16SourceMacId-MAC_ID_MOD1_REC;

		//��������
		objMonInterface.m_st_DiscreteState[u16Idx].Word1_U.u16Word1All			= pApdu->pDataPtr[0].u16Word;	

	}

	return i16NewMsg;
}


/********************************************************************************
BattPreWarn-----���Ԥ�澯����
                     	
Parameters:            
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanDataExchg::BattPreWarn(VOID)
{
	objPowCANSched.m_st_BatteryStatus.bBattPreWarn = 1;	//������Ӧ����
}



VOID SysCanDataExchg::BattPreWarnCancel(VOID)
{
	objPowCANSched.m_st_BatteryStatus.bBattPreWarn = 0;	//ȡ����Ӧ����	
}
/********************************************************************************
PostBattConvFltChg-----������ת����
                     	
Parameters:           
				 
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanDataExchg::PostBattConvFltChg(VOID)
{
	objMonInterface.m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16PostBattFltChgCmd = 1;
	objMonInterface.m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16PostBattEqualChgCmd = 0;
}

/********************************************************************************
NegBattConvFltChg-----������ת����
                     	
Parameters:           
				 
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanDataExchg::NegBattConvFltChg(VOID)
{
	objMonInterface.m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16NegBattFltChgCmd = 1;
	objMonInterface.m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16NegBattEqualChgCmd = 0;
}

/********************************************************************************
PostBattConvFltChg-----������ת����
                     	
Parameters:           
				 
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanDataExchg::PostBattConvEqualChg(VOID)
{
	objMonInterface.m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16PostBattEqualChgCmd = 1;
	objMonInterface.m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16PostBattFltChgCmd = 0;
}

/********************************************************************************
NegBattConvFltChg-----������ת����
                     	
Parameters:           
				 
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanDataExchg::NegBattConvEqualChg(VOID)
{
	objMonInterface.m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16NegBattEqualChgCmd = 1;
	objMonInterface.m_st_wInvReceiveMonitor.m_st_SysCtrCmd.u16NegBattFltChgCmd = 0;
}

