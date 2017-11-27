/***************************************************************************************
* 
* Filename: 		TemplateQueuel.h
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

#ifndef 	TEMPLATE_QUEUE_H
#define	TEMPLATE_QUEUE_H

//-----------------------------------------------------------------------------
//#include 
//#include "Define.h"

//--------------------------------------------------------------------------------
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
template <class MessageType>
class TemplateQueue
{
public:
	TemplateQueue(UINT16 size);
	~TemplateQueue(void);
//	void TemplateQueueIni(UINT16 size);

	
	INT16 InsQueue(MessageType MsgType);
	MessageType * DelQueue(void);
	INT16 FlagQueue(void);
	MessageType GetOldestNode(void);

private:
	UINT16	m_size;		//队列容量
	UINT16	m_front;	//头指针
	UINT16	m_rear;		//尾指针
	UINT16	m_counter;	//队内消息数
	MessageType 	*m_queue;	//存储空间首地址
};

/**********************************************************************
ItcOfModuleComm::ItcOfModuleComm-----类构造函数定义 
                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
template <class MessageType>
TemplateQueue<MessageType>::TemplateQueue(UINT16 size)
{
	m_size = size;
	m_front = 0;
	m_rear = 0;
	m_counter = 0;
	m_queue = new MessageType[m_size];	//申请队列内存
}

/**********************************************************************
ItcOfModuleComm::~ItcOfModuleComm-----类析构函数定义 
                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
template <class MessageType>
TemplateQueue<MessageType>::~TemplateQueue(void)
{
	delete []m_queue;//释放内存
}

/*************************************************************************
InsTxQueue() ---新的subID入队
                     
Parameters:    MessageType
			
Return Value: -1:队列满,  1: 正常
Precondition: 
Postcondition: 
*************************************************************************/
template <class MessageType>
INT16 TemplateQueue<MessageType>::InsQueue(MessageType Message)
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
		
		m_rear = (m_rear+1)%m_size; //防止队列尾指针过界
		
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
template <class MessageType>
MessageType *TemplateQueue<MessageType>::DelQueue(void)
{
	MessageType *pMessageType;

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
		
		m_front = (m_front + 1)%m_size;//防止队列头指针过界
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
template <class MessageType>
INT16 TemplateQueue<MessageType>::FlagQueue(void)
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
template <class MessageType>
MessageType TemplateQueue<MessageType>::GetOldestNode(void)
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

#endif

