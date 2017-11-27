/***************************************************************************************
* 
* Filename: 		SysCanQueue.h
* Contributor:
* Author: 			
* Date: 			
* Copyright 		2008 ENPC Corporation, All Rights Reserved
*
* Description: 	
* 
* simliar with FIFO
* Version
* Last rev by:		
* Last rev date:	
* Last rev desc:	
****************************************************************************************/

#ifndef 	SysCanQueue_h
#define	SysCanQueue_h

//-----------------------------------------------------------------------------
//#include 
//#include "Define.h"

//--------------------------------------------------------------------------------
#define XMIT_QUEUE_SIZE 64		//发送队列的结构体数组数量
#define RECV_QUEUE_SZIE 16		//接收队列的结构体数组数量，用于将CAN 邮箱中的内容存入队列的尾部

#define FRAG_MSG_QUEUE_SIZE 5

//----------------------------------------
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
//驱动层与协议层的格式互换
	typedef union CAN_PACKED_PROTOCOL_STRUCT
	{
	    SYS_CAN_FRAME Frame;
	    struct
	    {
	        UINT16 u16ErrorStatus;
	        UINT16 b6SourceMacId: 6;
	        UINT16 b10MsgClass: 10;
	        UINT16 u16Dlc;				//Data field all len,bytes
		 	UINT16 b6DestinationMacId: 6;
	        UINT16 b1NotUsed: 1;
	        UINT16 b1Fragment: 1;
	        UINT16 b7ServiceCode: 7;
	        UINT16 b1RsRq: 1;

	        WORD_TO_2BYTES_T MsgData[3];
	    }PackedMsg;
	}CAN_PACKED_PROTOCOL_U;

	//应用层帧格式
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


// Constants and Macro Definitions
enum
{
	QUEUE_FULL = -1,	//队列满
	QUEUE_NORMAL = 1,  //队列正常
	QUEUE_NULL = 0		//队列空
};

//-----------------------------------------------------------------------------
// Public Enumerated and Structure Definitions


//-----------------------------------------------------------------------------
// Public Function Prototypes

//-----------------------------------------------------------------------------
//Class prototype
//-----------------------------------------
//接收队列定义
class SysCanRecvQueue
{
public:
	void SysCanRecvQueueIni(void);
	INT16 InsQueue(CAN_PACKED_PROTOCOL_U MsgType);
	CAN_PACKED_PROTOCOL_U * DelQueue(void);
	INT16 FlagQueue(void);
	CAN_PACKED_PROTOCOL_U GetOldestNode(void);

private:
	UINT16	m_size;		//队列容量
	UINT16	m_front;	//头指针
	UINT16	m_rear;		//尾指针
	UINT16	m_counter;	//队内消息数
	CAN_PACKED_PROTOCOL_U 	m_queue[RECV_QUEUE_SZIE];	//存储空间首地址

	
};

//队列对象初始化
void SysCanRecvQueue::SysCanRecvQueueIni(void)
{
	m_size = RECV_QUEUE_SZIE;
	m_front = 0;
	m_rear = 0;
	m_counter = 0;
#if (0)
	//under code line was added only for test
	CAN_PACKED_PROTOCOL_U uTemp;
	uTemp.PackedMsg.b6SourceMacId = 0x01;//Monitor
	uTemp.PackedMsg.b10MsgClass = 0x01;	//CFG msg
	uTemp.PackedMsg.u16Dlc = 6; 			//data length
	uTemp.PackedMsg.b6DestinationMacId = 0x0f;	//destination ID Broadcast
	uTemp.PackedMsg.b1Fragment=0;			//Non frag
	uTemp.PackedMsg.b7ServiceCode = 0x10;		//time cfg
	uTemp.PackedMsg.b1RsRq = 1;
	uTemp.PackedMsg.MsgData[0].u16Word= 30;
	uTemp.PackedMsg.MsgData[1].u16Word = 1;
	

	InsQueue(uTemp);
	
#endif	
	
}

/*************************************************************************
InsTxQueue() ---新的subID入队
                     
Parameters:    MessageType
			
Return Value: -1:队列满,  1: 正常
Precondition: 
Postcondition: 
*************************************************************************/
INT16 SysCanRecvQueue::InsQueue(CAN_PACKED_PROTOCOL_U Message)
{
	//是否队列满?
	if (m_size == m_counter)
	{
#ifdef  DEBUG_CODE	
		printf(" Queue is overflow!\n");
#endif
		return (QUEUE_FULL);
	}
	else
	{
		m_queue[m_rear] = Message;	//写入信息
		
		m_rear = (m_rear+1)%m_size; //防止队列尾指针过界,并形成循环队列
		
		m_counter++;

		return (QUEUE_NORMAL);
	}
	
}


/*************************************************************************
DelQueue() ---最老的subID退队
                     					
Parameters:   
			
Return Value: the oldest task type and subTaskID in queue
Precondition: 
Postcondition: 
*************************************************************************/
CAN_PACKED_PROTOCOL_U *SysCanRecvQueue::DelQueue(void)
{
	CAN_PACKED_PROTOCOL_U *pMessageType;

	if (m_counter == 0) //is queue empty?
	{
	
#ifdef  DEBUG_CODE	
		printf("Queue is underflow!\n" );
#endif
		return NULL;
	}
	else 
	{
		pMessageType = &m_queue[m_front];	//取出最老的信息
		
		m_front = (m_front + 1)%m_size;//防止队列头指针过界，并形成循环队列
		m_counter--;
				
		return (pMessageType);
	}
}

/*************************************************************************
static char FlagTxQueue(char tskType)) ---检测循环队列的状态
                     					 
Parameters:    
			
Return Value: 
			-1, queue is full;
                       0,  queue is empty;
                       n, normal, 表示还有几个空闲位; 
Precondition: 
Postcondition: 
*************************************************************************/
INT16 SysCanRecvQueue::FlagQueue(void)
{
	if (m_counter == m_size) // Is queue full?
	{
		return (QUEUE_FULL);	
	}
	
	if ( m_counter == 0)
	{
		return (QUEUE_NULL);
	}

	return (m_size -m_counter);		//返回队列的空闲位数
}

/*************************************************************************
GetOldestNode ---读取最老的节点内容
                     					 
Parameters:    
			
Return Value: ItcMessageType
		
Precondition: 
Postcondition: 
*************************************************************************/
CAN_PACKED_PROTOCOL_U SysCanRecvQueue::GetOldestNode(void)
{
	//ItcMessageType MessageType;

#ifdef DEBUG_CODE
	if (QUEUE_NULL == m_size)
	{
		printf("The Queue is null, can't get zhe node");
		
	}
#endif

	return m_queue[m_front];
}

//-------------发送队列定义---------------------
//发送队列定义
class SysCanXmitQueue
{
public:
	void SysCanXmitQueueIni(void);
	INT16 InsQueue(CAN_PACKED_PROTOCOL_U MsgType);
	CAN_PACKED_PROTOCOL_U * DelQueue(void);
	INT16 FlagQueue(void);
	CAN_PACKED_PROTOCOL_U GetOldestNode(void);

private:
	UINT16	m_size;		//队列容量
	UINT16	m_front;	//头指针
	UINT16	m_rear;		//尾指针
	UINT16	m_counter;	//队内消息数
	CAN_PACKED_PROTOCOL_U 	m_queue[XMIT_QUEUE_SIZE];	//存储空间首地址

	
};

//队列对象初始化
void SysCanXmitQueue::SysCanXmitQueueIni(void)
{
	m_size = XMIT_QUEUE_SIZE;
	m_front = 0;
	m_rear = 0;
	m_counter = 0;
	
}

/*************************************************************************
InsTxQueue() ---新的subID入队
                     
Parameters:    MessageType
			
Return Value: -1:队列满,  1: 正常
Precondition: 
Postcondition: 
*************************************************************************/
INT16 SysCanXmitQueue::InsQueue(CAN_PACKED_PROTOCOL_U Message)
{
	//是否队列满?
	if (m_size == m_counter)
	{
#ifdef  DEBUG_CODE	
		printf(" Queue is overflow!\n");
#endif
		return (QUEUE_FULL);
	}
	else
	{
		m_queue[m_rear] = Message;	//写入信息
		
		m_rear = (m_rear+1)%m_size; //防止队列尾指针过界，并且当m_rear = RECV_QUEUE_SZIE,m_rear = 0;形成循环队列
		
		m_counter++;

		return (QUEUE_NORMAL);
	}
	
}


/*************************************************************************
DelQueue() ---最老的subID退队
                     					
Parameters:   
			
Return Value: the oldest task type and subTaskID in queue
Precondition: 
Postcondition: 
*************************************************************************/
CAN_PACKED_PROTOCOL_U *SysCanXmitQueue::DelQueue(void)
{
	CAN_PACKED_PROTOCOL_U *pMessageType;

	if (m_counter == 0) //is queue empty?
	{
	
#ifdef  DEBUG_CODE	
		printf("Queue is underflow!\n" );
#endif
		return NULL;
	}
	else 
	{
		pMessageType = &m_queue[m_front];	//取出最老的信息
		
		m_front = (m_front + 1)%m_size;//防止队列头指针过界 ，并且当m_front = RECV_QUEUE_SZIE,m_front = 0,形成循环队列
		m_counter--;
				
		return (pMessageType);
	}
}

/*************************************************************************
static char FlagTxQueue(char tskType)) ---检测循环队列的状态
                     					 
Parameters:    
			
Return Value: 
			-1, queue is full;
                       0,  queue is empty;
                       n, normal, 表示还有几个空闲位; 
Precondition: 
Postcondition: 
*************************************************************************/
INT16 SysCanXmitQueue::FlagQueue(void)
{
	if (m_counter == m_size) // Is queue full?
	{
		return (QUEUE_FULL);	
	}
	
	if ( m_counter == 0)	//Is quene null?
	{
		return (QUEUE_NULL);
	}

	return (m_size -m_counter);	//返回空闲位数量
}

/*************************************************************************
GetOldestNode ---读取最老的节点内容
                     					 
Parameters:    
			
Return Value: ItcMessageType
		
Precondition: 
Postcondition: 
*************************************************************************/
CAN_PACKED_PROTOCOL_U SysCanXmitQueue::GetOldestNode(void)
{
	//ItcMessageType MessageType;

#ifdef DEBUG_CODE
	if (QUEUE_NULL == m_size)
	{
		printf("The Queue is null, can't get zhe node");
		
	}
#endif

	return m_queue[m_front];	//返回头指针的内容
}



//分段帧队列定义
class SysCanFragXmitQueue
{
public:
	void SysCanFragXmitQueueIni(void);
	INT16 InsQueue(CAN_APDU_T MsgType);
	CAN_APDU_T * DelQueue(void);		//函数返回值为CAN_APDU_T结构体类型
	INT16 FlagQueue(void);
	CAN_APDU_T GetOldestNode(void);//函数返回值为CAN_APDU_T结构体类型

private:
	UINT16	m_size;		//队列容量
	UINT16	m_front;	//头指针
	UINT16	m_rear;		//尾指针
	UINT16	m_counter;	//队内消息数
	CAN_APDU_T 	m_queue[FRAG_MSG_QUEUE_SIZE];	//存储空间首地址

	
};

//队列对象初始化
void SysCanFragXmitQueue::SysCanFragXmitQueueIni(void)
{
	m_size = FRAG_MSG_QUEUE_SIZE;
	m_front = 0;
	m_rear = 0;
	m_counter = 0;
	
}

/*************************************************************************
InsTxQueue() ---新的subID入队
                     
Parameters:    MessageType
			
Return Value: -1:队列满,  1: 正常
Precondition: 
Postcondition: 
*************************************************************************/
INT16 SysCanFragXmitQueue::InsQueue(CAN_APDU_T Message)
{
	//是否队列满?
	if (m_size == m_counter)
	{
#ifdef  DEBUG_CODE	
		printf(" Queue is overflow!\n");
#endif
		return (QUEUE_FULL);
	}
	else
	{
		m_queue[m_rear] = Message;	//写入信息
		
		m_rear = (m_rear+1)%m_size; //防止队列尾指针过界，并形成循环队列
		
		m_counter++;

		return (QUEUE_NORMAL);
	}
	
}


/*************************************************************************
DelQueue() ---最老的subID退队
                     					
Parameters:   
			
Return Value: the oldest task type and subTaskID in queue
Precondition: 
Postcondition: 
*************************************************************************/
CAN_APDU_T *SysCanFragXmitQueue::DelQueue(void)
{
	CAN_APDU_T *pMessageType;

	if (m_counter == 0) //is queue empty?
	{
	
#ifdef  DEBUG_CODE	
		printf("Queue is underflow!\n" );
#endif
		return NULL;
	}
	else 
	{
		pMessageType = &m_queue[m_front];	//取出最老的信息
		
		m_front = (m_front + 1)%m_size;//防止队列头指针过界 形成循环队列
		m_counter--;
				
		return (pMessageType);
	}
}

/*************************************************************************
static char FlagTxQueue(char tskType)) ---检测循环队列的状态
                     					 
Parameters:    
			
Return Value: 
			-1, queue is full;
                       0,  queue is empty;
                       n, normal, 表示还有几个空闲位; 
Precondition: 
Postcondition: 
*************************************************************************/
INT16 SysCanFragXmitQueue::FlagQueue(void)
{
	if (m_counter == m_size) // Is queue full?
	{
		return (QUEUE_FULL);	
	}
	
	if ( m_counter == 0)
	{
		return (QUEUE_NULL);
	}

	return (m_size -m_counter);
}

/*************************************************************************
GetOldestNode ---读取最老的节点内容
                     					 
Parameters:    
			
Return Value: ItcMessageType
		
Precondition: 
Postcondition: 
*************************************************************************/
CAN_APDU_T SysCanFragXmitQueue::GetOldestNode(void)
{
	//ItcMessageType MessageType;

#ifdef DEBUG_CODE
	if (QUEUE_NULL == m_size)
	{
		printf("The Queue is null, can't get zhe node");
		
	}
#endif

	return m_queue[m_front];
}

//-------------------------------------------


#endif





