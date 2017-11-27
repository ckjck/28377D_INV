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

//配置信息中的时间同步配置的service code
#define TIME_SYNC_SRVE_CODE 0x01

//控制命令sevice code码相关宏定义
//逆变FLASH UPDATE
#define CTRL_INV_FLASHUPDATE_SRVE_CODE 0x21

//重发未收到应答的帧定时
//--500ms
#define RESEND_TIMER_CNT	500

//定时500ms将本模块功率信息传递一次给本模块的整流
#define PERIOD_SEND_POWER	500

//同步帧监听延时2000ms
#define SYNC_MONITOR_MAX_CNT  2000


//宏定义
//同步帧监听中
#define SYNC_FRAME_MONITORING		0
//同步帧监听完毕,可以开始通讯
#define SYNC_FRAME_MONITORED		1
//检测到总线上有相同ID节点,放弃监听
#define SYNC_FRAME_MONITOR_ABORD 0xff

//Test Macro
//#define TEST_MODE


//-------------------------------------------------------------------------------
//Private Structure and Const variable defintion
UINT16 uInvAnalogLen[INV_ANALOG_MSG_OBJ_SIZE];
UINT16 uBypAnalogLen[BYP_ANALOG_MSG_OBJ_SIZE];

//--------------------------------------------------------------------------------
//public variable

//发送、接收处理函数指针类型
typedef VOID (SysCanDataExchg::*_MSG_XMIT_FCB)(VOID);
typedef VOID (SysCanDataExchg::*_MSG_RECV_FCB)(VOID);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//模拟量处理结构体
typedef struct
{
	UINT16 u16ServiceCode;
	_MSG_RECV_FCB pMsgRecvFcb;
}_ANALOG_RECV_MSG_T;


CONST ANALOG_MSG_OBJ_T CanAnlogRecvMsgObjs[] = 
{
	//相输出有功功率
	{0x1F, 6, RECV_DIRECTION, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvRecvAnalog.OutActPwr},
	//相输出视在功率
	{0x23, 6, RECV_DIRECTION, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvRecvAnalog.OutApparentPwr},
	//环境温度
	{0x22, 2,RECV_DIRECTION,(UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvRecvAnalog.u16EnvirTemp},

	//机架间并机系统总的有功功率
	{0x32, 6,RECV_DIRECTION,(UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvRecvAnalog.RackOutActPwr},
	//机架间并机系统总的视在功率
	{0x33, 6,RECV_DIRECTION,(UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvRecvAnalog.RackOutApparentPwr},
	
};

#define ANALOG_RECV_OBJ_SIZE (sizeof(CanAnlogRecvMsgObjs) / sizeof(ANALOG_MSG_OBJ_T))


//~~~~~~~~~~~~~~~~~~~~~~
//控制信息

//控制信息对象
CONST CAN_CTRL_MSG_T CanCtrlMsgObjs[] =
{
	
	//正组电池转浮充
	{0x01, 0x00, &SysCanDataExchg::PostBattConvFltChg},
	//负组电池转浮充
	{0x02, 0x00, &SysCanDataExchg::NegBattConvFltChg},
	//正组电池转均充
	{0x03, 0x00, &SysCanDataExchg::PostBattConvEqualChg},
	//负组电池转均充
	{0x04, 0x00, &SysCanDataExchg::NegBattConvEqualChg},
	//电池电压低关机命令
	{0x08, 0x02, &SysCanDataExchg::BattEodTurnOff},
	//电池BCB命令
//	{0x09, 0x02, &SysCanDataExchg::BattTrip},
	//输出允许命令
	{0x0A, 0x02, &SysCanDataExchg::OutputEnableCmd},
	//输入禁止命令
	{0x0B, 0x02, &SysCanDataExchg::OutputDisableCmd},
	//电池接地保护命令
	{0x0C, 0x02, &SysCanDataExchg::BattGndPtcCmd},

	//20%自检放电命令
	{0x05, 0x02, &SysCanDataExchg::BattSelfChk20Percent},
	//深度自检放电命令
	{0x06, 0x02, &SysCanDataExchg::BattSelfChkDeep},
	//电池放电结束命令
	{0x07, 0x02, &SysCanDataExchg::BattSelfChkEnd},

	//系统控制命令
	//故障消除命令
	{0x10, 0, &SysCanDataExchg::FaultClrCmd},
	//手动开机命令
	{0x11, 0, &SysCanDataExchg::MaualStartupCmd},
	//手动关机机命令
	{0x12, 0, &SysCanDataExchg::MaualShutCmd},
	//紧急关机命令
	{0x13, 0, &SysCanDataExchg::EpoCmd},
	//异常关机确认命令
	{0x14, 0, &SysCanDataExchg::AbnormalShutConfirmCmd},
	//异常关机取消
	{0x15, 0, &SysCanDataExchg::AbnormalShutCancelCmd},
	//间接切换命令
	{0x16, 0, &SysCanDataExchg::IndirectSwitchCmd},
	

	//FlashUpdate
	//整流Flashupdate
	{0x20, 0, &SysCanDataExchg::RecFlashUpdate},
	//逆变Flashupdate
	{0x21, 0, &SysCanDataExchg::InvFlashUpdate},
	//旁路Flashupdate
	{0x22, 0, &SysCanDataExchg::BypFlashUpdate},

	//电池预告警就绪	
	{0x23, 0, &SysCanDataExchg::BattPreWarn},

	//电池预告静取消
	{0x24, 0, &SysCanDataExchg::BattPreWarnCancel}

		
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`
//模拟量信息//TBD 数据指针未定义
//随同步帧上传
CONST ANALOG_MSG_OBJ_T CanAnlogMsgObjs[]=
{

	//相输出有功功率
	{0x10, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.OutActPwr},
	//相输出视在功率
	{0x11, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.OutApparentPwr},
	//相输出功率因素
	{0x12, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.OutFactor},
	
	//相输出电压
	{0x13, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.OutPhaseVlt},
	//相输出电流
	{0x14, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.OutPHaseCurr},
	//逆变相电压
	{0x15, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.InvPhaseVlt},
	
	//逆变相电流
	{0x16, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.InvPhaseCurr},
	//逆变电感电流
	{0x17, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.InductCurr},

	//旁路相电压
	{0x1C, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.BypPhaseVlt},
	
	//输出负载百分比
	{0x18, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.OutLoadRate},
	//输出峰值比
	{0x19, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.OutPeakRate},
	//输出频率
	{0x1A, 2, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.OutFreq},
	//旁路输出频率
	{0x1D, 2, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.BypFreq},
	
	//逆变测试变量
	{0x1B, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_InvAnalog.DbgVar}
	
};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`
//旁路模拟量信息//TBD 数据指针未定义
//其中功率和环境温度要定时广播
CONST ANALOG_MSG_OBJ_T BypCanAnlogMsgObjs[]=
{
	//旁路相电压
	{0x2C, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.BypPhaseVlt},
	//输出相电压
	{0x27, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.BypOutPhaseVlt},
	//旁路输出电流
	{0x2E, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.BypPhaseCurr},
	//相输出功率因素
	{0x24, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.OutFactor},
	//旁路频率
	{0x2D, 2, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.BypFreq},
	//输出频率
	{0x28, 2, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.BypOutFreq},

	//输出有功功率
	{0x2F, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.OutActPwr},
	//输出视在功率
	{0x23, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.OutApparentPwr},

	//输出负载百分比
	{0x25, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.OutLoadRate},
	//输出峰值比
	{0x26, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.OutPeakRate},

	//增加机架系统有功/视在功率
	//输出有功功率
	{0x32, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.RackOutActPwr},
	//输出视在功率
	{0x33, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.RackOutApparentPwr},
	
	//电池温度---包括电池内部和外部温度,所以长度从2-->4
//	{0x20, 4, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.BatteryInTemp},
	//环境温度
//	{0x22, 2, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.EnvirTemp},
	//旁路调试变量
//	{0x21, 6, XMIT_DIRECTION, (UINT16 *)&objMonInterface.m_st_BypSendMonitor.m_st_BypAnalog.BypDebugVari}
//缺少锂电池信模块息
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`
//CFG msg objs //TBD 数据指针未定义
CONST CFG_MSG_T CfgFromMonObjs[] = 
{
	//System dsp
	//模块机型容量
	{0x10, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16CapType},
	//系统电压等级
	{0x11, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16SysVltGrade},

//mask the REC parameter
#if (0)
	//输入频率设置
	{0x12, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16InFreq},
	//智能发电机模式设置
	{0x13, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16GenMod},
	//智能发电机限功率设置
	{0x14, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16GenPwrLtd},
	//智能发电机启机延时时间设置
	{0x15, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16GenStartDelay},
#endif

	//系统机架号设置
	{0x16, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16RMIdx},
	//并机系统机架数设置
	{0x17, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16RMNum},
	//本机基本模块数设置
	{0x18, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BasicModNum},
	//本机冗余模块数设置
	{0x19, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16ReModNum},
	//LBS模式
	{0x1A, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16LBSMode},
	//ECO 模式TBD
	{0x1B, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16RunMod},
	//逆变器工作模式设置
	{0x1C, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16InvRunMode},
	//输出频率设置
	{0x1D, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutFreq},
	//自老化输出电流百分比设置
	{0x1E, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16AgingOutCurrRate},
	//输出间断切换时间设置
	{0x1F, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutSwitchTime},
	//输出频率跟踪速率设置
	{0x20, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutFreqTrackRate},
	//逆变器输出功率因素设置
	{0x21, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16InvOutFactor},
	//输出电压微调系数设置
	{0x22, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutVltMicroAdj},

	//旁路切换次数设置
	{0x23, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypSwitchTime},
	//旁路频率跟踪范围设置
	{0x24, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypFreqTrackLtd},
	//旁路电压包含范围设置(上限)
	{0x25, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypVltUpLtd},
	//旁路电压包含范围设置(下限)
	{0x26, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypVltDownLtd},
	//旁路频率保护范围设置
	{0x27, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypFreqLtd},
	//旁路反灌保护用断路器TBD
	{0x28, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BypProtectBrk},
	//三三、三单设置
	{0x29, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16OutputPhase},

	//2017.06.10
	//逆变调试参数1
	{0x2b, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16DegVar1},
	//逆变调试参数2
	{0x2c, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16DegVar2},
	//逆变调试参数3
	{0x2d, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16DegVar3},

	//冗余机架数
	{0x35, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16ReRMNum},
	//配置属性---单机/并机/主从热备份
	{0x36, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16UpsConfigType},
	//share bat	
	{0x3C, 6, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.SetParam.u16BatShare},	
	//逆变调试变量1地址
	{0x69, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16DebugVar1Addr},
	//逆变调试变量2地址
	{0x6A, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16DebugVar2Addr},
	//逆变调试变量3地址
	{0x6B, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16DebugVar3Addr},
	
		

	
//-----------系统配置到设置完变量地址即结束------------------------------------------------

	//INV set
	//the follow parameter need store to EEPROM
	//输出A相电压校正系数
	{0x5A, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_a},
	//输出B相电压校正系数
	{0x5B, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_b},
	//输出C相电压校正系数
	{0x5C, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutVltRevCoef_c},
	//输出A相电流校正系数
	{0x5D, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_a},
	//输出B相电流校正系数
	{0x5E, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_b},
	//输出C相电流校正系数
	{0x5F, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16OutCurrRevCoef_c},
	//逆变A相电压校正系数
	{0x60, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_a},
	//逆变B相电压校正系数
	{0x61, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_b},
	//逆变C相电压校正系数
	{0x62, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvVltRevCoef_c},
	//逆变A相电流校正系数
	{0x63, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_a},
	//逆变B相电流校正系数
	{0x64, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_b},
	//逆变C相电流校正系数
	{0x65, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16InvCurrRevCoef_c},
	//旁路A相电压校正系数
	{0x66, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_a},
	//旁路B相电压校正系数
	{0x67, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_b},
	//旁路C相电压校正系数
	{0x68, 4, NONFRAG_MSG, FALSE, RECV_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam.u16BypVltRevCoef_c}
	

	
};

CONST CFG_MSG_T CfgToMonObjs[] = 
{
	//配置信息查询TBD liuj
	//INV config requirement
	{0x71, INV_SET_INFO_SIZE, FRAG_MSG, FALSE, XMIT_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.SetParam},
	//协议版本查询
	{0x73, 6, NONFRAG_MSG, FALSE, XMIT_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_ModuleVerInfo.ProtocolVer},
	//固件版本查询
	{0x74, 6, NONFRAG_MSG, FALSE, XMIT_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_ModuleVerInfo.DspFwVer},
	//CPLD版本查询
	{0x75, 6, NONFRAG_MSG, FALSE, XMIT_DIRECTION, FALSE, (UINT16 *)&objMonInterface.m_st_InvSendMonitor.m_st_ModuleVerInfo.CpldFwVer},
	//模块序列号查询TBD
	{0x76, 6, NONFRAG_MSG, FALSE, XMIT_DIRECTION, FALSE, NULL}

};


CONST UINT16 uInvSetMsgBitSet[]=
{
	//输入A相电压校正系数
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

//静态成员变量初始化
SysCanDataExchg* SysCanDataExchg::m_pDataExchgPtr = NULL;

//--------------------------------------------------------------------------------
//private  function

//--------------------------------------------------------------------------------
//function definiton


#if (0)
//状态COV

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

//对象初始化
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
		m_StateEventObj.CovObj[i].bXmitActive= FALSE;	//事件发送使能
		m_StateEventObj.CovObj[i].bCovFlag = FALSE;		//事件COV标志
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

	//同步帧监控信息初始化
	m_SyncFrameMonitor.usMonitorNodeIdFlag = SYNC_FRAME_MONITORING;
	m_SyncFrameMonitor.usMonitorCnt = 0;
	

	
	// 4 Sync Flag
	//同步模拟量
	m_u16SyncFlag = FALSE;
	//同步配置请求
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
	//批量设置的第一个参数SRVS CODE
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

//CAN通讯与DSP逻辑交互数据处理
//包括1.COV标志产生，2.事件产生 3.事件存EEPROM
/**********************************************************************
InterfaceDataProc-----接口数据处理

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
VOID SysCanDataExchg::InterfaceDataProc(VOID)
{
//	INT16 i16NewMsg;
//	UINT16 i;
	
	//上电配置尚未完成
	if ((objMonInterface.m_st_InvSendMonitor.m_st_InvBreakerStatus.Word1_U.tWord1Bits.bParamSetReq)
		&& (0 == objMonInterface.m_st_IdErrorFlag.bUsIdNumExpire) )
	{
		//配置未完成，不做COV处理
		//设置中事件处理
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

			//旁路不需要做任何事情
		}
		
		
		else
		{
			;//do nothing
		}

	}
	else
	{
		//--1.逆变状态COV加工
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
		//旁路状态COV加工
		m_StateMsgObj.Msg2 = objMonInterface.m_st_BypSendMonitor.m_st_BypBreakerStatus;
		if ( (m_StateMsgObj.Msg2.Word1_U.u16Word1All != m_BypStateMsgBak.Word1_U.u16Word1All))
		{
			m_StateMsgObj.CovObj[MODULE_BYP].bCovFlag = TRUE;
		}
		
		else
		{
			m_StateMsgObj.CovObj[MODULE_BYP].bCovFlag = FALSE;
		}		

		//--2.逆变告警COV加工
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

		//--2.旁路告警COV加工
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


		//--3.状态事件COV
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

		//其他状态事件
		GenStateEvent();

		//--4.告警事件COV
		GenAlarmEvent();

		//--5.并机状态COV TBD,不产生事件的
		
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
		
		//旁路状态事件
		GenBypStateEvent();
		//旁路告警事件
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
			//第一个状态字
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

					//TBD 将事件写入EEPROM相关缓冲区动作liuj
					
				}
			}

			//第二个状态字
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

					//TBD 将事件写入EEPROM相关缓冲区动作liuj
					
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
			//告警字1
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
			
			//告警字2
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

			//告警字3
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
			
			//TBD 将事件写入EEPROM相关缓冲区动作liuj
						
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
GetStateCovMsg-----获取整流状态发送信息

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
INT16 SysCanDataExchg::GetStateCovMsg(OUT CAN_APDU_T *pApdu,UCHAR modul)
{
	INT16 i16NewMsg = NON_COV_MSG;


	//初始配置未完成,不做COV处理
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

//----------并机相关状态量处理TBD-----------------
/**********************************************************************
GetParallelStateMsg-----获取机架内并机状态发送信息

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
INT16 SysCanDataExchg::GetParallelStateMsg(OUT CAN_APDU_T *pApdu)
{
	INT16 i16NewMsg = NON_COV_MSG;
	INT16 i;

	//初始配置未完成,不做COV处理
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
		
			//维护相应数据
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
MonRealMsgRecvProc-----来自监控的实时数据接收处理,监控定时1S下发
                     	
Parameters:            pApdu:接收到的数据帧
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanDataExchg::MonRealMsgRecvProc(IN CAN_APDU_T *pApdu)
{
	INT16 i16NewMsg = NON_SYNC_MSG;
	
	if (pApdu->u16SourceMacId == MAC_ID_MON)
		
	{
		//接收数据
		objMonInterface.m_st_MonRealMsg.Word1_U.u16Word1All = pApdu->pDataPtr[0].u16Word;	

	}

	return i16NewMsg;
}

/**********************************************************************
ParallelStateXmitMsgGen-----机架内并机状态信息加工

                     
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
ReqSetMsg-----请求设置信息处理

                     
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
	//收到同步帧，如果请求配置信息尚未收到监控的回应，则
	//继续重发发请求
	//批量请求设置标志有效
	if (TRUE == m_ReqSetFlag)
	{
		if (m_ucSyncCfgReqFlag == TRUE)
		{
			m_ucSyncCfgReqFlag = FALSE;
			//批量设置请求打包
			pApdu->u16ByteLength = 0;
			pApdu->u16ServiceCode = 0x10;      		//第一个参数:机型容量设置
			pApdu->u16Fragment = NONFRAG_MSG;
			pApdu->u16MsgClass = CAN_CFG_MSG_CLASS;
			pApdu->u16RsRq = RQ_MSG;
			pApdu->u16DestinationMacId = MAC_ID_MON;
			pApdu->u16SourceMacId = objMonInterface.m_i16wSerialNumSet_0;//LOCAL_SYS_CAN_ID;
			
			i16NewMsg = HAVE_REQ_MSG;
			
		}

		
		
	}

	//已经开始配置,但没配置完定时到后重发
	else
	{
		//但没配置完
		if (TRUE ==m_CfgMsgObj.FirstCfgFlag.bFromMon)
		{
			//定时到500MS
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
GetResendSysSetFlag-----获取重发标志

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
INT16 SysCanDataExchg::GetResendSysSetFlag(void)
{
	INT16 Ret=FALSE;
	
	//--1ms定时到
	if (objSystem.m_st_wFlagSystem.bTimer1ms == 1)
	{
		//50ms定时到准备重发
		if (++m_ResendReqSetTimer > RESEND_TIMER_CNT)
			Ret = TRUE;
		
	}
	
	return Ret;
}

/**********************************************************************
EndResendTimer-----停止重发定时

                     
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
RestartResendTimer-----重新启动重发定时

                     
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
StateCovMsgXmitState-----整流状态发送状态

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
VOID SysCanDataExchg::StateCovMsgXmitState(IN INT16 i16XmitState)
{
	
}


/**********************************************************************
StateCovMsg1Fcb-----状态信息加工

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: u16IDx :模块的类型 逆变还是旁路
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
	
	if(u16Idx == MODULE_INV)//逆变
	{
		m_XmitMsgBuff.CanApdu.u16SourceMacId = objMonInterface.m_i16wSerialNumSet_0;//LOCAL_SYS_CAN_ID;
		for (i = 0; i < (INV_SW1_LENGTH / 2); i++)
		{
			*pTemp++ = m_StateMsgObj.CovObj[MODULE_INV].pData[i];
		}
	}
	if(u16Idx == MODULE_BYP)//旁路
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
GetAlrmCovMsg-----获取整流告警发送信息

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
INT16 SysCanDataExchg::GetAlrmCovMsg(OUT CAN_APDU_T *pApdu,UCHAR modul)
{
	INT16 i16NewMsg = NON_COV_MSG;

	//初始配置未完成,不做COV处理
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
AlrmCovMsgXmitState-----整流告警发送状态

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
VOID SysCanDataExchg::AlrmCovMsgXmitState(IN INT16 i16XmitState)
{


}

/**********************************************************************
StateCovMsg1Fcb-----告警信息加工

                     
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

	if(u16Idx == MODULE_INV)//逆变
	{
		m_XmitMsgBuff.CanApdu.u16SourceMacId = objMonInterface.m_i16wSerialNumSet_0;//LOCAL_SYS_CAN_ID;
		for (i = 0; i < (INV_ALRM1_LENGTH / 2); i++)
		{
			*pTemp++ = m_AlrmMsgObj.CovObj[MODULE_INV].pData[i];
		}
	}
	else		//旁路
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
GetEventLogMsg-----获取事件记录COV信息

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
INT16 SysCanDataExchg::GetEventLogMsg(OUT CAN_APDU_T *pApdu,UCHAR modul)
{
	UINT16 i;
	
	INT16 i16NewMsg = NON_COV_MSG;

	//初始配置未完成,不做COV处理
	if (objMonInterface.m_st_InvSendMonitor.m_st_InvBreakerStatus.Word1_U.tWord1Bits.bParamSetReq == 0)
	{
		if(modul == MODULE_INV)
		{
		//告警事件COV
		for (i=0;i<INV_ALARM_EVENT_OBJ_SIZE;i++)
		{
			if (m_AlarmEventObj.CovObj[i].bCovFlag == TRUE)
			{
				AlarmEventXmitMsgGen(i,modul);
				i16NewMsg = HAVE_COV_MSG;
				break;
			}
		}

		//状态事件COV
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
		//旁路告警事件
		for (i=0;i<BYP_ALARM_EVENT_OBJ_SIZE;i++)
		{
			if (m_BypAlarmEventObj.CovObj[i].bCovFlag == TRUE)
			{
				AlarmEventXmitMsgGen(i,modul);
				i16NewMsg = HAVE_COV_MSG;
				break;
			}
		}

		//状态事件COV
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

	//批量设置事件
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
	
	m_XmitMsgBuff.CanApdu.u16ByteLength = 6;			//6BYTES 有效数据
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
	
	m_XmitMsgBuff.CanApdu.u16ByteLength = 6;			//6BYTES 有效数据
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
GetStateSyncMsg-----获取同步状态信息

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
INT16 SysCanDataExchg::GetStateSyncMsg(OUT CAN_APDU_T *pApdu,UCHAR modul)
{
	INT16 i16NewMsg = NON_SYNC_MSG;

	//初始配置未完成,不做COV处理
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

		//机架内并机状态数据SYNC
		else if ((TRUE == m_ParallelStateMsgObj.SyncInfo.i16XmitActive)
				&&(modul == MODULE_INV))
		{
			if (m_ParallelStateMsgObj.SyncInfo.i16XmitIdx < PARALLEL_STATE_MSG_OBJ_SIZE ) 
			{
				ParallelStateXmitMsgGen(m_ParallelStateMsgObj.SyncInfo.i16XmitIdx);
				m_ParallelStateMsgObj.SyncInfo.i16XmitIdx++;
				i16NewMsg = HAVE_SYNC_MSG;

				//维护相应数据
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
StateSyncMsgXmitState-----同步状态信息发送状态

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
VOID SysCanDataExchg::StateSyncMsgXmitState(IN INT16 i16XmitState)
{

}


/**********************************************************************
GetAlrmSyncMsg-----获取同步告警信息

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
INT16 SysCanDataExchg::GetAlrmSyncMsg(OUT CAN_APDU_T *pApdu,UCHAR modul)
{
	INT16 i16NewMsg = NON_SYNC_MSG;

	//初始配置未完成,不做COV处理
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
AlrmSyncMsgXmitState-----同步告警信息发送状态

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
VOID SysCanDataExchg::AlrmSyncMsgXmitState(IN INT16 i16XmitState)
{


}

/**********************************************************************
GetAnalogSyncMsg-----获取同步模拟量信息

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
INT16 SysCanDataExchg::GetAnalogSyncMsg(OUT CAN_APDU_T *pApdu,UCHAR modul)
{
//	INT16 i;
	INT16 i16NewMsg = NON_SYNC_MSG;

	//初始配置未完成,不做COV处理
	if (objMonInterface.m_st_InvSendMonitor.m_st_InvBreakerStatus.Word1_U.tWord1Bits.bParamSetReq == 0)
	{
		if (TRUE == m_AnalogSyncMsgObj.SyncInfo.i16XmitActive)
		{
			//仅BYP_ANALOG_MSG_OBJ_SIZE<INV_ANALOG_MSG_OBJ_SIZE适应以下结构，否则二者位置对调
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
				else //如果程序调用错误
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
PeriodAnalogSyncMsg-----定时传递本模块的逆变信息发送给整流

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
INT16 SysCanDataExchg::PeriodAnalogSyncMsg(OUT CAN_APDU_T *pApdu)
{
	INT16 i16NewMsg = NON_SYNC_MSG;
	
	//初始配置未完成,不做COV处理
	if (objMonInterface.m_st_InvSendMonitor.m_st_InvBreakerStatus.Word1_U.tWord1Bits.bParamSetReq == 0)
	{
		//定时处理
		//--1ms定时到
		if (objSystem.m_st_wFlagSystem.bTimer1ms == 1)
		{
			//功率信息定时
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
PeriodPowerXmitMsgGen-----定时发送功率信息准备

                     
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
		//有功功率
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
		//视在功率
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
			
		//旁路电流
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

		//最小充电器ID
		case 3:

			m_XmitMsgBuff.CanApdu.u16ByteLength = 6;
			m_XmitMsgBuff.CanApdu.u16ServiceCode = 0x34;
			m_XmitMsgBuff.CanApdu.u16Fragment = NONFRAG_MSG;
			m_XmitMsgBuff.CanApdu.u16MsgClass = CAN_ANALOG_MSG_CLASS;
			m_XmitMsgBuff.CanApdu.u16RsRq = RS_MSG;
			m_XmitMsgBuff.CanApdu.u16DestinationMacId = 0x10+objMonInterface.m_i16wSerialNumSet_0;
			m_XmitMsgBuff.CanApdu.u16SourceMacId = objMonInterface.m_i16wSerialNumSet_0;

			//已开启最小充电器ID

			*pTemp++ = objPowCANSched.m_i16ChgOnMinID;
			*pTemp++ = objPowCANSched.m_i16ChgPBoostTotal;//强制转均冲命令
			*pTemp++ = 0;//预留其他
			
			m_XmitMsgBuff.CanApdu.pDataPtr = m_XmitMsgBuff.DataBuff;
			break;	
		default:

			break;
	}
}


/**********************************************************************
AnalogSyncMsgXmitState-----同步模拟量信息发送状态

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
VOID SysCanDataExchg::AnalogSyncMsgXmitState(IN INT16 i16XmiyState)
{
	

}

/**********************************************************************
AnalogXmitMsgGen-----模拟量信息加工

                     
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
BattVoltMsgRecvFcb-----接收电池浮充、均充电压

                     
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
TotalPwrMsgRecvFcb-----接收总功率信息

                     
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
CfgMsgRecvProc-----配置帧接收处理
                     	
Parameters:            pApdu:接收到的数据帧
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

		//时间同步不做回应
		return NON_CFG_MSG;
	}

	//监控主动设置信息或DSP批量设置请求后监控的回应设置
	for (i = 0; i < INV_CFG_FROM_MON_OBJ_SIZE; i++)
	{
		if (pApdu->u16ServiceCode == m_CfgMsgObj.FromMon[i].bSrvcCod)
		{
			i16NewMsg = CfgMsgFromMonProc(pApdu, i);
			return i16NewMsg;
		}
	}

	//回应监控侧的请求配置信息的查询
	//即监控可以查询当前的配置参数值
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
SyncMsgProc-----同步帧处理
                     	
Parameters:            pApdu:接收到的数据帧
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
			//启动上电后开始监听同步帧定时器
			m_SyncFrameMonitor.usMonitorCnt = 1;
		}

		//定时器已经启动,如果监听ID结果通过,则回应同步帧

		//监控每隔1S下发一次同步信息请求，本模块开始发送
		if ((m_SyncFrameMonitor.usMonitorNodeIdFlag == SYNC_FRAME_MONITORED)
			&& (1 != objMonInterface.m_st_IdErrorFlag.bUsIdNumExpire))
		{
			m_u16SyncFlag = TRUE;

			//置标志
			m_StateMsgObj.SyncInfo.i16XmitActive = TRUE;
			m_StateMsgObj.SyncInfo.i16XmitIdx = 0;
			m_AlrmMsgObj.SyncInfo.i16XmitActive = TRUE;
			m_AlrmMsgObj.SyncInfo.i16XmitIdx = 0;
			m_AnalogSyncMsgObj.SyncInfo.i16XmitActive = TRUE;
			m_AnalogSyncMsgObj.SyncInfo.i16XmitIdx = 0;

			//置机架内并机状态SYNC标志
			m_ParallelStateMsgObj.SyncInfo.i16XmitActive = TRUE;
			m_ParallelStateMsgObj.SyncInfo.i16XmitIdx = 0;

			m_ucSyncCfgReqFlag = TRUE;
			//同步帧回应打包
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

	//监听同步帧
	else
	{
		//找到ID号相同的节点,将不再发任何信息
		if (pApdu->u16SourceMacId  ==  objMonInterface.m_i16wSerialNumSet_0)
		{
			//置退出监听标志=0正在监听,=1不再监听开始通讯,=0xff放弃监听
			m_SyncFrameMonitor.usMonitorNodeIdFlag = SYNC_FRAME_MONITOR_ABORD;
			//ID号冲突,置标志传递给逻辑使用
			objMonInterface.m_st_IdErrorFlag.bUsIdNumOverlap = 1;
			//不再监听
			m_SyncFrameMonitor.usMonitorCnt = SYNC_MONITOR_MAX_CNT;
		}

		//没监听到相同节点的ID
		else
		{
			//do nothing
		}
	}

	return i16NewMsg;
}

/********************************************************************************
MonitorSyncMsgProc-----同步帧开始监听处理
                     	
Parameters:            
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanDataExchg::MonitorSyncMsgProc( VOID)
{
	//如果在总线上没发现相同的节点
	if (m_SyncFrameMonitor.usMonitorNodeIdFlag != SYNC_FRAME_MONITOR_ABORD)
	{
		//还在监听过程中
		if (SYNC_FRAME_MONITORED != m_SyncFrameMonitor.usMonitorNodeIdFlag )
		{
			//每个主循环维护计数器
			if (GetSyncMonitorCnt())
			{
				m_SyncFrameMonitor.usMonitorNodeIdFlag = SYNC_FRAME_MONITORED;

			}

			//监听延时未到,继续监听
			else
			{
				//do nothing
			}
		}
	}

	//总线上已有相同的ID节点
	else
	{
		//do nothing
	}

	if (m_SyncFrameMonitor.usMonitorNodeIdFlag == SYNC_FRAME_MONITORED)
	{
		
		//批量设置中开始
		if ((m_BatchSetEvent == 0)
			&& (objMonInterface.m_SyncTime.u16HaveSyncClkFlag == 1))
		{
			m_BatchSetEvent = 1;
		}
	}
	
}

/**********************************************************************
GetSyncMonitorCnt-----获取延时计数器

                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
INT16 SysCanDataExchg::GetSyncMonitorCnt(void)
{
	INT16 Ret=FALSE;
	
	//--1ms定时到
	if (objSystem.m_st_wFlagSystem.bTimer1ms == 1)
	{
		//5ms定时到
		//已启动计数器则计数
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
SyncTimeMsgProc-----时间同步处理
                     	
Parameters:            pApdu:接收到的数据帧
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
		//将同步时间保存到InterfaceMon接口中定义的时间变量中
		objMonInterface.m_SyncTime.u16TimeS_L= pApdu->pDataPtr[0].u16Word;
		objMonInterface.m_SyncTime.u16TimeS_H = pApdu->pDataPtr[1].u16Word;

		//同时将时间ms清0，以便从新时间开始重新计时
		//如果要精确到us则可将此监控下传的量取过来用，现清0处理
		objMonInterface.m_SyncTime.u16TimeMS = pApdu->pDataPtr[2].u16Word;
		//objMonInterface.m_SyncTime.u16TimeMS = 0;
		if (0 == objMonInterface.m_SyncTime.u16HaveSyncClkFlag)
		{
			objMonInterface.m_SyncTime.u16HaveSyncClkFlag = 1;
		}

		//通知
		objMonInterface.m_i16TimeSynCommand_0 = 1;
	}

	return i16NewMsg;
}


/********************************************************************************
CfgMsgFromMon-----配置信息设置处理
                     	
Parameters:            pApdu:接收到的数据帧
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanDataExchg::CfgMsgFromMonProc(IN CAN_APDU_T *pApdu, IN UINT16 u16Idx)
{
	INT16 i16NewMsg = NON_CFG_MSG;

	INT16 i16NewCfgFromMon=0;
	UINT16 u16SrvCodeBak ;
	
	//配置信息来自监控的应答或监控主动下设信息
	if (RS_MSG == pApdu->u16RsRq)
	{
		EndResendTimer();
		
		//来自监控批量设置信息处理
		if (TRUE == m_CfgMsgObj.FirstCfgFlag.bFromMon)
		{
			//保证设置的连续性
			if (m_CfgMsgIdxBak != u16Idx)
			{
				//还有参数未设置则继续下一个参数设置请求
				//未连续,重发

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
						//判断是否为相同电池组
						//pApdu->pDataPtr[0].u16Word为电池组号
						*(m_CfgMsgObj.FromMon[u16Idx].pData) = pApdu->pDataPtr[1].u16Word;
						
						i16NewCfgFromMon=1;
						u16SrvCodeBak = pApdu->u16ServiceCode;
					}

					//LIUJ 请求监控设置完毕
					if ((INV_SYS_CFG_FROM_MON_OBJ_SIZE -1) == u16Idx)
					{
						m_CfgMsgObj.FirstCfgFlag.bFromMon = FALSE;
						objSystem.m_st_wFlagSystem.bParamSetReq = 0;
						i16NewMsg = NON_CFG_MSG;
					}

					//还有参数未设置则继续下一个参数设置请求
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

		//监控主动下设设置参数或非批量设置时对DSP侧请求的应答
		else
		{
			//支持此量，则取用此数据
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
					//判断是否为相同电池组
					//pApdu->pDataPtr[0].u16Word为电池组号
					*(m_CfgMsgObj.FromMon[u16Idx].pData) = pApdu->pDataPtr[1].u16Word;
					i16NewCfgFromMon=1;
					u16SrvCodeBak = pApdu->u16ServiceCode;
				}
			}

			//无论支持与否，均不回应
			i16NewMsg = NON_CFG_MSG;
		}

		//监控有新的参数设置，置更新标志
		if (i16NewCfgFromMon == 1) 
		{
			//系统设置参数
			if (u16SrvCodeBak < 0x40||(u16SrvCodeBak == 0x6f))
			{
				objMonInterface.m_st_wInvReceiveMonitor.m_st_SysDSPSettings.u16SetParamFresh = TRUE;
				objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh2 = TRUE;
			}
			//逆变设置参数
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
				//TBD 写EEPROM操作
				
			}
		}

	}

	//TBD liuj 监控主动获取设置信息的处理20080524
	else 
	{
#if (0)
		//如果监控是要获取整流配置信息,则为分段帧上传
		if (0x70 == pApdu->u16ServiceCode )
		{

		}
#endif		
//		else
//		{
		
			//配置信息应答帧打包
			CfgMsgRsPacked(pApdu, u16Idx );
			
			//监控获取信息，准备信息应答给监控
			i16NewMsg = HAVE_CFG_MSG;
	
//		}
		#if (0)
		//不支持
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
GetFaultData-----故障信息上传处理准备
                     	
Parameters:            pApdu:接收到的数据帧
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanDataExchg::GetFaultData(OUT CAN_APDU_T *pApdu)
{
	INT16 i16NewMsg = NON_CFG_MSG;
	
	//故障记录缓冲区数据已准备好
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
CfgMsgToMonProc-----配置信息查询处理,监控请求
                     	
Parameters:            pApdu:接收到的数据帧
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanDataExchg::CfgMsgToMonProc(IN CAN_APDU_T *pApdu, IN UINT16 u16Idx)
{
	UINT16 i;
	INT16 i16NewMsg = NON_CFG_MSG;

	//监控请求信息查询
	if (RQ_MSG == (pApdu->u16RsRq))
	{
		if (TRUE == m_CfgMsgObj.FirstCfgFlag.bToMon)
		{
			m_CfgMsgObj.ToMon[u16Idx].bCompleted =  TRUE;

			//疑问:配置信息查询是否应该无论何时都可以查，所以
			//标志m_CfgMsgObj.ToMon[i].bCompleted不应该起作用，没有意义?TBD liuj 20060524
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
CfgMsgRqPacked-----配置信息请求帧打包
                     	
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

	//重新开始计时是否需要重发
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
CtrlMsgToMonProc-----控制信息查询处理
                     	
Parameters:            pApdu:接收到的数据帧
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
		//增加一个调试帧，自定义的servcode = 0xaa
		if (pApdu->u16ServiceCode = 0xaa)
		{
			//pDataPtr[0]为需要查询的servcode
			pApdu->pDataPtr[0] = 
			UINT16 *pAnswer = &objMonInterface.m_st_CtrlCmd_D ;
			
		}
		#endif
	}
	
	//TBD
	return 0;
}


/********************************************************************************
AlrmMsgRecvProc-----获取故障数据处理
                     	
Parameters:            pApdu:接收到的数据帧
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanDataExchg::AlrmMsgRecvProc(IN CAN_APDU_T *pApdu)
{
//	INT16 i;
//	INT16 ret = HAVE_FAULT_MSG;
	
	//故障记录目前只读三个点
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
StateMsgRecvProc-----状态信息中的同步帧信息处理
                     	
Parameters:            pApdu:接收到的数据帧
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanDataExchg::StateMsgRecvProc(IN CAN_APDU_T *pApdu)
{

	INT16 ret = NON_SYNC_MSG;
	
	//同步帧，监控每隔1S请求模块发送信息
	if (SYNC_MSG_SRVC_CODE == pApdu->u16ServiceCode )
	{
		ret = SyncMsgProc(pApdu);
	}

	//机架内并机数据接收处理
	else if (PARALLEL_STATE_MSG_SRVC_CODE == pApdu->u16ServiceCode)
	{
		ret = ParallelMsgRecvProc(pApdu);
	}

	//机架内旁路柜数据接收处理
	else if (BYPMAINT_STATE_MSG_SRVC_CODE == pApdu->u16ServiceCode)
	{
		ret = BypMaintMsgRecvProc(pApdu);
	}

	//监控每隔1S下传的实时信息
	else if (MON_REALTIME_MSG_SRVC_CODE == pApdu->u16ServiceCode)
	{
		ret = MonRealMsgRecvProc(pApdu);
	}

	//机架间状态信息接收
	else if (RACK_STATE_MSG_SRVC_CODE == pApdu->u16ServiceCode)
	{
		ret = RackStateMsgRecvProc(pApdu);
	}

	//接收来自整流的零散信号信息,目前只定义了主路反灌信号
	else if ((REC_DISCRETE_MSG_SRVC_CODE == pApdu->u16ServiceCode)
		    &&(MODULE_NUM == 2))//针对20K，同时具备逆变和旁路的功能
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
BypMaintMsgRecvProc-----机架内旁路柜数据接收处理
                     	
Parameters:            pApdu:接收到的数据帧
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanDataExchg::BypMaintMsgRecvProc(IN CAN_APDU_T *pApdu)
{
	INT16 i16NewMsg = NON_SYNC_MSG;
	
	if (pApdu->u16SourceMacId == MAC_ID_BYP)
	{
		
		//接收数据
		objMonInterface.m_st_wBypBreakData.Word1_U.u16Word1All = pApdu->pDataPtr[0].u16Word;

	}

	return i16NewMsg;
}


/********************************************************************************
RackStateMsgRecvProc-----接收来自旁路机架间状态信息接收处理
                     	
Parameters:            pApdu:接收到的数据帧
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanDataExchg::RackStateMsgRecvProc(IN CAN_APDU_T *pApdu)
{
	INT16 i16NewMsg = NON_SYNC_MSG;
	
	if (pApdu->u16SourceMacId == MAC_ID_BYP)
	{
		
		//接收数据
		objMonInterface.m_st_wRackStateMsg.Word1_U.u16Word1All = pApdu->pDataPtr[0].u16Word;
		objMonInterface.m_st_wRackStateMsg.Word2_U.u16Word2All = pApdu->pDataPtr[1].u16Word;


	}

	return i16NewMsg;
}

/********************************************************************************
ParallelMsgRecvProc-----机架内并机数据接收处理
                     	
Parameters:            pApdu:接收到的数据帧
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

		//接收数据
		objMonInterface.m_st_wParallelData.ParallelState[u16Idx].Word1_U.u16Word1All
			= pApdu->pDataPtr[0].u16Word;

		//维护相应数据
		MaintParallelData();
		

	}

	return i16NewMsg;
}

/********************************************************************************
MaintParallelData-----机架内并机数据维护处理
                     	
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
	
	//计算并机系统逆变总开机台数和总供电台数 
	for(UINT16 i=0;i<ParaNumCNST;i++)
	{    
		if ( objMonInterface.m_st_wParallelData.ParallelState[i].Word1_U.tWord1Bits.bInvOn != 0 )
		{
			iNumInv++;					//逆变开机总台数
		}
		if ( ( objMonInterface.m_st_wParallelData.ParallelState[i].Word1_U.tWord1Bits.bInvPowerOn ) != 0 )
		{
			iNumOn ++;					//逆变供电总台数
		}
		if ( ( objMonInterface.m_st_wParallelData.ParallelState[i].Word1_U.tWord1Bits.bBattCapAlarm  ) != 0 )
		{
			iNumBattWarn ++;			//并机系统电池容量告警总台数
		}
		if ( ( objMonInterface.m_st_wParallelData.ParallelState[i].Word1_U.tWord1Bits.bBattInvPower ) != 0 )
		{
			iNumBattInv ++;				//并机系统电池逆变供电总台数
		}
		if ( ( objMonInterface.m_st_wParallelData.ParallelState[i].Word1_U.tWord1Bits.bSingleOnLine ) != 0 )
		{
			iNumUPS ++;					//并机系统实际上电总台数
		}

		
		//旁路上电台数
		if ( objMonInterface.m_st_wParallelData.ParallelState[i].Word1_U.tWord1Bits.bBypPowerOn )
		{
			iNumBp ++;					//并机系统旁路供电总台数
		}

		// 计算处在逆变供电状态下的各台机器的GrandKVA之和
		//TBD,将来考虑不同容量时需处理
	#if (0)
		if((objMonInterface.m_st_wParallelData.ParallelState[i].Word1_U.tWord1Bits.bInvPowerOn !=0)
		{
			iSumOfInvONCapacity += sParaData[i].m_i16wGrantKVA;
		}
	

		// 计算处在电池逆变状态下的各台机器的GrandKVA之和;
		if((sParaData[i].uSystemFlag & SBIT3)!=0)
		{
			iSumOfBattInvCapacity += sParaData[i].m_i16wGrantKVA;
		}


		// 计算处在电池预告警状态下的各台机器的GrandKVA之和;
		if((sParaData[i].uSystemFlag & SBIT2)!=0)
		{
			iSumOfBattWarCapacity += sParaData[i].m_i16wGrantKVA;
		}
	
			

		if ( ( sParaData[i].uSystemFlag & SBIT0 ) != 0 )
		{
			iCapacitySum_InvOn = iCapacitySum_InvOn + sParaData[i].m_i16wGrantKVA;

			//不同容量并联
			if (iMaxCapacityInv < sParaData[i].m_i16wGrantKVA)
			{
				iMaxCapacityInv = sParaData[i].m_i16wGrantKVA;	
			}

		}

		if ( ( sParaData[i].uSystemFlag & SBIT1 ) != 0 )
		{
			iCapacitySum_Invmod = iCapacitySum_Invmod + sParaData[i].m_i16wGrantKVA;

			//不同容量并联
			if (iMaxCapacityOn < sParaData[i].m_i16wGrantKVA)
			{
				iMaxCapacityOn = sParaData[i].m_i16wGrantKVA;	
			}

			//降额机型的不同容量并联
			iKCapSum_Invmod += (((INT32)sParaData[i].m_i16wGrantKVA)<<12)/(sParaData[i].m_i16ModelKVA);

		}

		//并机系统实际上电
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
						
		/* 合成并机系统的告警标志 */
		UINT16 uTmpBuff = ( objMonInterface.m_st_wParallelData.ParallelState[i].Word1_U.u16Word1All >> 4 ) & 0xF;
		iParaWarnFlag = iParaWarnFlag | uTmpBuff;
		
	}

	//更新当前值
	//本机架内的逆变开启台数
	objMonInterface.m_st_wParallelData.iNumInv = objPowCANSched.m_i16InvTotalNum;
	//接收总并机系统 逆变开启台数--包含其他机架的台数
	objMonInterface.m_st_wParallelData.iTotalNumInv = objPowCANSched.m_i16InvTotalNum;
	//本机架内的逆变供电台数
	objMonInterface.m_st_wParallelData.iNumOn = objPowCANSched.m_i16OnTotalNum;
	//接收总并机逆变供电台数--包含其他机架的台数
	objMonInterface.m_st_wParallelData.iTotalNumOn = objPowCANSched.m_i16OnTotalNum;
	objMonInterface.m_st_wParallelData.iNumBattWarn = iNumBattWarn;
	objMonInterface.m_st_wParallelData.iNumBattInv = iNumBattInv;
	objMonInterface.m_st_wParallelData.iNumUPS = iNumUPS;
	objMonInterface.m_st_wParallelData.iNumBp = iNumBp;
	
	objMonInterface.m_st_wParallelData.iParaWarnFlag = iParaWarnFlag;
	
}

/********************************************************************************
AnalogMsgRecvProc-----模拟量接收处理
                     	
Parameters:            pApdu:接收到的数据帧
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
FragXmitResult-----分断帧发送结果处理
                     	
Parameters:            pApdu:接收到的数据帧
				 i16Result:发送结果
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
			//事件记录和故障点
			break;

		default:
			break;

	}
	
	//TBD
	return 0;
}

/********************************************************************************
FragMsgPacked-----分断帧打包
                     	
Parameters:            pApdu:接收到的数据帧
				 
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
			//故障点
			FaultFragMsgPacked(pApdu);
			break;

		default:
			break;

	}

	//TBD
	return 0;
}

/********************************************************************************
FragMsgPacked-----分断帧打包
                     	
Parameters:            pApdu:接收到的数据帧
				 
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
FaultFragMsgPacked-----分断帧打包
                     	
Parameters:            pApdu:接收到的数据帧
				 
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
BattSelfChk20Percent-----20%放电自检
                     	
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
BattSelfChkDeep-----深度放电自检
                     	
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
BattSelfChkEnd-----放电自检结束
                     	
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
BattEodTurnOff-----电池电压低关机
                     	
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
BattTrip-----电池trip
                     	
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
OutputEnalbeCmd-----输出使能
                     	
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
OutputDisalbeCmd-----输出禁止
                     	
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
BattGndPtcCmd-----电池接地保护
                     	
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
FaultClrCmd-----故障清除
                     	
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
MaualStartupCmd-----手动开机命令
                     	
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
MaualShutCmd-----手动关机命令
                     	
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
EpoCmd-----EPO命令
                     	
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
AbnormalShutConfirmCmd-----异常关机确认命令
                     	
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
AbnormalShutCancelCmd-----异常关机取消命令
                     	
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
IndirectSwitchCmd-----间接切换命令
                     	
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
RecFlashUpdate-----整流Flash update命令
                     	
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
InvFlashUpdate-----逆变Flash update命令
                     	
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
BypFlashUpdate-----旁路Flash update命令
                     	
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
		//--3.状态事件COV
		if (m_StateMsgObj.CovObj[MODULE_BYP].bCovFlag == TRUE)
		{
			
			
			for (i=0;i<(sizeof(BypStateEventIdObj)/sizeof(EVENT_MSG_T));i++)
			{
				//第一个状态字
				if (BypStateEventIdObj[i].bEventInWord == 0)
				{
					//有变化,产生记录
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

						//TBD 将事件写入EEPROM相关缓冲区动作liuj
						
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

			//之前已有故障清除事件,先产生一个故障清除消失事件
			if (m_BypStateEventObj.CovObj[BYP_STATE_EVENT_OBJ_SIZE-1].bXmitActive == TRUE)
			{
				m_BypStateEventObj.CovObj[BYP_STATE_EVENT_OBJ_SIZE-1].bXmitActive = FALSE;
				m_BypStateEventObj.CovObj[BYP_STATE_EVENT_OBJ_SIZE-2].bCovFlag = TRUE;
				m_BypStateEventObj.CovObj[BYP_STATE_EVENT_OBJ_SIZE-2].bXmitActive = TRUE;
				m_BypStateEventObj.EventLog[BYP_STATE_EVENT_OBJ_SIZE-2].u16Second = objMonInterface.m_u16TimeLabel2_0;
				m_BypStateEventObj.EventLog[BYP_STATE_EVENT_OBJ_SIZE-2].u16Millisecond = objMonInterface.m_u16TimeLabel1_0;
				m_BypStateEventObj.EventLog[BYP_STATE_EVENT_OBJ_SIZE-2].u16ExEventID = ((BypStateEventIdObj[BYP_STATE_EVENT_OBJ_SIZE-2].uEventID) & (EVENT_CLEAR)) ;
			}

			//故障清除事件产生
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
		//--4.告警事件COV
		if (m_AlrmMsgObj.CovObj[MODULE_BYP].bCovFlag == TRUE)
		{
			for (i=0;i<(sizeof(BypAlarmEventIdObj)/sizeof(EVENT_MSG_T));i++)
			{
				//有变化,产生事件
				//告警字1
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
				

				//其他告警
				else
				{

				}
				
				
			}

			//TBD 将事件写入EEPROM相关缓冲区动作liuj

			m_BypAlarmMsgBak = objMonInterface.m_st_BypSendMonitor.m_st_BypWarn;
			
		}
}

/********************************************************************************
RecDiscreteMsgRecvProc-----机架内整流零散信号数据接收处理
                     	
Parameters:            pApdu:接收到的数据帧
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

		//接收数据
		objMonInterface.m_st_DiscreteState[u16Idx].Word1_U.u16Word1All			= pApdu->pDataPtr[0].u16Word;	

	}

	return i16NewMsg;
}


/********************************************************************************
BattPreWarn-----监控预告警就绪
                     	
Parameters:            
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanDataExchg::BattPreWarn(VOID)
{
	objPowCANSched.m_st_BatteryStatus.bBattPreWarn = 1;	//增加响应操作
}



VOID SysCanDataExchg::BattPreWarnCancel(VOID)
{
	objPowCANSched.m_st_BatteryStatus.bBattPreWarn = 0;	//取消响应操作	
}
/********************************************************************************
PostBattConvFltChg-----正组电池转浮充
                     	
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
NegBattConvFltChg-----负组电池转浮充
                     	
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
PostBattConvFltChg-----正组电池转均充
                     	
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
NegBattConvFltChg-----负组电池转均充
                     	
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

