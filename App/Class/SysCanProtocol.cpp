/***************************************************************************************
* 
* Filename: 		SysCanProtocol.cpp
* Contributor:
* Author: 		Wang Zhihua	
* Date: 			1/21/2008
* Copyright 		2008 ENPC Corporation, All Rights Reserved
*
* Description: 	CAN protocol layer
*			
* 
* Version			0
* Last rev by:		Wang Zhihua
* Last rev date:	1/21/2008
* Last rev desc:	


****************************************************************************************/

//---------------------------------------------------------------
//Include header
#include "SysCanProtocol.h"

//--------------------------------------------------------------------------------
//Private Enumerated and macro Definitions

	//Transmit and receive queue szie
//	#define XMIT_QUEUE_SIZE 32
//	#define RECV_QUEUE_SZIE 16
	#define FRAG_FRAME_MAX_CNT 64

	#define HIGH_BYTE 1
	#define LOW_BYTE 0

	//fragment type
	enum FRAG_TYPE_ENUM
	{
		FIRST_FRAG = 0,
		MIDDLE_FRAG,
		LAST_FRAG,
		ACK_FRAG
	};

	//fragment ack msg type
	enum FRAG_ACK_ENUM
	{
		FRAG_ACK_STATE_PASS = 0,
		FRAG_ACK_STATE_FAIL,
		FRAG_ACK_STATE_BUSY
	};

	//��ʱ�����������
	#define NON_FRAG_RQ_TIMEOUT_MAX_CNT 2
	#define FRAG_ACK_TIMEOUT_MAX_CNT 2
	#define FRAG_XMIT_BUSY_TIMEOUTMAX_CNT 10
	#define  FRAG_RECV_TIMEOUT_MAX_CNT 1

	
	//��ʱ����ʱֵ10msʱ��
//	#define NON_FRAG_RQ_MSG_MAX_TIME_VAL 100
//	#define FRAG_ACK_MAX_TIME_VAL 100
//	#define FRAG_XMIT_BUSY_MAX_TIME_VAL 100
//	#define FRAG_RECV_MAX_TIME_VAL 100

	#define NON_FRAG_RQ_MSG_MAX_TIME_VAL 10
	#define FRAG_ACK_MAX_TIME_VAL 10
	#define FRAG_XMIT_BUSY_MAX_TIME_VAL 10
	#define FRAG_RECV_MAX_TIME_VAL 10

	#define NO_FREE_TIMER -1
	#define REPEATED_TIMER -2


	
//-------------------------------------------------------------------------------
//Private Structure and Const variable defintion

//--------------------------------------------------------------------------------
//public variable
//TemplateQueue<CAN_PACKED_PROTOCOL_U>  SysCanRecvQueueObj(RECV_QUEUE_SZIE);
//TemplateQueue<CAN_PACKED_PROTOCOL_U>  SysCanXmitQueueObj(XMIT_QUEUE_SIZE);

//ProtocolSubCmd CanIsrRecvCmd;
//ProtocolSubCmd CanIsrXmitCmd;

//--------------------------------------------------------------------------------
//private  function

//--------------------------------------------------------------------------------
//function definiton



//---------------------------------------------------------------
//Macro definition 



//------------------------------------------------------------------------------

#if (0)
/********************************************************************************
SysCanProtocol-----Constructor function
                     	
Parameters:            
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
SysCanProtocol::SysCanProtocol(VOID)
{
	INT16 i;
	UINT16 *pTemp;
	_TIMER_T TimerSetting;
	TimerSubject *pTimerObj;


	//��ȡʱ�����ָ��
	pTimerObj = TimerSubject::GetGblTimerPtr();

	//���ա����Ͷ��г�ʼ��
//	m_pSysCanRecvQueue = new TemplateQueue<CAN_PACKED_PROTOCOL_U>(RECV_QUEUE_SZIE);
//	m_pSysCanXmitQueue = new TemplateQueue<CAN_PACKED_PROTOCOL_U>(XMIT_QUEUE_SIZE);

	//Isr����ע��
	m_pCanIsrRecvCmd = new TemplateCmd<SysCanProtocol>(this, &SysCanProtocol::SysCanRecvFcb);
	m_pCanIsrXmitCmd = new TemplateCmd<SysCanProtocol>(this, &SysCanProtocol::SysCanXmitFcb);
	SysCanFcbRegister(m_pCanIsrRecvCmd, m_pCanIsrXmitCmd);
	
	//--------------------------------------------------------------------
	//�ֶ�֡����״̬��������ݽṹ��ʼ��
	for (i = 0, pTemp = (UINT16 *)&(m_FragXmitMsg.HeadInfo); i < sizeof(CAN_APDU_T); i++)
	{
		*pTemp++ = NULL;
	}
	
	m_FragXmitMsg.pData = (UINT16 *)(m_FragXmitMsg.HeadInfo.pDataPtr);
	m_FragXmitMsg.pSendData = m_FragXmitMsg.pData;
	
	m_FragXmitMsg.u16State = XMIT_STATE_IDLE;
	m_FragXmitMsg.u16OldState = XMIT_STATE_IDLE;

	m_FragXmitMsg.u16FragType = FIRST_FRAG;
	m_FragXmitMsg.u16FrameCnt = 0;
	m_FragXmitMsg.u16ByteSentCount = 0;

	//Ӧ���ʱʱ��ʼ��
	TimerSetting.lSeconds = 0;
	TimerSetting.i16MilliSeconds = FRAG_ACK_MAX_TIME_VAL;
	m_FragXmitMsg.AckTimeHandle.u16TimeOutMaxCnt = FRAG_ACK_TIMEOUT_MAX_CNT;
	m_FragXmitMsg.AckTimeHandle.u16TimeOutCnt = 0;
	m_FragXmitMsg.AckTimeHandle.pTimeOutFcb = &SysCanProtocol::FragAckTimeoutFcb;
	m_FragXmitMsg.AckTimeHandle.pTimeOutObsvr = new SysCanTimer<SysCanProtocol> (TimerSetting, 
												TIMER_ONE_SHOT, this, 
												m_FragXmitMsg.AckTimeHandle.pTimeOutFcb);
	pTimerObj->TimerNew(m_FragXmitMsg.AckTimeHandle.pTimeOutObsvr);

	//busy��ʱ��ʼ��
	TimerSetting.i16MilliSeconds = FRAG_XMIT_BUSY_MAX_TIME_VAL;
	m_FragXmitMsg.BusyTimeHandle.u16TimeOutMaxCnt = FRAG_XMIT_BUSY_TIMEOUTMAX_CNT;
	m_FragXmitMsg.BusyTimeHandle.u16TimeOutCnt = 0;
	m_FragXmitMsg.BusyTimeHandle.pTimeOutFcb = &SysCanProtocol::FragXmitBusyTimeoutFcb;
	m_FragXmitMsg.BusyTimeHandle.pTimeOutObsvr = new SysCanTimer<SysCanProtocol>(TimerSetting, 
												TIMER_ONE_SHOT, this, 
												m_FragXmitMsg.BusyTimeHandle.pTimeOutFcb);
	pTimerObj->TimerNew(m_FragXmitMsg.BusyTimeHandle.pTimeOutObsvr);

	//���ϴ�����
	m_FragXmitMsg.pErrCallBack = NULL;

	//--------------------------------------------------------------------------
	//�ֶ�֡����״̬����ʼ��
	for (i = 0, pTemp = (UINT16 *)&(m_FragRecvMsg.HeadInfo); i < sizeof(CAN_APDU_T); i++)
	{
		*pTemp++ = NULL;
	}
	
	for (i = 0; i < (CAN_FRAG_DATA_LEN / 2); i++)
	{
		m_FragRecvMsg.Data[i].u16Word = 0;
	}
	m_FragRecvMsg.HeadInfo.pDataPtr = m_FragRecvMsg.Data;
	m_FragRecvMsg.pData = (UINT16*)(m_FragRecvMsg.Data);
	m_FragRecvMsg.u16State = RECV_STATE_IDLE;
	m_FragRecvMsg.u16OldState = RECV_STATE_IDLE;
	m_FragRecvMsg.u16FragType = FIRST_FRAG;
	m_FragRecvMsg.u16FrameCnt = 0;

	//��ʱ����ʼ��
	TimerSetting.i16MilliSeconds = FRAG_RECV_MAX_TIME_VAL;
	m_FragRecvMsg.TimeHandle.u16TimeOutMaxCnt = FRAG_RECV_TIMEOUT_MAX_CNT;
	m_FragRecvMsg.TimeHandle.u16TimeOutCnt = 0;
	m_FragRecvMsg.TimeHandle.pTimeOutFcb = &SysCanProtocol::CANFragRecvTimeoutFcb;
	m_FragRecvMsg.TimeHandle.pTimeOutObsvr = new SysCanTimer<SysCanProtocol>(TimerSetting, 
											TIMER_ONE_SHOT, this, 
											m_FragRecvMsg.TimeHandle.pTimeOutFcb);
	pTimerObj->TimerNew(m_FragRecvMsg.TimeHandle.pTimeOutObsvr);


}

/********************************************************************************
SysCanProtocol-----Destructor function
                     	
Parameters:            
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
SysCanProtocol::~SysCanProtocol(VOID)
{
//	m_pSysCanXmitQueue = 
}
#endif


//Э�������ʼ������
VOID SysCanProtocol::SysCanProtocolIni(VOID)
{
	INT16 i;
	UINT16 *pTemp;
//	_TIMER_T TimerSetting;
//	TimerSubject *pTimerObj;


	//��ȡʱ�����ָ��
//	pTimerObj = TimerSubject::GetGblTimerPtr();

	//���ա����Ͷ��г�ʼ��
//	m_pSysCanRecvQueue = new TemplateQueue<CAN_PACKED_PROTOCOL_U>(RECV_QUEUE_SZIE);
//	m_pSysCanXmitQueue = new TemplateQueue<CAN_PACKED_PROTOCOL_U>(XMIT_QUEUE_SIZE);
//	m_pSysCanRecvQueue = &SysCanRecvQueueObj;
//	m_pSysCanXmitQueue = &SysCanXmitQueueObj;
	//���г�ʼ��
	m_SysCanRecvQueue.SysCanRecvQueueIni();
	m_SysCanXmitQueue.SysCanXmitQueueIni();

	//Isr����ע��
//	m_pCanIsrRecvCmd = new TemplateCmd<SysCanProtocol>(this, &SysCanProtocol::SysCanRecvFcb);
//	m_pCanIsrXmitCmd = new TemplateCmd<SysCanProtocol>(this, &SysCanProtocol::SysCanXmitFcb);
	m_CanIsrRecvCmd.ProtocolSubCmdIni(this, &SysCanProtocol::SysCanRecvFcb);
	m_CanIsrXmitCmd.ProtocolSubCmdIni(this, &SysCanProtocol::SysCanXmitFcb);
	m_pCanIsrRecvCmd = &m_CanIsrRecvCmd;
	m_pCanIsrXmitCmd = &m_CanIsrXmitCmd;
	SysCanFcbRegister(m_pCanIsrRecvCmd, m_pCanIsrXmitCmd);
	
	//--------------------------------------------------------------------
	//�ֶ�֡����״̬��������ݽṹ��ʼ��
	for (i = 0, pTemp = (UINT16 *)&(m_FragXmitMsg.HeadInfo); i < sizeof(CAN_APDU_T); i++)
	{
		*pTemp++ = NULL;
	}
	
	m_FragXmitMsg.pData = (UINT16 *)(m_FragXmitMsg.HeadInfo.pDataPtr);
	m_FragXmitMsg.pSendData = m_FragXmitMsg.pData;
	
	m_FragXmitMsg.u16State = XMIT_STATE_IDLE;
	m_FragXmitMsg.u16OldState = XMIT_STATE_IDLE;

	m_FragXmitMsg.u16FragType = FIRST_FRAG;
	m_FragXmitMsg.u16FrameCnt = 0;
	m_FragXmitMsg.u16ByteSentCount = 0;

	//Ӧ���ʱʱ��ʼ��

//	TimerSetting.lSeconds = 0;
//	TimerSetting.i16MilliSeconds = FRAG_ACK_MAX_TIME_VAL;
	m_FragXmitMsg.AckTimeHandle.u16TimeOutMaxCnt = FRAG_ACK_TIMEOUT_MAX_CNT;
	m_FragXmitMsg.AckTimeHandle.u16TimeOutCnt = 0;
	m_FragXmitMsg.AckTimeHandle.u16TimeExpireCnt = FRAG_ACK_MAX_TIME_VAL;
	m_FragXmitMsg.AckTimeHandle.u16TimeDelayCnt = 0;
//	m_FragXmitMsg.AckTimeHandle.pTimeOutFcb = &SysCanProtocol::FragAckTimeoutFcb;

	

	//TBD ???LIUJ
//	m_AckTimeOutObsvr(TimerSetting, TIMER_ONE_SHOT, this, m_FragXmitMsg.AckTimeHandle.pTimeOutFcb);
	
//	m_FragXmitMsg.AckTimeHandle.pTimeOutObsvr = &m_AckTimeOutObsvr;
	//ע��ACK��ʱ��
//	pTimerObj->TimerNew(m_FragXmitMsg.AckTimeHandle.pTimeOutObsvr);

	//busy��ʱ��ʼ��
//	TimerSetting.i16MilliSeconds = FRAG_XMIT_BUSY_MAX_TIME_VAL;
	m_FragXmitMsg.BusyTimeHandle.u16TimeOutMaxCnt = FRAG_XMIT_BUSY_TIMEOUTMAX_CNT;
	m_FragXmitMsg.BusyTimeHandle.u16TimeOutCnt = 0;
	m_FragXmitMsg.BusyTimeHandle.u16TimeExpireCnt = FRAG_XMIT_BUSY_MAX_TIME_VAL;
	m_FragXmitMsg.BusyTimeHandle.u16TimeDelayCnt = 0;
//	m_FragXmitMsg.BusyTimeHandle.pTimeOutFcb = &SysCanProtocol::FragXmitBusyTimeoutFcb;
//	m_BusyTimeOutObsvr(TimerSetting, TIMER_ONE_SHOT, this, m_FragXmitMsg.BusyTimeHandle.pTimeOutFcb);
//	m_FragXmitMsg.BusyTimeHandle.pTimeOutObsvr = &m_BusyTimeOutObsvr;
//	pTimerObj->TimerNew(m_FragXmitMsg.BusyTimeHandle.pTimeOutObsvr);


	//���ϴ�����
	m_FragXmitMsg.pErrCallBack = NULL;

	//--------------------------------------------------------------------------
	//�ֶ�֡����״̬����ʼ��
	for (i = 0, pTemp = (UINT16 *)&(m_FragRecvMsg.HeadInfo); i < sizeof(CAN_APDU_T); i++)
	{
		*pTemp++ = NULL;
	}
	
	for (i = 0; i < (CAN_FRAG_DATA_LEN / 2); i++)
	{
		m_FragRecvMsg.Data[i].u16Word = 0;
	}
	m_FragRecvMsg.HeadInfo.pDataPtr = m_FragRecvMsg.Data;
	m_FragRecvMsg.pData = (UINT16*)(m_FragRecvMsg.Data);
	m_FragRecvMsg.u16State = RECV_STATE_IDLE;
	m_FragRecvMsg.u16OldState = RECV_STATE_IDLE;
	m_FragRecvMsg.u16FragType = FIRST_FRAG;
	m_FragRecvMsg.u16FrameCnt = 0;

	//��ʱ����ʼ��

//	TimerSetting.i16MilliSeconds = FRAG_RECV_MAX_TIME_VAL;
	m_FragRecvMsg.TimeHandle.u16TimeOutMaxCnt = FRAG_RECV_TIMEOUT_MAX_CNT;
	m_FragRecvMsg.TimeHandle.u16TimeOutCnt = 0;
	m_FragRecvMsg.TimeHandle.u16TimeExpireCnt = FRAG_RECV_MAX_TIME_VAL;
//	m_FragRecvMsg.TimeHandle.pTimeOutFcb = &SysCanProtocol::CANFragRecvTimeoutFcb;
//	m_FragTimeOutObsvr(TimerSetting, TIMER_ONE_SHOT, this, m_FragRecvMsg.TimeHandle.pTimeOutFcb);
//	m_FragRecvMsg.TimeHandle.pTimeOutObsvr = &m_FragTimeOutObsvr;
//	pTimerObj->TimerNew(m_FragRecvMsg.TimeHandle.pTimeOutObsvr);


	//������ʱ��ע��
	//ע��ACK��ʱ��
//	m_FragXmitMsg.ucFragAckTimerID = SysCanTimerObj.Timer_Srv_New(SysCanProtocol::FragAckTimeoutFcb,this);

	//ע��BUSY��ʱ��
//	m_FragXmitMsg.ucFragBusyTimerID = SysCanTimerObj.Timer_Srv_New(SysCanProtocol::FragXmitBusyTimeoutFcb);
	
	//ע��RECEV��ʱ��
//	m_FragRecvMsg.ucFragRecvTimerID = SysCanTimerObj.Timer_Srv_New(SysCanProtocol::CANFragRecvTimeoutFcb);
	
}

/********************************************************************************
SysCanXmitFcb-----�����Ͷ��е���Ϣд��CAN���ͼĴ��������жϳ������
Transmit Forms Control Buffer                     	
Parameters:              pUnused: unused
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanProtocol::SysCanXmitFcb(VOID *pUnused)
{
	CAN_PACKED_PROTOCOL_U TempPacked;

	//queue is not null
	if ( QUEUE_NULL != m_SysCanXmitQueue.FlagQueue())
	{
		//remove from stransmit queue
		TempPacked = m_SysCanXmitQueue.GetOldestNode();
		m_SysCanXmitQueue.DelQueue();

		//set sending flag
		m_u16CanXmitBusy = TRUE;

		//TBD toggle CAN XMIT state LED

		//Transmit msg
		SysCanSendData(&(TempPacked.Frame));

	}

	//queue is null
	else
	{
		//clear sending flag
		m_u16CanXmitBusy = FALSE;
	}

	//TBD
	return 0;
}

/********************************************************************************
SysCanRecvFcb-----�����յ�����Ӧ��Ϣ�浽���У����жϳ������
                     	
Parameters:              pUnused: unused
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanProtocol::SysCanRecvFcb(IN VOID *pFrame)
{
	m_CurRecvedFrame.Frame = *(SYS_CAN_FRAME *)pFrame; 

	//only for this node or broadcast msg will be received
	//if the received the all len of bytes is abnormal it will not be received
	if (((objMonInterface.m_i16wSerialNumSet_0 == m_CurRecvedFrame.PackedMsg.b6DestinationMacId)
		|| (MAC_ID_BROADCAST == m_CurRecvedFrame.PackedMsg.b6DestinationMacId)
		|| (MAC_ID_BYP == m_CurRecvedFrame.PackedMsg.b6DestinationMacId))
			&& ((m_CurRecvedFrame.PackedMsg.u16Dlc >= 2) && (m_CurRecvedFrame.PackedMsg.u16Dlc <= 8) ))
	{
		m_SysCanRecvQueue.InsQueue(m_CurRecvedFrame);
	}
	
	//TBD
	return 0;
}

/********************************************************************************
MsgRecvProc-----��Ϣ���մ���,Ӧ�ò�ӿ�
                     	
Parameters:              pUnused: unused
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanProtocol::MsgRecvProc(OUT CAN_APDU_T *pApdu)
{
	CAN_PACKED_PROTOCOL_U TempPacked;
	UINT16 u16FragType;
	INT16 ret = 0;

	//queue is not null
	while ( QUEUE_NULL != m_SysCanRecvQueue.FlagQueue())
	{
		//remove from receive queue
		TempPacked = m_SysCanRecvQueue.GetOldestNode();

		//if closed CAN INT?  TBD
		m_SysCanRecvQueue.DelQueue();

		// bus off error
		if (CAN_STATUS_ERROR_BUS_OFF == TempPacked.Frame.CANErrorStatus)
		{
			//reset CAN controller, TBD 

			//toggle CAN error status LED, TBD
			
		}

		//Valid frame
		else
		{
			//fragment msg processing
			if (FRAG_MSG == TempPacked.PackedMsg.b1Fragment)
			{
				u16FragType = (TempPacked.PackedMsg.MsgData[0].ByteWide.Low) >> MSG_FRAG_TYPE_OFFSET;

				if (ACK_FRAG == u16FragType)
				{
					//���յ�ACK�����ŷ�����һ���ֶ�֡
					FragMsgXmitStateMachine(&TempPacked);
				}

				//�ֶ�֡����
				else
				{
					FragMsgRecvStateMachine(&TempPacked);
				}
			}

			//Nonfrag msg process
			else
			{
				//Parse HAL frame to application layer frame 
				FrameParseToApdu(&(m_NonFragRecvMsg.HeadInfo), &TempPacked);

				//return msg to application layer
				*pApdu = m_NonFragRecvMsg.HeadInfo;

				ret = sizeof(CAN_APDU_T);

				return ret;
			}
		}
	}

	return ret;
}

/********************************************************************************
MsgRecvProc-----��Ϣ���ʹ���,Ӧ�ò�ӿ�
                     	
Parameters:              pUnused: unused
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanProtocol::MsgXmitProc(CAN_APDU_T *pApdu)
{
	INT16 ret = 0;
	CAN_PACKED_PROTOCOL_U TempFrame;

	if (FRAG_MSG == pApdu->u16Fragment)
	{
		if (TRUE == m_FragXmitMsg.u16Allocted)
		{
			ret = -1;
		}
		else
		{
			m_FragXmitMsg.HeadInfo = *pApdu;
			FragMsgXmitStateMachineEnty();
		}
	}

	else
	{
		
		FrameParseToPpdu(&TempFrame, pApdu);
		ret = XmitFramePost(&TempFrame);
	}

	return ret;
}

/********************************************************************************
MsgXmitProc_Test-----��Ϣ���ʹ���,Ӧ�ò�ӿ�
                     	
Parameters:              pUnused: unused
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanProtocol::MsgXmitProc_Test(void)
{
	INT16 ret = 0;
	CAN_PACKED_PROTOCOL_U TempFrame;

	CAN_APDU_T tTmpApdu;
	tTmpApdu.u16ByteLength = 4;
	tTmpApdu.u16DestinationMacId= 0x01;
	tTmpApdu.u16Fragment=0;
	tTmpApdu.u16MsgClass=0x04;
	tTmpApdu.u16RsRq=1;
	tTmpApdu.u16ServiceCode=0x01;
	tTmpApdu.u16SourceMacId=0x10;
	tTmpApdu.pDataPtr[0].u16Word=0x55;
	tTmpApdu.pDataPtr[1].u16Word=0xaa;
	
	FrameParseToPpdu(&TempFrame, &tTmpApdu);
#if(0)
	TempFrame.PackedMsg.b1RsRq = tTmpApdu.u16RsRq;
	TempFrame.PackedMsg.b10MsgClass = tTmpApdu.u16MsgClass;
	TempFrame.PackedMsg.b6SourceMacId = tTmpApdu.u16SourceMacId;
	TempFrame.PackedMsg.b6DestinationMacId = tTmpApdu.u16DestinationMacId;
	TempFrame.PackedMsg.b7ServiceCode = tTmpApdu.u16ServiceCode;
	TempFrame.PackedMsg.b1Fragment = tTmpApdu.u16Fragment;
	TempFrame.PackedMsg.u16Dlc = tTmpApdu.u16ByteLength + NONFRAG_HEAD_BYTE_SIZE;
#endif	
	ret = XmitFramePost(&TempFrame);
	return ret;
}


/********************************************************************************
FrameParseToApdu-----Transfer message frame from Driver PDU to Application PDU
                     	
Parameters:              pUnused: unused
Return Value: 
Precondition:   Application Protocol Data Unit
Postcondition: 
********************************************************************************/
VOID SysCanProtocol::FrameParseToApdu(INOUT CAN_APDU_T *pApdu,
										IN CAN_PACKED_PROTOCOL_U *pFrame)
{
	UINT16 i;
	
	pApdu->u16RsRq = pFrame->PackedMsg.b1RsRq;
	pApdu->u16MsgClass = pFrame->PackedMsg.b10MsgClass;
	pApdu->u16SourceMacId = pFrame->PackedMsg.b6SourceMacId;
	pApdu->u16DestinationMacId = pFrame->PackedMsg.b6DestinationMacId;
	pApdu->u16ServiceCode = pFrame->PackedMsg.b7ServiceCode;
	pApdu->u16Fragment = pFrame->PackedMsg.b1Fragment;

	//change by liuj 20080524
//	if (pApdu->u16Fragment == FRAG_MSG)
	if (pApdu->u16Fragment != FRAG_MSG)
	{
		//���ݳ����쳣����,��ֹԽ���ܷ�
		if ((pFrame->PackedMsg.u16Dlc < 2) ||(pFrame->PackedMsg.u16Dlc > 8) )
		{
			//MSG CLASS=0Ϊ����,�˴��ñ�ʾ���ݸ�Ӧ�ò�Ϊһ��Ч����Ϣ���
			pApdu->u16MsgClass = 0;
		}
		
		else
		{
			pApdu->u16ByteLength = pFrame->PackedMsg.u16Dlc - NONFRAG_HEAD_BYTE_SIZE;
		
			for (i = 0; i < pApdu->u16ByteLength; i++)
			{
				if (i % 2 == 0)
				{
					pApdu->pDataPtr[i /2].ByteWide.Low = pFrame->PackedMsg.MsgData[i /2].ByteWide.Low;
				}
				else
				{
					pApdu->pDataPtr[i /2].ByteWide.High = pFrame->PackedMsg.MsgData[i /2].ByteWide.High;
				}
			}
		}
	}
}

/********************************************************************************
FrameParseToApdu-----Transfer message frame from Driver PDU to Application PDU
                     	
Parameters:              pUnused: unused
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/

VOID SysCanProtocol::FrameParseToPpdu(OUT CAN_PACKED_PROTOCOL_U *pFrame,
													IN CAN_APDU_T *pApdu)
{
	UINT16 i;

	pFrame->PackedMsg.b1RsRq = pApdu->u16RsRq;
	pFrame->PackedMsg.b10MsgClass = pApdu->u16MsgClass;
	pFrame->PackedMsg.b6SourceMacId = pApdu->u16SourceMacId;
	pFrame->PackedMsg.b6DestinationMacId = pApdu->u16DestinationMacId;
	pFrame->PackedMsg.b7ServiceCode = pApdu->u16ServiceCode;
	pFrame->PackedMsg.b1Fragment = pApdu->u16Fragment;

	if (pApdu->u16Fragment == NONFRAG_MSG)
	{
		pFrame->PackedMsg.u16Dlc = pApdu->u16ByteLength + NONFRAG_HEAD_BYTE_SIZE;
		
		for (i = 0; i < pApdu->u16ByteLength; i++)
		{
			if (i % 2 == 0)
			{
				pFrame->PackedMsg.MsgData[i /2].ByteWide.Low = pApdu->pDataPtr[i /2].ByteWide.Low;
			}

			else
			{
				pFrame->PackedMsg.MsgData[i /2].ByteWide.High = pApdu->pDataPtr[i /2].ByteWide.High;
			}
		}
	}
}
/********************************************************************************
FragMsgRecvStateMachine-----Fragment Frame receiving state machine
                     	
Parameters:              pCanPacked: reveived fragment message
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanProtocol::FragMsgRecvStateMachine(IN CAN_PACKED_PROTOCOL_U *pCanPacked)
{
	UINT16 u16FrameType,  u16AckType, i;
//	UINT16 u16RecvByteCnt;
	//timer Delay ms num
//	unsigned int msDelay;

	UINT16 u16SourceByteIdx, u16SourceWordIdx, u16DesByteIdx, u16DesWordIdx;

	UCHAR ucTempData;

	CAN_PACKED_PROTOCOL_U TempFrame;

//	TimerSubject *pTimerSubject = TimerSubject::GetGblTimerPtr();
	
	do
	{
		switch (m_FragRecvMsg.u16State)
		{
			//����״̬�����յ�һ֡
			case RECV_STATE_IDLE:
				if (m_FragRecvMsg.u16State != m_FragRecvMsg.u16OldState)
				{
					m_FragRecvMsg.u16OldState = m_FragRecvMsg.u16State;
					m_FragRecvMsg.u16Allocated = FALSE;
				}

				if ( NULL != pCanPacked)
				{
					u16FrameType = pCanPacked->PackedMsg.MsgData[0].ByteWide.Low >> MSG_FRAG_TYPE_OFFSET;
					if (FIRST_FRAG == u16FrameType)
					{
						m_FragRecvMsg.u16Allocated = TRUE;
						m_FragRecvMsg.HeadInfo.u16MsgClass = pCanPacked->PackedMsg.b10MsgClass;
						m_FragRecvMsg.HeadInfo.u16Fragment = FRAG_MSG;
						m_FragRecvMsg.HeadInfo.u16RsRq = pCanPacked->PackedMsg.b1RsRq;
						m_FragRecvMsg.HeadInfo.u16ServiceCode = pCanPacked->PackedMsg.b7ServiceCode;
						m_FragRecvMsg.HeadInfo.u16SourceMacId = pCanPacked->PackedMsg.b6SourceMacId;
						
						m_FragRecvMsg.HeadInfo.u16ByteLength = 0;
						m_FragRecvMsg.HeadInfo.pDataPtr = m_FragRecvMsg.Data;
						
						m_FragRecvMsg.u16FrameCnt = 0;

						//���յ���ȷ�ĵ�һ֡��״̬ת�Ƶ��ֶ�֡����״̬
						m_FragRecvMsg.u16State = RECV_STATE_RECV_FRAG;
					}
				}

				
				break;

			case RECV_STATE_RECV_FRAG:
				if (m_FragRecvMsg.u16State != m_FragRecvMsg.u16OldState)
				{
					m_FragRecvMsg.u16OldState = m_FragRecvMsg.u16State;
				}

				//check frame count
				//֡����ڷ�Χ��
				if (((pCanPacked->PackedMsg.MsgData[0].ByteWide.Low & FRAG_COUNT_MASK) == (m_FragRecvMsg.u16FrameCnt -1))
				|| ((pCanPacked->PackedMsg.MsgData[0].ByteWide.Low & FRAG_COUNT_MASK) == m_FragRecvMsg.u16FrameCnt))
				{
					u16AckType = FRAG_ACK_STATE_PASS;
					m_FragRecvMsg.u16State = RECV_STATE_WAIT_FOR_FRAG;

					if ((pCanPacked->PackedMsg.MsgData[0].ByteWide.Low & FRAG_COUNT_MASK) == m_FragRecvMsg.u16FrameCnt)
					{
						//u16RecvByteCnt = 0;
						//i = 0;
										
						for (i = 0; i < (pCanPacked->Frame.CANDlc - FRAG_HEAD_BYTE_SZIE); i++)
						{

							//compute data section index of received frame 
							u16SourceByteIdx = (i + 1) % 2;
							u16SourceWordIdx = (i + 1) / 2;

							//compute receive buffer index
							if ((m_FragRecvMsg.HeadInfo.u16ByteLength % 2) == 0)
							{
								u16DesByteIdx = i % 2;
								u16DesWordIdx = m_FragRecvMsg.HeadInfo.u16ByteLength / 2 + i / 2; 
							}

							else
							{
								u16DesByteIdx = (i + 1) % 2;
								u16DesWordIdx = m_FragRecvMsg.HeadInfo.u16ByteLength / 2 + (i + 1) / 2; 
							}

							//remove a byte from received frame data section 
							if (u16SourceByteIdx == HIGH_BYTE)	
							{
								ucTempData = pCanPacked->PackedMsg.MsgData[u16SourceWordIdx].ByteWide.High;
							}
							else
							{
								ucTempData = pCanPacked->PackedMsg.MsgData[u16SourceWordIdx].ByteWide.Low;
							}

							//save a  byte to receive buffer according index
							if (u16DesByteIdx == HIGH_BYTE)
							{
								m_FragRecvMsg.Data[u16DesWordIdx].ByteWide.High = ucTempData;
							}

							else
							{
								m_FragRecvMsg.Data[u16DesWordIdx].ByteWide.Low = ucTempData;
							}
													
							// check receive buffer expired
							//֡���洦���ص�����״̬����Ӧ��ACKʧ��
							if ((m_FragRecvMsg.HeadInfo.u16ByteLength + i) >= CAN_FRAG_DATA_LEN)
							{
								i =  pCanPacked->Frame.CANDlc - FRAG_HEAD_BYTE_SZIE;
								u16AckType = FRAG_ACK_STATE_FAIL;
								m_FragRecvMsg.u16State = RECV_STATE_IDLE;
							}
						}
						
					}

					m_FragRecvMsg.u16FrameCnt++;
					if (m_FragRecvMsg.u16FrameCnt >= FRAG_FRAME_MAX_CNT)
					{
						m_FragRecvMsg.u16FrameCnt = 0;
					}

					//���յ��������һ֡ת���ݴ���״̬
					if (((pCanPacked->PackedMsg.MsgData[0].ByteWide.Low) >> MSG_FRAG_TYPE_OFFSET) == LAST_FRAG)
					{
						m_FragRecvMsg.u16State = RECV_STATE_HANDLE_DATA;
					}

					//sending ack msg
					TempFrame.PackedMsg.b10MsgClass = pCanPacked->PackedMsg.b10MsgClass;
					TempFrame.PackedMsg.b6SourceMacId = objMonInterface.m_i16wSerialNumSet_0;
					TempFrame.PackedMsg.b6DestinationMacId = pCanPacked->PackedMsg.b6SourceMacId;
					TempFrame.PackedMsg.b1Fragment = FRAG_MSG;
					TempFrame.PackedMsg.b7ServiceCode = pCanPacked->PackedMsg.b7ServiceCode;
					TempFrame.PackedMsg.b1RsRq = RS_MSG;
					TempFrame.PackedMsg.u16Dlc = FRAG_HEAD_BYTE_SZIE + 1;

					TempFrame.PackedMsg.MsgData[0].ByteWide.Low = m_FragRecvMsg.u16FrameCnt;
					TempFrame.PackedMsg.MsgData[0].ByteWide.Low |= (ACK_FRAG << MSG_FRAG_TYPE_OFFSET);
					TempFrame.PackedMsg.MsgData[0].ByteWide.High = u16AckType;
				
					//Post the msg to Xmit queue
					XmitFramePost(&TempFrame);
					pCanPacked = NULL;

					//�������ն�ʱ��
					//Timer_Post( &m_FragRecvMsg.TimeHandle);
					
				}

				//֡������ȷ��ֱ�ӻص�����״̬������Ӧ��
				else
				{
					m_FragRecvMsg.u16State = RECV_STATE_IDLE;
					//should clear pCanPacked
					pCanPacked = NULL;
					
					//Timer Post TBD,Delay 100ms
					//pTimerSubject->TimerPost(m_FragRecvMsg.TimeHandle.pTimeOutObsvr);
					//TBD liuj20060525 ������ʱ��Ӧ������һ��״̬����
					//�˴�Ӧ���ζ�ʱ������
					//Timer_Post( &m_FragRecvMsg.TimeHandle);
					
				}

				break;
			
			case RECV_STATE_WAIT_FOR_FRAG:
				if (m_FragRecvMsg.u16State != m_FragRecvMsg.u16OldState)
				{
					m_FragRecvMsg.u16OldState = m_FragRecvMsg.u16State;
					
					//�������ն�ʱ��
					Timer_Post( &m_FragRecvMsg.TimeHandle);
				}

				if (NULL == pCanPacked)
				{

				}

				//Check service code, message class, source MAC ID
				else if ((pCanPacked->PackedMsg.b10MsgClass == m_FragRecvMsg.HeadInfo.u16MsgClass)
					&& (pCanPacked->PackedMsg.b6SourceMacId == m_FragRecvMsg.HeadInfo.u16SourceMacId)
					&& (pCanPacked->PackedMsg.b7ServiceCode == m_FragRecvMsg.HeadInfo.u16ServiceCode))
				{
					m_FragRecvMsg.u16State = RECV_STATE_RECV_FRAG;

					//Timer flush,Disable Recv timer
					//pTimerSubject->TimerFlush(m_FragRecvMsg.TimeHandle.pTimeOutObsvr);
					//SysCanTimerObj.Timer_Srv_Flush(m_FragRecvMsg.ucFragRecvTimerID);
					Timer_Flush( &m_FragRecvMsg.TimeHandle);
				}

				//wrong frame, send a busy ACK frame
				else
				{
					TempFrame = *pCanPacked;
					TempFrame.PackedMsg.b6DestinationMacId = pCanPacked->PackedMsg.b6SourceMacId;
					TempFrame.PackedMsg.b6SourceMacId = objMonInterface.m_i16wSerialNumSet_0;
					TempFrame.PackedMsg.b1RsRq = RS_MSG;
					TempFrame.PackedMsg.u16Dlc = FRAG_HEAD_BYTE_SZIE + 1;
					TempFrame.PackedMsg.MsgData[0].ByteWide.Low &= FRAG_COUNT_MASK;
					TempFrame.PackedMsg.MsgData[0].ByteWide.Low |= (ACK_FRAG << MSG_FRAG_TYPE_OFFSET);
					TempFrame.PackedMsg.MsgData[0].ByteWide.High = FRAG_ACK_STATE_BUSY;

					//Post msg
					XmitFramePost(&TempFrame);
					pCanPacked = NULL;
				}

				break;

			case RECV_STATE_HANDLE_DATA:
				if (m_FragRecvMsg.u16State != m_FragRecvMsg.u16OldState)
				{
					m_FragRecvMsg.u16OldState = m_FragRecvMsg.u16State;
				}

				//Call Back function
				m_FragRecvMsg.pSuccessCallBack->Excute(&(m_FragRecvMsg.HeadInfo));
				
				m_FragRecvMsg.u16State = RECV_STATE_IDLE;

				break;

			default:
				m_FragRecvMsg.u16State = RECV_STATE_IDLE;
				break;

		}
	}while (m_FragRecvMsg.u16State != m_FragRecvMsg.u16OldState);
}

/********************************************************************************
FragMsgXmitStateMachineEnty-----The entry of fragment frame transmitting state machine
                     	
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanProtocol::FragMsgXmitStateMachineEnty(VOID)
{
	//�ֶ�֡����ָ��
	//m_FragXmitMsg.HeadInfo��Ϊ�����͵�Դʼ��Ϣ
	m_FragXmitMsg.pData = (UINT16 *)(m_FragXmitMsg.HeadInfo.pDataPtr);

	//�ֶ�֡��ر���
	m_FragXmitMsg.u16FrameCnt = 0;
	m_FragXmitMsg.u16FragType = FIRST_FRAG;
	m_FragXmitMsg.u16State = XMIT_STATE_XMIT;
	m_FragXmitMsg.u16ByteSentCount = 0;
	m_FragXmitMsg.AckTimeHandle.u16TimeOutCnt = 0;
	m_FragXmitMsg.BusyTimeHandle.u16TimeOutCnt = 0;
	m_FragXmitMsg.u16Allocted = TRUE;

	FragMsgXmitStateMachine(NULL);		
}


/********************************************************************************
FragMsgRecvStateMachine-----Fragment Frame transmitting state machine
                     	
Parameters:              pCanPacked: reveived fragment ACK message
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanProtocol::FragMsgXmitStateMachine(IN CAN_PACKED_PROTOCOL_U *pCanPacked)
{
	CAN_PACKED_PROTOCOL_U TempFrame;
	UINT16 u16SendCount, i;
	UINT16 u16SourceByteIdx, u16SourceWordIdx, u16DesByteIdx, u16DesWordIdx;
	UCHAR ucTempData;
//	UINT16 msDelay;
//	TimerSubject *pTimerSubject = TimerSubject::GetGblTimerPtr();

	do
	{
		switch (m_FragXmitMsg.u16State)
		{
			case XMIT_STATE_IDLE:
				if (m_FragXmitMsg.u16State != m_FragXmitMsg.u16OldState)
				{
					m_FragXmitMsg.u16OldState = m_FragXmitMsg.u16State;

				}

				break;

			case XMIT_STATE_XMIT:
				if (m_FragXmitMsg.u16State != m_FragXmitMsg.u16OldState)
				{
					m_FragXmitMsg.u16OldState = m_FragXmitMsg.u16State;
				}

				//Transmit fragment message
				TempFrame.PackedMsg.b10MsgClass = m_FragXmitMsg.HeadInfo.u16MsgClass;
				TempFrame.PackedMsg.b6SourceMacId = m_FragXmitMsg.HeadInfo.u16SourceMacId;
				TempFrame.PackedMsg.b6DestinationMacId = m_FragXmitMsg.HeadInfo.u16DestinationMacId;
				TempFrame.PackedMsg.b1Fragment = FRAG_MSG;
				TempFrame.PackedMsg.b7ServiceCode = m_FragXmitMsg.HeadInfo.u16ServiceCode;
				TempFrame.PackedMsg.b1RsRq = RS_MSG;//RQ_MSG;

				//m_FragXmitMsg.HeadInfo.u16ByteLength-�ܵĴ����͵���Ϣ�ֽ���
				//m_FragXmitMsg.u16ByteSentCount--�ѷ��͵��ֽ���
				//u16SendCount--���δ����͵�����
				if ((m_FragXmitMsg.HeadInfo.u16ByteLength - m_FragXmitMsg.u16ByteSentCount) 
					<= (CAN_DATA_FIELD_SIZE - FRAG_HEAD_BYTE_SZIE))
				{
					u16SendCount = m_FragXmitMsg.HeadInfo.u16ByteLength - m_FragXmitMsg.u16ByteSentCount;
					m_FragXmitMsg.u16FragType = LAST_FRAG;
				}

				else
				{
					u16SendCount = CAN_DATA_FIELD_SIZE - FRAG_HEAD_BYTE_SZIE;
				}

				TempFrame.PackedMsg.u16Dlc = u16SendCount + FRAG_HEAD_BYTE_SZIE;

				for (i = 0; i < u16SendCount; i++)
				{
					//compute data section index of received frame 
					u16DesByteIdx = (i + 1) % 2;
					u16DesWordIdx = (i + 1) / 2;

					//compute send buffer index
					if ((m_FragXmitMsg.u16ByteSentCount % 2) == LOW_BYTE)
					{
						u16SourceByteIdx = i % 2;
						u16SourceWordIdx = m_FragXmitMsg.u16ByteSentCount / 2 + i / 2;
					}

					else
					{
						u16SourceByteIdx = (i + 1) % 2;
						u16SourceWordIdx = m_FragXmitMsg.u16ByteSentCount / 2 + (i + 1) / 2; 
					}

					//remove a byte from received frame data section 
					if (u16SourceByteIdx == HIGH_BYTE)	
					{
						//ucTempData = m_FragXmitMsg.Data[u16SourceWordIdx].ByteWide.High;
						ucTempData = m_FragXmitMsg.HeadInfo.pDataPtr[u16SourceWordIdx].ByteWide.High;
					}
					else
					{
						//ucTempData = m_FragXmitMsg.Data[u16SourceWordIdx].ByteWide.Low;
						ucTempData = m_FragXmitMsg.HeadInfo.pDataPtr[u16SourceWordIdx].ByteWide.Low;
					}

					//save a  byte to receive buffer according index
					if (u16DesByteIdx == HIGH_BYTE)
					{
						TempFrame.PackedMsg.MsgData[u16DesWordIdx].ByteWide.High = ucTempData;
					}

					else
					{
						TempFrame.PackedMsg.MsgData[u16DesWordIdx].ByteWide.Low = ucTempData;
					}
				}

				TempFrame.PackedMsg.MsgData[0].ByteWide.Low = ((m_FragXmitMsg.u16FragType << MSG_FRAG_TYPE_OFFSET)
															+ (m_FragXmitMsg.u16FrameCnt & FRAG_COUNT_MASK));

				//Post msg
				XmitFramePost(&TempFrame);

				m_FragXmitMsg.u16State = XMIT_STATE_WAIT_FOR_ACK;

				break;

			case XMIT_STATE_WAIT_FOR_ACK:
				if (m_FragXmitMsg.u16State != m_FragXmitMsg.u16OldState)
				{
					m_FragXmitMsg.u16OldState = m_FragXmitMsg.u16State;
					
					//Post Timer Delay 200ms Enable timer
					//pTimerSubject->TimerPost(m_FragXmitMsg.AckTimeHandle.pTimeOutObsvr);
					//msDelay = FRAG_ACK_MAX_TIME_VAL;
					//SysCanTimerObj.Timer_Srv_Post( m_FragXmitMsg.ucFragAckTimerID, msDelay);
					Timer_Post( &m_FragXmitMsg.AckTimeHandle);
				}

				if (NULL == pCanPacked)
				{

				}

				else if ((pCanPacked->PackedMsg.b7ServiceCode == m_FragXmitMsg.HeadInfo.u16ServiceCode)
					&& ((pCanPacked->PackedMsg.MsgData[0].ByteWide.Low & FRAG_COUNT_MASK) == m_FragXmitMsg.u16FrameCnt))
				{
					
					//Timer Flush Disable Timer
					//pTimerSubject->TimerFlush(m_FragXmitMsg.AckTimeHandle.pTimeOutObsvr);
					//SysCanTimerObj.Timer_Srv_Flush(m_FragXmitMsg.ucFragAckTimerID);
					Timer_Flush( &m_FragXmitMsg.AckTimeHandle);

					switch (pCanPacked->PackedMsg.MsgData[0].ByteWide.High)
					{
						case FRAG_ACK_STATE_PASS:
							if (LAST_FRAG == m_FragXmitMsg.u16FragType)
							{
								m_FragXmitMsg.u16State = XMIT_STATE_FINISHED;
							}
							else
							{
								//setup to send next frame
								m_FragXmitMsg.pData = m_FragXmitMsg.pSendData;
								m_FragXmitMsg.u16FrameCnt ++;
								//changed by liuj
								//m_FragXmitMsg.u16ByteSentCount = CAN_DATA_FIELD_SIZE - FRAG_HEAD_BYTE_SZIE;
								m_FragXmitMsg.u16ByteSentCount += CAN_DATA_FIELD_SIZE - FRAG_HEAD_BYTE_SZIE;
								m_FragXmitMsg.u16FragType = MIDDLE_FRAG;

								if (m_FragXmitMsg.u16FrameCnt >= FRAG_FRAME_MAX_CNT)
								{
									m_FragXmitMsg.u16FrameCnt = 0;
								}

								//clear error flag
								m_FragXmitMsg.AckTimeHandle.u16TimeOutCnt = 0;
								m_FragXmitMsg.BusyTimeHandle.u16TimeOutCnt = 0;

								m_FragXmitMsg.u16State = XMIT_STATE_XMIT;
							}

							break;

						case FRAG_ACK_STATE_FAIL:
							m_FragXmitMsg.u16State = XMIT_STATE_FINISHED;
							break;

						case FRAG_ACK_STATE_BUSY:
							m_FragXmitMsg.AckTimeHandle.u16TimeOutCnt = 0;
							m_FragXmitMsg.u16State = XMIT_STATE_RECEIVER_BUSY;
							break;

						default:
							break;
					}
				}
				
				break;
				
			case XMIT_STATE_RECEIVER_BUSY:
				if (m_FragXmitMsg.u16State != m_FragXmitMsg.u16OldState)
				{
					m_FragXmitMsg.u16OldState = m_FragXmitMsg.u16State;
					
					//Post Timer ,delay 1000ms
					//pTimerSubject->TimerPost(m_FragXmitMsg.BusyTimeHandle.pTimeOutObsvr);
					//msDelay = FRAG_XMIT_BUSY_MAX_TIME_VAL;
					//SysCanTimerObj.Timer_Srv_Post( m_FragXmitMsg.ucFragBusyTimerID, msDelay);
					Timer_Post( &m_FragXmitMsg.BusyTimeHandle);
				}

				break;

			case XMIT_STATE_FINISHED:
				if (m_FragXmitMsg.u16State != m_FragXmitMsg.u16OldState)
				{
					m_FragXmitMsg.u16OldState = m_FragXmitMsg.u16State;
				}

				m_FragXmitMsg.u16State = XMIT_STATE_IDLE;
				m_FragXmitMsg.u16Allocted = FALSE;

				// callback after XMIT successfully
				//m_FragXmitMsg.pSuccessCallBack->Excute(&m_FragXmitMsg.HeadInfo);

				break;

			case XMIT_STATE_FAIL:
				if (m_FragXmitMsg.u16State != m_FragXmitMsg.u16OldState)
				{
					m_FragXmitMsg.u16OldState = m_FragXmitMsg.u16State;
				}

				m_FragXmitMsg.u16State = XMIT_STATE_IDLE;
				m_FragXmitMsg.u16Allocted = FALSE;

				//err call back
				//m_FragXmitMsg.pErrCallBack->Excute(&m_FragXmitMsg.HeadInfo);

			default:
				m_FragXmitMsg.u16State = XMIT_STATE_IDLE;
				break;
		}
	}while(m_FragXmitMsg.u16State != m_FragXmitMsg.u16OldState);
}



/********************************************************************************
XmitFramePost-----�����µĴ�������Ϣ�����У����Ӷ�����ȡ��
				�ϵ���Ϣ������CAN���ͼĴ�������Ӧ�ò����
                     	
Parameters:              pCanPacked: msg will be insert XMIT queue
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 SysCanProtocol::XmitFramePost(IN CAN_PACKED_PROTOCOL_U *pCanPacked)
{
	CAN_PACKED_PROTOCOL_U TempPacked = *pCanPacked;

	//�޹�λֱ�ӹ̶�Ϊ0
	TempPacked.PackedMsg.b1NotUsed = 0;

	CloseSysCanInt();       //Close Interrupt,for Transmit Data
	if (QUEUE_FULL == m_SysCanXmitQueue.FlagQueue())
	{
		OpenSysCanInt();    //Open Interrupt
		return -1;	
	}

	else
	{
		m_SysCanXmitQueue.InsQueue(TempPacked);
	}

	if (FALSE == m_u16CanXmitBusy)
	{
		m_u16CanXmitBusy = TRUE;
		//OpenSysCanInt();

		//Transmit oldest frame
		TempPacked = m_SysCanXmitQueue.GetOldestNode();
		m_SysCanXmitQueue.DelQueue();
		SysCanSendData(&(TempPacked.Frame));
	}
	
	OpenSysCanInt();    //Open Interrupt

	return 0;
}

#if (0)
//��ʱ��ע��
void SysCanProtocol::Timer_New(TIMER_HANDLE_T  *pTimer,TIMER_OUT_FCB pExpierCB)
{
	pTimer->pTimeOutFcb = pExpierCB;
	pTimer->ucTimerEnable = TIMER_DISABLE;
}
#endif

//������ʱ��
void SysCanProtocol::Timer_Post(TIMER_HANDLE_T  *pTimer)
{
	pTimer->ucTimerEnable = TIMER_ENABLE;
	pTimer->u16TimeOutCnt = 0;
	pTimer->u16TimeDelayCnt = 0;
}

//��ֹ��ʱ��
void SysCanProtocol::Timer_Flush(TIMER_HANDLE_T  *pTimer)
{
	pTimer->ucTimerEnable = TIMER_DISABLE;
}

//��ʱ������ۺϴ���
void SysCanProtocol::Timer_Expire_Handle(VOID)
{
	
	FragAckTimeoutFcb();
	FragXmitBusyTimeoutFcb();
	CANFragRecvTimeoutFcb();
	
	
}

/**********************************************************************
FragAckTimeoutFcb-----�ֶ�֡Ӧ��ʱ������
                     
Parameters:  
					pObsvr:��ʱ������
			           
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
VOID SysCanProtocol::FragAckTimeoutFcb(VOID)
{
	if (m_FragXmitMsg.AckTimeHandle.ucTimerEnable == TIMER_ENABLE)
	{
		//��ʱ100ms��
		if ((++m_FragXmitMsg.AckTimeHandle.u16TimeDelayCnt) >= m_FragXmitMsg.AckTimeHandle.u16TimeExpireCnt)
		{
			m_FragXmitMsg.AckTimeHandle.u16TimeDelayCnt = 0;
			//����������ά��
			if ((++m_FragXmitMsg.AckTimeHandle.u16TimeOutCnt) >= m_FragXmitMsg.AckTimeHandle.u16TimeOutMaxCnt)
			{
				//m_FragXmitMsg.pErrCallBack->Excute(&(m_FragXmitMsg.HeadInfo));
				//TBD add clear timer
				//��������,��״̬Ϊ����ʧ��
				m_FragXmitMsg.AckTimeHandle.u16TimeOutCnt = 0;
				m_FragXmitMsg.u16State = XMIT_STATE_FAIL;
			}

			//��ʱ����δ��,�ط�
			else
			{
				m_FragXmitMsg.u16State = XMIT_STATE_XMIT;
			}

			FragMsgXmitStateMachine(NULL);
			
		}

		//---100ms��ʱδ��,�����ȴ�
		else
		{
			;//DO NOTHING
		}

	}

	//��ʱ��δ����,�����κδ���
	else
	{
		;//DO NOTHING
	}
	
	//TBD 
//	return 0;
}

/**********************************************************************
FragXmitBusyTimeoutFcb-----�ֶ�֡Ӧ��ʱ������
                     
Parameters:  
					pObsvr:��ʱ������
			           
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
VOID SysCanProtocol::FragXmitBusyTimeoutFcb(VOID)
{
	if (m_FragXmitMsg.BusyTimeHandle.ucTimerEnable == TIMER_ENABLE)
	{
		//��ʱ100ms��
		if ((++m_FragXmitMsg.BusyTimeHandle.u16TimeDelayCnt) >= m_FragXmitMsg.BusyTimeHandle.u16TimeExpireCnt)
		{
			m_FragXmitMsg.BusyTimeHandle.u16TimeDelayCnt = 0;
			//����������ά��
			if ((++m_FragXmitMsg.BusyTimeHandle.u16TimeOutCnt) >= m_FragXmitMsg.BusyTimeHandle.u16TimeOutMaxCnt)
			{
				//m_FragXmitMsg.pErrCallBack->Excute(&(m_FragXmitMsg.HeadInfo));
				m_FragXmitMsg.BusyTimeHandle.u16TimeOutCnt = 0;
				m_FragXmitMsg.u16State = XMIT_STATE_FINISHED;
			}

			//��ʱ����δ��,�ط�
			else
			{
				m_FragXmitMsg.u16State = XMIT_STATE_XMIT;
			}

			FragMsgXmitStateMachine(NULL);
			
		}

		//---100ms��ʱδ��,�����ȴ�
		else
		{
			;//DO NOTHING
		}

	}

	//��ʱ��δ����,�����κδ���
	else
	{
		;//DO NOTHING
	}

}

/**********************************************************************
CANFragRecvTimeoutFcb-----�ֶ�֡���ճ�ʱ������
                     
Parameters:  
					pObsvr:��ʱ������
			           
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
VOID SysCanProtocol::CANFragRecvTimeoutFcb(VOID)
{
	if (m_FragRecvMsg.TimeHandle.ucTimerEnable == TIMER_ENABLE)
	{
		m_FragRecvMsg.u16State = RECV_STATE_IDLE;
	}

	//TBD liuj 20080525 �Ƿ����ӳ�ʱ��æ��Ӧ���Ӧ,��������
	
	//TBD
//	return 0;
}


/**********************************************************************
SetFragRecvOkHandleFcb-----�ֶ�֡���ճɹ�������ע��
                     
Parameters:  
					pCallBack:����������
			           
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
INT16 SysCanProtocol::SetFragRecvOkHandleFcb(CommandBase *pCallBack)
{
	if (NULL == pCallBack)
	{
		return CAN_PROTOCOL_INVALID_FRAG_RECV_FCB;
	}

	m_FragRecvMsg.pSuccessCallBack = pCallBack;

	return CAN_PROTOCOL_OK;

}


/**********************************************************************
SetFragXmitErrHandleFcb-----�ֶ�֡���͹��ϴ�����ע��
                     
Parameters:  
					pCallBack:����������
			           
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
INT16 SysCanProtocol::SetFragXmitErrHandleFcb(CommandBase *pCallBack)
{
	if (NULL == pCallBack)
	{
		return CAN_PROTOCOL_INVALID_FRAG_XMIT_FCB;
	}

	m_FragXmitMsg.pErrCallBack = pCallBack;
	
	return CAN_PROTOCOL_OK;
}	

/**********************************************************************
SetFragXmitErrHandleFcb-----�ֶ�֡���ͳɹ�������ע��
                     
Parameters:  
					pCallBack:����������
			           
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
INT16 SysCanProtocol::SetFragXmitOkHandleFcb(CommandBase *pCallBack)
{
	if (NULL == pCallBack)
	{
		return CAN_PROTOCOL_INVALID_FRAG_XMIT_FCB;
	}

	m_FragXmitMsg.pSuccessCallBack = pCallBack;

	return CAN_PROTOCOL_OK;
}

