/***************************************************************************************
* 
* Filename: 		SysCanHal.cpp
* Contributor:
* Author: 		Wang Zhihua	
* Date: 			4/11/2008
* Copyright 		2008 ENPC Corporation, All Rights Reserved
*
* Description: 	This module definite HAL(hardware abstraction layer) between CAN controller and 
*			CAN protocol Layer.
*				HAL provide methods to receive CAN message and transmit CAN message. These 
*			is achieved by interrupt mode
*
* 
* Version			0
* Last rev by:		Wang Zhihua
* Last rev date:	4/11/2008
* Last rev desc:	
****************************************************************************************/

//---------------------------------------------------------------
//Include header


//--------------------------------------------------------------------------------
//Private Enumerated and macro Definitions
	
//-------------------------------------------------------------------------------
//Private Structure and Const variable defintion

typedef VOID (SysCanDataExchg:: *CAN_DATA_RECV_T)(VOID *DataPtr);


#define FRAG_MSG_QUEUE_SIZE 5
//TemplateQueue<CAN_APDU_T> FragXmitQueue(FRAG_MSG_QUEUE_SIZE);


//--------------------------------------------------------------------------------
//public variable
//SysCanApp SysCanAppObj;

//--------------------------------------------------------------------------------
//private  function

//--------------------------------------------------------------------------------
//function definiton



/********************************************************************************
SysCanInitial-----ϵͳCANӦ�ò��ʼ�������湹�캯��
                     	
Parameters:            
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanApp::SysCanInitial(VOID)
{
	//Э��������ʼ��

//#if (0)
//	m_pFragRecvOkCmd = new TemplateCmd<SysCanApp>(this, &SysCanApp::FragRecvOkHandle);
	TemplateCmd<SysCanApp> FragRecvOkCmd(this, &SysCanApp::FragRecvOkHandle);
//	if (NULL == m_pFragRecvOkCmd)
//	{
		//CAN_APP_TRACE("memory out\n");
		//__ASSERT__;
//	}

//	m_pFragXmitErrCmd = new TemplateCmd<SysCanApp>(this, &SysCanApp::FragXmitErrHandle);
	TemplateCmd<SysCanApp> FragXmitErrCmd(this, &SysCanApp::FragXmitErrHandle);
//	if (NULL == m_pFragXmitErrCmd)
//	{
		//CAN_APP_TRACE("memory out\n");
		//__ASSERT__;
//	}
//#endif
//	m_pFragXmitOkCmd = new TemplateCmd<SysCanApp>(this, &SysCanApp::FragXmitOkHandle);
	TemplateCmd<SysCanApp> FragXmitOkCmd(this, &SysCanApp::FragXmitOkHandle);
//	if (NULL == m_pFragXmitOkCmd)
//	{
		//CAN_APP_TRACE("memory out\n");
		//__ASSERT__;
//	}

	//����ע��
	//m_ProtocolObj.SetRqMsgErrHandleFcb(m_pRqMsgErrCmd);
	//m_ProtocolObj.SetFragRecvErrHandleFcb(m_pFragRecvErrCmd);
//ok
	ProtocolObj.SetFragRecvOkHandleFcb(&FragRecvOkCmd);
//ok
	ProtocolObj.SetFragXmitErrHandleFcb(&FragXmitErrCmd);
//ok
	ProtocolObj.SetFragXmitOkHandleFcb(&FragXmitOkCmd);

	m_SysCanFragXmitQueue.SysCanFragXmitQueueIni();

//	m_pFragXmitQueue = &FragXmitQueue;

}

/********************************************************************************
RecvMsgProc-----������Ϣ����
                     	
Parameters:            pApdu:���յ�������֡
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanApp::RecvMsgDispatch(IN CAN_APDU_T *pApdu)
{
	//CAN_APDU_T TempApdu;
	INT16 i16NewMsg = FALSE;
	INT16 ret;
	
	switch (pApdu->u16MsgClass)
	{
		case CAN_CFG_MSG_CLASS:
			i16NewMsg = DataExchgObj.CfgMsgRecvProc(pApdu);
			break;

		case CAN_CTRL_MSG_CLASS:
			i16NewMsg = DataExchgObj.CtrlMsgRecvProc(pApdu);
			break;

		case CAN_ALRM_MSG_CLASS:
			i16NewMsg = DataExchgObj.AlrmMsgRecvProc(pApdu);
			break;

		case CAN_STATE_MSG_CLASS:
			i16NewMsg = DataExchgObj.StateMsgRecvProc(pApdu);
			break;
		//ϵͳ�ܹ��ʣ�������·
		case CAN_ANALOG_MSG_CLASS:
			i16NewMsg = DataExchgObj.AnalogMsgRecvProc(pApdu);
			break;

		default:
			break;
	}

	//���µ���������
	if (TRUE == i16NewMsg)
	{
		if (FRAG_MSG == pApdu->u16Fragment)
		{
			ret = m_SysCanFragXmitQueue.InsQueue(*pApdu);
			
			if (QUEUE_FULL == ret)
			{
				//TBD
			}
			else
			{
				FragMsgPost();
			}
		}	

		else
		{
			ret = ProtocolObj.MsgXmitProc(pApdu);

			if (-1 == ret)
			{
				//TBD ���ϴ���
			}
		}
	}
	
}

#if (0)
/********************************************************************************
CfgMsgRecvProc-----����֡���մ���
                     	
Parameters:            pApdu:���յ�������֡
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanApp::CfgMsgRecvProc(CAN_APDU_T *pApdu)
{
	INT16 i;
	
	for (i = 0; i < CFG_MSG_OBJS_CNT; i++)
	{
		if (pApdu->u16ServiceCode == CanCfgMsgObjs[i].b6SrvcCode)
		{
			CanCfgMsgObjs[i].b1CfgOkFlag = TRUE;

			if (NULL != CanCfgMsgObjs[i].MsgRecvFcb)
			{
				(DataExchgObj.*CanCfgMsgObjs[i].MsgRecvFcb)(pApdu);
			}

			break;
		}
	}

#ifdef DEBUG_CAN
	//debug 
	if (i == CFG_MSG_OBJS_CNT)
	{
		//TBD
	}
#endif


	//�ж����������Ƿ����
	for (i = 0; i <	CFG_MSG_OBJS_CNT; i++)
	{
		if (CFG_REQUIRE == CanCfgMsgObjs[i].b4CfgType)
		{
			if (FALSE == CanCfgMsgObjs[i].b1CfgOkFlag)
			{
				break;
			}
		}
	}

	if ( CFG_MSG_OBJS_CNT == i)
	{
		m_CfgState.b1RequireOk = 1;
	}

}


/********************************************************************************
CtrlMsgRecvProc-----����֡���մ���
                     	
Parameters:            pApdu:���յ�������֡
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanApp::CtrlMsgRecvProc(CAN_APDU_T * pApdu)
{
	INT16 i;
	
	for (i = 0; i < CAN_CTRL_OBJS_CNT; i++)
	{
		if ((pApdu->u16ServiceCode == CanCtrlMsgObjs[i].b6SrvcCode)
			&& (pApdu->u16RsRq == RS_MSG))
			
		{
			CanCtrlMsgObjs[i].b1CfgOkFlag = TRUE;

			if (NULL != CanCtrlMsgObjs[i].MsgRecvFcb)
			{
				(DataExchgObj.*CanCtrlMsgObjs[i].MsgRecvFcb)(pApdu);
			}

			break;
		}
	}

#ifdef DEBUG_CAN
	//debug 
	if (i == CAN_CTRL_OBJS_CNT)
	{
		//TBD
	}
#endif

}


/********************************************************************************
AlrmMsgRecvProc-----�澯֡���մ���
                     	
Parameters:            pApdu:���յ�������֡
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanApp::AlrmMsgRecvProc(CAN_APDU_T * pApdu)
{
	INT16 i;
	
	for (i = 0; i < CAN_ALRM_OBJS_CNT; i++)
	{
		if (pApdu->u16ServiceCode == CanAlrmMsgObjs[i].b6SrvcCode)
		{
			CanAlrmMsgObjs[i].b1CfgOkFlag = TRUE;

			if (NULL != CanAlrmMsgObjs[i].MsgRecvFcb)
			{
				(m_pDataExchgObj->*CanAlrmMsgObjs[i].MsgRecvFcb)(pApdu);
			}

			break;
		}
	}

#ifdef DEBUG_CAN
	//debug 
	if (i == CAN_ALRM_OBJS_CNT)
	{
		//TBD
	}
#endif

}


/********************************************************************************
AlrmMsgRecvProc-----�澯֡���մ���
                     	
Parameters:            pApdu:���յ�������֡
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanApp::StateMsgRecvProc(CAN_APDU_T * pApdu)
{

	//TBD
}

/********************************************************************************
AlrmMsgRecvProc-----�澯֡���մ���
                     	
Parameters:            pApdu:���յ�������֡
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanApp::AnalogMsgRecvProc(CAN_APDU_T * pApdu)
{
	INT16 i;
	
	for (i = 0; i < CAN_ANLOG_OBJS_CNT; i++)
	{
		if (pApdu->u16ServiceCode == CanAnlogMsgObjs[i].b6SrvcCode)
		{
			CanAnlogMsgObjs[i].b1CfgOkFlag = TRUE;

			if (NULL != CanAnlogMsgObjs[i].MsgRecvFcb)
			{
				(m_pDataExchgObj->*CanAnlogMsgObjs[i].MsgRecvFcb)(pApdu);
			}

			break;
		}
	}

#ifdef DEBUG_CAN
	//debug 
	if (i == CAN_ANLOG_OBJS_CNT)
	{
		//TBD
	}
#endif

}
#endif


/********************************************************************************
CovMsgProc-----Cov��Ϣ����
                     	
Parameters:            
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanApp::CovMsgProc(VOID)
{
	CAN_APDU_T TempData;
	INT16 i16NewMsg, ret;
	UINT16 u16Count;
	UCHAR  i;

	//SysCanDataExchg pDataExchg = SysCanDataExchg::GetDataExchgPtr();

	// 0 ����������Ϣ�����ϵ��ʼģ������Ӽ�ػ�ȡ���ã����й����м�ص�������Ϣ
	i16NewMsg = DataExchgObj.ReqSetMsg(&TempData);

	if (i16NewMsg != NON_REQ_MSG)
	{
		ret = ProtocolObj.MsgXmitProc(&TempData);
	}
	
	// 1 ״̬COV
	for(i=0;i<MODULE_NUM;i++)
	{
		i16NewMsg = DataExchgObj.GetStateCovMsg(&TempData,i);

		if (i16NewMsg != NON_COV_MSG)
		{
			ret = ProtocolObj.MsgXmitProc(&TempData);

			if (0 == ret)
			{
				DataExchgObj.StateCovMsgXmitState(COV_XMIT_OK);
			}
			else
			{
				DataExchgObj.StateCovMsgXmitState(COV_XMIT_FAIL);
			}
		
		}
	
	
	// 2 �澯COV
		i16NewMsg = DataExchgObj.GetAlrmCovMsg(&TempData,i);

		if (i16NewMsg != NON_COV_MSG)
		{
				ret = ProtocolObj.MsgXmitProc(&TempData);

			if (0 == ret)
			{
				DataExchgObj.AlrmCovMsgXmitState(COV_XMIT_OK);
			}
			else
			{
				DataExchgObj.AlrmCovMsgXmitState(COV_XMIT_FAIL);
			}	
		}
	

		// 3 �¼���¼COV
		for (u16Count=0; u16Count<10; u16Count++)
		{
			i16NewMsg = DataExchgObj.GetEventLogMsg(&TempData,i);

			if (i16NewMsg != NON_COV_MSG)
			{
				ret = ProtocolObj.MsgXmitProc(&TempData);
			}

			//���¼�ֱ���˳�
			else
			{
				break;
			}
		}

	}

	// 4.�����ڲ���״̬COV
	i16NewMsg = DataExchgObj.GetParallelStateMsg(&TempData);

	if (i16NewMsg != NON_COV_MSG)
	{
		ret = ProtocolObj.MsgXmitProc(&TempData);
	}
		
	// 4 ���ϵ�COV???
		
}

/********************************************************************************
SyncMsgProc-----ͬ����Ϣ����
                     	
Parameters:            
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanApp::SyncMsgProc(VOID)
{
	CAN_APDU_T TempData;
	INT16 i16NewMsg, ret;
	UCHAR i;

	for(i=0;i<MODULE_NUM;i++)
	{
		// 1 ״̬��Ϣ����
		
		i16NewMsg = DataExchgObj.GetStateSyncMsg(&TempData,i);

		if (i16NewMsg != NON_SYNC_MSG)
		{
			ret = ProtocolObj.MsgXmitProc(&TempData);

			if (0 == ret)
			{
				DataExchgObj.StateSyncMsgXmitState(COV_XMIT_OK);
			}
			else
			{
				DataExchgObj.StateSyncMsgXmitState(COV_XMIT_FAIL);
			}
		
		}

		// 2 �澯��Ϣ����
		i16NewMsg = DataExchgObj.GetAlrmSyncMsg(&TempData,i);
	
		if (i16NewMsg != NON_SYNC_MSG)
		{
			ret = ProtocolObj.MsgXmitProc(&TempData);

			if (0 == ret)
			{
				DataExchgObj.AlrmSyncMsgXmitState(COV_XMIT_OK);
			}
			else
			{
				DataExchgObj.AlrmSyncMsgXmitState(COV_XMIT_FAIL);
			}
		
		}
	
		// 3 ģ������Ϣ���� 
		i16NewMsg = DataExchgObj.GetAnalogSyncMsg(&TempData,i);

		if (i16NewMsg != NON_SYNC_MSG)
		{
			ret = ProtocolObj.MsgXmitProc(&TempData);

			if (0 == ret)
			{
				DataExchgObj.AnalogSyncMsgXmitState(COV_XMIT_OK);
			}
			else
			{
				DataExchgObj.AnalogSyncMsgXmitState(COV_XMIT_FAIL);
			}
		}
	}

	// 4.����ģ�鹦����Ϣ�ȴ��ݸ���ģ������
	i16NewMsg = DataExchgObj.PeriodAnalogSyncMsg(&TempData);

	if (i16NewMsg != NON_SYNC_MSG)
	{
		ret = ProtocolObj.MsgXmitProc(&TempData);
	}
	
}

/********************************************************************************
FaultDataXimt-----������Ϣ����
                     	
Parameters:            
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanApp::FaultDataXimt(VOID)
{
	CAN_APDU_T tempCanApdu;
	INT16 i16NewMsg = FALSE;
	INT32 ret;
	
	i16NewMsg = DataExchgObj.GetFaultData(&tempCanApdu);

	//�й��ϼ�¼����,
	if (i16NewMsg)
	{
		ret = m_SysCanFragXmitQueue.InsQueue(tempCanApdu);  //������Ϣ���
			
		if (QUEUE_FULL == ret)
		{
			//TBD
		}
		else
		{
			FragMsgPost();  //��ȡ���ϵĶ������ݲ�����                    
		}
	}
	
}

/********************************************************************************
FragMsgPost-----�ֶ�֡����
                     	
Parameters:            
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanApp::FragMsgPost(VOID)
{
	CAN_APDU_T TempMsg;
	INT16 ret;
	
	TempMsg = m_SysCanFragXmitQueue.GetOldestNode();
	TempMsg.pDataPtr = m_FragMsgBuff;

	//���ֶ�֡�����͵�����һ���Է���TempMsg.pDataPtr
	DataExchgObj.FragMsgPacked(&TempMsg);

	ret = ProtocolObj.MsgXmitProc(&TempMsg);
	if (0 == ret)
	{
		m_SysCanFragXmitQueue.DelQueue();
	}
	
}

/********************************************************************************
MainRoutie-----�����̺���
ApplicationProtocolDataUnit                     	
Parameters:            
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanApp::MainRoutie(VOID)
{
	//���ݽ�������  ��״̬��澯���仯ʱ����CovFlag = TRUE�����������������
	DataExchgObj.InterfaceDataProc();
	
	// 1 ��������
	MsgRecvProc();
	
	// 2 COV msg process
	CovMsgProc();	

    //�����յ����Լ�ص�����ʱ��ִ����������ݴ���Ŀǰ���1S����һ��ͬ������
	// 3 Sync msg process
	SyncMsgProc();

	// 4.Fault data xmit ��·û�иù���
	FaultDataXimt();

}

/********************************************************************************
MsgRecvProc-----��Ϣ���մ���
                     	
Parameters:            
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
VOID SysCanApp::MsgRecvProc(VOID)
{
	#define MSG_RECV_CNT 5
	
	INT16 i, ret;
	CAN_APDU_T TempData;

	//ͬ��֡����������ά��
	DataExchgObj.MonitorSyncMsgProc();
	
	for (i = 0; i < MSG_RECV_CNT; i++)
	{
		ret = ProtocolObj.MsgRecvProc(&TempData);
		
		if (ret == 0)
		{
			break;
		}
		else
		{
			RecvMsgDispatch(&TempData);
		}
	}

}



INT16 SysCanApp::FragXmitOkHandle(IN VOID *pArg)
{
	DataExchgObj.FragXmitResult((CAN_APDU_T *)pArg, FRAG_XMIT_OK);

	//������һ֡
	FragMsgPost();
	
	//TBD
	return 0;
}

INT16 SysCanApp::FragXmitErrHandle(IN VOID * pArg)
{
	DataExchgObj.FragXmitResult((CAN_APDU_T *)pArg, FRAG_XMIT_FAIL);
	
	//TBD
	return 0;
}


INT16 SysCanApp::FragRecvOkHandle(IN VOID *pApduMsg)
{
	//Ŀǰ��û�зֶ���Ϣ,�����Ҫʱ������
	
	return 0;
}

