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
	QUEUE_FULL = -1,	//������
	QUEUE_NORMAL = 1,  //��������
	QUEUE_NULL = 0		//���п�
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
	UINT16	m_size;		//��������
	UINT16	m_front;	//ͷָ��
	UINT16	m_rear;		//βָ��
	UINT16	m_counter;	//������Ϣ��
	MessageType 	*m_queue;	//�洢�ռ��׵�ַ
};

/**********************************************************************
ItcOfModuleComm::ItcOfModuleComm-----�๹�캯������ 
                     
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
	m_queue = new MessageType[m_size];	//��������ڴ�
}

/**********************************************************************
ItcOfModuleComm::~ItcOfModuleComm-----�������������� 
                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
**********************************************************************/
template <class MessageType>
TemplateQueue<MessageType>::~TemplateQueue(void)
{
	delete []m_queue;//�ͷ��ڴ�
}

/*************************************************************************
InsTxQueue() ---�µ�subID���
                     
Parameters:    MessageType
			
Return Value: -1:������,  1: ����
Precondition: 
Postcondition: 
*************************************************************************/
template <class MessageType>
INT16 TemplateQueue<MessageType>::InsQueue(MessageType Message)
{
	//�Ƿ������?
	if (m_size == m_counter)
	{
#ifdef  DEBUG_CODE	
		printf(" Queue is overflow!\n");
#endif
		return (QUEUE_FULL);
	}
	else
	{
		m_queue[m_rear] = Message;	//д����Ϣ
		
		m_rear = (m_rear+1)%m_size; //��ֹ����βָ�����
		
		m_counter++;

		return (QUEUE_NORMAL);
	}
	
}


/*************************************************************************
DelQueue() ---���ϵ�subID�˶�
                     					
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
		pMessageType = &m_queue[m_front];	//ȡ�����ϵ���Ϣ
		
		m_front = (m_front + 1)%m_size;//��ֹ����ͷָ�����
		m_counter--;
				
		return (pMessageType);
	}
}

/*************************************************************************
static char FlagTxQueue(char tskType)) ---���ѭ�����е�״̬
                     					 
Parameters:    
			
Return Value: 
			-1, queue is full;
                       0,  queue is empty;
                       n, normal, ��ʾ���м�������λ; 
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
GetOldestNode ---��ȡ���ϵĽڵ�����
                     					 
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

