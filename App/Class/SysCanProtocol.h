/***************************************************************************************
* 
* Filename: 		SysCanHal.h
* Contributor:
* Author: 		Wang Zhihua	
* Date: 			1/21/2008
* Copyright 		2008 ENPC Corporation, All Rights Reserved
*
* Description: 	
* 
* 
* Version
* Last rev by:		Wang Zhihua
* Last rev date:	1/21/2008
* Last rev desc:	
****************************************************************************************/

#ifndef SYSCANPROTOCOL_H
#define SYSCANPROTOCOL_H

//---------------------------------------------------------------
//Include header
//#include "Define.h"
#include "SysCanHal.h"
#include "TemplateCmd.h"
#include "SysCanQueue.h"
//#include "TemplateQueue.h"
//#include "TimerObserver.h"
//#include "SysCanTimer.h"

//-----------------------------------------------------------------------------
//Macro definition and enum

	//Fragment msg
	#define FRAG_MSG 1			//�ֶ�֡
	#define NONFRAG_MSG 0		//�Ƿֶ�֡

	/*��λָʾ��ǰ����֡��һ֡��Ӧ֡��������֡����Ӧ֡�����Ƕ�һ������֡����Ӧ��
	Ҳ������һ����������Ӧ�����ߵ���˼����Ӧ֡���˱�����֡�������⣬���п��ܱ�һ���ⲿ����������
	ͬʱ��Ӧ֡��������ӦһЩ�ض���֡(��Sync/WD)������֡����һ��ϣ���õ��Է���Ӧ����Ϣ��*/
	//request or response msg
	#define RS_MSG 1		//��Ӧ֡
	#define RQ_MSG 0		//����֡

	//msg head format
	#define MSG_CLASS_OFFSET 6
	#define MSG_RSRQ_BIT_OFFSET 7
	#define MSG_FRAG_BIT_OFFSET 7
	#define MSG_FRAG_TYPE_OFFSET 6

	#define FRAG_COUNT_MASK 0x3f
	#define MSG_CLASS_MASK 0x3c0
	#define SOURCE_ID_MASK 0x3f
	#define DESTINATION_ID_MASK 0x3f
	#define SRVC_CODE_MASK 0x7f

	//frame field length
	#define FRAG_HEAD_BYTE_SZIE 3
	#define NONFRAG_HEAD_BYTE_SIZE 2
	#define CAN_DATA_FIELD_SIZE 8

	//myself MAC ID
	#define LOCAL_NODE_ID MAC_ID_MOD1_INV

	//message buffer length
	#define CAN_FRAG_DATA_LEN 500
	#define CAN_NONFRAG_DATA_LEN 6

	//Э���״̬
	enum CAN_PROTOCOL_ST
	{
		CAN_PROTOCOL_OK = 0,
		CAN_PROTOCOL_NO_MSG,
		CAN_PROTOCOL_NO_TIMER,
		CAN_PROTOCOL_PORT_ERR,
		CAN_PROTOCOL_INVALID_RQ_FCB,
		CAN_PROTOCOL_INVALID_FRAG_XMIT_FCB,
		CAN_PROTOCOL_INVALID_FRAG_RECV_FCB,

		CAN_PROTOCOL_EOL
	};

#if (0)	
	//system CAN PDU frame
	typedef union
	{
	    UINT16 u16Word;
	    struct
	    {
	        UINT16 Low: 8;
	        UINT16 High: 8;
	    }ByteWide;
	}WORD_TO_2BYTES_T;

	//��������Э���ĸ�ʽ����
	typedef union CAN_PACKED_PROTOCOL_STRUCT
	{
	    SYS_CAN_FRAME Frame;
	    struct
	    {
	        UINT16 u16ErrorStatus;
	        UINT16 b6SourceMacId: 6;
	        UINT16 b10MsgClass: 10;
	        UINT16 u16Dlc;
	        UINT16 b6DestinationMacId: 6;
	        UINT16 b1NotUsed: 1;
	        UINT16 b1Fragment: 1;
	        UINT16 b7ServiceCode: 7;
	        UINT16 b1RsRq: 1;

	        WORD_TO_2BYTES_T MsgData[3];
	    }PackedMsg;
	}CAN_PACKED_PROTOCOL_U;

	//Ӧ�ò�֡��ʽ
	typedef struct can_msg_data_struct
	{
		UINT16 u16MsgClass;
		UINT16 u16ServiceCode;
		UINT16 u16RsRq;
		UINT16 u16SourceMacId;
		UINT16 u16Fragment;
		UINT16 u16DestinationMacId;
		UINT16 u16ByteLength;
		WORD_TO_2BYTES_T *pDataPtr;

	}CAN_APDU_T;
#endif

	//��֡��Ϣ
	typedef struct 
	{
		CAN_APDU_T HeadInfo;
		WORD_TO_2BYTES_T Data[CAN_NONFRAG_DATA_LEN / 2];
	}NONFRAG_MSG_T;

	//System CAN MAC ID alloction
	enum _CAN_MAC_ID_ENUM
	{
		MAC_ID_MON = 1,
		MAC_ID_BYP = 0x0F,
		MAC_ID_MOD1_INV = 0x10,
		MAC_ID_MOD2_INV = 0x11,
		MAC_ID_MOD3_INV = 0x12,
		MAC_ID_MOD4_INV = 0x13,
		MAC_ID_MOD5_INV = 0x14,
		MAC_ID_MOD6_INV = 0x15,
		MAC_ID_MOD7_INV = 0x16,
		MAC_ID_MOD8_INV = 0x17,
		MAC_ID_MOD9_INV = 0x18,
		MAC_ID_MOD10_INV = 0x19,
		MAC_ID_MOD1_REC = 0x20,
		MAC_ID_MOD2_REC = 0x21,
		MAC_ID_MOD3_REC = 0x22,
		MAC_ID_MOD4_REC = 0x23,
		MAC_ID_MOD5_REC = 0x24,
		MAC_ID_MOD6_REC = 0x25,
		MAC_ID_MOD7_REC = 0x26,
		MAC_ID_MOD8_REC = 0x27,
		MAC_ID_MOD9_REC = 0x28,
		MAC_ID_MOD10_REC = 0x29,
		MAC_ID_BROADCAST = 0x3f
	};

	//protocol layer error code enumeration
	enum
	{
		
		PROTOCOL_LAYER_XMIT_QUEUE_FULL
	};

	
	//��ʱ���ṹ��
	#define TIMER_MAX_COUNT	3
	enum
	{
		TIMER_DISABLE,
		TIMER_ENABLE
	};
	
	class SysCanProtocol;
	typedef INT16 (SysCanProtocol::*TIMER_OUT_FCB)(VOID);
	typedef struct
	{
		UCHAR ucTimerEnable;			/*��ʱ��ʹ�����*/
		UINT16 u16TimeOutMaxCnt;		/*�������*/
		UINT16 u16TimeOutCnt;			/*��ʱ��������*/
		UINT16 u16TimeExpireCnt;		/*��ʱ���������Ŀ��ֵ*/
		UINT16 u16TimeDelayCnt;			/*��ʱ����ʱ����*/
//		TimerObserver *pTimeOutObsvr;
//		TIMER_OUT_FCB pTimeOutFcb;
	}TIMER_HANDLE_T;

	//�ֶ�֡�������ݽṹ
	typedef struct
	{
		CAN_APDU_T HeadInfo;
		UINT16 *pData;
		UINT16 *pSendData;
		//WORD_TO_2BYTES_T Data[CAN_FRAG_DATA_LEN / 2];

		UINT16 u16Allocted; 
		UINT16 u16State;
		UINT16 u16OldState;
		UINT16 u16FragType;
		UINT16 u16FrameCnt;
		UINT16 u16ByteSentCount;

		TIMER_HANDLE_T AckTimeHandle;
		TIMER_HANDLE_T BusyTimeHandle;
//		UCHAR	ucFragAckTimerID;
//		UCHAR	ucFragBusyTimerID;
				
		CommandBase *pErrCallBack;
		CommandBase *pSuccessCallBack;
		
	}_CAN_FRAG_XMIT_MSG_T;

	//�ֶ�֡�������ݽṹ��
	typedef struct
	{
		CAN_APDU_T HeadInfo;
		UINT16 *pData;
		
		WORD_TO_2BYTES_T Data[CAN_FRAG_DATA_LEN / 2];

		UINT16 u16Allocated;
		UINT16 u16State;
		UINT16 u16OldState;
		UINT16 u16FragType;
		UINT16 u16FrameCnt;

		TIMER_HANDLE_T TimeHandle;
		//UCHAR ucFragRecvTimerID;
		CommandBase *pSuccessCallBack;
	
	}_CAN_FRAG_RECV_MSG_T;

	//�ֶ�֡����״̬��״̬ö��
	enum FRAG_XMIT_STATE
	{
		XMIT_STATE_IDLE = 0,
        XMIT_STATE_XMIT,
        XMIT_STATE_WAIT_FOR_ACK,
        XMIT_STATE_RECEIVER_BUSY,
		XMIT_STATE_FINISHED,
		XMIT_STATE_FAIL
	};

	//�ֶ�֡����״̬��״̬ö��
	enum FRAG_RECV_STATE
	{
		RECV_STATE_IDLE = 0,
		RECV_STATE_RECV_FRAG,
		RECV_STATE_WAIT_FOR_FRAG,
		RECV_STATE_HANDLE_DATA 
	};
//------------------------------------------------------------------------------
//Public variable definition
	
//-------------------------------------------------------------------------------
//func prototype definiton

//-------------------------------------------------------------------------
//Class prototype

class SysCanProtocol
{
public:
//	SysCanProtocol(VOID);
//	~SysCanProtocol(VOID);

	//��ʼ���������
	VOID SysCanProtocolIni(VOID);

	//------------------------------------------------------------
	//��Ϣ���ա����ͻص�����
	INT16 SysCanRecvFcb(VOID *pFrame);
	INT16 SysCanXmitFcb(VOID *pUnused);

	//------------------------------------------------------------
	//������Ϣ������
	INT16 XmitFramePost(IN CAN_PACKED_PROTOCOL_U *pCanPacked);
	
	//��Ϣ���մ���
	INT16 MsgRecvProc(CAN_APDU_T *pApdu);
	//��Ϣ���ʹ���
	INT16 MsgXmitProc(CAN_APDU_T *pApdu);

	INT16 MsgXmitProc_Test(void);

	//����ע��
	INT16 SetFragRecvOkHandleFcb(CommandBase *pCallBack);
	INT16 SetFragXmitErrHandleFcb(CommandBase *pCallBack);
	INT16 SetFragXmitOkHandleFcb(CommandBase *pCallBack);

	//��ʱ������ۺϴ�����
	void Timer_Expire_Handle(void);

	

private:
	VOID FrameParseToApdu(INOUT CAN_APDU_T *pApdu,
								IN CAN_PACKED_PROTOCOL_U *pFrame);
	VOID FrameParseToPpdu(OUT CAN_PACKED_PROTOCOL_U *pFrame,
													IN CAN_APDU_T *pApdu);

	//�ֶ�֡������
	VOID FragMsgXmitStateMachineEnty(VOID);
	VOID FragMsgXmitStateMachine(IN CAN_PACKED_PROTOCOL_U *pCanPacked);
	VOID FragMsgRecvStateMachine(IN CAN_PACKED_PROTOCOL_U *pCanPacked);

	VOID FragAckTimeoutFcb(VOID);
	VOID FragXmitBusyTimeoutFcb(VOID);
	VOID CANFragRecvTimeoutFcb(VOID);

	//-----------------------------------------------------------
	//�ֶ�֡��ʱ��������
	//��ʱ��ע��
//	void Timer_New(TIMER_HANDLE_T  *pTimer,TIMER_OUT_FCB pExpierCB);
	//������ʱ��
	void Timer_Post(TIMER_HANDLE_T  *pTimer);

	//��ֹ��ʱ��
	void Timer_Flush(TIMER_HANDLE_T  *pTimer);

	//��ʱ������ۺϴ�����
//	void Timer_Expire_Handle(void);

	//��ʱ��ID���ݳ�
//	UCHAR m_tTimerIDPool[TIMER_MAX_COUNT];

	//------------------------------------------------------------
	//�ֶ�֡����
	 _CAN_FRAG_XMIT_MSG_T m_FragXmitMsg;
	 _CAN_FRAG_RECV_MSG_T m_FragRecvMsg;

	//CAN ��ʱ��ע���ID��
//	unsigned char m_FragAckTimerID;
//	unsigned char m_FragBusyTimerID;
//	unsigned char m_FragRecvTimerID;
	
	//-------------------------------------------------------------
	//���͡����ն��ж���
//	TemplateQueue<CAN_PACKED_PROTOCOL_U> *m_pSysCanXmitQueue;
	SysCanRecvQueue m_SysCanRecvQueue;
	SysCanXmitQueue m_SysCanXmitQueue;
	UINT16 m_u16CanXmitBusy;
	
//	TemplateQueue<CAN_PACKED_PROTOCOL_U> *m_pSysCanRecvQueue;

	//------------------------------------------------------------
	//Э������������ݽṹ
	CAN_PACKED_PROTOCOL_U m_CurRecvedFrame;

	//-----------------------------------------------------------
	//��Ӧ�ò㽻������
	NONFRAG_MSG_T m_NonFragXmitMsg;
	NONFRAG_MSG_T m_NonFragRecvMsg;

	//------------------------------------------------------------
	//�жϽ��ա���������
	ProtocolSubCmd m_CanIsrRecvCmd;
	ProtocolSubCmd m_CanIsrXmitCmd;
	CommandBase *m_pCanIsrXmitCmd;
	CommandBase *m_pCanIsrRecvCmd;
	
};


#endif


