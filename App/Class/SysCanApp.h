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
#ifndef SYS_CAN_APP_H
#define SYS_CAN_APP_H

//---------------------------------------------------------------------------
//Head files
//#include "Define.h"
#include "TemplateCmd.h"
#include "SysCanQueue.h"
//#include "TemplateQueue.h"
#include "TemplateList.h"
//#include "TimerObserver.h"
#include "SysCanProtocol.h"
#include "SysCanDataExchg.h"

//-----------------------------------------------------------------------------
// Public Enumerated and Structure Definitions
//CAN信息类别枚举
enum CAN_MSG_CLASS_ENUM
{
	CAN_CFG_MSG_CLASS = 1,		//Configuration Msg
	CAN_CTRL_MSG_CLASS,			//Control Msg
	CAN_ALRM_MSG_CLASS,			//ALarm Msg
	CAN_STATE_MSG_CLASS,		//Status Msg
	CAN_ANALOG_MSG_CLASS,		//Analog Msg
	CAN_MSG_CLASS_EOL			//
};



//-----------------------------------------------------------------------------
// Public Function Prototypes


//-----------------------------------------------------------------------------
//Class prototype
class SysCanApp
{
public:

	//Init(void);

	VOID MsgRecvProc(VOID);
	
	VOID MainRoutie(VOID);
	VOID SysCanInitial(VOID);
	

private:
	

	//COV function
	VOID CovMsgProc(VOID);
	VOID SyncMsgProc(VOID);
	
	VOID FaultDataXimt();

private:
	VOID RecvMsgDispatch(IN CAN_APDU_T *pApdu);
	VOID FragMsgPost(VOID);


	//注册到驱动层的命令
	CommandBase *m_pFragXmitOkCmd;
	INT16 FragXmitOkHandle(IN VOID *pArg);
	
	CommandBase *m_pFragXmitErrCmd;
	INT16 FragXmitErrHandle(IN VOID *pArg);
	
	CommandBase *m_pFragRecvOkCmd;
	INT16 FragRecvOkHandle(IN VOID *pApduMsg);

	//CommandBase *m_pFragRecvErrCmd;
	//INT16 FragRecvErrCmd(IN VOID *pArg);
	
	CommandBase *m_pRqMsgErrCmd;
	INT16 RqMsgErrHandle(IN VOID *pApduMsg){return 0;};

		
	//协议层对象
//	SysCanProtocol m_ProtocolObj;

	//应用层消息处理对象
//	SysCanDataExchg *m_pDataExchgObj;
	EVENT_LOG_FCB m_pEvtLogFcb;

//	TemplateQueue<CAN_APDU_T> *m_pFragXmitQueue;
	SysCanFragXmitQueue m_SysCanFragXmitQueue;
	
	WORD_TO_2BYTES_T m_FragMsgBuff[CAN_FRAG_DATA_LEN / 2];

};



#endif






