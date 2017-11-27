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

#ifndef TEMPLATE_LIST_H
#define TEMPLATE_LIST_H

//---------------------------------------------------------------
//Include header
//#include "Define.h"
//-----------------------------------------------------------------------------
//Macro definition and enum
#define FIND_OBJ_OK 1
#define FIND_OBJ_ERR 0
//------------------------------------------------------------------------------
//Public variable definition

//-------------------------------------------------------------------------------
//func prototype definiton

//-------------------------------------------------------------------------
//Class prototype




template <class ClassType>
class TemplateList
{
public:
	typedef struct NODE_T NODE_T_;

	typedef struct NODE_T
	{
		NODE_T_ *pPrev;
		ClassType *pObj;
		NODE_T_ *pNext;
	}_NODE_T;

	TemplateList(VOID);
	~TemplateList(VOID);



	VOID PushBack(ClassType *pObj);
	VOID Remove(ClassType *pObj);
	
	ClassType *Begin(VOID);
	ClassType *End(VOID);

	ClassType *Next(ClassType *pObj);

	VOID Clear(VOID);

	INT16 Find(ClassType *pObj);
private:

	_NODE_T *m_pHead;
	_NODE_T *m_pTail;
	_NODE_T *m_pCurr;
	_NODE_T m_tNodeHead;
	_NODE_T m_tNodeTail;
	
	
};


template<class ClassType>
TemplateList<ClassType>::TemplateList(VOID)
{
	
//	m_pHead = new _NODE_T;
//	m_pTail = new _NODE_T;
	m_pHead = &m_tNodeHead;
	m_pTail = &m_tNodeTail;

	m_pHead->pPrev = NULL;
	m_pHead->pObj = NULL;
	m_pHead->pNext = (NODE_T_ *)m_pTail;

	m_pCurr = m_pHead;

	m_pTail->pPrev = (NODE_T_ *)m_pHead;
	m_pTail->pObj = NULL;
	m_pTail->pNext = NULL;

}

template<class ClassType>
TemplateList<ClassType>::~TemplateList(VOID)
{
#if (0)
	_NODE_T *pTempNext;
 	_NODE_T *pTempCurr;
	 
	for (pTempNext = (_NODE_T *)(m_pHead->pNext), pTempCurr = m_pHead; 
		((pTempCurr != NULL) && (pTempNext != NULL)); pTempNext = (_NODE_T *)(pTempNext->pNext))
	{
		delete pTempCurr; 
		pTempCurr = pTempNext;
	}
#endif
}

template<class ClassType>
VOID TemplateList<ClassType>::PushBack(ClassType *pObj)
{
	if (m_pHead->pObj == NULL)
	{
		m_pHead->pObj = pObj;
	}
	else 
	{
		_NODE_T *pTemp = new _NODE_T;
/*
		pTemp->pPrev = m_pCurr;
		pTemp->pNext = NULL;
		pTemp->pObj = pObj;
		m_pTail = pTemp;

		if (m_pCurr != m_pHead)
		{
			m_pCurr->pNext = pTemp;
		}

		m_pCurr = pTemp;
*/
		pTemp->pPrev = (NODE_T_ *)m_pCurr;
		pTemp->pNext = (NODE_T_ *)m_pTail;
		pTemp->pObj = pObj;


		
		m_pTail->pPrev = (NODE_T_ *)pTemp;
		m_pCurr->pNext = (NODE_T_ *)pTemp;

		m_pCurr = pTemp;
		
	}
}

template<class ClassType>
VOID TemplateList<ClassType>::Remove(ClassType *pObj)
{
	//ClassType *pTemp = NULL;
	_NODE_T *pCurr;
	
	if (pObj == NULL)
	{
		return;
	}

	for ( pCurr = m_pHead; pCurr != m_pTail; pCurr = (_NODE_T *)(pCurr->pNext))
	{
		if (pObj == pCurr->pObj)
		{
			//pTemp = pCurr->pNext->pObj;
			if (pCurr != m_pHead)
			{
				_NODE_T *pTemp;
				pTemp = (_NODE_T *)(pCurr->pPrev);
				pTemp->pNext = pCurr->pNext;
			}

			if (pCurr != m_pTail)
			{
				_NODE_T *pTemp;
				pTemp = (_NODE_T *)(pCurr->pNext);
				
				pTemp->pPrev = pCurr->pPrev;
			}
			
			delete pCurr;
			break;
		}
	}
}

template<class ClassType>
ClassType* TemplateList<ClassType>::Begin(VOID)
{
	return m_pHead->pObj;
}

template<class ClassType>
ClassType* TemplateList<ClassType>::End(VOID)
{
	return m_pTail->pObj;
}

template<class ClassType>
ClassType* TemplateList<ClassType>::Next(ClassType *pDesObj)
{
	ClassType *pTemp = NULL;
	_NODE_T *pCurr;
	_NODE_T *pTempNode;
	
	for ( pCurr = m_pHead; pCurr != m_pTail; pCurr = (_NODE_T *)(pCurr->pNext))
	{
		if (pDesObj == pCurr->pObj)
		{
			pTempNode = (_NODE_T *)(pCurr->pNext);
			pTemp = pTempNode->pObj;
			break;
		}
	}

	return pTemp;
}


template<class ClassType>
VOID TemplateList<ClassType>::Clear(VOID)
{
	m_pHead->pPrev = NULL;
	m_pHead->pObj = NULL;
	m_pHead->pNext = (NODE_T_ *)m_pTail;

	m_pCurr = m_pHead;

	m_pTail->pPrev = (NODE_T_ *)m_pHead;
	m_pTail->pObj = NULL;
	m_pTail->pNext = NULL;

}

template<class ClassType>
INT16 TemplateList<ClassType>::Find(ClassType *pObj)
{
	INT16 ret = FIND_OBJ_ERR;
	for (ClassType *pTimeObsrvr = Begin(); 
						pTimeObsrvr != End();
						pTimeObsrvr = Next(pTimeObsrvr))
	{
		if (pTimeObsrvr == pObj)
		{
			ret = FIND_OBJ_OK;
			break;
		}
	}	

	return ret;
}
#endif
